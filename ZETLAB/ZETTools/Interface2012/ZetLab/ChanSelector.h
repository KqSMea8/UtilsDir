//------------------------------------------------------------------------------
// ChanSelector.h : ���� ���������

//
//��� ����������� ChanSelector ����������: 
//- �������� svn (ChannelInfo.h + ChannelInfo.cpp; ChanSelector.h + ChanSelector.cpp, DChanSelector.h + DChanSelector.cpp, Button + DButton);
//- �������� ����� � ������
//	- ���������� #include "C:\ZETTools\Interface2012\ZetLab\ChanSelector.h" ���� ��, ��� ������������ ������ ������ ���������
//	- CChanSelector *m_pChan; ������� ������ ������ � �������������
//	- m_pChan = new CChanSelector(m_pMainContainer, CRect(90, 150, 330, 190), m_psrv, CRect(0, 0, WIDTH, HEIGHT)); 
//m_pMainContainer - ������������ ���������
//	CRect(90, 150, 330, 190) - ����� ������������ ����� ChanSelector
//	m_psrv - �������� � ��� �� Dialog_ZET, �������� ��� ����
//	CRect(0, 0, WIDTH, HEIGHT) - ������� ������� ������������� ����������
//
//	��� ������: 
//- ������������� ������ ������� � ����������� ���������;
//- ����� ����������� � ������������� �������� �� ����������� ������� ������;
//- ��������� ���������� ����� ������ ��� ������� �� �������������� ����� ����;
//- ��� ������ ������ (��� ��������� ��������� ������, ������ ������ ��������� � ��������� ����� ������������������ �� ������ ����);
//
//��� �� �����������:
//- ��������� ���������, ���� ������ ��������� �� ��������� � ������� ������;
//- ����������� ���������� ���������� �������;
//- ����������� ������� ���������� ������ ��� ������� ���������. ���������� ������ ������� "Channel"

//------------------------------------------------------------------------------
#pragma once
#include "afxcmn.h"
#include "C:\ZETTools\Interface2012\Draw\DChanSelector.h"
#include "C:\ZETTools\ZETView_basic_class\CDSRV.h"
#include "C:\ZETTools\Interface2012\ZetLab\ChannelInfo.h"
#include "C:\ZETTools\Interface2012\Custom\CustomItem.h"
#include "C:\ZETTools\Interface2012\ZetLab\MainParam.h"
#include "C:\ZETTools\Interface2012\Custom\Dialog_ZETi12.h"
#include "C:\ZETTools\Interface2012\Custom\Custom_ZETi12.h"
// 
// CCriticalSection m_ChannelInfoCriticalSection;




//------------------------------------------------------------------------------
class CChanSelector : public CCustomItem
{
public:
	CChanSelector(CCustomContainer *owner, CRect rect, CDSRV* m_psrv, CRect ownerRect);
	virtual ~CChanSelector();

	CTreeCtrl* m_ChannelTree;
	CChannelInfo* m_ChannelInfo;
	CRect m_rect;

private:
	CDChanSelector *m_pDChanSelector;

public:
	
	double GetTimeAnimation() { return m_pDChanSelector->GetTimeAnimation();}
	void SetTimeAnimation(double val){m_pDChanSelector->SetTimeAnimation(val);}

	bool GetAnimation()	 { return m_pDChanSelector->GetAnimation(); }
	void SetAnimation(bool val) { m_pDChanSelector->SetAnimation(val); }

	CString GetImageFileName() { return m_pDChanSelector->GetImageFileName();}
	void SetImageFileName(CString str){m_pDChanSelector->SetImageFileName(str);}

	int GetBorderWidth() { return m_pDChanSelector->GetBorderWidth(); }
	void SetBorderWidth(int val) { m_pDChanSelector->SetBorderWidth(val); }

	CString GetCaption() { return m_pDChanSelector->GetCaption(); }
	void SetCaption(CString str) { m_pDChanSelector->SetCaption(str); }

	virtual void Clear (void);

	//CRect GetElementRect() {return m_pDChanSelector->GetElementRect();}
	//bool SetAutoTreePanel(CTreePanel *pTreePanel);
	// ������ ������
	//virtual void OnMouseWheel(UINT nFlags, CPoint point, short delta);
	//virtual void OnLButtonClk(UINT nFlags, CPoint point);
	//virtual void OnPickItem(long index);

	virtual void OnLButtonClk(UINT nFlags, CPoint point);

	void FillChannelTree();
	bool FindItemInLevel(HTREEITEM item, CString sLevel, HTREEITEM &findItem);
	virtual void OnPickItem(long index);

	long GetIndexCur() { return m_pDChanSelector->GetIndexCur(); }
	void SetIndexCur(long val) { m_pDChanSelector->SetIndexCur(val);}

	//�������� ��������� ������ ���� � ������ ����� ������ �������
	void SetNewTextString(long val);

	virtual void SetPress(bool val) { m_pDChanSelector->SetPress(val); }

	void SetFilterByMeasure(CString val);
	void SetFilterByName(CString val);
	void SetFilterByIdChan(CString val);
};
//------------------------------------------------------------------------------