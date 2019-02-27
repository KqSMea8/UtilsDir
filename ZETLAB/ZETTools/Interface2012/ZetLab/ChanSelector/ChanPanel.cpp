//------------------------------------------------------------------------------
// ChanSelector.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\ZetLab\ChanSelector\ChanPanel.h"
#include "C:\ZETTools\Interface2012\Custom\Custom_ZETi12.h"
//------------------------------------------------------------------------------
CChanPanel::CChanPanel(CCustomContainer *owner, CRect rect)
	: CCustomButton(owner)
{
	/*m_bPress = false;	// кнопка не нажата
	m_CurChan = 0;		// текущий канал - 0-ой
	m_pTree = NULL;		// дерева ещё нет
	if (m_pOwner)
	{
		
		if (m_pParent)
		{
			m_psrv = new CDSRV();
			m_psrv->Create(L"SRV for ChanSelector", WS_CHILD, CRect(0,0,0,0), m_pParent, 1701);
		}
	}*/

	CString name;
	m_rect = rect;
	m_pDChanPanel = NULL;
	if (m_pOwner)
	{
#ifdef _GDIPLUS
		m_pDChanPanel = new CDChanPanel(rect, m_pOwner->GetPointerToGraphics(),
			m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime() );
#else
		m_pDChanPanel = new CDChanPanel(rect,
			m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime() );
#endif
		m_pDClass = m_pDChanPanel;
	}
}
//------------------------------------------------------------------------------
CChanPanel::~CChanPanel()
{
	if (m_pDChanPanel)
		delete m_pDChanPanel;
}



