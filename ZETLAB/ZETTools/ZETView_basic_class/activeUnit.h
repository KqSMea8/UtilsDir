//------------------------------------------------------------------------------
#pragma once
#include "afxwin.h"
#include "afxdisp.h"
#include <Dialog_ZET\CDUnit.h>

#define UNIT_ID 1001
static long unitCounter;
//------------------------------------------------------------------------------
//структура, описывающая связь программы с Юнитом
struct InsideUnitXS
{
//	LPOLESTR clsidStr;			// нигде не использовался
//	CLSID clsidActiveX;			// CLSID Unit-a (он не меняется)
	CWnd* ActiveXcwnd;			// экз. класса CDUnit
	CWnd* parent;				// экз. класса ZET-компонента - обёртки над программой
	CString associatedProgName;	// имя запускаемой программы, без exe
//	long unit_id;				// ID экз. класса CDUnit
};
//------------------------------------------------------------------------------
class CUnit : public CDUnit
{
private:
	bool m_bCreate;
public:
	CUnit();
	virtual ~CUnit();
	InsideUnitXS InsideUnit;
	bool cwndValidate(CWnd* cwnd);	//проверка актуальности CWbnd*
	void SetUnitInterface(CString associatedProg, CWnd* parentCWnd);
	long startAll(WPARAM wParam, LPARAM lParam);
};
//------------------------------------------------------------------------------