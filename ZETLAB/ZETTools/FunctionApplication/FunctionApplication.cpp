//------------------------------------------------------------------------------
#include "StdAfx.h"
#include "TlHelp32.h"
#include "Psapi.h"

//#include <Intel\ipp\ipps.h>
#include <FunctionApplication\FunctionApplication.h>
//------------------------------------------------------------------------------
typedef struct
{
	std::vector<HWND>* pVector;	// ��������� �� ������ � HWND ���� ����
	bool bVisible;				// ���� ������ ������� ����
	bool bInvisible;			// ���� ������ ��������� ����
} FindWindowStruct;
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// �������� ���� �� ������� �� ������� �����������.
// hWin - ����������� ����;
// TimeOut - ������������ ����� �������� �������, ����.
// 
// ������������ ��������:
// -1 - ������ ���� ���;
//	0 - ���� �������� �� �������;
//	1 - ���� �� �������� �� ������� (��� ������� �����?).
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
// ����� ���� �� ��� ���������.
// title - ����� ���������;
// bFullOverlap = true => title - ��� ������ ���������;
// bFullOverlap = false => title - ��� ����� ���������.
//
HWND FindWindowFromTitle(const CString & title, bool bFullOverlap)
{
	HWND ret(0), hWnd(0);
	if (bFullOverlap)
		ret = ::FindWindowW(0, title);
	else
	{
		hWnd= ::GetTopWindow(0); //����� ������ ����
		while(hWnd)
		{
			WCHAR stroka[200];
			if ( (GetWindowText(hWnd, stroka, 200) > 0) &&
				 (CString(stroka).Find(title) != -1) )
			{
				ret = hWnd;
				break;
			}
			hWnd = ::GetNextWindow(hWnd, GW_HWNDNEXT);	// ��������� ����
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
// ����� �������� ���� � �������.
// ������� ����, ��� ���� � �������� ��� ��������.
// ���� ����� ���������, ��, ������ ������� ���� ������� �������.
//
// ������������ ��������:
// ���� ������ ������� = 1, �� ��������� HWND ������������� ����.
// ����� HWND ���������� ����, ��� 0, ���� ����� ���������.
//
HWND FindHwndInVector(std::vector<HWND> *pvect)
{
	HWND ret(0);
	if (pvect != nullptr)
	{
		UINT size = pvect->size();
		if (size == 1)
		{// ���� ����
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
			{// ���� ����
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
// ����� �������� ���� �� ID ������.
// ����������� �������� ���� � ������� FindHwndInVector.
// threadID - ID ������;
// pVector - ������ ��� ���� ������������ ����, ��������� ���� �������,
//			� ������������ � ��������� �������;
// bVisible - �����/�� ����� ����, ������� �������
// bInvisible - �����/�� ����� ����, ������� �� �������.
//
// ������������ ��������:
// HWND ���������� ����, ��� 0, ���� ����� ���������.
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
		HWND hWnd = ::GetTopWindow(0); //����� ������ ����
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
			hWnd = ::GetNextWindow(hWnd, GW_HWNDNEXT);	// ��������� ����
		}
		ret = FindHwndInVector(pvect);
	}
	return ret;
}
//------------------------------------------------------------------------------
// ����� �������� ���� �� ID ��������.
// ����������� �������� ���� � ������� FindHwndInVector.
// ProcessID - ID ��������;
// pVector - ������ ��� ���� ������������ ����, ��������� ���� ���������.
//
// ������������ ��������:
// HWND ���������� ����, ��� 0, ���� ����� ���������.
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
		HWND hWnd = ::GetTopWindow(0); //����� ������ ����
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
			hWnd = ::GetNextWindow(hWnd, GW_HWNDNEXT);	// ��������� ����
		}
		ret = FindHwndInVector(pvect);
	}
	return ret;
}
//------------------------------------------------------------------------------
// �������� ����-�� ������� � �������� ������ exe-��
// name - ��� exe-����� (����� ��� ".exe");
// pParentID - ��������� ��� ID �������� ���������� ��������.
//
// ������������ ��������:
//	0 - ������� �� ������;
//	!0 - ID ���������� ��������.
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
// ����������� ID ��������, ��������� ��������� �����
// hWnd - HWND ������ ����;
// pParentProcessID - ��������� ��� ID �������� ���������� ��������.
//
// ������������ ��������:
//	0 - ������� �� ������;
//	!0 - ID ���������� ��������.
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
// ���������� ���������� ��������� � �������� ������ exe-��
// ������������ ��������:
//	0 - ������� �� ������;
//	!0 - ���������� ��������� � �������� ������.
int NumberOfProcess(
	const CString &name,		// ��� exe-����� (����� ��� ".exe")
	DWORD *ProcessID)	// ID ���������� ����������� ��������
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
// ����� ���� �������� �� �������������� ������ ��������.
// ������� ��������� ������
BOOL CALLBACK CallBackFindAllWin(HWND hwnd, LPARAM lParam)
{	// ��������� ��� ����
	wchar_t text[200] = { 0 };
	std::set<HWND> *pset = (std::set<HWND>*)lParam;
	if (IsWindowVisible(hwnd) != FALSE)
	{
		if (GetWindowTextW(hwnd, text, 200) > 0)
		{// ���� � ����������
			pset->insert(hwnd);
		}
	}
	return TRUE;
}
// ����� ���� ���� ����������
void FindAllWindowFromThreadID(
	const DWORD threadID,			// ������������� ��������
	std::set<HWND> & pset)			// HWND ��������� ����
{
	EnumThreadWindows(threadID,	CallBackFindAllWin, (LPARAM)&pset);
}
//------------------------------------------------------------------------------
// ������� PostMessage ���� ����� �� set
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
// ��������� ��������� ���� �����
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