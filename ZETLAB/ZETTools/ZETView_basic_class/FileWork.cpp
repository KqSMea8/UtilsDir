//------------------------------------------------------------------------------
#include "StdAfx.h"
#include "fileWork.h"	// Для работы с библиотекой filedll.dll (библиотека д.Володи Торопова)
//------------------------------------------------------------------------------
//извлечение пути к директории SCADA системы
//CString CFileWork::GetConfigPath()
//{
//	CString ret;
//	CRegKey rKey;
//	WCHAR keyValue[255] = L"";
//	ULONG keyValueSize = 255;
//	if ( ( rKey.Open(HKEY_LOCAL_MACHINE, L"SOFTWARE\\ZET\\ZETLAB", KEY_READ) ==
//						ERROR_SUCCESS ) &&
//		 (rKey.QueryStringValue(L"InstallLocation", keyValue, &keyValueSize) ==
//						ERROR_SUCCESS ) )
//		ret = CString(keyValue) + L"scada\\";
//	else
//		ret = L"";
//	return ret;
//}

CString CFileWork::GetSCADAAppData()
{
	CString ret;
	CRegKey rKey;
	WCHAR keyValue[MAX_PATH] = { 0 };
	ULONG keyValueSize = MAX_PATH;
	if ( ( rKey.Open(HKEY_LOCAL_MACHINE, L"SOFTWARE\\ZET\\ZETLAB", KEY_READ) ==
						ERROR_SUCCESS ) &&
		(rKey.QueryStringValue(L"DirScadaAppData", keyValue, &keyValueSize) ==
						ERROR_SUCCESS ) )
		ret = CString(keyValue);
	return ret;
}

//CString CFileWork::GetZETLabPath()
//{
//	CString ret;
//	CRegKey rKey;
//	WCHAR keyValue[255] = L"";
//	ULONG keyValueSize = 255;
//	if ( ( rKey.Open(HKEY_LOCAL_MACHINE, L"SOFTWARE\\ZET\\ZETLAB", KEY_READ) ==
//		ERROR_SUCCESS ) &&
//		(rKey.QueryStringValue(L"InstallLocation", keyValue, &keyValueSize) ==
//		ERROR_SUCCESS ) )
//		ret = CString(keyValue);
//	else
//		ret = L"";
//	return ret;
//}

//bool CFileWork::CreateNewFile(CString fileName)
//{
//	FILE *in(nullptr);
////	in = _wfsopen(fileName, L"w", 0x40);
//	in = _wfsopen(fileName.GetString(), L"w, ccs=UTF-8", _SH_DENYNO);
//	if (in != NULL)
//	{
//		fclose(in);
//		in = NULL;
//		return true;
//	}
//	else
//	{
//		long ind = 0;
//		long k = 0;
//		while (k != -1)
//		{
//			ind = k;
//			k = fileName.Find(L"\\", ind + 1);
//		}
//		CString dir = fileName.Mid(0, ind);
//
//		if (CreateNewDirectory(dir))
//		{
//			return CreateNewFile(fileName);
//		}
//		else
//			return false;
//	}
//}

bool CFileWork::CreateNewDirectory(LPCTSTR directoryName)
{
	if (DirectoryExists(directoryName))
		return TRUE;
	CString sDir = (CString)directoryName;
	BOOL bRet = sDir.IsEmpty() ? FALSE : TRUE;
	while ((bRet != FALSE) &&
		( (bRet = CreateDirectory(directoryName, NULL)) == FALSE) )
	{
		int n = sDir.ReverseFind('\\');
		int m = sDir.ReverseFind('/');
		n = max(n, m);
		if (n != -1)
		{
			sDir = sDir.Mid(0, n);
			bRet = CreateNewDirectory(sDir);
		}
		else
			break;
	}
	return bRet != FALSE;
}

//CString CFileWork::LoadAllFromFile(CString fileName, long maxLines)
//{
//	CString ret(L"");
//	if (FileExists(fileName))
//	{
//		FILE *in(nullptr);
//		
//		wchar_t buf=1;
//		CString sr_("");
//		_wfopen_s(&in, fileName.GetString(), L"rt, ccs=UTF-8");
//		long curr = 0;
//		if (in != NULL)
//		{
//			wchar_t* StringData = new wchar_t[2000];
//			ZeroMemory(StringData, 2000 * sizeof(wchar_t));
//			while (!feof(in))
//			{
//				curr++;
//				if (maxLines != -1)
//					if (curr >= maxLines)
//				break;
//				if(!fgetws(StringData, 2000, in))	
//					break;
//				//fread(&buf, sizeof(char), 1, in);
//				//if (buf!=0)
//				//	sr_+=buf;
//				sr_+=CString(StringData);
//
//			}
//			delete []StringData;
//
//			if (in != NULL)
//			{
//				fclose(in);
//				in = NULL;
//			}			//ret = CString(sr_);
//			//ret.Delete(ret.GetLength()-1,1);
//			//удаляем последний символ, почему-то при чтении дублируется последний символ файла
//			//потом разберусь.
//			ret = CString(sr_);
//		}
//	}
//	return ret;
//}

//CStringA CFileWork::LoadAllFromFileA(CString fileName, long maxLines)
//{
//	CStringA ret(L"");
//	if (FileExists(fileName))
//	{
//		FILE *in(nullptr);
//		CString sr_("");
//		in = _fsopen(CStringA(fileName), "rt", _SH_DENYNO);
//		long curr = 0;
//		if (in != NULL)
//		{
//			char* StringData = new char[2000];
//			ZeroMemory(StringData, 2000 * sizeof(char));
//			while (!feof(in))
//			{
//				curr++;
//				if (maxLines != -1)
//					if (curr >= maxLines)
//						break;
//				if(!fgets(StringData, 2000, in))	
//					break;
//				//fread(&buf, sizeof(char), 1, in);
//				//if (buf!=0)
//				//	sr_+=buf;
//				sr_+=CString(StringData);
//
//			}
//			delete []StringData;
//
//			fclose(in);
//			in = NULL;
//
//			//ret = CString(sr_);
//			//ret.Delete(ret.GetLength()-1,1);
//			//удаляем последний символ, почему-то при чтении дублируется последний символ файла
//			//потом разберусь.
//			ret = CString(sr_);
//		}
//	}
//	return ret;
//}

bool CFileWork::LoadAllFromANSIFileByCodePage(CString &fileName, CString &outString, UINT codePage)
{
	outString.Empty();
	HANDLE hFile(CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL));
	bool ret = hFile != INVALID_HANDLE_VALUE;
	if (ret)
	{
		char charData[513] = { 0 };
		TCHAR wideData[513] = { 0, };
		DWORD bytesForRead = 512;
		DWORD bytesRead(0);
		while ((ReadFile(hFile, charData, bytesForRead, &bytesRead, NULL)!= FALSE) && (bytesRead > 0))
		{
			charData[bytesRead] = '\0';
			if (MultiByteToWideChar(codePage, 0, charData, bytesRead + 1, wideData, 512) > 0)
			{
				outString += wideData;
			}
		}
		CloseHandle(hFile);
	}
	return ret;
}

//void CFileWork::AddStringToFile(CString fileName, CString addingStr, CString endOfString)
//{
//	//если файл имеется, то записываем данные в него
//	if (FileExists(fileName))
//	{
//		FILE *out(nullptr);
//		_wfopen_s(&out, fileName.GetString(), L"a, ccs=UTF-8");
//		addingStr += endOfString;
//	
//		if (out != NULL)
//		{
//			fputws(addingStr, out);
//			{
//				fclose(out);
//				out = NULL;
//			}
//		}
//	}
//	else
//	//если такого файла нет, то создаем его
//	{
//		if (CreateNewFile(fileName))
//		{
//			//если все-таки файл создать получилось, то проверяем его еще раз 
//			if (FileExists(fileName))
//			{
//				//и только после этого записываем в него что-либо
//				FILE *out(nullptr);
//				_wfopen_s(&out, fileName.GetString(), L"a, ccs=UTF-8");
//				addingStr += endOfString;
//				if (out != NULL)
//				{ 
//                    	fputws(addingStr, out);
//      					fclose(out);
//						out = nullptr;
//              	}
//			}
//		}
//	}
//}

//void CFileWork::WriteFile(CString fileName, CString addingStr)
//{
//	if (FileExists(fileName))
//	{
//		FILE *out(nullptr);
//		_wfopen_s(&out, fileName, L"w, ccs=UTF-8");
//		if (out != NULL)
//		{
//			fputws(addingStr, out);
//			fclose(out);
//			out = nullptr;
//		}
//	}
//}

//void CFileWork::showFiles(CString dir, CString mask)
//{
//	HANDLE hFind;
//	WIN32_FIND_DATA find_data;
//	WCHAR f_dir[MAX_PATH] = L"";
//
//	wcscpy_s(f_dir, MAX_PATH, dir);
//	wcscat_s(f_dir, MAX_PATH, mask);
//	//для начала очищаем БД файлов
//	m_sFiles.RemoveAll();
//	//и начинаем искать все файлы по заданному в параметрах критерию
//	hFind = FindFirstFileW(f_dir, &find_data);
//	if (hFind != INVALID_HANDLE_VALUE)
//	{
//		do
//		{
//			if(!(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
//			{
//				CString fileName = CString(find_data.cFileName);
//				m_sFiles.Add(fileName);
//			}
//		}
//		while(FindNextFileW(hFind, &find_data));
//		FindClose(hFind);
//	}
//}