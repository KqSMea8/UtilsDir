//------------------------------------------------------------------------------
// Main.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include <Interface2012\ZetLab\Main.h>
#include <Interface2012\Custom\Dialog_ZETi12.h>
//------------------------------------------------------------------------------
CMain::CMain(CWnd* parent, CRect rect, CString sFolderPNG,
		bool bButtonHelp, bool bButtonMin, bool bButtonMax, bool bButtonEnd)
	: CCustomMain(parent, rect, sFolderPNG)
{
	//TRACE("CREATE CMain %s\n", (CStringA)sFolderPNG);
#ifdef _GDIPLUS
	m_pDMain = new CDMain(rect, &m_pGraphics, &m_bIsChange, GetPointerToTime());
	m_pDMain->SetImageFileName(m_sFolderPNG + L"\\Main\\Main");
#else
	m_pDMain = new CDMain(rect, &m_bIsChange, GetPointerToTime());
	if (m_pParent)
		m_pDMain->SethWnd(m_pParent->m_hWnd);
    
#endif
	m_sizeBorder = 5;
	CRect w_rect(0, 0, 0, 0);
	m_pDClass = m_pDMain;	
	m_rect = rect;
	m_data = m_param = 0;
	m_paramAdd = 0;
	m_bAddRect = false;
	GetClientRect(m_pParent->m_hWnd, &w_rect);
	m_topSizeRect = CRect(m_rect.left + m_sizeBorder, m_rect.top, m_rect.right - m_sizeBorder, m_rect.top + m_sizeBorder);
	m_leftTopSizeRect = CRect(m_rect.left, m_rect.top, m_rect.left + m_sizeBorder, m_rect.top + m_sizeBorder);
	m_leftSizeRect = CRect(m_rect.left, m_rect.top + m_sizeBorder, m_rect.left + m_sizeBorder, m_rect.bottom - m_sizeBorder);
	m_leftBottomSizeRect = CRect(m_rect.left, m_rect.bottom - m_sizeBorder, m_rect.left + m_sizeBorder, m_rect.bottom);
	m_bottomSizeRect = CRect(m_rect.left + m_sizeBorder, m_rect.bottom - m_sizeBorder, m_rect.right - m_sizeBorder, m_rect.bottom);
	m_rightBottomSizeRect = CRect(m_rect.right - m_sizeBorder, m_rect.bottom - m_sizeBorder, m_rect.right, m_rect.bottom);
	m_rightSizeRect = CRect(m_rect.right - m_sizeBorder, m_rect.top + m_sizeBorder, m_rect.right, m_rect.bottom - m_sizeBorder);
	m_rightTopSizeRect = CRect(m_rect.right - m_sizeBorder, m_rect.top, m_rect.right, m_rect.top + m_sizeBorder);
	m_topSize = m_bottomSize =	m_leftSize = m_rightSize = false;
	if (m_pParent)
	{
		m_pRegion0 = new CRgn();
		m_pRegion1 = new CRgn();
		m_pRegion2 = new CRgn();
		m_pRegion3 = new CRgn();
		m_pRegion4 = new CRgn();
		m_pRegion5 = new CRgn(); //zettest
		//m_RegionWnd.CreateRoundRectRgn( 0, 0, m_rect.right+10, m_rect.bottom+10, 20, 20);
#ifdef _ACTIVEX2012
		m_RegionWnd.CreateRectRgn(0, 0, m_rect.right + 1, m_rect.bottom + 1);
#else
		m_RegionWnd.CreateRoundRectRgn(0, 0, w_rect.right + 1, w_rect.bottom + 1, 20, 20);
#endif
		m_pParent->SetWindowRgn((HRGN)m_RegionWnd.m_hObject, TRUE);
		m_hCursor = (HCURSOR)GetClassLongW(m_pParent->m_hWnd, GCL_HCURSOR);
	}
	else
		m_hCursor = 0;
	m_bIsChange = false;
	// создаю системные кнопки -----------------------------------------------
	int height = m_pDMain->GetSizeIcon();		// высота кнопок 13	// 19
	int width  = int(height * 1.8f);		// ширина кнопок 21	// 31//1.618f
	//int width = height;
	CRect _rect = m_pDMain->GetRect2();
	_rect.top += 5;
	_rect.right = _rect.right - 5;
	_rect.left = _rect.right - width;
	_rect.bottom = _rect.top + height;
	// создаю кнопки именно в таком пор€дке
	int i = 0;
	m_pButtonEnd = m_pButtonMax = m_pButtonMin = m_pButtonHelp = nullptr;
	if (bButtonEnd)
	{
		m_pButtonEnd = new CButtonHMME(this, _rect, tbHMME_End);
		i++;
	}
	if (bButtonMax)
	{
		_rect.right = _rect.left;
		_rect.left = _rect.right - width;
		m_pButtonMax = new CButtonHMME(this, _rect, tbHMME_Max);
		i++;
	}
	if (bButtonMin)
	{
		_rect.right = _rect.left;
		_rect.left = _rect.right - width;
		m_pButtonMin = new CButtonHMME(this, _rect, tbHMME_Min);
		i++;
	}
	if (bButtonHelp)
	{
		_rect.right = _rect.left;
		_rect.left = _rect.right - width;
		m_pButtonHelp = new CButtonHMME(this, _rect, tbHMME_Help);
		i++;
	}
	m_pDMain->SetWidthHMME(i * width);

}
//------------------------------------------------------------------------------
CMain::~CMain()
{
	//TRACE("DELETE CMain \n");
	if (m_pDMain)
	{ 
		delete m_pDMain;
		m_pDMain = nullptr;
	}
	if (m_pRegion0)
	{ 
		delete m_pRegion0;
		m_pRegion0 = nullptr;
	}
	if (m_pRegion1)
	{ 
		delete m_pRegion1;
		m_pRegion1 = nullptr;
	}

	if (m_pRegion2)
	{
		delete m_pRegion2;
		m_pRegion2 = nullptr;
	}
	if (m_pRegion3)
	{
		delete m_pRegion3;
		m_pRegion3 = nullptr;
	}

	if (m_pRegion4)
	{
		delete m_pRegion4;
		m_pRegion4 = nullptr;
	}
	if (m_pButtonEnd)
	{ 
		delete m_pButtonEnd;
		m_pButtonEnd = nullptr;
	}
	if (m_pButtonMax)
	{ 
		delete m_pButtonMax;
		m_pButtonMax = nullptr;
	}
	if (m_pButtonMin)
	{ 
		delete m_pButtonMin;
		m_pButtonMin = nullptr;
	}
	if (m_pButtonHelp)
	{ 
		delete m_pButtonHelp;
		m_pButtonHelp = nullptr;
	}
}
//------------------------------------------------------------------------------
void CMain::Resize(CRect rect)
{
	m_rect = rect;
	Init();
	m_topSizeRect = CRect(m_rect.left + m_sizeBorder, m_rect.top, m_rect.right - m_sizeBorder, m_rect.top + m_sizeBorder);
	m_leftTopSizeRect = CRect(m_rect.left, m_rect.top, m_rect.left + m_sizeBorder, m_rect.top + m_sizeBorder);
	m_leftSizeRect = CRect(m_rect.left, m_rect.top + m_sizeBorder, m_rect.left + m_sizeBorder, m_rect.bottom - m_sizeBorder);
	m_leftBottomSizeRect = CRect(m_rect.left, m_rect.bottom - m_sizeBorder, m_rect.left + m_sizeBorder, m_rect.bottom);
	m_bottomSizeRect = CRect(m_rect.left + m_sizeBorder, m_rect.bottom - m_sizeBorder, m_rect.right - m_sizeBorder, m_rect.bottom);
	m_rightBottomSizeRect = CRect(m_rect.right - m_sizeBorder, m_rect.bottom - m_sizeBorder, m_rect.right, m_rect.bottom);
	m_rightSizeRect = CRect(m_rect.right - m_sizeBorder, m_rect.top + m_sizeBorder, m_rect.right, m_rect.bottom - m_sizeBorder);
	m_rightTopSizeRect = CRect(m_rect.right - m_sizeBorder, m_rect.top, m_rect.right, m_rect.top + m_sizeBorder);
	m_pDMain->Resize(rect);
	CRect _rect = m_pDMain->GetRect2();
	int height = m_pDMain->GetSizeIcon();		// высота кнопок 13	// 19
	int width = int(height * 1.8f);		// ширина кнопок 21	// 31//1.618f
	//int width = height;
	_rect.top += 5;
	_rect.right = _rect.right - 5;
	_rect.left = _rect.right - width;
	_rect.bottom = _rect.top + height;
	// создаю кнопки именно в таком пор€дке
	int i = 0;
	if (m_pButtonEnd)
	{
		m_pButtonEnd->Resize(_rect);
		i++;
	}
	if (m_pButtonMax)
	{
		_rect.right = _rect.left;
		_rect.left = _rect.right - width;
		m_pButtonMax->Resize(_rect);
		i++;
	}
	if (m_pButtonMin)
	{
		_rect.right = _rect.left;
		_rect.left = _rect.right - width;
		m_pButtonMin->Resize(_rect);
		i++;
	}
	if (m_pButtonHelp)
	{
		_rect.right = _rect.left;
		_rect.left = _rect.right - width;
		m_pButtonHelp->Resize(_rect);
		i++;
	}
	m_pDMain->SetWidthHMME(i * width);
}
//------------------------------------------------------------------------------
void CMain::Clear (void)
{
	if (m_pDMain)
		m_pDMain->Clear();
	if (m_pButtonEnd)
		m_pButtonEnd->Clear();
	if (m_pButtonMax)
		m_pButtonMax->Clear();
	if (m_pButtonMin)
		m_pButtonMin->Clear();
	if (m_pButtonHelp)
		m_pButtonHelp->Clear();
}
//------------------------------------------------------------------------------
void CMain::Init()
{
	m_pRegion0->DeleteObject();
	m_pRegion1->DeleteObject();
	m_pRegion2->DeleteObject();
	m_pRegion3->DeleteObject();
	m_pRegion4->DeleteObject();
	m_pRegion5->DeleteObject(); //zettest
	m_RegionWnd.DeleteObject();
#ifdef _ACTIVEX2012
	m_pRegion0->CreateRectRgn( 0, 0, m_rect.right + 1, m_rect.bottom + 1);
#else
	m_pRegion0->CreateRoundRectRgn(0, 0, m_rect.right + 1, m_rect.bottom + 1, 20, 20);
#endif
	m_pRegion1->CreateRectRgn(m_rect.left + m_data.left, m_rect.bottom, m_rect.left + m_data.right, m_rect.bottom + m_data.Height()/2);
	m_pRegion2->CreateRoundRectRgn(m_rect.left + m_data.left, m_rect.bottom, m_rect.left + m_data.right + 1, m_rect.bottom + m_data.Height() + 1, 20, 20);
	//m_pRegion3->CreateRectRgn(m_rect.right, m_rect.top + m_param.top, m_rect.right + m_param.Width()/2, m_rect.top + m_param.bottom);
	m_pRegion3->CreateRectRgn(m_rect.right, m_rect.top + m_param.top, m_rect.right + m_param.Width()/2, m_rect.bottom - 10);
	//m_pRegion4->CreateRoundRectRgn(m_rect.right, m_rect.top + m_param.top, m_rect.right + m_param.Width() + 1, m_rect.top + m_param.bottom + 1, 20, 20);
	m_pRegion4->CreateRoundRectRgn(m_rect.right, m_rect.top + m_param.top, m_rect.right + m_param.Width() + 1, m_rect.bottom - 10, 20, 20);
	m_pRegion5->CreateRectRgn(m_paramAdd.left, m_paramAdd.top, m_paramAdd.right, m_paramAdd.bottom); //zettest
	m_RegionWnd.CreateRectRgn(0, 0, 0, 0);
	m_RegionWnd.CombineRgn(m_pRegion1, m_pRegion2, RGN_OR);
	m_RegionWnd.CombineRgn(&m_RegionWnd, m_pRegion0, RGN_OR);
	m_RegionWnd.CombineRgn(&m_RegionWnd, m_pRegion3, RGN_OR);
	m_RegionWnd.CombineRgn(&m_RegionWnd, m_pRegion4, RGN_OR);
	if (m_bAddRect) m_RegionWnd.CombineRgn(&m_RegionWnd, m_pRegion5, RGN_OR); //zettest

	m_pParent->SetWindowRgn((HRGN)m_RegionWnd.m_hObject, TRUE);
}
//------------------------------------------------------------------------------
void CMain::OnLButtonDown(UINT nFlags, CPoint point)
{
	CloseMenu();
	CCustomContainer::OnLButtonDown(nFlags, point);
	m_leftButtonDownPoint = point;
	//≈сли клик мыши пришелс€ на одну из областей ресайза, то взводим флаги
	//об изменении размера
	if (PointInRect(point, &m_topSizeRect))
		m_topSize = true;

	if (PointInRect(point, &m_bottomSizeRect))
		m_bottomSize = true;

	if (PointInRect(point, &m_leftSizeRect))
		m_leftSize = true;

	if (PointInRect(point, &m_rightSizeRect))
		m_rightSize = true;

	//ѕосылаем сообщение окну, чтобы оно думало, что кликнули по его заголовку,
	//что позволит его перетаскивать
	if (PointInRect(point, &CRect(m_rect.left + m_sizeBorder, m_rect.top + m_sizeBorder,
		m_rect.right - m_sizeBorder, m_rect.bottom - m_sizeBorder))){
		MessageToParent(WM_NCLBUTTONDOWN, HTCAPTION, 0);
	}
	else
		SetCapture(m_pParent->m_hWnd);
}
//------------------------------------------------------------------------------
void CMain::OnMouseMove(UINT nFlags, CPoint point)
{
	CCustomContainer::OnMouseMove(nFlags, point);
	if (nFlags == MK_LBUTTON){
		if (m_topSize){
			MessageToParent(WM_SIZE, SIZE_RESTORED, MAKELPARAM(m_rect.Width(), m_rect.Height() + (m_leftButtonDownPoint.y - point.y)));
		}

		if (m_bottomSize){
			MessageToParent(WM_SIZE, SIZE_RESTORED, MAKELPARAM(m_rect.Width(), m_rect.Height() - (m_leftButtonDownPoint.y - point.y)));
		}

		if (m_leftSize){
			MessageToParent(WM_SIZE, SIZE_RESTORED, MAKELPARAM(m_rect.Width() + (m_leftButtonDownPoint.x - point.x), m_rect.Height()));
		}

		if (m_rightSize){
			MessageToParent(WM_SIZE, SIZE_RESTORED, MAKELPARAM(m_rect.Width() - (m_leftButtonDownPoint.x - point.x), m_rect.Height()));
		}

		m_leftButtonDownPoint = point;
	}

	if (PointInRect(point, &m_topSizeRect))
	{
		m_hCursor = LoadCursorW(NULL, IDC_SIZENS);
		SetClassLongW(GetParent()->m_hWnd, GCL_HCURSOR, (LONG)m_hCursor);
	}

	if (PointInRect(point, &m_bottomSizeRect))
	{
		m_hCursor = LoadCursorW(NULL, IDC_SIZENS);
		SetClassLongW(GetParent()->m_hWnd, GCL_HCURSOR, (LONG)m_hCursor);
	}

	if (PointInRect(point, &m_leftSizeRect))
	{
		m_hCursor = LoadCursorW(NULL, IDC_SIZEWE);
		SetClassLongW(GetParent()->m_hWnd, GCL_HCURSOR, (LONG)m_hCursor);
	}

	if (PointInRect(point, &m_rightSizeRect))
	{
		m_hCursor = LoadCursorW(NULL, IDC_SIZEWE);
		SetClassLongW(GetParent()->m_hWnd, GCL_HCURSOR, (LONG)m_hCursor);
	}

	if (PointInRect(point, &CRect(m_rect.left + m_sizeBorder, m_rect.top + m_sizeBorder,
		m_rect.right - m_sizeBorder, m_rect.bottom - m_sizeBorder))){
		m_hCursor = LoadCursorW(NULL, IDC_ARROW);
		SetClassLongW(GetParent()->m_hWnd, GCL_HCURSOR, (LONG)m_hCursor);
	}
}
//------------------------------------------------------------------------------
void CMain::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_topSize){
		MessageToParent(WM_SIZE, SIZE_RESTORED, MAKELPARAM(m_rect.Width(),
			m_rect.Height() + (m_leftButtonDownPoint.y - point.y)));
		m_topSize = false;
	}

	if (m_bottomSize){
		MessageToParent(WM_SIZE, SIZE_RESTORED, MAKELPARAM(m_rect.Width(),
			m_rect.Height() - (m_leftButtonDownPoint.y - point.y)));
		m_bottomSize = false;
	}

	if (m_leftSize){
		MessageToParent(WM_SIZE, SIZE_RESTORED, MAKELPARAM(m_rect.Width()
			+ (m_leftButtonDownPoint.x - point.x), m_rect.Height()));
		m_leftSize = false;
	}

	if (m_rightSize){
		MessageToParent(WM_SIZE, SIZE_RESTORED, MAKELPARAM(m_rect.Width()
			- (m_leftButtonDownPoint.x - point.x), m_rect.Height()));
		m_rightSize = false;
	}

	ReleaseCapture();
}
//------------------------------------------------------------------------------
void CMain::SetDataRect(CRect rect)
{
	if (m_data != rect)
	{
		m_data = rect;
		Init();
	}
}
//------------------------------------------------------------------------------
void CMain::SetParamRect(CRect rect)
{
	if (m_param != rect)
	{
		m_param = rect;
		Init();
	}
}
//------------------------------------------------------------------------------
void CMain::SaveParameters(SimpleXML *pSimpleXML, CString FileName)
{
    m_pDMain->SaveParameters(pSimpleXML);
    CCustomContainer::SaveParameters(pSimpleXML);
    pSimpleXML->Save(FileName);
}
//------------------------------------------------------------------------------
void CMain::LoadParameters(SimpleXML *pSimpleXML, CString FileName)
 {
     pSimpleXML->Load(FileName);
     m_pDMain->LoadParameters(pSimpleXML);
     CCustomContainer::LoadParameters(pSimpleXML);
 }
//------------------------------------------------------------------------------
void CMain::SetParamAddRect(CRect rect)
{
	if (m_paramAdd != rect)
	{
		m_paramAdd = rect;
		if (m_paramAdd.Width() > 0 ) m_bAddRect = true;
		else m_bAddRect = false;
		Init();
	}
}