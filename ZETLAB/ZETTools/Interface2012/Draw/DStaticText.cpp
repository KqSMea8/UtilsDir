//------------------------------------------------------------------------------
// DStaticText.cpp : файл реализации
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\Draw\DStaticText.h"
//------------------------------------------------------------------------------
int g_nStaticText = 0;		// счётчик статических картинок
//------------------------------------------------------------------------------
#ifdef _GDIPLUS
CDStaticText::CDStaticText(CRect rect, PVOID ppGraphics,
				bool *pFlagIsChange, double *pTimeAnimation) :
				CCustomDraw(rect, ppGraphics, pFlagIsChange, pTimeAnimation)
#else
CDStaticText::CDStaticText(CRect rect,
	bool *pFlagIsChange, double *pTimeAnimation) :
CCustomDraw(rect, pFlagIsChange, pTimeAnimation)
#endif
{
	m_pParamDraw = &m_param;	// обеспечение ОБЩИХ параметров
	m_param.nElement = ++g_nStaticText;
	SetNameWithNumber(L"StaticText");
	//TRACE("CREATE CDStaticText %s\n", (CStringA)m_pParamDraw->Name);
#ifdef _GDIPLUS
	m_param.pFont = new CZFont();
#else
	m_param.pFont = new CDFonts(m_param.FontName, int(rect.Height() - 6) , FontsStyleRegular, m_param.TextColor);
	m_param.pFont->SetSize(m_param.TextSize);
#endif
}
//------------------------------------------------------------------------------
CDStaticText::~CDStaticText()
{
	//TRACE("DELETE CDStaticText %s\n", (CStringA)m_pParamDraw->Name);
	if (m_param.pFont)
	{
//		m_param.pFont->Clear();
		delete m_param.pFont;
	}
}
//------------------------------------------------------------------------------
void CDStaticText::SetText(CString val)
{
	if (m_param.Text != val)
	{
		m_param.Text = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
#ifdef _GDIPLUS
void CDStaticText::SetFont(CZFont *pfont)
{
	if (m_param.pFont != pfont)
	{
		m_param.pFont = pfont;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDStaticText::SetAlignmentHorizontal(StringAlignment val)
{
	if (m_param.AlignmentHorizontal != val)
	{
		m_param.AlignmentHorizontal = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDStaticText::SetAlignmentVertical(StringAlignment val)
{
	if (m_param.AlignmentVertical != val)
	{
		m_param.AlignmentVertical = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDStaticText::OnDraw()
{// отображение
	if (m_param.bVisible)
	{
		Graphics *pGraphics = *((Graphics**)m_ppGraphics);
		DrawString(pGraphics, m_param.Text, m_param.pFont, 
					RectF( (float)m_rect.left, (float)m_rect.top,
							(float)m_rect.Width(), (float)m_rect.Height() ),
					m_param.AlignmentHorizontal,
					m_param.AlignmentVertical);
	}
}
//------------------------------------------------------------------------------
#else
void CDStaticText::SetFont(CDFonts *pfont)
{
	if (m_param.pFont != pfont)
	{
		m_param.pFont = pfont;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDStaticText::SetAlignmentHorizontal(StringAlignmentOGL val)
{
	if (m_param.AlignmentHorizontal != val)
	{
		m_param.AlignmentHorizontal = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDStaticText::SetAlignmentVertical(StringAlignmentOGL val)
{
	if (m_param.AlignmentVertical != val)
	{
		m_param.AlignmentVertical = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDStaticText::OnDraw()
{// отображение
    CString text = m_param.Text;
    CRect rect = m_rect;
	if (m_param.bVisible)
	{
		m_param.pFont->DrawString(rect, text, m_param.AlignmentHorizontal, m_param.AlignmentVertical);
	}
}
//------------------------------------------------------------------------------
void CDStaticText::SetTextSize (float val)
{
	if (m_param.TextSize != val && val > 0.f)
	{
		m_param.TextSize = val;
		m_param.pFont->SetSize(m_param.TextSize);
	}
}
//------------------------------------------------------------------------------
float CDStaticText::GetTextSize ()
{
	return m_param.pFont->GetSize();
}
//------------------------------------------------------------------------------
void CDStaticText::Clear()
{
	m_param.pFont->Clear();
}
//------------------------------------------------------------------------------
void CDStaticText::SaveParameters(SimpleXML *pSimpleXML)
{
    CString cString;
    char String[16];

    sprintf_s(String, "%d", m_param.nElement);

    cString = CString(String);

    hXml hRoot = pSimpleXML->GetRootNode();
    hXml hInterfaceRoot = pSimpleXML->GetChildNode(hRoot);

    hXml hIndicatorRoot = pSimpleXML->AddElement(L"StaticText_"+cString, hInterfaceRoot);

    m_param.TextColor = m_param.pFont->GetColor();
    hXml hTextRoot = pSimpleXML->AddElement(L"Font", hIndicatorRoot);
    hXml hColorRoot = pSimpleXML->AddElement(L"Color", hTextRoot);
    pSimpleXML->SetAttributeValue(hColorRoot, L"red", CComVariant(GetRValue(m_param.TextColor)));
    pSimpleXML->SetAttributeValue(hColorRoot, L"green", CComVariant(GetGValue(m_param.TextColor)));
    pSimpleXML->SetAttributeValue(hColorRoot, L"blue", CComVariant(GetBValue(m_param.TextColor)));

    //m_param.FontName = m_param.pFont->GetName();
    cString = CString(m_param.FontName);
    hXml hTypeRoot = pSimpleXML->AddElement(L"Type", hTextRoot);
    pSimpleXML->SetAttributeValue(hTypeRoot, L"name", CComVariant(m_param.FontName));
}
//------------------------------------------------------------------------------
void CDStaticText::LoadParameters(SimpleXML *pSimpleXML)
{
    VARIANT vAtrrib;
    CString str = L"";
    CString Pattern = L"StaticText_";
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
    m_param.TextColor = RGB(R, G, B);

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
    m_param.pFont->SetStyle(FontsStyleRegular);
}
//------------------------------------------------------------------------------
void CDStaticText::SetTextColor (COLORREF val)
{
	if (m_param.TextColor != val)
	{
		m_param.TextColor = val;
		m_param.pFont->SetColor(m_param.TextColor);
	}
}
//------------------------------------------------------------------------------
COLORREF CDStaticText::GetTextColor ()
{
	return m_param.pFont->GetColor();
}
//------------------------------------------------------------------------------
#endif