#include "StdAfx.h"
#include "FileHelper.h"
#include <filesystem>

CFileHelper::CFileHelper(void)
{
}

CFileHelper::~CFileHelper(void)
{
}

long CFileHelper::FileExists(CString &sFileName)
{
	return std::experimental::filesystem::is_regular_file(sFileName.GetString());
}

long CFileHelper::DirectoryExists(CString &sDirectory)
{
	return std::experimental::filesystem::is_directory(sDirectory.GetString());
}

bool CFileHelper::CheckDirectory(CString &sDirectory)
{
	if (!std::experimental::filesystem::is_directory(sDirectory.GetString()))
	{
		if (!CreateDirectory(sDirectory.GetString(), nullptr))
		{
			// ���� �� ��������� ������� ���������� �������� ������� ��������
			int subDirs = 0;
			CString tempDir = sDirectory;
			do
			{
				DWORD code = GetLastError();

				if (code == ERROR_PATH_NOT_FOUND)
				{
					int slashInd = tempDir.ReverseFind(L'\\');
					if (slashInd == -1)
						slashInd = tempDir.ReverseFind(L'//');
					tempDir = tempDir.Left(slashInd);

					++subDirs;
				}
				else if (code == ERROR_ACCESS_DENIED)
					throw CString(L"���������� ���������� " + sDirectory);
				else
					throw CString(L"������ �������� ���������� " + sDirectory);
			} while (!CreateDirectory(tempDir.GetString(), nullptr));

			bool bDirCreated = std::experimental::filesystem::is_directory(tempDir.GetString());
			if (bDirCreated)
			{
				for (subDirs--; subDirs >= 0; --subDirs)
				{
					tempDir = sDirectory;

					// ������� � ����� ����������
					for (int i = 0; i != subDirs; ++i)
					{
						int slashInd = tempDir.ReverseFind(L'\\');
						if (slashInd == -1)
							slashInd = tempDir.ReverseFind(L'//');

						tempDir = tempDir.Left(slashInd);
					}

					if (!CreateDirectory(tempDir.GetString(), nullptr))
						throw CString(L"������ �������� ���������� " + sDirectory);
				}
			}
			else
				throw CString(L"������ �������� ���������� " + sDirectory);

			if (!std::experimental::filesystem::is_directory(sDirectory.GetString()))
				throw CString(L"������ �������� ���������� " + sDirectory);
		}
	}
	return true;
}

