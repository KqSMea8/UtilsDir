//------------------------------------------------------------------------------
// ChanSelector.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "resource.h"
//#include "resource.h"
#include "C:\ZETTools\Interface2012\ZetLab\ChanSelector.h"
#include "C:\ZETTools\Interface2012\Custom\Custom_ZETi12.h"
//#include "TreePanel.h"
//------------------------------------------------------------------------------
CChanSelector::CChanSelector(CCustomContainer *owner, CRect rect, CDSRV* m_psrv, CRect ownerRect)
	: CCustomItem(owner)
	, m_ChannelTree(NULL)
	, m_ChannelInfo(NULL)
{
	CString name;
	m_pDChanSelector = NULL;
	if (m_pOwner)
	{
		CWnd *m_pParent = GetParent();
#ifdef _GDIPLUS
		m_pDChanSelector = new CDChanSelector(rect, m_pOwner->GetPointerToGraphics(),
			m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime() );
		m_pDClass = m_pDChanSelector;
		m_psrv = new CDSRV();
		m_psrv->Create(L"SRV for ChanSelector", WS_CHILD, CRect(0,0,0,0), m_pParent, 1701);
		m_psrv->Connect();
		SetCaption(m_psrv->Commentary(0));
#else
		m_ChannelInfo = new CChannelInfo(m_psrv);
		m_ChannelTree = new CTreeCtrl();
		m_ChannelTree->Create(WS_CHILD, CRect(0, 0, 0, 0), owner->GetParent(), IDC_TREE);

		m_pDChanSelector = new CDChanSelector(rect, m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime() );
		m_pDClass = m_pDChanSelector;
		m_pDChanSelector->SetParent(owner);
		m_rect = rect;
		m_pDChanSelector->SetStartItemID(m_StartItemID);
		m_pDChanSelector->SetSpaceSize(ownerRect);
#endif
	}
	//FillChannelTree();
}
//------------------------------------------------------------------------------
CChanSelector::~CChanSelector()
{
	if (m_pDChanSelector)
		delete m_pDChanSelector;
	if (m_ChannelInfo)
		delete m_ChannelInfo;
	if (m_ChannelTree)
		delete m_ChannelTree;
}
//------------------------------------------------------------------------------
void CChanSelector::SetNewTextString (long val)
{
	m_pDChanSelector->ClearVector();
	FillChannelTree(); 
	SetIndexCur(val); 
	m_pDChanSelector->SetCaption(m_pDChanSelector->GetUseButtonCaption());
	m_pOwner->OpenMenu = false;
}
//------------------------------------------------------------------------------
void CChanSelector::Clear(void)
{
	m_pDChanSelector->Clear();
}
//------------------------------------------------------------------------------
void CChanSelector::FillChannelTree()
{
	m_ChannelInfo->FillChannelInfo();
	m_ChannelTree->DeleteAllItems();

	UINT_PTR id = m_StartItemID;
	CString pTemp; //хранит строку родительского элемента
	HTREEITEM hParentItem(NULL);

	HTREEITEM item = NULL;
	TVINSERTSTRUCT structure;
	structure.item.mask = TVIF_TEXT | TVIF_PARAM;
	for (int i = 0; i < m_ChannelInfo->m_ChannelInfoMap[0].lLevels; ++i)
	{

		if (item != NULL)
		{
			structure.hParent = item;
			structure.item.pszText = m_ChannelInfo->m_ChannelInfoMap[0].pLevels[i].GetBuffer();
			//structure.item.lParam = 0;
			structure.item.lParam = (LPARAM)(LPCTSTR)m_ChannelInfo->m_ChannelInfoMap[i].sConversion;

			item = m_ChannelTree->InsertItem(&structure);
			m_ChannelTree->SelectItem(item);

			hParentItem = m_ChannelTree->GetParentItem(item);
			pTemp = m_ChannelTree->GetItemText(hParentItem);
			m_pDChanSelector->SetItemID(id);
			m_pDChanSelector->SetItemVisible(false);
			m_pDChanSelector->SetItemCaption(m_ChannelTree->GetItemText(item));
			m_pDChanSelector->SetItemParentCaption(pTemp);
			m_pDChanSelector->SetHTREEITEMItem(item);
			m_pDChanSelector->SetHTREEITEMParent(hParentItem);
			m_pDChanSelector->SetItemConversion(m_ChannelInfo->m_ChannelInfoMap[i].sConversion);
			m_pDChanSelector->SetIdChan(m_ChannelInfo->m_ChannelInfoMap[0].idChan);
			m_pDChanSelector->InsertVectorItem();
			id++;


		}
		else
		{
			structure.hParent = NULL;
			structure.item.pszText = m_ChannelInfo->m_ChannelInfoMap[0].pLevels[i].GetBuffer();
			structure.item.lParam = 0;
			structure.item.lParam = (LPARAM)(LPCTSTR)m_ChannelInfo->m_ChannelInfoMap[i].sConversion;
			LPCTSTR lpszString = (LPCTSTR)structure.item.lParam;
			CString s = lpszString;

			item = m_ChannelTree->InsertItem(&structure);
			m_ChannelTree->SelectItem(item);

			hParentItem = m_ChannelTree->GetParentItem(item);
			pTemp = m_ChannelTree->GetItemText(hParentItem);
			m_pDChanSelector->SetItemID(id);
			m_pDChanSelector->SetItemVisible(false);
			m_pDChanSelector->SetItemCaption(m_ChannelTree->GetItemText(item));
			m_pDChanSelector->SetItemParentCaption(pTemp);
			m_pDChanSelector->SetHTREEITEMItem(item);
			m_pDChanSelector->SetHTREEITEMParent(hParentItem);
			m_pDChanSelector->SetItemConversion(m_ChannelInfo->m_ChannelInfoMap[i].sConversion);
			
			if (i == 2) m_pDChanSelector->SetIdChan(m_ChannelInfo->m_ChannelInfoMap[0].idChan); // присваивается id первого сигнала в дереве
			m_pDChanSelector->InsertVectorItem();
			id++;

		}
	}
	long lQuantity(m_ChannelInfo->m_ChannelInfoMap.size());

	for (int i = 1; i < lQuantity; i++)
	{
		HTREEITEM tempItem = NULL;
		tempItem = m_ChannelTree->GetParentItem(item);
		while (tempItem != NULL)
		{
			item = tempItem;
			tempItem = m_ChannelTree->GetParentItem(item);
		}
		tempItem = m_ChannelTree->GetNextItem(item, TVGN_PREVIOUS);
		while (tempItem != NULL)
		{
			item = tempItem;
			tempItem = m_ChannelTree->GetNextItem(item, TVGN_PREVIOUS);
		}

		for (int j = 0; j < m_ChannelInfo->m_ChannelInfoMap[i].lLevels; ++j)
		{
			bool bFind;
			CString sTemp(m_ChannelTree->GetItemText(item));
			if (j < m_ChannelInfo->m_ChannelInfoMap[i].lLevels - 1)
				//поиск записи на данном уровне дерева
				bFind = FindItemInLevel(item, m_ChannelInfo->m_ChannelInfoMap[i].pLevels[j], tempItem);
			else
				bFind = false;
			//если запись существует
			if (bFind)
			{
				//переходим к найденной записи
				item = tempItem;
				//переходим на уровень ниже
				item = m_ChannelTree->GetChildItem(item);
				sTemp = m_ChannelTree->GetItemText(item);
			}
			else
			{
				//если запись находится в середине дерева
				if (m_ChannelTree->GetParentItem(item))
				{
					//переходим к ее родителю
					item = m_ChannelTree->GetParentItem(item);
					//вставляем цепочку этой записи до конца
					for (int k = j; k < m_ChannelInfo->m_ChannelInfoMap[i].lLevels; ++k)
					{
						structure.hParent = item;
						structure.item.pszText = m_ChannelInfo->m_ChannelInfoMap[i].pLevels[j].GetBuffer();
						structure.item.lParam = i;

						item = m_ChannelTree->InsertItem(&structure);
						m_ChannelTree->SelectItem(item);

						hParentItem = m_ChannelTree->GetParentItem(item);
						pTemp = m_ChannelTree->GetItemText(hParentItem);
						m_pDChanSelector->SetItemID(id);
						m_pDChanSelector->SetItemVisible(false);
						m_pDChanSelector->SetItemCaption(m_ChannelTree->GetItemText(item));
						m_pDChanSelector->SetItemParentCaption(pTemp);
						m_pDChanSelector->SetHTREEITEMItem(item);
						m_pDChanSelector->SetHTREEITEMParent(hParentItem);
						m_pDChanSelector->SetItemConversion(m_ChannelInfo->m_ChannelInfoMap[i].sConversion);
						m_pDChanSelector->SetIdChan(m_ChannelInfo->m_ChannelInfoMap[i].idChan);
						m_pDChanSelector->InsertVectorItem();
						id++;

						++j;
					}
				}
				//если это вершина дерева
				else
				{
					//вставляем корень
					if (m_ChannelInfo->m_ChannelInfoMap[i].iStatus != 5)
					{

						structure.hParent = item;
						structure.item.pszText = m_ChannelInfo->m_ChannelInfoMap[i].pLevels[j].GetBuffer();

						item = m_ChannelTree->InsertItem(m_ChannelInfo->m_ChannelInfoMap[i].pLevels[j]);

						hParentItem = m_ChannelTree->GetParentItem(item);
						pTemp = m_ChannelTree->GetItemText(hParentItem);
						m_pDChanSelector->SetItemID(id);
						m_pDChanSelector->SetItemVisible(false);
						m_pDChanSelector->SetItemCaption(m_ChannelTree->GetItemText(item));
						m_pDChanSelector->SetItemParentCaption(pTemp);
						m_pDChanSelector->SetHTREEITEMItem(item);
						m_pDChanSelector->SetHTREEITEMParent(hParentItem);
						m_pDChanSelector->SetItemConversion(m_ChannelInfo->m_ChannelInfoMap[i].sConversion);
						m_pDChanSelector->SetIdChan(m_ChannelInfo->m_ChannelInfoMap[i].idChan);
						m_pDChanSelector->InsertVectorItem();
						id++;

						++j;
					}
					//вставляем всю цепочку до конца
					for (int k = j; k < m_ChannelInfo->m_ChannelInfoMap[i].lLevels; ++k)
					{
						structure.hParent = item;
						structure.item.pszText = m_ChannelInfo->m_ChannelInfoMap[i].pLevels[j].GetBuffer();
						structure.item.lParam = i;

						item = m_ChannelTree->InsertItem(&structure);
						m_ChannelTree->SelectItem(item);

						hParentItem = m_ChannelTree->GetParentItem(item);
						pTemp = m_ChannelTree->GetItemText(hParentItem);
						m_pDChanSelector->SetItemID(id);
						m_pDChanSelector->SetItemVisible(false);
						m_pDChanSelector->SetItemCaption(m_ChannelTree->GetItemText(item));
						m_pDChanSelector->SetItemParentCaption(pTemp);
						m_pDChanSelector->SetHTREEITEMItem(item);
						m_pDChanSelector->SetHTREEITEMParent(hParentItem);
						m_pDChanSelector->SetItemConversion(m_ChannelInfo->m_ChannelInfoMap[i].sConversion);
						m_pDChanSelector->SetIdChan(m_ChannelInfo->m_ChannelInfoMap[i].idChan);
						m_pDChanSelector->InsertVectorItem();
						id++;

						++j;
					}

				}
			}
		}
	}
	m_pDChanSelector->SetTextString(m_ChannelTree);
}
//------------------------------------------------------------------------------
bool CChanSelector::FindItemInLevel( HTREEITEM item, CString sLevel, HTREEITEM &findItem )
{
	bool bFind(false);

	while(item)
	{
		if (m_ChannelTree->GetItemText(item) == sLevel)
		{
			findItem = item;
			bFind = true;
			break;
		}
		item = m_ChannelTree->GetNextItem(item, TVGN_NEXT);
	}

	return bFind;
}
//------------------------------------------------------------------------------
void CChanSelector::OnLButtonClk( UINT nFlags, CPoint point )
{
	if (!m_Press)
	{
		if (m_pOwner->OpenMenu)
			m_pOwner->CloseMenu();
		m_pOwner->SetParamAddRect(CRect(m_pDChanSelector->GetButRect().left,
										m_pDChanSelector->GetButRect().top,
										m_pDChanSelector->GetButRect().right + m_pDChanSelector->GetWidhtChs(),
										m_pDChanSelector->GetButRect().bottom + m_pDChanSelector->GetHeightChs() + 3));
		m_pDChanSelector->SetMenuButtonsState(true);
		m_pOwner->OpenMenu = true;
		m_Press = true;
		SetPress(true);
	}
	else
	{
		m_Press = false;
		m_pOwner->CloseMenu();
		m_pOwner->SetParamAddRect(CRect(0,0,0,0));
		SetPress(false);
	}
}
//------------------------------------------------------------------------------
void CChanSelector::OnPickItem( long index )
{
	int n = m_pDChanSelector->GetItemsCount();
	if (index == n) m_pDChanSelector->ButtonUp();
	else if (index == n + 1) m_pDChanSelector->ButtonDown();
	else {
	m_pDChanSelector->SetUnusedButtons(index);	
	NewLong(m_pDChanSelector->GetIndexCur()); 
	}
}
//------------------------------------------------------------------------------
void CChanSelector::SetFilterByMeasure(CString val)
{
	m_pDChanSelector->SetFilterMeasure(true);
	m_pDChanSelector->SetMeasureString(val);
}
//------------------------------------------------------------------------------
void CChanSelector::SetFilterByName(CString val)
{
	m_pDChanSelector->SetFilterName(true);
	m_pDChanSelector->SetNameString(val);
}
//------------------------------------------------------------------------------
void CChanSelector::SetFilterByIdChan(CString val)
{
	m_pDChanSelector->SetFilterIdChan(true);
	m_pDChanSelector->SetIdChanString(val);
}
//------------------------------------------------------------------------------