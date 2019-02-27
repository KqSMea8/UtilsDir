#pragma once

class CFileHelper
{
public:
	CFileHelper(void);
	~CFileHelper(void);

	long FileExists(CString &sFileName);
	long DirectoryExists(CString &sDirectory);
	bool CheckDirectory(CString &sDirectory);
};

