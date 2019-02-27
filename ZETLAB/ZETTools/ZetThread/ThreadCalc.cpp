//------------------------------------------------------------------------------
//	Модуль ThreadCalc.cpp
//	Версия 02.08.2012
//	Зеленоград, ЗАО "ЭТМС", Савостин Ю.М.
//------------------------------------------------------------------------------
#include "StdAfx.h"
#include "ThreadCalc.h"
//------------------------------------------------------------------------------
DWORD WINAPI Calc_Thread(PVOID pParam)
{// функция рабочего потока сервера
	// определяю адрес структуры общих параметров потока (см. класс-родитель)
	ZetThreadParam0 *par0 = (ZetThreadParam0*)pParam;
	
	while ( par0->FlagJob )
	{// цикл потока -----------------------------------------------------
		par0->FlagViewStop = false;
		// что-то делаю (вычисляю, ...)
		if (par0->Function)
		{// если функция обработки данных задана
			long kod = (*par0->Function)(par0->pParamFunction);
			if ( kod >= 0 )
			{// сообщаю о завершении обработки данных если не было ошибок
				if ( par0->hWndParent )
					::PostMessageW( par0->hWndParent, WM_END_CALC,
							(WPARAM)kod, 0 );		// код возврата
			}
			else
			{// функция вернула ошибку
				if ( par0->hWndParent )
					::PostMessageW(par0->hWndParent, WM_ERROR_THREAD,
							(WPARAM)kod,		// код ошибки
							(LPARAM)(LPCWSTR)par0->NameThread);
			}			
		}
		// всегда останавливаю поток ----------------------------------
		par0->FlagViewStop = true;
		SuspendThread(par0->hThread);	// последняя команда в цикле while
	}// while ( par0->FlagJob )
	return 0;
}
//------------------------------------------------------------------------------
CThreadCalc::CThreadCalc() : CCustomZetThread()
{
}
//------------------------------------------------------------------------------
CThreadCalc::~CThreadCalc()
{// Необходимо вначале синхронно остановить поток, а затем удалить массивы,
// иначе возможна ситуация, когда поток обратится к уже удалённым объектам
	SynchroFinishThread();
}
//------------------------------------------------------------------------------
bool CThreadCalc::Create( CString nameThread, HWND hwnd,
						FunctionThread function, PVOID paramFunction )
{
	bool ret = function &&	// адрес функции должен быть задан!
		CCustomZetThread::Create(nameThread, hwnd, function, paramFunction);
	if (ret)
		ret = CreateZetThread(&Calc_Thread);	// Создаю приостановленный поток
	return ret;
}
//------------------------------------------------------------------------------