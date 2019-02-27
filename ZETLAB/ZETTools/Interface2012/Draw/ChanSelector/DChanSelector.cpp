//------------------------------------------------------------------------------
// DChanSelector.cpp : файл реализации
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\Draw\ChanSelector\DChanSelector.h"

//------------------------------------------------------------------------------
int g_nChanSelector = 0;		// счётчик
//------------------------------------------------------------------------------
#ifdef _GDIPLUS
CDChanSelector::CDChanSelector(CRect rect, PVOID ppGraphics,
	bool *pFlagIsChange, double *pTimeAnimation) :
	CCustomDraw(rect, ppGraphics, pFlagIsChange, pTimeAnimation)
#else
CDChanSelector::CDChanSelector(CRect rect,
	bool *pFlagIsChange, double *pTimeAnimation) :
	CCustomDraw(rect, pFlagIsChange, pTimeAnimation)
#endif
{
	m_pParamDraw = &m_param;	// обеспечение ОБЩИХ параметров
	m_param.nElement = ++g_nChanSelector;
	m_param.bMouseChange = true;
#ifdef _GDIPLUS
	m_param.pFont = new CZFont();
#else
	m_param.pFont = new CDFonts();

#endif
	m_BackColorCur = m_param.BackColor;
	m_BorderColorCur = m_param.BorderColor;
	m_bEvent = false;
	m_bFixation = false;
	SetBorderWidth(1);
	SetNameWithNumber(L"Channel");
	m_param.Caption = m_param.Name;
	SetNameWithNumber(L"Channel");
	m_param.CaptionChange = m_param.Name;
	m_Text = m_param.Caption;
	//m_pImage_Cur =
		//m_pImage_pm =
		//m_pImage_s =
		//m_pImage_m =
		//m_pImage_p = NULL;
}
//------------------------------------------------------------------------------
CDChanSelector::~CDChanSelector()
{
	if (m_param.pFont)
	{
		delete m_param.pFont;
	}
	//DeleteGdiPlusObject(m_pImage_s);
	//DeleteGdiPlusObject(m_pImage_m);
	//DeleteGdiPlusObject(m_pImage_p);
	//DeleteGdiPlusObject(m_pImage_pm);
}

//------------------------------------------------------------------------------
/*void CDChanSelector::SetImageFileName(CString str)
{
	if ( !str.IsEmpty() && (m_param.fileName != str) )
	{
		m_param.fileName = str;
		LoadImageFromFile(&m_pImage_s, str + L"_s");
		LoadImageFromFile(&m_pImage_m, str + L"_m");
		LoadImageFromFile(&m_pImage_p, str + L"_p");
		LoadImageFromFile(&m_pImage_pm, str + L"_pm");
		m_pImage_Cur = m_pImage_s;
		IsChange();
	}
}*/
//------------------------------------------------------------------------------
void CDChanSelector::SetBorderWidth(int val)
{
	int n;
	if (m_param.BorderWidth != val)
	{
		m_param.BorderWidth = val;
		DefineRect2(&m_rect, &m_rect1, val);
		n = m_rect1.Height();
		n /= 3;
		m_param.pt[1].X = m_rect1.right - n;
		m_param.pt[2].X = m_rect1.right - m_rect1.Height() / 2;
		m_param.pt[0].X = m_param.pt[2].X - (m_param.pt[1].X - m_param.pt[2].X);
		m_param.pt[0].Y = m_param.pt[1].Y = m_rect1.top + n + 1;
		m_param.pt[2].Y = m_rect1.bottom - n + 1;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDChanSelector::SetBorderColor(COLORREF val)
{
	if (m_param.BorderColor != val)
	{
		m_param.BorderColor = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDChanSelector::SetBorderColorFocus(COLORREF val)
{
	if (m_param.BorderColorFocus != val)
	{
		m_param.BorderColorFocus = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDChanSelector::SetSizeText(int val)
{
	if (m_param.SizeText != val)
	{
		m_param.SizeText = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDChanSelector::SetCaption(CString str)
{
	if (m_param.Caption != str)
	{
		m_param.Caption = str;
		m_Text = m_param.Caption;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDChanSelector::SetCaptionChange(CString str)
{
	if (m_param.CaptionChange != str)
	{
		m_param.CaptionChange = str;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDChanSelector::GetFocus()
{
	m_BorderColorCur = m_param.BorderColorFocus;
	SetBorderWidth(2);
	CCustomDraw::GetFocus();
}
//------------------------------------------------------------------------------
void CDChanSelector::KillFocus()
{
	m_BorderColorCur = m_param.BorderColor;
	SetBorderWidth(1);
	CCustomDraw::KillFocus();
}
//------------------------------------------------------------------------------
void CDChanSelector::OnMouseEnter()
{
		//m_pImage_Cur = m_pImage_m;
		m_BackColorCur = m_param.BackColorMouse;
	CCustomDraw::OnMouseEnter();
}
//------------------------------------------------------------------------------
void CDChanSelector::OnMouseLeave()
{
	if (!m_bFixation)
	{
		//m_pImage_Cur = m_pImage_s;
		m_BackColorCur = m_param.BackColor;
	}
	else 
	{	
		//m_pImage_Cur = m_pImage_p;
		m_BackColorCur = m_param.BackColorEnter;
	}
	CCustomDraw::OnMouseLeave();
}
void CDChanSelector::SetTimeAnimation(double val)
{
	if (val > 0.)
		m_param.dTimeAnimation = val;
}
//------------------------------------------------------------------------------
void CDChanSelector::OnLButtonClk(UINT nFlags, CPoint point)
{
	if (m_bFixation)
	{
		//m_pImage_Cur = m_pImage_s;
		m_bFixation = false;
		m_BackColorCur = m_param.BackColorEnter;
	}		
	else
	{
		//m_pImage_Cur = m_pImage_p;
		m_BackColorCur = m_param.BackColorMouse;
		m_bFixation = true;
	}
	IsChange();
}
//------------------------------------------------------------------------------
/*void CDChanSelector::DoProcess()
{
	if (m_param.bAnimation)
	{
		m_param.dTimeCur = GetTime() - m_param.dTimeStart;
		IsChange();
	}
}*/
//------------------------------------------------------------------------------
#ifdef _GDIPLUS
void CDChanSelector::SetAlignmentHorizontal(StringAlignment val)
{
	if (m_param.AlignmentHorizontal != val)
	{
		m_param.AlignmentHorizontal = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDChanSelector::SetAlignmentVertical(StringAlignment val)
{
	if (m_param.AlignmentVertical != val)
	{
		m_param.AlignmentVertical = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
/*void CDChanSelector::OnDraw()
{// отображение
	if (m_param.bVisible)
	{
		if (m_param.ButtonType)
		{
			if (m_param.bAnimation)
			{
				if (m_param.dTimeCur < m_param.dTimeAnimation)
					m_pImage_Cur = m_pImage_p;
				else
				{
					if (m_param.bMouseCome)
						m_pImage_Cur = m_pImage_m;
					else
						m_pImage_Cur = m_pImage_s;
					m_param.bAnimation = false;
				}
			}
			else
			{
				if (m_param.bMouseCome)
					m_pImage_Cur = m_pImage_m;
				else
					m_pImage_Cur = m_pImage_s;
			}
		}
		Graphics *pGraphics = *((Graphics**)m_ppGraphics);

		if (!m_param.CheckBox)
		{
			// граница
			FillRectSolid(pGraphics, &m_rect, m_BorderColorCur);
			// поле
			pGraphics->DrawImage(m_pImage_Cur,m_rect2.left, m_rect2.top, m_rect2.Width(), m_rect2.Height() );
			// текст
			DrawString(pGraphics, m_param.Caption, m_param.pFont,
				RectF( (float)m_rect2.left, (float)m_rect2.top,
				(float)m_rect2.Width(), (float)m_rect2.Height() ),
				m_param.AlignmentHorizontal,
				m_param.AlignmentVertical);
		}
		else
		{
			m_param.pFont->SetSize(m_rect.Height()*0.6);
			// граница
			FillRectSolid(pGraphics, &m_rect1, m_BorderColorCur);
			// рамка
			FillRectSolid(pGraphics, &m_rect3, m_param.BackColor);
			// заливка градиентом 
			FillRectLinearGradient(pGraphics, &m_rect4, m_BackColorCur, m_param.BackColor, m_param.GradientMode);
			//FillRectLinearGradient(pGraphics, &m_rect2,m_param.BackColorMouse, m_param.BackColor, m_param.GradientMode);
			// галочка
			if (m_bFixation)
			{
				DrawLine(pGraphics, &CRect(m_rect3.left + m_rect3.Width()/4, m_rect3.top + m_rect3.Height()/2,
					m_rect3.left + m_rect3.Width()/2,m_rect3.bottom - 3), m_CheckColorCur, 1);
				DrawLine(pGraphics, &CRect(m_rect3.right - 2, m_rect3.top,
					m_rect3.left + m_rect3.Width()/2, m_rect3.bottom - 3), m_CheckColorCur, 2);
			}
			//текст
			DrawString(pGraphics, m_param.Caption, m_param.pFont,
				RectF( (float)m_rect1.right + 3, (float)m_rect3.top,
				(float)(m_rect.Width() - m_rect1.Width()), (float)m_rect3.Height()),
				StringAlignmentNear, StringAlignmentCenter);
		}
	}
}*/
//------------------------------------------------------------------------------
void CDChanSelector::OnDraw()
{
	Graphics *pGraphics = *((Graphics**)m_ppGraphics);
		// граница
		FillRectSolid(pGraphics, &m_rect, m_BorderColorCur);
		// рамка
		FillRectSolid(pGraphics, &CRect(m_rect1.left, m_rect1.top, INT(m_rect1.right - m_rect1.Width()/4 - m_param.BorderWidth), m_rect1.bottom), m_BackColorCur);
		FillRectSolid(pGraphics, &CRect(INT(m_rect1.right - m_rect1.Width()/4), m_rect1.top, m_rect1.right, m_rect1.bottom), m_BackColorCur);
		// треугольник
		SolidBrush sb( Color(190, 23, 23, 23) );
		pGraphics->FillPolygon(&sb, m_param.pt, 3);
		// text
		DrawString(pGraphics, m_param.Caption,  m_param.pFont,
			RectF(m_rect1.left, m_rect1.top,
			m_rect1.Width() - m_rect1.Height(),m_rect1.Height() ),
			StringAlignmentNear);


		
		// заливка градиентом 
		//FillRectLinearGradient(pGraphics, &m_rect4, m_BackColorCur, m_param.BackColor, m_param.GradientMode);
	}
#else
void CDChanSelector::OnDraw()
{
	// граница
	FillRectSolid(&m_rect, 0,m_BorderColorCur);
	// рамка
	FillRectSolid(&CRect(m_rect1.left, m_rect1.top, INT(m_rect1.right - m_rect1.Width()/4 - m_param.BorderWidth), m_rect1.bottom), 0.1, m_BackColorCur);
	//FillRectSolid(&CRect(INT(m_rect1.right - m_rect1.Width()/4), m_rect1.top, m_rect1.right, m_rect1.bottom),0.2, m_BackColorCur);
	// треугольник
	// text
	// m_param.pFont->DrawString(CRect(m_rect1.left, m_rect1.top,
		//m_rect1.Width() - m_rect1.Height(),m_rect1.Height()), m_param.Caption,
		//StrAlignmentCenter, StrAlignmentCenter);
}
#endif