#include "stdafx.h"
#include "OfficeBase.h"

COfficeBase::COfficeBase()
{
}

COfficeBase::~COfficeBase()
{
}

int COfficeBase::Read(LPCTSTR FileName)
{
	tstring filename(FileName);
	int ret = ReadZip(filename);
	if ( ret == ZIP_OK )
		ret = FindDocument();
	return ret;
}

int COfficeBase::Write(LPCTSTR FileName)
{
	tstring filename(FileName);
	return WriteZip(filename);
}

int COfficeBase::FindString(LPCTSTR strFind, unsigned int start /* = 0 */)
{
	char* chr = getUtf8String(strFind);
	int res = FindString(chr, start);
	delete  chr;
	chr = nullptr;
	return res;
}

int COfficeBase::FindString(LPCSTR strFind, unsigned int start /* = 0 */)
{
	if ( start > m_iterFile->zdesc.uncompressed_size )
		return -1;

	char* pos = strstr((char*)&m_iterFile->pUncmData[start], strFind);

	if ( pos == NULL )
		return -1;
	else
		return pos - (char*)m_iterFile->pUncmData;
}

int COfficeBase::ReplaceString(LPCTSTR strFind, LPCTSTR strReplace, unsigned int start /* = 0 */)
{
	int pos = FindString(strFind, start);
	if ( pos >= 0 )
	{
		char* chrFind = getUtf8String(strFind);
		char* chrReplace = getUtf8String(strReplace);

		int ret = ReplaceString(chrFind, chrReplace);

		delete chrFind;
		chrFind = nullptr;
		delete chrReplace;
		chrReplace = nullptr;

		return ret;
	}
	return -1;
}

int COfficeBase::ReplaceString(LPCSTR strFind, LPCSTR strReplace, unsigned int start /* = 0 */)
{
	int pos = FindString(strFind, start);
	if ( pos >= 0 )
	{
		int sizeFind = strlen(strFind);
		int sizeReplase = strlen(strReplace);

		ReplaceChars((char*)m_iterFile->pUncmData + pos, sizeFind, (char*)strReplace, sizeReplase);

		/*Byte* newBuffer = new Byte[m_iterFile->zdesc.uncompressed_size + sizeReplase - sizeFind];

		memcpy_s(newBuffer, pos, m_iterFile->pUncmData, pos);
		memcpy_s(&newBuffer[pos], sizeReplase, strReplace, sizeReplase);
		memcpy_s(&newBuffer[pos + sizeReplase], m_iterFile->zdesc.uncompressed_size - pos - sizeFind, &m_iterFile->pUncmData[pos + sizeFind], m_iterFile->zdesc.uncompressed_size - pos - sizeFind);

		delete[] m_iterFile->pUncmData;
		m_iterFile->pUncmData = newBuffer;
		m_iterFile->zdesc.uncompressed_size += sizeReplase - sizeFind;
		if ( m_iterFile->zdesc.signature != 0x08074B50 )
			m_iterFile->zhead.uncompressed_size = m_iterFile->zdesc.uncompressed_size;*/

		return 0;
	}
	return -1;
}

int COfficeBase::ReplaceChars(char* src, long srcsize, char* dest, long destsize)
{
	Byte* newBuffer = nullptr;
	try 
	{
		newBuffer = new Byte[m_iterFile->zdesc.uncompressed_size + destsize - srcsize];
	}
	catch (...)
	{
		return -1;
	}

	long pos = src - (char*)m_iterFile->pUncmData;
	if (pos <= 0)
		return -2;
	//if (destsize <= 0)
	//	return -3;
	long size1 = long(m_iterFile->zdesc.uncompressed_size) - pos - srcsize;
	if (size1 <= 0)
		return -4;
	
	memcpy_s(newBuffer, pos, m_iterFile->pUncmData, pos);
	memcpy_s(&newBuffer[pos], destsize, dest, destsize);
	memcpy_s(&newBuffer[pos + destsize], size1, &m_iterFile->pUncmData[pos + srcsize], size1);
	
	delete[] m_iterFile->pUncmData;
	m_iterFile->pUncmData = nullptr;
	m_iterFile->pUncmData = newBuffer;
	m_iterFile->zdesc.uncompressed_size += destsize - srcsize;
	if (m_iterFile->zdesc.signature != 0x08074B50)
		m_iterFile->zhead.uncompressed_size = m_iterFile->zdesc.uncompressed_size;

	return pos;
}

int COfficeBase::DeleteChars(char* src, long srcsize)
{
	Byte* newBuffer = nullptr;
	try
	{
		newBuffer = new Byte[m_iterFile->zdesc.uncompressed_size  - srcsize];
	}
	catch (...)
	{
		return -1;
	}

	long pos = src - (char*)m_iterFile->pUncmData;
	if (pos <= 0)
		return -2;
	long size1 = long(m_iterFile->zdesc.uncompressed_size) - pos - srcsize;
	if (size1 <= 0)
		return -3;
	memcpy_s(newBuffer, pos, m_iterFile->pUncmData, pos);
	memcpy_s(&newBuffer[pos], size1, &m_iterFile->pUncmData[pos + srcsize], size1);

	delete[] m_iterFile->pUncmData;
	m_iterFile->pUncmData = nullptr;
	m_iterFile->pUncmData = newBuffer;
	m_iterFile->zdesc.uncompressed_size -= srcsize;
	if (m_iterFile->zdesc.signature != 0x08074B50)
		m_iterFile->zhead.uncompressed_size = m_iterFile->zdesc.uncompressed_size;

	return pos;
}

char* COfficeBase::getUtf8String(LPCTSTR str)
{
	int result_u, result_c;
	result_u = WideCharToMultiByte(CP_UTF8, 0, str, -1, 0, 0, 0, 0);
	if ( !result_u )
		return 0;
	char* chr = new char[result_u];

	result_c = WideCharToMultiByte(CP_UTF8, 0, str, -1, chr, result_u, 0, 0);

	return chr;
}

int COfficeBase::GetDocumentSize()
{
	return m_iterFile->zdesc.uncompressed_size;
}

LPSTR COfficeBase::GetString(int position, int length)
{
	char* str = new char[length+1];
	memcpy_s(str, length, (char*)&m_iterFile->pUncmData[position], length);
	str[length] = '\0';
	return str;
}

int COfficeBase::FindFile(LPCSTR FileName)
{
	std::list<LocalFile>::iterator itFile;
	std::list<CentralDir>::iterator itDir;

	for (itFile = m_lFiles.begin(), itDir = m_lDirs.begin(); itFile != m_lFiles.end(), itDir != m_lDirs.end(); ++itFile, ++itDir)
	{
		int ret = strcmp(itFile->pFileName, FileName);
		if (ret == 0)
		{
			m_iterFile = itFile;
			m_iterDir = itDir;
			return 0;
		}
	}

	return -1;
}