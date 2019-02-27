//------------------------------------------------------------------------------
#include "StdAfx.h"
#include "activeunit.h"
//------------------------------------------------------------------------------
CUnit::CUnit() : CDUnit()
{// Вызывать Create пока нет необходимости
	InsideUnit.ActiveXcwnd = NULL;
	InsideUnit.parent = NULL;
	m_bCreate = false;
}
//------------------------------------------------------------------------------
CUnit::~CUnit()
{
}
//------------------------------------------------------------------------------
void CUnit::SetUnitInterface(CString associatedProg, CWnd* parentCWnd)
{// Пока просто запоминаем имя программы и его будущего родителя
	InsideUnit.associatedProgName = associatedProg;
	InsideUnit.parent = parentCWnd;
}
//------------------------------------------------------------------------------
bool CUnit::cwndValidate(CWnd* cwnd)
{// Проверка
	bool result;
	if (cwnd)
		result = cwnd->GetControlUnknown() != NULL;
	else
		result = false;
	return result;
}
//------------------------------------------------------------------------------
long CUnit::startAll(WPARAM wParam, LPARAM lParam)
{//			Функция вызывается только 2 раза:
// - в начале работы SCADA-проекта (wParam = 1) и
// - при его завершении (wParam = 0).
//			Возвращаемое значение:
// 1 - программа-сервер запущена через Unit,
// 0 - программа-сервер не запустилась при старте или выгружена при завершении
	long ret = 0;
	if (wParam == 1)
	{	// Вначале надо создать себя
		if (!m_bCreate)
			m_bCreate = Create(L"", WS_CHILD, CRect(0, 0, 0, 0), InsideUnit.parent, UNIT_ID + unitCounter) != FALSE;
		if (m_bCreate)
		{	// Теперь запускаем программу-сервер через Unit
			if (Activate(InsideUnit.associatedProgName) == 0)
				ret = 1;
			InsideUnit.ActiveXcwnd = this;
		}
		else		
			InsideUnit.ActiveXcwnd = NULL;
		++unitCounter;	// не важно, что вернул Create
	}
	else if (wParam == 0)
	{	// закрытие программы-сервер
		DisActivate();
		--unitCounter;
	}
	return ret;
}
//------------------------------------------------------------------------------