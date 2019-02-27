//------------------------------------------------------------------------------
// CNumIndicator.cpp : файл реализации
// Прямоугольник элемента в процессе работы не меняется, поэтому положение
// (горизонтальное или вертикальное) определяется один раз в конструкторе.
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\Draw\DNumIndicator.h"

//------------------------------------------------------------------------------
int g_nIndicatorLevel = 0;		// счётчик статических картинок
//------------------------------------------------------------------------------
#ifdef _GDIPLUS
CDNumIndicator::CDNumIndicator(CRect rect, PVOID ppGraphics,
	bool *pFlagIsChange, double *pTimeAnimation) :
	CCustomDraw(rect, ppGraphics, pFlagIsChange, pTimeAnimation)
#else
CDNumIndicator::CDNumIndicator(CRect rect,
	bool *pFlagIsChange, double *pTimeAnimation) :
	CCustomDraw(rect, pFlagIsChange, pTimeAnimation)
#endif
{
	m_pParamDraw = &m_param;	// обеспечение ОБЩИХ параметров
	m_param.nElement = ++g_nIndicatorLevel;
	SetNameWithNumber(L"Numeric_indicator");
	//TRACE("CREATE CDNumIndicator %s\n", (CStringA)m_pParamDraw->Name);
	// переменные класса
	DefineRectAll();
	m_bDanger = false;
	flag = false;
	m_rect3 = m_rect;
#ifdef _GDIPLUS
	m_param.pFont = new CZFont();
#else
	m_param.pFont = new CDFonts(m_param.FontName, int(rect.Height() - 6) , FontsStyleRegular, m_param.TextColor);
	m_param.pFont->SetSize(m_param.TextSize);
#endif
}
//------------------------------------------------------------------------------
CDNumIndicator::~CDNumIndicator()
{
	//TRACE("DELETE CDNumIndicator %s\n", (CStringA)m_pParamDraw->Name);
	if (m_param.pFont)
	{
		m_param.pFont->Clear();
		delete m_param.pFont;
	}
}
//------------------------------------------------------------------------------
void CDNumIndicator::SetTextColor (COLORREF colortext)
{
	if (m_param.TextColor != colortext)
	{
		m_param.TextColor = colortext;
		m_param.pFont->SetColor(m_param.TextColor);
	}
}
//------------------------------------------------------------------------------
COLORREF CDNumIndicator::GetTextColor ()
{
	return m_param.TextColor;
}
//------------------------------------------------------------------------------
void CDNumIndicator::SetText(CString val)
{
	if (m_param.Text != val)
	{
		m_param.Text = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDNumIndicator::DefineRectAll()
{
	m_rect1 = m_rectT = m_rect;
	if (m_param.PositionType == tpHorizontal)
	{
		m_rectT.left = m_rect.right - (m_rect.bottom - m_rect.top);
		m_rect1.right = m_rectT.left - m_param.Distance;
		m_SizeRectT = m_rectT.right - m_rectT.left;
	}
	else
	{
		m_rectT.bottom = m_rect.top + (m_rect.right - m_rect.left);
		m_rect1.top = m_rectT.bottom + m_param.Distance;
		m_SizeRectT = m_rectT.bottom - m_rectT.top;
	}
	DefineRect2(&m_rect1, &m_rect2, m_param.BorderWidth);
	if (m_param.PositionType == tpHorizontal)
		m_LengthRect2 = m_rect2.Width();
	else
		m_LengthRect2 = m_rect2.Height();
}
//------------------------------------------------------------------------------
void CDNumIndicator::SetAmplitude(float val)
{
	if (m_param.Amplitude != val)	
		m_param.Amplitude = val;
	IsChange();
}
//------------------------------------------------------------------------------
void CDNumIndicator::SetThresholdDanger(float val)
{
	if ( (val > 0.8f) && (val <= 1.f) && (m_param.ThresholdDanger != val) )
	{
		m_param.ThresholdDanger = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDNumIndicator::SetLinLog(long val)
{
	long newVal;
	if (val != 0)
		newVal = 1;
	else
		newVal = 0;
	if (m_param.LinLog != newVal)
	{
		m_param.LinLog = newVal;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDNumIndicator::OnMouseEnter()
{
	flag = true;
	CCustomDraw::OnMouseEnter();
}
//------------------------------------------------------------------------------
void CDNumIndicator::OnMouseLeave()
{
	flag = false;
	CCustomDraw::OnMouseLeave();
}
//------------------------------------------------------------------------------
void CDNumIndicator::SetDistance(int val)
{
	int newVal;
	if (val < 2)
		newVal = 2;
	else
		newVal = val;
	if (m_param.Distance != newVal)
	{
		m_param.Distance = newVal;
		DefineRectAll();
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDNumIndicator::SetBorderWidth(int val)
{

	int newVal;
	if (val < 2)
		newVal = 2;
	else
		newVal = val;
	if (m_param.BorderWidth != newVal)
	{
		m_param.BorderWidth = newVal;
		DefineRect2(&m_rect1, &m_rect2, m_param.BorderWidth);
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDNumIndicator::SetBorderColor(COLORREF val)
{
	if (m_param.BorderColor != val)
	{
		m_param.BorderColor = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDNumIndicator::SetBackColor(COLORREF val)
{
	if (m_param.BackColor != val)
	{
		m_param.BackColor = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDNumIndicator::SetTimeAnimation(double val)
{
	if (val > 0.)
		m_param.dTimeAnimation = val;
}
//------------------------------------------------------------------------------
void CDNumIndicator::SetPrecision(int val)
{
	if (val >= 0 && val <= 7)
		m_param.Precision = val;
}
//------------------------------------------------------------------------------
void CDNumIndicator::DoProcess()
{
	if (m_param.bAnimation)
	{
		if ( (GetTime() - m_dTimeStart) > m_param.dTimeAnimation )
		{
			m_param.bAnimation = false;
			if (m_param.Amplitude > m_param.ThresholdDanger)
				m_bDanger = true;
		}
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDNumIndicator::OnLButtonClk(UINT nFlags, CPoint point)
{
	if ( m_bDanger && PointInRect(point, &m_rectT) )
	{
		m_bDanger = false;
		m_param.bAnimation = true;
		m_dTimeStart = GetTime();
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDNumIndicator::SetIntegerPart (int val)
{
	m_param.Integer_part = val;
}
//------------------------------------------------------------------------------
#ifdef _GDIPLUS
void CDNumIndicator::OnDraw()
{// отображение

}
#else
//------------------------------------------------------------------------------
void CDNumIndicator::SetFont(CDFonts *pfont)
{
	if (m_param.pFont != pfont)
	{
		m_param.pFont = pfont;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDNumIndicator::OnDraw()
{// отображение
	//	Каждая цифра в своей ячейке
	char sfromat[20];
	//sprintf(vOut, "%*.*f", m_param.Integer_part + m_param.Precision + 1, m_param.Precision, m_param.Amplitude);
	//m_param.Text = (CString)vOut;
	//	преобразуем число к виду с постоянным положением точки
	if (m_param.TypeView)
	{
		m_param.Text = L"";
		sprintf(vOut, "%*.*f", m_param.Integer_part + m_param.Precision + 1, m_param.Precision, m_param.Amplitude);
		for (int i = 0; i < 6-m_param.Integer_part; i++)//
			m_param.Text += L" ";
		m_param.Text += (CString)vOut;
		for (int i = 0; i < 6-m_param.Precision; i++)//
			m_param.Text += L" ";
	}
	else
	{
		m_param.Text = m_param.TextValue;
	}
	m_rect3 = m_rect;
	m_rect3.top = m_rect3.top + (m_rect3.bottom - m_rect3.top)/7;
	m_rect3.bottom = m_rect3.bottom - (m_rect3.bottom - m_rect3.top)/7;
	if (m_param.bVisible)
	{
		if (flag)
			FillRectSolid(&m_rect, 10, m_param.BackColor);
		m_param.pFont->DrawNumber(m_rect, m_param.Text, StrAlignmentCenter, StrAlignmentCenter);
	}
}
//------------------------------------------------------------------------------
void CDNumIndicator::SaveParameters(SimpleXML *pSimpleXML)
{
    CString cString;
    char String[16];

    sprintf_s(String, "%d", m_param.nElement);

    cString = CString(String);

    hXml hRoot = pSimpleXML->GetRootNode();
    hXml hInterfaceRoot = pSimpleXML->GetChildNode(hRoot);

    hXml hIndicatorRoot = pSimpleXML->AddElement(L"NumIndicator_"+cString, hInterfaceRoot);

    hXml hTextRoot = pSimpleXML->AddElement(L"Font", hIndicatorRoot);
    hXml hColorRoot = pSimpleXML->AddElement(L"Color", hTextRoot);
    int a = GetRValue(m_param.TextColor);
    pSimpleXML->SetAttributeValue(hColorRoot, L"red", CComVariant(GetRValue(m_param.TextColor)));
    pSimpleXML->SetAttributeValue(hColorRoot, L"green", CComVariant(GetGValue(m_param.TextColor)));
    pSimpleXML->SetAttributeValue(hColorRoot, L"blue", CComVariant(GetBValue(m_param.TextColor)));

    cString = CString(m_param.FontName);
    hXml hTypeRoot = pSimpleXML->AddElement(L"Type", hTextRoot);
    pSimpleXML->SetAttributeValue(hTypeRoot, L"name", CComVariant(m_param.FontName));
}
//------------------------------------------------------------------------------
void CDNumIndicator::LoadParameters(SimpleXML *pSimpleXML)
{
    VARIANT vAtrrib;
    CString str = L"";
    CString Pattern = L"NumIndicator_";
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
    m_param.pFont->SetColor(m_param.TextColor);
    m_param.pFont->SetName(m_param.FontName);
}
//------------------------------------------------------------------------------
void CDNumIndicator::SetTextSize (float val)
{
	if(m_param.TextSize != val && val > 0.f)
	{
		m_param.TextSize = val;
		m_param.pFont->SetSize(m_param.TextSize);
	}
}
//------------------------------------------------------------------------------
float CDNumIndicator::GetTextSize ()
{
	return m_param.pFont->GetSize();
}
//------------------------------------------------------------------------------
void CDNumIndicator::SetTextValue (CString val)
{
	if (m_param.TextValue != val)
		m_param.TextValue = val;
}
//------------------------------------------------------------------------------
CString CDNumIndicator::GetTextVal (void)
{
	return m_param.TextValue;
}
//------------------------------------------------------------------------------
bool CDNumIndicator::GetTextCeils ()
{
	return m_param.pFont->GetCeils();
}
//------------------------------------------------------------------------------
void CDNumIndicator::SetTextCeils (bool val)
{
	m_param.pFont->SetCeils(val);
}
//------------------------------------------------------------------------------
#endif