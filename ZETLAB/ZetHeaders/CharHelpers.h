#pragma once
#include <string>

#if defined(__ATLSTR_H__) || defined (_AFX)
#ifdef _UNICODE
//по умолчанию берем 1251 кодировку
//из char в WCHAR
inline CString STD2MFCstring(std::string const& s, UINT cp=1251/*CP_ACP*/)
{
    CString ss;
    int len = MultiByteToWideChar(cp, 0, s.c_str(), (int)s.size(), ss.GetBuffer((int)s.size()), (int)s.size());
    ss.ReleaseBuffer(len);
    return ss;
}

//по умолчанию берем 1251 кодировку
//из WCHAR в char
inline std::string MFC2STDstring(CString const& s,UINT cp = 1251/*CP_ACP*/)
{
    std::string ss(s.GetLength()*2, ' ');
    int len = WideCharToMultiByte(cp, 0, s, s.GetLength(), &ss[0], (int)ss.size(), 0, 0);
    ss.resize(len);
    return ss;
}
#else
//из char в char
inline CString STD2MFCstring (std::string const& s)
{
    return s.c_str();
}

//из char в char
inline std::string MFC2STDstring(CString const& s)
{
    return (LPCSTR)s;
}

#endif
#endif //__ATLSTR_H__

//по умолчанию берем 1251 кодировку
//из WCHAR в char
inline std::string WCHAR2STDstring(std::wstring const& s,UINT cp = 1251/*CP_ACP*/)
{
    std::string ss(s.size()*2, ' ');
    int len = WideCharToMultiByte(cp, 0, s.c_str(), s.size(), &ss[0], (int)ss.size(), 0, 0);
    ss.resize(len);
    return ss;
}
//по умолчанию берем 1251 кодировку
//из char в WCHAR
inline std::wstring STD2WCHARstring(std::string const& s,UINT cp = 1251/*CP_ACP*/)
{
    std::wstring ss(s.size()+1, ' ');
    int len = MultiByteToWideChar(cp, 0, s.c_str(), s.size(), &ss[0], (int)ss.size());
    ss.resize(len);
    return ss;
}
