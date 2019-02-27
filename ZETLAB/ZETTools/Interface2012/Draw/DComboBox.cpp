//------------------------------------------------------------------------------
// DComboBox.cpp : файл реализации
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\Draw\DComboBox.h"
//------------------------------------------------------------------------------
int g_nComboBox = 0;		// счётчик
//------------------------------------------------------------------------------
#ifdef _GDIPLUS 
CDComboBox::CDComboBox(CRect rect, PVOID ppGraphics,
				bool *pFlagIsChange, double *pTimeAnimation) :
				CCustomDraw(rect, ppGraphics, pFlagIsChange, pTimeAnimation)
#else 
CDComboBox::CDComboBox(CRect rect, bool *pFlagIsChange, double *pTimeAnimation) :
	CCustomDraw(rect, pFlagIsChange, pTimeAnimation)
#endif
{
	m_pParamDraw = &m_param;	// обеспечение ОБЩИХ параметров
	m_param.nElement = ++g_nComboBox;
	SetNameWithNumber(L"ComboBox");
	//TRACE("DELETE CDComboBox %s\n", (CStringA)m_pParamDraw->Name);
	m_param.bMouseChange = true;
	// переменные класса
	m_BackColorCur = m_param.BackColor;
	m_BorderColorCur = m_param.BorderColor;
	m_CounterItem = 0;
	m_menu.CreatePopupMenu();
	m_bViewMenu = false;
	m_bEvent = false;
	size = 25;
	_pNum = 0;
	DCount = 0;
	Invert = false;
	m_ButtonUp = nullptr;
	m_ButtonDown = nullptr;
	m_ScrollBar = nullptr;
#ifdef _GDIPLUS
	m_pFont = NULL;
	m_pFont = new CZFont(L"Arial", 9);
#else
	int Depth = 3;
	int size_p = 3;
	m_pFont = new CDFonts("Arialbd", 15.0 , FontsStyleRegular, RGB(23, 23, 23));
#ifdef _ACTIVEX_ZIS
	for (int i = 0; i < 30; i++)
		m_Button[i] = nullptr;

	m_pButton = new CDetail(long(9), m_rect.right - (m_rect.Height())/2, (m_rect.bottom + m_rect.top)/2.0f, 0.75, 180, m_rect.Height() - 5, m_rect.Height() - 5, 12);

	//левая часть панели
	m_pComboBox[0] = new CDetail(long(26), size_p/2.0f + m_rect.left, (m_rect.bottom + m_rect.top)/2.0f, -Depth, 0, size_p, m_rect.Height() - 2*size_p, Depth);			
	//левый верхний угол
	m_pComboBox[1] = new CDetail(long(27), size_p/2.0f + m_rect.left, size_p/2.0f + m_rect.top, -Depth, 0, size_p, size_p, Depth);
	//верхняя часть панели
	m_pComboBox[2] = new CDetail(long(28), (m_rect.right + m_rect.left)/2.0f, size_p/2.0f + m_rect.top, -Depth, 0, m_rect.Width() - 2.0f*size_p, size_p, Depth);
	//правый верхний угол
	m_pComboBox[3] = new CDetail(long(29), m_rect.right - size_p/2.0f, size_p/2.0f + m_rect.top, -Depth, 0, size_p, size_p, Depth);
	//правая часть панели
	m_pComboBox[4] = new CDetail(long(30), m_rect.right - size_p/2.0f,(m_rect.bottom + m_rect.top)/2.0f, -Depth, 0, size_p, m_rect.Height() - 2.0f*size_p, Depth);
	//правый нижний угол
	m_pComboBox[5] = new CDetail(long(31), m_rect.right - size_p/2.0f, m_rect.bottom - size_p/2.0f, -Depth, 0, size_p, size_p, Depth);
	//нижняя часть панели
	m_pComboBox[6] = new CDetail(long(32), (m_rect.right + m_rect.left)/2.0f, m_rect.bottom - size_p/2.0f, -Depth, 0, m_rect.Width() - 2.0f*size_p, size_p, Depth);
	//левый нижний угол
	m_pComboBox[7] = new CDetail(long(33), size_p/2.0f + m_rect.left, m_rect.bottom - size_p/2.0f, -Depth, 0, size_p, size_p, Depth);			
	//центр
	m_pComboBox[8] = new CDetail(long(34), (m_rect.right + m_rect.left)/2.0f, (m_rect.bottom + m_rect.top)/2.0f, -Depth, 0, m_rect.Width() - 2.0f*size_p, m_rect.Height() - 2.0f*size_p, Depth);//*/

	m_param.rect2.bottom = m_rect.bottom - size_p;
	m_param.rect2.top = m_rect.top + size_p;
	m_param.rect2.right = m_rect.right - size_p;
	m_param.rect2.left = m_rect.left + size_p;

	m_Border = new CDetail(long(10), (rect.right + rect.left)/2, (rect.bottom + rect.top)/2, 0.75, 0, rect.right - rect.left, rect.bottom - rect.top, 10);//*/
#else

	m_pButton = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Кнопка квадратная 22,5х22,5х7.stl", 
		m_rect.right - (m_rect.Height())/2, (m_rect.bottom + m_rect.top)/2.0f, 0.75, 180, m_rect.Height() - 5, m_rect.Height() - 5, 15);

	m_pComboBox[0] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Поддон\\Поддон левый.stl", 
		size_p/2.0f + m_rect.left, (m_rect.bottom + m_rect.top)/2.0f, -Depth, 0, size_p, m_rect.Height() - 2*size_p, Depth);			
	//левый верхний угол
	m_pComboBox[1] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Поддон\\Поддон левый нижний.stl", 
		size_p/2.0f + m_rect.left, size_p/2.0f + m_rect.top, -Depth, 0, size_p, size_p, Depth);
	//верхняя часть панели
	m_pComboBox[2] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Поддон\\Поддон нижний.stl", 
		(m_rect.right + m_rect.left)/2.0f, size_p/2.0f + m_rect.top, -Depth, 0, m_rect.Width() - 2.0f*size_p, size_p, Depth);
	//правый верхний угол
	m_pComboBox[3] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Поддон\\Поддон правый нижний.stl", 
		m_rect.right - size_p/2.0f, size_p/2.0f + m_rect.top, -Depth, 0, size_p, size_p, Depth);
	//правая часть панели
	m_pComboBox[4] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Поддон\\Поддон правый.stl", 
		m_rect.right - size_p/2.0f,(m_rect.bottom + m_rect.top)/2.0f, -Depth, 0, size_p, m_rect.Height() - 2.0f*size_p, Depth);
	//правый нижний угол
	m_pComboBox[5] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Поддон\\Поддон правый верхний.stl", 
		m_rect.right - size_p/2.0f, m_rect.bottom - size_p/2.0f, -Depth, 0, size_p, size_p, Depth);
	//нижняя часть панели
	m_pComboBox[6] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Поддон\\Поддон верхний.stl", 
		(m_rect.right + m_rect.left)/2.0f, m_rect.bottom - size_p/2.0f, -Depth, 0, m_rect.Width() - 2.0f*size_p, size_p, Depth);
	//левый нижний угол
	m_pComboBox[7] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Поддон\\Поддон левый верхний.stl", 
		size_p/2.0f + m_rect.left, m_rect.bottom - size_p/2.0f, -Depth, 0, size_p, size_p, Depth);			
	//центр
	m_pComboBox[8] = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Поддон\\Поддон центр.stl", 
		(m_rect.right + m_rect.left)/2.0f, (m_rect.bottom + m_rect.top)/2.0f, -Depth, 0, m_rect.Width() - 2.0f*size_p, m_rect.Height() - 2.0f*size_p, Depth);//*/

#endif
    SetTexture(m_param.Texture_1_FileName, false);
	SetTexture(m_param.Texture_2_FileName, true);
#endif
	SetBorderWidth(3);
}
//------------------------------------------------------------------------------
void CDComboBox::Clear (void)
{
	for (int i = 0; i < 9; i++)
		m_pComboBox[i]->Clear();
	for (int i = 0; i < _pNum; i++)
	{
		if (m_Button[i])
			m_Button[i]->Clear();
	}
	m_Border->Clear();
	m_pFont->Clear();
	m_pButton->Clear();
}
//------------------------------------------------------------------------------
CDComboBox::~CDComboBox()
{
	m_menu.DestroyMenu();
	if (m_pFont)
		delete m_pFont;
	if (m_Border)
		delete m_Border;
	if (m_pButton)
		delete m_pButton;
	for (int i = 0; i < 9; i++)
		delete m_pComboBox[i];
	for (int i = 0; i < _pNum; i++)
		delete m_Button[i];
}
//------------------------------------------------------------------------------
void CDComboBox::SetBorderWidth(int val)
{
	int n;
	if (m_param.BorderWidth != val)
	{
		m_param.BorderWidth = val;
		DefineRect2(&m_rect, &m_rect2, val);
		n = m_rect2.Height();
		n /= 3;
		m_param.pt[1].X = m_rect2.right - n;
		m_param.pt[2].X = m_rect2.right - m_rect2.Height() / 2;
		m_param.pt[0].X = m_param.pt[2].X - (m_param.pt[1].X - m_param.pt[2].X);
		m_param.pt[0].Y = m_param.pt[1].Y = m_rect2.top + n + 1;
		m_param.pt[2].Y = m_rect2.bottom - n + 1;
		m_rect2.right -= (m_rect2.bottom - m_rect2.top);
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDComboBox::SetBorderColor(COLORREF val)
{
	if (m_param.BorderColor != val)
	{
		m_param.BorderColor = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDComboBox::SetBorderColorFocus(COLORREF val)
{
	if (m_param.BorderColorFocus != val)
	{
		m_param.BorderColorFocus = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDComboBox::SetBackColor(COLORREF val)
{
	if (m_param.BackColor != val)
	{
		m_param.BackColor = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDComboBox::SetBackColorMouse(COLORREF val)
{
	if (m_param.BackColorMouse != val)
	{
		m_param.BackColorMouse = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
#ifdef _GDIPLUS
void CDComboBox::SetIndexCur(long val)
{
	if ( (m_param.IndexCur != val) && (val >= 0) && (val < m_CounterItem) )
	{
		if (m_param.IndexCur != -1)
			m_menu.CheckMenuItem(m_param.StartItemID + m_param.IndexCur,
									MF_UNCHECKED);
		m_param.IndexCur = val;
		UINT n = m_param.StartItemID + val;
		m_menu.CheckMenuItem(n, MF_CHECKED);
		m_menu.GetMenuStringW(n, m_param.Caption, MF_BYCOMMAND);	// MF_BYPOSITION
		IsChange();
	}
	else if (val == -1)
	{
		m_menu.CheckMenuItem(m_param.StartItemID + m_param.IndexCur,
									MF_UNCHECKED);
		m_param.Caption = L"";
		IsChange();
	}
}
#else
void CDComboBox::SetIndexCur(long val)
{
	if (   (val >= 0) && (val < NumberOfStrings) )//(m_param.IndexCur != val) &&
	{
		if (m_param.IndexCur >= 0 && m_param.IndexCur <= _pNum)
			m_Vector.at(m_param.IndexCur)->SetTextColor(53,53,53); //предыдущий выбранный элемент меняет цвет надписи цвет невыбранных элементов
		SetCaption(arrayOfNames[val]);
		m_param.IndexCur = val;
		m_Vector.at(m_param.IndexCur)->SetTextColor(1, 203, 247); //цвет выбранного элемента подсвечивается
		UINT n = m_param.StartItemID + val;
		IsChange();
	}
	else
	{
		m_param.IndexCur = 0;
	}
	if (val == -1)
	{
		m_param.Caption = L"";
		IsChange();
	}
}
#endif
//------------------------------------------------------------------------------
void CDComboBox::SetCaption(CString str)
{
	if (m_param.Caption != str)
	{
		m_param.Caption = str;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDComboBox::SetType(ComboBoxType val)
{
	if (m_param.Type != val)
	{
		m_param.Type = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDComboBox::SetTexture(CString path, bool texture)
{
    if ( !path.IsEmpty() && (m_param.m_textureName != path) )
    {
        m_param.m_textureName = path;
#ifdef _GDIPLUS
        LoadImageFromFile(&m_pImage, str);
#else
		if (texture == false)
		{
			Texture1 = LoadGLTextures(m_param.m_textureName, PicSize1, m_param.Number_Texture[0]);
			for(int i=0; i < DETAILS; i++)
				m_pComboBox[i]->CalcTexCoors(PicSize1[0], PicSize1[1]);
		}
		else
		{
			Texture2 = LoadGLTextures(m_param.m_textureName, PicSize2, m_param.Number_Texture[1]);
			if (texture == true)
				m_pButton->CalcTexCoordHMME(PicSize2[0], PicSize2[1]);
		}
#endif	
        IsChange();
    }
}
//------------------------------------------------------------------------------
void CDComboBox::GetFocus()
{
	m_BorderColorCur = m_param.BorderColorFocus;
	SetBorderWidth(2);
	CCustomDraw::GetFocus();
}
//------------------------------------------------------------------------------
void CDComboBox::KillFocus()
{
	m_BorderColorCur = m_param.BorderColor;
	SetBorderWidth(1);
	CCustomDraw::KillFocus();
}
//------------------------------------------------------------------------------
void CDComboBox::OnMouseEnter()
{
	m_BackColorCur = m_param.BackColorMouse;
	m_param.FieldSolidColor = RGB (255,255,255);
	m_param.mat_ambient[0] = 0.3f;
	m_param.mat_ambient[1] = 0.3f;
	m_param.mat_ambient[2] = 0.3f;
	m_param.mat_emission[0] = 1.f/255.f;
	m_param.mat_emission[1] = 101.f/255.f;
	m_param.mat_emission[2] = 128.f/255.f;
	CCustomDraw::OnMouseEnter();
}
//------------------------------------------------------------------------------
void CDComboBox::OnMouseLeave()
{
	m_BackColorCur = m_param.BackColor;
	m_param.FieldSolidColor = RGB (203,203,203);
	m_param.mat_ambient[0] = 0.2f;
	m_param.mat_ambient[1] = 0.2f;
	m_param.mat_ambient[2] = 0.2f;
	m_param.mat_emission[0] = 0.5f;
	m_param.mat_emission[1] = 0.5f;
	m_param.mat_emission[2] = 0.5f;
	CCustomDraw::OnMouseLeave();
}
//------------------------------------------------------------------------------
void CDComboBox::OnMouseWheel(UINT nflags, CPoint point, short delta)
{
	long indOld = m_param.IndexCur;
	if (delta < 0)
	{
		m_param.IndexCur++;
		if (m_param.IndexCur >= m_CounterItem)
			m_param.IndexCur = m_CounterItem - 1;
	}
	else
	{
		m_param.IndexCur--;
		if (m_param.IndexCur < 0)
			m_param.IndexCur = 0;
	}

	if (indOld != m_param.IndexCur)
	{
		if (indOld != -1)
		m_menu.CheckMenuItem(m_param.StartItemID + indOld, MF_UNCHECKED);
		UINT n = m_param.StartItemID + m_param.IndexCur;
		m_menu.GetMenuStringW(n, m_param.Caption, MF_BYCOMMAND);
		m_menu.CheckMenuItem(n, MF_CHECKED);
		IsChange();
	}
}
//------------------------------------------------------------------------------
#ifdef _GDIPLUS
void CDComboBox::SetTextString(CString val)
{	
	m_menu.DestroyMenu();
	m_menu.CreatePopupMenu();
	if ( !val.IsEmpty() )
	{
		UINT_PTR nID = m_param.StartItemID;	// ID new item
		CString s1, str = val;
		for(int i=0; i<m_param.NumberMax; i++)
		{// подстраховка, чтобы не было более 256 позиций
			int n = str.Find(L";");
			if (n != -1)
			{
				s1 = str;
				s1.Delete(n, s1.GetLength() - n);
				m_menu.AppendMenuW(MF_STRING, nID++, s1);
				str.Delete(0, n+1);
				//zettest
				arrayOfNames[i] = s1;
			}
			else
				break;
		}
	}
	m_CounterItem = m_menu.GetMenuItemCount();
	_pNum = m_CounterItem;
	IsChange();
}
//------------------------------------------------------------------------------
#else
void CDComboBox::SetTextString(CString val) 
{	
	if (_pNum > 0) DeleteMenuButtons();
	if ( !val.IsEmpty() )
	{
		_pNum = 0;
		UINT_PTR nID = m_param.StartItemID;	// ID new item
		CString s1, str = val;
		for(unsigned int i=0; i<m_param.NumberMax; i++)
		{// подстраховка, чтобы не было более 256 позиций
			int n = str.Find(L";");
			if (n != -1)
			{
				s1 = str;
				s1.Delete(n, s1.GetLength() - n);
				s1.Trim();
				arrayOfNames[i] = s1;
				str.Delete(0, n+1);			
				_pNum++;
			}
			else
				break;
		}
	}
	NumberOfStrings = _pNum;
	
	if (m_param.IndexCur >=0 ) m_param.Caption = arrayOfNames[m_param.IndexCur]; //написать правильную проверку!!

	_pMaxVisNum = (spaceRect.bottom - m_rect.bottom) / m_param.ItemHeight;
	if (_pNum > _pMaxVisNum)
	{
		m_bVisUD = true;
		_pVisNum = _pMaxVisNum;
	}
	else
	{
		m_bVisUD = false;
		_pVisNum = _pNum;
	}
	CreateMenuButtons();
	IsChange();
}
#endif
//------------------------------------------------------------------------------
void CDComboBox::EnableMenuItem(long index, bool bEnable)
{
	if ( (index >= 0) && (index < m_CounterItem) )
	{
		UINT flag;
		if (bEnable)
			flag = MF_ENABLED;
		else
			flag = MF_GRAYED;
		m_menu.EnableMenuItem(m_param.StartItemID + index, flag);
	}
}
//------------------------------------------------------------------------------
#ifdef _GDIPLUS
void CDComboBox::OnDraw()
{// отображение
	if (m_param.bVisible)
	{
		Graphics *pGraphics = *((Graphics**)m_ppGraphics);
		// граница
		FillRectSolid(pGraphics, &m_rect, m_BorderColorCur);
		// поле
		FillRectSolid(pGraphics, &m_rect2, m_BackColorCur);
		// треугольник
		SolidBrush sb( Color(190, 23, 23, 23) );
		pGraphics->FillPolygon(&sb, m_param.pt, 3);
		// text
		int n = m_rect2.Height();
		DrawString(pGraphics, m_param.Caption, m_pFont,
			RectF( (REAL)m_rect2.left, (REAL)m_rect2.top,
					(REAL)(m_rect2.Width() - n), (REAL)n ),
			StringAlignmentNear);
	}
}
//------------------------------------------------------------------------------
#else
void CDComboBox::OnDraw()
{// отображение
	if (m_param.bVisible)
	{
		glPushAttrib(GL_LIGHTING_BIT | GL_TEXTURE_BIT);

		glMaterialfv(GL_FRONT, GL_AMBIENT,   m_param.mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE,   m_param.mat_diffuse);
		glMaterialfv(GL_FRONT, GL_EMISSION,  m_param.mat_emission);
		glMaterialfv(GL_FRONT, GL_SPECULAR,  m_param.mat_specular);
		glMaterialf (GL_FRONT, GL_SHININESS, m_param.mat_shininess);
		switch (m_param.Type)
		{
			case cbtSimple :
			{
				FillParallepiped(&m_rect, 0.3, 0.2, m_param.BorderColor);
				FillParallepiped(&m_rect2, 0.4, 0.3, m_param.BackColor);
				break;
			}
			case cbtDropDown :
			{
				glBindTexture(GL_TEXTURE_2D, Texture2);
				m_pButton->Draw_HMME(m_pButton->ReadFacet(), m_pButton->GetQntPlanes(), PicSize2);
				glBindTexture(GL_TEXTURE_2D, Texture1); 
				glPushMatrix();
				for (int i = 0; i < DETAILS; i++)
					m_pComboBox[i]->Draw_texture(m_pComboBox[i]->ReadFacet(), m_pComboBox[i]->GetQntPlanes(), PicSize1, true);
				glPopMatrix();
				FillRectSolid(&m_param.rect2, 2.15, m_param.FieldSolidColor);
				break;
			}
			case cbtDropList :
			{
				glBindTexture(GL_TEXTURE_2D, Texture1); 
				for (int i = 0; i < DETAILS; i++)
					m_pComboBox[i]->Draw_texture(m_pComboBox[i]->ReadFacet(), m_pComboBox[i]->GetQntPlanes(), PicSize1, true);
				break;
			}
		}
		m_pFont->DrawString(m_rect2, m_param.Caption, StrAlignmentCenter, StrAlignmentCenter);
		glPopAttrib();
	}
	if (m_ScrollBar->GetVisible())
		FillParallepiped(&CRect(m_rect.left, m_rect.bottom,
		m_rect.right + m_param.ItemHeight - 1, m_rect.bottom + m_param.ItemHeight*_pMaxVisNum ), 3.0, 3.0, m_param.BorderColor);
	if (flagScroll && m_ScrollBar->GetNowUsing())
	{
		DCount = m_ScrollBar->GetValueCur() * (m_Vector.size() - _pMaxVisNum);
		ReDrawMenuButtons();
		flagScroll = false;
		m_ScrollBar->SetNowUsing(false);
	}
}
//------------------------------------------------------------------------------
void CDComboBox::SetTextColor( int r, int g, int b )
{
	m_pFont->SetColor(RGB(r, g, b));
}
//------------------------------------------------------------------------------
void CDComboBox::CreateMenuButtons()
{
	UINT_PTR nID = m_param.StartItemID;
	for (int i = 0; i < _pNum; i++)
	{
		mButton = new CZButton(pParent, CRect(m_rect.left, m_rect.bottom + m_param.ItemHeight * i - 1, m_rect.right, m_rect.bottom + m_param.ItemHeight * (i + 1) - 1));
		mButton->SetButtonID(nID);
		CString str = this->arrayOfNames[i];
		mButton->SetCaption(str);
		mButton->SetTextColor(53, 53, 53);
		mButton->SetBelongsMenu(true);
		mButton->SetFixation();
		mButton->SetVisible(false);
		mButton->LiftButton(12.0);
		m_Vector.push_back(mButton);
		nID++;
	}
	CreateScrollingItems();
}
//------------------------------------------------------------------------------
void CDComboBox::ReDrawMenuButtons()
{
	SetMenuButtonsState(false);
	int k = 0;
	if (m_Vector.size() > 0)
	{
		for (int i = DCount; i < DCount + _pMaxVisNum; i++)
		{
			m_Vector.at(i)->SetVisible(true);
			m_Vector.at(i)->SetRectForDraw(CRect(m_rect.left, m_rect.bottom + m_param.ItemHeight * k - 1, m_rect.right, m_rect.bottom + m_param.ItemHeight * (k + 1) - 1));
			m_Vector.at(i)->LiftButton(12.);
			k++;
		}
	}
	if (m_bVisUD)
	{
		m_ButtonUp->SetVisible(true);
		m_ButtonDown->SetVisible(true);
		m_ScrollBar->SetVisible(true);
	}

	pParent->SetParamAddRect(CRect(m_rect.left,
		m_rect.top,
		m_rect.right + m_bVisUD*m_param.ItemHeight,
		m_rect.bottom + m_param.ItemHeight*_pVisNum + 3));

}
//------------------------------------------------------------------------------
void CDComboBox::SetMenuButtonsState( bool val )
{
	if (val)
	{
		SetMenuButtonsState(false);
		for (int i = DCount; i < DCount+_pVisNum; i++)
			m_Vector.at(i)->SetVisible(val);
		if (m_bVisUD)
		{
			m_ButtonUp->SetVisible(val);
			m_ButtonDown->SetVisible(val);
			m_ScrollBar->SetVisible(val);
		}
	}
	else
	{
		if (m_Vector.size() > 0)
		{
			for (auto it = m_Vector.begin(); it != m_Vector.end(); it++)
			{
				(*it)->SetVisible(false);
			}
		}
		m_ButtonUp->SetVisible(false);
		m_ButtonDown->SetVisible(false);
		m_ScrollBar->SetVisible(false);
	}
}
//------------------------------------------------------------------------------
void CDComboBox::ComboboxUpButton()
{
	if (DCount != 0)
	{
		DCount -= 1;
		ReDrawMenuButtons();
		float value = (float)DCount / ((float)(m_Vector.size() - _pMaxVisNum));
		m_ScrollBar->SetValueCur(value);
	}
}
//------------------------------------------------------------------------------
void CDComboBox::ComboboxDownButton()
{
	if (DCount + _pMaxVisNum != _pNum )
	{
		DCount += 1;
		ReDrawMenuButtons();
		float value = (float)DCount / ((float)(m_Vector.size() - _pMaxVisNum));
		m_ScrollBar->SetValueCur(value);
	}
}
//------------------------------------------------------------------------------
void CDComboBox::CreateScrollingItems()
{
	// сделать проверку от дублирования
	if (!m_ScrollBar)
	{
		m_ScrollBar = new CZScrollBar(pParent, CRect(m_rect.right - 1, m_rect.bottom + m_param.ItemHeight, 
			m_rect.right + m_param.ItemHeight - 1, m_rect.bottom + m_param.ItemHeight*(_pMaxVisNum - 1)));
		m_ScrollBar->SetBelongsMenu(true);
		m_ScrollBar->SetVisible(false);
		m_ScrollBar->LiftScrollBar(5.);
		pointFun = (FunctionChan)ScrollItems;
		m_ScrollBar->PointerForScroll(pointFun);
	}
	if (!m_ButtonUp)
	{
		m_ButtonUp = new CZButton(pParent, CRect(m_rect.right - 1, m_rect.bottom, m_rect.right + m_param.ItemHeight,
			 m_rect.bottom + m_param.ItemHeight), m_param.Texture_3_FileName, m_param.Number_Texture[2]);
		m_ButtonUp->SetCaption(L"");
		m_ButtonUp->SetCaptionChange(L"");
		m_ButtonUp->SetBelongsMenu(true);
		m_ButtonUp->SetVisible(false);
		m_ButtonUp->LiftButton(12.f);
		m_ButtonUp->SetButtonID(m_param.StartItemID + _pNum);
	}
	else
	{
		m_ButtonUp->SetButtonID(m_param.StartItemID + _pNum);
	}
	if (!m_ButtonDown)
	{
		m_ButtonDown = new CZButton(pParent, CRect(m_rect.right - 1, m_rect.bottom + m_param.ItemHeight*(_pMaxVisNum - 1), m_rect.right + m_param.ItemHeight,
			m_rect.bottom + m_param.ItemHeight*_pMaxVisNum), m_param.Texture_4_FileName, m_param.Number_Texture[3]);
		m_ButtonDown->SetCaption(L"");
		m_ButtonDown->SetCaptionChange(L"");
		m_ButtonDown->SetBelongsMenu(true);
		m_ButtonDown->SetVisible(false);
		m_ButtonDown->LiftButton(12.f);
		m_ButtonDown->SetButtonID(m_param.StartItemID + _pNum + 1);
	}
	else
	{
		m_ButtonDown->SetButtonID(m_param.StartItemID + _pNum + 1);
	}
}
//------------------------------------------------------------------------------
void CDComboBox::ScrollItems()
{
	flagScroll = true;
}
//------------------------------------------------------------------------------
void CDComboBox::DeleteMenuButtons()
{
	DCount = 0; //обнуляется каждый раз при обновлении списка элементов
	m_ScrollBar->SetValueCur(0.); //курсор ставится в изначальное положение
	if (m_Vector.size() > 0)	//
		for (auto it = m_Vector.begin(); it != m_Vector.end(); it++)
			(*it)->Removal = true;
	pParent->CleanVector();
	m_Vector.clear();
}
//------------------------------------------------------------------------------
void CDComboBox::SaveParameters(SimpleXML *pSimpleXML)
{
    CString cString;
    char String[16];

    sprintf_s(String, "%d", m_param.nElement);

    cString = CString(String);
    hXml hRoot = pSimpleXML->GetRootNode();
    hXml hInterfaceRoot = pSimpleXML->GetChildNode(hRoot);
    hXml hComboRoot = pSimpleXML->AddElement(L"ZComboBox_"+cString, hInterfaceRoot);

    hXml hTextureRoot_1 = pSimpleXML->AddElement(L"Texture_1", hComboRoot);
	pSimpleXML->SetAttributeValue(hTextureRoot_1, L"ZISNumber_Texture1", CComVariant(m_param.Number_Texture[0]));
    pSimpleXML->SetAttributeValue(hTextureRoot_1, L"path1", CComVariant(m_param.Texture_1_FileName));

    hXml hTextureRoot_2 = pSimpleXML->AddElement(L"Texture_2", hComboRoot);
	pSimpleXML->SetAttributeValue(hTextureRoot_2, L"ZISNumber_Texture2", CComVariant(m_param.Number_Texture[1]));
    pSimpleXML->SetAttributeValue(hTextureRoot_2, L"path2", CComVariant(m_param.Texture_2_FileName));

    hXml hMaterialRoot = pSimpleXML->AddElement(L"Material", hComboRoot);

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
void CDComboBox::LoadParameters(SimpleXML *pSimpleXML)
{
    VARIANT vAtrrib;
    CString str = L"";
    CString Pattern = L"ZComboBox_";
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
    hXml hTextureRoot_1 = pSimpleXML->GetChildNode(hElement);
    pSimpleXML->GetAttributeValue(hTextureRoot_1, L"ZISNumber_Texture1", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
			m_param.Number_Texture[0] = _wtoi(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hTextureRoot_1, L"path1", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            SetTexture(CString(vAtrrib.bstrVal), false);

    hXml hTextureRoot_2 = pSimpleXML->GetNextNode(hTextureRoot_1);
    pSimpleXML->GetAttributeValue(hTextureRoot_2, L"ZISNumber_Texture2", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
			m_param.Number_Texture[1] = _wtoi(vAtrrib.bstrVal);
    pSimpleXML->GetAttributeValue(hTextureRoot_2, L"path2", vAtrrib);
    if (vAtrrib.vt == VT_BSTR)
        if (CString(vAtrrib.bstrVal).GetLength() != 0)
            SetTexture(CString(vAtrrib.bstrVal), true);

    //рассеянный цвет материала (цвет материала в тени)
    hXml hMaterialRoot = pSimpleXML->GetNextNode(hTextureRoot_2);
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
void CDComboBox::CompareStrings(CString val)
{
	val.Trim();
	for (int i = 0; i < arrayOfNames->GetLength(); i++)
	{
		if (val == arrayOfNames[i]) SetIndexCur(i);
		else SetCaption(val);
	}
}
//------------------------------------------------------------------------------
#endif