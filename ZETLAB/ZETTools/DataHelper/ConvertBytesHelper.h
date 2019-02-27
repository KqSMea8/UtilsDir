#pragma once

//////////////////////////////////////////////////////////////////////////
/// Data bytes convertions help functions.
//////////////////////////////////////////////////////////////////////////

//value of some type to byte
template<typename T> DWORD type2bytes(const T &val, byte *buf)
{
	DWORD size = sizeof(T);
	memcpy_s(buf, size, &val, size);
	return size;
}
//byte to value of some type
template<typename T> DWORD bytes2type(const byte *buf, T *val)
{
	DWORD size = sizeof(T);
	memcpy_s(val, size, buf, size);
	return size;
}

//////////////////////////////////////////////////////////////////////////
class ByteConverter
{
private:
	ByteConverter();
	ByteConverter(const ByteConverter&);
	ByteConverter operator=(const ByteConverter&);
public:
	static DWORD CString2bytes(const CString &str, byte *buf)
	{
		DWORD strLen = str.GetLength() + 1, strSize = sizeof(TCHAR) * strLen, shift;
		shift = type2bytes(strLen, buf);
		memmove(buf + shift, str.GetString(), strSize);
		return shift + strSize;
	}
	static DWORD bytes2CString(const byte *buf, CString *str)
	{
		DWORD shift, strLen, strSize;
		shift = bytes2type(buf, &strLen);
		strSize = sizeof(TCHAR) * strLen;
		TCHAR strBuffer[1024] = { 0 };
		memmove(strBuffer, buf + shift, strSize);
		*str = strBuffer;
		return shift + strSize;
	}
};