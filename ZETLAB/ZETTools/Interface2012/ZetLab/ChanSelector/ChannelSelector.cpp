//------------------------------------------------------------------------------
// ChanSelector.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\ZetLab\ChanSelector\ChannelSelector.h"
#include "C:\ZETTools\Interface2012\Custom\Custom_ZETi12.h"
#include "C:\ZETTools\Interface2012\Custom\Dialog_ZETi12.h"
//------------------------------------------------------------------------------
CChannelSelector::CChannelSelector(CCustomContainer *owner, CRect rect)
	: CCustomItem(owner)
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
	m_pDChanSelector = NULL;
	if (m_pOwner)
	{
		CWnd *m_pParent = GetParent();
		StrHeight = 15;
#ifdef _GDIPLUS
		m_pDChanSelector = new CDChannelSelector(rect, m_pOwner->GetPointerToGraphics(),
			m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime() );
#else
		m_pDChanSelector = new CDChannelSelector(rect,
			m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime() );
#endif
		m_pDClass = m_pDChanSelector;
		m_psrv = new CDSRV();
		m_psrv->Create(L"SRV for ChanSelector", WS_CHILD, CRect(0,0,0,0), m_pParent, 1701);
		m_psrv->Connect();
		quan = m_psrv->QuanChan();
		m_pPanel = new CChanPanel(owner, CRect(rect.left, rect.bottom, rect.right, rect.bottom + StrHeight*quan + 2));
		m_pPanel->SetCDSRV(m_psrv);
	}
}
//------------------------------------------------------------------------------
CChannelSelector::~CChannelSelector()
{
	if (m_pDChanSelector)
		delete m_pDChanSelector;
}
//------------------------------------------------------------------------------
void CChannelSelector::SetPositionValue(BOOL val)
{
	BOOL oldVal = m_pDChanSelector->GetPositionValue();
	m_pDChanSelector->SetPositionValue(val);
	if ( oldVal != m_pDChanSelector->GetPositionValue() )
		NewLong( m_pDChanSelector->GetPositionValue() );
}
//------------------------------------------------------------------------------
void CChannelSelector::OnMouseLeave()
{
	CCustomElement::OnMouseLeave();
	if ( m_pDChanSelector->GetNeedEvent() )
		NewLong( m_pDChanSelector->GetPositionValue() );
}
//------------------------------------------------------------------------------
void CChannelSelector::OnLButtonClk(UINT nFlags, CPoint point)
{
	CCustomItem::OnLButtonClk(nFlags, point);
	if ( m_pDChanSelector->GetNeedEvent() )
		NewLong( m_pDChanSelector->GetPositionValue() );
	m_pPanel->ClickButton();
}
//------------------------------------------------------------------------------
void CChannelSelector::OnLButtonUp(UINT nFlags, CPoint point)
{
	CCustomItem::OnLButtonUp(nFlags, point);
	if ( m_pDChanSelector->GetNeedEvent() )
		NewLong( m_pDChanSelector->GetPositionValue() );
}
//------------------------------------------------------------------------------

