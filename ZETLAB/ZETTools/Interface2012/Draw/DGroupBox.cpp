//------------------------------------------------------------------------------
// DGroupBox.cpp : файл реализации
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\Draw\DGroupBox.h"
//------------------------------------------------------------------------------
int g_nGroupBox = 0;		// счётчик
//------------------------------------------------------------------------------
#ifdef _GDIPLUS
CDGroupBox::CDGroupBox(CRect rect, PVOID ppGraphics,
				bool *pFlagIsChange, double *pTimeAnimation) :
				CCustomDraw(rect, ppGraphics, pFlagIsChange, pTimeAnimation)
#else
CDGroupBox::CDGroupBox(CRect rect,
				bool *pFlagIsChange, double *pTimeAnimation) :
				CCustomDraw(rect, pFlagIsChange, pTimeAnimation)
#endif
{
	m_pParamDraw = &m_param;	// обеспечение ОБЩИХ параметров
	m_param.nElement = ++g_nGroupBox;
	SetNameWithNumber(L"GroupBox");
	//TRACE("CREATE CDGroupBox %s\n", (CStringA)m_pParamDraw->Name);
#ifdef _GDIPLUS
	m_param.pFont = new CZFont();
#else
	m_param.pFont = new CDFonts("Arialbd", 12.0 , FontsStyleRegular, m_param.Color);
	m_param.pFont->SetAligmentHorizontal(StrAlignmentCenter);
#endif
}
//------------------------------------------------------------------------------
CDGroupBox::~CDGroupBox()
{
	//TRACE("DELETE CDGroupBox %s\n", (CStringA)m_pParamDraw->Name);
	if (m_param.pFont)
	{
		m_param.pFont->Clear();
		delete m_param.pFont;
	}
}
//------------------------------------------------------------------------------
void CDGroupBox::SetCaption(CString val)
{
	if (m_param.Caption != val)
	{
		m_param.Caption = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDGroupBox::SetCoordZ(float val)
{
	if ( m_param.CoordZ != val)
	{
		m_param.CoordZ = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDGroupBox::SetRadius(int val)
{
	int n;
	if (val < 2)
		n = 2;
	else if (val > 13)
		n = 13;
	else
		n = val;
	if (m_param.Radius != n)
	{
		m_param.Radius = n;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDGroupBox::SetLengthLine0(int val)
{
	int n;
	if (val < m_param.Radius)
		n = m_param.Radius;
	else
		n = val;
	if (m_param.LengthLine0 != n)
	{
		m_param.LengthLine0 = n;		
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDGroupBox::SetColor(COLORREF val)
{
	if (m_param.Color != val)
	{
		m_param.Color = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDGroupBox::SetAligment( StringAlignmentOGL val )
{
	if (m_param.Alignment != val)
	{
		m_param.Alignment = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
#ifdef _GDIPLUS
void CDGroupBox::SetFont(CZFont *pfont)
{
	if (m_param.pFont != pfont)
	{
		m_param.pFont = pfont;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDGroupBox::OnDraw()
{// отображение
	int size, WidthText, HeightText;
	if (m_param.bVisible)
	{
		Graphics *pGraphics = *((Graphics**)m_ppGraphics);
		// длина и ширина текста ---------------------------------------------
		if ( !m_param.Caption.IsEmpty() )
		{
			PointF point((REAL)m_param.LengthLine0, (REAL)m_rect.top);
			RectF rectf;
			pGraphics->MeasureString(m_param.Caption, -1,
				m_param.pFont->GetFont(), point, &rectf);
			WidthText = int(rectf.Width) + 4;
			HeightText = int(rectf.Height) + 1;
			size = m_rect.Height() - 2 * m_param.LengthLine0;
			if (HeightText > size)
				HeightText = size;
		}
		else
		{
			WidthText = HeightText = 0;
		}
		// точки по изображаемой кривой на стыках линий и дуг ----------------
		CRect rect = m_rect;
		rect.top = m_rect.top + HeightText / 2;
		Point p[10], pa[4];
		DefinePointForRoundRect(&rect, p, pa, m_param.Radius);
		p[8].X = m_rect.left + m_param.LengthLine0;
		p[9].X = p[8].X + WidthText;
		p[8].Y = p[9].Y = p[0].Y;

		// рисую не замкнутую кривую по часовой стрелке ----------------------
		Color col;
		col.SetFromCOLORREF(m_param.Color);
		Pen pen(col);
		size = 2 * m_param.Radius;
		pGraphics->DrawLine(&pen, p[9], p[0]);	// горизонт. линия справа от текста
		pGraphics->DrawArc (&pen, pa[0].X, pa[0].Y, size, size, 270, 90);	// правый верхний угол
		pGraphics->DrawLine(&pen, p[1], p[2]);	// правая вертикальная линия
		pGraphics->DrawArc (&pen, pa[1].X, pa[1].Y, size, size, 0, 90);	// правый нижний угол
		pGraphics->DrawLine(&pen, p[3], p[4]);	// нижняя горизонтальная линия
		pGraphics->DrawArc (&pen, pa[2].X, pa[2].Y, size, size, 90, 90);	// левый нижний угол
		pGraphics->DrawLine(&pen, p[5], p[6]);	// левая вертикальная линия
		pGraphics->DrawArc (&pen, pa[3].X, pa[3].Y, size, size, 180, 90);	// левый верхний угол
		pGraphics->DrawLine(&pen, p[7], p[8]);	// горизонтальная линия слева от текста

		// текст -------------------------------------------------------------
		if ( !m_param.Caption.IsEmpty() )
			DrawString(pGraphics, m_param.Caption, m_param.pFont,
			RectF( (float)p[8].X, (float)m_rect.top,
			(float)WidthText, (float)HeightText ),
			StringAlignmentCenter, StringAlignmentCenter);
	}//	if (m_param.bVisible)
}
#else
//------------------------------------------------------------------------------
void CDGroupBox::SetFont(CDFonts *pfont)
{
	if (m_param.pFont != pfont)
	{
		m_param.pFont = pfont;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDGroupBox::OnDraw()
{// отображение
	float /*size,*/ WidthText, HeightText, xt,yt;
	if (m_param.bVisible)
	{
		// длина и ширина текста ---------------------------------------------
		if ( !m_param.Caption.IsEmpty() )
		{
			FTBBox bbox;
			FTPoint p1, p2;
			bbox = m_param.pFont->GetFont()->BBox(m_param.Caption, wcslen(m_param.Caption), p1, p2);
			WidthText = bbox.Upper().Xf() - bbox.Lower().Xf() + 4;
			HeightText = bbox.Upper().Yf() - bbox.Lower().Yf();
		}
		else
		{
			WidthText = HeightText = 0;
		}
		// точки по изображаемой кривой на стыках линий и дуг ----------------
		CRect rect = m_rect;
		rect.top = m_rect.top + HeightText / 2.f;
		Point p[10], pa[4];
		DefinePointForRoundRect(&rect, p, pa, m_param.Radius);
		switch(m_param.Alignment)
		{
			case StrAlignmentNear:
				p[8].X = m_rect.left + m_param.LengthLine0;
				p[9].X = p[8].X + WidthText + 1;
			break;
			case StrAlignmentCenter:
				p[8].X = m_rect.left + (m_rect.Width() - WidthText) / 2;
				p[9].X = p[8].X + WidthText + 1;
			break;
			case StrAlignmentFar:
				p[9].X = m_rect.right + m_param.LengthLine0;
				p[8].X = p[9].X - WidthText - 1;
			break;
		}
		
		p[8].Y = p[9].Y = p[0].Y;
		
		// рисую не замкнутую кривую по часовой стрелке ----------------------
		//size = 2 * m_param.Radius;
		glPushAttrib(GL_LIGHTING_BIT);
		glColor3ub(GetRValue(m_param.Color), GetGValue(m_param.Color), GetBValue(m_param.Color));
		glDisable(GL_LIGHTING);
        glEnable(GL_COLOR_MATERIAL);
		glLineWidth(m_param.thickness);
		glBegin(GL_LINE_STRIP);
		glVertex3f(p[9].X, p[9].Y, m_param.CoordZ);
		for(float t = 0.0; t <= 1.0; t += 0.1) {
			xt = (p[0].X*(1-t) + 2*t*rect.right)*(1-t) + pow(t,2)*p[1].X;
			yt = (p[0].Y*(1-t) + 2*t*rect.top)*(1-t) + pow(t,2)*p[1].Y;
			glVertex3f(xt, yt, m_param.CoordZ);
			
		}
		for(float t = 0.0; t <= 1.0; t += 0.1) {
			xt = (p[2].X*(1-t) + 2*t*rect.right)*(1-t) + pow(t,2)*p[3].X;
			yt = (p[2].Y*(1-t) + 2*t*rect.bottom)*(1-t) + pow(t,2)*p[3].Y;
			glVertex3f(xt, yt, m_param.CoordZ);
		}
		for(float t = 0.0; t <= 1.0; t += 0.1) {
			xt = (p[4].X*(1-t) + 2*t*rect.left)*(1-t) + pow(t,2)*p[5].X;
			yt = (p[4].Y*(1-t) + 2*t*rect.bottom)*(1-t) + pow(t,2)*p[5].Y;
			glVertex3f(xt, yt, m_param.CoordZ);
		}
		for(float t = 0.0; t <= 1.0; t += 0.1) {
			xt = (p[6].X*(1-t) + 2*t*rect.left)*(1-t) + pow(t,2)*p[7].X;
			yt = (p[6].Y*(1-t) + 2*t*rect.top)*(1-t) + pow(t,2)*p[7].Y;
			glVertex3f(xt, yt, m_param.CoordZ);
		}
		glVertex3f(p[8].X, p[8].Y, m_param.CoordZ);
		glEnd();
		glDisable(GL_COLOR_MATERIAL);
        glEnable(GL_LIGHTING);
		// текст -------------------------------------------------------------
		if ( !m_param.Caption.IsEmpty() )
		{
			//glDisable(GL_DEPTH_TEST);
			//glColor3ub(GetRValue(m_param.Color), GetGValue(m_param.Color), GetBValue(m_param.Color));
			//glEnable(GL_COLOR_MATERIAL);
			m_param.pFont->DrawString(CRect((float)p[8].X, (float)m_rect.top,
				(float)p[8].X + (float)WidthText, (float)m_rect.top + (float)HeightText), m_param.Caption);
			//glDisable(GL_COLOR_MATERIAL);
			//glEnable(GL_DEPTH_TEST);
		}
		glPopAttrib();
	}//	if (m_param.bVisible)
}
//------------------------------------------------------------------------------
void CDGroupBox::SetThickness (float val)
{
	if (val > 0)
	{
		if (m_param.thickness != val)
			m_param.thickness = val;
	}
}
//------------------------------------------------------------------------------
void CDGroupBox::SetTextColor (COLORREF val)
{
	if (m_param.pFont->GetColor() != val)
	{
		m_param.TextColor = val;
		m_param.pFont->SetColor(val);
	}
}
//------------------------------------------------------------------------------
void CDGroupBox::SetTextSize (float val)
{
	if (m_param.TextSize != val)
		m_param.pFont->SetSize(val);
}
//------------------------------------------------------------------------------
void CDGroupBox::SaveParameters(SimpleXML *pSimpleXML)
{
    CString cString;
    char String[16];

    sprintf_s(String, "%d", m_param.nElement);

    cString = CString(String);

    hXml hRoot = pSimpleXML->GetRootNode();
    hXml hInterfaceRoot = pSimpleXML->GetChildNode(hRoot);

    hXml hIndicatorRoot = pSimpleXML->AddElement(L"GroupBox_"+cString, hInterfaceRoot);

    hXml hTextRoot = pSimpleXML->AddElement(L"Font", hIndicatorRoot);
    hXml hColorRoot = pSimpleXML->AddElement(L"Color", hTextRoot);
    pSimpleXML->SetAttributeValue(hColorRoot, L"red", CComVariant(GetRValue(m_param.Color)));
    pSimpleXML->SetAttributeValue(hColorRoot, L"green", CComVariant(GetGValue(m_param.Color)));
    pSimpleXML->SetAttributeValue(hColorRoot, L"blue", CComVariant(GetBValue(m_param.Color)));

    cString = CString(m_param.FontName);
    hXml hTypeRoot = pSimpleXML->AddElement(L"Type", hTextRoot);
    pSimpleXML->SetAttributeValue(hTypeRoot, L"name", CComVariant(m_param.FontName));
}
//------------------------------------------------------------------------------
void CDGroupBox::LoadParameters(SimpleXML *pSimpleXML)
{
    VARIANT vAtrrib;
    CString str = L"";
    CString Pattern = L"GroupBox_";
    char String[16];
    int R, G, B, i;

    sprintf_s(String, "%d", m_param.nElement);
    for(int Counter2 = 0, Size = strlen(String); Counter2 < Size; Counter2++)
        Pattern = Pattern + String[Counter2];

    hXml hRoot = pSimpleXML->GetRootNode();
    hXml hInterfaceRoot = pSimpleXML->GetChildNode(hRoot);
    hXml hElement = pSimpleXML->GetChildNode(hInterfaceRoot);
    while(str!=Pattern)
    {
        hElement = pSimpleXML->GetNextNode(hElement);
        str = pSimpleXML->GetName(hElement);
        if(str == L"")
            return;
    }
    hXml hFont = pSimpleXML->GetChildNode(hElement);
    hXml hColor = pSimpleXML->GetChildNode(hFont);

    pSimpleXML->GetAttributeValue(hColor, L"red", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            R = _wtoi(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hColor, L"green", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            G = _wtoi(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hColor, L"blue", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            B = _wtoi(vAtrrib.bstrVal);
    m_param.Color = RGB(R, G, B);

    hXml hType = pSimpleXML->GetNextNode(hColor);
    pSimpleXML->GetAttributeValue(hType, L"name", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            str = CString(vAtrrib.bstrVal);
    for(i=0; i<str.GetLength(); i++)
        String[i] = str[i];
    String[i] = '\0';
    strcpy(m_param.FontName, String);
    m_param.pFont->SetName(m_param.FontName);
    m_param.pFont->SetColor(m_param.TextColor);
    SetColor(m_param.Color);
    m_param.pFont->SetStyle(FontsStyleRegular);
}
//------------------------------------------------------------------------------
#endif