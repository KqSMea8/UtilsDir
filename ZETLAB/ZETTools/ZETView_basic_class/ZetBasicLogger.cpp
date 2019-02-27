//------------------------------------------------------------------------------
#include "StdAfx.h"
#include "ZetBasicLogger.h"
int g_num(0);
//------------------------------------------------------------------------------
extern uint32_t gComponentNumber;
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CZetBasicLogger, CZetBasicOle)
	ON_MESSAGE(WM_USER_START_ALL, OnStartAll)
END_MESSAGE_MAP()
//------------------------------------------------------------------------------
CZetBasicLogger::CZetBasicLogger() : CZetBasicOle()
{
}
//------------------------------------------------------------------------------
CZetBasicLogger::~CZetBasicLogger()
{
	if (m_pLogger)
	{
		delete m_pLogger;
		m_pLogger = nullptr;
	}
}
//------------------------------------------------------------------------------
int CZetBasicLogger::OnCreate(LPCREATESTRUCT lpCreateStruct, CString progid)
{
	if (CZetBasicOle::OnCreate(lpCreateStruct, progid) == -1)
		return -1;

	// Идентификаторы структур данного компонента для записи в логгер
	// 1. ID процесса (по его имени)
	wchar_t name[256] = { 0 };
	long err = ztGetNameSelfExe(name, 256);
	CharLowerW(name);
	m_structWithAllID.device_id = ztCRC32(name);

	// 2. Номер процесса
	err = ztGetNumberSelfExe(&m_structWithAllID.process_id);

	// 3. ID компонента (по его имени)
	wcsncpy_s(name, 256, AfxGetAppName(), 255);
	CharLowerW(name);
	m_structWithAllID.module_id = ztCRC32(name);

	// 4. Номер компонента
	m_structWithAllID.channel_id = (uint16_t)gComponentNumber;
	
	return 0;
}
//------------------------------------------------------------------------------
bool CZetBasicLogger::CreateLogger()
{
	m_pLogger = new CZETLoggerClient();
	return m_pLogger != nullptr;
}
//------------------------------------------------------------------------------
BOOL CALLBACK CallBackEnumResNameProc(
	HMODULE hModule, LPCTSTR lpszType, LPTSTR lpszName, LONG_PTR lParam)
{
	BOOL ret(FALSE);
	std::vector<ZetHeader*> *descriptors = (std::vector<ZetHeader*> *)lParam;
	HRSRC myResource = ::FindResourceW(hModule, lpszName, lpszType);
	if (myResource)
	{
		HGLOBAL myResourceData = ::LoadResource(hModule, myResource);
		if (myResourceData)
		{
			ZetHeader * pzh = (ZetHeader*)::LockResource(myResourceData);
			if (pzh)
			{
				try
				{
					if (ztIsStructureValid(pzh) == 0)
					{
						descriptors->push_back(pzh);
						ret = TRUE;
					}
				}
				catch (...)
				{
				}
			}
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
void CZetBasicLogger::RegisterBinary()
{	// Читаем из ресурсов бинарники
	HINSTANCE hInstance = AfxGetInstanceHandle();
	EnumResourceNames(hInstance, MAKEINTRESOURCE(7777), CallBackEnumResNameProc, (LONG_PTR)&m_descriptors);

	// Регистрация бинарников
	for (auto & it : m_descriptors)
	{
		if (0 == ztIsStructureValid(it))
		{
			m_pLogger->saveStructureDefinition(it, m_structWithAllID);
		}
	}
}
//------------------------------------------------------------------------------
LRESULT CZetBasicLogger::OnStartAll(WPARAM wParam, LPARAM lParam)
{// вызывается при старте (wParam = 1) и финише проекта (wParam = 0)
	CZetBasicOle::OnStartAll(wParam, lParam);
	if (wParam == 1)
	{
		if (m_pLogger == nullptr)
		{
			if (CreateLogger())
				RegisterBinary();
		}
	}
	return 0;
}
//------------------------------------------------------------------------------