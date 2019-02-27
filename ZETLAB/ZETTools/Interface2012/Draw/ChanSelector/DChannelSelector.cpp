//------------------------------------------------------------------------------
// DChanSelector.cpp : файл реализации
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\Draw\ChanSelector\DChannelSelector.h"

//------------------------------------------------------------------------------
int g_nChannelSelector = 0;		// счЄтчик
//------------------------------------------------------------------------------
#ifdef _GDIPLUS
CDChannelSelector::CDChannelSelector(CRect rect, PVOID ppGraphics,
	bool *pFlagIsChange, double *pTimeAnimation) :
	CCustomDraw(rect, ppGraphics, pFlagIsChange, pTimeAnimation)
#else
CDChannelSelector::CDChannelSelector(CRect rect,
	bool *pFlagIsChange, double *pTimeAnimation) :
	CCustomDraw(rect, pFlagIsChange, pTimeAnimation)
#endif
{
	m_pParamDraw = &m_param;	// обеспечение ќЅў»’ параметров
	m_param.nElement = ++g_nChannelSelector;
	m_param.bMouseChange = true;
	size = 8;
#ifdef _GDIPLUS
	m_param.pFont = new CZFont();
#else
	m_param.pFont = new CDFonts();
#ifdef _ACTIVEX_ZIS
	Texture = LoadGLTextures(_T("C:\\ZETTools\\Interface2012\\Textures\\Texture6.bmp"), PicSize, 0);
	//лева€ часть панели
	//лева€ часть панели
	m_ChSelector[0] = new CDetail(long(26), size/2.0 + m_rect.left, (m_rect.bottom + m_rect.top)/2.0, 1.75, 0, 
		size, m_rect.Height() - 2.0*size, 1.5);			
	//левый верхний угол
	m_ChSelector[1] = new CDetail(long(27), size/2.0 + m_rect.left, size/2.0 + m_rect.top, 1.75, 0, 
		size, size, 1.5);
	//верхн€€ часть панели
	m_ChSelector[2] = new CDetail(long(28), (m_rect.right + m_rect.left)/2.0, size/2.0 + m_rect.top, 1.75, 0, 
		m_rect.Width() - 2.0*size, size, 1.5);
	//правый верхний угол
	m_ChSelector[3] = new CDetail(long(29), m_rect.right - size/2.0, size/2.0 + m_rect.top, 1.75, 0, 
		size, size, 1.5);
	//права€ часть панели
	m_ChSelector[4] = new CDetail(long(30), m_rect.right - size/2.0,(m_rect.bottom + m_rect.top)/2.0, 1.75, 0, 
		size, m_rect.Height() - 2.0*size, 1.5);
	//правый нижний угол
	m_ChSelector[5] = new CDetail(long(31), m_rect.right - size/2.0, m_rect.bottom - size/2.0, 1.75, 0, 
		size, size, 1.5);
	//нижн€€ часть панели
	m_ChSelector[6] = new CDetail(long(32), (m_rect.right + m_rect.left)/2.0, m_rect.bottom - size/2.0, 1.75, 0, 
		m_rect.Width() - 2.0*size, size, 1.5);
	//левый нижний угол
	m_ChSelector[7] = new CDetail(long(33), size/2.0 + m_rect.left, m_rect.bottom - size/2.0, 1.75, 0, 
		size, size, 1.5);			
	//центр
	m_ChSelector[8] = new CDetail(long(34), (m_rect.right + m_rect.left)/2.0, (m_rect.bottom + m_rect.top)/2.0, 1.75, 0, 
		m_rect.Width() - 2.0*size, m_rect.Height() - 2.0*size, 1.5);//*/
#else
	Texture = LoadGLTextures(_T("C:\\ZETTools\\Interface2012\\Textures\\Texture6.bmp"), PicSize, 0);
	//лева€ часть панели
	m_ChSelector[0] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\ComboBox\\лев_комб.stl",
		size/2.0 + m_rect.left, (m_rect.bottom + m_rect.top)/2.0, 1.75, 0, 
		size, m_rect.Height() - 2.0*size, 1.5);		
	//левый верхний угол
	m_ChSelector[1] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\ComboBox\\лев ниж_комб.stl",
		size/2.0 + m_rect.left, size/2.0 + m_rect.top, 1.75, 0, 
		size, size, 1.5);
	//верхн€€ часть панели
	m_ChSelector[2] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\ComboBox\\низ_комб.stl",
		(m_rect.right + m_rect.left)/2.0, size/2.0 + m_rect.top, 1.75, 0, 
		m_rect.Width() - 2.0*size, size, 1.5);
	//правый верхний угол
	m_ChSelector[3] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\ComboBox\\прав ниж_комб.stl",
		m_rect.right - size/2.0, size/2.0 + m_rect.top, 1.75, 0, 
		size, size, 1.5);	
	//права€ часть панели
	m_ChSelector[4] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\ComboBox\\правый_комб.stl",
		m_rect.right - size/2.0,(m_rect.bottom + m_rect.top)/2.0, 1.75, 0, 
		size, m_rect.Height() - 2.0*size, 1.5);
	//правый нижний угол
	m_ChSelector[5] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\ComboBox\\прав верх_комб.stl",
		m_rect.right - size/2.0, m_rect.bottom - size/2.0, 1.75, 0, 
		size, size, 1.5);
	//нижн€€ часть панели
	m_ChSelector[6] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\ComboBox\\верх_комб.stl",
		(m_rect.right + m_rect.left)/2.0, m_rect.bottom - size/2.0, 1.75, 0, 
		m_rect.Width() - 2.0*size, size, 1.5);
	//левый нижний угол
	m_ChSelector[7] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\ComboBox\\лев верх_комб.stl",
		size/2.0 + m_rect.left, m_rect.bottom - size/2.0, 1.75, 0, 
		size, size, 1.5);			
	//центр
	m_ChSelector[8] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\ComboBox\\центр_комб.stl",
		(m_rect.right + m_rect.left)/2.0, (m_rect.bottom + m_rect.top)/2.0, 1.75, 0, 
		m_rect.Width() - 2.0*size, m_rect.Height() - 2.0*size, 1.5);
#endif
	m_ChSelector[8]->SetScale(m_rect.Width() - 2.0*size, m_rect.Height() - 2.0*size, 1.5*(m_ChSelector[8]->Depth/m_ChSelector[1]->Depth));
	m_ChSelector[8]->SetOffset((m_rect.right + m_rect.left)/2.0, (m_rect.bottom + m_rect.top)/2.0, 1.75*(m_ChSelector[8]->Depth/m_ChSelector[1]->Depth));
#endif
	rect_panel.top = rect_panel.bottom = m_rect.bottom;
	rect_panel.left = m_rect.left;
	rect_panel.right = m_rect.right;
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
	Width = 100;
	//m_pImage_Cur =
		//m_pImage_pm =
		//m_pImage_s =
		//m_pImage_m =
		//m_pImage_p = NULL;
}
//------------------------------------------------------------------------------
CDChannelSelector::~CDChannelSelector()
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
void CDChannelSelector::SetBorderWidth(int val)
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
void CDChannelSelector::SetBorderColor(COLORREF val)
{
	if (m_param.BorderColor != val)
	{
		m_param.BorderColor = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDChannelSelector::SetBorderColorFocus(COLORREF val)
{
	if (m_param.BorderColorFocus != val)
	{
		m_param.BorderColorFocus = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDChannelSelector::SetSizeText(int val)
{
	if (m_param.SizeText != val)
	{
		m_param.SizeText = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDChannelSelector::SetCaption(CString str)
{
	if (m_param.Caption != str)
	{
		m_param.Caption = str;
		m_Text = m_param.Caption;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDChannelSelector::SetCaptionChange(CString str)
{
	if (m_param.CaptionChange != str)
	{
		m_param.CaptionChange = str;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDChannelSelector::SetPositionValue(BOOL val)
{
	m_bEvent = true;
	if (m_param.PositionValue != val)
		m_param.PositionValue = val;
	IsChange();
}
//------------------------------------------------------------------------------
void CDChannelSelector::GetFocus()
{
	m_BorderColorCur = m_param.BorderColorFocus;
	SetBorderWidth(2);
	CCustomDraw::GetFocus();
}
//------------------------------------------------------------------------------
void CDChannelSelector::KillFocus()
{
	m_BorderColorCur = m_param.BorderColor;
	SetBorderWidth(1);
	CCustomDraw::KillFocus();
}
//------------------------------------------------------------------------------
void CDChannelSelector::OnMouseEnter()
{
		//m_pImage_Cur = m_pImage_m;
		m_BackColorCur = m_param.BackColorMouse;
	CCustomDraw::OnMouseEnter();
}
//------------------------------------------------------------------------------
void CDChannelSelector::OnMouseLeave()
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
//------------------------------------------------------------------------------
void CDChannelSelector::OnLButtonClk(UINT nFlags, CPoint point)
{
	m_bEvent = true;
	if (m_bEvent)
	{
		m_param.PositionValue = -m_param.PositionValue + 1;
		/*if (m_param.PositionValue)
			m_param.PositionValue = false;
		else
			m_param.PositionValue = true;*/
		m_param.bAnimation = true;
		m_param.dTimeCur = 0.;
		m_param.dTimeStart = GetTime();
	}
	IsChange();
}
//------------------------------------------------------------------------------
bool CDChannelSelector::GetNeedEvent()
{
	bool ret = m_bEvent;
	m_bEvent = false;
	return ret;
}
//------------------------------------------------------------------------------
void CDChannelSelector::SetTimeAnimation(double val)
{
	if (val > 0.)
		m_param.dTimeAnimation = val;
}
//------------------------------------------------------------------------------
#ifdef _GDIPLUS
void CDChannelSelector::SetAlignmentHorizontal(StringAlignment val)
{
	if (m_param.AlignmentHorizontal != val)
	{
		m_param.AlignmentHorizontal = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDChannelSelector::SetAlignmentVertical(StringAlignment val)
{
	if (m_param.AlignmentVertical != val)
	{
		m_param.AlignmentVertical = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
/*void CDChannelSelector::OnDraw()
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
void CDChannelSelector::OnDraw()
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
void CDChannelSelector::OnDraw()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, m_param.mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m_param.mat_diffuse);
	glMaterialfv(GL_FRONT, GL_EMISSION, m_param.mat_emission);
	glMaterialfv(GL_FRONT, GL_SPECULAR, m_param.mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, m_param.mat_shininess);

	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Texture);
	for (int i = 0; i < DETAILS; i++)
		m_ChSelector[i]->Draw_texture(m_ChSelector[i]->ReadFacet(), m_ChSelector[i]->GetQntPlanes(), PicSize);
	glDisable(GL_TEXTURE_2D);


	// граница
	//FillRectSolid(&rect_panel, 2, m_BorderColorCur);
	// рамка
	/*FillRectSolid(&CRect(m_rect1.left, m_rect1.top, INT(m_rect1.right - m_rect1.Width()/4 - m_param.BorderWidth), m_rect1.bottom), 0.1, m_BackColorCur);*/
	//FillRectSolid(&CRect(INT(m_rect1.right - m_rect1.Width()/4), m_rect1.top, m_rect1.right, m_rect1.bottom),0.2, m_BackColorCur);
	// треугольник
	// text
	// m_param.pFont->DrawString(CRect(m_rect1.left, m_rect1.top,
		//m_rect1.Width() - m_rect1.Height(),m_rect1.Height()), m_param.Caption,
		//StrAlignmentCenter, StrAlignmentCenter);
}
#endif