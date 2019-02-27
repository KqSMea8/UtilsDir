//------------------------------------------------------------------------------
//	Модуль CThreadWait.h
//	Зеленоград, ЗАО "ЭТМС", Савостин Ю.М.
//------------------------------------------------------------------------------
//   Класс ждущего потока.
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <ZetThread\CustomZetThread.h>
//------------------------------------------------------------------------------
struct ParamThreadWait
{
	HANDLE pHandle[2];
};
//------------------------------------------------------------------------------
class CThreadWait : public CCustomZetThread
{
private:
	bool m_bObjectInternal;
	ParamThreadWait m_ptw;

protected:

public:
	CThreadWait();
	virtual bool Create( CString nameThread, HWND hwnd,
		FunctionThread function, PVOID paramFunction = NULL);
	virtual ~CThreadWait();

	// члены класса
	// методы свойств
	// методы класса

	virtual long End();				// асинхронная команда потоку "Конец"
	virtual long EndAndWait();		// синхронная команда потоку "Конец"
	virtual long Stop();			// асинхронная команда потоку "Стоп"
	virtual long StopAndWait();		// синхронная команда потоку "Стоп"

	// Однократный запуск
	void SetSynchro();

	// Задание внешнего синхронизируещего объекта ядра,
	// который должен быть с автосбросом
	void SetExternalSynchroObject(HANDLE handle);
};
//------------------------------------------------------------------------------