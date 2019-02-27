//-----------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
#ifdef ZETFILE_EXPORTS
#define ZETFILE_API __declspec(dllexport)
#else
#define ZETFILE_API __declspec(dllimport)
#endif
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <ZetErrorCodes.h>
#include "share.h"
//-----------------------------------------------------------------------------
#define NUMBER_BITS_IN_BYTE         8
#define NUMBER_BYTES_IN_KBYTE		1024
#define NUMBER_KBYTES_IN_MBYTE		1024
#define NUMBER_BYTES_IN_MBYTE		(NUMBER_KBYTES_IN_MBYTE * NUMBER_BYTES_IN_KBYTE)
#define NUMBER_MBYTES_IN_GBYTE		1024
#define NUMBER_KBYTES_IN_GBYTE		(NUMBER_MBYTES_IN_GBYTE * NUMBER_KBYTES_IN_MBYTE)
#define NUMBER_BYTES_IN_GBYTE		(NUMBER_KBYTES_IN_GBYTE * NUMBER_BYTES_IN_KBYTE)

#define MAX_SIZE_OF_STRING_IN_FILE	200
#define LOG_FILE_MAX_SIZE			1048576	// 1 ��
#define LOG_FILE_SIZE_FOR_DELETE	102400	// 100 ��
#define TIME_FOR_NEXT_PING_IN_SEC	5		// 5 ������
//-----------------------------------------------------------------------------
// �������� ������������� ����� � �������� char-������.
// ������������ �������� :
// ������������� �������� - ��� ������;
// 0 - ����� ���;
// 1 - ���� ����.
_Check_return_
long ZETFILE_API zfCheckDriveLetterA(_In_ const char letter);
// �������� ������������� ����� � �������� wchar-������.
// ������������ �������� :
// ������������� �������� - ��� ������;
// 0 - ����� ���;
// 1 - ���� ����.
_Check_return_
long ZETFILE_API zfCheckDriveLetterW(_In_ const wchar_t letter);

#ifdef UNICODE
#define zfCheckDriveLetter  zfCheckDriveLetterW
#else
#define zfCheckDriveLetter  zfCheckDriveLetterA
#endif
//-----------------------------------------------------------------------------
// ������� ping �������� �����, ��� �������� ������ char-�������.
// numPackage - ���-�� ���������� �������.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ��� ������� �� �������� ����� ��� ��� ��� ������������;
// <= numPackage - ���-�� ���������� �������;
// numPackage + 1 - ping �� ����, �.�. ��������� ���� ��� ������� � �������
_Check_return_
long ZETFILE_API zfPingA(_In_ LPCSTR lpIPorName, _In_ const BYTE numPackage = 4);
// ������� ping �������� �����, ��� �������� ������ wchar-�������.
// numPackage - ���-�� ���������� �������.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ��� ������� �� �������� ����� ��� ��� ��� ������������;
// <= numPackage - ���-�� ���������� �������;
// numPackage + 1 - ping �� ����, �.�. ��������� ���� ��� ������� � �������
_Check_return_
long ZETFILE_API zfPingW(_In_ LPCWSTR lpIPorName, _In_ const BYTE numPackage = 4);

#ifdef UNICODE
#define zfPing  zfPingW
#else
#define zfPing  zfPingA
#endif
//-----------------------------------------------------------------------------
// �������� �� ������������� �����, ��� �������� ������ char-�������.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ����� ���;
// 1 - ���� ����.
_Check_return_
long ZETFILE_API zfFileExistsA(_In_ LPCSTR lpFileName);

// �������� �� ������������������ , ��� �������� ������ wchar-�������..
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ����� ���;
// 1 - ���� ����.
_Check_return_
long ZETFILE_API zfFileExistsW(_In_ LPCWSTR lpFileName);

#ifdef UNICODE
#define zfFileExists  zfFileExistsW
#else
#define zfFileExists  zfFileExistsA
#endif
//-----------------------------------------------------------------------------
// ��������� ������� �����.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ������ ��������.
_Check_return_
long ZETFILE_API zfGetFileSizeInBytes(_In_ LPCWSTR lpFileName, _Out_ DWORD * pSize);
//-----------------------------------------------------------------------------
// �������� �� ������������� �����, �������� wchar-�������.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ����� ���;
// 1 - ����� ����.
_Check_return_
long ZETFILE_API zfDirectoryExists(_In_ LPCWSTR lpDirectoryName);
//-----------------------------------------------------------------------------
// C������� �������������� ����� (���� ������ ������������).
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ����� ������� ��� ��� ����������.
_Check_return_
long ZETFILE_API zfCreateNewDirectory(_In_ LPCWSTR lpDirectoryName);
//-----------------------------------------------------------------------------
// �������� ��������������� ���� �� �����.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ���� ������ ��� ��� ����������.
_Check_return_
long ZETFILE_API zfCreatePath(_In_ LPCWSTR lpFileName);
//-----------------------------------------------------------------------------
// �������� ����� ��� ������/������ ������.
// ���� ������, �� ������� �������� ����� �� �������������.
// ���� ������, �� ������� �������� ����, ���� ���, �� ���� ��������.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ���� ������.
//-----------------------------------------------
// ��� ������� (lpwsFileMode)
// r - ������ ������, ���� ������ ������������
// w - ������ ������, ������������ ���� ���������
// a - ���������� � �����, ����� ����� �� ����
// r+ - ������ � ������, ���� ������ ������������
// w+ - ������ � ������, ������������ ���� ���������
// a+ - ������ � ���������� � �����, ����� ����� �� ����
//
// ��� ��������� ��������� ������ � UNICODE-��������� ����������
// ����� ������� �������� ���� "ccs". ���������� ��������:
// ccs=UNICODE
// ccs=UTF-8
// ccs=UTF-16LE
// ��������: UTF-16BE - �� ��������������, ������� ����� ��� ������ = -118.
//
// ������ L"rt, ccs=UTF-8"
//-----------------------------------------------
// ��� ������ � �����: t - ���������, b - ��������
//-----------------------------------------------
// ��� ����� ���������� �� ������� ������������ ��������
_Check_return_
long ZETFILE_API zfOpenFileEx(_In_ LPCWSTR lpwsFileName, _In_ LPCWSTR lpwsFileMode, _In_ int iFlag, _Out_ FILE **ppFile);
// ����� ��� ����������
_Check_return_
long ZETFILE_API zfOpenFile(_In_ LPCWSTR lpwsFileName, _In_ LPCWSTR lpwsFileMode, _Out_ FILE **ppFile);
// ����� � �����������
_Check_return_
long ZETFILE_API zfOpenFileDeny(_In_ LPCWSTR lpwsFileName, _In_ LPCWSTR lpwsFileMode, _Out_ FILE **ppFile);

//-----------------------------------------------------------------------------
// �������� ��������� �����.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ���� ������.
long ZETFILE_API zfCloseFile(_In_ FILE *pFile);
//-----------------------------------------------------------------------------
// ���������� char-������ � ����.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ����� ������� � ����.
_Check_return_
long ZETFILE_API zfAddTextToFileA(_In_ LPCWSTR lpwsFileName, _In_ LPCSTR lpsText);
// ���������� wchar-������ � ����.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ����� ������� � ����.
_Check_return_
long ZETFILE_API zfAddTextToFileW(_In_ LPCWSTR lpwsFileName, _In_ LPCWSTR lpwsText);

#ifdef UNICODE
#define zfAddTextToFile  zfAddTextToFileW
#else
#define zfAddTextToFile  zfAddTextToFileA
#endif
//-----------------------------------------------------------------------------
// ������ ����� char-������ �� �����.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ����� �������� �� �����.
_Check_return_
long ZETFILE_API zfReadFileTextA(_In_ LPCWSTR lpwsFileName, _In_opt_ LPSTR lpsBuffer, _In_ const DWORD sizeBuffer);
// ������ ����� wchar-������ �� �����.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ����� �������� �� �����.
_Check_return_
long ZETFILE_API zfReadFileTextW(_In_ LPCWSTR lpwsFileName, _In_opt_ LPWSTR lpwsBuffer, _In_ const DWORD sizeBuffer);

#ifdef UNICODE
#define zfReadFileText  zfReadFileTextW
#else
#define zfReadFileText  zfReadFileTextA
#endif
//-----------------------------------------------------------------------------
// ������ (��� ����������) char-������ � ����
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ����� � ���� �������.
_Check_return_
long ZETFILE_API zfWriteFileTextA(_In_ LPCWSTR lpwsFileName, _In_ LPCSTR lpsText);
_Check_return_
// ������ (��� ����������) wchar-������ � ����
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ����� � ���� �������.
long ZETFILE_API zfWriteFileTextW(_In_ LPCWSTR lpwsFileName, _In_ LPCWSTR lpwsText);

#ifdef UNICODE
#define zfWriteFileText  zfWriteFileTextW
#else
#define zfWriteFileText  zfWriteFileTextA
#endif
//-----------------------------------------------------------------------------
// ������ ������ � �������� ����.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ������ � ���� ��������.
_Check_return_
long ZETFILE_API zfWriteFileBinary(_In_ LPCWSTR lpwsFileName, _In_ const PVOID pointer, _In_ const DWORD size);
//-----------------------------------------------------------------------------
// ������ ������ �� ��������� �����.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ������ �� ����� ���������.
_Check_return_
long ZETFILE_API zfReadFileBinary(_In_ LPCWSTR lpwsFileName, _In_opt_ PVOID pointer, _In_  const DWORD size);
//-----------------------------------------------------------------------------
// ������ � ��������� log-����.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ������ � ���� ��������.
_Check_return_
long ZETFILE_API zfAddLog(_In_ LPCWSTR lpwsTitle, _In_ LPCWSTR lpwsText, _In_ LPCWSTR lpcwsFileNameLog, _In_ const bool bNeedNameexe = true);
//-----------------------------------------------------------------------------
// ������ char-������ � �������� ����, ��� ��� ��������.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ����� � ���� �������.
_Check_return_
long ZETFILE_API zfWriteIntoFileA(_In_ FILE *pFile, _In_ LPCSTR lpsText);
// ������ wchar-������ � �������� ����, ��� ��� ��������.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ����� � ���� �������.
_Check_return_
long ZETFILE_API zfWriteIntoFileW(_In_ FILE *pFile, _In_ LPCWSTR lpwsText);

#ifdef UNICODE
#define zfWriteIntoFile  zfWriteIntoFileW
#else
#define zfWriteIntoFile  zfWriteIntoFileA
#endif
//-----------------------------------------------------------------------------
// ������ � �������� �������� ����, ��� ��� ��������.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ������ � ���� ��������.
_Check_return_
long ZETFILE_API zfWriteIntoFileB(_In_ FILE *pFile, _In_ const PVOID pointer, _In_ const DWORD size);
//-----------------------------------------------------------------------------
// ������ char-������ �� ��������� ���������� �����, ��� ��� ��������.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ����� �� ����� ��������.
_Check_return_
long ZETFILE_API zfReadFromFileA(_In_ FILE *pFile, _In_opt_ LPSTR lpsBuffer, _In_ const DWORD sizeBuffer);
_Check_return_
// ������ wchar-������ �� ��������� ���������� �����, ��� ��� ��������.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ����� �� ����� ��������.
long ZETFILE_API zfReadFromFileW(_In_ FILE *pFile, _In_opt_ LPWSTR lpwsBuffer, _In_ const DWORD sizeBuffer);

#ifdef UNICODE
#define zfReadFromFile  zfReadFromFileW
#else
#define zfReadFromFile  zfReadFromFileA
#endif
//-----------------------------------------------------------------------------
// ������ ������ �� ��������� ��������� �����, ��� ��� ��������.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ������ �� ����� ���������.
_Check_return_
long ZETFILE_API zfReadFromFileB(_In_ FILE *pFile, _In_opt_ PVOID pointer, _In_ const DWORD size);
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//	����� CFolderDialog ������������ ��� ������ ������� ������ ����� (�� �����!)
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/*	������������� ������:
CFolderDialog cd(L"����� ����� ��� ...", L"c:\\Maker", L"c:", m_hWnd);
if (cd.DoModal() == IDOK)
{
	m_CEdit.SetWindowText(cd.GetSelectedFolder());
}	*/
//------------------------------------------------------------------------------
/* ���� ������� ������� ��������� �������, ���������, �������� ����� ������ �
������� �����.
���� ����� �������� ������, �� ���� ������� ����� ���������.
��������� - ��� �� ��� ���� ������. �� ������������ ������ ����,
� ������� ������.
���� ������ �������� ����� ������ (��������: �:), �� ����� �������� ������ �
��������� ���� ����� (������ �� ����� �).
���� ������ ������� �����, �� �� ��������� ��� ����� ���������.
*/
//------------------------------------------------------------------------------
#define CFOLDER_MAX_PATH		MAX_PATH * 10		// ������������ ����� ���� �����
//------------------------------------------------------------------------------
/* ����������� ������
 CFolderDialog(LPCWSTR lpwsTitle = nullptr, LPCWSTR lpwsCurFolder = nullptr, LPCWSTR lpwsRoot = nullptr, const HWND hWndOwner = NULL)
 ���������:
	- ��������� � ���� ������ �����;
	- ��� ����� �� ���������;
	- ��� �������� ����� ������;
	- ���������� ���� ���������.
*/			 
//------------------------------------------------------------------------------
class ZETFILE_API CFolderDialog
{
private:
	HWND m_hWndOwner;
	wchar_t *m_sTitle;
	wchar_t *m_sRoot;
	wchar_t *m_sFolder;
	wchar_t *m_sTmp;

	bool b_UseRootFolder;
protected:

public:
	CFolderDialog(LPCWSTR lpwsTitle = nullptr, LPCWSTR lpwsCurFolder = nullptr,
		LPCWSTR lpwsRoot = nullptr, const HWND hWndOwner = NULL);
	~CFolderDialog();

	// ���������� � ����� ��������� ����� �����
	bool m_bAddingSlash;
	// ����������� ������ ������� ����� �����.
	// ��������� ����������� ��������� ����� ��� ���������� �
	// ����������� ������ � �����
	bool m_bCreateNewFolder;
	// ����������� ����� ��������� ����� ��� ����������
	bool m_bStatusText;
	// ����������� ������ � �����
	bool m_bViewFilesInFolder;

	// ������� ������� --------------------------------------------------------
	/* ��� ��������� ����� */
	wchar_t* GetSelectedFolder();
	// ��� �������� ����� ������
	wchar_t* GetRootFolder();
	// ��������� � ���� ������ �����
	wchar_t* GetTitle();

	// ������� ����� ��������� ����� �� ���������
	void SetCurFolder(LPCWSTR lpwsCurFolder);
	// ������� �������� ����� ������
	void SetRootFolder(LPCWSTR lpwsRoot);
	// ������� ������ ��������� � ���� ������
	void SetTitle(LPCWSTR lpwsTitle);
	// ������� ����������� ���� ���������
	void SetOwner(const HWND hWndOwner);

	//// ������� ������ ---------------------------------------------------------
	/* ����� ���� �������. ������������ ��������:
	- IDOK - ����� �������;
	- IDCANCEL - ����� �� �������
	*/
	INT_PTR DoModal();
};
//-----------------------------------------------------------------------------
// �������� �����, ��� �������� ������ char-�������.
// ������� ����������� �������� ������� �����, ����� �������� ������� �����.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ���� ����� ��� ��� �� ����.
_Check_return_ ZETFILE_API 
long zfDeleteFileA(_In_opt_ LPCSTR lpsName);

// �������� �����, ��� �������� ������ wchar-�������.
// ������� ����������� �������� ������� �����, ����� �������� ������� �����.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ���� ����� ��� ��� �� ����.
_Check_return_ ZETFILE_API 
long zfDeleteFileW(_In_opt_ LPCWSTR lpwsName);

#ifdef UNICODE
#define  zfDeleteFile   zfDeleteFileW
#else
#define  zfDeleteFile   zfDeleteFileA
#endif
//-----------------------------------------------------------------------------
// �������� ���� ������ �� �����, ��� ������� ������ char-�������.
// ������� ����������� �������� ������� �����.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ����� ������� ��� �� �� ����.
_Check_return_ ZETFILE_API
long zfDeleteAllFilesInDirectoryA(_In_ LPCSTR lpsName);

// �������� ���� ������ �� �����, ��� ������� ������ wchar-�������.
// ������� ����������� �������� ������� �����.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ����� ������� ��� �� �� ����.
_Check_return_ ZETFILE_API
long zfDeleteAllFilesInDirectoryW(_In_ LPCWSTR lpwsName);

#ifdef UNICODE
#define  zfDeleteAllFilesInDirectory   zfDeleteAllFilesInDirectoryW
#else
#define  zfDeleteAllFilesInDirectory   zfDeleteAllFilesInDirectoryA
#endif
//-----------------------------------------------------------------------------
// �������� ������ �����, ��� ������� ������ char-�������.
// ������� ����������� �������� ������� �����.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ����� ������� ��� � �� ����.
_Check_return_ ZETFILE_API
long zfDeleteDirectoryA(_In_opt_ LPCSTR lpsName);

// �������� ������ �����, ��� ������� ������ wchar-�������.
// ������� ����������� �������� ������� �����.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ����� ������� ��� � �� ����.
_Check_return_ ZETFILE_API
long zfDeleteDirectoryW(_In_opt_ LPCWSTR lpwsName);

#ifdef UNICODE
#define  zfDeleteDirectory   zfDeleteDirectoryW
#else
#define  zfDeleteDirectory   zfDeleteDirectoryA
#endif
//-----------------------------------------------------------------------------
// �������� ���� (����� � ������), ��� ������� ������ char-�������.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ��� ����� � ����� ������� ��� �� �� ����.
_Check_return_ ZETFILE_API
long zfDeletePathA(_In_opt_ LPCSTR lpsName);

// �������� ���� (����� � ������), ��� ������� ������ wchar-�������.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ��� ����� � ����� ������� ��� �� �� ����.
_Check_return_ ZETFILE_API
long zfDeletePathW(_In_opt_ LPCWSTR lpwsName);

#ifdef UNICODE
#define  zfDeletePath   zfDeletePathW
#else
#define  zfDeletePath   zfDeletePathA
#endif
//-----------------------------------------------------------------------------
// ��������� ������� �����, ��� �������� ������ char-�������.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ������ ��������.
_Check_return_ ZETFILE_API
long zfGetDirectorySizeInBytesA(_In_opt_ LPCSTR lpName, _Out_ ULONGLONG * pSize, _Out_ ULONGLONG * pSizeInDisk);

// ��������� ������� �����, ��� �������� ������ wchar-�������.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ������ ��������.
_Check_return_ ZETFILE_API
long zfGetDirectorySizeInBytesW(_In_opt_ LPCWSTR lpwName, _Out_ ULONGLONG * pSize, _Out_ ULONGLONG * pSizeInDisk);

#ifdef UNICODE
#define  zfGetDirectorySizeInBytes   zfGetDirectorySizeInBytesW
#else
#define  zfGetDirectorySizeInBytes   zfGetDirectorySizeInBytesA
#endif
//-----------------------------------------------------------------------------
// ������� ��� ����� � ����� � �������� �����������
//@param lpsDirectory - ���������� � ������� ����� ����������� �������� ������
//@param lpsExtension - ���������� ��������� ������, �������� � ����� ������� ���� (� ������ � ���), ���� ������ ������ ������ ������ ��� �����
//@param bIncludeSubdirectory - ���� ��� ����� ������� ����� �� �������������
//@return 0 ���� ��������� �������
_Check_return_ ZETFILE_API
long zfDeleteFilesInDirectoryByExtensionA(_In_ LPCSTR lpDirectory, _In_ LPCSTR lpExtension, _In_opt_ bool bIncludeSubdirectory = false);
// ������� ��� ����� � ����� � �������� �����������
//@param lpsDirectory - ���������� � ������� ����� ����������� �������� ������
//@param lpsExtension - ���������� ��������� ������, �������� � ����� ������� ���� (� ������ � ���), ���� ������ ������ ������ ������ ��� �����
//@param bIncludeSubdirectory - ���� ��� ����� ������� ����� �� �������������
//@return 0 ���� ��������� �������
_Check_return_ ZETFILE_API
long zfDeleteFilesInDirectoryByExtensionW(_In_ LPCWSTR lpDirectory, _In_ LPCWSTR lpExtension, _In_opt_ bool bIncludeSubdirectory = false);

#ifdef UNICODE
#define  zfDeleteFilesInDirectoryByExtension  zfDeleteFilesInDirectoryByExtensionW
#else
#define  zfDeleteFilesInDirectoryByExtension  zfDeleteFilesInDirectoryByExtensionA
#endif
//-----------------------------------------------------------------------------
// �������� ����� � ��� �� ���������� �� ���������� ���� � ��������� �����
//@param lpFolderDir	  - ���������� � ������� ��������� ���������� �����
//@param lpDestinationDir - ���������� ���� ����� ����������� �����
//@return 0 ���� ��������� �������, ����� ����� - ��� ������
ZETFILE_API long zfCopyFolderA(_In_ LPCSTR lpFolderDir, _In_ LPCSTR lpDestinationDir);

// �������� ����� � ��� �� ���������� �� ���������� ���� � ��������� �����
//@param lpFolderDir	  - ���������� � ������� ��������� ���������� �����
//@param lpDestinationDir - ���������� ���� ����� ����������� �����
//@return 0 ���� ��������� �������, ����� ����� - ��� ������
ZETFILE_API long zfCopyFolderW(_In_ LPCWSTR lpFolderDir, _In_ LPCWSTR lpDestinationDir);

// �������� ����� � ��� �� ���������� �� ���������� ���� � ��������� �����
#ifdef UNICODE
#define  zfCopyFolder  zfCopyFolderW
#else
#define  zfCopyFolder  zfCopyFolderA
#endif
//-----------------------------------------------------------------------------
// �������� ���������� ����� ������ �� ��� ���
//@param lpFileName	- ��� �����
//@param bTextFile	- ���� true �� ���� ����������� ��� ���������, � ��������� ������ ��� ��������
//@return 1 ���� ������, 0 ���� �� ������, ����� ����� - ��� ������
ZETFILE_API long zfIsFileEmptyA(_In_ LPCSTR lpFileName, _In_opt_ bool bTextFile = true);

// �������� ���������� ����� ������ �� ��� ���
//@param lpFileName	- ��� �����
//@param bTextFile	- ���� true �� ���� ����������� ��� ���������, � ��������� ������ ��� ��������
//@return 1 ���� ������, 0 ���� �� ������, ����� ����� - ��� ������
ZETFILE_API long zfIsFileEmptyW(_In_ LPCWSTR lpwsFileName, _In_opt_ bool bTextFile = true);

// �������� ���������� ����� ������ �� ��� ���
//@param lpFileName	- ��� �����
//@param bTextFile	- ���� true �� ���� ����������� ��� ���������, � ��������� ������ ��� ��������
//@return 1 ���� ������, 0 ���� �� ������, ����� ����� - ��� ������

long ZETFILE_API zfWriteIntoFileFlushW(_In_ FILE *pFile, _In_ LPCWSTR lpwsText);
// �������������� ������ � ���� pFile.
// ������������ ��������:
// ������������� �������� - ��� ������;
// 0 - ������ � ���� ������������� ��������.

#ifdef UNICODE
#define  zfIsFileEmpty  zfIsFileEmptyW
#else
#define  zfIsFileEmpty  zfIsTextFileEmptyA
#endif
//-----------------------------------------------------------------------------