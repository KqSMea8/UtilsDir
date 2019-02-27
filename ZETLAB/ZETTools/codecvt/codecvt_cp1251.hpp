#pragma once

#ifndef __CODECVT_CP1251_HPP__
#define __CODECVT_CP1251_HPP__

#include <windows.h>

#include <string>
#include <vector>

inline ::std::wstring WStringFromCP1251(const char *src, size_t len)
{
	std::wstring ret;
	if (src != nullptr)
	{
		len = min(len, strlen(src));
		if (len > 0)
		{
			ret.resize(len);
			MultiByteToWideChar(1251, 0, src, len, (LPWCH)ret.c_str(), len);
		}
	}
	return ret;

// 	if (src && len > 0)
// 	{
// 		// Надеемся, что 64 символа хватит
// 		WCHAR pre[64];
// 		int cch = MultiByteToWideChar(1251, 0, src, len, pre, sizeof(pre));
// 		if (cch > 0)
// 			return ::std::wstring(pre, cch);
// 
// 		if (cch == ERROR_INSUFFICIENT_BUFFER)
// 		{
// 			// Если не хватило, спрашиваем, сколько нужно
// 			cch = MultiByteToWideChar(1251, 0, src, len, NULL, 0);
// 			if (cch > 0)
// 			{
// 				::std::vector<WCHAR> vec(cch);
// 				cch = MultiByteToWideChar(1251, 0, src, len, &vec[0], cch);
// 				if (cch > 0)
// 					return ::std::wstring(vec.data(), cch);
// 			}
// 		}
// 	}
// 	return ::std::wstring();
}

inline ::std::wstring WStringFromCP1251(const char *src)
{
	return WStringFromCP1251(src, strlen(src));
}

inline ::std::string WStringToCP1251(const ::std::wstring &src)
{
	if (!src.empty())
	{
		// Надеемся, что 128 символов хватит
		char pre[128];
		int cb = WideCharToMultiByte(1251, 0, src.c_str(), src.size(), pre, sizeof(pre), NULL, NULL);
		if (cb > 0)
			return ::std::string(pre, cb);

		if (cb == ERROR_INSUFFICIENT_BUFFER)
		{
			// Если не хватило, спрашиваем, сколько нужно
			int cb = WideCharToMultiByte(1251, 0, src.c_str(), src.size(), NULL, 0, NULL, NULL);
			if (cb > 0)
			{
				::std::vector<char> vec(cb);
				cb = WideCharToMultiByte(1251, 0, src.c_str(), src.size(), &vec[0], cb, NULL, NULL);
				if (cb > 0)
					return ::std::string(vec.data(), cb);
			}
		}
	}
	return ::std::string();
}

inline ::std::wstring WStringFromUtf8(const char *src, size_t len)
{
	std::wstring ret;
	if (src != nullptr)
	{
		len = min(len, strlen(src));
		if (len > 0)
		{
			ret.resize(len);
			MultiByteToWideChar(CP_UTF8, 0, src, len, (LPWCH)ret.c_str(), len);
		}
	}
	return ret;
}

inline ::std::wstring WStringFromUtf8(const char *src)
{
	return WStringFromUtf8(src, strlen(src));
}

inline ::std::string WStringToUtf8(const ::std::wstring &src)
{
	if (!src.empty())
	{
		// Надеемся, что 128 символов хватит
		char pre[128];
		int cb = WideCharToMultiByte(CP_UTF8, 0, src.c_str(), src.size(), pre, sizeof(pre), NULL, NULL);
		if (cb > 0)
			return ::std::string(pre, cb);

		if (cb == ERROR_INSUFFICIENT_BUFFER)
		{
			// Если не хватило, спрашиваем, сколько нужно
			int cb = WideCharToMultiByte(CP_UTF8, 0, src.c_str(), src.size(), NULL, 0, NULL, NULL);
			if (cb > 0)
			{
				::std::vector<char> vec(cb);
				cb = WideCharToMultiByte(CP_UTF8, 0, src.c_str(), src.size(), &vec[0], cb, NULL, NULL);
				if (cb > 0)
					return ::std::string(vec.data(), cb);
			}
		}
	}
	return ::std::string();
}

#endif // __CODECVT_CP1251_HPP__
