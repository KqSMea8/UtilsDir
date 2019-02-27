// (C) Ильназ Низаметдинов, ЗАО "ЭТМС"
// Все права защищены.
//
// Класс для предотвращения запуска нескольких копий приложения.
// Версия 1.2.1 @ 2012.08.30
//
// Использование:
//	- определить директиву SINGLE_INSTANCE_UUID перед включением этого файла
//		#define SINGLE_INSTANCE_UUID	_T("uuid")
//	  где uuid - уникальный идентификатор приложения
//
//	- в заголовочном файле приложения (обычно [имя_проекта].h):
//		AVOID_MULTI_INSTANCE()
//
//	- в функцию InitInstance(), после CWinApp::InitInstance(); :
//		HandleOtherInstances();
//
//	- в заголовочном файле диалога ([имя_проектаDlg].h):
//		DECLARE_INSTANCE()
//
//	- в карту сообщений диалога:
//		ON_OTHER_INSTANCE()

#pragma once
#ifndef SINGLE_INSTANCE_H
#define SINGLE_INSTANCE_H

#ifndef SINGLE_INSTANCE_UUID
//  Не объявлен уникальный идентификатор приложения
#   error Not announced a unique application ID
#endif

#include <windows.h>

class CSingleInstance
{
public:
    CSingleInstance() 
		: m_message(::RegisterWindowMessage( SINGLE_INSTANCE_UUID ))
        , m_hMutex (INVALID_HANDLE_VALUE)
		, m_hWnd   (NULL)
        
    {   
    };

    ~CSingleInstance()
    {
        if (INVALID_HANDLE_VALUE != m_hMutex)
            ::CloseHandle( m_hMutex );
    };

    const BOOL createLock( const TCHAR* pszMutexName )
    {
        // Set explicit NULL security attributes to get it working between user sessions
        PSECURITY_DESCRIPTOR psd = (PSECURITY_DESCRIPTOR) LocalAlloc( LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH );
		if ((psd != nullptr) && 
			(InitializeSecurityDescriptor( psd, SECURITY_DESCRIPTOR_REVISION ) != FALSE) &&
#pragma warning(suppress: 6248)
			(SetSecurityDescriptorDacl(psd, TRUE, NULL, FALSE) != FALSE))
		{
			SECURITY_ATTRIBUTES sa;
			SecureZeroMemory(&sa, sizeof(sa));
			sa.nLength = sizeof(sa);
			sa.lpSecurityDescriptor = psd;
			sa.bInheritHandle = FALSE;

			m_hMutex = ::CreateMutex(&sa, FALSE, pszMutexName);
		}
		else
			m_hMutex = INVALID_HANDLE_VALUE;

        LocalFree(psd);

        if (ERROR_ALREADY_EXISTS == GetLastError())
        {
            // Find previous instance
            ::EnumWindows( findPreviousInstance, reinterpret_cast< LPARAM >(this) );

            return FALSE;
        }

        return TRUE;
    };

private:
    static BOOL CALLBACK findPreviousInstance( HWND hWnd, LPARAM lParam )
    {
        DWORD_PTR result;

		CSingleInstance* pThis = reinterpret_cast<CSingleInstance*>(lParam);
		UINT message = pThis->m_message;

		if (::SendMessageTimeout(hWnd, message, 0, 0, SMTO_BLOCK, 200, &result) &&
                                  message == result)
        {	// we've got an answer

            ::SetForegroundWindow( hWnd );
			::ShowWindow(hWnd, SW_SHOW);
			if (::IsIconic(hWnd))
				::ShowWindow(hWnd, SW_RESTORE);

			pThis->m_hWnd = hWnd;

            return FALSE; // break
        }

        return TRUE; // continue
    };

public:
    UINT   m_message;
	HWND   m_hWnd;

private:
    HANDLE m_hMutex;
};

#define AVOID_MULTI_INSTANCE() \
    public:CSingleInstance m_singleInstance;

#define HandleOtherInstances(); \
    if (!m_singleInstance.createLock( SINGLE_INSTANCE_UUID )) return FALSE;

#define CheckOtherInstances()  \
	m_singleInstance.createLock( SINGLE_INSTANCE_UUID )

#define OtherInstanceHwnd()  \
	m_singleInstance.m_hWnd

#define DECLARE_INSTANCE() \
    LRESULT __OnSingleInstanceMessage__( WPARAM, LPARAM ) { return theApp.m_singleInstance.m_message; };

#define ON_OTHER_INSTANCE() \
    ON_REGISTERED_MESSAGE( theApp.m_singleInstance.m_message, __OnSingleInstanceMessage__ )

#endif /* SINGLE_INSTANCE_H */
