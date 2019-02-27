//------------------------------------------------------------------------------
// MainParam.h : ���� ���������
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Custom\CustomContainer.h"
#include "C:\ZETTools\Interface2012\Custom\CustomMain.h"
#include "C:\ZETTools\Interface2012\Draw\DMainParam.h"
//------------------------------------------------------------------------------
class CMainParam : public CCustomContainer
{
public:
	CMainParam(CCustomContainer *owner, CRect rect, int height);//, double *pdTime);
	virtual ~CMainParam();
	CRect m_rect;
	CRect m_addRect; //������ �������������� ������� ��� ���� ����������
private:
	CDMainParam *m_pDParam;
protected:

public:
	CString GetTitle() { return m_pDParam->GetTitle(); }
	void SetTitle(CString val) { m_pDParam->SetTitle(val); }
	virtual void OnDraw();
	virtual void Clear (void);
    virtual void SaveParameters(SimpleXML *pSimpleXML);
    virtual void LoadParameters(SimpleXML *pSimpleXML);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);

	virtual void SetParamAddRect(CRect rect);
	 // ������������� ������ ������������� ������ ���� ����������(����� ��� combobox � chanselector) 
};
//------------------------------------------------------------------------------
