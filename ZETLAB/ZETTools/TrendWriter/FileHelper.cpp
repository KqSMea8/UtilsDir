#include "StdAfx.h"
#include "FileHelper.h"
#include <ZETFile\include\ZETFile.h>

CFileHelper::CFileHelper(void)
{
}

CFileHelper::~CFileHelper(void)
{
}

long CFileHelper::FileExists(CString &sFileName)
{
	return zfFileExists(sFileName);
}

long CFileHelper::DirectoryExists(CString &sDirectory)
{
	return zfDirectoryExists(sDirectory);
}

bool CFileHelper::CheckDirectory(CString &sDirectory)
{
	if (0 == zfDirectoryExists(sDirectory))
		return 0 == zfCreatePath(sDirectory + L"\\");
	else
		return true;
}

