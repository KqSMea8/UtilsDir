//------------------------------------------------------------------------------
// DButton.cpp : файл реализации
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\Draw\DButton.h"
//------------------------------------------------------------------------------
int g_nButton = 0;		// счётчик
int CDButton::m_number_buttons = 0;
//------------------------------------------------------------------------------
#ifdef _GDIPLUS
CDButton::CDButton(CRect rect, PVOID ppGraphics,
	bool *pFlagIsChange, double *pTimeAnimation) :
CCustomDraw(rect, ppGraphics, pFlagIsChange, pTimeAnimation)
#else
CDButton::CDButton(CRect rect,	bool *pFlagIsChange, double *pTimeAnimation) :
CCustomDraw(rect, pFlagIsChange, pTimeAnimation)
#endif
{
	m_pParamDraw = &m_param;	// обеспечение ОБЩИХ параметров
	m_param.nElement = ++g_nButton;
	m_param.bMouseChange = true;
	m_CheckColorCur = m_param.CheckColor;
	m_BackColorCur = m_param.BackColor;
	m_BorderColorCur = m_param.BorderColor;
	m_bEvent = false;
	m_bFixation = false;
	SetBorderWidth(1);
	SetNameWithNumber(L"Button");
	m_param.Caption = m_param.Name;
	SetNameWithNumber(L"ButtonChange");
	m_param.CaptionChange = m_param.Name;
	m_Text = m_param.Caption;
	m_CheckBox = nullptr;
#ifdef _GDIPLUS
	m_param.pFont = new CZFont();
	m_pImage_Cur =
		m_pImage_pm =
		m_pImage_s =
		m_pImage_m =
		m_pImage_p = NULL;
#else
	m_param.pFont = new CDFonts("Arialbd", m_param.SizeText , FontsStyleRegular, RGB(255,255,255));	
	m_Border = m_Button = m_CheckBox = nullptr;

#ifdef _ACTIVEX_ZIS
	m_Border = new CDetail(long(11), (rect.right + rect.left)/2, (rect.bottom + rect.top)/2, 0, 0, rect.right - rect.left, rect.bottom - rect.top, 1);

	m_Button = new CDetail(long(12), (rect.right + rect.left)/2, (rect.bottom + rect.top)/2, 0, 0, rect.right - rect.left, rect.bottom - rect.top, 1);//*/
#else
	m_Border = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Посадочное кнопки квадратной 23х23х7.stl",
		(rect.right + rect.left)/2, (rect.bottom + rect.top)/2, 0, 0, 
		rect.right - rect.left, rect.bottom - rect.top, 1);

	m_Button = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Кнопка квадратная 22,5х22,5х7.stl",
		(rect.right + rect.left)/2, (rect.bottom + rect.top)/2, 0, 0, 
		rect.right - rect.left, rect.bottom - rect.top, 1);//*/
#endif

	float x,y;
	m_Button->GetOffset(x,y,m_param.generalDepth);
	m_param.posZ = m_param.generalDepth;

	SetImageFileName(_T("C:\\ZETTools\\Interface2012\\Textures\\Patterns\\metal_mart14.bmp"));
	
	m_rect = rect;
	m_number_buttons++;
	//TRACE (L"Number of buttons = %i\n", m_number_buttons);
	m_param.rectForCaption = rect;
	m_param.rectForCaption.left += 3;
	m_param.rectForCaption.top += 3;
	m_param.rectForCaption.right -= 3;
	m_param.rectForCaption.bottom -= 3;
 	m_param.pFont->SetAligmentHorizontal(StrAlignmentCenter);
	//SetAligmentHorizontal(StrAlignmentNear);
 	m_param.pFont->SetAligmentVertical(StrAlignmentCenter);
//	m_param.pFont->SetColor(RGB(0, 0, 0));
#endif
}
//------------------------------------------------------------------------------
CDButton::CDButton(CRect rect, bool *pFlagIsChange, double *pTimeAnimation, CString ImageFile, int NumberTexture) :
CCustomDraw(rect, pFlagIsChange, pTimeAnimation)
{
	m_pParamDraw = &m_param;	// обеспечение ОБЩИХ параметров
	m_param.nElement = ++g_nButton;
	m_param.bMouseChange = true;
	m_CheckColorCur = m_param.CheckColor;
	m_BackColorCur = m_param.BackColor;
	m_BorderColorCur = m_param.BorderColor;
	m_bEvent = false;
	m_bFixation = false;
	SetBorderWidth(1);
	SetNameWithNumber(L"Button");
	m_param.Caption = m_param.Name;
	SetNameWithNumber(L"ButtonChange");
	m_param.CaptionChange = m_param.Name;
	m_Text = m_param.Caption;
	m_CheckBox = NULL;
#ifdef _GDIPLUS
	m_param.pFont = new CZFont();
	m_pImage_Cur =
		m_pImage_pm =
		m_pImage_s =
		m_pImage_m =
		m_pImage_p = NULL;
#else
	m_param.pFont = new CDFonts("Arialbd", m_param.SizeText, FontsStyleRegular, RGB(255, 255, 255));
	m_Border = m_Button = m_CheckBox = nullptr;

#ifdef _ACTIVEX_ZIS
	m_Border = new CDetail(long(11), (rect.right + rect.left) / 2, (rect.bottom + rect.top) / 2, 0, 0, rect.right - rect.left, rect.bottom - rect.top, 1);

	m_Button = new CDetail(long(12), (rect.right + rect.left) / 2, (rect.bottom + rect.top) / 2, 0, 0, rect.right - rect.left, rect.bottom - rect.top, 1);//*/
#else
	m_Border = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Посадочное кнопки квадратной 23х23х7.stl",
		(rect.right + rect.left) / 2, (rect.bottom + rect.top) / 2, 0, 0,
		rect.right - rect.left, rect.bottom - rect.top, 1);

	m_Button = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Кнопка квадратная 22,5х22,5х7.stl",
		(rect.right + rect.left) / 2, (rect.bottom + rect.top) / 2, 0, 0,
		rect.right - rect.left, rect.bottom - rect.top, 1);//*/
#endif

	float x, y;
	m_Button->GetOffset(x, y, m_param.generalDepth);
	m_param.posZ = m_param.generalDepth;
	m_param.Number_Texture = NumberTexture;
	SetImageFileNameHMME(ImageFile);

	m_rect = rect;
	m_number_buttons++;
	//TRACE (L"Number of buttons = %i\n", m_number_buttons);
	m_param.rectForCaption = rect;
	m_param.rectForCaption.left += 3;
	m_param.rectForCaption.top += 3;
	m_param.rectForCaption.right -= 3;
	m_param.rectForCaption.bottom -= 3;
	m_param.pFont->SetAligmentHorizontal(StrAlignmentCenter);
	//SetAligmentHorizontal(StrAlignmentNear);
	m_param.pFont->SetAligmentVertical(StrAlignmentCenter);
	//	m_param.pFont->SetColor(RGB(0, 0, 0));
#endif
}
//------------------------------------------------------------------------------
void CDButton::Clear (void)
{
	if (m_Button)
		m_Button->Clear();
	if (m_Border)
		m_Border->Clear();
	if (m_CheckBox)
		m_CheckBox->Clear();
	m_param.pFont->Clear();
	m_number_buttons--;
	TRACE (L"Clearing buttons = %i\n", m_number_buttons);
	//glDeleteTextures(1,&Texture);
}
//------------------------------------------------------------------------------
CDButton::~CDButton()
{
#ifdef _GDIPLUS
	if (m_param.pFont)
	{
		delete m_param.pFont;
	}
	DeleteGdiPlusObject(m_pImage_s);
	DeleteGdiPlusObject(m_pImage_m);
	DeleteGdiPlusObject(m_pImage_p);	
	DeleteGdiPlusObject(m_pImage_pm);
#else
	delete m_param.pFont;
	delete m_Border;
	delete m_Button;
	if (m_CheckBox != NULL)
		delete m_CheckBox;
#endif
}
//------------------------------------------------------------------------------
void CDButton::SetImageFileName(CString str)
{
	if ( !str.IsEmpty() && (m_param.fileName != str) )
	{
		m_param.fileName = str;
	#ifdef _GDIPLUS
		LoadImageFromFile(&m_pImage_s, str + L"_s");
		LoadImageFromFile(&m_pImage_m, str + L"_m");
		LoadImageFromFile(&m_pImage_p, str + L"_p");
		LoadImageFromFile(&m_pImage_pm, str + L"_pm");
		m_pImage_Cur = m_pImage_s;
	#else
		Texture = LoadGLTextures(m_param.fileName, PicSize, m_param.Number_Texture);
        m_Button->CalcTexCoors(PicSize[0], PicSize[1]);
	#endif
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDButton::SetImageFileNameHMME(CString str)
{
	if (!str.IsEmpty() && (m_param.fileName != str))
	{
		m_param.fileName = str;
#ifdef _GDIPLUS
		LoadImageFromFile(&m_pImage_s, str + L"_s");
		LoadImageFromFile(&m_pImage_m, str + L"_m");
		LoadImageFromFile(&m_pImage_p, str + L"_p");
		LoadImageFromFile(&m_pImage_pm, str + L"_pm");
		m_pImage_Cur = m_pImage_s;
#else
		Texture = LoadGLTextures(m_param.fileName, PicSize, m_param.Number_Texture);
		m_Button->CalcTexCoordHMME(PicSize[0], PicSize[1]);
#endif
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDButton::SetBorderWidth(int val)
{
	if (m_param.BorderWidth != val)
	{
		m_param.BorderWidth = val;
		DefineRect2(&m_rect, &m_rect2, val);
		DefineRect3(&m_rect, &m_rect1);
		DefineRect2(&m_rect1, &m_rect3, val);
		DefineRect2(&m_rect3, &m_rect4, 1);
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDButton::SetBorderColor(COLORREF val)
{
	if (m_param.BorderColor != val)
	{
		m_param.BorderColor = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDButton::SetBorderColorFocus(COLORREF val)
{
	if (m_param.BorderColorFocus != val)
	{
		m_param.BorderColorFocus = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDButton::SetSizeText(int val)
{
	if (m_param.SizeText != val)
	{
		m_param.pFont->SetSize(val);
		m_param.SizeText = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDButton::SetCaption(CString str)
{
	if (m_param.Caption != str)
	{
		m_param.Caption = str;
		m_Text = m_param.Caption;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDButton::SetCaptionChange(CString str)
{
	if (m_param.CaptionChange != str)
	{
		m_param.CaptionChange = str;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDButton::GetFocus()
{
	m_BorderColorCur = m_param.BorderColorFocus;
	SetBorderWidth(2);
	CCustomDraw::GetFocus();
}
//------------------------------------------------------------------------------
void CDButton::KillFocus()
{
	m_BorderColorCur = m_param.BorderColor;
	SetBorderWidth(1);
	CCustomDraw::KillFocus();
}
//------------------------------------------------------------------------------
void CDButton::OnMouseEnter()
{
#ifdef _GDIPLUS
		if (!m_param.CheckBox)
			m_pImage_Cur = m_pImage_m;
		else
		{
			m_BackColorCur = m_param.BackColorMouse;
			m_CheckColorCur = m_param.CheckColorMouse;
		}
#endif
	// у элементов ChanSelector'a собственная рекция на наведение мыши
	if (m_param.ButtonType != ChanSelectorMenuButton && m_param.ButtonType != ChanSelectorScroll)
	{
		for (int i = 0; i < 3; i++)
		{
			m_param.mat_ambient[i] = 0.0f;
			m_param.mat_ambient_b[i] = 0.0f;
			m_param.mat_emission[i] = 0.0f;
		}
		m_param.mat_ambient[0] += 60.0f/255.f; // при наведении мыши, кнопка с бортиком начинает немного подсвечиваться
		m_param.mat_ambient[1] += 87.f/255.f;
		m_param.mat_ambient[2] += 103.f/255.f;
	}
	else if (m_param.ButtonType == ChanSelectorScroll)
	{
		m_param.mat_ambient[0] += 1.f / 255.f;
		m_param.mat_ambient[1] += 203.f / 255.f;
		m_param.mat_ambient[2] += 247.f / 255.f;
	}
	else
	{
		if (m_param.ActiveBut == true)
			SetTextColor (255, 90, 90);
		m_param.mat_ambient[0] -= 0.3; 
		m_param.mat_ambient[1] -= 0.15;
		//m_param.mat_ambient[2] -= 0.05;
	}
	IsChange();
	CCustomDraw::OnMouseEnter();
}
//------------------------------------------------------------------------------
void CDButton::OnMouseLeave()
{
#ifdef _GDIPLUS	
	if (m_bFixation == false)
	{
		m_pImage_Cur = m_pImage_s;
		m_BackColorCur = m_param.BackColorEnter;
		m_CheckColorCur = m_param.CheckColor;
	}
	else 
	{	
		m_pImage_Cur = m_pImage_p;
		m_BackColorCur = m_param.BackColorEnter;
		m_CheckColorCur = m_param.CheckColor;
	}
#endif
	if (m_param.ButtonType != ChanSelectorMenuButton && m_param.ButtonType != ChanSelectorScroll)
	{
		for (int i = 0; i < 3; i++)
			m_param.mat_emission[i] = 0.3f;
		m_param.mat_ambient[0] -= 60.0f/255.f; //
		m_param.mat_ambient[1] -= 87.f/255.f;
		m_param.mat_ambient[2] -= 103.f/255.f;
	}
	else if (m_param.ButtonType == ChanSelectorScroll)
	{
		m_param.mat_ambient[0] -= 1.f / 255.f;
		m_param.mat_ambient[1] -= 203.f / 255.f;
		m_param.mat_ambient[2] -= 247.f / 255.f;
	}
	else 
	{
		if (m_param.ActiveBut == true)
			SetTextColor(1, 203, 247);
		m_param.mat_ambient[0] += 0.3; 
		m_param.mat_ambient[1] += 0.15;
		//m_param.mat_ambient[2] += 0.05;
	}

	IsChange();
	CCustomDraw::OnMouseLeave();
}
//------------------------------------------------------------------------------
void CDButton::SetTimeAnimation(double val)
{
	if (val > 0.)
		m_param.dTimeAnimation = val;
}
//------------------------------------------------------------------------------
void CDButton::OnLButtonClk(UINT nFlags, CPoint point)
{
#ifdef _GDIPLUS
	if (m_param.ButtonType)
	{// Кнопка без фиксации
		m_param.bAnimation = true;
		m_param.dTimeCur = 0.;
		m_param.dTimeStart = GetTime();
	}
	else
	{// Кнопка с фиксацией
		if (m_bFixation)
		{
			m_BackColorCur = m_param.BackColorEnter;
			if (!m_param.CheckBox)
			m_param.Caption = m_Text;
			m_bFixation = false;
		}		
		else
		{
			m_BackColorCur = m_param.BackColor;
			if (!m_param.CheckBox)
			m_param.Caption = m_param.CaptionChange;
			m_bFixation = true;
		}
	}
	IsChange();
#else
	
/* //Менять STL миодель кнопки
	CString str;
	CFileDialog fileDialog(TRUE,NULL,_T("*.stl"));	// объект класса выбора файла
	int result = fileDialog.DoModal();			// запустить диалоговое окно
	if (result==IDOK)							// если файл выбран
	{
		m_param.fileName = fileDialog.GetPathName(); // показать полный путь
		int iNeeded = WideCharToMultiByte( CP_ACP, 0, m_param.fileName, -1, NULL, 0, NULL, NULL);
		char* psz = new char[iNeeded];
		WideCharToMultiByte( CP_ACP, 0, m_param.fileName, -1, psz, iNeeded, NULL, NULL);
		m_Button = new CDetail(psz, (m_rect.left + m_rect.right)/2,
			(m_rect.bottom + m_rect.top)/2, 0, 1, 
			m_rect.right - m_rect.left, m_rect.bottom - m_rect.top, 1);
		//ChangeSTLModel(psz);
	}*/
	if (m_param.Active)
	{
	if (m_param.ButtonType == NotFixed)
	{// Кнопка без фиксации

		m_param.bAnimation = true;
		m_param.dTimeCur = 0.;
		m_param.dTimeStart = GetTime();	
		
	}
	else if (m_param.ButtonType == Fixed)
	{// Кнопка с фиксацией
		m_param.bAnimation = true;
		m_param.dTimeCur = 0.;
		m_param.dTimeStart = GetTime();
			if (m_bFixation && m_param.ButtonAnimation == AnimationOff)
			{
					m_param.posZ = m_param.generalDepth;
					m_Button->SetOffsetZ(m_param.posZ);
					m_bFixation = false;
				
			}		
			else if (!m_bFixation && m_param.ButtonAnimation == AnimationOff)
			{
					m_param.posZ = m_param.maxDepth;
					m_Button->SetOffsetZ(m_param.posZ);
					m_bFixation = true;
			}
	
	//TRACE("************************\n ButtonType: %d \n m_bFixation:%d \n ", m_param.ButtonType, m_bFixation);
	}
	else if (m_param.ButtonType == CheckBox)
	{
		if(m_bFixation)
		{
 			m_bFixation = false;
		}
		else
		{
 			m_bFixation = true;
		}
	}
	else if (m_param.ButtonType == RadioButton)
	{
		if(m_bFixation)
		{
			m_bFixation = false;
		}
		else
		{
			m_bFixation = true;
		}
	}
	else if (m_param.ButtonType == ChanSelectorMenuButton)
	{
		if (m_param.isClicked)
		{
			m_param.isClicked = false;
		}
		else
		{
			m_param.isClicked = true;
		}
	}
	}
	IsChange();
#endif	

}
//------------------------------------------------------------------------------
void CDButton::DoProcess()
{

	if (m_param.ButtonType == ChanSelectorMenuButton)
	{
		if (m_bFixation)
		{
			m_param.Caption = m_param.CaptionChange;
		}
		else
		{
			m_param.Caption = m_Text;
		}
	}

	if (m_param.bAnimation)
	{
		m_param.fValueCur = 
		float( (GetTime() - m_param.dTimeStart) / m_param.dTimeAnimation );		
		if(m_param.ButtonType == NotFixed)
		{
		//изменение цвета, без изменения глубины кнопки
			if(m_param.ButtonColoring == ColoringOn && m_param.ButtonAnimation == AnimationOff)
			{
				if (m_bFixation)
				{
					m_param.mat_diffuse_b[1] += m_param.fValueCur; //затухание кнопки 
					if (m_param.mat_diffuse_b[1] >= 1.0) 
					{
						m_bFixation = false;
						m_param.bAnimation = false;
										}
				}
				else 
				{	m_param.mat_diffuse_b[1] -= m_param.fValueCur; 
					if (m_param.mat_diffuse_b[1] <= 0.0) 
					{
						m_bFixation = true;
						m_param.dTimeCur = 0.;
						m_param.dTimeStart = GetTime();					
					}
				
				}
			}
			//изменение цвета кнопки при ее движении
			if(m_param.ButtonColoring == ColoringOn && m_param.ButtonAnimation == AnimationOn)
			{
				if (m_bFixation)
				{
					m_param.mat_diffuse_b[1] += m_param.fValueCur; //затухание кнопки 
					if (m_param.mat_diffuse_b[1] >= 1.0) m_param.mat_diffuse_b[1] = 1.0;			
				}
				else 
				{	
					m_param.mat_diffuse_b[1] -= m_param.fValueCur;
					if (m_param.mat_diffuse_b[1] <= 0.0) m_param.mat_diffuse_b[1] = 0.0;
				}
			}
			//анимация движения кнопки без фиксации
			if (m_param.ButtonAnimation == AnimationOn)
			{
				if (m_bFixation)
				{
					m_param.ScaleStep = m_param.fValueCur*m_param.maxDepth;
					m_param.posZ -= m_param.ScaleStep;
					m_Button->SetOffsetZ(m_param.posZ);
					
					
						m_param.SizeText -=  m_param.ScaleStep*2;
						m_param.pFont->SetSize(m_param.SizeText);
					
					
					if (m_param.posZ >= m_param.generalDepth) {
						m_param.posZ = m_param.generalDepth;
						m_param.bAnimation = false;
						m_bFixation = false;
					}
				}
				else 
				{
					m_param.ScaleStep = m_param.fValueCur*m_param.maxDepth;
					m_param.posZ += m_param.ScaleStep;
					m_Button->SetOffsetZ(m_param.posZ);

						m_param.SizeText +=  m_param.ScaleStep*2;
						m_param.pFont->SetSize(m_param.SizeText);
					

					if (m_param.posZ <= m_param.maxDepth)
					{
						m_param.posZ = m_param.maxDepth;
						m_bFixation = true;
						m_param.bAnimation = true;
						m_param.dTimeCur = 0.;
						m_param.dTimeStart = GetTime();
					}
				}
			}
			
		}
		else if (m_param.ButtonType == Fixed)
		{
			//изменение цвета кнопки при нажатии без анимации
			if(m_param.ButtonColoring == ColoringOn && m_param.ButtonAnimation == AnimationOff)
			{
				if (m_bFixation)
				{
					m_param.Caption = m_param.CaptionChange;
					m_param.mat_diffuse_b[1] -= m_param.fValueCur; 
					m_param.SizeText = 15;
					m_param.pFont->SetSize(m_param.SizeText);
					if (m_param.mat_diffuse_b[1] <= 0.2) 
					{
						
						m_param.bAnimation = false;
					}
				}

				else 
				{
					m_param.Caption = m_Text;
					m_param.mat_diffuse_b[1] += m_param.fValueCur; 
					m_param.SizeText = 17;
					m_param.pFont->SetSize(m_param.SizeText);
					
					if (m_param.mat_diffuse_b[1] >= 1.0) 
						{
							m_param.bAnimation = false;
							
						}
				
				}
			}
			//изменение цвета кнопки при нажатии с анимацией
			else if(m_param.ButtonColoring == ColoringOn && m_param.ButtonAnimation == AnimationOn)
			{
				if (m_bFixation)
				{
					m_param.Caption = m_Text;
					m_param.mat_diffuse_b[1] += m_param.fValueCur;  
					if (m_param.mat_diffuse_b[1] >= 1.0) m_param.mat_diffuse_b[1] = 1.0;
				}
				else 
				{
					m_param.Caption =  m_param.CaptionChange;
					m_param.mat_diffuse_b[1] -= m_param.fValueCur; 
					if (m_param.mat_diffuse_b[1] <= 0.2) m_param.mat_diffuse_b[1] = 0.2;
				}
			}
			//анимация кнопки с фиксацией
			else if (m_param.ButtonAnimation == AnimationOn)
			{
				if (m_bFixation)// при отжатии
				{
					m_param.Caption = m_Text;
					m_param.ScaleStep = m_param.fValueCur*m_param.maxDepth;
					m_param.posZ -= m_param.ScaleStep;
					m_Button->SetOffsetZ(m_param.posZ);
					
				
						m_param.SizeText -=  m_param.ScaleStep*2;
						m_param.pFont->SetSize(m_param.SizeText);
					

					if (m_param.posZ >= m_param.generalDepth) 
					{
						m_param.posZ = m_param.generalDepth;
						m_param.bAnimation = false;
						m_bFixation = false;
					}
				}
				else // при нажатии
				{
					m_param.Caption =  m_param.CaptionChange;
					m_param.ScaleStep = m_param.fValueCur*m_param.maxDepth;
					m_param.posZ += m_param.ScaleStep;
					m_Button->SetOffsetZ(m_param.posZ);


						m_param.SizeText += m_param.ScaleStep*2;
						m_param.pFont->SetSize(m_param.SizeText);
					

					if (m_param.posZ <= m_param.maxDepth)
					{
						m_param.posZ = m_param.maxDepth;
						m_param.bAnimation = false;
						m_bFixation = true;
					}
				}		
			}
			else if (m_param.ButtonColoring == ColoringOff && m_param.ButtonAnimation == AnimationOff)
			{
				if (m_bFixation)// при отжатии
				{
					m_param.Caption = m_Text;
				}
				else // при нажатии
				{
					m_param.Caption =  m_param.CaptionChange;
				}		

			}
		}
	
		IsChange();
	}
}
//------------------------------------------------------------------------------
#ifdef _GDIPLUS
void CDButton::SetAlignmentHorizontal(StringAlignment val)
{
	if (m_param.AlignmentHorizontal != val)
	{
		m_param.AlignmentHorizontal = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDButton::SetAlignmentVertical(StringAlignment val)
{
	if (m_param.AlignmentVertical != val)
	{
		m_param.AlignmentVertical = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDButton::OnDraw()
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
}
#else
void CDButton::OnDraw()
{// отображение
	if (m_param.bVisible == true)
	{ 
		if (!m_param.Active) 
			m_param.mat_shininess = 200.0f; 		
		glPushAttrib(GL_LIGHTING_BIT);
		glMaterialfv(GL_FRONT, GL_AMBIENT, m_param.mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, m_param.mat_diffuse);
		glMaterialfv(GL_FRONT, GL_EMISSION, m_param.mat_emission);
		glMaterialfv(GL_FRONT, GL_SPECULAR, m_param.mat_specular);
		glMaterialf(GL_FRONT, GL_SHININESS, m_param.mat_shininess);

		glBindTexture(GL_TEXTURE_2D, Texture);

		switch (m_param.ButtonType)
		{
		case Fixed:
			{
				m_Border->Draw(m_Border->ReadFacet(), m_Border->GetQntPlanes());
				m_Button->Draw_texture(m_Button->ReadFacet(), m_Button->GetQntPlanes(), PicSize, false);
				break;
			}
		case NotFixed:
			{
				m_Border->Draw(m_Border->ReadFacet(), m_Border->GetQntPlanes());
				m_Button->Draw_texture(m_Button->ReadFacet(), m_Button->GetQntPlanes(), PicSize, false);
				break;
			}
		case CheckBox:
			{
				m_Border->Draw(m_Border->ReadFacet(), m_Border->GetQntPlanes());
				m_Button->Draw_texture(m_Button->ReadFacet(), m_Button->GetQntPlanes(), PicSize, false);	
				if (m_bFixation==true) 
				{
					glMaterialfv(GL_FRONT, GL_EMISSION, m_param.mat_unit);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, m_param.mat_unit);
					glMaterialfv(GL_FRONT, GL_AMBIENT, m_param.mat_unit);
					m_CheckBox->Draw(m_CheckBox->ReadFacet(), m_CheckBox->GetQntPlanes());
				}
				break;
			}
		case RadioButton:
			{
				m_ButtonType = m_param.ButtonType;
				m_Border->Draw(m_Border->ReadFacet(), m_Border->GetQntPlanes());
				if(m_bFixation)
				{
					m_param.mat_diffuse_rb[0] = 1.0;
					m_param.mat_diffuse_rb[1] = 1.0;
					m_param.mat_diffuse_rb[2] = 1.0;
					m_param.mat_shininess_rb = 200.0f; 
				}
				else
				{
					m_param.mat_diffuse_rb[0] = 1.0;
					m_param.mat_diffuse_rb[1] = 1.0;
					m_param.mat_diffuse_rb[2] = 1.0;
					m_param.mat_shininess_rb = 0.0f; 
				}
				glMaterialf(GL_FRONT, GL_SHININESS, m_param.mat_shininess_rb);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, m_param.mat_diffuse_rb);
				m_Button->Draw_texture(m_Button->ReadFacet(), m_Button->GetQntPlanes(), PicSize, false);
				break;
			}
		case ComboboxMenuButton:
			{
				//glTranslatef(0,0,10.0f);
				FillParallepiped(&m_rect, 0.3, 0.2, m_param.BorderColor);
				FillParallepiped(&m_param.rectForCaption, 0.4, 0.3, m_param.BorderColor);
				//glPopMatrix(); 
				break;
			}
		case ChanSelectorScroll:
		case ChanSelectorMenuButton:
			{
				m_Button->SetScale(m_rect.right - m_rect.left, m_rect.bottom - m_rect.top,0.01);
				m_Button->Draw_texture(m_Button->ReadFacet(), m_Button->GetQntPlanes(), PicSize, false);
				break;
			}
		}
		if (m_param.ButtonType != CheckBox && m_param.ButtonType != RadioButton)
		{
			OnDrawText();
		}

		glPopAttrib();
		//glFlush ();
	}
}
//------------------------------------------------------------------------------
void CDButton::ReturnButtonToState (bool val)
{
	if (m_param.ButtonType == Fixed)
	{// Кнопка с фиксацией
		m_param.bAnimation = true;
		m_param.dTimeCur = 0.;
		m_param.dTimeStart = GetTime();
	}
	if (val)// при отжатии
	{
		m_param.ScaleStep = m_param.fValueCur*m_param.maxDepth;
		m_param.posZ -= m_param.ScaleStep;
		m_Button->SetOffsetZ(m_param.posZ);		
		m_param.SizeText -=  m_param.ScaleStep*2;
		m_param.pFont->SetSize(m_param.SizeText);
		if (m_param.posZ >= m_param.generalDepth) 
		{
			m_param.posZ = m_param.generalDepth;
			m_param.bAnimation = false;
			m_bFixation = false;
		}
	}
	else // при нажатии
	{
		m_param.ScaleStep = m_param.fValueCur*m_param.maxDepth;
		m_param.posZ += m_param.ScaleStep;
		m_Button->SetOffsetZ(m_param.posZ);
		m_param.SizeText += m_param.ScaleStep*2;
		m_param.pFont->SetSize(m_param.SizeText);
		if (m_param.posZ <= m_param.maxDepth)
		{
			m_param.posZ = m_param.maxDepth;
			m_param.bAnimation = false;
			m_bFixation = true;
		}
	}	
	IsChange();
}
//------------------------------------------------------------------------------
void CDButton::SetColor(GLfloat *ma, GLfloat *md, GLfloat *me, GLfloat *msp, GLfloat msh)
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
void CDButton::OnDrawText()
{
//	glDisable(GL_LIGHTING);
	//glDisable(GL_DEPTH_TEST);
	
	if (m_param.pFont->CalcSize(CRect(m_param.rectForCaption.left,
		m_param.rectForCaption.top,
		m_param.rectForCaption.right,
		m_param.rectForCaption.bottom), m_param.Caption) < m_param.SizeText)
	{
		m_param.CaptionCut = m_param.pFont->CutString(m_param.Caption);
		m_param.pFont->SetSize(m_param.SizeText);
		m_param.pFont->DrawString(CRect(m_param.rectForCaption.left,
			m_param.rectForCaption.top,
			m_param.rectForCaption.right,
			m_param.rectForCaption.bottom), m_param.CaptionCut);
	}
	else
	{
		m_param.pFont->DrawString(CRect(m_param.rectForCaption.left,
			m_param.rectForCaption.top,
			m_param.rectForCaption.right,
			m_param.rectForCaption.bottom), m_param.Caption);
	}
//	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_LIGHTING);
}
//------------------------------------------------------------------------------
void CDButton::SetTextColor( int r, int g, int b )
{
	m_param.pFont->SetColor(RGB(r, g, b));
}
//------------------------------------------------------------------------------
void CDButton::SetButtonType( eButtonType val )
{
	m_param.ButtonType = val;
	switch (m_param.ButtonType)
	{
	case CheckBox:
		{
#ifdef _ACTIVEX_ZIS
			m_CheckBox = new CDetail(long(40),
				(m_rect.right + m_rect.left)/2, (m_rect.bottom + m_rect.top)/2, 0.75, 0, 
				(m_rect.right - m_rect.left)/1.3, (m_rect.bottom - m_rect.top)/1.3, 1);
			break;
#else
			m_CheckBox = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Галочка.stl",
				(m_rect.right + m_rect.left)/2, (m_rect.bottom + m_rect.top)/2, 0.75, 0, 
				(m_rect.right - m_rect.left)/1.3, (m_rect.bottom - m_rect.top)/1.3, 1);
			break;
#endif
		}
	case RadioButton:
		{
#ifdef _ACTIVEX_ZIS
			m_Border = new CDetail(long(41),
				(m_rect.right + m_rect.left)/2, (m_rect.bottom + m_rect.top)/2, 0, 0, 
				m_rect.right - m_rect.left, m_rect.bottom - m_rect.top, 1);

			m_Button = new CDetail(long(42),
				(m_rect.right + m_rect.left)/2, (m_rect.bottom + m_rect.top)/2, 0, 0, 
				m_rect.right - m_rect.left, m_rect.bottom - m_rect.top, 1);
			break;
#else
			m_Border = new CDetail("C:\\ZETTools\\Interface2012\\Models\\ободок RadioButton.stl",
				(m_rect.right + m_rect.left)/2, (m_rect.bottom + m_rect.top)/2, 0, 0, 
				m_rect.right - m_rect.left, m_rect.bottom - m_rect.top, 1);

			m_Button = new CDetail("C:\\ZETTools\\Interface2012\\Models\\RadioButton.stl",
				(m_rect.right + m_rect.left)/2, (m_rect.bottom + m_rect.top)/2, 0, 0, 
				m_rect.right - m_rect.left, m_rect.bottom - m_rect.top, 1);
			break;
#endif
		}
		//combobox2014
	case ComboboxMenuButton:
		{
			m_Button->Clear();
			m_Border->Clear();
		}
	}
}
//------------------------------------------------------------------------------
void CDButton::SetRectForDraw( CRect val )
{
	this->SetRect(val);
	m_param.rectForCaption = val;
	m_param.rectForCaption.left += 3;
	m_param.rectForCaption.top += 3;
	m_param.rectForCaption.right -= 3;
	m_param.rectForCaption.bottom -= 3;
	m_Border->SetOffset((val.right + val.left)/2, (val.bottom + val.top)/2, 0);
	m_Button->SetOffset((val.right + val.left)/2, (val.bottom + val.top)/2, 0);
}
//------------------------------------------------------------------------------
void CDButton::SaveParameters(SimpleXML *pSimpleXML)
{
    CString cString;
    char String[16];

    sprintf_s(String, "%d", m_param.nElement);

    cString = CString(String);
    hXml hRoot = pSimpleXML->GetRootNode();
    hXml hInterfaceRoot = pSimpleXML->GetChildNode(hRoot);
    hXml hButtonRoot = pSimpleXML->AddElement(L"ZButton_"+cString, hInterfaceRoot);
	
    hXml hTextureRoot = pSimpleXML->AddElement(L"Texture", hButtonRoot);
	pSimpleXML->SetAttributeValue(hTextureRoot, L"ZISNumber_Texture", CComVariant(m_param.Number_Texture));
    pSimpleXML->SetAttributeValue(hTextureRoot, L"path", CComVariant(m_param.Texture_FileName));

    hXml hMaterialRoot = pSimpleXML->AddElement(L"Material", hButtonRoot);

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
void CDButton::LoadParameters(SimpleXML *pSimpleXML)
{
    VARIANT vAtrrib;
    CString str = L"";
    CString Pattern = L"ZButton_";
    char String[16];
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
    hXml hTextureRoot = pSimpleXML->GetChildNode(hElement);
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
    hXml hAmbientRoot = pSimpleXML->GetChildNode(hMaterialRoot);
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
    hXml hDiffuseRoot = pSimpleXML->GetNextNode(hAmbientRoot);
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
    hXml hSpecularRoot = pSimpleXML->GetNextNode(hDiffuseRoot);
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
#endif