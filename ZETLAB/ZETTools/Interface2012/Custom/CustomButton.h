//------------------------------------------------------------------------------
// CustomButton.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Custom\CustomElement.h"
#include "C:\ZETTools\Interface2012\Custom\Custom_ZETi12.h"
//------------------------------------------------------------------------------
typedef void (CCustom_ZETi12::*NotifyEvent)(void);
//------------------------------------------------------------------------------
class CCustomButton : public CCustomElement
{
public:
	CCustomButton(CCustomContainer *owner);
	virtual ~CCustomButton();

private:
	UINT_PTR ButtonID;
protected:

public:
	NotifyEvent m_FunctionEvent;
	virtual void OnLButtonClk(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);

	UINT_PTR GetButtonID() { return ButtonID; }
	void SetButtonID(UINT_PTR val) { ButtonID = val; }
};
//------------------------------------------------------------------------------