//------------------------------------------------------------------------------
// DChanPanel.cpp : файл реализации
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\Draw\ChanSelector\DChanPanel.h"

//------------------------------------------------------------------------------
int g_nChannPanel = 0;		// счётчик
//------------------------------------------------------------------------------
#ifdef _GDIPLUS
CDChanPanel::CDChanPanel(CRect rect, PVOID ppGraphics,
	bool *pFlagIsChange, double *pTimeAnimation) :
	CCustomDraw(rect, ppGraphics, pFlagIsChange, pTimeAnimation)
#else
CDChanPanel::CDChanPanel(CRect rect,
	bool *pFlagIsChange, double *pTimeAnimation) :
	CCustomDraw(rect, pFlagIsChange, pTimeAnimation)
#endif
{
	m_pParamDraw = &m_param;	// обеспечение ОБЩИХ параметров
	m_param.nElement = ++g_nChannPanel;
	m_param.bMouseChange = true;
	m_param.CurChannel = 0;
#ifdef _GDIPLUS
	m_param.pFont = new CZFont();
#else
	m_param.pFont = new CDFonts("Calibri", 15.0 , FontsStyleItalic, RGB(255,255,255));
	//Texture = LoadGLTextures(_T("C:\\ZETTools\\Interface2012\\Textures\\Texture6.bmp"), PicSize);
	//m_ChSelector = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Квадртаная ванночка острый кромки 100х100х7.stl",
		//(m_rect.right + m_rect.left)/2, (m_rect.bottom + m_rect.top)/2, 0, 0, 
		//m_rect.right - m_rect.left, m_rect.bottom - m_rect.top, 0.5);
#endif
	rect_panel.top = rect_panel.bottom = m_rect.top;
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
	Width = rect.Height();
	//m_pImage_Cur =
		//m_pImage_pm =
		//m_pImage_s =
		//m_pImage_m =
		//m_pImage_p = NULL;
}
//------------------------------------------------------------------------------
CDChanPanel::~CDChanPanel()
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
void CDChanPanel::SetCurChannel(long val)
{
	if (m_param.CurChannel != val)
		m_param.CurChannel = val;
}
//------------------------------------------------------------------------------
/*void CDChanPanel::SetImageFileName(CString str)
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
void CDChanPanel::SetCDSRV(CDSRV *srv)
{
	num_device = 1;
	if (m_psrv != srv)
		m_psrv = srv;
	device[0] =  m_psrv->GroupName(0);
	for (int i = 0; i < m_psrv->QuanChan(); i++)
		for (int j = 0; j < num_device; j++)
			if (device[j] != m_psrv->GroupName(i))
			{
				num_device ++;
				device[num_device] != m_psrv->GroupName(i);
			}
}
//------------------------------------------------------------------------------
void CDChanPanel::SetBorderWidth(int val)
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
void CDChanPanel::SetBorderColor(COLORREF val)
{
	if (m_param.BorderColor != val)
	{
		m_param.BorderColor = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDChanPanel::SetBorderColorFocus(COLORREF val)
{
	if (m_param.BorderColorFocus != val)
	{
		m_param.BorderColorFocus = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDChanPanel::SetSizeText(int val)
{
	if (m_param.SizeText != val)
	{
		m_param.SizeText = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDChanPanel::SetCaption(CString str)
{
	if (m_param.Caption != str)
	{
		m_param.Caption = str;
		m_Text = m_param.Caption;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDChanPanel::SetCaptionChange(CString str)
{
	if (m_param.CaptionChange != str)
	{
		m_param.CaptionChange = str;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDChanPanel::GetFocus()
{
	m_BorderColorCur = m_param.BorderColorFocus;
	SetBorderWidth(2);
	CCustomDraw::GetFocus();
}
//------------------------------------------------------------------------------
void CDChanPanel::KillFocus()
{
	m_BorderColorCur = m_param.BorderColor;
	SetBorderWidth(1);
	CCustomDraw::KillFocus();
}
//------------------------------------------------------------------------------
void CDChanPanel::OnMouseEnter()
{
		//m_pImage_Cur = m_pImage_m;
		m_BackColorCur = m_param.BackColorMouse;
	CCustomDraw::OnMouseEnter();
}
//------------------------------------------------------------------------------
void CDChanPanel::OnMouseLeave()
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
void CDChanPanel::OnLButtonClk(UINT nFlags, CPoint point)
{

	IsChange();
}
void CDChanPanel::ClickButton()
{
	m_bEvent = true;
	if (m_bEvent)
	{
		if (m_bFixation)
			m_bFixation = false;
		else
			m_bFixation = true;
		m_param.bAnimation = true;
		m_param.dTimeCur = 0.;
		m_param.dTimeStart = GetTime();
	}
	IsChange();
}
//------------------------------------------------------------------------------
void CDChanPanel::DoProcess()
{
	if (m_param.bAnimation)
	{
		m_param.dTimeCur = GetTime() - m_param.dTimeStart;
		if (m_param.dTimeCur <= m_param.dTimeAnimation)
		{
			if (m_bFixation)
			{
			rect_panel.bottom = rect_panel.top + Width*m_param.dTimeCur/m_param.dTimeAnimation;
			}
			else
			{
			rect_panel.bottom = rect_panel.top + Width*(1 - m_param.dTimeCur/m_param.dTimeAnimation);
			}
		}
		else
		{
			m_param.bAnimation = false;
			m_bEvent = false;
			if (m_bFixation)
				rect_panel.bottom = rect_panel.top + Width;
			else
			{
			rect_panel.bottom = rect_panel.top;
			}
		}
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDChanPanel::SetTimeAnimation(double val)
{
	if (val > 0.)
		m_param.dTimeAnimation = val;
}
//------------------------------------------------------------------------------
void CDChanPanel::SetPanelRect(CRect rect)
{
	if (rect != rect_panel)
		rect = rect_panel;
}
//------------------------------------------------------------------------------
#ifdef _GDIPLUS
void CDChanPanel::SetAlignmentHorizontal(StringAlignment val)
{
	if (m_param.AlignmentHorizontal != val)
	{
		m_param.AlignmentHorizontal = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDChanPanel::SetAlignmentVertical(StringAlignment val)
{
	if (m_param.AlignmentVertical != val)
	{
		m_param.AlignmentVertical = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
/*void CDChanPanel::OnDraw()
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
void CDChanPanel::OnDraw()
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
void CDChanPanel::OnDraw()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, m_param.mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m_param.mat_diffuse);
	glMaterialfv(GL_FRONT, GL_EMISSION, m_param.mat_emission);
	glMaterialfv(GL_FRONT, GL_SPECULAR, m_param.mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, m_param.mat_shininess);
	// граница
	FillRectSolid(&rect_panel, 2, m_BorderColorCur);

	// рамка
	/*FillRectSolid(&CRect(m_rect1.left, m_rect1.top, INT(m_rect1.right - m_rect1.Width()/4 - m_param.BorderWidth), m_rect1.bottom), 0.1, m_BackColorCur);*/
	//FillRectSolid(&CRect(INT(m_rect1.right - m_rect1.Width()/4), m_rect1.top, m_rect1.right, m_rect1.bottom),0.2, m_BackColorCur);
	// треугольник
	int num = m_psrv->QuanChan() + num_device + 1;
	float delta = rect_panel.Height()/num;
	// text
	/*if ((m_bFixation) && (!m_param.bAnimation))*/
	{
		glDisable(GL_DEPTH_TEST);
		glColor3f(1.0f,1.0f,1.0f);
		glEnable(GL_COLOR_MATERIAL);
		for (int i = 0; i < num_device; i++)
		{
			m_param.pFont->DrawString(CRect(rect_panel.left + delta , rect_panel.top + delta*i,
				rect_panel.right, rect_panel.top + delta*(i+1)), device[i]);
			FillRectSolid(&CRect(rect_panel.left, rect_panel.top + delta*i,
				rect_panel.left + delta, rect_panel.top + delta*(i+1)), 2.1, RGB(255,255,255));
		}
		for (int i = 0; i < num; i++)
			m_param.pFont->DrawString(CRect(rect_panel.left, rect_panel.top + delta*(i + 2),
				rect_panel.right, rect_panel.top + delta*(i + 3)), m_psrv->Commentary(i), StrAlignmentCenter, StrAlignmentCenter);
		glDisable(GL_COLOR_MATERIAL);
		glEnable(GL_DEPTH_TEST);
	}
}
#endif