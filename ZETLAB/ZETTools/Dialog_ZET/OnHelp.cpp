//------------------------------------------------------------------------------
#include "stdafx.h"
#include "OnHelp.h"
#include <ZETFile\include\ZETFile.h>
#include <Dialog_ZET\Translate.h>
//------------------------------------------------------------------------------
COnHelp::COnHelp()
	: m_sExeName(L"")
	, m_sSystemRoot(L"")
	, m_sFileHelp(L"")
{
	m_piHelp.hProcess = NULL;
}
//------------------------------------------------------------------------------
COnHelp::~COnHelp()
{
	if (m_piHelp.hProcess)		// закрываю окно справки
	{
		TerminateProcess(m_piHelp.hProcess, 0);
		CloseHandle(m_piHelp.hProcess);
	}
}
//------------------------------------------------------------------------------
// Задать надо либо указатель на ZetPath, либо на родителя.
// В функции нужен ZetPath. Если у родителя его нет, но его создадут
// 
bool COnHelp::Create(CDZetPath *pZetPath, CWnd *pParent)
{
	bool ret = (pParent != NULL) || (pZetPath != NULL);
	if (ret)
	{
		CString sFolderHelp(L"");
		CDZetPath *pzPath = pZetPath;
		bool bPathInside = pzPath == NULL;

		if (bPathInside)
		{
			pzPath = new CDZetPath();
			ret = pzPath->Create(L"", WS_CHILD, CRect(0, 0, 0, 0), pParent, 1701)
				!= FALSE;
		}

		BSTR bres = _T("DirHelp");
		sFolderHelp = pzPath->ZetPathW(&bres);

		if (bPathInside)
		{ 
			delete pzPath;
			pzPath = nullptr;
		}

		ret = Create(sFolderHelp);
	}
	return ret;
}
//------------------------------------------------------------------------------
bool COnHelp::Create(const CString sFolderHelp,CString chmName)
{
	if (!chmName.IsEmpty())
	{
		m_sExeName = chmName;
		m_sFileHelp = sFolderHelp + m_sExeName + L".htm";
	}
	else
	{
		m_sExeName = GetCommandLineW();

		int n = m_sExeName.Find(L".exe");
		if (n != -1)
			m_sExeName.Delete(n, m_sExeName.GetLength() - n);
		n = m_sExeName.ReverseFind(L'\\');
		if (n != -1)
			m_sExeName.Delete(0, n + 1);


		m_sFileHelp = sFolderHelp + m_sExeName + L".chm";
	}
		
	size_t size;
	if ((_wgetenv_s(&size, NULL, 0, L"SystemRoot") == 0) && (size > 0))
	{// Размер получен, вычитываю саму строку
		wchar_t *text = new wchar_t[size];
		if (_wgetenv_s(&size, text, size, L"SystemRoot") == 0)
			m_sSystemRoot = (CString)text + L"\\";
		delete[] text;
		text = nullptr;
	}		
	// Вначале хелп по имени программы
	bool bNotRu(false);
	CString sLang = GetLanguageDst();
	int m = sLang.Find(L"-");
	if (m != -1)
	{
		sLang.SetAt(m, 0);
		sLang.ReleaseBuffer();
		bNotRu = sLang != L"ru";
	}
	if (chmName.IsEmpty())
	{
		m_sFileHelp = sFolderHelp + m_sExeName;
		if (bNotRu)
			m_sFileHelp += L"_" + sLang;
		m_sFileHelp += L".chm";

		if (1 != zfFileExists(m_sFileHelp))
		{// Тогда хелп общий по zetlab
			m_sFileHelp = sFolderHelp + L"help_zetlab";
			if (bNotRu)
				m_sFileHelp += L"_" + sLang;
			m_sFileHelp += L".chm";
			if (1 == zfFileExists(m_sFileHelp))
				m_sFileHelp += L"::" + m_sExeName + L".htm";
			else
				// никакого файла нет
				m_sFileHelp.Empty();
		}
	}
	return !m_sFileHelp.IsEmpty();
}
//------------------------------------------------------------------------------
bool COnHelp::OnHelp()
{
	bool ret = !m_sFileHelp.IsEmpty() && !m_sSystemRoot.IsEmpty();
	if (ret)
	{	// если файл справки был найден
		STARTUPINFO si = { sizeof(si) };
		CString strComLine = m_sSystemRoot + L"hh.exe " + m_sFileHelp;
		// создавать переменную strComLine ОБЯЗАТЕЛЬНО (см. Рихтера)!!!
		ret = CreateProcessW(NULL, (LPWSTR)strComLine.GetString(),
			NULL, NULL, false, 0, NULL, NULL, &si, &m_piHelp) != FALSE;
			CloseHandle(m_piHelp.hThread);
	}
	return ret;
}
//------------------------------------------------------------------------------