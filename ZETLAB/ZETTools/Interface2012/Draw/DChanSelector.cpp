//------------------------------------------------------------------------------
// DChanSelector.cpp : файл реализации
//------------------------------------------------------------------------------
//#include "C:\TestDialog\TestDialog\stdafx.h"
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\Draw\DChanSelector.h"

//------------------------------------------------------------------------------
int g_nChanSelector = 0;		// счётчик
//------------------------------------------------------------------------------
#ifdef _GDIPLUS
CDChanSelector::CDChanSelector(CRect rect, PVOID ppGraphics,
	bool *pFlagIsChange, double *pTimeAnimation) :
CCustomDraw(rect, ppGraphics, pFlagIsChange, pTimeAnimation)
#else
//------------------------------------------------------------------------------
CDChanSelector::CDChanSelector(CRect rect, bool *pFlagIsChange, double *pTimeAnimation):
CCustomDraw(rect, pFlagIsChange, pTimeAnimation)
#endif
{
	m_pParamDraw = &m_param;	// обеспечение ОБЩИХ параметров
	m_param.nElement = ++g_nChanSelector;
	m_param.bMouseChange = true;
#ifdef _GDIPLUS
	m_param.pFont = new CZFont();
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
	m_ButtonUp = nullptr;
	m_ButtonDown = nullptr;
	m_pScrollBar = nullptr;
	int Depth = 5;
	int size_p = 3;
	m_pFont = new CDFonts("Arialbd", 15.f , FontsStyleRegular, RGB(23, 23, 23));
#ifdef _ACTIVEX_ZIS
	m_pButton = new CDetail(long(9), m_rect.right - (m_rect.Height())/2, (m_rect.bottom + m_rect.top)/2.0f, 0.75, 180, m_rect.Height() - 5, m_rect.Height() - 5, 20);

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

	m_Border = new CDetail(long(10), (rect.right + rect.left)/2, (rect.bottom + rect.top)/2, 0.75f, 0, rect.right - rect.left, rect.bottom - rect.top, 10);//*/
#else	
	//кнопка 	
	m_pButton = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Кнопка квадратная 22,5х22,5х7.stl",
		(m_rect.left + m_rect.right)/2.0f, (m_rect.bottom + m_rect.top)/2.0f, 0.75, 0, 
		m_rect.Width(), m_rect.Height(), 15);

	m_Border = new CDetail("C:\\ZETTools\\Interface2012\\Models\\Посадочное кнопки квадратной 23х23х7.stl",
		(m_rect.right + m_rect.left)/2, (m_rect.bottom + m_rect.top)/2, 0.75, 0, 
		m_rect.right - m_rect.left, m_rect.bottom - m_rect.top, 1);

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
//	SetTexture(L"C:\\ZETTools\\Interface2012\\Textures\\Patterns\\metal_01.bmp");

	m_ButRect = m_rect;
	m_ButRect.bottom += 3; // делаем отступ для отображения меню
	m_ButRect.left -= 7;
	m_ButRect.right -= 1;
	SetMenuColors(); //создаем цвета для меню 
	m_Vector.clear();
	DCount = 0;
	flagScroll = false;
	mPress = false;
	b_filterMeasure = false;
	b_FilterName = false;
	b_FilterIdChan = false;
	s_filterMeasure = L"";
	s_filterName = L"";
	s_filterIdChan = L"";

}
//------------------------------------------------------------------------------
CDChanSelector::~CDChanSelector()
{
#ifdef _GDIPLUS
	if (m_param.pFont)
	{
		delete m_param.pFont;
	}
#endif
	if (m_ButtonUp)
		delete m_ButtonUp;
	if (m_ButtonDown)
		delete m_ButtonDown;
	if (m_Border)
		delete m_Border;
	if (m_pButton)
		delete m_pButton;
	if (m_pFont)
		delete m_pFont;
	if (m_pScrollBar)
		delete m_pScrollBar;
	for (int i = 0; i < 9; i++)
		delete m_pComboBox[i];
	if (m_Vector.size() > 0)
	{
		for (UINT i = 0; i < m_Vector.size(); i++)
		{
			delete m_Vector[i].m_pButItem;
			if (m_Vector[i].hasChildren)
				delete m_Vector[i].m_pButSlew;
		}
		for(auto it=m_Vector.begin(); it!=m_Vector.end(); )
		{
			it = m_Vector.erase(it);
		}
	}
	m_Vector.clear();
}
//------------------------------------------------------------------------------
long CDChanSelector::GetIndexCur (void)
{
	int realsize = m_param.IndexCur;
	if (m_Vector.size() > 0)
	{
		for (int i = 0; i < m_param.IndexCur; i++)
		{
			if (m_Vector[i].hasChildren)
				realsize--;
		}
	}
	return realsize;
}
//------------------------------------------------------------------------------
void CDChanSelector::Clear()
{
	if (m_ButtonUp)
		m_ButtonUp->Clear();
	if (m_ButtonDown)
		m_ButtonDown->Clear();
	if (m_Border)
		m_Border->Clear();
	if (m_pButton)
		m_pButton->Clear();
	if (m_pFont)
		m_pFont->Clear();
	if (m_pScrollBar)
		m_pScrollBar->Clear();
	for (int i = 0; i < 9; i++)
		m_pComboBox[i]->Clear();
	if (m_Vector.size() > 0)
	{
		for (UINT i = 0; i < m_Vector.size(); i++)
		{
			m_Vector[i].m_pButItem->Clear();
			if (m_Vector[i].hasChildren)
				m_Vector[i].m_pButSlew->Clear();
		}
	}
}
//------------------------------------------------------------------------------
void CDChanSelector::SetImageFileName(CString str)
{
/*	if ( !str.IsEmpty() && (m_param.fileName != str) )
	{
		m_param.fileName = str;
	#ifdef _GDIPLUS
		LoadImageFromFile(&m_pImage_s, str + L"_s");
		LoadImageFromFile(&m_pImage_m, str + L"_m");
		LoadImageFromFile(&m_pImage_p, str + L"_p");
		LoadImageFromFile(&m_pImage_pm, str + L"_pm");
		m_pImage_Cur = m_pImage_s;
	#else
		Texture = LoadGLTextures(m_param.fileName, PicSize, 0);
        m_pButton->CalcTexCoors(PicSize[0], PicSize[1]);
	#endif
		IsChange();
	}//*/
}
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
	m_BackColorCur = m_param.BackColorMouse;
	m_param.FieldSolidColor = RGB (255,255,255);
	m_dinamic_color.mat_ambient[0] = 0.3f;
	m_dinamic_color.mat_ambient[1] = 0.3f;
	m_dinamic_color.mat_ambient[2] = 0.3f;
	m_dinamic_color.mat_emission[0] = 1.f/255.f;
	m_dinamic_color.mat_emission[1] = 101.f/255.f;
	m_dinamic_color.mat_emission[2] = 128.f/255.f;
	CCustomDraw::OnMouseEnter();
}
//------------------------------------------------------------------------------
void CDChanSelector::OnMouseLeave()
{
	m_BackColorCur = m_param.BackColor;
	m_param.FieldSolidColor = RGB (203,203,203);
	m_dinamic_color.mat_ambient[0] = 0.2f;
	m_dinamic_color.mat_ambient[1] = 0.2f;
	m_dinamic_color.mat_ambient[2] = 0.2f;
	m_dinamic_color.mat_emission[0] = 0.5f;
	m_dinamic_color.mat_emission[1] = 0.5f;
	m_dinamic_color.mat_emission[2] = 0.5f;
	CCustomDraw::OnMouseLeave();
}
//------------------------------------------------------------------------------
void CDChanSelector::SetTimeAnimation(double val)
{
	if (val > 0.)
		m_param.dTimeAnimation = val;
}
//------------------------------------------------------------------------------
void CDChanSelector::OnLButtonClk(UINT nFlags, CPoint point)
{
	IsChange();
}
//------------------------------------------------------------------------------
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
#ifdef _GDIPLUS
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
	if (m_param.bVisible)
	{	
		glPushAttrib(GL_LIGHTING_BIT | GL_TEXTURE_BIT);		
		glMaterialfv(GL_FRONT, GL_AMBIENT,   m_dinamic_color.mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE,   m_dinamic_color.mat_diffuse);
		glMaterialfv(GL_FRONT, GL_EMISSION,  m_dinamic_color.mat_emission);
		glMaterialfv(GL_FRONT, GL_SPECULAR,  m_dinamic_color.mat_specular);
		glMaterialf (GL_FRONT, GL_SHININESS, m_dinamic_color.mat_shininess);
		glBindTexture(GL_TEXTURE_2D, Texture2);
		m_pButton->Draw_texture(m_pButton->ReadFacet(), m_pButton->GetQntPlanes(), PicSize1, true);
		glBindTexture(GL_TEXTURE_2D, Texture1); 
		glPushMatrix();
		for (int i = 0; i < DETAILS; i++) 
			m_pComboBox[i]->Draw_texture(m_pComboBox[i]->ReadFacet(), m_pComboBox[i]->GetQntPlanes(), PicSize1, true);
		glPopMatrix();
		FillRectSolid(&m_param.rect2, 6.15, m_param.FieldSolidColor);
		if (mPress)
		{
			glPushMatrix();
			glTranslatef(0, 0, 10.f);
			if (!m_pScrollBar->GetVisible())
				FillParallepiped(&CRect(m_ButRect.left - 1, m_ButRect.bottom - 2, m_ButRect.right, m_ButRect.bottom + m_param.ItemHeight*_pVisNum + 3), 
				-3.0, -3.0, m_param.BorderColor);
			else
				FillParallepiped(&CRect(m_ButRect.left - 1, m_ButRect.bottom - 2, m_ButRect.right + m_param.ItemHeight - 1, m_ButRect.bottom + m_param.ItemHeight*_pVisNum + 3), 
				-3.0, -3.0, m_param.BorderColor);
			glPopMatrix();
		}
		glPopAttrib();	
		m_pFont->DrawString(m_rect, m_param.Caption, StrAlignmentCenter, StrAlignmentCenter);//*/
	}	
	if (flagScroll == true && pParent->OpenMenu && m_pScrollBar->GetNowUsing())
	{
		DCount = m_pScrollBar->GetValueCur() * (m_VisVector.size() - _pVisNum);
		ReDrawChannelTree();	
		flagScroll = false;
		m_pScrollBar->SetNowUsing(false);
	}
}
//------------------------------------------------------------------------------
void CDChanSelector::SetTextString( CTreeCtrl *var )
{
	Tree = var;

	int n = var->GetCount();
	CString sTemp, sTab(L"");
	CString pTemp; //строка с надписью родительского элемента
	HTREEITEM hItem(var->GetSelectedItem());
	sTemp = var->GetItemText(hItem);
	HTREEITEM hTempItem(NULL);
	hTempItem = var->GetParentItem(hItem); 
	HTREEITEM hParentItem(NULL); 
	while(hTempItem != NULL)
	{
		hItem = hTempItem;
		hTempItem = var->GetParentItem(hItem); 
	}
	hTempItem = var->GetNextItem(hItem, TVGN_PREVIOUS);
	while(hTempItem != NULL)
	{
		hItem = hTempItem;
		hTempItem = var->GetNextItem(hItem, TVGN_PREVIOUS);
	} 
	sTemp = var->GetItemText(hItem);

	int iCounter(0), iChannelCounter(0);
	while(hItem != NULL)
	{
		sTemp = var->GetItemText(hItem);

		hParentItem = var->GetParentItem(hItem);
		SetHTREEITEMParent(hParentItem);

		Tree->SetItemState(hItem, NULL, NULL);

		if (var->ItemHasChildren(hItem))
		{	
			SetItemHasChildren(true);
			m_Vector.at(iCounter).hasChildren = true;
			if (m_ExpVector.size() > 0)
			{
				for (UINT  i = 0; i < m_ExpVector.size(); i++)
				{
					if (sTemp == m_ExpVector.at(i).Caption
						&& pTemp == m_ExpVector.at(i).ParentCaption)
					{
						if (m_ExpVector.at(i).expand) var->Expand(hItem, TVE_EXPAND);
						else var->Expand(hItem, TVE_COLLAPSE);
					}
				}
			}

			m_Vector.at(iCounter).levelType = DeviceName;
			hItem = var->GetChildItem(hItem);
			if (var->ItemHasChildren(hItem))
				m_Vector.at(iCounter).levelType = ComputerName;

		}
		else
		{
			m_Vector.at(iCounter).levelType = ChannelName;
			SetItemHasChildren(false);
			m_Vector.at(iCounter).hasChildren = false;
			++iChannelCounter;
			CString sTemp, sTab(L"");
			//		

			hTempItem = var->GetNextSiblingItem(hItem);
			if (hTempItem == NULL)
			{
				while (hTempItem == NULL)
				{
					hTempItem = var->GetParentItem(hItem);
					if (sTab.GetLength() > 0)
						sTab.Delete(0, 6);
					if (hTempItem == NULL)
					{
						hItem = hTempItem;
						break;
					}
					else
					{
						hItem = hTempItem;
						hTempItem = var->GetNextSiblingItem(hItem);
					}
				}
				hItem = hTempItem;
			}
			else
				hItem = hTempItem;
		}
		++iCounter;
	}

	_pNum =  iCounter;
	_pVisNum = 0;
	_pChanNum = iChannelCounter;

	if (b_filterMeasure) FilterByMeasure(s_filterMeasure);
	if (b_FilterName) FilterByName(s_filterName);
	if (b_FilterIdChan) FilterByIdChan(s_filterIdChan);

	DrawChannelTree();
}
//------------------------------------------------------------------------------
void CDChanSelector::SetUnusedButtons (int val)
{
	CString sTemp;
	HTREEITEM hItem(Tree->GetSelectedItem());
	HTREEITEM hTempItem(NULL);
	hTempItem = Tree->GetParentItem(hItem); 
	while(hTempItem != NULL)
	{
		hItem = hTempItem;
		hTempItem = Tree->GetParentItem(hItem); 
	}
	hTempItem = Tree->GetNextItem(hItem, TVGN_PREVIOUS);
	while(hTempItem != NULL)
	{
		hItem = hTempItem;
		hTempItem = Tree->GetNextItem(hItem, TVGN_PREVIOUS);
	}
	int iCounter(0);
	while(hItem != NULL)
	{			
		if (Tree->ItemHasChildren(hItem) == false && m_Vector.at(iCounter).index == val) // действия после нажатия кнопки мыши			
		{
			sTemp = Tree->GetItemText(hItem);
			SetCaption(sTemp);
			if (m_param.IndexCur > 0 && m_param.IndexCur <= _pNum) 
				m_Vector.at(m_param.IndexCur).m_pButItem->SetTextColor(32, 32, 32); // пердыдущий выбранный канал становится обычного цвета
			m_param.IndexCur = val;
			SetActiveButton(iCounter);
			pParent->CloseMenu();
			break;
		}
		else 
		{				
			if (m_Vector.at(iCounter).index == val)
			{	
				TRACE(_T("Expand item  %d to  0x%x \n"), iCounter, Tree->GetItemState(hItem,TVIS_EXPANDED));
				Tree->Expand(m_Vector.at(iCounter).personalID, TVE_TOGGLE);
				TRACE(_T("Expand item  %d to  0x%x \n"), iCounter, Tree->GetItemState(hItem,TVIS_EXPANDED));
				ExpandBranch();
			}
			if (Tree->ItemHasChildren(hItem))
			{	 
				hItem = Tree->GetChildItem(hItem);
			}
			else
			{
				hTempItem = Tree->GetNextSiblingItem(hItem);
				if (hTempItem == NULL)
				{
					while (hTempItem == NULL)
					{
						hTempItem = Tree->GetParentItem(hItem);
						if (hTempItem == NULL)
						{
							hItem = hTempItem;
							break;
						}
						else
						{
							hItem = hTempItem;
							hTempItem = Tree->GetNextSiblingItem(hItem);
						}
					}
					hItem = hTempItem;
				}
				else
					hItem = hTempItem;
			}
			++iCounter;
		}		
	}
	TRACE ("++++++++++++++UINT val %d\n", val);
	IsChange();
}
//------------------------------------------------------------------------------
void CDChanSelector::SetIndexCur( long val )
{
	CString sTemp;
	val += 2;
	if (m_Vector.size() > 0)
		for (int i = 2; i < val + 1; i++)
		{
			if (m_Vector[i].hasChildren)
				val++;
		}
	HTREEITEM hItem(Tree->GetSelectedItem());
	HTREEITEM hTempItem(NULL);
	hTempItem = Tree->GetParentItem(hItem); 
	while(hTempItem != NULL)
	{
		hItem = hTempItem;
		hTempItem = Tree->GetParentItem(hItem); 
	}
	hTempItem = Tree->GetNextItem(hItem, TVGN_PREVIOUS);
	while(hTempItem != NULL)
	{
		hItem = hTempItem;
		hTempItem = Tree->GetNextItem(hItem, TVGN_PREVIOUS);
	}
	int iCounter(0);
	while(hItem != NULL)
	{			
		if (Tree->ItemHasChildren(hItem) == false && m_Vector.at(iCounter).index == val) 				
		{
			sTemp = Tree->GetItemText(hItem);
			SetCaption(sTemp);
			if (m_param.IndexCur > 0 && m_param.IndexCur <= _pNum) m_Vector.at(m_param.IndexCur).m_pButItem->SetTextColor(32, 32, 32); // пердыдущий выбранный канал становится обычного цвета
			m_param.IndexCur = val;
			SetActiveButton(iCounter);
			SetMenuButtonsState(false);
			pParent->CloseMenu();
			break;
		}
		else 
		{				
			if (m_Vector.at(iCounter).index == val)
			{	
				TRACE(_T("Expand item  %d to  0x%x \n"), iCounter, Tree->GetItemState(hItem,TVIS_EXPANDED));
				Tree->Expand(m_Vector.at(iCounter).personalID, TVE_TOGGLE);
				TRACE(_T("Expand item  %d to  0x%x \n"), iCounter, Tree->GetItemState(hItem,TVIS_EXPANDED));
				ExpandBranch();
			}
			if (Tree->ItemHasChildren(hItem))
			{	 
				hItem = Tree->GetChildItem(hItem);
			}
			else
			{
				hTempItem = Tree->GetNextSiblingItem(hItem);
				if (hTempItem == NULL)
				{
					while (hTempItem == NULL)
					{
						hTempItem = Tree->GetParentItem(hItem);
						if (hTempItem == NULL)
						{
							hItem = hTempItem;
							break;
						}
						else
						{
							hItem = hTempItem;
							hTempItem = Tree->GetNextSiblingItem(hItem);
						}
					}
					hItem = hTempItem;
				}
				else
					hItem = hTempItem;
			}
			++iCounter;
		}		
	}
	TRACE ("++++++++++++++UINT val %d\n", val);
	IsChange();
}
//------------------------------------------------------------------------------
void CDChanSelector::DrawChannelTree()
{
	_pMaxVisNum = (spaceRect.bottom - m_ButRect.bottom) / m_param.ItemHeight - 1; 
	if (m_Vector.size() > (UINT)_pMaxVisNum)
	{
		m_bVisUD = true; _pVisNum = _pMaxVisNum; 
	}
	else 
	{ 
		m_bVisUD = false;  
		_pVisNum = m_Vector.size(); 
	}
	DrawScrollingItems();
	
	//создание кнопок для меню ChanSelector'a
	int n = 0; //счетчик уровней, на которых рисуются кнопки 
	if (m_Vector.size() > 0)	//
	{
		for(auto it=m_Vector.begin(); it!=m_Vector.end(); it++)
		{			
				if ((*it).levelType == ComputerName)
				{
					(*it).index = n;
					(*it).m_pButItem = new CZButton(pParent, CRect(m_ButRect.left+m_param.ItemHeight+3,
																	m_ButRect.bottom +m_param.ItemHeight*n+1,
																	m_ButRect.right-3,
																	m_ButRect.bottom+m_param.ItemHeight*(1+n)));
					(*it).m_pButItem->SetBelongsMenu(true);
					(*it).m_pButItem->SetCaption((*it).Caption);
					(*it).m_pButItem->AddToolTip((*it).Caption);
					(*it).m_pButItem->SetCaptionChange((*it).Caption);
					(*it).m_pButItem->SetButtonType(ChanSelectorMenuButton);
					(*it).m_pButItem->SetButtonID(m_param.StartItemID + n);	
					(*it).m_pButItem->SetTextColor(240,240,240);
					(*it).m_pButItem->SetVisible(false);

 					(*it).m_pButItem->SetColor(m_color_com.mat_ambient,m_color_com.mat_diffuse,m_color_com.mat_emission, m_color_com.mat_specular,m_color_com.mat_shininess);
					(*it).m_pButItem->LiftButton(12.f);

					(*it).m_pButSlew = new CZButton(pParent, CRect(m_ButRect.left + 3,
																	m_ButRect.bottom +m_param.ItemHeight*n+1,
																	m_ButRect.left+m_param.ItemHeight + 3,
																	m_ButRect.bottom+m_param.ItemHeight*(1+n)));
					(*it).m_pButSlew->SetBelongsMenu(true);
					(*it).m_pButSlew->SetCaption(L"+");	
					(*it).m_pButSlew->SetButtonID(m_param.StartItemID + n);		
					(*it).m_pButSlew->SetButtonType(ChanSelectorMenuButton);
					(*it).m_pButSlew->SetCaptionChange(L"-");
					(*it).m_pButSlew->SetButtonState(true);
					(*it).m_pButSlew->SetTextColor(240,240,240);
					(*it).m_pButSlew->SetColor(m_color_com.mat_ambient,m_color_com.mat_diffuse,m_color_com.mat_emission, m_color_com.mat_specular,m_color_com.mat_shininess);
					(*it).m_pButSlew->LiftButton(12.f);
					(*it).m_pButSlew->SetVisible(false);
				}
				else if ((*it).levelType == DeviceName)
				{
					(*it).index = n;
					(*it).m_pButItem = new CZButton(pParent, CRect(m_ButRect.left+2*m_param.ItemHeight,
																	m_ButRect.bottom +m_param.ItemHeight*n+1,
																	m_ButRect.right-3,
																	m_ButRect.bottom+m_param.ItemHeight*(1+n)));
					(*it).m_pButItem->SetBelongsMenu(true);
					(*it).m_pButItem->SetCaption((*it).Caption);
					(*it).m_pButItem->AddToolTip((*it).Caption);
					(*it).m_pButItem->SetCaptionChange((*it).Caption);
					(*it).m_pButItem->SetButtonType(ChanSelectorMenuButton);
					(*it).m_pButItem->SetButtonID(m_param.StartItemID + n);		
					(*it).m_pButItem->SetTextColor(240,240,240);
					(*it).m_pButItem->SetColor(m_color_device.mat_ambient,m_color_device.mat_diffuse,m_color_device.mat_emission, m_color_device.mat_specular,m_color_device.mat_shininess);
					(*it).m_pButItem->LiftButton(12.f);
					(*it).m_pButItem->SetVisible(false);

					(*it).m_pButSlew = new CZButton(pParent, CRect(m_ButRect.left + m_param.ItemHeight,
																	m_ButRect.bottom +m_param.ItemHeight*n+1,
																	m_ButRect.left+2*m_param.ItemHeight,
																	m_ButRect.bottom+m_param.ItemHeight*(1+n)));
					(*it).m_pButSlew->SetBelongsMenu(true);
					(*it).m_pButSlew->SetCaption(L"+");	
					(*it).m_pButSlew->SetButtonID(m_param.StartItemID + n);		
					(*it).m_pButSlew->SetButtonType(ChanSelectorMenuButton);
					(*it).m_pButSlew->SetCaptionChange(L"-");
					(*it).m_pButSlew->SetButtonState(true);
					(*it).m_pButSlew->SetTextColor(240,240,240);
					(*it).m_pButSlew->SetColor(m_color_device.mat_ambient,m_color_device.mat_diffuse,m_color_device.mat_emission, m_color_device.mat_specular,m_color_device.mat_shininess);
					(*it).m_pButSlew->LiftButton(12.f);
					(*it).m_pButSlew->SetVisible(false);
				}
				else if ((*it).levelType == ChannelName)
				{
					(*it).index = n;
					(*it).m_pButItem = new CZButton(pParent, CRect(m_ButRect.left+50,
																	m_ButRect.bottom + m_param.ItemHeight*n+1,
																	m_ButRect.right-3,
																	m_ButRect.bottom+m_param.ItemHeight*(1+n)));
					(*it).m_pButItem->SetBelongsMenu(true);
					(*it).m_pButItem->SetCaption((*it).Caption);
					(*it).m_pButItem->AddToolTip(L"");
					(*it).m_pButItem->SetCaptionChange((*it).Caption);
					(*it).m_pButItem->SetButtonType(ChanSelectorMenuButton);
					(*it).m_pButItem->SetButtonID(m_param.StartItemID + n);	
					(*it).m_pButItem->SetTextColor(32,32,32);
					(*it).m_pButItem->SetColor(m_color_chan.mat_ambient,m_color_chan.mat_diffuse,m_color_chan.mat_emission, m_color_chan.mat_specular,m_color_chan.mat_shininess);
					(*it).m_pButItem->LiftButton(12.f);
					(*it).m_pButItem->SetVisible(false);
				}
				n++;
		}		
	}
	m_param.bVisible = true;
	ExpandBranch();
}
//------------------------------------------------------------------------------
void CDChanSelector::ExpandBranch()
{
	m_VisVector.clear();

	HTREEITEM hItem(Tree->GetSelectedItem());
	HTREEITEM hTempItem(NULL);
	hTempItem = Tree->GetParentItem(hItem); 
	while(hTempItem != NULL)
	{
		hItem = hTempItem;
		hTempItem = Tree->GetParentItem(hItem); 
	}
	hTempItem = Tree->GetNextItem(hItem, TVGN_PREVIOUS);
	while(hTempItem != NULL)
	{
		hItem = hTempItem;
		hTempItem = Tree->GetNextItem(hItem, TVGN_PREVIOUS);
	}

	int iCounter(0);
	while(hItem != NULL)
	{
		if (Tree->ItemHasChildren(hItem))
		{	
			if (m_Vector.size() > 0)	//
			{
				int i(0);
				for(auto it=m_Vector.begin(); it!=m_Vector.end(); it++)
				{
					if (it->personalID == hItem) m_VisVector.push_back(&m_Vector.at(i));
					i++;
				}
			}
			
			bool fExpanded = ((Tree->GetItemState(hItem, TVIS_EXPANDED) & TVIS_EXPANDED) != TVIS_EXPANDED) ? FALSE : TRUE; 		
			if (fExpanded)	
			{
				if (m_Vector.size() > 0)	//
				{
					int i(0);
					for(auto it=m_Vector.begin(); it!=m_Vector.end(); it++)
					{
						if (it->personalID == hItem)
						{
							m_Vector.at(i).m_pButSlew->SetButtonState(true);
							m_Vector.at(i).expanded = fExpanded;
						}
						
						i++;
					}
				}

				hItem = Tree->GetChildItem(hItem);		
			}	
			
			else 
			{
				if (m_Vector.size() > 0)	//
					{
						int i(0);
						for(auto it=m_Vector.begin(); it!=m_Vector.end(); it++)
						{
							if (it->personalID == hItem)
							{
								m_Vector.at(i).m_pButSlew->SetButtonState(false);
								m_Vector.at(i).expanded = fExpanded;
							}
							i++;
						}
					}

				hTempItem = Tree->GetNextSiblingItem(hItem);
					if (hTempItem == NULL)
					{
						while (hTempItem == NULL)
						{
							hTempItem = Tree->GetParentItem(hItem);

							if (hTempItem == NULL)
							{
								hItem = hTempItem;
								break;
							}
							else
							{
								hItem = hTempItem;
								hTempItem = Tree->GetNextSiblingItem(hItem);
							}
						}
						hItem = hTempItem;
					}
					else
						hItem = hTempItem;
			}
		}
		else
		{	
			
				if (m_Vector.size() > 0)	//
				{
					int i(0);
					for(auto it=m_Vector.begin(); it!=m_Vector.end(); it++)
					{
						if (it->personalID == hItem) m_VisVector.push_back(&m_Vector.at(i));
						i++;
					}
				}
			hTempItem = Tree->GetNextSiblingItem(hItem);
			if (hTempItem == NULL)
			{
				while (hTempItem == NULL)
				{
					hTempItem = Tree->GetParentItem(hItem);

					if (hTempItem == NULL)
					{
						hItem = hTempItem;
						break;
					}
					else
					{
						hItem = hTempItem;
						hTempItem = Tree->GetNextSiblingItem(hItem);
					}
				}
				hItem = hTempItem;
			}
			else
				hItem = hTempItem;
			
		}
		++iCounter;
	}
	float value = (float)DCount / ((float)(m_VisVector.size() - _pVisNum));
	m_pScrollBar->SetValueCur(value);
	ReDrawChannelTree();
	SaveExpand();
}
//------------------------------------------------------------------------------
void CDChanSelector::ReDrawChannelTree()
{
 	SetMenuButtonsState(false);
	if (m_VisVector.size() <= (UINT)_pMaxVisNum)
	{ 
		m_bVisUD = false;
		DCount = 0;
	}
	else m_bVisUD = true;
	//чтобы элементы не пропадали из-за значения DCount большего, чем нужно
	if ((UINT)DCount > m_VisVector.size() - _pVisNum) DCount = m_VisVector.size() - _pVisNum;

	int k = 0;
	if (!m_VisVector.empty())
	{
		while (k<_pMaxVisNum ) 
		{
			if ((UINT)(k + DCount) >= m_VisVector.size())
				break;
			else
			{
				if (m_VisVector.at(k+DCount)->levelType == ComputerName)
					{
						m_VisVector.at(k+DCount)->m_pButItem->SetVisible(true);
						m_VisVector.at(k+DCount)->m_pButItem->SetRectForDraw(CRect(m_ButRect.left + m_param.ItemHeight + 3 ,
																						m_ButRect.bottom + m_param.ItemHeight*k + 1, 
																						m_ButRect.right - 3 ,
																						m_ButRect.bottom + m_param.ItemHeight*(1 + k)));
						m_VisVector.at(k+DCount)->m_pButItem->LiftButton(12.f);
						m_VisVector.at(k + DCount)->m_pButItem->AddToolTip(m_VisVector.at(k + DCount)->Caption);

						m_VisVector.at(k+DCount)->m_pButSlew->SetVisible(true);
						m_VisVector.at(k+DCount)->m_pButSlew->SetRectForDraw(CRect(m_ButRect.left + 3, 
																						m_ButRect.bottom + m_param.ItemHeight*k + 1, 
																						m_ButRect.left+m_param.ItemHeight + 3,
																						m_ButRect.bottom + m_param.ItemHeight*(1 + k)));
						m_VisVector.at(k+DCount)->m_pButSlew->LiftButton(12.f);
					}
					else if (m_VisVector.at(k+DCount)->levelType == DeviceName)
					{
						m_VisVector.at(k+DCount)->m_pButItem->SetVisible(true);
						m_VisVector.at(k+DCount)->m_pButItem->SetRectForDraw(CRect(m_ButRect.left + 2*m_param.ItemHeight, 
																						m_ButRect.bottom + m_param.ItemHeight*k + 1, 
																						m_ButRect.right - 3,
																						m_ButRect.bottom + m_param.ItemHeight*(1 + k)));
						m_VisVector.at(k+DCount)->m_pButItem->LiftButton(12.);
						m_VisVector.at(k + DCount)->m_pButItem->AddToolTip(m_VisVector.at(k + DCount)->Caption);

						m_VisVector.at(k+DCount)->m_pButSlew->SetVisible(true);
						m_VisVector.at(k+DCount)->m_pButSlew->SetRectForDraw(CRect(m_ButRect.left + m_param.ItemHeight, 
																						m_ButRect.bottom + m_param.ItemHeight*k + 1, 
																						m_ButRect.left + 2*m_param.ItemHeight,
																						m_ButRect.bottom+m_param.ItemHeight*(1 + k)));
						m_VisVector.at(k+DCount)->m_pButSlew->LiftButton(12.f);
					}
					else if (m_VisVector.at(k+DCount)->levelType == ChannelName)
					{
						m_VisVector.at(k+DCount)->m_pButItem->SetVisible(true);
						m_VisVector.at(k+DCount)->m_pButItem->SetRectForDraw(CRect(m_ButRect.left + 50, 
																						m_ButRect.bottom + m_param.ItemHeight*k + 1, 
																						m_ButRect.right - 3, 
																						m_ButRect.bottom + m_param.ItemHeight*(1 + k)));
						m_VisVector.at(k+DCount)->m_pButItem->LiftButton(12.f);	
						m_VisVector.at(k + DCount)->m_pButItem->AddToolTip(m_VisVector.at(k + DCount)->Caption);
						//m_VisVector.at(k + DCount)->m_pButItem->AddToolTip(L"");
					}
				k++;
			}
		}
	}	
	if (m_bVisUD)
	{
		m_ButtonUp->SetVisible(true);
		m_ButtonUp->SetRectForDraw(CRect(m_ButRect.right - 1, 
											m_ButRect.bottom + 1, 
											m_ButRect.right + m_param.ItemHeight - 3, 
											m_ButRect.bottom + m_param.ItemHeight));
		m_ButtonUp->LiftButton(12.f);	
		m_ButtonDown->SetVisible(true);
		m_ButtonDown->SetRectForDraw(CRect(m_ButRect.right - 1,	
											m_ButRect.bottom + (_pMaxVisNum - 1) * m_param.ItemHeight + 1, 
											m_ButRect.right + m_param.ItemHeight - 3, 
											m_ButRect.bottom + _pMaxVisNum * m_param.ItemHeight));
		m_ButtonDown->LiftButton(12.f);
		m_pScrollBar->SetVisible(true);
		m_pScrollBar->LiftScrollBar(5.);
	}
	else 
	{		
		m_ButtonUp->SetVisible(false);
		m_ButtonDown->SetVisible(false);
		m_pScrollBar->SetVisible(false);
	}
	_pVisNum = k; 
	pParent->SetParamAddRect(CRect(m_ButRect.left,
										m_ButRect.top,
										m_ButRect.right + m_bVisUD*m_param.ItemHeight,
										m_ButRect.bottom + GetHeightChs() + 3));
	IsChange();
}
//------------------------------------------------------------------------------
void CDChanSelector::DeleteMenuButtons()
{
	if (m_Vector.size() > 0)	//
	{
		for(auto it=m_Vector.begin(); it!=m_Vector.end(); it++)
		{
			(*it).m_pButItem->Removal = true;
			if ((*it).hasChildren) (*it).m_pButSlew->Removal = true;
		}
	}
	pParent->CleanVector();
	m_Vector.clear(); 
}
//------------------------------------------------------------------------------
void CDChanSelector::SetMenuColors()
{
	m_dinamic_color.mat_ambient[0] = 0.2;
	m_dinamic_color.mat_ambient[1] = 0.2;
	m_dinamic_color.mat_ambient[2] = 0.2;
	m_dinamic_color.mat_ambient[3] = 0.0;
	
	m_dinamic_color.mat_diffuse[0] = 0.72;
	m_dinamic_color.mat_diffuse[1] = 0.72;
	m_dinamic_color.mat_diffuse[2] = 0.72;
	m_dinamic_color.mat_diffuse[3] = 0.0;
	
	m_dinamic_color.mat_specular[0] = 0.296648;
	m_dinamic_color.mat_specular[1] = 0.296648;
	m_dinamic_color.mat_specular[2] = 0.296648;
	m_dinamic_color.mat_specular[3] = 0.0;
	
	m_dinamic_color.mat_emission[0] = 0.5;
	m_dinamic_color.mat_emission[1] = 0.5;
	m_dinamic_color.mat_emission[2] = 0.5;
	m_dinamic_color.mat_emission[3] = 0.0;

	m_dinamic_color.mat_shininess = 10.24f;

	m_color_com.mat_ambient[0] = 0.0;
	m_color_com.mat_ambient[1] = 0.0;
	m_color_com.mat_ambient[2] = 0.0;
	m_color_com.mat_ambient[3] = 1.0;

	m_color_com.mat_diffuse[0] = 1.0;
	m_color_com.mat_diffuse[1] = 1.0;

	m_color_com.mat_diffuse[2] = 1.0;
	m_color_com.mat_diffuse[3] = 1.0;

	m_color_com.mat_specular[0] = 1.0;
	m_color_com.mat_specular[1] = 1.0;
	m_color_com.mat_specular[2] = 1.0;
	m_color_com.mat_specular[3] = 1.0;

	m_color_com.mat_emission[0] = 0.2;
	m_color_com.mat_emission[1] = 0.2;
	m_color_com.mat_emission[2] = 0.2;
	m_color_com.mat_emission[3] = 1.;

	m_color_com.mat_shininess = 4.24f;

	m_color_com1.mat_ambient[0] = 0.4;
	m_color_com1.mat_ambient[1] = 0.4;
	m_color_com1.mat_ambient[2] = 0.4;
	m_color_com1.mat_ambient[3] = 1.0;

	m_color_com1.mat_diffuse[0] = 0.0;
	m_color_com1.mat_diffuse[1] = 0.0;
	m_color_com1.mat_diffuse[2] = 0.0;
	m_color_com1.mat_diffuse[3] = 1.0;

	m_color_com1.mat_specular[0] = 0.0;
	m_color_com1.mat_specular[1] = 0.0;
	m_color_com1.mat_specular[2] = 0.0;
	m_color_com1.mat_specular[3] = 1.0;

	m_color_com1.mat_emission[0] = 0.4;
	m_color_com1.mat_emission[1] = 0.4;
	m_color_com1.mat_emission[2] = 0.4;
	m_color_com1.mat_emission[3] = 1.0;

	m_color_com1.mat_shininess = 14.24f;

	m_color_device.mat_ambient[0] = 0.35;
	m_color_device.mat_ambient[1] = 0.35;
	m_color_device.mat_ambient[2] = 0.35;
	m_color_device.mat_ambient[3] = 1.0;

	m_color_device.mat_diffuse[0] = 0.5;
	m_color_device.mat_diffuse[1] = 0.5;
	m_color_device.mat_diffuse[2] = 0.5;
	m_color_device.mat_diffuse[3] = 1.0;

	m_color_device.mat_specular[0] = 0.296648;
	m_color_device.mat_specular[1] = 0.296648;
	m_color_device.mat_specular[2] = 0.296648;
	m_color_device.mat_specular[3] = 1.0;

	m_color_device.mat_emission[0] = 0.2;
	m_color_device.mat_emission[1] = 0.2;
	m_color_device.mat_emission[2] = 0.2;
	m_color_device.mat_emission[3] = 1.;

	m_color_device.mat_shininess = 4.24f;

	m_color_chan.mat_ambient[0] = 0.3;
	m_color_chan.mat_ambient[1] = 0.3;
	m_color_chan.mat_ambient[2] = 0.3;
	m_color_chan.mat_ambient[3] = 1.0;

	m_color_chan.mat_diffuse[0] = 0.5;
	m_color_chan.mat_diffuse[1] = 0.5;
	m_color_chan.mat_diffuse[2] = 0.5;
	m_color_chan.mat_diffuse[3] = 1.0;

	m_color_chan.mat_specular[0] = 0.7;
	m_color_chan.mat_specular[1] = 0.7;
	m_color_chan.mat_specular[2] = 0.7;
	m_color_chan.mat_specular[3] = 1.0;

	m_color_chan.mat_emission[0] = 0.5;
	m_color_chan.mat_emission[1] = 0.5;
	m_color_chan.mat_emission[2] = 0.5;
	m_color_chan.mat_emission[3] = 1.;

	m_color_chan.mat_shininess = 4.24f;
}
//------------------------------------------------------------------------------
void CDChanSelector::SetMenuButtonsState(bool val)
{	
	if (val)
	{
		SetMenuButtonsState(false);
		int k = 0;
		if (m_VisVector.size() > 0)
		{
			while (k<_pMaxVisNum ) 
			{
				if ((UINT)(k + DCount)  >= m_VisVector.size()) break;
				else
				{
					if (m_VisVector.at(k+DCount)->levelType == ComputerName)
						{
							m_VisVector.at(k+DCount)->m_pButItem->SetVisible(true);	
							m_VisVector.at(k + DCount)->m_pButItem->AddToolTip(m_VisVector.at(k + DCount)->Caption);
							m_VisVector.at(k+DCount)->m_pButSlew->SetVisible(true);
						}
						else if (m_VisVector.at(k+DCount)->levelType == DeviceName)
						{
							m_VisVector.at(k+DCount)->m_pButItem->SetVisible(true);		
							m_VisVector.at(k+DCount)->m_pButItem->AddToolTip(m_VisVector.at(k + DCount)->Caption);
							m_VisVector.at(k+DCount)->m_pButSlew->SetVisible(true);
						}
						else if (m_VisVector.at(k+DCount)->levelType == ChannelName)
						{
							m_VisVector.at(k+DCount)->m_pButItem->SetVisible(true);		
							m_VisVector.at(k+DCount)->m_pButItem->AddToolTip(m_VisVector.at(k + DCount)->Caption);
						}
					k++;
				}
			}
		}
		if (m_bVisUD) 
		{
			m_ButtonUp->SetVisible(val);
			m_ButtonDown->SetVisible(val);
			m_pScrollBar->SetVisible(val);
		}
	}
	else 
	{
		if (m_Vector.size() > 0)	//
		{
			for(auto it=m_Vector.begin(); it!=m_Vector.end(); it++)
			{
					if ((*it).levelType == ComputerName)
					{
						(*it).m_pButItem->SetVisible(val);
						(*it).m_pButItem->AddToolTip(L"");
						(*it).m_pButSlew->SetVisible(val);		
					}
					else if ((*it).levelType == DeviceName)
					{
						(*it).m_pButItem->SetVisible(val);
						(*it).m_pButItem->AddToolTip(L"");
						(*it).m_pButSlew->SetVisible(val);	
					}
					else if ((*it).levelType == ChannelName)
					{
						(*it).m_pButItem->SetVisible(val);
						(*it).m_pButItem->AddToolTip(L"");
					}
			}
		}
		m_ButtonUp->SetVisible(val);
		m_ButtonDown->SetVisible(val);

		m_pScrollBar->SetVisible(val);
	}
}
//------------------------------------------------------------------------------
void CDChanSelector::DrawScrollingItems()
{
	if (!m_ButtonUp)
	{
		m_ButtonUp = new CZButton(pParent, CRect(m_ButRect.right - 1, m_ButRect.bottom + 1, m_ButRect.right + m_param.ItemHeight - 3, 
			m_ButRect.bottom + m_param.ItemHeight), m_param.Texture_3_FileName, m_param.Number_Texture[2]);
		m_ButtonUp->SetBelongsMenu(true);
		m_ButtonUp->SetCaption(L"");
		m_ButtonUp->SetButtonType(ChanSelectorScroll);
		m_ButtonUp->SetTextColor(80,80,80);
		m_ButtonUp->SetVisible(false);
		m_ButtonUp->SetButtonID(m_param.StartItemID + _pNum);	
		m_ButtonUp->SetColor(m_color_device.mat_ambient,m_color_device.mat_diffuse,m_color_device.mat_emission, m_color_device.mat_specular,m_color_device.mat_shininess);
		m_ButtonUp->LiftButton(12.f);
		m_ButtonUp->SetCaptionChange(L"");
		m_ButtonUp->SetAligmentHorizontal(StrAlignmentNear);
	}
	else
	{
		m_ButtonUp->SetButtonID(m_param.StartItemID + _pNum);
	}
	if (!m_ButtonDown)
	{
		m_ButtonDown = new CZButton(pParent, CRect(m_ButRect.right - 1,	m_ButRect.bottom + (_pMaxVisNum - 1) * m_param.ItemHeight + 1, m_ButRect.right + m_param.ItemHeight - 3, 
			m_ButRect.bottom + _pMaxVisNum * m_param.ItemHeight), m_param.Texture_4_FileName, m_param.Number_Texture[3]);
		m_ButtonDown->SetBelongsMenu(true);
		m_ButtonDown->SetCaption(L"");
		m_ButtonDown->SetButtonType(ChanSelectorScroll);
		m_ButtonDown->SetTextColor(255,255,255);
		m_ButtonDown->SetVisible(false);
		m_ButtonDown->SetColor(m_color_device.mat_ambient,m_color_device.mat_diffuse,m_color_device.mat_emission, m_color_device.mat_specular,m_color_device.mat_shininess);
		m_ButtonDown->LiftButton(12.f);
		m_ButtonDown->SetCaptionChange(L"");
		m_ButtonDown->SetAligmentHorizontal(StrAlignmentNear);
		m_ButtonDown->SetButtonID(m_param.StartItemID + _pNum + 1);	
	}
	else
	{
		m_ButtonDown->SetButtonID(m_param.StartItemID + _pNum + 1);
	}
	if (!m_pScrollBar)
	{
		m_pScrollBar = new CZScrollBar(pParent, CRect(m_ButRect.right - 2, m_ButRect.bottom + m_param.ItemHeight + 1, 
			m_ButRect.right + m_param.ItemHeight - 4, m_ButRect.bottom + (_pMaxVisNum - 1) * m_param.ItemHeight - 2));
		m_pScrollBar->SetBelongsMenu(true);
		m_pScrollBar->LiftScrollBar(5.f);
		pointFun = (FunctionChan)ScrollItems;
		m_pScrollBar->PointerForScroll(pointFun);
	}
}
//------------------------------------------------------------------------------
void CDChanSelector::ButtonUp()
{
	//if (DCount < _pNum) m_ButtonDown->SetTextColor(255,255,255);
	if (DCount != 0) 
	{
		//m_ButtonUp->SetTextColor(255,255,255);
		DCount -= 1;
		ReDrawChannelTree();
		TRACE ("UP >>>>>>>> val %d\n", DCount);

		float value = (float)DCount / ((float)(m_VisVector.size() - _pVisNum));
		m_pScrollBar->SetValueCur(value);
		//if (DCount == 0) m_ButtonUp->SetTextColor(80,80,80);
	}
}
//------------------------------------------------------------------------------
void CDChanSelector::ButtonDown()
{	
	//if (DCount >= 0) m_ButtonUp->SetTextColor(255,255,255);
	if (_pVisNum + DCount != m_VisVector.size())  
	{
		//m_ButtonDown->SetTextColor(255,255,255);
 		DCount += 1; 
		ReDrawChannelTree();		 
		TRACE ("DOWN >>>> val %d\n", DCount);
		float value = (float)DCount / ((float)(m_VisVector.size() - _pVisNum));
		m_pScrollBar->SetValueCur(value);
		//if (_pVisNum + DCount == m_VisVector.size()) m_ButtonDown->SetTextColor(80,80,80);
	}
}
//------------------------------------------------------------------------------
void CDChanSelector::SetTexture(CString path, bool texture)
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
			m_pButton->CalcTexCoors(PicSize1[0], PicSize1[1]);
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
CString CDChanSelector::GetUseButtonCaption (void)
{
	return m_Vector[m_param.IndexCur].Caption;
}
//------------------------------------------------------------------------------
void CDChanSelector::SetActiveButton (long val)
{
	if (m_Vector[val].hasChildren) val++;
	m_Vector.at(val).m_pButItem->SetTextColor(1,203,247); //выделяем выбранный канал 
	for (UINT i = 0; i < m_Vector.size(); i++)
		m_Vector.at(i).m_pButItem->SetActivePos(false);
	m_Vector.at(val).m_pButItem->SetActivePos(true);
}
//------------------------------------------------------------------------------
void CDChanSelector::ScrollItems()
{
		flagScroll = true;
}
//------------------------------------------------------------------------------
void CDChanSelector::DeleteElementDetails (void)
{
/*	m_param.bVisible = false;
	for (int i = 0; i < m_Vector.size(); i++)
	{
		if (m_Vector[i].m_pButItem)
		{
			m_Vector[i].m_pButItem->Clear();
			delete m_Vector[i].m_pButItem;
		}
		if (m_Vector[i].hasChildren)
		{
			if (m_Vector[i].m_pButSlew)
			{
				m_Vector[i].m_pButSlew->Clear();
				delete m_Vector[i].m_pButSlew;
			}
		}
	}//*/
}
//------------------------------------------------------------------------------
void CDChanSelector::SaveExpand()
{
	m_ExpVector.clear();
	for (UINT i = 0; i < m_Vector.size(); i++)
	{
		if (m_Vector.at(i).hasChildren)
		{
			m_exp.ParentCaption = m_Vector.at(i).ParentCaption;
			m_exp.Caption = m_Vector.at(i).Caption;
			m_exp.expand = m_Vector.at(i).expanded;
			m_ExpVector.push_back(m_exp);
		}
	}
}
//------------------------------------------------------------------------------
void CDChanSelector::ClearVector()
{
	if (m_Vector.size() > 0) 
	{
		DeleteElementDetails();
		DeleteMenuButtons();
	}
	m_Vector.clear();
	m_VisVector.clear();
}
//------------------------------------------------------------------------------
void CDChanSelector::FilterByMeasure(CString val)
{
	int nMeasure(0); //количество строк для сравнения 
	CString arrayOfMeasure[20];
	if (!val.IsEmpty())
	{
		CString s1, str = val;
		for (unsigned int i = 0; i<20; i++)
		{// подстраховка, чтобы не было более 20 позиций
			int n = str.Find(L";");
			if (n != -1)
			{
				s1 = str;
				s1.Delete(n, s1.GetLength() - n);
				s1.Trim();
				arrayOfMeasure[i] = s1;
				str.Delete(0, n + 1);
				nMeasure++;
			}
			else
				break;
		}
	}
	//цикл, который удаляет элементы из вектора, которые не подходят для фильтра
	if (m_Vector.size() > 0)
	{
		int i = 0;
		m_VisVector.clear();
		while (i < m_Vector.size())
		{
			if (m_Vector.at(i).levelType == ChannelName)
			{
				int k = 0;
				for (int j = 0; j < nMeasure; j++)			
					if (m_Vector.at(i).conversion == arrayOfMeasure[j]) k++;
				if (k == 0)
					{
						Tree->DeleteItem(m_Vector.at(i).personalID);
						m_Vector.erase(m_Vector.begin() + i);
					}
				else { i++; } 
			}
			else 
			{
				i++;
			}	
		}

		
		i = 0;
		int h = 0;
		HTREEITEM tItem(NULL);
		HTREEITEM tItem1(NULL);
		//тут происходит удаление элементов, не попавших в отфильтрованный вектор, из дерева
		while (i < m_Vector.size())
		{
			if (m_Vector.at(i).levelType == ComputerName)
			{
				tItem = m_Vector.at(i).personalID;
				int j = 1;
				while ((i + j) < m_Vector.size())
					{
						if (m_Vector.at(i+j).levelType == DeviceName)
						{
							if (m_Vector.at(i + j).parentID == tItem)
							{
								tItem1 = m_Vector.at(i+j).personalID;
								int z = 1;
								while ((i + j + z) < m_Vector.size())
								{
									if (m_Vector.at(i + j + z).levelType == ChannelName)
									{
										if (m_Vector.at(i + j + z).parentID == tItem1) { z++; h++; }
									}
									else if (m_Vector.at(i + j + z).levelType != ChannelName)
										break;					
								}
								if (z == 1)
								{
									Tree->DeleteItem(m_Vector.at(i + j).personalID);
									m_Vector.erase(m_Vector.begin() + i + j);
								}
								else { j++; }
							}
						}
						else { break; }
						j += h;
						h = 0;
					}
				if (j == 1)
				{
					Tree->DeleteItem(m_Vector.at(i).personalID);
					m_Vector.erase(m_Vector.begin() + i);
				}
				else { i++;}
				
			}
			else { i++; }  
		}

		for (int i = 0; i < m_Vector.size(); i++)
		{
			m_VisVector.push_back(&m_Vector.at(i));
		}

	}

	//общие цифры чанселектора обновляются после фильтра
	_pNum = m_Vector.size();
	_pVisNum = m_VisVector.size();
	_pChanNum = 0;
	for (int i = 0; i < m_Vector.size(); i++)
	if (m_Vector.at(i).levelType == ChannelName) _pChanNum++;

	TRACE("FILTERED  BY MEASURE left %d channels \n", _pChanNum);
}
//------------------------------------------------------------------------------
void CDChanSelector::FilterByName(CString val)
{
	int nMeasure(0); //количество строк для сравнения 
	CString arrayOfNames[20];
	if (!val.IsEmpty())
	{
		CString s1, str = val;
		for (unsigned int i = 0; i<20; i++)
		{// подстраховка, чтобы не было более 20 позиций
			int n = str.Find(L";");
			if (n != -1)
			{
				s1 = str;
				s1.Delete(n, s1.GetLength() - n);
				s1.Trim();
				arrayOfNames[i] = s1;
				str.Delete(0, n + 1);
				nMeasure++;
			}
			else
				break;
		}
	}
	int g = 0;
	//цикл, который удаляет элементы из вектора, которые не подходят для фильтра
	if (m_Vector.size() > 0)
	{
		int i = 0;
		m_VisVector.clear();
		while (i < m_Vector.size())
		{
			if (m_Vector.at(i).levelType == ChannelName)
			{
				int k = 0;
				for (int j = 0; j < nMeasure; j++)
					//if (m_Vector.at(i).Caption.Find(arrayOfNames[j]) > 0) k++;
					if (IsWildCardMatching(m_Vector.at(i).Caption, arrayOfNames[j])) k++;
				if (k == 0)
				{
					Tree->DeleteItem(m_Vector.at(i).personalID);
					m_Vector.erase(m_Vector.begin() + i);
				}
				else { i++; }
			}
			else
			{
				i++;
			}

		}

		i = 0;
		int h = 0;
		HTREEITEM tItem(NULL);
		HTREEITEM tItem1(NULL);
		//тут происходит удаление элементов, не попавших в отфильтрованный вектор, из дерева
		while (i < m_Vector.size())
		{
			if (m_Vector.at(i).levelType == ComputerName)
			{
				tItem = m_Vector.at(i).personalID;
				int j = 1;
				while ((i + j) < m_Vector.size())
				{
					if (m_Vector.at(i + j).levelType == DeviceName)
					{
						if (m_Vector.at(i + j).parentID == tItem)
						{
							tItem1 = m_Vector.at(i + j).personalID;
							int z = 1;
							while ((i + j + z) < m_Vector.size())
							{
								if (m_Vector.at(i + j + z).levelType == ChannelName)
								{
									if (m_Vector.at(i + j + z).parentID == tItem1) { z++; h++; }
								}
								else if (m_Vector.at(i + j + z).levelType != ChannelName)
									break;
							}
							if (z == 1)
							{
								Tree->DeleteItem(m_Vector.at(i + j).personalID);
								m_Vector.erase(m_Vector.begin() + i + j);
							}
							else { j++; }
						}
					}
					else { break; }
					j += h;
					h = 0;
				}
				if (j == 1)
				{
					Tree->DeleteItem(m_Vector.at(i).personalID);
					m_Vector.erase(m_Vector.begin() + i);
				}
				else { i++; }

			}
			else { i++; }
		}

		for (int i = 0; i < m_Vector.size(); i++)
		{
			m_VisVector.push_back(&m_Vector.at(i));
		}

	}

	//общие цифры чанселектора обновляются после фильтра
	_pNum = m_Vector.size();
	_pVisNum = m_VisVector.size();
	_pChanNum = 0;
	for (int i = 0; i < m_Vector.size(); i++)
	if (m_Vector.at(i).levelType == ChannelName) _pChanNum++;

	TRACE("FILTERED  BY NAME left %d channels \n", _pChanNum);
}
//------------------------------------------------------------------------------
bool CDChanSelector::IsWildCardMatching(const CString &sStr, const CString &sWild)
{
	if (sStr.Find(sWild) != -1)
		return true;

	int i = 0, j = 0, strLen = sStr.GetLength(), wildLen = sWild.GetLength();
	wchar_t strChr, wildChr;
	bool skipAll = false, result = true; 
	int skipOne = 0, diffs = 0;

	while (true)
	{
		if (j < wildLen)
		{
			wildChr = sWild[j];
			if (wildChr == L'*')
			{
				skipAll = true;
				j++;
				continue;
			}
			else if (wildChr == L'?')
			{
				j++;
				skipOne++;
				if (skipAll)
				{
					continue;
				}
			}
			else if (!skipAll)
			{
				j++;
			}
		}
		else
		{
			result = skipOne <= (diffs + strLen - i) && (skipAll || i >= strLen);
			break;
		}

		if (i >= strLen)
		{
			result = j >= wildLen;
			break;
		}
		else
		{
			strChr = i < strLen ? sStr[i] : L'';
			bool diffirent = ChrCmpI(strChr, wildChr) != 0;
			if (diffirent)
			{
				if (!skipAll)
				{
					if (skipOne == 0)
					{
						result = false;
						break;
					}
					skipOne--;
				}
				diffs++;
			}
			else
			{
				if (skipOne > 0)
				{
					bool equalDiff = skipAll ? skipOne > diffs : skipOne != diffs;
					if (equalDiff)
					{
						result = false;
						break;
					}
					skipOne = 0;
				}
				if (skipAll)
				{
					skipAll = false;
					j++;
				}
				diffs = 0;
			}
			i++;
		}
	}

	return result;
}
//------------------------------------------------------------------------------
void CDChanSelector::FilterByIdChan(CString val)
{
	int nMeasure(0); //количество строк для сравнения 
	long arrayOfIdChan[20];
	if (!val.IsEmpty())
	{
		CString s1, str = val;
		for (unsigned int i = 0; i<20; i++)
		{// подстраховка, чтобы не было более 20 позиций
			int n = str.Find(L";");
			if (n != -1)
			{
				s1 = str;
				s1.Delete(n, s1.GetLength() - n);
				s1.Trim();
				CStringA q(s1);
				arrayOfIdChan[i] = atol(q);
				str.Delete(0, n + 1);
				nMeasure++;
			}
			else
				break;
		}
	}

	//удаляются те элементы, параметры которых указаны в фильтре
	if (m_Vector.size() > 0)
	{
		int i = 0;
		m_VisVector.clear();
		while (i < m_Vector.size())
		{
			if (m_Vector.at(i).levelType == ChannelName)
			{
				int k = 0;
				for (int j = 0; j < nMeasure; j++)
					if (m_Vector.at(i).idChan == arrayOfIdChan[j])
					{
						Tree->DeleteItem(m_Vector.at(i).personalID);
						m_Vector.erase(m_Vector.begin() + i);
						k++;
					}
				if (k == 0) i++;
			}
			else
			{
				i++;
			}
		}

		i = 0;
		int h = 0;
		HTREEITEM tItem(NULL);
		HTREEITEM tItem1(NULL);
		//тут происходит удаление элементов, не попавших в отфильтрованный вектор, из дерева
		while (i < m_Vector.size())
		{
			if (m_Vector.at(i).levelType == ComputerName)
			{
				tItem = m_Vector.at(i).personalID;
				int j = 1;
				while ((i + j) < m_Vector.size())
				{
					if (m_Vector.at(i + j).levelType == DeviceName)
					{
						if (m_Vector.at(i + j).parentID == tItem)
						{
							tItem1 = m_Vector.at(i + j).personalID;
							int z = 1;
							while ((i + j + z) < m_Vector.size())
							{
								if (m_Vector.at(i + j + z).levelType == ChannelName)
								{
									if (m_Vector.at(i + j + z).parentID == tItem1) { z++; h++; }
									//else
								}
								else if (m_Vector.at(i + j + z).levelType != ChannelName)
									break;
							}
							if (z == 1)
							{
								Tree->DeleteItem(m_Vector.at(i + j).personalID);
								m_Vector.erase(m_Vector.begin() + i + j);
							}
							else { j++; }
						}
					}
					else { break; }
					j += h;
					h = 0;
				}
				if (j == 1)
				{
					Tree->DeleteItem(m_Vector.at(i).personalID);
					m_Vector.erase(m_Vector.begin() + i);
				}
				else { i++; }

			}
			else { i++; }
		}

		for (int i = 0; i < m_Vector.size(); i++)
		{
			m_VisVector.push_back(&m_Vector.at(i));
		}
	}

	//общие цифры чанселектора обновляются после фильтра
	_pNum = m_Vector.size();
	_pVisNum = m_VisVector.size();
	_pChanNum = 0;
	for (int i = 0; i < m_Vector.size(); i++)
	if (m_Vector.at(i).levelType == ChannelName) _pChanNum++;

	TRACE("FILTERED  BY IDCHAN left %d channels \n", _pChanNum);
}
//------------------------------------------------------------------------------
#endif
