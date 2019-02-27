//------------------------------------------------------------------------------
#include "StdAfx.h"
#include "TlHelp32.h"
#include "Psapi.h"

//#include <Intel\ipp\ipps.h>
#include <FunctionApplication\FunctionApplication.h>
//------------------------------------------------------------------------------
typedef struct
{
	std::vector<HWND>* pVector;	// указатель на вектор с HWND всех окон
	bool bVisible;				// флаг поиска видимых окон
	bool bInvisible;			// флаг поиска невидимых окон
} FindWindowStruct;
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Проверка окна по отклику на предмет активнорсти.
// hWin - проверяемое окно;
// TimeOut - максимальное время ожидания отклика, мсек.
// 
// Возвращаемое значение:
// -1 - такого окна нет;
//	0 - окно отвечает на запросы;
//	1 - окно не отвечает на запросы (его процесс завис?).
//
long IsProcessHung(HWND hWin, UINT TimeOut)
{
	long ret(-1);
	DWORD_PTR res(0);
	if ( (hWin != 0) && (IsWindow(hWin) != FALSE))
	{
		if ( ::SendMessageTimeout(hWin, WM_NULL, 0, 0,
		//	SMTO_ABORTIFHUNG, TimeOut, &res) )
			SMTO_NORMAL, TimeOut, &res) )
			ret = 0;
		else
			ret = 1;
	}
	return ret;
}
//------------------------------------------------------------------------------
// Поиск окна по его заголовку.
// title - текст заголовка;
// bFullOverlap = true => title - это полный заголовок;
// bFullOverlap = false => title - это часть заголовка.
//
HWND FindWindowFromTitle(const CString & title, bool bFullOverlap)
{
	HWND ret(0), hWnd(0);
	if (bFullOverlap)
		ret = ::FindWindowW(0, title);
	else
	{
		hWnd= ::GetTopWindow(0); //берем первое окно
		while(hWnd)
		{
			WCHAR stroka[200];
			if ( (GetWindowText(hWnd, stroka, 200) > 0) &&
				 (CString(stroka).Find(title) != -1) )
			{
				ret = hWnd;
				break;
			}
			hWnd = ::GetNextWindow(hWnd, GW_HWNDNEXT);	// следующее окно
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
// Поиск главного окна в векторе.
// Главное окно, это окно у которого нет родителя.
// Если таких несколько, то, первое видимое окно считаем главным.
//
// Возвращаемое значение:
// Если размер вертора = 1, то возвращаю HWND единственного окна.
// Иначе HWND найденного окна, или 0, если поиск неудачный.
//
HWND FindHwndInVector(std::vector<HWND> *pvect)
{
	HWND ret(0);
	if (pvect != nullptr)
	{
		UINT size = pvect->size();
		if (size == 1)
		{// окно одно
			ret = *pvect->cbegin();
		}
		else if (size > 1)
		{
			std::vector<HWND> vect;
			CString name;
			name.Preallocate(MAX_PATH);
			for (auto it : *pvect)
			{
				if (::GetParent(it) == NULL)
				{
					::GetWindowTextW(it, name.GetBuffer(), MAX_PATH);
					name.ReleaseBuffer();
					if ((name.Find(L"MSCTFIME") == -1) &&
						(name.Find(L"Default IME") == -1))
						vect.push_back(it);
				}
			}

			size = vect.size();
			if (size == 1)
			{// окно одно
				ret = *vect.cbegin();
			}
			else if (size > 1)
			{
				for (auto it : vect)
				{
					if (IsWindowVisible(it) != FALSE)
					{
						ret = it;
						break;
					}
				}
			}
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
// Поиск главного окна по ID потока.
// Определение гдавного окна в функции FindHwndInVector.
// threadID - ID потока;
// pVector - вектор для всех дескрипторов окон, созданных этим потоком,
//			в соответствии с заданными флагами;
// bVisible - нужны/не нужны окна, которые видимые
// bInvisible - нужны/не нужны окна, которые не видимые.
//
// Возвращаемое значение:
// HWND найденного окна, или 0, если поиск неудачный.
//
HWND FindWindowFromThreadID(const DWORD threadID,
	std::vector<HWND> * pVector,
	const bool bVisible, const bool bInvisible)
{
	HWND ret(0);
	if ((threadID > 0) &&
		(bVisible || bInvisible))
	{
		std::vector<HWND> vect;
		std::vector<HWND> * pvect = (pVector != nullptr) ? pVector : &vect;

		CString name;
		name.Preallocate(MAX_PATH);
		HWND hWnd = ::GetTopWindow(0); //берем первое окно
		while (hWnd != NULL)
		{
			DWORD procID(0);
			if (GetWindowThreadProcessId(hWnd, &procID) == threadID)
			{
				::GetWindowTextW(hWnd, name.GetBuffer(), MAX_PATH);
				name.ReleaseBuffer();
				if (!name.IsEmpty() &&
					(name.Find(L"MSCTFIME") == -1) &&
					(name.Find(L"Default IME") == -1))
				{
					bool bWinVis = IsWindowVisible(hWnd) != FALSE;
					bool bCount(false);
					if (bVisible)
						bCount |= bWinVis;
					if (bInvisible)
						bCount |= !bWinVis;

					if (bCount)
						pvect->push_back(hWnd);
				}
			}
			hWnd = ::GetNextWindow(hWnd, GW_HWNDNEXT);	// следующее окно
		}
		ret = FindHwndInVector(pvect);
	}
	return ret;
}
//------------------------------------------------------------------------------
// Поиск главного окна по ID процесса.
// Определение гдавного окна в функции FindHwndInVector.
// ProcessID - ID процесса;
// pVector - вектор для всех дескрипторов окон, созданных этим процессом.
//
// Возвращаемое значение:
// HWND найденного окна, или 0, если поиск неудачный.
//
HWND FindWindowFromProcessID(DWORD ProcessID, std::vector<HWND> * pVector)
{
	HWND ret(0);
	if (ProcessID > 0)
	{
		std::vector<HWND> vect;
		std::vector<HWND> * pvect = (pVector != nullptr) ? pVector : &vect;

		CString name;
		name.Preallocate(MAX_PATH);
		HWND hWnd = ::GetTopWindow(0); //берем первое окно
		while (hWnd != NULL)
		{
			DWORD procID(0);
			GetWindowThreadProcessId(hWnd, &procID);
			if (procID == ProcessID)
			{
				::GetWindowTextW(hWnd, name.GetBuffer(), MAX_PATH);
				name.ReleaseBuffer();
				if (!name.IsEmpty() &&
					(name.Find(L"MSCTFIME") == -1) &&
					(name.Find(L"Default IME") == -1))
				{
					pvect->push_back(hWnd);
				}
			}
			hWnd = ::GetNextWindow(hWnd, GW_HWNDNEXT);	// следующее окно
		}
		ret = FindHwndInVector(pvect);
	}
	return ret;
}
//------------------------------------------------------------------------------
// Проверка есть-ли процесс с заданным именем exe-ки
// name - имя exe-файла (можно без ".exe");
// pParentID - указатель для ID родителя найденного процесса.
//
// Возвращаемое значение:
//	0 - процесс не найден;
//	!0 - ID найденного процесса.
//
DWORD IsProcess(const CString & name, DWORD *pParentID)
{
	DWORD ret(0);
	HANDLE m_hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
	if (m_hSnapshot != INVALID_HANDLE_VALUE)
	{
		CString str = name;
		str.MakeLower();
		if (str.Find(L".exe") == -1)
			str += L".exe";
		PROCESSENTRY32 pe = { sizeof(pe) };

		for (BOOL bOk = Process32First(m_hSnapshot, &pe); bOk;
			bOk = Process32Next(m_hSnapshot, &pe))
		{
			if (((CString)pe.szExeFile).MakeLower() == str)
			{
				ret = pe.th32ProcessID;
				if (pParentID)
					*pParentID = pe.th32ParentProcessID;
				break;
			}
		}
		CloseHandle(m_hSnapshot);
	}
	return ret;
}
//------------------------------------------------------------------------------
// Определение ID процесса, владеющим некоторым окном
// hWnd - HWND этотго окна;
// pParentProcessID - указатель для ID родителя найденного процесса.
//
// Возвращаемое значение:
//	0 - процесс не найден;
//	!0 - ID найденного процесса.
//
DWORD IsProcess(_In_ HWND hWnd, _Inout_opt_ DWORD *pParentProcessID /*= nullptr*/)
{
	DWORD ret(NULL);
	DWORD procid(NULL);

	GetWindowThreadProcessId(hWnd, &procid);
	if (procid)
	{
		HANDLE m_hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
		if (m_hSnapshot != INVALID_HANDLE_VALUE)
		{
			PROCESSENTRY32 pe = { sizeof(pe) };

			for (BOOL bOk = Process32First(m_hSnapshot, &pe); bOk;
				 bOk = Process32Next(m_hSnapshot, &pe))
			{
				if (pe.th32ProcessID == procid)
				{
					ret = pe.th32ProcessID;
					if (pParentProcessID)
						*pParentProcessID = pe.th32ParentProcessID;
					
					break;
				}
			}
			CloseHandle(m_hSnapshot);
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
// Нахождение количество процессов с заданным именем exe-ки
// Возвращаемое значение:
//	0 - процесс не найден;
//	!0 - количество процессов с заданным именем.
int NumberOfProcess(
	const CString &name,		// имя exe-файла (можно без ".exe")
	DWORD *ProcessID)	// ID последнего запущенного процесса
{
	int count(0);
	HANDLE m_hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
	if (m_hSnapshot != INVALID_HANDLE_VALUE)
	{
		CString str = name;
		str.MakeLower();
		if (str.Find(L".exe") == -1)
			str += L".exe";
		PROCESSENTRY32 pe = { sizeof(pe) };

		for (BOOL bOk = Process32First(m_hSnapshot, &pe); bOk;
			bOk = Process32Next(m_hSnapshot, &pe))
		{
			if (((CString)pe.szExeFile).MakeLower() == str)
			{
				count++;
				if (ProcessID)
					*ProcessID = pe.th32ProcessID;
			}
		}
		CloseHandle(m_hSnapshot);
	}
	return count;
}
//------------------------------------------------------------------------------
// Поиск окон процесса по идентификатору потока процесса.
// Функция обратного вызова
BOOL CALLBACK CallBackFindAllWin(HWND hwnd, LPARAM lParam)
{	// перебираю все окна
	wchar_t text[200] = { 0 };
	std::set<HWND> *pset = (std::set<HWND>*)lParam;
	if (IsWindowVisible(hwnd) != FALSE)
	{
		if (GetWindowTextW(hwnd, text, 200) > 0)
		{// окно с заголовком
			pset->insert(hwnd);
		}
	}
	return TRUE;
}
// Поиск всех окон приложения
void FindAllWindowFromThreadID(
	const DWORD threadID,			// идентификатор процесса
	std::set<HWND> & pset)			// HWND найденных окон
{
	EnumThreadWindows(threadID,	CallBackFindAllWin, (LPARAM)&pset);
}
//------------------------------------------------------------------------------
// Посылка PostMessage всем окнам из set
void PostMessageForAllWindows(const std::set<HWND> & set, const UINT msg,
	const WPARAM wParam, const LPARAM lParam)
{
	if (!set.empty())
	{
		for (auto hwnd : set)
			::PostMessageW(hwnd, msg, wParam, lParam);
	}
}
//------------------------------------------------------------------------------
// Установки видимости всем окнам
void ShowAllWindow(const std::set<HWND> & set, const int nShow)
{
	if (!set.empty())
	{
		for (auto hwnd : set)
		{
			if (IsWindowVisible(hwnd) != FALSE)
				::ShowWindow(hwnd, nShow);
		}
	}
}
//------------------------------------------------------------------------------
CString GetProgramNameFromHWND(_In_ HWND hWnd)
{
	HMODULE* lphModule(nullptr);
	wchar_t FileName[1024] = {0};
	DWORD procid(0);
	DWORD modulesize(0);
	BOOL bInheritHandle(false);

	if (hWnd && (IsWindow(hWnd) != FALSE))
	{
		GetWindowThreadProcessId(hWnd, &procid);
		return GetProgramNameFromProcessID(procid);
	}

	return CString();
}
//------------------------------------------------------------------------------
CString GetProgramNameFromProcessID(_In_ DWORD ProcessID)
{
	HMODULE* lphModule(nullptr);
	wchar_t FileName[MAX_PATH * 10] = { 0 };
	DWORD modulesize(0);
	HANDLE process = OpenProcess(PROCESS_ALL_ACCESS |
								 PROCESS_QUERY_INFORMATION, false, ProcessID);
	if (process != NULL)
	{
		lphModule = new HMODULE[1];
		if (EnumProcessModules(process, lphModule, (sizeof(HMODULE)), &modulesize) != 0)
		{
			GetModuleBaseName(process, lphModule[0], FileName, MAX_PATH * 10);
			CloseHandle(process);
			delete[]lphModule;
			lphModule = nullptr;
			return FileName;
		}
		else
			CloseHandle(process);
		delete[]lphModule;
		lphModule = nullptr;
	}

	return CString();
}
//------------------------------------------------------------------------------