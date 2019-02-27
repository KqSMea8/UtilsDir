#pragma once
#include <atlstr.h>
#include <pugixml-1.4/src/pugixml.hpp>

using namespace pugi;

//******************************************************************************
// преобразуем строку в значение
template<typename T>
T StrToValue(const CString& str);

// преобразуем значение атрибута
template<typename T>
T AtrToValue(const pugi::xml_attribute& atr);

// преобразуем значение в строку
template<typename T>
std::wstring ValToStr(const T& val);

//******************************************************************************
template <class T>
void AddChildToNode(_In_ pugi::xml_node Node, _In_ const CString &Name, _In_ const T &Val)
{
    Node.append_child(Name).append_child(pugi::node_pcdata).set_value(ValToStr<T>(Val).c_str());
}

template <class T>
void AddAttributeToChild(_In_ pugi::xml_node Node, _In_ const CString &Name, _In_ const T &Val)
{
    Node.append_attribute(Name).set_value(ValToStr<T>(Val).c_str());
}

template <class T>
bool GetAttributeFromNode(_In_ pugi::xml_node Node, _In_ const CString &Name, _In_ T *Val)
{
    bool bRes(Val != nullptr);
    pugi::xml_attribute Atr = Node.attribute(Name);
    bRes &= (Atr != nullptr);
    if (bRes)
        *Val = AtrToValue<T>(Atr);

    return bRes;
}

template <class T>
bool GetValueFromNode(_In_ pugi::xml_node Node, _In_ const CString &Name, _In_ T *Val)
{
    bool bRes(Val != nullptr);
    pugi::xml_node Child = Node.child(Name).first_child();
    bRes &= (Child != nullptr);
    if (bRes)
        *Val = StrToValue<T>(Child.value());

    return bRes;
}

//******************************************************************************
#define CONVERT_TO_STRING(specifier, val) \
CString tempStr, Format((CString)L'%' + L#specifier); \
tempStr.Format(Format, val); \
return tempStr.GetString();

// преобразуем значение в строку
template<>
std::wstring ValToStr(const bool& val) { return val ? L"1" : L"0"; }
template<>
std::wstring ValToStr(const int& val) { CONVERT_TO_STRING(d, val) }
template<>
std::wstring ValToStr(const unsigned int& val) { CONVERT_TO_STRING(ud, val) }
template<>
std::wstring ValToStr(const long& val) { CONVERT_TO_STRING(l, val) }
template<>
std::wstring ValToStr(const long long& val) { CONVERT_TO_STRING(ll, val) }
template<>
std::wstring ValToStr(const unsigned long& val) { CONVERT_TO_STRING(ul, val) }
template<>
std::wstring ValToStr(const unsigned long long& val) { CONVERT_TO_STRING(ull, val) }
template<>
std::wstring ValToStr(const float& val) { CONVERT_TO_STRING(f, val) }
template<>
std::wstring ValToStr(const double& val) { CONVERT_TO_STRING(lf, val) }
template<>
std::wstring ValToStr(const CString& val) { return val.GetString(); }
template<>
std::wstring ValToStr(const std::string& val) { return std::wstring(val.begin(), val.end()); }
template<>
std::wstring ValToStr(const CTime& val) { return val.Format(L"%d.%m.%Y %H:%M:%S").GetString(); }

//******************************************************************************
// преобразуем значение атрибута
template<>
bool AtrToValue<bool>(const pugi::xml_attribute& atr) { return atr.as_bool(); }
template<>
int AtrToValue<int>(const pugi::xml_attribute& atr) { return atr.as_int(); }
template<>
unsigned int AtrToValue<unsigned int>(const pugi::xml_attribute& atr) { return atr.as_uint(); }
template<>
long AtrToValue<long>(const pugi::xml_attribute& atr) { return (long)atr.as_llong(); }
template<>
long long AtrToValue<long long>(const pugi::xml_attribute& atr) { return atr.as_llong(); }
template<>
unsigned long AtrToValue<unsigned long>(const pugi::xml_attribute& atr) { return (unsigned long)atr.as_ullong(); }
template<>
unsigned long long AtrToValue<unsigned long long>(const pugi::xml_attribute& atr) { return atr.as_ullong(); }
template<>
float AtrToValue<float>(const pugi::xml_attribute& atr) { return atr.as_float(); }
template<>
double AtrToValue<double>(const pugi::xml_attribute& atr) { return atr.as_double(); }
template<>
CString AtrToValue<CString>(const pugi::xml_attribute& atr) { return atr.as_string(); }
template<>
std::string AtrToValue<std::string>(const pugi::xml_attribute& atr)
{
    std::wstring str(atr.as_string());
    return std::string(str.begin(), str.end());
}
template<>
CTime AtrToValue<CTime>(const pugi::xml_attribute& atr)
{
    SYSTEMTIME st = { 0 };
    swscanf_s(CString(atr.as_string()).GetString(), L"%hd.%hd.%hd %hd:%hd:%hd",
              &st.wDay, &st.wMonth, &st.wYear,
              &st.wHour, &st.wMinute, &st.wSecond);
    return st;
}

//******************************************************************************
// преобразуем значение строки
template<>
bool StrToValue<bool>(const CString& str) { return _wtoi(str) != 0; }
template<>
int StrToValue<int>(const CString& str) { return _wtoi(str); }
template<>
unsigned int StrToValue<unsigned int>(const CString& str) { return _wtoi(str); }
template<>
long StrToValue<long>(const CString& str) { return (long)_wtoll(str); }
template<>
long long StrToValue<long long>(const CString& str) { return _wtoll(str); }
template<>
unsigned long StrToValue<unsigned long>(const CString& str) { return (unsigned long)_wtoll(str); }
template<>
unsigned long long StrToValue<unsigned long long>(const CString& str) { return (unsigned long long)_wtoll(str); }
template<>
float StrToValue<float>(const CString& str) { return (float)_wtof(str); }
template<>
double StrToValue<double>(const CString& str) { return _wtof(str); }
template<>
CString StrToValue<CString>(const CString& str) { return str; }
template<>
std::string StrToValue<std::string>(const CString& atr) { return std::string(CStringA(atr)); }
template<>
CTime StrToValue<CTime>(const CString& str)
{
    SYSTEMTIME st = { 0 };
    swscanf_s(str.GetString(), L"%hd.%hd.%hd %hd:%hd:%hd",
              &st.wDay, &st.wMonth, &st.wYear,
              &st.wHour, &st.wMinute, &st.wSecond);
    return st;
}

