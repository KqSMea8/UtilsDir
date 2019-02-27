#include "stdafx.h"
#include "TimeGridDrow.h"
#include <stdio.h>
#include <conio.h>
#include <windowsx.h>
#include <Dialog_ZET/ConnectToZetPath.h>
#include <Dialog_ZET/ConnectToZetFile.h>
#include <Channels Filter/ChannelsFilter.h>

CTimeGridDrow::CTimeGridDrow()
	: m_FontListsNum		(m_FontListsEnd - m_FontListsStart + 1)
	, m_CountGraph			(0)
	, m_FontList			(0)
	, m_hPalette			(NULL)
	, m_FontNew				(NULL)
	, bUseOrtho2D			(false)
	, stop					(FALSE)
	, m_CurGraphCount		(NULL)
	, CurSelectedCursX		(0)
	, CurSelectedCursY		(0)
	, VertCursMoves			(false)
	, GorCursMoves			(false)
	, m_LeftXCoord			(NUMBER_SEC_IN_3000)
	, m_RightXCoord			(0)
	, m_VisibleLeftXCoord	(NUMBER_SEC_IN_3000)
	, m_VisibleRightXCoord	(0)
	, m_BottomYCoord		(DBL_MAX)
	, m_TopYCoord			(-DBL_MAX)
	, m_VisibleBottomYCoord	(DBL_MAX)
	, m_VisibleTopYCoord	(-DBL_MAX)
	, m_IntervalBottomYCoord(DBL_MAX)
	, m_IntervalTopYCoord	(-DBL_MAX)
	, m_bNeedLoadNewFont	(false)
	, m_SelectedGraphName	(_T(""))
{
}

CTimeGridDrow::~CTimeGridDrow()
{
	if (m_FontNew != nullptr)
	{
		DeleteFont(m_FontNew);
		m_FontNew = nullptr;
	}
	glDeleteLists(m_FontList, m_FontListsNum);        // �������� ���� 256 ������� ����������� 
}

// ��������� ������ ������ �� ��������
void CalculateNewStr(_Inout_ std::vector<CString> &DataVect, _In_ GLfloat Width, _In_ FreeTypeFont *pFont)
{
	CSize StrokaLen;
	int MaxLength(0), NewLength(0);

	for (auto &it : DataVect)
	{
		// ���� ����������� ������ ������
		StrokaLen = pFont->GetTextSize(it);
		if (StrokaLen.cx > Width)
		{
			CString TempName(it);
			while ((StrokaLen.cx > Width) && (StrokaLen.cx > 0))
			{
				TempName.Delete(TempName.GetLength() / 2, 1);
				StrokaLen = pFont->GetTextSize(TempName);
			}
			NewLength = TempName.GetLength();
		}
		else
			NewLength = max(MaxLength, it.GetLength());

		if (MaxLength < NewLength)
			MaxLength = NewLength;
	}

	ShortenChannelNames(DataVect, MaxLength);
}

void CTimeGridDrow::TextGL(GLfloat x, GLfloat y, LPCTSTR text, GLubyte R, GLubyte G, GLubyte B)
{
	glColor3ub(R, G, B);
	glRasterPos2f(x, y);
	glPrint(text);
}

void CTimeGridDrow::glPrint(LPCTSTR text)
{
	if (text == NULL)   			        // If There's No Text
		return;								// Do Nothing

	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	glListBase(m_FontList);								// Sets The Base Character to 32
#ifndef _UNICODE
	glCallLists((GLsizei)_tcslen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
#else
	glCallLists((GLsizei)_tcslen(text), GL_UNSIGNED_SHORT, text);	// Draws The Display List Text
#endif
	glPopAttrib();										// Pops The Display List Bits
}

float CTimeGridDrow::GetPixX(struct TrendGrid *grd, double x)
{
	float mnoj;
	// ���������� �������������� ������ � 1 �������
	mnoj = (float)((grd->width_x - 2) / grd->math_width_x);
	// � ������ ������� ��������� ������������ ���������� ��������
	return grd->left_x + mnoj * float(x - grd->math_min_x);
}

float CTimeGridDrow::GetPixY(struct TrendGrid *grd, double y)
{
	float mnoj;
	float shif;
	mnoj = float((2 - grd->height_y) / grd->math_width_y);//������� �� Y
	shif = float(grd->top_y + grd->height_y - 1. - mnoj * grd->math_min_y);//������� ���. ���� � �������� ����-���
	return shif + mnoj * (float)y;
}

double CTimeGridDrow::GetXFromCoord(struct TrendGrid *grd, long CoordX)
{
	double mnoj, shif;
	// ���������� �������� �� 1 �������
	mnoj = ((grd->width_x - 2) / grd->math_width_x);
	shif = grd->left_x + 1 - mnoj * (grd->math_min_x);
	return (((double)CoordX - shif) / mnoj);
}

double CTimeGridDrow::GetYFromCoord(struct TrendGrid *grd, long CoordY)
{
	double mnoj;
	// ���������� �������� �� 1 �������
	mnoj = (double)grd->math_width_y / grd->height_y;
	if (grd->bVerticalGraphs)
		// � ������������ �������� �� Y ���������� �������� � ����� ������� ���������� �� ���������� ��������
		return grd->math_min_y + double(CoordY - grd->bottom_y) * mnoj;
	else
		// � ������������ �������� �� Y ���������� �������� � ����� ������� ���������� �� ���������� ��������
		return grd->math_min_y + grd->math_width_y - double(CoordY - grd->top_y) * mnoj;
}

void CTimeGridDrow::CheckScaleX(struct TrendGrid *grd)
{
	//��������� � ����������� ���������� �� �	����� ��� �����
	int numvert = grd->width_x / WIDTH_COLUMN_IN_PIX;
	CZetTime last = grd->math_min_x + grd->math_width_x;				// �������� ����� �����������
	CZetTime first = grd->math_min_x;									// �������� ����� �����������
	CZetTimeSpan dt = last - first;
	double kolnanoseconds = dt;
	kolnanoseconds *= PRECISION_NANSEC;												// 3 ����� ����� �������
	double shvert = round((double)kolnanoseconds / numvert);

	if (shvert <= 0)													// ���� ��������� ������ ��� ����� �������� ���������� ����� ��������� �� ������������ ���������� �����
	{
		dt = 0.501 * numvert / PRECISION_NANSEC;
		last = first + dt;
		grd->math_width_x = last;
		grd->math_width_x -= grd->math_min_x;
	}

	if (grd->math_width_x > grd->x_end - grd->x_first)
		grd->math_width_x = grd->x_end - grd->x_first;
	if (grd->math_min_x < grd->x_first)
		grd->math_min_x = grd->x_first;
	if (grd->math_min_x + grd->math_width_x > grd->x_end)
		grd->math_min_x = grd->x_end - grd->math_width_x;
}

void CTimeGridDrow::CheckScaleY(struct TrendGrid *grd)
{
	if (grd->make_upor == 0)
	{
		stop = FALSE;
		if (grd->math_width_y <= MIN_WIDTH)
		{
			grd->math_width_y = MIN_WIDTH;
			stop = TRUE;
		}

		if (grd->math_width_y > 2.*FLT_MAX)
			grd->math_width_y = 2.*FLT_MAX;
		if (grd->math_min_y < (-1.)*FLT_MAX)
			grd->math_min_y = (-1.)*FLT_MAX;
		if (grd->math_min_y + grd->math_width_y > FLT_MAX)
			grd->math_min_y = FLT_MAX - grd->math_width_y;
	}
	else if (grd->make_upor == 1)
	{
		stop = FALSE;
		if (grd->math_width_y <= grd->upor_delta)
		{
			grd->math_width_y = grd->upor_delta;
			stop = TRUE;
		}
		if (grd->math_width_y > grd->upor_up - grd->upor_down)
			grd->math_width_y = grd->upor_up - grd->upor_down;
		if (grd->math_min_y < grd->upor_down)
			grd->math_min_y = grd->upor_down;
		if (grd->math_min_y + grd->math_width_y > grd->upor_up)
			grd->math_min_y = grd->upor_up - grd->math_width_y;
	}
}

#pragma region �������������
bool CTimeGridDrow::PrepareOGL(struct TrendGrid *grd, CDC* pdc, const CRect &rc)
{
	int w, h;
	int iPixFormat;
	CSize stroka;

	PIXELFORMATDESCRIPTOR pfd;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_COLORINDEX;
	pfd.cColorBits = 8;
	pfd.cRedBits = 8;
	pfd.cRedShift = 16;
	pfd.cGreenBits = 8;
	pfd.cGreenShift = 8;
	pfd.cBlueBits = 8;
	pfd.cBlueShift = 0;
	pfd.cAlphaBits = 0;
	pfd.cAlphaShift = 0;
	pfd.cAccumBits = 64;
	pfd.cAccumRedBits = 16;
	pfd.cAccumGreenBits = 16;
	pfd.cAccumBlueBits = 16;
	pfd.cAccumAlphaBits = 0;
	pfd.cDepthBits = 16;
	pfd.cStencilBits = 8;
	pfd.cAuxBuffers = 0;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.bReserved = 0;
	pfd.dwLayerMask = 0;
	pfd.dwVisibleMask = 0;
	pfd.dwDamageMask = 0;

	iPixFormat = ChoosePixelFormat(pdc->m_hDC, &pfd);
	//iPixFormat = EnumPixelFormats(pdc->m_hDC, 32, 1); // ����� �������������� ��� ���������

	bool InitError(true);

	if (!iPixFormat){
		ASSERT(FALSE);
		//AfxMessageBox(_T("Error - PrepareOGL - iPixFormat"),MB_OK|MB_ICONERROR);
		InitError = true;
		return InitError;
	}
	if (!SetPixelFormat(pdc->m_hDC, iPixFormat, &pfd)){
		ASSERT(FALSE);
		//AfxMessageBox(_T("Error - PrepareOGL - SetPixelFormat"),MB_OK|MB_ICONERROR);
		InitError = true;
		return InitError;
	}
	DescribePixelFormat(pdc->m_hDC, iPixFormat, sizeof(pfd), &pfd);
	if (pfd.dwFlags & PFD_NEED_PALETTE){
		//AfxMessageBox(_T("Need Palette"),MB_OK);
		SetupPalette();
	}
	m_hRC = wglCreateContext(pdc->m_hDC);
	if (m_hRC == NULL){
		ASSERT(FALSE);
		//AfxMessageBox(_T("Error - PrepareOGL - HRC"),MB_OK|MB_ICONERROR);
		InitError = true;
		return InitError;
	}
	w = rc.right - rc.left;
	h = rc.bottom - rc.top;

	wglMakeCurrent(pdc->m_hDC, m_hRC);
	BuildFontOGL(pdc->m_hDC, m_FontNew, grd->font_size);
	InicializeOrtho(rc);

	if (!bUseOrtho2D)
	{
		// ������ ������� ��������� 10 - ����� � ��� -10 - ������
		gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
		glEnable(GL_DEPTH_TEST);
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	stroka = pdc->GetTextExtent(_T("100"));
	grd->left_x = stroka.cx + 4;
	grd->top_y = stroka.cy + 4;
	grd->width_x = w - grd->left_x - grd->right_x;
	grd->height_y = h - grd->top_y - grd->bottom_y;
	grd->mouse_x = grd->left_x;
	grd->mouse_y = grd->bottom_y;
	GLenum err = glGetError();
	if (err != 0)	InitError = true;
	else InitError = false;
	glEnable(GL_NORMALIZE);
	glPolygonMode(GL_FRONT, GL_FILL);
	//wglMakeCurrent(pdc->m_hDC, NULL);

	// ������� ����� ������� ����� �������� ���
	CString WindowsDir;
	::ExpandEnvironmentStringsW(L"%WINDIR%", WindowsDir.GetBuffer(MAX_PATH * 10), MAX_PATH * 10);
	WindowsDir.ReleaseBuffer();
	const static CString FontPath(WindowsDir + L"\\Fonts\\segoeui.ttf");
	const static CString RobotoFontDir(CString(zpGetInstallLocation().c_str()) + L"template\\Fonts\\Roboto-Regular.ttf");
	CString FontDir(_T(""));
	if (zfFileExists(FontPath) == 1)
		FontDir = FontPath;
	else if (zfFileExists(RobotoFontDir) == 1)
		FontDir = RobotoFontDir;
	else
		FontDir.Empty();

	if (!FontDir.IsEmpty())
	{
		ListOfFonts FontList;
		FontList.push_back(std::make_pair(m_FontListsStart, m_FontListsNum));
		FontList.push_back(std::make_pair(0x2070, 0x2070 + 48));
		FontList.push_back(std::make_pair(0x2219, 0x2219 + 1));
		FontList.push_back(std::make_pair(0x2022, 0x2022 + 5));
		// ������� ����� ������� ����� �������� ���
		m_TextFont.InitFont(FontDir, grd->font_size, FontList);
	}
	else
	{
		const static CString ErrorText(TranslateString(L"���������� ��������� �����"));
		MessageBox(NULL, ErrorText, L"GridGL Error", MB_OK);
	}

	return InitError;
}

void CTimeGridDrow::InicializeOrtho(CRect Rect)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (bUseOrtho2D)
		gluOrtho2D(0.0f, (GLdouble)Rect.Width(), (GLdouble)Rect.Height(), 0.0f);
	else
		glOrtho((GLdouble)Rect.left, (GLdouble)Rect.right, (GLdouble)Rect.bottom, (GLdouble)Rect.top, -100, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

bool CTimeGridDrow::BuildFontOGL(HDC hDC, HFONT hFont, long FontSize)
{
	SelectObject(hDC, hFont);		       	        // Selects The Font We Want
	int listStart = m_FontListsStart;
	//���� �Ѩ !!!
	m_FontListsNum = max(max(L'�', L'�'), max(L'�', L'�')) + 5;

	if (0 == m_FontList)
		m_FontList = glGenLists(m_FontListsNum + 48);

	for (int retry = 0; retry < 10; retry++)
	{
		if (wglUseFontBitmaps(hDC, listStart, m_FontListsNum - 1, m_FontList) != FALSE)
		{
			if (wglUseFontBitmaps(hDC, 0x2070, 48, 0x2070 + 1) != FALSE)
				if (wglUseFontBitmaps(hDC, 0x2219, 1, 0x2219 + 1) != FALSE)
					break;
		}
		else
		{
			DWORD dw = GetLastError();
			int sdfs = 0;
		}

	}
	return false;
}

struct tagLogicalPalette
{
	WORD wVersion;
	WORD wNumEntries;
	PALETTEENTRY peaEntries[256];
};

void CTimeGridDrow::SetupPalette(void)
{

	int iColor;
	tagLogicalPalette pal = { 0x300, 256 };
	BYTE baRed[] = { 0, 36, 72, 109, 145, 182, 218, 255 };
	BYTE baGreen[] = { 0, 36, 72, 109, 145, 182, 218, 255 };
	BYTE baBlue[] = { 0, 85, 170, 255 };

	// Fill our logical palette structure with color data
	for (iColor = 0; iColor < 256; ++iColor) {
		pal.peaEntries[iColor].peRed = baRed[iColor & 0x07];
		pal.peaEntries[iColor].peGreen = baGreen[(iColor >> 0x03) & 0x07];
		pal.peaEntries[iColor].peBlue = baBlue[(iColor >> 0x06) & 0x03];
		pal.peaEntries[iColor].peFlags = 0;
	} // for
	// Build our palette
	if (m_hPalette != NULL)
	{
		delete m_hPalette;
		m_hPalette = nullptr;
	}
	m_hPalette = CreatePalette((LOGPALETTE*)&pal);
}
#pragma endregion

void CTimeGridDrow::CalculateControlSize(CDC* pdc, struct TrendGrid *grd, const CRect &rcBounds)
{
	//���������� �������� �����
	grd->width_x = rcBounds.right - rcBounds.left - grd->left_x - grd->right_x;
	if (grd->width_x < 80)
		grd->width_x = 80;

	grd->height_y = rcBounds.bottom - rcBounds.top - grd->top_y - grd->bottom_y;
	if (grd->height_y < 50)
		grd->height_y = 50;

	// ��������� ������ ���������� ��� ��������� ColorLine
	grd->ColorLine_Height = m_ColorLine.size() * COLORLINE_HEIGHT + INTERVAL_BETWENN_COLORLINES * m_ColorLine.size();

	//�������� �������������� ��������	
	if (grd->upor_delta <= 0.) grd->upor_delta = 1.e-10;
	if (grd->math_width_y <= 0.) grd->math_width_y = grd->upor_delta;
	if (grd->math_width_x <= 0.)
		grd->math_width_x = grd->upor_delta;
	if (grd->x_end - grd->x_first <= 0) grd->x_end = grd->x_first + grd->upor_delta;
	if (grd->upor_up - grd->upor_down <= 0) grd->upor_up = grd->upor_down + grd->upor_delta;

	m_VertDigits.CalcDigits(grd->math_min_y, grd->math_min_y + grd->math_width_y, grd->height_y);	
}

#pragma region ����� � �����������
int CTimeGridDrow::SetSetka(CDC* pdc, struct TrendGrid *grd, const CRect &rcBounds, bool m_bEnable, int CountGraphs)
// ���������� � ������� �����
{
	if (!pdc)
		return -1;

	CalculateControlSize(pdc, grd, rcBounds); 

	m_CountGraph = CountGraphs;

	if (m_bValidGraphs.empty())
		m_CurGraphCount = m_CountGraph;
	else
	{
		m_CurGraphCount = 0;
		for (auto it : m_bValidGraphs)
		{
			if (it)
				m_CurGraphCount++;
		}
	}

	if (IsEqual(grd->math_width_x, 0., 0.0001) || (IsEqual(grd->math_width_y, 0., 0.0001)))
		return -1;
	if (m_bEnable)
	{
		glColor3ub(GetRValue(grd->grid_color), GetGValue(grd->grid_color), GetBValue(grd->grid_color));
		if ((grd->type_abs & 4) == 0) //�������� �������������� �����
		{
			switch (grd->Display_Mode)
			{
				case DisplayMode::Events:
				case DisplayMode::Default:
					DefaultHorLines(grd);
					break;
				case DisplayMode::SplittingFields:
					if (m_CurGraphCount == 0)	// ���� ���� ��������� ����� ��� ����������� ������ ������� �����
						DefaultHorLines(grd);
					else if (m_bSplitFieldsNeedNameOnTop)
						SplitFieldsHorLines(pdc, grd);
					break;
				case DisplayMode::ImpositionGraths:
					ImpositionHorLines(pdc, grd);
					break;
				default:
					break;
			}				
		}
		if ((grd->type_abs & 1) == 0)//�������� ������������ �����
			VertLines(pdc, grd);
	}
	// �������� ��������������� ����������� ������
	glPointSize(1);
	COLORREF color;
	if (m_bEnable)
		color = grd->grid_color;
	else
		color = GetSysColor(COLOR_GRAYTEXT);
	glColor3ub(GetRValue(color), GetGValue(color), GetBValue(color));
	glLineWidth(2);
	glBegin(GL_LINE_LOOP);
	{
		glVertex2i((GLint)grd->left_x, (GLint)grd->top_y);
		glVertex2i((GLint)grd->left_x, (GLint)(grd->top_y + grd->height_y - 1));
		glVertex2i((GLint)(grd->left_x + grd->width_x - 3), (GLint)(grd->top_y + grd->height_y - 1));
		glVertex2i((GLint)(grd->left_x + grd->width_x - 3), (GLint)grd->top_y);
	}
	glEnd();
	glBegin(GL_POINTS);
	{
		if (grd->bVerticalGraphs)
		{
			glVertex2i((GLint)grd->left_x, (GLint)grd->top_y);
			glVertex2i((GLint)grd->left_x, (GLint)(grd->top_y + grd->height_y));
			glVertex2i((GLint)(grd->left_x + grd->width_x - 2), (GLint)(grd->top_y + grd->height_y));
			glVertex2i((GLint)(grd->left_x + grd->width_x - 2), (GLint)grd->top_y);
		}
		else
		{
			glVertex2i((GLint)grd->left_x - 1, (GLint)grd->top_y);
			glVertex2i((GLint)grd->left_x - 1, (GLint)(grd->top_y + grd->height_y));
			glVertex2i((GLint)(grd->left_x + grd->width_x - 3), (GLint)(grd->top_y + grd->height_y));
			glVertex2i((GLint)(grd->left_x + grd->width_x - 3), (GLint)grd->top_y);
		}
	}
	glEnd();
	if (grd->fon_color != GetSysColor(COLOR_3DFACE))
	{	// �������� ����� �� ������� ���������� ������ � ������� �������� �����
		glLineWidth(1);
		glColor3ub(GetRValue(0/*GetSysColor(COLOR_3DSHADOW)*/), GetGValue(0/*GetSysColor(COLOR_3DSHADOW)*/), GetBValue(0/*GetSysColor(COLOR_3DSHADOW)*/));
		glBegin(GL_LINES);
		{
			glVertex2i((GLint)rcBounds.left, (GLint)(rcBounds.top));
			glVertex2i((GLint)(rcBounds.right), (GLint)(rcBounds.top));
			glVertex2i((GLint)rcBounds.left, (GLint)rcBounds.top);
			glVertex2i((GLint)(rcBounds.left), (GLint)(rcBounds.bottom));
		}
		glEnd();
		glColor3ub(GetRValue(GetSysColor(COLOR_3DDKSHADOW)), GetGValue(GetSysColor(COLOR_3DDKSHADOW)), GetBValue(GetSysColor(COLOR_3DDKSHADOW)));
		glBegin(GL_LINES);
		{
			glVertex2i((GLint)(rcBounds.left + 1), (GLint)(rcBounds.top + 1));
			glVertex2i((GLint)(rcBounds.right - 2), (GLint)(rcBounds.top + 1));
			glVertex2i((GLint)(rcBounds.left + 1), (GLint)(rcBounds.top + 1));
			glVertex2i((GLint)(rcBounds.left + 1), (GLint)(rcBounds.bottom - 1));
		}
		glEnd();
		glColor3ub(GetRValue(GetSysColor(COLOR_3DHILIGHT)), GetGValue(GetSysColor(COLOR_3DHILIGHT)), GetBValue(GetSysColor(COLOR_3DHILIGHT)));
		glBegin(GL_LINES);
		{
			glVertex2i((GLint)(rcBounds.right), (GLint)(rcBounds.top));
			glVertex2i((GLint)(rcBounds.right), (GLint)rcBounds.bottom);
			glVertex2i((GLint)(rcBounds.right), (GLint)(rcBounds.bottom));
			glVertex2i((GLint)(rcBounds.left), (GLint)(rcBounds.bottom));
		}
		glEnd();
		glColor3ub(GetRValue(GetSysColor(COLOR_3DLIGHT)), GetGValue(GetSysColor(COLOR_3DLIGHT)), GetBValue(GetSysColor(COLOR_3DLIGHT)));
		glBegin(GL_LINES);
		{
			glVertex2i((GLint)(rcBounds.right - 1), (GLint)(rcBounds.top + 1));
			glVertex2i((GLint)(rcBounds.right - 1), (GLint)rcBounds.bottom - 1);
			glVertex2i((GLint)(rcBounds.right - 1), (GLint)(rcBounds.bottom - 1));
			glVertex2i((GLint)(rcBounds.left + 1), (GLint)(rcBounds.bottom - 1));
		}
		glEnd();
	}
	
	// ������������ ���������� �����
	DrowColorLine(pdc, grd, rcBounds);
	return 0;
}

long CTimeGridDrow::DefaultHorLines(struct TrendGrid* grd)
{
	if (!_finite(grd->math_width_x) || !_finite(grd->math_width_y))
		return -1;
	
	double pos;
	POINT pnt1, pnt2;
	glColor3ub(GetRValue(grd->grid_color), GetGValue(grd->grid_color), GetBValue(grd->grid_color));

	glLineWidth(1.0f);
	glBegin(GL_LINES); //�������������� ������ �����
	for (auto &ItDigits : m_VertDigits.m_IntermediateDigits)
	{
		pos = GetPixY(grd, ItDigits);

		pnt1.x = grd->left_x + 1;//����� ���� 
		pnt1.y = (long)(pos);
		pnt2.x = grd->left_x + grd->width_x - 2;
		pnt2.y = (long)(pos);//������ ����
		glVertex2i((GLint)pnt1.x, (GLint)pnt1.y);
		glVertex2i((GLint)pnt2.x, (GLint)pnt2.y);
	}
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES); //�������������� ������ �����
	for (auto &ItDigits : m_VertDigits.m_DigitsList)
	{
		pos = GetPixY(grd, ItDigits);

		pnt1.x = grd->left_x + 1;//����� ���� 
		pnt1.y = (long)(pos);
		pnt2.x = grd->left_x + grd->width_x - 2;
		pnt2.y = (long)(pos);//������ ����
		glVertex2i((GLint)pnt1.x, (GLint)pnt1.y);
		glVertex2i((GLint)pnt2.x, (GLint)pnt2.y);
	}
	glEnd();

	return 0;
}

void CTimeGridDrow::PaintIntermediateLines(struct TrendGrid* grd, float Top, float Bottom)
{
	MyPointsStruct pnt1, pnt2;		// ����� �� ������� ������ �����

	GLfloat Middle;
	if (Bottom - Top >= 100)
	{
		Middle = (Bottom - Top) / 2;
		pnt1.x = grd->left_x + 1;	//����� ���� 
		pnt1.y = Top + Middle;
		pnt2.x = grd->left_x + grd->width_x - 2;
		pnt2.y = Top + Middle;		// ������ ����
		glVertex3f((GLfloat)pnt1.x, pnt1.y, 0.f);
		glVertex3f((GLfloat)pnt2.x, pnt2.y, 0.f);

		PaintIntermediateLines(grd, Top, Top + Middle);
		PaintIntermediateLines(grd, Top + Middle, Bottom);
	}
}

long CTimeGridDrow::SplitFieldsHorLines(CDC* pdc, struct TrendGrid* grd)
{
	if (!_finite(grd->math_width_x) || !_finite(grd->math_width_y))
		return -1;

	if (m_CountGraph == 0)
		return -1;
	
	MyPointsStruct pnt1, pnt2;		// ����� �� ������� ������ �����

	glColor3ub(0, 0, 0);
	int CurIndex(0);
	unsigned CurrentField;
	CSize stroka;

	if (!grd->bVerticalGraphs)
	{
		// ������ ������ �������� ����� ��� ���������� �� �������
		glLineWidth(3.0f);
		glBegin(GL_LINES);	//�������������� ������� �����
		{
			//CurCoord = TopCoord;
			CurIndex = 0;
			for (unsigned Field = 0; Field < m_CountGraph; Field++)
			{
				CurrentField = Field;

				if (!m_bValidGraphs.empty() && !m_bValidGraphs[CurrentField])
					continue;

				if (m_FieldsName.size() >= CurrentField && !m_FieldsName.empty())
				{
					stroka = m_TextFont.GetTextSize(L" " + m_FieldsName[CurrentField] + L" ");


					pnt1.x = grd->left_x + 1;	//����� ���� 
					pnt1.y = m_SplitFieldsData[CurIndex].TopLineCoord;
					pnt2.x = grd->left_x + (grd->width_x - 2) / 4;
					pnt2.y = m_SplitFieldsData[CurIndex].TopLineCoord;			//������ ����
					glVertex3f((GLfloat)pnt1.x, pnt1.y, 0.f);
					glVertex3f((GLfloat)pnt2.x, pnt2.y, 0.f);

					pnt1.x = grd->left_x + (grd->width_x - 2) / 4 + stroka.cx;	//����� ���� 
					pnt1.y = m_SplitFieldsData[CurIndex].TopLineCoord;
					pnt2.x = grd->left_x + grd->width_x - 2;
					pnt2.y = m_SplitFieldsData[CurIndex].TopLineCoord;			//������ ����
					glVertex3f((GLfloat)pnt1.x, pnt1.y, 0.f);
					glVertex3f((GLfloat)pnt2.x, pnt2.y, 0.f);
					glEnd();

					m_TextFont.PrintTextGL(grd->left_x + GLfloat(grd->width_x - 2) / 4, grd->top_y + grd->height_y + grd->bottom_y - (m_SplitFieldsData[CurIndex].TopLineCoord + stroka.cy / 2),
										   GetRValue(grd->digits_color), GetGValue(grd->digits_color), GetBValue(grd->digits_color),
										   L" " + m_FieldsName[CurrentField]);
					
					glColor3ub(0, 0, 0);
					glBegin(GL_LINES);	//�������������� ������� �����
				}
				else
				{
					pnt1.x = grd->left_x + 1;	//����� ���� 
					pnt1.y = m_SplitFieldsData[CurIndex].TopLineCoord;
					pnt2.x = grd->left_x + grd->width_x - 2;
					pnt2.y = m_SplitFieldsData[CurIndex].TopLineCoord;			//������ ����
					glVertex3f((GLfloat)pnt1.x, pnt1.y, 0.f);
					glVertex3f((GLfloat)pnt2.x, pnt2.y, 0.f);
				}

				CurIndex++;
			}
		}
		glEnd();
	}

	glColor3ub(GetRValue(grd->grid_color), GetGValue(grd->grid_color), GetBValue(grd->grid_color));
	glLineWidth(3.0f);
	glBegin(GL_LINES); //�������������� ������ �����
	{
		// ������ ����� MAX
		CurIndex = 0;
		for (unsigned Field = 0; Field < m_CountGraph; Field++)
		{
			if (grd->bVerticalGraphs)	// ��������� � �������� �������
				CurrentField = m_CountGraph - Field - 1;
			else
				CurrentField = Field;

			if (!m_bValidGraphs.empty() && !m_bValidGraphs[CurrentField])
				continue;

			pnt1.x = grd->left_x + 1;	//����� ���� 
			pnt1.y = m_SplitFieldsData[CurIndex].PixYMax;
			pnt2.x = grd->left_x + grd->width_x - 2;
			pnt2.y = m_SplitFieldsData[CurIndex].PixYMax;			//������ ����
			glVertex3f((GLfloat)pnt1.x, pnt1.y, 0.f);
			glVertex3f((GLfloat)pnt2.x, pnt2.y, 0.f);

			CurIndex++;
		}

		// ������ ����� MIN
		CurIndex = 0;
		for (unsigned Field = 0; Field < m_CountGraph; Field++)
		{
			if (grd->bVerticalGraphs)	// ��������� � �������� �������
				CurrentField = m_CountGraph - Field - 1;
			else
				CurrentField = Field;

			if (!m_bValidGraphs.empty() && !m_bValidGraphs[CurrentField])
				continue;

			pnt1.x = grd->left_x + 1;	//����� ���� 
			pnt1.y = m_SplitFieldsData[CurIndex].PixYMin;
			pnt2.x = grd->left_x + grd->width_x - 2;
			pnt2.y = m_SplitFieldsData[CurIndex].PixYMin;			//������ ����
			glVertex3f((GLfloat)pnt1.x, pnt1.y, 0.f);
			glVertex3f((GLfloat)pnt2.x, pnt2.y, 0.f);

			CurIndex++;
		}
	}
	glEnd();

	// ������ ������������� �����
	glLineWidth(1.0f);
	glBegin(GL_LINES); //�������������� ������ �����
	{
		CurIndex = 0;
		for (unsigned Field = 0; Field < m_CountGraph; Field++)
		{
			if (grd->bVerticalGraphs)	// ��������� � �������� �������
				CurrentField = m_CountGraph - Field - 1;
			else
				CurrentField = Field;

			if (!m_bValidGraphs.empty() && !m_bValidGraphs[CurrentField])
				continue;

			PaintIntermediateLines(grd, m_SplitFieldsData[CurIndex].PixYMax, m_SplitFieldsData[CurIndex].PixYMin);
			CurIndex++;
		}
	}
	glEnd();

	return 0;
}

long CTimeGridDrow::ImpositionHorLines(CDC* pdc, struct TrendGrid* grd)
{
	if (!_finite(grd->math_width_x) || !_finite(grd->math_width_y))
		return -1;

	glColor3ub(GetRValue(grd->grid_color), GetGValue(grd->grid_color), GetBValue(grd->grid_color));

	// ����������� ����� 
	GLfloat CentralPoint(grd->top_y + (float)grd->height_y / 2);

	glLineWidth(5.0f);
	glBegin(GL_LINES); //�������������� ������� �����
	glVertex3f((GLfloat)grd->left_x + 1,				CentralPoint, 0.f);
	glVertex3f((GLfloat)grd->left_x + grd->width_x - 2, CentralPoint, 0.f);
	glEnd();

	glLineWidth(1.0f);
	glBegin(GL_LINES); //�������������� ������ ������������� �����
	PaintIntermediateLines(grd, (GLfloat)grd->top_y, CentralPoint);
	PaintIntermediateLines(grd, CentralPoint, (GLfloat)(grd->top_y + grd->height_y));
	glEnd();

	return 0;
}

void CTimeGridDrow::DrowVertLine(struct TrendGrid* grd, MyPointsStruct pnt1, MyPointsStruct pnt2, long posX)
{
	switch (grd->Display_Mode)
	{
		case DisplayMode::Events:
		case DisplayMode::Default:
		case DisplayMode::SeismicGrath:
		case DisplayMode::ImpositionGraths:
			glVertex2f((GLfloat)posX, (GLfloat)pnt1.y);
			glVertex2f((GLfloat)posX, (GLfloat)pnt2.y);
			break;
		case DisplayMode::SplittingFields:
		{
			if (m_CurGraphCount == 0 || !m_bSplitFieldsNeedNameOnTop)
			{
				glVertex2f((GLfloat)posX, (GLfloat)pnt1.y);
				glVertex2f((GLfloat)posX, (GLfloat)pnt2.y);
				break;
			}

			GLfloat CurCoord = (GLfloat)grd->top_y + 1 + SPLIT_FIELDS_DELTA;
			GLfloat HeightVert = GLfloat(grd->height_y - SPLIT_FIELDS_DELTA) / m_CurGraphCount;	// ������������� �������

			for (unsigned Field = 0; Field < m_CountGraph; Field++)
			{
				if (!m_bValidGraphs.empty() && !m_bValidGraphs[Field])
					continue;

				pnt1.y = CurCoord + SPLIT_FIELDS_DELTA;
				pnt2.y = CurCoord + HeightVert - SPLIT_FIELDS_DELTA;			//������ ����
				glVertex2f((GLfloat)posX, (GLfloat)pnt1.y);
				glVertex2f((GLfloat)posX, (GLfloat)pnt2.y);

				CurCoord += HeightVert;
			}
			break;
		}
		default:
			break;
	}
}

long CTimeGridDrow::VertLines(CDC* pdc, struct TrendGrid* grd)
{
	if (!_finite(grd->math_width_x) || !_finite(grd->math_width_x))
		return -1;

	MyPointsStruct pnt1, pnt2;
	long posX;
	double ShagX = 0;							// ��� � �������� ��� ��������� �����
	double PosFirstX = 0, PosSecondX = 0;
	bool flag(false);							// ���� ���������� �� ������������� �����
	
	CZetTime last = grd->math_min_x + grd->math_width_x;
	CZetTime first = grd->math_min_x;
	GridLayoutX Layout(first, last, grd->width_x);

	glColor3ub(GetRValue(grd->grid_color), GetGValue(grd->grid_color), GetBValue(grd->grid_color));
	glLineWidth(1.f); // �������� ������� ������������ ����� �������
	glBegin(GL_LINES);
		
	for (int h = 0; h < (int)Layout.VectorLayout.size(); h++)
	{
		double PosX = Layout.VectorLayout[h];

		if ((h == 0) && (Layout.VectorLayout.size() > 1))
		{
			// ��������� ������������� �������������� ��������� 
			PosFirstX = Layout.VectorLayout[0];
			PosFirstX = GetPixX(grd, PosFirstX);

			PosSecondX = Layout.VectorLayout[1];
			PosSecondX = GetPixX(grd, PosSecondX);
			if (PosSecondX - PosFirstX >= MAX_PIX_SIZE)
			{
				flag = true;
				ShagX = (PosSecondX - PosFirstX) / 2;
			}
		}

		if (GetPixX(grd, PosX) >= grd->left_x)
		{
			if (GetPixX(grd, PosX) >= grd->left_x + grd->width_x)
				break;

			pnt1.y = GLfloat(grd->top_y + grd->height_y);
			pnt2.y = GLfloat(grd->top_y);

			// ������ ����� � ������� ������ ����� ������������� �����
			if ((flag && h == 0) ||
				// ������ ������������� ����� ������ ��������� �����
				(flag && (h == Layout.VectorLayout.size() - 1)) ||
				// ���� �� ������ � �� ��������� ����� � ���������� ���������� �� ������������� ����� ������ �� ������
				(flag))
			{				
				posX = (long)(GetPixX(grd, PosX) + ShagX);

				if (posX <= grd->left_x + grd->width_x)
					DrowVertLine(grd, pnt1, pnt2, posX);				
			}

			if (flag && h == 0)
			{
				posX = (long)(GetPixX(grd, PosX) - ShagX);

				if (posX >= grd->left_x)
					DrowVertLine(grd, pnt1, pnt2, posX);
			}

			glLineWidth(1); // �������� ������� ������������ ����� �������
			posX = (long)GetPixX(grd, PosX);

			DrowVertLine(grd, pnt1, pnt2, posX);
		}
	}
	glEnd();

	CursorCoords.CalcGrid(first, last, grd->width_x, SHAG_CURSOR_INPIX);
	
	return 0;
}
#pragma endregion

#pragma region ���������� ����
_Check_return_ int CTimeGridDrow::SetSetkaDigits(CDC* pdc, struct TrendGrid *grd, const CRect &rcBounds)
// ���������� � ������� �������� �����
{
	if (!pdc)
		return -1;
	if ((grd->math_width_x == 0.) || (grd->math_width_y == 0))
		return -1;
	CSize stroka;
	int err = 0;
	long mynan = 0x7f800000;
	glColor3ub(GetRValue(grd->grid_color), GetGValue(grd->grid_color), GetBValue(grd->grid_color));
	bool bNeedInvalidate(false);
	if ((grd->type_abs & 8) == 0)
	{
		switch (grd->Display_Mode)
		{
			case DisplayMode::Events:
				break;
			case DisplayMode::Default:
				if (VertDigits(pdc, grd, rcBounds) == NEED_INVALIDATE)				//���������� ����. ���
					bNeedInvalidate = true;
				break;
			case DisplayMode::SplittingFields:
				if (m_bSplitFieldsNeedNameOnTop)
				{
					if (SplitFieldsVertDigits(pdc, grd, rcBounds) == NEED_INVALIDATE)	//���������� ����. ���
						bNeedInvalidate = true;
				}
				else
				{
					if (VertDigitsSeismo(pdc, grd, rcBounds) == NEED_INVALIDATE)		//���������� ����. ���
						bNeedInvalidate = true;
				}
				break;
			case DisplayMode::SeismicGrath:
				if (VertDigitsSeismo(pdc, grd, rcBounds) == NEED_INVALIDATE)
					bNeedInvalidate = true;
				break;
			case DisplayMode::ImpositionGraths:
				if (grd->left_x != 16)
				{
					grd->left_x = 16; 
					bNeedInvalidate = true;
				}
				break;
			default:
				break;
		}
	}
	if ((grd->type_abs & 16) == 0)//
		if (HorDigits(pdc, grd) == NEED_INVALIDATE)			//���������� �������������� ���	
			bNeedInvalidate = true;

	if (bNeedInvalidate)
		return NEED_INVALIDATE;
	else
		return 0;
}

CSize GetStrSize(_In_ CDC* pdc, _In_ CString &Str1, _In_ CString &Str2)
{
	CSize stroka1, stroka2;

	stroka1 = pdc->GetTextExtent(Str1);
	stroka2 = pdc->GetTextExtent(Str2);

	return CSize(max(stroka1.cx, stroka2.cx), stroka1.cy + stroka2.cy);
}

long CTimeGridDrow::HorDigits(CDC* pdc, struct TrendGrid* grd)
{
	if (!_finite(grd->math_width_x) || !_finite(grd->math_width_y))
		return -1;
	int vert, vert2, sdvig;
	CSize stroka;
	
	GridLayoutX Layout(grd->math_min_x, grd->math_min_x + grd->math_width_x, grd->width_x);

	CString str1, str2;
	CString sData1;

	long max_cy = 0;
	for (auto &ItDigit : Layout.DateString)
	{
		sData1 = ItDigit;
		if (Layout.del == 0)
		{
			static const CString temp(TranslateString(L"�."));
			sData1 += L" " + temp; // ���������� ���
		}

		if (sData1.Find(L"_") == -1)
		{
			str1 = sData1;
			str2.Empty();
		}
		else
		{
			str1 = sData1.Mid(0, sData1.Find(L"_"));
			str2 = sData1.Mid(sData1.Find(L"_") + 1, sData1.GetLength());
		}

		stroka = GetStrSize(pdc, str1, str2);

		if (grd->bVerticalGraphs)
		{
			if (max_cy < stroka.cx)
				max_cy = stroka.cx;
		}
		else
		{
			if (max_cy < stroka.cy)
				max_cy = stroka.cy;
		}
	}
	
	max_cy += 9;

	if (max_cy < 17)
		max_cy = 17;

	max_cy += grd->ColorLine_Height;

	if (grd->bottom_y != max_cy)
	{
		grd->bottom_y = max_cy;
		return NEED_INVALIDATE;
	}

	for (int i = 0, Size = Layout.VectorLayout.size(); i < Size; i++)
	{
		sData1 = Layout.DateString[i];
		if (Layout.del == 0)
		{
			static const CString temp(TranslateString(L"�."));
			sData1 += L" " + temp; // ���������� ���
		}

		if (sData1.Find(L"_") == -1)
		{
			str1 = sData1;
			str2.Empty();
		}
		else
		{
			str1 = sData1.Mid(0, sData1.Find(L"_"));
			str2 = sData1.Mid(sData1.Find(L"_") + 1, sData1.GetLength());
		}

		stroka = GetStrSize(pdc, str1, str2);
		if (grd->bVerticalGraphs)
			sdvig = (long)(GetPixX(grd, Layout.VectorLayout[i]));
		else
			sdvig = (long)(GetPixX(grd, Layout.VectorLayout[i]));

		
		if ((sdvig > grd->left_x) && (sdvig < (grd->left_x + grd->width_x)))
		{
			stroka = pdc->GetTextExtent(str1);

			vert = grd->top_y + grd->height_y + (grd->bVerticalGraphs ? stroka.cx : stroka.cy) + 2;

			GLfloat SdvigCur = sdvig - GLfloat(grd->bVerticalGraphs ? 0 : stroka.cx / 2);
			if (grd->bVerticalGraphs && str2.IsEmpty())
				SdvigCur += stroka.cy / 2;
			
			TextGL(SdvigCur, (GLfloat)vert, str1,
				   GetRValue(grd->digits_color), GetGValue(grd->digits_color), GetBValue(grd->digits_color));

			if (!str2.IsEmpty())
			{
				stroka = pdc->GetTextExtent(str2);
				if (grd->bVerticalGraphs)
					vert2 = grd->top_y + grd->height_y + (grd->bVerticalGraphs ? stroka.cx : stroka.cy) + 2;
				else
					vert2 = vert + stroka.cy + 4;
				
				TextGL((GLfloat)sdvig + (grd->bVerticalGraphs ? stroka.cy : -stroka.cx / 2), (GLfloat)vert2, str2,
					   GetRValue(grd->digits_color), GetGValue(grd->digits_color), GetBValue(grd->digits_color));
			}
		}
	}
	return 0;
}

long CTimeGridDrow::VertDigits(CDC* pdc, struct TrendGrid* grd, const CRect &rcBounds)
{
	if (!_finite(grd->math_width_x) || !_finite(grd->math_width_y))
		return -1;

	CSize stroka, str_text_y(0, 0);
	CString Unit(GetCurrentUnitStr());	// ������� ��������� 
	
	if (!Unit.IsEmpty())
		str_text_y = m_TextFont.GetTextSize(Unit);

	long max_cx = 0;
	float pos;
	CString str;
	
	if (grd->bVerticalGraphs)
	{
		// �������� ��� �� ������ ����� � ������ ������� ��� �����
		stroka = pdc->GetTextExtent(L"0");
		if (max_cx < stroka.cy + str_text_y.cy)
			max_cx = stroka.cy + str_text_y.cy;
	}
	else
	{
		for (auto &itDigit : m_VertDigits.m_DigitsList)
		{
			str.Format(m_VertDigits.m_Format, itDigit);
			stroka = pdc->GetTextExtent(str);

			if (max_cx < stroka.cx + str_text_y.cy)
				max_cx = stroka.cx + str_text_y.cy;
		}
	}

	if (grd->bVerticalGraphs)
	{
		max_cx += 9;
		if (max_cx < 15)
			max_cx = 15;

		stroka = m_TextFont.GetTextSize(m_SelectedGraphName);
		stroka.cy += 4;
		if (grd->gor_CursValid)
			stroka.cy += 2 * CURSOR_SIZE;

		if (stroka.cy < 15)
			stroka.cy = 15;

		if ((grd->right_x != max_cx) || (grd->left_x != stroka.cy))
		{
			grd->right_x = max_cx;
			grd->left_x = stroka.cy;
			return NEED_INVALIDATE;
		}
// 		if (grd->right_x < 30)
// 			grd->right_x = 30;
	}
	else
	{
		max_cx += 9;
		if (max_cx < 30)
			max_cx = 30;

		if ((grd->left_x != max_cx) || (grd->right_x != 15))
		{
			grd->left_x = max_cx;
			grd->right_x = 15;
			return NEED_INVALIDATE;
		}
	}

// 	grd->width_x = rcBounds.right - rcBounds.left - grd->left_x - grd->right_x;
// 	if (grd->fon_color != GetSysColor(COLOR_3DFACE))
// 		grd->width_x -= 2;

	for (auto &itDigit : m_VertDigits.m_DigitsList)
	{
		str.Format(m_VertDigits.m_Format, itDigit);
		stroka = pdc->GetTextExtent(str);

		pos = GetPixY(grd, itDigit);
		if (grd->bVerticalGraphs)
		{
			// ��������� ����� ����� � �������� ��������
			if (pos - stroka.cx / 2 < grd->top_y)
				break;
			if (pos + stroka.cx / 2 <= grd->top_y + grd->height_y)
				TextGL((GLfloat)grd->left_x + grd->width_x + stroka.cy - 3, (GLfloat)(pos + stroka.cx / 2), str,
					   GetRValue(grd->digits_color), GetGValue(grd->digits_color), GetBValue(grd->digits_color));
		}
		else
		{
			if (pos - stroka.cy / 2 < grd->top_y)
				break;
			if (pos + stroka.cy / 2 <= grd->top_y + grd->height_y)
				TextGL(GLfloat(grd->left_x - stroka.cx - 4), (GLfloat)(pos + stroka.cy / 2), str,
				GetRValue(grd->digits_color), GetGValue(grd->digits_color), GetBValue(grd->digits_color));
		}
	}
	
	// ����� ������� ��������� �� �������
	if (!Unit.IsEmpty())
	{
		if (grd->bVerticalGraphs)
		{
			glPushMatrix();
			glLoadIdentity();
			m_TextFont.PrintTextGL(GLfloat(grd->bottom_y + grd->height_y / 2 - str_text_y.cx / 2), GLfloat(4/*grd->right_x + grd->width_x + grd->left_x - str_text_y.cy*/),
								   GetRValue(grd->digits_color), GetGValue(grd->digits_color), GetBValue(grd->digits_color), Unit);
			glPopMatrix();
		}
		else
		{
			glPushMatrix();
			glLoadIdentity();
			glRotatef(90, 0, 0, 1);
			m_TextFont.PrintTextGL(GLfloat(str_text_y.cy + 2), GLfloat(grd->bottom_y + grd->height_y / 2 - str_text_y.cx / 2),
								   GetRValue(grd->digits_color), GetGValue(grd->digits_color), GetBValue(grd->digits_color), Unit);
			glPopMatrix();
		}
	}
	
	return 0;
}

struct GraphNameStruct
{
	CString Name;		// ��� �������
	float TopCoord;		// ���������� � �������� �  ������� ����������    �������� ����� �������
	float BottomCoord;	// ���������� � �������� �� ������� ������������� �������� ����� �������

	float TextTopCoord;		// ���������� � �������� �  ������� ����������    �������� ����� �������
	float TextBottomCoord;	// ���������� � �������� �� ������� ������������� �������� ����� �������

	bool bUnited;		// ���� ����, ��� ��� ��� �������� ��������� ������	
	bool bNeedDelete;	// ���� ��� ����� ������� ��� ���������

	enum UniteType
	{
		PREV_ELEMENT = 0,
		NEXT_ELEMENT
	};
	int CountLetters;
	UniteType StructUniteType;

	FreeTypeFont *pdc;
	GraphNameStruct()
		: CountLetters(0)
		, TopCoord(0.f)
		, BottomCoord(0.f)
		, TextTopCoord(0.f)
		, TextBottomCoord(0.f)
		, bUnited(false)
		, bNeedDelete(false)
	{}
	void CalculateTextPos()
	{
		CSize Stroka = pdc->GetTextSize(Name);
		TextTopCoord = TopCoord + (BottomCoord - TopCoord - 5) / 2 - (float)Stroka.cy / 2;
		TextBottomCoord = TextTopCoord + Stroka.cy;
	}

	GraphNameStruct(_In_ FreeTypeFont* _pdc, _In_ CString _Name, GLfloat StartPix, _In_ GLfloat EndPix)
		: Name			(_Name)
		, CountLetters	(0)
		, TopCoord		(StartPix)
		, BottomCoord	(EndPix)
		, bUnited		(false)
		, bNeedDelete	(false)
	{
		pdc = _pdc;
		CalculateTextPos();
	}

	void Unite(_In_ GraphNameStruct &_NewVal)
	{
		//if (StructUniteType == PREV_ELEMENT)
			//TopCoord	= _NewVal.TopCoord;
		//else
			BottomCoord = _NewVal.BottomCoord;

		Name = Name.Left(CountLetters);
		bUnited = true;

		CalculateTextPos();
	}	
	// ��������� ������������� �� ��� �� ��������� ���������
	bool IsSuperimposed(_In_ GraphNameStruct &Val)
	{
		return TextBottomCoord > Val.TextTopCoord;
	}
	// ���������� ���������� ��������� ���� � �����
	int GetNumberOfMatchingLetters(_In_ GraphNameStruct &Val)
	{
		unsigned Letter(0);	// ���������� ���������� �����
		for (size_t CountLetters = min(Name.GetLength(), Val.Name.GetLength()); Letter < CountLetters; Letter++)
		{
			if (Name.Left(Letter) != Val.Name.Left(Letter))
				break;
		}
		Letter--;

		// ��������� ���������� ������ �� ������� ������������ ������
		while (Letter > 0)
		{
			int Character = Name[Letter - 1];
			if (Character >= 0 && Character <= 256)
			{
				if ((isalnum(Character)) != 0)
					break;
				else
					Letter--;
			}
			else
				break;
		}

		return Letter;
	}
};

CString CalculateName(_In_ FreeTypeFont *pdc, _In_ CString Name, _In_ float MaxWidth)
{
	for (;;)
	{
		if (Name.IsEmpty() || (pdc->GetTextSize(Name).cx <= MaxWidth))
			break;
		else
			Name = Name.Left(Name.GetLength() - 1);// �������� ���� ������ � �����
	}
	return Name;
}

long CTimeGridDrow::VertDigitsSeismo(CDC* pdc, struct TrendGrid* grd, const CRect &rcBounds)
{
	if (!_finite(grd->math_width_x) || !_finite(grd->math_width_y))
		return -1;
	
	// ������ ��� ������� �����
	int LeftDelta(16);
	CSize Stroka;

	if (!m_FieldsName.empty())
	{
		CString Name;

		GLfloat CurHeight((GLfloat)grd->top_y);
		GLfloat HeightOnOneName = ((GLfloat)grd->height_y - 1) / m_CurGraphCount;	// ���������� ��������� ��� ��������� ������� �������

		if (grd->bVerticalGraphs)
		{
			// ������ ���� ���� ��������
			std::vector<CString> NamesVect(m_CurGraphCount);
			
			UINT CurrentIndex(0);
			for (unsigned Field = 0, FieldNamesCount = m_FieldsName.size(); (Field < FieldNamesCount) && (Field < m_CountGraph); Field++)
			{
				if (!m_bValidGraphs[Field])
					continue;

				Stroka = m_TextFont.GetTextSize(m_FieldsName[Field]);
				if (LeftDelta < Stroka.cy + 4)
					LeftDelta = Stroka.cy + 4;
				NamesVect[CurrentIndex] = m_FieldsName[Field];
				CurrentIndex++;
			}
			// ��������� ����� �������� �� �������� ������
			CalculateNewStr(NamesVect, HeightOnOneName - 5, &m_TextFont);

			glPushMatrix();
			glLoadIdentity();
			CurHeight = grd->bottom_y + HeightOnOneName / 2;
			for (auto &GraphName : NamesVect)
			{
				Stroka = m_TextFont.GetTextSize(GraphName);

				m_TextFont.PrintTextGL(CurHeight - (GLfloat)Stroka.cx / 2, (GLfloat)(grd->right_x + grd->width_x + 2),
									   GetRValue(grd->digits_color), GetGValue(grd->digits_color), GetBValue(grd->digits_color),
									   GraphName);
				CurHeight += HeightOnOneName;
			}
			glPopMatrix();
		}
		else
		{
			// ������ ������������ ������ ������
			float Max_Width = float(grd->left_x + grd->width_x + grd->right_x)/*(float)rcBounds.Width()*/ / 4;
			if (Max_Width > 200)
				Max_Width = 200;

			std::vector<GraphNameStruct> NamesVector;
			for (unsigned Field = 0, FieldNamesCount = m_FieldsName.size(); (Field < FieldNamesCount) && (Field < m_CountGraph); Field++)
			{
				if (!m_bValidGraphs[Field])
					continue;

				Name = CalculateName(&m_TextFont, m_FieldsName[Field], Max_Width);
				CurHeight += HeightOnOneName;
				if (Name.IsEmpty())
					continue;

				if (NamesVector.empty())
				{
					NamesVector.push_back({ &m_TextFont, Name, (float)grd->top_y, CurHeight });
				}
				else
				{
					if (NamesVector.back().Name != Name)
						NamesVector.push_back({ &m_TextFont, Name, NamesVector.back().BottomCoord, CurHeight });
					else
					{
						// ���� ��������� ��� ������� � ������ �������� ����, ������ ��������� ������
						NamesVector.back().BottomCoord = CurHeight;
						NamesVector.back().bUnited = true;
						NamesVector.back().CalculateTextPos();
					}
				}
			}

			// ��������� �����
			bool bNamesOk = false;
			while (!bNamesOk)
			{
				bNamesOk = true;
			
				for (int Index = 0, VectorSize = (int)NamesVector.size(); Index < VectorSize - 1; Index++)
				{
					if (NamesVector[Index].bNeedDelete)
						continue;

					unsigned NextIndex = Index + 1;
					for (; NextIndex < (unsigned)VectorSize; NextIndex++)
					{
						if (NamesVector[NextIndex].bNeedDelete)
							continue;
						else
							break;
					}
					if (NextIndex == VectorSize)
						break;;
								
					// ���� ������� ������� �������� �� ���������
					if (NamesVector[Index].IsSuperimposed(NamesVector[NextIndex]))
					{
						// ���������� ����� �������� � ������� ������
						int CountMatchedLetters		= NamesVector[Index]  .GetNumberOfMatchingLetters(NamesVector[NextIndex]);
						// ���������� ����� �������� �� ��������� ������
						int CountMatchedLettersNext = NextIndex == VectorSize - 1 ? 0 :
													NamesVector[NextIndex].GetNumberOfMatchingLetters(NamesVector[NextIndex + 1]);

						bool bNextElementUnited(false);	// ���� ���� ��� ��������� ������� �������� � ���-����

						// ��������� ����� �� ������������ ��������� ������� � ���-����
						if (CountMatchedLetters > 0 || CountMatchedLettersNext > 0)
						{
							// ���� � ���������� ���������� ������ ��� ������� �� ������� � � �������, �������� ����������
							if (CountMatchedLetters >= CountMatchedLettersNext)
							{
								unsigned DeleteIndex = NextIndex;
								// �������� �� ���� ��������� ��������� ���� �� ����� ����������
								for (; DeleteIndex < NamesVector.size(); DeleteIndex++)
								{
									if (NamesVector[DeleteIndex].bNeedDelete)
										continue;

									// ���������� ����� �������� �� ��������� ������
									CountMatchedLettersNext = DeleteIndex == VectorSize - 1 ? 0 :
										NamesVector[DeleteIndex].GetNumberOfMatchingLetters(NamesVector[DeleteIndex + 1]);

									// ���� ���������� ��������� �������� � ���������� ��������� ������ ��� �� ���������
									if (CountMatchedLetters >= CountMatchedLettersNext)
										NamesVector[DeleteIndex].bNeedDelete = true;
									else
										break;

									if (DeleteIndex == VectorSize - 1)
									{
										DeleteIndex++;
										break;
									}

									if (CountMatchedLetters != CountMatchedLettersNext)
										break;
								}
								DeleteIndex--;

								bNextElementUnited = true;

								NamesVector[Index].CountLetters = CountMatchedLetters;
								NamesVector[Index].StructUniteType = GraphNameStruct::NEXT_ELEMENT;

								// ������ ��������� �������� �����						
								NamesVector[Index].Unite(NamesVector[DeleteIndex]);
								Index = DeleteIndex;
							}
							else
							{
								CountMatchedLetters = CountMatchedLettersNext;

								unsigned DeleteIndex = NextIndex + 1;
								// �������� �� ���� ��������� ��������� ���� �� ����� ����������
								for (; DeleteIndex < (unsigned)VectorSize; DeleteIndex++)
								{
									if (NamesVector[DeleteIndex].bNeedDelete)
										continue;

									// ���������� ����� �������� �� ��������� ������
									CountMatchedLettersNext = DeleteIndex == VectorSize - 1 ? 0 :
										NamesVector[DeleteIndex].GetNumberOfMatchingLetters(NamesVector[DeleteIndex + 1]);

									// ���� ���������� ��������� �������� � ���������� ��������� ������ ��� �� ���������
									if (CountMatchedLetters >= CountMatchedLettersNext)
									{
										NamesVector[DeleteIndex].bNeedDelete = true;
									}
									else
										break;

									if (DeleteIndex == VectorSize - 1)
									{
										DeleteIndex++;
										break;
									}

									if (CountMatchedLetters != CountMatchedLettersNext)
										break;
								}
								DeleteIndex--;

								bNextElementUnited = true;

								NamesVector[NextIndex].CountLetters = CountMatchedLetters;
								NamesVector[NextIndex].StructUniteType = GraphNameStruct::NEXT_ELEMENT;

								// ������ ��������� �������� �����						
								NamesVector[NextIndex].Unite(NamesVector[DeleteIndex]);
								Index--;
							}
						}

						if (!bNextElementUnited)
						{
							// ������� ������� � �������� ��� ��� �� ����� ��� �������� ��������

							// TO DO �������� �������� ��� ��������� ������� ���������
							NamesVector[NextIndex].bNeedDelete = true;
							Index--;
						}
						bNamesOk = false;
					}
				}
									
				for (auto it = NamesVector.begin(); it != NamesVector.end();)
				{
					if (it->bNeedDelete)
						it = NamesVector.erase(it);
					else
						++it;
				}
			}

			for (unsigned Index = 0, CountNames = NamesVector.size(); Index < CountNames; Index++)
			{
				Stroka = m_TextFont.GetTextSize(NamesVector[Index].Name);
				if (LeftDelta < Stroka.cx + 4)
					LeftDelta = Stroka.cx + 4;

				glColor3ub(GetRValue(grd->grid_color), GetGValue(grd->grid_color), GetBValue(grd->grid_color));
				glLineWidth(2.0f);
				if (NamesVector[Index].bUnited)
				{
					glBegin(GL_LINES);

					bool bCanDrow = true;
					if (Index > 0)
						if (NamesVector[Index - 1].TextBottomCoord > NamesVector[Index].TopCoord)
							bCanDrow = false;				
				
					if (bCanDrow)
					{
						// ������� �����
						glVertex3f(GLfloat(0),			 GLfloat(NamesVector[Index].TopCoord), 0.f);			// ����� ������
						glVertex3f(GLfloat(grd->left_x), GLfloat(NamesVector[Index].TopCoord), 0.f);			// ������ �����
					}
				
					// ������ �����
					glVertex3f(GLfloat(0),			 GLfloat(NamesVector[Index].BottomCoord), 0.f);			// ����� ������
					glVertex3f(GLfloat(grd->left_x), GLfloat(NamesVector[Index].BottomCoord), 0.f);			// ������ �����

					glEnd();
				}
				m_TextFont.PrintTextGL((GLfloat)(grd->left_x) / 2 - (GLfloat)Stroka.cx / 2, 
									   (GLfloat)(grd->bottom_y + grd->height_y + grd->top_y - NamesVector[Index].TextBottomCoord),
									   GetRValue(grd->digits_color), GetGValue(grd->digits_color), GetBValue(grd->digits_color),
									   NamesVector[Index].Name);
			}
		}
	}

	if (grd->bVerticalGraphs)
	{
		int NameDelta(0);
		if (!m_SelectedGraphName.IsEmpty())
			NameDelta = m_TextFont.GetTextSize(m_SelectedGraphName).cy + 4;		
		
		LeftDelta += NameDelta;
		if (LeftDelta < 15)
			LeftDelta = 15;
	}

	if (grd->left_x != LeftDelta)
	{
		grd->left_x = LeftDelta;
		return NEED_INVALIDATE;
	}
	
	return 0;
}

long CTimeGridDrow::CalculateIntermediateSplitDigits(_Inout_ struct TrendGrid *grd, _In_ CDC* pdc, 
													 _In_ double MinVal, _In_ double MaxVal, 
													 _In_  GLfloat CurCoord, _In_ GLfloat HeightVert, _In_ bool bNeedDrow)
{
	TCHAR str[200];
	CSize stroka;

	GLint sdvig;
	GLfloat vert;

	long Max_Cx(0);
	if (HeightVert >= 100)
	{
		double AverageVal = MinVal + (MaxVal - MinVal) / 2;
		_stprintf_s(str, 200, _T("%g"), AverageVal);
		stroka = pdc->GetTextExtent(str, wcslen(str));

		if (bNeedDrow)
		{
			if (grd->bVerticalGraphs)
			{
				// ���� ����� ���������� �������� 
				sdvig = grd->left_x - 3 + grd->width_x + stroka.cy;
				vert = CurCoord + HeightVert / 2 + stroka.cx / 2;
			}
			else
			{
				// ���� ����� ���������� �������� 
				sdvig = grd->left_x - stroka.cx - 4;
				vert = CurCoord + HeightVert / 2 + stroka.cy / 2;
			}
			TextGL((GLfloat)sdvig, vert, str, GetRValue(grd->digits_color), GetGValue(grd->digits_color), GetBValue(grd->digits_color));
			CalculateIntermediateSplitDigits(grd, pdc, AverageVal, MaxVal, CurCoord, HeightVert / 2, true);
			CalculateIntermediateSplitDigits(grd, pdc, MinVal, AverageVal, CurCoord + HeightVert / 2, HeightVert / 2, true);
		}
		else
		{
			// ���� ����� ��������� ������
			if (Max_Cx < stroka.cx)
				Max_Cx = stroka.cx;

			long CurMax = CalculateIntermediateSplitDigits(grd, pdc, AverageVal, MaxVal, 0, HeightVert / 2);
			if (Max_Cx < CurMax)
				Max_Cx = CurMax;
			CurMax = CalculateIntermediateSplitDigits(grd, pdc, MinVal, AverageVal, 0, HeightVert / 2);
			if (Max_Cx < CurMax)
				Max_Cx = CurMax;
		}
	}

	return Max_Cx;
}

void CTimeGridDrow::CalculateSplitFieldsSettings(_In_ struct TrendGrid* grd, _In_ size_t CountOfFields, _In_opt_ bool bCheckHeight /*= true*/)
{
	m_SplitFieldsData.resize(CountOfFields);
	
	m_bSplitFieldsHeight = ((GLfloat)grd->height_y - SPLIT_FIELDS_DELTA) / CountOfFields;	// ������������� �������
	GLfloat CurCoord;	// ���������� �� ��������� ��� ��������� �����
	// ������������ � ����������� ���������� ��� �������
	GLfloat PixYMax, PixYMin, TopLineCoord;

	// ���� �� ������� ������ ��� ����������� ����������� ��������� �����
	if ((m_bSplitFieldsHeight < 150) && bCheckHeight)
	{
		m_bSplitFieldsNeedNameOnTop = false;

		m_bSplitFieldsHeight = ((GLfloat)grd->height_y) / CountOfFields;	// ������������� �������		
		CurCoord = (GLfloat)grd->top_y + 1;	// ���������� �� ��������� ��� ��������� �����
		// ������������ � ����������� ���������� ��� �������
		PixYMax = CurCoord;
		PixYMin = CurCoord + m_bSplitFieldsHeight;
	}
	else
	{
		m_bSplitFieldsNeedNameOnTop  = true;

		CurCoord = (GLfloat)grd->top_y + 1 + SPLIT_FIELDS_DELTA;	// ���������� �� ��������� ��� ��������� �����
		// ������������ � ����������� ���������� ��� �������
		PixYMax = CurCoord + SPLIT_FIELDS_DELTA;
		PixYMin = CurCoord + m_bSplitFieldsHeight - SPLIT_FIELDS_DELTA;
	}
	TopLineCoord = CurCoord;
	
	int CurIndex = 0;
	for (unsigned Index = 0; Index < CountOfFields; Index++)
	{
		if (grd->bVerticalGraphs)
			CurIndex = CountOfFields - Index - 1;
		else
			CurIndex = Index;

		m_SplitFieldsData[CurIndex].PixYMax = PixYMax;
		m_SplitFieldsData[CurIndex].PixYMin = PixYMin;
		m_SplitFieldsData[CurIndex].TopLineCoord = TopLineCoord;

		PixYMax += m_bSplitFieldsHeight;
		PixYMin += m_bSplitFieldsHeight;
		TopLineCoord += m_bSplitFieldsHeight;
	}
}

long CTimeGridDrow::SplitFieldsVertDigits(CDC* pdc, struct TrendGrid* grd, const CRect &rcBounds)
{
	if (!_finite(grd->math_width_x) || !_finite(grd->math_width_y))
		return -1;

	TCHAR str[MAX_PATH];
	CSize stroka, UnitHeight;
		
	GLfloat sdvig, vert;
	long max_cx = 0;
	UINT Index(0);

	int MaxHeight(0);
	std::vector<CString> ChanNames(m_CurGraphCount);
	std::vector<CString> ChanUnits(m_CurGraphCount);
	if (grd->bVerticalGraphs)
	{
		// �������� ��� �� ������ ����� � ������ ������� ��� �����
		UnitHeight = pdc->GetTextExtent(L"0");
		CSize str_text_Units;

		for (UINT CurIndex = 0; CurIndex < m_CountGraph; CurIndex++)
		{
			if (!m_bValidGraphs.empty() && !m_bValidGraphs[CurIndex])
				continue;

			if (m_FieldsUnits.size() > CurIndex)
			{
				ChanUnits[Index] = m_FieldsUnits[CurIndex];

				// ������� ���������
				str_text_Units = m_TextFont.GetTextSize(m_FieldsUnits[CurIndex]);
				if (max_cx < UnitHeight.cy + str_text_Units.cy)
					max_cx = UnitHeight.cy + str_text_Units.cy;
			}

			// ��������� ������ ����
			ChanNames[Index] = m_FieldsName[CurIndex];

			// ���� ����������� ������ ������
			stroka = m_TextFont.GetTextSize(m_FieldsName[CurIndex]);

			if (MaxHeight < stroka.cy)
				MaxHeight = stroka.cy;

			Index++;
		}

		// ������������ ����������� ������
		CalculateNewStr(ChanNames, m_bSplitFieldsHeight - 5, &m_TextFont);
		CalculateNewStr(ChanUnits, m_bSplitFieldsHeight - 5, &m_TextFont);
	}
	else
	{
		// ��������� �� ������� ����� �������� ����� �������
		for (UINT CurIndex = 0; CurIndex < m_CountGraph; CurIndex++)
		{
			if (!m_bValidGraphs.empty() && !m_bValidGraphs[CurIndex])
				continue;

			if (m_FieldsUnits.size() > CurIndex)
			{
				ChanUnits[Index] = m_FieldsUnits[CurIndex];

				// ������� ���������
				stroka = m_TextFont.GetTextSize(m_FieldsUnits[CurIndex]);
				if (max_cx < stroka.cx)
					max_cx = stroka.cx;
			}

			// ������������ �����
			_stprintf_s(str, MAX_PATH, _T("%g"), m_SplitFieldsData[Index].MaxValue);
			stroka = pdc->GetTextExtent(str, wcslen(str));
			if (max_cx < stroka.cx)
				max_cx = stroka.cx;

			// ����������� �����
			_stprintf_s(str, MAX_PATH, _T("%g"), m_SplitFieldsData[Index].MinValue);
			stroka = pdc->GetTextExtent(str, wcslen(str));
			if (max_cx < stroka.cx)
				max_cx = stroka.cx;

			// ���������� ������������� ����� ���� ��� �����
			long CurMax = CalculateIntermediateSplitDigits(grd, pdc,
														   m_SplitFieldsData[Index].MinValue, m_SplitFieldsData[Index].MaxValue,
														   0, m_bSplitFieldsHeight - SPLIT_FIELDS_DELTA * 2);

			if (max_cx < CurMax)
				max_cx = CurMax;

			Index++;
		}
	}

	if (grd->bVerticalGraphs)
	{
		if (max_cx < 30)
			max_cx = 30;
		
		stroka = m_TextFont.GetTextSize(m_SelectedGraphName);
		
		MaxHeight += stroka.cy + 8;
		if (MaxHeight < 15)
			MaxHeight = 15;

		if ((grd->right_x != max_cx) || (grd->left_x != MaxHeight))
		{
			grd->right_x = max_cx;
			grd->left_x = MaxHeight;
			return NEED_INVALIDATE;
		}
	}
	else
	{
		max_cx += 9;
		if (max_cx < 30)
			max_cx = 30;

		if ((grd->left_x != max_cx) || (grd->right_x != 15))
		{
			grd->left_x = max_cx;
			grd->right_x = 15;
			return NEED_INVALIDATE;
		}
	}
	
	UINT CurIndex(0);
	Index = 0;
	// ������� ��������
	for (size_t Size = m_SplitFieldsData.size(); Index < Size; Index++)
	{
/*
		if (grd->bVerticalGraphs) // � �������� �������
			CurIndex = Size - Index - 1;
		else*/
			CurIndex = Index;

		if (grd->bVerticalGraphs)
		{
			if (ChanNames.size() >= CurIndex && !ChanNames.empty())
			{
				stroka = m_TextFont.GetTextSize(ChanNames[CurIndex]);

				glPushMatrix();
				glLoadIdentity();
				m_TextFont.PrintTextGL(GLfloat(grd->top_y + grd->height_y + grd->bottom_y - ((m_SplitFieldsData[CurIndex].PixYMin + m_SplitFieldsData[CurIndex].PixYMax) / 2 + stroka.cx / 2)),
									   GLfloat(grd->right_x + grd->width_x + 4),
									   GetRValue(grd->digits_color), GetGValue(grd->digits_color), GetBValue(grd->digits_color),
									   ChanNames[CurIndex]);
				glPopMatrix();
			}			
		}

		if (ChanUnits.size() > CurIndex)
		{
			// ������� ���������
			stroka = m_TextFont.GetTextSize(ChanUnits[CurIndex]);
			if (grd->bVerticalGraphs)
			{
				// ���� ����� ���������� �������� 
				sdvig = grd->top_y + grd->height_y + grd->bottom_y - (m_SplitFieldsData[CurIndex].TopLineCoord + m_bSplitFieldsHeight / 2 + stroka.cx / 2);
				vert = 3.f;
			}
			else
			{
				// ���� ����� ���������� �������� 
				sdvig = GLfloat(grd->left_x - stroka.cx - 4);
				vert = grd->top_y + grd->height_y + grd->bottom_y - (m_SplitFieldsData[CurIndex].TopLineCoord + stroka.cy / 2);
			}

			glPushMatrix();
			glLoadIdentity();
			m_TextFont.PrintTextGL((GLfloat)sdvig, (GLfloat)(vert), 
								   GetRValue(grd->digits_color), GetGValue(grd->digits_color), GetBValue(grd->digits_color), 
								   ChanUnits[CurIndex]);
			glPopMatrix();
		}

		// ������������ �����
		_stprintf_s(str, MAX_PATH, _T("%g"), m_SplitFieldsData[CurIndex].MaxValue);
		stroka = pdc->GetTextExtent(str, wcslen(str));
		if (grd->bVerticalGraphs)
		{
			// ���� ����� ���������� �������� 
			sdvig = GLfloat(grd->left_x - 3 + grd->width_x + stroka.cy);
			vert = m_SplitFieldsData[CurIndex].PixYMax + stroka.cx / 2;
		}
		else
		{
			// ���� ����� ���������� �������� 
			sdvig = GLfloat(grd->left_x - stroka.cx - 4);
			vert = m_SplitFieldsData[CurIndex].PixYMax + stroka.cy / 2;
		}
		TextGL((GLfloat)sdvig, (GLfloat)(vert), str, GetRValue(grd->digits_color), GetGValue(grd->digits_color), GetBValue(grd->digits_color));

		// ����������� �����
		_stprintf_s(str, MAX_PATH, _T("%g"), m_SplitFieldsData[CurIndex].MinValue);
		stroka = pdc->GetTextExtent(str, wcslen(str));
		if (grd->bVerticalGraphs)
		{
			// ���� ����� ���������� �������� 
			sdvig = GLfloat(grd->left_x - 3 + grd->width_x + stroka.cy);
			vert = m_SplitFieldsData[CurIndex].PixYMin + stroka.cx / 2;
		}
		else
		{
			// ���� ����� ���������� �������� 
			sdvig = GLfloat(grd->left_x - stroka.cx - 4);
			vert = m_SplitFieldsData[CurIndex].PixYMin + stroka.cy / 2;
		}
		TextGL((GLfloat)sdvig, (GLfloat)(vert), str, GetRValue(grd->digits_color), GetGValue(grd->digits_color), GetBValue(grd->digits_color));


		// ���������� ������������� ����� ���� ��� �����
		CalculateIntermediateSplitDigits(grd, pdc, 
										 m_SplitFieldsData[CurIndex].MinValue, 
										 m_SplitFieldsData[CurIndex].MaxValue,
										 m_SplitFieldsData[CurIndex].PixYMax, 
										 m_bSplitFieldsHeight - SPLIT_FIELDS_DELTA * 2, true);
	}

	return 0;
}
#pragma endregion

#pragma region ��������� ��������
void CTimeGridDrow::DrowVertCursor(_In_ struct TrendGrid *grd, _In_ const CursorStruct &CursInfo, _In_ bool bSelected)
{
	COLORREF Color;
	if (bSelected)
		Color = RGB(255, 0, 0);
	else if (CursInfo.Type == CursorType::Type_Default)
		Color = grd->cursor_color;
	else
		Color = CursInfo.Color;

	double TempCurPosition(CursInfo.CurrentPos);
	if (TempCurPosition < grd->math_min_x)
		TempCurPosition = grd->math_min_x;
	else if (TempCurPosition > grd->math_min_x + grd->math_width_x)
		TempCurPosition = grd->math_min_x + grd->math_width_x;
	else if (!grd->bVerticalGraphs)
	{
		CZetTime zt(TempCurPosition);
		CString temp;
		zt.ConvertToString(temp.GetBuffer(MAX_PATH), MAX_PATH, CursorCoords.FirstStrPrecision, CursorCoords.FirstStrFormat);
		temp.ReleaseBuffer();
		CSize TextSize(CDC::FromHandle(m_hDC)->GetTextExtent(temp));

		TextGL(GetPixX(grd, TempCurPosition) + CURSOR_SIZE, grd->top_y - CURSOR_SIZE - 1 + (GLfloat)TextSize.cy / 4, temp,
			   GetRValue(Color), GetGValue(Color), GetBValue(Color));
	}
	GLfloat CursCoord(GetPixX(grd, TempCurPosition)), ZCoord;

	if (bSelected)
		ZCoord = bUseOrtho2D ? 0.f : 9.f;
	else
		ZCoord = bUseOrtho2D ? 0.f : 8.f;

	glLineWidth(1);
	glColor3ub(GetRValue(Color), GetGValue(Color), GetBValue(Color));

	glBegin(GL_LINES);
	glVertex3f(CursCoord, GLfloat(grd->top_y), ZCoord);							// ������
	glVertex3f(CursCoord, GLfloat(grd->height_y + grd->top_y), ZCoord);			// �����
	glEnd();

	switch (CursInfo.Figure)
	{
		case CursorFigure::Figure_Thriangle:
			glBegin(GL_TRIANGLES);
			glVertex3f(CursCoord - CURSOR_SIZE, GLfloat(grd->top_y - CURSOR_SIZE * 2), ZCoord);	// ����� ������
			glVertex3f(CursCoord + CURSOR_SIZE, GLfloat(grd->top_y - CURSOR_SIZE * 2), ZCoord);	// ������ ������
			glVertex3f(CursCoord, GLfloat(grd->top_y + 1), ZCoord);								// �����
			glEnd();

			glLineWidth(2);
			glColor3ub(0, 0, 0);
			glBegin(GL_LINES);
			glVertex3f(CursCoord - CURSOR_SIZE, GLfloat(grd->top_y - CURSOR_SIZE * 2), ZCoord);			// ����� ������
			glVertex3f(CursCoord + CURSOR_SIZE, GLfloat(grd->top_y - CURSOR_SIZE * 2), ZCoord);			// ������ ������

			glVertex3f(CursCoord + CURSOR_SIZE, GLfloat(grd->top_y - CURSOR_SIZE * 2), ZCoord);			// ������ ������
			glVertex3f(CursCoord, GLfloat(grd->top_y + 1), ZCoord);										// �����

			glVertex3f(CursCoord, GLfloat(grd->top_y + 1), ZCoord);										// �����
			glVertex3f(CursCoord - CURSOR_SIZE, GLfloat(grd->top_y - CURSOR_SIZE * 2), ZCoord);			// ����� ������
			glEnd();
			break;
		case CursorFigure::Figure_Square:
			glBegin(GL_QUADS);
			glVertex3f(CursCoord - CURSOR_SIZE, GLfloat(grd->top_y - CURSOR_SIZE * 2), ZCoord);			// ����� ������
			glVertex3f(CursCoord + CURSOR_SIZE, GLfloat(grd->top_y - CURSOR_SIZE * 2), ZCoord);			// ������ ������
			glVertex3f(CursCoord + CURSOR_SIZE, GLfloat(grd->top_y), ZCoord);							// ������ �����
			glVertex3f(CursCoord - CURSOR_SIZE, GLfloat(grd->top_y), ZCoord);							// ����� �����
			glEnd();

			glLineWidth(2);
			glColor3ub(0, 0, 0);
			glBegin(GL_LINES);
			glVertex3f(CursCoord - CURSOR_SIZE, GLfloat(grd->top_y - CURSOR_SIZE * 2), ZCoord);			// ����� ������
			glVertex3f(CursCoord + CURSOR_SIZE, GLfloat(grd->top_y - CURSOR_SIZE * 2), ZCoord);			// ������ ������

			glVertex3f(CursCoord + CURSOR_SIZE, GLfloat(grd->top_y - CURSOR_SIZE * 2), ZCoord);			// ������ ������
			glVertex3f(CursCoord + CURSOR_SIZE, GLfloat(grd->top_y), ZCoord);							// ������ �����

			glVertex3f(CursCoord + CURSOR_SIZE, GLfloat(grd->top_y), ZCoord);							// ������ �����
			glVertex3f(CursCoord - CURSOR_SIZE, GLfloat(grd->top_y), ZCoord);							// ����� �����

			glVertex3f(CursCoord - CURSOR_SIZE, GLfloat(grd->top_y), ZCoord);							// ����� �����
			glVertex3f(CursCoord - CURSOR_SIZE, GLfloat(grd->top_y - CURSOR_SIZE * 2), ZCoord);			// ����� ������
			glEnd();
			break;
		case CursorFigure::Figure_Circle:
		{
			GLUquadric *qua = gluNewQuadric();
			glPushMatrix();
			glTranslatef(CursCoord, GLfloat(grd->top_y - CURSOR_SIZE), ZCoord);
			gluDisk(qua, 0, (GLdouble)CURSOR_SIZE, 2 * 
					GLint(2 * M_PI * CURSOR_SIZE / 3), 1);
			glColor3ub(0, 0, 0);
			gluDisk(qua, (GLdouble)(CURSOR_SIZE - 2), (GLdouble)CURSOR_SIZE, 
					GLint(2 * M_PI * CURSOR_SIZE / 3), 1);
			glPopMatrix();
			gluDeleteQuadric(qua);
			break;
		}
		default:
			break;
	}
}

void CTimeGridDrow::DrowHorzCursor(_In_ struct TrendGrid *grd, _In_ const CursorStruct &CursInfo, _In_ bool bSelected)
{
	COLORREF Color;
	if (bSelected)
		Color = RGB(255, 0, 0);
	else if (CursInfo.Type == CursorType::Type_Default)
		Color = grd->cursor_color;
	else
		Color = CursInfo.Color;

	glLineWidth(1);
	glColor3ub(GetRValue(Color), GetGValue(Color), GetBValue(Color));

	double TempCurPosition(CursInfo.CurrentPos);
	if (TempCurPosition < grd->math_min_y)
		TempCurPosition = grd->math_min_y;
	if (TempCurPosition > grd->math_min_y + grd->math_width_y)
		TempCurPosition = grd->math_min_y + grd->math_width_y;
	GLfloat CursCoord(GetPixY(grd, TempCurPosition)), ZCoord;

	if (bSelected)
		ZCoord = bUseOrtho2D ? 0.f : 9.f;
	else
		ZCoord = bUseOrtho2D ? 0.f : 8.f;

	glBegin(GL_LINES);
	glVertex3f(GLfloat(grd->left_x), CursCoord, ZCoord);							// ������
	glVertex3f(GLfloat(grd->left_x + grd->width_x - 2), CursCoord, ZCoord);			// �����
	glEnd();

	switch (CursInfo.Figure)
	{
		case CursorFigure::Figure_Thriangle:
			glBegin(GL_TRIANGLES);
			if (grd->bVerticalGraphs)
			{
				glVertex3f(GLfloat(grd->left_x - CURSOR_SIZE * 2), CursCoord - CURSOR_SIZE, ZCoord);		// ������ ������
				glVertex3f(GLfloat(grd->left_x - CURSOR_SIZE * 2), CursCoord + CURSOR_SIZE, ZCoord);		// ������ �����
				glVertex3f(GLfloat(grd->left_x), CursCoord, ZCoord);										// �����
			}
			else
			{
				glVertex3f(GLfloat(grd->left_x + grd->width_x - 3 + CURSOR_SIZE * 2), CursCoord - CURSOR_SIZE, ZCoord);		// ������ ������
				glVertex3f(GLfloat(grd->left_x + grd->width_x - 3 + CURSOR_SIZE * 2), CursCoord + CURSOR_SIZE, ZCoord);		// ������ �����
				glVertex3f(GLfloat(grd->left_x + grd->width_x - 3), CursCoord, ZCoord);										// �����
			}
			glEnd();

			glLineWidth(2);
			glColor3ub(0, 0, 0);
			glBegin(GL_LINES);
			if (grd->bVerticalGraphs)
			{
				glVertex3f(GLfloat(grd->left_x - CURSOR_SIZE * 2), CursCoord - CURSOR_SIZE, ZCoord);			// ������ ������
				glVertex3f(GLfloat(grd->left_x - CURSOR_SIZE * 2), CursCoord + CURSOR_SIZE, ZCoord);			// ������ �����

				glVertex3f(GLfloat(grd->left_x - CURSOR_SIZE * 2), CursCoord + CURSOR_SIZE, ZCoord);			// ������ �����
				glVertex3f(GLfloat(grd->left_x), CursCoord, ZCoord);											// �����

				glVertex3f(GLfloat(grd->left_x), CursCoord, ZCoord);											// �����
				glVertex3f(GLfloat(grd->left_x - CURSOR_SIZE * 2), CursCoord - CURSOR_SIZE, ZCoord);			// ������ ������
			}
			else
			{
				glVertex3f(GLfloat(grd->left_x + grd->width_x - 3 + CURSOR_SIZE * 2), CursCoord - CURSOR_SIZE, ZCoord);			// ������ ������
				glVertex3f(GLfloat(grd->left_x + grd->width_x - 3 + CURSOR_SIZE * 2), CursCoord + CURSOR_SIZE, ZCoord);			// ������ �����

				glVertex3f(GLfloat(grd->left_x + grd->width_x - 3 + CURSOR_SIZE * 2), CursCoord + CURSOR_SIZE, ZCoord);			// ������ �����
				glVertex3f(GLfloat(grd->left_x + grd->width_x - 3), CursCoord, ZCoord);											// �����

				glVertex3f(GLfloat(grd->left_x + grd->width_x - 3), CursCoord, ZCoord);											// �����
				glVertex3f(GLfloat(grd->left_x + grd->width_x - 3 + CURSOR_SIZE * 2), CursCoord - CURSOR_SIZE, ZCoord);			// ������ ������
			}
			glEnd();
			break;
		case CursorFigure::Figure_Square:
			glBegin(GL_QUADS);

			if (grd->bVerticalGraphs)
			{
				glVertex3f(GLfloat(grd->left_x - CURSOR_SIZE * 2), CursCoord - CURSOR_SIZE, ZCoord);		// ������ ������
				glVertex3f(GLfloat(grd->left_x - CURSOR_SIZE * 2), CursCoord + CURSOR_SIZE, ZCoord);		// ������ �����
				glVertex3f(GLfloat(grd->left_x), CursCoord + CURSOR_SIZE, ZCoord);							// ����� �����
				glVertex3f(GLfloat(grd->left_x), CursCoord - CURSOR_SIZE, ZCoord);							// ����� ������
			}
			else
			{
				glVertex3f(GLfloat(grd->left_x + grd->width_x - 3 + CURSOR_SIZE * 2), CursCoord - CURSOR_SIZE, ZCoord);		// ������ ������
				glVertex3f(GLfloat(grd->left_x + grd->width_x - 3 + CURSOR_SIZE * 2), CursCoord + CURSOR_SIZE, ZCoord);		// ������ �����
				glVertex3f(GLfloat(grd->left_x + grd->width_x - 3), CursCoord + CURSOR_SIZE, ZCoord);						// ����� �����
				glVertex3f(GLfloat(grd->left_x + grd->width_x - 3), CursCoord - CURSOR_SIZE, ZCoord);						// ����� ������
			}
			glEnd();

			glLineWidth(2);
			glColor3ub(0, 0, 0);
			glBegin(GL_LINES);
			if (grd->bVerticalGraphs)
			{
				glVertex3f(GLfloat(grd->left_x - CURSOR_SIZE * 2), CursCoord - CURSOR_SIZE, ZCoord);		// ������ ������
				glVertex3f(GLfloat(grd->left_x - CURSOR_SIZE * 2), CursCoord + CURSOR_SIZE, ZCoord);		// ������ �����

				glVertex3f(GLfloat(grd->left_x - CURSOR_SIZE * 2), CursCoord + CURSOR_SIZE, ZCoord);		// ������ �����
				glVertex3f(GLfloat(grd->left_x), CursCoord + CURSOR_SIZE, ZCoord);							// ����� �����

				glVertex3f(GLfloat(grd->left_x), CursCoord + CURSOR_SIZE, ZCoord);							// ����� �����
				glVertex3f(GLfloat(grd->left_x), CursCoord - CURSOR_SIZE, ZCoord);							// ����� ������

				glVertex3f(GLfloat(grd->left_x), CursCoord - CURSOR_SIZE, ZCoord);							// ����� ������
				glVertex3f(GLfloat(grd->left_x - CURSOR_SIZE * 2), CursCoord - CURSOR_SIZE, ZCoord);		// ������ ������
			}
			else
			{
				glVertex3f(GLfloat(grd->left_x + grd->width_x - 3 + CURSOR_SIZE * 2), CursCoord - CURSOR_SIZE, ZCoord);		// ������ ������
				glVertex3f(GLfloat(grd->left_x + grd->width_x - 3 + CURSOR_SIZE * 2), CursCoord + CURSOR_SIZE, ZCoord);		// ������ �����

				glVertex3f(GLfloat(grd->left_x + grd->width_x - 3 + CURSOR_SIZE * 2), CursCoord + CURSOR_SIZE, ZCoord);		// ������ �����
				glVertex3f(GLfloat(grd->left_x + grd->width_x - 3), CursCoord + CURSOR_SIZE, ZCoord);						// ����� �����

				glVertex3f(GLfloat(grd->left_x + grd->width_x - 3), CursCoord + CURSOR_SIZE, ZCoord);						// ����� �����
				glVertex3f(GLfloat(grd->left_x + grd->width_x - 3), CursCoord - CURSOR_SIZE, ZCoord);						// ����� ������

				glVertex3f(GLfloat(grd->left_x + grd->width_x - 3), CursCoord - CURSOR_SIZE, ZCoord);						// ����� ������
				glVertex3f(GLfloat(grd->left_x + grd->width_x - 3 + CURSOR_SIZE * 2), CursCoord - CURSOR_SIZE, ZCoord);		// ������ ������
			}
			glEnd();
			break;
		case CursorFigure::Figure_Circle:
		{
			GLUquadric *qua = gluNewQuadric();
			glPushMatrix();
			if (grd->bVerticalGraphs)
				glTranslatef(GLfloat(grd->left_x - CURSOR_SIZE), CursCoord, ZCoord);
			else
				glTranslatef(GLfloat(grd->left_x + grd->width_x - 3 + CURSOR_SIZE), CursCoord, ZCoord);
			gluDisk(qua, 0, (GLdouble)CURSOR_SIZE,
					GLint(2 * M_PI * CURSOR_SIZE / 3), 1);
			glColor3ub(0, 0, 0);
			gluDisk(qua, (GLdouble)(CURSOR_SIZE - 2), (GLdouble)CURSOR_SIZE,
					GLint(2 * M_PI * CURSOR_SIZE / 3), 1);
			glPopMatrix();
			gluDeleteQuadric(qua);
			break;
		}
		default:
			break;
	}
}

// ������������ �������
void CTimeGridDrow::DrawCursorsX(_Inout_ struct TrendGrid *grd)
{
	glEnable(GL_SCISSOR_TEST);//���. ���������
	if (grd->bVerticalGraphs)
		glScissor(grd->bottom_y, grd->right_x - CURSOR_SIZE, grd->height_y + CURSOR_SIZE * 2, grd->width_x + CURSOR_SIZE * 2);
	else 
		glScissor(grd->left_x - CURSOR_SIZE, grd->bottom_y, grd->width_x + CURSOR_SIZE * 2, grd->height_y + grd->top_y);

	for (int i = 0; i < (int)m_CoordVertCurs.size(); i++)
	{
		if (!VertCursMoves || (CurSelectedCursX != i))
			DrowVertCursor(grd, m_CoordVertCurs[i], false);
	}

	// �������� ������������ ����� �������
	if (VertCursMoves)
		DrowVertCursor(grd, m_CoordVertCurs[CurSelectedCursX], true);

	glDisable(GL_SCISSOR_TEST);//���. ���������
}

// �������������� �������
void CTimeGridDrow::DrawCursorsY(_Inout_ struct TrendGrid *grd)
{
	for (int i = 0; i < (int)m_CoordGorCurs.size(); i++)
	{
		if (!GorCursMoves || (CurSelectedCursY != i))
			DrowHorzCursor(grd, m_CoordGorCurs[i], false);
	}

	// �������� ������������ ����� �������
	if (GorCursMoves)
		DrowHorzCursor(grd, m_CoordGorCurs[CurSelectedCursY], true);
}
#pragma endregion

void CTimeGridDrow::PaintDistinguish(CPoint FirstPoint, CPoint LastPoint)
{
	glEnable(GL_LINE_STIPPLE);
	glLineWidth(2);
	glPointSize(2);
	glColor3ub(0, 0, 0);
	glLineStipple(1, 0x00F0);
	glBegin(GL_LINES);

	glVertex3i(FirstPoint.x, FirstPoint.y, bUseOrtho2D ? 0 : 10);						// ����� ������
	glVertex3i(LastPoint.x,  FirstPoint.y, bUseOrtho2D ? 0 : 10);						// ������ ������

	glVertex3i(LastPoint.x,  LastPoint.y,  bUseOrtho2D ? 0 : 10);						// ������ �����
	glVertex3i(FirstPoint.x, LastPoint.y,  bUseOrtho2D ? 0 : 10);						// ����� �����

	glVertex3i(FirstPoint.x, FirstPoint.y, bUseOrtho2D ? 0 : 10);						// ����� ������
	glVertex3i(FirstPoint.x, LastPoint.y,  bUseOrtho2D ? 0 : 10);						// ����� �����

	glVertex3i(LastPoint.x,  FirstPoint.y, bUseOrtho2D ? 0 : 10);						// ������ ������
	glVertex3i(LastPoint.x,  LastPoint.y,  bUseOrtho2D ? 0 : 10);						// ������ �����

	glEnd();
	glDisable(GL_LINE_STIPPLE);
}

void CTimeGridDrow::AutoScale(_Inout_ struct TrendGrid* grd, _In_ double _LeftXCoord, _In_ double _RightXCoord, _In_ double _BottomYCoord, _In_ double _TopYCoord)
{
	AutoScaleX(grd, _LeftXCoord, _RightXCoord);
	AutoScaleY(grd, _BottomYCoord, _TopYCoord);
}

void CTimeGridDrow::AutoScaleX(_Inout_ struct TrendGrid* grd, _In_ double _LeftXCoord, _In_ double _RightXCoord)
{
	if ((_LeftXCoord >= 0) && (_RightXCoord > 0))
	{
		// ���� ������ ������������ ��� ������ ��������
		if (_LeftXCoord != _RightXCoord)
		{
			CZetTimeSpan dt = _RightXCoord - _LeftXCoord;
			dt *= PERSENTAGE_INDENTION;
			_LeftXCoord -= dt;
			_RightXCoord += dt;

			grd->math_min_x = _LeftXCoord;
			grd->math_width_x = _RightXCoord - _LeftXCoord;
		}
		else
			grd->math_min_x = _LeftXCoord - grd->math_width_x / 2;

		CheckScaleX(grd);
	}
}

void CTimeGridDrow::AutoScaleY(_Inout_ struct TrendGrid* grd, _In_ double _BottomYCoord, _In_ double _TopYCoord)
{
	if ((_BottomYCoord != DBL_MAX) && (_TopYCoord != -DBL_MAX) && 
		((grd->Display_Mode == DisplayMode::Default) || 
		 (grd->Display_Mode == DisplayMode::SeismicGrath) ||
		 (grd->Display_Mode == DisplayMode::ImpositionGraths)))
	{
		if (!IsEqual(_TopYCoord, _BottomYCoord, 0.00000001))
		{
			_TopYCoord    += (_TopYCoord - _BottomYCoord) * PERSENTAGE_INDENTION;
			_BottomYCoord -= (_TopYCoord - _BottomYCoord) * PERSENTAGE_INDENTION;
			grd->math_min_y = _BottomYCoord;
			grd->math_width_y = _TopYCoord - _BottomYCoord;
		}
		else
			grd->math_min_y = _BottomYCoord - grd->math_width_y / 2;

		CheckScaleY(grd);
	}

	if ((grd->Display_Mode == DisplayMode::SeismicGrath) ||
		(grd->Display_Mode == DisplayMode::ImpositionGraths))
	{
		// ��������� �� ����������� ��������� ����� � ������� �������
		float CurVal;
		float NextVal;
		CurVal = CalculateStunnerValues(float(grd->math_width_y), nullptr, &NextVal);
		if (grd->math_width_y <= CurVal)
			grd->math_width_y = CurVal;
		else
			grd->math_width_y = NextVal;
	}
}

void CTimeGridDrow::AutoScaleVisibleData(struct TrendGrid* grd)
{
	AutoScale(grd, m_VisibleLeftXCoord, m_VisibleRightXCoord, m_VisibleBottomYCoord, m_VisibleTopYCoord);
}

void CTimeGridDrow::AutoScaleAllData(struct TrendGrid* grd)
{
	AutoScale(grd, m_LeftXCoord, m_RightXCoord, m_BottomYCoord, m_TopYCoord);
}

void CTimeGridDrow::AutoScaleYData(struct TrendGrid* grd)
{
	AutoScaleY(grd, m_IntervalBottomYCoord, m_IntervalTopYCoord);
}

void CTimeGridDrow::DrowColorLine(CDC* pdc, struct TrendGrid* grd, const CRect &rcBounds)
{
	CColorLine ColorLine;

	int TopCoord = grd->top_y + grd->height_y - 1 + grd->bottom_y - grd->ColorLine_Height;
	for (auto &it : m_ColorLine)
	{
		it.FullRect = CRect(rcBounds.left + 1,
			TopCoord,
			rcBounds.right - 1,
			TopCoord + COLORLINE_HEIGHT);

		TopCoord += COLORLINE_HEIGHT + INTERVAL_BETWENN_COLORLINES;

		it.PixBegin = float(grd->left_x + 1);
		it.PixEnd = float(grd->left_x + grd->width_x - 1);
		it.FontList = m_FontList;
		
		size_t Span_Count = it.SpanDateVector.size();
		it.SpanDrowVector.resize(Span_Count);
		for (unsigned Index = 0; Index < Span_Count; Index++)
		{
			it.SpanDrowVector[Index] = {	(it.SpanDateVector[Index].IntervalStart < 0?  0 : GetPixX(grd, it.SpanDateVector[Index].IntervalStart)),
											(it.SpanDateVector[Index].IntervalEnd	< 0 ? 0 : GetPixX(grd, it.SpanDateVector[Index].IntervalEnd)),
											it.SpanDateVector[Index].Color };
		}

		ColorLine.DrowColorLine(pdc, &it);
	}
}

void CTimeGridDrow::DrowMarkLine(CDC* pdc, struct TrendGrid* grd, const CRect &rcBounds)
{
	glEnable(GL_SCISSOR_TEST);//���. ���������
	if (grd->bVerticalGraphs)
		glScissor(grd->bottom_y + 2, grd->right_x + 4, grd->height_y - 2, grd->width_x - 4);
	else
		glScissor(grd->left_x + 1, 0, grd->width_x - 5, rcBounds.Height());

	glLineWidth(2);
	for (auto &Mark : m_MarkLines)
	{
		CSize TextSize(pdc->GetTextExtent(Mark.Text));
		GLfloat XCoord(GetPixX(grd, Mark.Time));
		// ������ �����
		glColor3ub(GetRValue(Mark.Color), GetGValue(Mark.Color), GetBValue(Mark.Color));
		glBegin(GL_LINES);
		{
			glVertex3f(XCoord, (GLfloat)grd->top_y, bUseOrtho2D ? 0.f : 1.f);
			glVertex3f(XCoord, (GLfloat)grd->top_y + grd->height_y - 1, bUseOrtho2D ? 0.f : 1.f);
		}
		glEnd();

		if (grd->bVerticalGraphs)
			TextGL(XCoord - 4, (GLfloat)grd->top_y + TextSize.cx + 2, Mark.Text,
				   GetRValue(Mark.Color), GetGValue(Mark.Color), GetBValue(Mark.Color));
		else
			TextGL(XCoord, grd->top_y - CURSOR_SIZE - 1 + (GLfloat)TextSize.cy / 4, Mark.Text, 
				   GetRValue(Mark.Color), GetGValue(Mark.Color), GetBValue(Mark.Color));
	}
	glDisable(GL_SCISSOR_TEST);//���. ���������
}

void CTimeGridDrow::AddOrRemoveToolTipToVisible(CPoint point)
{
	if (!m_toolTips.empty())
	{
		for (int i = 0; i < int(m_toolTips.size()); i++) // ����������� ��� ���������� �������� �� ����� �������, ������������� ����� �������� ������� ������.
		{
			if (PtInRect(m_toolTips[i].rect, point))
			{
				m_toolTips[i].addedToVisible = !m_toolTips[i].addedToVisible;	// ��� ����� ����������/�������� ������
				break;
			}
		}
	}
}

void CTimeGridDrow::DrawToolTips(struct TrendGrid* grd, CDC* pdc)
{
	for (int i = 0; i < int(m_toolTips.size()); i++)
	{
		long pos = m_toolTips[i].pos;
		long min_pos = (long)GetPixX(grd, grd->math_min_x);
		long max_pos = (long)GetPixX(grd, grd->math_min_x + grd->math_width_x);
		if (pos > min_pos && pos < max_pos)
		{
			m_toolTips[i].rect.left = 10;//coords_cursor[pos - (long)grd->math_min_x] - 10;
			m_toolTips[i].rect.right = 10;//coords_cursor[pos - (long)grd->math_min_x] + 10;

			m_toolTips[i].rect.top = 20;
			m_toolTips[i].rect.bottom = 40;

			/*			m_tools.SetToolRect(this, 2000 + i, m_toolTips[i].rect);*/

			int X = 10;//coords_cursor[pos - (long)grd->math_min_x] - 10; // 10 - �������� ��������������
			int Y = m_toolTips[i].rect.top;

			glColor3ub(grd->cursor_color & 255, (grd->cursor_color >> 8) & 255, (grd->cursor_color >> 16) & 255);

			glBegin(GL_TRIANGLES);
			glVertex2i((GLint)X, (GLint)Y);
			glVertex2i((GLint)X + 10, (GLint)Y + 10);
			glVertex2i((GLint)X + 20, (GLint)Y);
			glEnd();

			if (m_toolTips[i].addedToVisible)
			{
				GLUquadricObj *quadObj;
				quadObj = gluNewQuadric();
				gluQuadricDrawStyle(quadObj, GLU_FILL); // �����: ��������

				int x = m_toolTips[i].rect.left + 10;
				int y = m_toolTips[i].rect.bottom + 2;
				CSize sz = pdc->GetTextExtent(m_toolTips[i].text);

				glTranslatef((float)x, (float)y, (float)0);
				gluCylinder(quadObj, 0, m_toolTips[i].rect.Height() / 2, 0, 15, 15);	//������ ������ �������. � ����� ����� ������ ������
				glTranslatef((float)-(x), (float)-(y), (float)0);

				glBegin(GL_QUADS);
				glVertex2i(x, y - 10);
				glVertex2i(x, y + 10);
				glVertex2i(x + sz.cx, y + 10);
				glVertex2i(x + sz.cx, y - 10);
				glEnd();

				glTranslatef((float)x + (float)sz.cx, (float)y, 0);
				gluCylinder(quadObj, 0, m_toolTips[i].rect.Height() / 2, 0, 15, 15);
				glTranslatef((float)-(x + (float)sz.cx), (float)-y, 0);

				glColor3ub(grd->fon_color & 255, (grd->fon_color >> 8) & 255, (grd->fon_color >> 16) & 255);
				glTranslatef((float)x, (float)y, (float)0);
				gluCylinder(quadObj, 0, m_toolTips[i].rect.Height() / 2 - 2, 0, 15, 15);	//������ ������ ����
				glTranslatef((float)-(x), (float)-(y), (float)0);

				glBegin(GL_QUADS);
				glVertex2i(x, y - 8);
				glVertex2i(x, y + 8);
				glVertex2i(x + sz.cx, y + 8);
				glVertex2i(x + sz.cx, y - 8);
				glEnd();

				glTranslatef((float)x + (float)sz.cx, (float)y, 0); //���������� ��-�� �������������� studio
				gluCylinder(quadObj, 0, m_toolTips[i].rect.Height() / 2 - 2, 0, 15, 15);
				glTranslatef((float)-(x + (int)sz.cx), (float)-y, (float)0);

				TextGL((float)m_toolTips[i].rect.left + 10, (float)m_toolTips[i].rect.bottom + 5, m_toolTips[i].text, 0, 0, 0);

				//break;
			}
		}
	}
}

CString FormatFloatString(_In_ double FloatVal)
{
	CString ResStr(L"0");

	if (FloatVal != 0.f)
	{
		CString FormatStr;
		FormatStr.Format(L"%%0.%dlf", abs(int(log10(FloatVal))) + 2);
		ResStr.Format(FormatStr, FloatVal);
	}

	return ResStr;
}

// ������� �� ������� � ��������� ����
CString CTimeGridDrow::GetTimeText(_In_ CZetTimeSpan DeltaTime)
{
	double kolyears		= DeltaTime.GetYears();
	double kolmonth		= DeltaTime.GetMonths();
	double koldays		= DeltaTime.GetDays();
	double kolhours		= DeltaTime.GetHours();
	double kolminutes	= DeltaTime.GetMinutes();
	int64_t kolseconds	= DeltaTime.GetSeconds();
	double kolnansec	= DeltaTime.GetNanoseconds();

	CString sInterval(_T(""));
	if (kolyears > 1.5)
	{
		if (kolyears >= 5)
			sInterval.Format(L"%.001f " + TranslateString(L"���"), kolyears);
		else
			sInterval.Format(L"%.01lf " + TranslateString(L"����"), kolyears);
	}
	else if (kolmonth >= 2)
	{
		if (kolmonth >= 5)
			sInterval.Format(L"%.01lf " + TranslateString(L"�������"), kolmonth);
		else
			sInterval.Format(L"%.01lf " + TranslateString(L"������"), kolmonth);
	}
	else if (koldays >= 4)
	{
		if (koldays >= 5)
			sInterval.Format(L"%.01lf " + TranslateString(L"����"), koldays);
		else
			sInterval.Format(L"%.01lf " + TranslateString(L"���"), koldays);
	}
	else if (kolhours >= 3)
	{
		if (kolhours >= 5)
			sInterval.Format(L"%.01lf " + TranslateString(L"�����"), kolhours);
		else
			sInterval.Format(L"%.01lf " + TranslateString(L"����"), kolhours);
	}
	else if (kolminutes >= 3)
	{
		if (kolminutes >= 5)
			sInterval.Format(L"%.01lf " + TranslateString(L"�����"), kolminutes);
		else
			sInterval.Format(L"%.01lf " + TranslateString(L"������"), kolminutes);
	}
	else if (kolseconds >= 1)
	{
		if (kolseconds >= 20)
			sInterval.Format(L"%I64d " + TranslateString(L"������"), kolseconds);
		else if (kolseconds >= 5)
			sInterval.Format(L"%I64d.%01.00lf " + TranslateString(L"������"), kolseconds, round(kolnansec / 1.e8));
		else
			sInterval.Format(L"%I64d.%02.00lf " + TranslateString(L"�������"), kolseconds, round(kolnansec / 1.e7));
	}
	else
	{
		sInterval = FormatFloatString(kolnansec / 1.e9) + TranslateString(L" ������");
	}

	return sInterval;
}
// ������� � ������ � ��������� ����
CString CTimeGridDrow::GetDataText(_In_ double Delta)
{
	CString sInterval(_T(""));
	
	if (Delta > 20)			sInterval.Format(L"%.00lf", round(Delta));
	else if (Delta > 1)		sInterval.Format(L"%.01lf", Delta);
	else if (Delta > 0.3)	sInterval.Format(L"%.02lf", Delta);
	else if (Delta > 0.1)	sInterval.Format(L"%.03lf", Delta);
	else					sInterval = FormatFloatString(Delta);

	return sInterval;
}

void CTimeGridDrow::GetCursDistance(struct TrendGrid* grd, _Out_ CString &HorStr, _Out_ CString &VertStr)
{
	HorStr.Empty();
	VertStr.Empty();

	if (m_CoordVertCurs.size() < 2)
		return;

	// ����� ��������� ������ ����� ������ � ������ ��������
	HorStr = GetTimeText(fabs((CZetTime)m_CoordVertCurs[1].CurrentPos - (CZetTime)m_CoordVertCurs[0].CurrentPos));
		
	if (m_CoordGorCurs.size() < 2 || (grd->Display_Mode != DisplayMode::Default))
		return;

	VertStr = GetDataText(fabs(m_CoordGorCurs[1].CurrentPos - m_CoordGorCurs[0].CurrentPos));
}

CString CTimeGridDrow::GetCurrentUnitStr()
{
	CString Unit(_T(""));	// ������� ��������� 

	for (unsigned Graph = 0, GraphCount = m_bValidGraphs.size(); Graph < GraphCount; Graph++)
	{
		// ��������� ���������� ������������ ��������
		if (m_bValidGraphs[Graph])
		{
			if ((int)m_FieldsUnits.size() > Graph)
			{
				if (!m_FieldsUnits[Graph].IsEmpty())
				{
					if (Unit.IsEmpty())
						Unit = m_FieldsUnits[Graph];
					else if (Unit != m_FieldsUnits[Graph])
					{
						Unit.Empty();
						break;
					}
				}
			}
		}
	}
	return Unit;
}

float CalculateStunnerValues(_In_ float Val, _Out_opt_ float *PrevVal /*= nullptr*/, _Out_opt_ float *NextVal /*= nullptr*/)
{
	// 1, 2, 5 * 10^N ��� N - ����������� �����

	// ������� �����
	float Exponent = log10(Val);
	// ������� ��������
	float CurrentVal;

	// ������� �������� �����
	int CurrentExponent = (int)Exponent;
	if (Exponent < 0)
		CurrentExponent--;

	CurrentVal = pow(10.f, Exponent - CurrentExponent);

	if (CurrentVal >= 0.5 && CurrentVal < 1.5)
	{
		if (PrevVal != nullptr)
			*PrevVal = 0.5;
		CurrentVal = 1;
		if (NextVal != nullptr)
			*NextVal = 2;
	}
	else if (CurrentVal >= 1.5 && CurrentVal < 3.25)
	{
		if (PrevVal != nullptr)
			*PrevVal = 1;
		CurrentVal = 2;
		if (NextVal != nullptr)
			*NextVal = 5;
	}
	else if (CurrentVal >= 3.25 && CurrentVal < 7.5)
	{
		if (PrevVal != nullptr)
			*PrevVal = 2;
		CurrentVal = 5;
		if (NextVal != nullptr)
			*NextVal = 10;
	}
	else if (CurrentVal >= 7.5)
	{
		if (PrevVal != nullptr)
			*PrevVal = 5;
		CurrentVal = 10;
		if (NextVal != nullptr)
			*NextVal = 20;
	}

	CurrentVal = CurrentVal * pow(10.f, CurrentExponent);
	if (PrevVal != nullptr)
		*PrevVal *= pow(10.f, CurrentExponent);
	if (NextVal != nullptr)
		*NextVal *= pow(10.f, CurrentExponent);

	return CurrentVal;
}