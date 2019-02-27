#include "stdafx.h"
#include <wchar.h>
#include <math.h>
#include <Interface2012\Custom\CFonts.h>
#pragma comment (lib, "freetype.lib")
#pragma comment(lib, "ftgl.lib")


char* CDFonts::scat(char *d, char *s)
{
	char *r = (char*)calloc(strlen(d) + strlen(s), sizeof(char));
	strcpy(r, d);
	strcpy(r + strlen(d), s);
	return r;
}
//------------------------------------------------------------------------------
CDFonts::CDFonts()
	: m_bSetCeils(false)
{
	AligmentHorizontal = AligmentVertical = StrAlignmentNear;

	Init();
}
//------------------------------------------------------------------------------
CDFonts::CDFonts(ParamFonts paramFont)
{
	AligmentHorizontal = AligmentVertical = StrAlignmentNear;
	m_param = paramFont;
	Init();
}
//------------------------------------------------------------------------------
CDFonts::CDFonts(char* font_name, float font_size, FontsStyle font_style, COLORREF font_color)
{
	AligmentHorizontal = AligmentVertical = StrAlignmentNear;
	m_param.Name = font_name;
	m_param.Size = font_size;
	m_param.Style = font_style;
	m_param.Color = font_color;
	Init();
	m_param.MonoSpace = checkMonoSpace();
}
//------------------------------------------------------------------------------
CDFonts::~CDFonts()
{
	if (fonts)
	{
		delete fonts;
		fonts = nullptr;
	}
}
//------------------------------------------------------------------------------
void CDFonts::Clear(void)
{
	counter--;
	//TRACE (L" coouner CDFonts = %i\n", counter);
	delete fonts;
	fonts = nullptr;
}
//------------------------------------------------------------------------------
int CDFonts::counter = 0;
//------------------------------------------------------------------------------
void CDFonts::Init()
{
	fonts = nullptr;
	counter++;
	char *ch = nullptr;
	char ch1[100] = "C:\\Windows\\Fonts\\";
	char ch2[100] = "";
	char ch3[100] = "";;
	if (m_param.Style == FontsStyleRegular) //обычный	//	ch2 = (".ttf");
		strcpy(ch2, ".ttf");
	if (m_param.Style == FontsStyleBold) //жирный	//	ch2 = ("b.ttf");
		strcpy(ch2, "b.ttf");
	if (m_param.Style == FontsStyleItalic) //курсив	//	ch2 = ("i.ttf");
		strcpy(ch2, "i.ttf");
	if (m_param.Style == FontsStyleBoldItalic) //жирный курсив	//	ch2 = ("z.ttf");
		strcpy(ch2, "z.ttf");
	strcat(strcat(ch1, m_param.Name), ch2);
	this->fonts = new FTGLPixmapFont((const char*)ch1);
	fonts->FaceSize(m_param.Size);
	fonts->Depth(10);
}
//------------------------------------------------------------------------------
bool CDFonts::checkMonoSpace()
{
	FTBBox bbox1, bbox2;
	FTPoint p1, p2;
	bbox1 = fonts->BBox(L"0", 1, p1, p2);
	bbox2 = fonts->BBox(L".", 1, p1, p2);
	float width1 = bbox1.Upper().Xf() - bbox1.Lower().Xf();
	float width2 = bbox2.Upper().Xf() - bbox2.Lower().Xf();
	if (fabsf(width1 - width2) < 0.001f)
		return true;
	else
		return false;
}
//------------------------------------------------------------------------------
void CDFonts::DrawString(CRect rect, const wchar_t* str, StringAlignmentOGL AligmentHor, StringAlignmentOGL AligmentVert)
{
	AligmentHorizontal = AligmentHor;
	AligmentVertical = AligmentVert;
	DrawString(rect, str);
}
//------------------------------------------------------------------------------
void CDFonts::DrawString(CRect rect, const wchar_t* str)
{

	if ((wcslen(str) != '\0') && ((rect.right - rect.left) != 0) && ((rect.bottom - rect.top) != 0))
	{
		float X(0.f), Y(0.f);
		float length(0.f), length_prev(0.f), length_all(0.f);
		float height(0);
		int i(0); int j(0); int k(0); int start(0); int num_string(0);
		FTBBox bbox;
		FTPoint p1, p2;

		bbox = fonts->BBox(L"W", 1, p1, p2);
		height = (bbox.Upper().Yf() - bbox.Lower().Yf());

		bbox = fonts->BBox(&(str[0]), wcslen(str), p1, p2);
		length_all = bbox.Upper().Xf() - bbox.Lower().Xf();

		float fDiv = length_all / float(rect.Width());
		num_string = int(fDiv) + 1;
		//если строка не вмещается в выделенную область, то размер текста уменьшаем
		while ((num_string*height>rect.Height())&&(m_param.Size - k>0))
		{
			k++;
			fonts->FaceSize(m_param.Size-k);
			bbox = fonts->BBox(L"W", 1, p1, p2);
			height = (bbox.Upper().Yf() - bbox.Lower().Yf());
			bbox = fonts->BBox(&(str[0]), wcslen(str), p1, p2);
			length_all = bbox.Upper().Xf() - bbox.Lower().Xf();
			num_string = int(length_all/rect.Width()) + 1;
		}
		m_param.Size -= k;

		//CString g;
		//if (m_param.Size < 14)
		//{
		//	g = CutString(str);
		//	str = g;
		//}

		do 
		{
			length_prev = 0;
			length = 0;
			i = 0;
			while ((length < rect.Width())&&(i <= wcslen(str)))
			{
				length_prev = length;
				bbox = fonts->BBox(&(str[start]), i+1, p1, p2);
				length = bbox.Upper().Xf() - bbox.Lower().Xf();
				i++;
			}
			wchar_t* tmpStr = new wchar_t[i+1];
			wcsncpy_s(tmpStr, i, &(str[start]), i-1);
			start = start + i-1;

			if (AligmentHorizontal == StrAlignmentNear)
				X = rect.left;
			if (AligmentVertical == StrAlignmentNear)
				Y = rect.top + height*(j+1) + j;
			if (AligmentHorizontal == StrAlignmentFar)
				X = rect.right - length_prev;
			if (AligmentVertical == StrAlignmentFar)
				Y = rect.bottom + height*j + j;
			if (AligmentHorizontal == StrAlignmentCenter)
				X = (rect.left + rect.right - length_prev)/2;
			if (AligmentVertical == StrAlignmentCenter)
				Y = (rect.top + rect.bottom - (height + 1)*num_string)/2 + height*(j+1) + 1;

			glColor3ub(GetRValue(m_param.Color), GetGValue(m_param.Color), GetBValue(m_param.Color));
			glPushAttrib(GL_LIST_BIT | GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_DEPTH_BUFFER_BIT);
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_LIGHTING);
//			glEnable(GL_COLOR_MATERIAL);
			glPushMatrix();
			glRasterPos2f(X, Y);
			
			fonts->Render(tmpStr);
			glPopMatrix();
			glPopAttrib();
			delete [] tmpStr;
			tmpStr = nullptr;
			j++;
		}  while (start <= wcslen(str));
	}
	//fonts->FaceSize(m_param.Size);

	//CString s = CutString(str);
}
//------------------------------------------------------------------------------
void CDFonts::DrawNumber(CRect rect, const wchar_t* text, StringAlignmentOGL AligmentHor, StringAlignmentOGL AligmentVert)
{
	AligmentHorizontal = AligmentHor;
	AligmentVertical = AligmentVert;
	if (!m_bSetCeils)
		DrawNumber(rect, text);
	else
		DrawNumberOnCeils(rect, text);
}
//------------------------------------------------------------------------------
void CDFonts::DrawNumberOnCeils(CRect rect, const wchar_t *text)
{
	int len = wcslen(text);
	if (len != 0)
	{
		FTBBox bbox;
		FTPoint p1, p2;
		float height(0.f), width(0.f);
		float length_all(0.f);
		float X(0.f), Y(0.f);
		float wKoef(1.2f);
		int k(0);
		int iwidth(0);
		int iX(0);
		wchar_t point[2] = L".";
		int pos = (int)(wcsstr(text, point) - text);

		bbox = fonts->BBox(L"0", 1, p1, p2);
		height = bbox.Upper().Yf() - bbox.Lower().Yf();
		while ( (height > rect.Height()) && (m_param.Size - k > 0) )
		{
			k++;
			fonts->FaceSize(m_param.Size - k);
			bbox = fonts->BBox(L"0", 1, p1, p2);
			height = bbox.Upper().Yf() - bbox.Lower().Yf();
		}

		width = bbox.Upper().Xf() - bbox.Lower().Xf();
		iwidth = ceil(width) + 2;
		while ( (width*len*wKoef > rect.Width()) && (m_param.Size - k > 0) )
		{
			k++;
			fonts->FaceSize(m_param.Size - k);
			bbox = fonts->BBox(L"0", 1, p1, p2);
			width = bbox.Upper().Xf() - bbox.Lower().Xf();
		}
		m_param.Size -= k;

		switch (AligmentHorizontal)
		{
			case StrAlignmentNear   : X = float(rect.left); break;
			case StrAlignmentFar    : X = float(rect.right) - width*len*wKoef; break;
			case StrAlignmentCenter : X = (float(rect.left + rect.right) - width*len*wKoef)/2.0f;
		}
		iX = ceil(X);
		switch (AligmentVertical)
		{
			case StrAlignmentNear   : Y = float(rect.top) + height; break;
			case StrAlignmentFar    : Y = float(rect.bottom); break;
			case StrAlignmentCenter : Y = (float(rect.top + rect.bottom) + height)/2.0f;
		}
		glColor3ub(GetRValue(m_param.Color), GetGValue(m_param.Color), GetBValue(m_param.Color));
		glPushAttrib(GL_LIST_BIT | GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glPushMatrix(); 
		m_rect = rect;
		m_rect.top = m_rect.top + (m_rect.bottom - m_rect.top)/7;
		m_rect.bottom = m_rect.bottom - (m_rect.bottom - m_rect.top)/8;
		for (k = 0; k < len; k++)
		{
			m_rect.left = iX + k*iwidth;
			m_rect.right = m_rect.left + iwidth - 1;
			FillRectSolid(&m_rect, 10, RGB(32,32,32));
		}
		m_rect = rect;
		float fraq;
		for (k = 0; k < len; k++)
		{
			fraq = width - floor(width);
			m_rect.left = iX + k*iwidth - int(fraq + 0.5f);
			glColor3ub(GetRValue(m_param.Color), GetGValue(m_param.Color), GetBValue(m_param.Color));
			glRasterPos2i(m_rect.left, static_cast<int>(Y));
			fonts->Render(&(text[k]), 1);

		}
		glPopMatrix();
		glPopAttrib();
	}
}
//------------------------------------------------------------------------------
void CDFonts::FillRectSolid(CRect *pRect, float Z, COLORREF color)
{
	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glBegin(GL_POLYGON);
	glColor3ub(GetRValue(color), GetGValue(color), GetBValue(color));
	glVertex3f(pRect->left, pRect->bottom, Z);
	glVertex3f(pRect->left, pRect->top, Z);
	glVertex3f(pRect->right, pRect->top, Z);
	glVertex3f(pRect->right, pRect->bottom, Z);
	glEnd();
	glPopAttrib();
}
//------------------------------------------------------------------------------
void CDFonts::DrawNumber(CRect rect, const wchar_t *text)
{
	int len = wcslen(text);
	if (len != 0)
	{
		FTBBox bbox;
		FTPoint p1, p2;
		float height(0.f), width(0.f);
		float length_all(0.f);
		float X(0.f), Y(0.f);
		float wKoef(1.2f);
		int k(0);
		wchar_t point[2] = L".";
		int pos = (int)(wcsstr(text, point) - text);

		bbox = fonts->BBox(L"0", 1, p1, p2);
		height = bbox.Upper().Yf() - bbox.Lower().Yf();
		while ( (height > rect.Height()) && (m_param.Size - k > 0) )
		{
			k++;
			fonts->FaceSize(m_param.Size - k);
			bbox = fonts->BBox(L"0", 1, p1, p2);
			height = bbox.Upper().Yf() - bbox.Lower().Yf();
		}

		width = bbox.Upper().Xf() - bbox.Lower().Xf();
		while ( (width*len*wKoef > rect.Width()) && (m_param.Size - k > 0) )
		{
			k++;
			fonts->FaceSize(m_param.Size - k);
			bbox = fonts->BBox(L"0", 1, p1, p2);
			width = bbox.Upper().Xf() - bbox.Lower().Xf();
		}
		m_param.Size -= k;

		switch (AligmentHorizontal)
		{
			case StrAlignmentNear   : X = float(rect.left); break;
			case StrAlignmentFar    : X = float(rect.right) - width*len*wKoef; break;
			case StrAlignmentCenter : X = (float(rect.left + rect.right) - width*len*wKoef)/2.0f;
		}
		switch (AligmentVertical)
		{
			case StrAlignmentNear   : Y = float(rect.top) + height; break;
			case StrAlignmentFar    : Y = float(rect.bottom); break;
			case StrAlignmentCenter : Y = (float(rect.top + rect.bottom) + height)/2.0f;
		}
		glColor3ub(GetRValue(m_param.Color), GetGValue(m_param.Color), GetBValue(m_param.Color));
		glPushAttrib(GL_LIST_BIT | GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glPushMatrix(); 
		for (k = 0; k < len; k++)
		{
//			TRACE (L" sizeof = %f\n", X + (k)*width*wKoef);
			if (k == pos && !m_param.MonoSpace)
				glRasterPos2f(X + (k+0.28f)*ceil(width)*wKoef, Y);
			else
				glRasterPos2f(X + k*ceil(width)*wKoef, Y);
			fonts->Render(&(text[k]), 1);
		}
		glPopMatrix();
		glPopAttrib();
	}
}
//------------------------------------------------------------------------------
void CDFonts::SetName(char* newVal)
{
	if (m_param.Name != newVal)
	{
		delete fonts;
		fonts = nullptr;
		m_param.Name = newVal;
		Init();
		m_param.MonoSpace = checkMonoSpace();
	}
}
//------------------------------------------------------------------------------
void CDFonts::SetSize(float newVal)
{
	if (m_param.Size != newVal)
	{
		m_param.Size = newVal;
		fonts->FaceSize(m_param.Size);
	}
}
//------------------------------------------------------------------------------
void CDFonts::SetStyle(FontsStyle newVal)
{
	if (m_param.Style != newVal)
	{
		delete fonts;
		fonts = nullptr;
		m_param.Style = newVal;
		Init();
	}
}
//------------------------------------------------------------------------------
void CDFonts::SetAligmentHorizontal(StringAlignmentOGL newVal)
{
	if (AligmentHorizontal != newVal)
	{
		AligmentHorizontal = newVal;
	}
}
//------------------------------------------------------------------------------
void CDFonts::SetAligmentVertical(StringAlignmentOGL newVal)
{
	if (AligmentVertical != newVal)
	{
		AligmentVertical = newVal;
	}
}
//------------------------------------------------------------------------------
CString CDFonts::CutString(CString val)
{
	CString sTemp1, sTemp2;
	if (val.GetLength() > 3) //сюда будут итак попадать строки длиннее 3х символов. Проверку - убрать.
	{
		for (int i = 0; i < 3; i++)
			sTemp1 += val[i];
	}
	sTemp1 += L"...";
	int count_ = 0; //для подсчета "_" и " "
	int count_b = 0; //для подсчета скобок, чтобы выделять ip в названии компьютера
	for (int i = 0; i < val.GetLength(); i++)
		{
			if (val[i] == (char)95) count_++;
			if (val[i] == (char)32) count_++;
			if (val[i] == (char)40) count_b++;
		}
	if (count_b == 0) //если скобок не обнаружено
		{
			int k = count_;
			int i = 0;
			while (i < val.GetLength())
			{
				char z = val[i];
				if (k < count_)
				{
					sTemp1 += val[i];
				}
				if (z == (char)95 || z == (char)32)
				{
					k--;
				}
				i++;
			}
		}
	else
		{
			int k = count_b;
			int i = 0;
			while (i < val.GetLength() && val[i]!=(char)41)
			{
				char z = val[i];
				if (k < count_b)
				{
					sTemp1 += val[i];
				}
				if (z == (char)40 || z == (char)41)
				{
					k--;
				}
				i++;
			}
		}
	return sTemp1;
}
//------------------------------------------------------------------------------
float CDFonts::CalcSize(CRect rect, const wchar_t *str)
{
	if (wcslen(str) != 0 && (rect.right - rect.left) != 0 && (rect.bottom - rect.top) != 0)
	{
		float X(0.f), Y(0.f);
		float length(0.f), length_prev(0.f), length_all(0.f);
		float height(0);
		int i(0); int j(0); int k(0); int start(0); int num_string = 0;
		FTBBox bbox;
		FTPoint p1, p2;

		bbox = fonts->BBox(L"W", 1, p1, p2);
		height = (bbox.Upper().Yf() - bbox.Lower().Yf());

		bbox = fonts->BBox(&(str[0]), wcslen(str), p1, p2);
		length_all = bbox.Upper().Xf() - bbox.Lower().Xf();

		float fDiv = length_all / float(rect.Width());
		num_string = int(fDiv) + 1;
		//если строка не вмещается в выделенную область, то размер текста уменьшаем
		while ((num_string*height > rect.Height()) && (m_param.Size - k > 0))
		{
			k++;
			fonts->FaceSize(m_param.Size - k);
			bbox = fonts->BBox(L"W", 1, p1, p2);
			height = (bbox.Upper().Yf() - bbox.Lower().Yf());
			bbox = fonts->BBox(&(str[0]), wcslen(str), p1, p2);
			length_all = bbox.Upper().Xf() - bbox.Lower().Xf();
			num_string = int(length_all / rect.Width()) + 1;
		}
		m_param.Size -= k;
	}
	return m_param.Size;
}