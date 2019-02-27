//------------------------------------------------------------------------------
// DMain.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include <Interface2012\Draw\DMain.h>
#include <Dialog_ZET\Dialog_ZET.h>
//PFNGLACTIVETEXTUREARBPROC		glActiveTextureARB		= NULL;
//PFNGLCLIENTACTIVETEXTUREARBPROC	glClientActiveTextureARB = NULL;

//------------------------------------------------------------------------------
#ifdef _GDIPLUS
CDMain::CDMain(CRect rect, PVOID ppGraphics,
	bool *pFlagIsChange, double *pTimeAnimation) :
CCustomDraw(rect, ppGraphics, pFlagIsChange, pTimeAnimation)
#else
CDMain::CDMain(CRect rect,
				bool *pFlagIsChange, double *pTimeAnimation) :
				CCustomDraw(rect, pFlagIsChange, pTimeAnimation)
#endif
{
	m_pParamDraw = &m_param;	// обеспечение ОБЩИХ параметров
	m_param.Name = L"Main";
	//TRACE("CREATE CDMain %s\n", (CStringA)m_pParamDraw->Name);
	SetWidthBorder(4);			// задание m_rect2
	m_param.PointIcon.x = m_rect2.left + 2*m_param.WidthBorder;
	m_param.PointIcon.y = m_rect2.top + 2*m_param.WidthBorder;//3;// + m_param.WidthBorder;
	// переменные класса
	Bolder = 40;
    Depth = 30;
	N = 0;
#ifdef _GDIPLUS
	m_pImage = NULL;						// FontStyleRegular FontStyleItalic
//	m_param.pFont = new CZFont(L"Verdana", 9, FontStyleBold, RGB(0,0,0));	// Arial Vrinda
#else
	//m_param.pFont = new CDFonts("Calibri", 20.0 , FontsStyleItalic, RGB(20,120,110));	
	m_param.pFont = new CDFonts("Arialbd", 20.0 , FontsStyleRegular, RGB(255,255,255));	
#ifdef _ACTIVEX_ZIS
	////MessageBox(NULL, L"CDMain Constructor 2", L"ACHTUNG!", 0);
	//правый поддон
	m_Main[0] = new CDetail(long(24), (m_rect.right + m_rect.left) / 2.0, (m_rect.bottom + m_rect.top) / 2.0, -Depth, 0, m_rect.Width() - 2 * Bolder, m_rect.Height() - 2 * Bolder, Depth);
	//левая нижняя панель
	m_Main[1] = new CDetail(long(16), Bolder / 2.0 + m_rect.left, Bolder / 2.0 + m_rect.top, -Depth, 0, Bolder, Bolder, Depth);
	//нижняя панель
	m_Main[2] = new CDetail(long(17), (m_rect.right + m_rect.left) / 2.0, Bolder / 2.0 + m_rect.top, -Depth, 0, m_rect.Width() - 2 * Bolder, Bolder, Depth);
	//правый верхний угол
	m_Main[3] = new CDetail(long(18), m_rect.right - Bolder / 2.0, Bolder / 2.0 + m_rect.top, -Depth, 0, Bolder, Bolder, Depth);
	//правая часть панели
	m_Main[4] = new CDetail(long(19), m_rect.right - Bolder / 2.0, (m_rect.bottom + m_rect.top) / 2.0, -Depth, 0, Bolder, m_rect.Height() - 2 * Bolder, Depth);
	//правый нижний угол
	m_Main[5] = new CDetail(long(20), m_rect.right - Bolder / 2.0, m_rect.bottom - Bolder / 2.0, -Depth, 0, Bolder, Bolder, Depth);
	//нижняя часть панели
	m_Main[6] = new CDetail(long(21), (m_rect.right + m_rect.left) / 2.0, m_rect.bottom - Bolder / 2.0, -Depth, 0, m_rect.Width() - 2 * Bolder, Bolder, Depth);
	//левый нижний угол
	m_Main[7] = new CDetail(long(22), Bolder / 2.0 + m_rect.left, m_rect.bottom - Bolder / 2.0, -Depth, 0, Bolder, Bolder, Depth);
	//левая часть панели
	m_Main[8] = new CDetail(long(23), Bolder / 2.0 + m_rect.left, (m_rect.bottom + m_rect.top) / 2.0, -Depth, 0, Bolder, m_rect.Height() - 2 * Bolder, Depth);//*/

#else
	//центр
    m_Main[0] = new CDetail("$(ZetToolsDir)Interface2012\\Models\\Панель\\центр_панелька.stl",
		(m_rect.right + m_rect.left) / 2.0, (m_rect.bottom + m_rect.top) / 2.0, -Depth, 0,
        m_rect.Width() - 2 * Bolder, m_rect.Height() - 2 * Bolder, Depth);
	//левый верхний угол
	m_Main[1] = new CDetail("$(ZetToolsDir)Interface2012\\Models\\Панель\\левый нижний_панелька.stl",
		Bolder / 2.0 + m_rect.left, Bolder / 2.0 + m_rect.top, -Depth, 0,
		Bolder, Bolder, Depth);
	//верхняя часть панели
	m_Main[2] = new CDetail("$(ZetToolsDir)Interface2012\\Models\\Панель\\низ_панелька.stl",
		(m_rect.right + m_rect.left) / 2.0, Bolder / 2.0 + m_rect.top, -Depth, 0,
		m_rect.Width() - 2 * Bolder, Bolder, Depth);
	//правый верхний угол
	m_Main[3] = new CDetail("$(ZetToolsDir)Interface2012\\Models\\Панель\\правый нижний _панелька.stl",
		m_rect.right - Bolder / 2.0, Bolder / 2.0 + m_rect.top, -Depth, 0,
		Bolder, Bolder, Depth);	
	//правая часть панели
	m_Main[4] = new CDetail("$(ZetToolsDir)Interface2012\\Models\\Панель\\правая_панелька.stl",
		m_rect.right - Bolder / 2.0, (m_rect.bottom + m_rect.top) / 2.0, -Depth, 0,
		Bolder, m_rect.Height() - 2 * Bolder, Depth);
	//правый нижний угол
	m_Main[5] = new CDetail("$(ZetToolsDir)Interface2012\\Models\\Панель\\правый верхний _панелька.stl",
		m_rect.right - Bolder / 2.0, m_rect.bottom - Bolder / 2.0, -Depth, 0,
		Bolder, Bolder, Depth);
	//нижняя часть панели
	m_Main[6] = new CDetail("$(ZetToolsDir)Interface2012\\Models\\Панель\\верх_панелька.stl",
		(m_rect.right + m_rect.left) / 2.0, m_rect.bottom - Bolder / 2.0, -Depth, 0,
		m_rect.Width() - 2 * Bolder, Bolder, Depth);
	//левый нижний угол
	m_Main[7] = new CDetail("$(ZetToolsDir)Interface2012\\Models\\Панель\\левый верхний _панелька.stl",
		Bolder / 2.0 + m_rect.left, m_rect.bottom - Bolder / 2.0, -Depth, 0,
		Bolder, Bolder, Depth);			
	//левая часть панели
	m_Main[8] = new CDetail("$(ZetToolsDir)Interface2012\\Models\\Панель\\левая_панелька.stl",
		Bolder / 2.0 + m_rect.left, (m_rect.bottom + m_rect.top) / 2.0, -Depth, 0,
		Bolder, m_rect.Height() - 2 * Bolder, Depth);//*/
#endif
#endif
}
//------------------------------------------------------------------------------
CDMain::~CDMain()
{
	//TRACE("DELETE CDMain %s\n", (CStringA)m_pParamDraw->Name);
#ifdef _GDIPLUS
	DeleteGdiPlusObject(m_pImage);
	if (m_param.pFont)
	{
		delete m_param.pFont;
		m_param.pFont = nullptr;
	}
#else
	//delete m_param.pFont;
	for (int i = 0; i < 9; i++)
	{ 
		delete m_Main[i];
		m_Main[i] = nullptr;
	}
#endif
}
//------------------------------------------------------------------------------
void CDMain::Resize(CRect rect)
{
	m_rect = rect;

	SetWidthBorder(4);

	m_Main[0]->Resize((m_rect.right + m_rect.left) / 2.0, (m_rect.bottom + m_rect.top) / 2.0, -Depth, 0,
		m_rect.Width() - 2 * Bolder, m_rect.Height() - 2 * Bolder, Depth);
	//левый верхний угол
	m_Main[1]->Resize(Bolder / 2.0 + m_rect.left, Bolder / 2.0 + m_rect.top, -Depth, 0,
		Bolder, Bolder, Depth);
	//верхняя часть панели
	m_Main[2]->Resize((m_rect.right + m_rect.left) / 2.0, Bolder / 2.0 + m_rect.top, -Depth, 0,
		m_rect.Width() - 2 * Bolder, Bolder, Depth);
	//правый верхний угол
	m_Main[3]->Resize(m_rect.right - Bolder / 2.0, Bolder / 2.0 + m_rect.top, -Depth, 0,
		Bolder, Bolder, Depth);
	//правая часть панели
	m_Main[4]->Resize(m_rect.right - Bolder / 2.0, (m_rect.bottom + m_rect.top) / 2.0, -Depth, 0,
		Bolder, m_rect.Height() - 2 * Bolder, Depth);
	//правый нижний угол
	m_Main[5]->Resize(m_rect.right - Bolder / 2.0, m_rect.bottom - Bolder / 2.0, -Depth, 0,
		Bolder, Bolder, Depth);
	//нижняя часть панели
	m_Main[6]->Resize((m_rect.right + m_rect.left) / 2.0, m_rect.bottom - Bolder / 2.0, -Depth, 0,
		m_rect.Width() - 2 * Bolder, Bolder, Depth);
	//левый нижний угол
	m_Main[7]->Resize(Bolder / 2.0 + m_rect.left, m_rect.bottom - Bolder / 2.0, -Depth, 0,
		Bolder, Bolder, Depth);
	//левая часть панели
	m_Main[8]->Resize(Bolder / 2.0 + m_rect.left, (m_rect.bottom + m_rect.top) / 2.0, -Depth, 0,
		Bolder, m_rect.Height() - 2 * Bolder, Depth);//*/
	IsChange();
}
//------------------------------------------------------------------------------
CString CDMain::StringFromRes(UINT nID)
{
	CString str(L"");
	if ( !str.LoadStringW(nID) )
		str.Empty();
	return str;
}
//------------------------------------------------------------------------------
void CDMain::Clear (void)
{
	//m_param.pFont->Clear();
	for (int i = 0; i < 9; i++)
		m_Main[i]->Clear();
}
//------------------------------------------------------------------------------
void CDMain::SetTitle(CString str)
{
	if (m_param.Title != str)
	{
		m_param.Title = str;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDMain::SetImageFileName(CString str)
{
	if ( !str.IsEmpty() && (m_param.fileName != str) )
	{
		m_param.fileName = str;
#ifdef _GDIPLUS
		LoadImageFromFile(&m_pImage, str);
#else
		Texture = LoadGLTextures(m_param.fileName, PicSize, m_param.Number_Texture);
#endif	
		IsChange();	
	}
}
//------------------------------------------------------------------------------
void CDMain::LoadIcon (CString str)
{
    HINSTANCE hInstance;
    hInstance = AfxGetInstanceHandle();
	HBITMAP icon = LoadBitmap(hInstance, L"Icon.bmp");
    if (icon)
		TRACE(L"sdjvaskdv\n");
	m_param.DrawIcon = true;
	PicSizeIcon[0] = PicSizeIcon[1] = 32;
//	a.LoadBitmapW(IDB_BITMAP1);
//	a.CreateBitmap(32,32,10,10,
//	Icon = LoadGLTextures(L"res\\Icon.bmp", PicSizeIcon);
	Icon = LoadGLTextures(L"res\\Icon.bmp", PicSizeIcon, 0);//res\\Icon.bmp
}
//------------------------------------------------------------------------------
void CDMain::SetWidthBorder(int val)
{
	m_param.WidthBorder = val;
	DefineRect2(&m_rect, &m_rect2, val);
	IsChange();
}
//------------------------------------------------------------------------------
void CDMain::SetSizeIcon(int val)
{
	if ( (val >= 0) && (val <= 32) && (m_param.SizeIcon != val) )
	{
		m_param.SizeIcon = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDMain::SetWidthHMME(int val)
{
	if (m_param.WidthHMME != val)
	{
		m_param.WidthHMME = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDMain::SetColorBorderStart(COLORREF val)
{
	if (m_param.ColorBorderStart != val)
	{
		m_param.ColorBorderStart = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDMain::SetColorBorderEnd(COLORREF val)
{
	if (m_param.ColorBorderEnd != val)
	{
		m_param.ColorBorderEnd = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDMain::SetMainWindow(bool val)
{
	if (m_param.MainWindow != val)
	{
		m_param.MainWindow = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
#ifdef _GDIPLUS
void CDMain::SetGradientMode(LinearGradientMode val)
{
	if (m_param.GradientMode != val)
	{
		m_param.GradientMode = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDMain::OnDraw()
{// Главный контейнер всегда отображается, поэтому без if (m_param.bVisible)
	Graphics *pGraphics = *((Graphics**)m_ppGraphics);
	if (m_param.MainWindow)
	{
		// граничная рамка с цветовым градиентом ----------------------------
		if (m_param.SizeIcon)
			FillRoundRectLinearGradient(pGraphics, &m_rect, 2*m_param.WidthBorder,
			m_param.ColorBorderStart, m_param.ColorBorderEnd,
			m_param.GradientMode, &m_GraphPath);
		else
			FillRectLinearGradient(pGraphics, &m_rect,
			m_param.ColorBorderStart, m_param.ColorBorderEnd,
			m_param.GradientMode);


		// прорисовка картинки с закруглёнными границами --------------------
		RoundRectInGraphicsPath(&m_GraphPath, &m_rect2, 2*m_param.WidthBorder);
		if (m_pImage)
		{
			TextureBrush tt_brush(m_pImage);
			pGraphics->FillPath(&tt_brush, &m_GraphPath);
		}
	}
	else
	{
		if (m_param.WindowParam)
		{
			m_rect2.left = m_rect.left;
			if (m_param.SizeIcon)
				FillRoundRectLinGradLeft(pGraphics, &m_rect, 2*m_param.WidthBorder,
				m_param.ColorBorderStart, m_param.ColorBorderEnd,
				m_param.GradientMode, &m_GraphPath);
			else
				FillRectLinearGradient(pGraphics, &m_rect,
				m_param.ColorBorderStart, m_param.ColorBorderEnd,
				m_param.GradientMode);
			RoundRectInGraphicsPathLeft(&m_GraphPath, &m_rect2, 2*m_param.WidthBorder);
			if (m_pImage)
			{
				TextureBrush tt_brush(m_pImage);
				pGraphics->FillPath(&tt_brush, &m_GraphPath);
			}
		}
		else
		{
			m_rect2.top = m_rect.top;
			if (m_param.SizeIcon)
				FillRoundRectLinGradDown(pGraphics, &m_rect, 2*m_param.WidthBorder,
				m_param.ColorBorderStart, m_param.ColorBorderEnd,
				m_param.GradientMode, &m_GraphPath);
			else
				FillRectLinearGradient(pGraphics, &m_rect,
				m_param.ColorBorderStart, m_param.ColorBorderEnd,
				m_param.GradientMode);
			RoundRectInGraphicsPathDown(&m_GraphPath, &m_rect2, 2*m_param.WidthBorder);
			if (m_pImage)
			{
				TextureBrush tt_brush(m_pImage);
				pGraphics->FillPath(&tt_brush, &m_GraphPath);
			}
		}
	}
	float left = m_param.PointIcon.x + 1.1f * m_param.SizeIcon;
	DrawString(pGraphics, m_param.Title, m_param.pFont, 
		RectF( left, (float)m_param.PointIcon.y,
		m_rect2.Width() - left - m_param.WidthBorder,
		(float)m_param.SizeIcon ),
		StringAlignmentNear, StringAlignmentCenter);

	// иконка программы рисуется в функции CCustomMain::OnDraw() т.к. иконка
}	// берётся из ресурсов, о которых данный класс не имеет права ничего знать!
//------------------------------------------------------------------------------
#else
void CDMain::InitOGL(){
	CRect rect(0, 0, 0, 0);
    if (!bSetupPixelFormat())
        return;
    hrc = wglCreateContext(pDC);	
    ASSERT(hrc != NULL);
    wglMakeCurrent(pDC, hrc);

    //GetClientRect(&rect);
    GetClientRect(m_param.hWnd, &rect);
    glViewport(0, 0, rect.right, rect.bottom);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    //glPolygonMode(GL_FRONT, GL_LINE);
    //glPolygonMode(GL_BACK, GL_LINE);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_FRONT);
    glEnable(GL_NORMALIZE);
	//	включаем массивы отображения
    glMatrixMode(GL_PROJECTION);
    glOrtho(0, rect.right, rect.bottom, 0, -100.0, 100.0);
	//	для вызова следующей функции необходим OpenGL версии 1.2 или выше
	//glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
    Lightning(true);
    glMatrixMode(GL_MODELVIEW);
    //glActiveTextureARB		= (PFNGLACTIVETEXTUREARBPROC)		wglGetProcAddress("glActiveTextureARB");
    //glClientActiveTextureARB = (PFNGLCLIENTACTIVETEXTUREARBPROC)	wglGetProcAddress("glClientActiveTextureARB");
    //GLenum glew_status = glewInit();
    //if(glew_status != GLEW_OK) 
    //{
    //    //! GLEW не проинициализировалась
    //    TRACE("Error: %s\n", glewGetErrorString(glew_status));
    //}
    //glewExperimental = GL_TRUE;
	//FreeCurrentContext();
}
//------------------------------------------------------------------------------
BOOL CDMain::bSetupPixelFormat()
{
    static PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
        1,                              // version number
        PFD_DRAW_TO_WINDOW |            // support window
        PFD_SUPPORT_OPENGL |          // support OpenGL
        PFD_DOUBLEBUFFER,             // double buffered
        PFD_TYPE_RGBA,                  // RGBA type
        24,                             // 24-bit color depth
        0, 0, 0, 0, 0, 0,               // color bits ignored
        0,                              // no alpha buffer
        0,                              // shift bit ignored
        0,                              // no accumulation buffer
        0, 0, 0, 0,                     // accum bits ignored
        32,                             // 32-bit z-buffer
        0,                              // no stencil buffer
        0,                              // no auxiliary buffer
        PFD_MAIN_PLANE,                 // main layer
        0,                              // reserved
        0, 0, 0                         // layer masks ignored
    };
    int pixelformat;

    if ( (pixelformat = ChoosePixelFormat(pDC, &pfd)) == 0 )
        return FALSE;

    if (SetPixelFormat(pDC, pixelformat, &pfd) == FALSE)
        return FALSE;

    return TRUE;
}
//------------------------------------------------------------------------------
void CDMain::Lightning(bool enable)
{
    if (enable)
    {
        glLightfv(GL_LIGHT0, GL_AMBIENT,  m_param.light_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE,  m_param.light_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, m_param.light_specular);
        glLightfv(GL_LIGHT0, GL_POSITION, m_param.light_position);

        glEnable(GL_LIGHTING);		//	включаем режим расчёта освещения
        glEnable(GL_LIGHT0);		//	включаем источник света 0
    }
    else
    {
        glDisable(GL_LIGHTING);		//	выключаем режим расчёта освещения
        glDisable(GL_LIGHT0);		//	выключаем источник света 0
    }
}
//------------------------------------------------------------------------------
void CDMain::OnDraw()
{// Главный контейнер всегда отображается, поэтому без if (m_param.bVisible)

	CRect clientRect(0, 0, 0, 0);
	int viewport[4];
	GetClientRect(m_param.hWnd, &clientRect);
	glGetIntegerv(GL_VIEWPORT, viewport);
	//TRACE("Width = %d | Height = %d \n", clientRect.right, clientRect.bottom);
	if (viewport[0] != clientRect.left || viewport[1] != clientRect.top
		|| viewport[2] != clientRect.right || viewport[3] != clientRect.bottom)
	{
		glViewport(0, 0, clientRect.right, clientRect.bottom);
		int a = glGetError();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, clientRect.right, clientRect.bottom, 0, -100.0, 100.0);
		glMatrixMode(GL_MODELVIEW);
	}
		//glGetIntegerv(GL_VIEWPORT, viewport);
		//TRACE("Width = %d | Height = %d \n", viewport[2] - viewport[0], viewport[3] - viewport[1]);

	glPushAttrib(GL_LIGHTING_BIT | GL_TEXTURE_BIT);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT,   m_param.mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE,   m_param.mat_diffuse);
	glMaterialfv(GL_FRONT, GL_EMISSION,  m_param.mat_emission);
	glMaterialfv(GL_FRONT, GL_SPECULAR,  m_param.mat_specular);
	glMaterialf (GL_FRONT, GL_SHININESS, m_param.mat_shininess);
	
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	/*glEnable(GL_STENCIL_TEST);
	glClearStencil(0);
	glStencilMask(1);

	glStencilFunc(GL_ALWAYS, 2, 1);
	glStencilOp(GL_KEEP, GL_REPLACE, GL_KEEP); 
	glStencilFunc(GL_NEVER, 1, 1);
	glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);
	glColor3f(0.0f, 1.0f, 0.0f);
	for (int i = 0; i < Bolder; i++)
	{
		glBegin(GL_POLYGON);
		glVertex3f(rect2[i].left, rect2[i].top, 0.0f);
		glVertex3f(rect2[i].right, rect2[i].top, 0.0f);
		glVertex3f(rect2[i].right, rect2[i].bottom, 0.0f);
		glVertex3f(rect2[i].left, rect2[i].bottom, 0.0f);
		glEnd();
	}
	glStencilFunc(GL_EQUAL, 2, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);*/

#ifdef _ACTIVEX2012
	glEnable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHTING);
	glColor3ub(236, 233, 216);
	glBegin(GL_POLYGON);
	glVertex3f(m_rect.left, m_rect.top, -5.0f);
	glVertex3f(m_rect.right, m_rect.top, -5.0f);
	glVertex3f(m_rect.right, m_rect.bottom, -5.0f);
	glVertex3f(m_rect.left, m_rect.bottom, -5.0f);
	glEnd();
	glEnable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
#else
	glBindTexture(GL_TEXTURE_2D, Texture);
	for (int i = 0; i < QNT_DETAILS; i++)
	{
        m_Main[i]->CalcTexCoors(PicSize[0], PicSize[1]);
        m_Main[i]->Draw_texture(m_Main[i]->ReadFacet(), m_Main[i]->GetQntPlanes(), PicSize, true);
        //TRACE("Main Panel Detail = %d\n", m_Main[i]->GetQntPlanes());
    }
	glEnable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
#endif

	//glDisable(GL_STENCIL_TEST);
	//m_Main_change->SetOffset((rect2[0].right + rect2[0].left)/2, (rect2[0].bottom + rect2[0].top)/2, -1);
	//m_Main_change->SetScale(rect2[0].Width(), rect2[0].Height(), 0.5);
	//m_Main_change->Draw_texture(m_Main_change->ReadFacet(), m_Main_change->GetQntPlanes(), PicSize, true);
	//искривление
	//glColor3f(1.0f,1.0f,1.0f);
	float left = m_param.PointIcon.x + 1.1f * m_param.SizeIcon;
	float right = m_rect2.Width() - left - m_param.WidthBorder - 3*20;
	float top = (float)m_param.PointIcon.y;
	float bottom = top + (float)m_param.SizeIcon;
	if (!m_param.DrawIcon)
		left -= 15;
	else
		left += 5;
	const CString tmp = m_param.Title;
    //tmp = L"a";
	m_param.pFont->DrawString(CRect( left + 5, top, right + 50, bottom - 5), m_param.Title);
	glPopAttrib();
	if (m_param.DrawIcon)
	{
		glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_TEXTURE_BIT);
		glPushMatrix();
		glLoadIdentity();
		glColor3f(1.0f,1.0f,1.0f);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, Icon);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 1);
		glVertex3f(19, 19, 0);
		glTexCoord2f(0, 0);
		glVertex3f(19, 37, 0);
		glTexCoord2f(1, 0);
		glVertex3f(37, 37, 0);
		glTexCoord2f(1, 1);
		glVertex3f(37, 19, 0);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_COLOR_MATERIAL);
		glPopMatrix();
		glPopAttrib();
		glFinish();
	}
}	// берётся из ресурсов, о которых данный класс не имеет права ничего знать
//------------------------------------------------------------------------------
void CDMain::SetColor(GLfloat *ma, GLfloat *md,	GLfloat *me, GLfloat *msp, GLfloat msh)
{
	for (int i = 0; i < 4; i++)
	{
		m_param.mat_ambient[i] = ma[i];
		m_param.mat_diffuse[i] = md[i];
		m_param.mat_emission[i] = me[i];
		m_param.mat_specular[i] = msp[i];
	}
	m_param.mat_shininess = msh;
}
//------------------------------------------------------------------------------
void CDMain::SetRectMask(CRect rect_mask)
{
	rect2[N] = rect_mask;
	N++;
}
//------------------------------------------------------------------------------
void CDMain::SethWnd(HWND hWnd)
{
    m_param.hWnd = hWnd;
    pDC = GetDC(m_param.hWnd);
    InitOGL();
    SetImageFileName(_T("$(ZetToolsDir)Interface2012\\Textures\\Patterns\\metal_mart14.bmp"));
    IsChange();
}
//------------------------------------------------------------------------------
void CDMain::SaveParameters(SimpleXML *pSimpleXML)
{
    pSimpleXML->Create(L"Config");
    hXml hInterfaceRoot = pSimpleXML->AddElement(L"Interface", pSimpleXML->GetRootNode());
    hXml hLightingRoot = pSimpleXML->AddElement(L"Lighting", hInterfaceRoot);

    hXml hAmbientRoot = pSimpleXML->AddElement(L"Ambient", hLightingRoot);
    pSimpleXML->SetAttributeValue(hAmbientRoot, L"red", CComVariant(m_param.light_ambient[0]));
    pSimpleXML->SetAttributeValue(hAmbientRoot, L"green", CComVariant(m_param.light_ambient[1]));
    pSimpleXML->SetAttributeValue(hAmbientRoot, L"blue", CComVariant(m_param.light_ambient[2]));
    pSimpleXML->SetAttributeValue(hAmbientRoot, L"alpha", CComVariant(m_param.light_ambient[3]));

    hXml hDiffuseRoot = pSimpleXML->AddElement(L"Diffuse", hLightingRoot);
    pSimpleXML->SetAttributeValue(hDiffuseRoot, L"red", CComVariant(m_param.light_diffuse[0]));
    pSimpleXML->SetAttributeValue(hDiffuseRoot, L"green", CComVariant(m_param.light_diffuse[1]));
    pSimpleXML->SetAttributeValue(hDiffuseRoot, L"blue", CComVariant(m_param.light_diffuse[2]));
    pSimpleXML->SetAttributeValue(hDiffuseRoot, L"alpha", CComVariant(m_param.light_diffuse[3]));

    hXml hSpecularRoot = pSimpleXML->AddElement(L"Specular", hLightingRoot);
    pSimpleXML->SetAttributeValue(hSpecularRoot, L"red", CComVariant(m_param.light_specular[0]));
    pSimpleXML->SetAttributeValue(hSpecularRoot, L"green", CComVariant(m_param.light_specular[1]));
    pSimpleXML->SetAttributeValue(hSpecularRoot, L"blue", CComVariant(m_param.light_specular[2]));
    pSimpleXML->SetAttributeValue(hSpecularRoot, L"alpha", CComVariant(m_param.light_specular[3]));

    hXml hPositionRoot = pSimpleXML->AddElement(L"Position", hLightingRoot);
    pSimpleXML->SetAttributeValue(hPositionRoot, L"position_x", CComVariant(m_param.light_position[0]));
    pSimpleXML->SetAttributeValue(hPositionRoot, L"position_y", CComVariant(m_param.light_position[1]));
    pSimpleXML->SetAttributeValue(hPositionRoot, L"position_z", CComVariant(m_param.light_position[2]));
    pSimpleXML->SetAttributeValue(hPositionRoot, L"position_type", CComVariant(m_param.light_position[3]));

    hXml hMainPanelRoot = pSimpleXML->AddElement(L"MainPanel", hInterfaceRoot);

    hXml hTextureRoot = pSimpleXML->AddElement(L"Texture", hMainPanelRoot);
	pSimpleXML->SetAttributeValue(hTextureRoot, L"ZISNumber_Texture", CComVariant(m_param.Number_Texture));
    pSimpleXML->SetAttributeValue(hTextureRoot, L"path", CComVariant(m_param.fileName));

    hXml hMaterialRoot = pSimpleXML->AddElement(L"Material", hMainPanelRoot);

    hXml hAmbientPanelRoot = pSimpleXML->AddElement(L"Ambient", hMaterialRoot);
    pSimpleXML->SetAttributeValue(hAmbientPanelRoot, L"red", CComVariant(m_param.mat_ambient[0]));
    pSimpleXML->SetAttributeValue(hAmbientPanelRoot, L"green", CComVariant(m_param.mat_ambient[1]));
    pSimpleXML->SetAttributeValue(hAmbientPanelRoot, L"blue", CComVariant(m_param.mat_ambient[2]));
    pSimpleXML->SetAttributeValue(hAmbientPanelRoot, L"alpha", CComVariant(m_param.mat_ambient[3]));

    hXml hDiffusePanelRoot = pSimpleXML->AddElement(L"Diffuse", hMaterialRoot);
    pSimpleXML->SetAttributeValue(hDiffusePanelRoot, L"red", CComVariant(m_param.mat_diffuse[0]));
    pSimpleXML->SetAttributeValue(hDiffusePanelRoot, L"green", CComVariant(m_param.mat_diffuse[1]));
    pSimpleXML->SetAttributeValue(hDiffusePanelRoot, L"blue", CComVariant(m_param.mat_diffuse[2]));
    pSimpleXML->SetAttributeValue(hDiffusePanelRoot, L"alpha", CComVariant(m_param.mat_diffuse[3]));

    hXml hSpecularPanelRoot = pSimpleXML->AddElement(L"Specular", hMaterialRoot);
    pSimpleXML->SetAttributeValue(hSpecularPanelRoot, L"red", CComVariant(m_param.mat_specular[0]));
    pSimpleXML->SetAttributeValue(hSpecularPanelRoot, L"green", CComVariant(m_param.mat_specular[1]));
    pSimpleXML->SetAttributeValue(hSpecularPanelRoot, L"blue", CComVariant(m_param.mat_specular[2]));
    pSimpleXML->SetAttributeValue(hSpecularPanelRoot, L"alpha", CComVariant(m_param.mat_specular[3]));

    hXml hEmissionPanelRoot = pSimpleXML->AddElement(L"Emission", hMaterialRoot);
    pSimpleXML->SetAttributeValue(hEmissionPanelRoot, L"red", CComVariant(m_param.mat_emission[0]));
    pSimpleXML->SetAttributeValue(hEmissionPanelRoot, L"green", CComVariant(m_param.mat_emission[1]));
    pSimpleXML->SetAttributeValue(hEmissionPanelRoot, L"blue", CComVariant(m_param.mat_emission[2]));
    pSimpleXML->SetAttributeValue(hEmissionPanelRoot, L"alpha", CComVariant(m_param.mat_emission[3]));

    hXml hShininessPanelRoot = pSimpleXML->AddElement(L"Shininess", hMaterialRoot);
    pSimpleXML->SetAttributeValue(hShininessPanelRoot, L"blink_coefficient", CComVariant(m_param.mat_shininess));
}
//------------------------------------------------------------------------------
void CDMain::LoadParameters(SimpleXML *pSimpleXML)
{
    VARIANT vAtrrib;
    hXml hRoot = pSimpleXML->GetRootNode();
    hXml hInterfaceRoot = pSimpleXML->GetChildNode(hRoot);
    hXml hLightningRoot = pSimpleXML->GetChildNode(hInterfaceRoot);

    //цвет фонового освещения
    hXml hAmbientRoot = pSimpleXML->GetChildNode(hLightningRoot);
    pSimpleXML->GetAttributeValue(hAmbientRoot, L"red", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.light_ambient[0] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hAmbientRoot, L"green", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.light_ambient[1] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hAmbientRoot, L"blue", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.light_ambient[2] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hAmbientRoot, L"alpha", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.light_ambient[3] = _wtof(vAtrrib.bstrVal);

    //цвет рассеянного освещения
    hXml hDiffuseRoot = pSimpleXML->GetNextNode(hAmbientRoot);
    pSimpleXML->GetAttributeValue(hDiffuseRoot, L"red", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.light_diffuse[0] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hDiffuseRoot, L"green", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.light_diffuse[1] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hDiffuseRoot, L"blue", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.light_diffuse[2] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hDiffuseRoot, L"alpha", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.light_diffuse[3] = _wtof(vAtrrib.bstrVal);

    //цвет зеркального отражения
    hXml hSpecularRoot = pSimpleXML->GetNextNode(hDiffuseRoot);
    pSimpleXML->GetAttributeValue(hSpecularRoot, L"red", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.light_specular[0] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hSpecularRoot, L"green", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.light_specular[1] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hSpecularRoot, L"blue", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.light_specular[2] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hSpecularRoot, L"alpha", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.light_specular[3] = _wtof(vAtrrib.bstrVal);

    //позиция источника света
    hXml hPositionRoot = pSimpleXML->GetNextNode(hSpecularRoot);
    pSimpleXML->GetAttributeValue(hPositionRoot, L"position_x", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.light_position[0] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hPositionRoot, L"position_y", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.light_position[1] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hPositionRoot, L"position_z", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.light_position[2] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hPositionRoot, L"position_type", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.light_position[3] = _wtof(vAtrrib.bstrVal);

    Lightning(true);

    hXml hMainPanelRoot = pSimpleXML->GetNextNode(hLightningRoot);
    hXml hTextureRoot = pSimpleXML->GetChildNode(hMainPanelRoot);
    pSimpleXML->GetAttributeValue(hTextureRoot, L"ZISNumber_Texture", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.Number_Texture = _wtoi(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hTextureRoot, L"path", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            SetImageFileName(CString(vAtrrib.bstrVal));

    //рассеянный цвет материала (цвет материала в тени)
    hXml hMaterialRoot = pSimpleXML->GetNextNode(hTextureRoot);
    hAmbientRoot = pSimpleXML->GetChildNode(hMaterialRoot);
    pSimpleXML->GetAttributeValue(hAmbientRoot, L"red", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_ambient[0] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hAmbientRoot, L"green", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_ambient[1] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hAmbientRoot, L"blue", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_ambient[2] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hAmbientRoot, L"alpha", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_ambient[3] = _wtof(vAtrrib.bstrVal);

    //цвет диффузного отражения материала
    hDiffuseRoot = pSimpleXML->GetNextNode(hAmbientRoot);
    pSimpleXML->GetAttributeValue(hDiffuseRoot, L"red", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_diffuse[0] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hDiffuseRoot, L"green", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_diffuse[1] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hDiffuseRoot, L"blue", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_diffuse[2] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hDiffuseRoot, L"alpha", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_diffuse[3] = _wtof(vAtrrib.bstrVal);

    //цвет зеркального отражения материала
    hSpecularRoot = pSimpleXML->GetNextNode(hDiffuseRoot);
    pSimpleXML->GetAttributeValue(hSpecularRoot, L"red", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_specular[0] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hSpecularRoot, L"green", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_specular[1] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hSpecularRoot, L"blue", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_specular[2] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hSpecularRoot, L"alpha", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_specular[3] = _wtof(vAtrrib.bstrVal);

    //интенсивность излучаемого света материала
    hXml hEmissionRoot = pSimpleXML->GetNextNode(hSpecularRoot);
    pSimpleXML->GetAttributeValue(hEmissionRoot, L"red", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_emission[0] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hEmissionRoot, L"green", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_emission[1] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hEmissionRoot, L"blue", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_emission[2] = _wtof(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hEmissionRoot, L"alpha", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_emission[3] = _wtof(vAtrrib.bstrVal);

    //определяет степень зеркального отражения материала
    hXml hShininessRoot = pSimpleXML->GetNextNode(hEmissionRoot);
    pSimpleXML->GetAttributeValue(hShininessRoot, L"blink_coefficient", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            m_param.mat_shininess = _wtof(vAtrrib.bstrVal);

}
//------------------------------------------------------------------------------
void CDMain::GetCurrentContext()
{
	wglMakeCurrent(pDC, hrc);
}
//------------------------------------------------------------------------------
void CDMain::FreeCurrentContext()
{
	if (pDC)
		wglMakeCurrent(pDC, NULL);
}
#endif