#include "stdafx.h"

#include "ConvertBytesHelper.h"

//////////////////////////////////////////////////////////////////////////
/// Data bytes convertions help functions.
//////////////////////////////////////////////////////////////////////////

DWORD CString2bytes(const CString &str, byte *buf)
{
	DWORD strLen = str.GetLength() + 1, strSize = sizeof(TCHAR) * strLen, shift;
	shift = type2bytes(strLen, buf);
	memmove(buf + shift, str.GetString(), strSize);
	return shift + strSize;
}

DWORD bytes2CString(const byte *buf, CString *str)
{
	DWORD shift, strLen, strSize;
	shift = bytes2type(buf, &strLen);
	strSize = sizeof(TCHAR) * strLen;
	TCHAR strBuffer[1024];
	memmove(strBuffer, buf + shift, strSize);
	return shift + strSize;
}