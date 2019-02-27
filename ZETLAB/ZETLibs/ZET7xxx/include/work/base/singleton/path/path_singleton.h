#pragma once

#ifndef path_singleton_h_
#define path_singleton_h_

#include <ZETSimple\include\singleton\singleton.h>

#include <boost\noncopyable.hpp>

#include <shlwapi.h>
#include <string>

class CPathSingletonImpl : private boost::noncopyable
{
public:
	CPathSingletonImpl();
	~CPathSingletonImpl();

	std::wstring getSignalDirectory() const;
	void setSignalDirectory(std::wstring& sPath);
	std::wstring getCompressedDirectory() const;
	void setCompressedDirectory(std::wstring& sPath);

protected:
	wchar_t m_sPath[MAX_PATH];
	wchar_t m_sSignalPath[MAX_PATH];
	wchar_t m_sCompressedPath[MAX_PATH];
};

typedef CSingleton<CPathSingletonImpl> CPathSingleton;

#endif //path_singleton_h_

