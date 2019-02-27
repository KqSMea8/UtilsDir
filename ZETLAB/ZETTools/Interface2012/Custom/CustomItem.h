//------------------------------------------------------------------------------
// CustomItem.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Custom\CustomElement.h"
#include "C:\ZETTools\Interface2012\Custom\Custom_ZETi12.h"
//------------------------------------------------------------------------------
typedef void (CCustom_ZETi12::*NewLongEvent)(long);
//------------------------------------------------------------------------------
class CCustomItem : public CCustomElement
{
public:
	CCustomItem(CCustomContainer *owner);
	virtual ~CCustomItem();

private:

protected:
	UINT_PTR m_StartItemID;
	CString m_StartItemName;

public:
	NewLongEvent m_FunctionEvent;
	virtual void NewLong(long val);

	UINT_PTR GetStartItemID() { return m_StartItemID; }
	void SetStartItemID(UINT_PTR val) { m_StartItemID = val; }

	CString GetStartItemName() { return m_StartItemName; }
	void SetStartItemName(CString val) { m_StartItemName = val; }

	virtual void OnPickItem(long index) {}
};
//------------------------------------------------------------------------------