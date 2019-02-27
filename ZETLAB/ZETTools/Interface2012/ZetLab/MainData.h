//------------------------------------------------------------------------------
// MainParam.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Custom\CustomContainer.h"
#include "C:\ZETTools\Interface2012\Custom\CustomMain.h"
#include "C:\ZETTools\Interface2012\Draw\DMainData.h"
//------------------------------------------------------------------------------
class CMainData : public CCustomContainer
{
public:
	CMainData(CCustomMain *owner, CRect rect);//, double *pdTime);
	virtual ~CMainData();
	CRect m_rect;
private:
	CDMainData *m_pDData;
protected:

public:
	CString GetTitle() { return m_pDData->GetTitle(); }
	void SetTitle(CString val) { m_pDData->SetTitle(val); }
	virtual void OnDraw();
	virtual void Clear (void);
    virtual void SaveParameters(SimpleXML *pSimpleXML);
    virtual void LoadParameters(SimpleXML *pSimpleXML);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);

};
//------------------------------------------------------------------------------
