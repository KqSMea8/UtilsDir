#include "StdAfx.h"
#include "DescriptorFileHelper.h"
#include <rpcdce.h>
#include <comutil.h>
#include <algorithm>
#include <pugixml-1.4/src/pugixml.hpp>

#pragma comment(lib, "Rpcrt4.lib")

using namespace pugi;

#define DESCRIPTOR_FILE_VERSION				L"1.0"
#define COMMON_DESCRIPTOR_FILE_VERSION		L"1.0"

CDescriptorFileHelper::CDescriptorFileHelper(void)
{
}

CDescriptorFileHelper::~CDescriptorFileHelper(void)
{
}

void CDescriptorFileHelper::AddDirectory( CString sDirectory )
{
    CChannelEx channelex;
    WIN32_FIND_DATA win32_find_data;
    WCHAR wcDirectory[MAX_PATH] = {0};
    wcscpy_s(wcDirectory, MAX_PATH, sDirectory);
    wcscat_s(wcDirectory, MAX_PATH, L"\\*.xml");

    HANDLE hFind = FindFirstFile(wcDirectory, &win32_find_data);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            CString sFileName(CString(win32_find_data.cFileName));
            if(!(win32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                sFileName = sDirectory + L"\\" + sFileName;
                if (std::find_if(m_mDescriptors[sDirectory].begin(), m_mDescriptors[sDirectory].end(), [&](CChannelEx &channelex)
                {
                    return channelex.GetFileName() == sFileName;
                }) == m_mDescriptors[sDirectory].end())
                {
                    xml_document hFile;
                    if (hFile.load_file(sFileName, parse_default, encoding_utf8))
                    {
                        xml_node hRoot = hFile.child(L"SignalDescriptor");
                        if (hRoot != NULL)
                        {
                            xml_node hSignal = hRoot.child(L"Signal");
                            if (hSignal != NULL)
                            {
                                channelex.SetFileName(sFileName);
                                if (hSignal.attribute(L"name") != NULL)
                                    channelex.SetName(hSignal.attribute(L"name").value());
                                if (hSignal.attribute(L"id") != NULL)
                                {
                                    GUID guid(GUID_NULL);
                                    if (S_OK == CLSIDFromString(hSignal.attribute(L"id").value(), &guid))
                                        channelex.SetID(guid);
                                    else if (RPC_S_OK == UuidFromString((RPC_WSTR)hSignal.attribute(L"id").value(), &guid))
                                        channelex.SetID(guid);
                                }
                                if (hSignal.attribute(L"sense") != NULL)
                                    channelex.SetSense(hSignal.attribute(L"sense").as_float());
                                if (hSignal.attribute(L"conversion") != NULL)
                                    channelex.SetConversion(hSignal.attribute(L"conversion").value());
                                if (hSignal.attribute(L"frequency") != NULL)
                                    channelex.SetFrequency(hSignal.attribute(L"frequency").as_double());
                                m_mDescriptors[sDirectory].push_back(channelex);
                            }
                        }
                        else
                        {
                            hRoot = hFile.child(L"CommonSignalDescriptor");
                            if (hRoot != NULL)
                            {
                                xml_node hSignal = hRoot.child(L"Signal");
                                while (hSignal != NULL)
                                {
                                    channelex.SetFileName(sFileName);
                                    if (hSignal.attribute(L"name") != NULL)
                                        channelex.SetName(hSignal.attribute(L"name").value());
                                    if (hSignal.attribute(L"id") != NULL)
                                    {
                                        GUID guid(GUID_NULL);
                                        if (S_OK == CLSIDFromString(hSignal.attribute(L"id").value(), &guid))
                                            channelex.SetID(guid);
                                        else if (RPC_S_OK == UuidFromString((RPC_WSTR)hSignal.attribute(L"id").value(), &guid))
                                            channelex.SetID(guid);
                                    }
                                    if (hSignal.attribute(L"sense") != NULL)
                                        channelex.SetSense(hSignal.attribute(L"sense").as_float());
                                    if (hSignal.attribute(L"conversion") != NULL)
                                        channelex.SetConversion(hSignal.attribute(L"conversion").value());
                                    if (hSignal.attribute(L"frequency") != NULL)
                                        channelex.SetFrequency(hSignal.attribute(L"frequency").as_double());
                                    m_mCommonDescriptors[sDirectory].push_back(channelex);
                                    hSignal = hSignal.next_sibling();
                                }
                            }
                        }
                    }
                }
            }
        }
        while(FindNextFile(hFind, &win32_find_data));
        FindClose( hFind );
    }
}

CString CDescriptorFileHelper::FindDescriptorFile( CString &sDirectory, ChannelInfo *channel)
{
    if (m_mDescriptors.find(sDirectory) == m_mDescriptors.end())
        AddDirectory(sDirectory);
    else
    {
        long lCounter(0);
        WIN32_FIND_DATA win32_find_data;
        WCHAR wcDirectory[MAX_PATH] = {0};
        wcscpy_s(wcDirectory, MAX_PATH, sDirectory);
        wcscat_s(wcDirectory, MAX_PATH, L"\\*.xml");
    
        HANDLE hFind = FindFirstFile(wcDirectory, &win32_find_data);
        if (hFind != INVALID_HANDLE_VALUE)
        {
            do
            {
                if(!(win32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                {
                    CString sFileName(CString(win32_find_data.cFileName));
                    if (sFileName.Find(L"0000.xml") == -1)
                        ++lCounter;
                }
            }
            while(FindNextFile(hFind, &win32_find_data));
            FindClose( hFind );
        }
        if (lCounter != m_mDescriptors[sDirectory].size())
        {
            AddDirectory(sDirectory);
        }
    }
    
    auto itFound = std::find_if(m_mDescriptors[sDirectory].begin(), m_mDescriptors[sDirectory].end(), [&](CChannelEx &channelex)
    {
        return (channelex.GetName() == channel->GetName())/* &&
                //(channelex.GetID() == channel->GetID()) && 
                //(channelex.GetMinLevel() == channel->GetMinLevel()) &&
                (channelex.GetConversion() == channel->GetConversion())*/;
    });
    if (itFound != m_mDescriptors[sDirectory].end())
        return itFound->GetFileName();
    else
        return L"";
}

CString CDescriptorFileHelper::FindCommonDescriptorFile( CString &sDirectory )
{
    if (m_mCommonDescriptors.find(sDirectory) == m_mCommonDescriptors.end())
        AddDirectory(sDirectory);
    
    if (m_mCommonDescriptors[sDirectory].empty())
        return L"";
    else
        return sDirectory + L"\\sig0000.xml";
}

double CDescriptorFileHelper::GetFrequencyFromDescriptorFile( CString &sDirectory, CString &sDescriptorFile )
{
    auto itFound = std::find_if(m_mDescriptors[sDirectory].begin(), m_mDescriptors[sDirectory].end(), [&](CChannelEx &channelex)
    {
        return (channelex.GetFileName() == sDescriptorFile);
    });
    if (itFound != m_mDescriptors[sDirectory].end())
        return itFound->GetFrequency();
    else
        return 0.;
}

bool CDescriptorFileHelper::FindChannelInCommonDescriptorFile( CString &sDirectory, ChannelInfo *channel )
{
    return std::find_if(m_mCommonDescriptors[sDirectory].begin(), m_mCommonDescriptors[sDirectory].end(), [&](ChannelInfo &channel_)
    {
        return (channel_.GetName() == channel->GetName())/* &&
            (channel_.GetID() == channel->GetID()) && 
            //(channel_.GetMinLevel() == channel->GetMinLevel()) &&
            (channel_.GetConversion() == channel->GetConversion())*/;
    }) != m_mCommonDescriptors[sDirectory].end();
}

void CDescriptorFileHelper::AppendCommonDescriptorFile(CString &sDirectory, CString &sCommonDescriptorFile, ChannelInfo *channel, CString &sDescriptorFile, CString &sDataFile/*, CString &sLogFile*/)
{
    xml_document hFile;
    if (hFile.load_file(sCommonDescriptorFile, parse_default, encoding_utf8))
    {
        xml_node hRoot = hFile.child(L"CommonSignalDescriptor");
        if (hRoot != NULL)
        {
            xml_node hSignal = hRoot.append_child(L"Signal");
            hSignal.append_attribute(L"name").set_value(channel->GetName());
            hSignal.append_attribute(L"comment").set_value(channel->GetComment());
            hSignal.append_attribute(L"format").set_value(L"f2");
            hSignal.append_attribute(L"frequency").set_value(channel->GetFrequency());
            hSignal.append_attribute(L"minlevel").set_value(channel->GetMinLevel());
            hSignal.append_attribute(L"maxlevel").set_value(channel->GetMaxLevel());
            hSignal.append_attribute(L"sense").set_value(channel->GetSense());
            hSignal.append_attribute(L"reference").set_value(channel->GetReference());
            hSignal.append_attribute(L"shift").set_value(channel->GetShift());
            hSignal.append_attribute(L"conversion").set_value(channel->GetConversion());
            hSignal.append_attribute(L"channel").set_value(channel->GetNumber());
            hSignal.append_attribute(L"typeADC").set_value(channel->GetTypeADC());
            hSignal.append_attribute(L"numberDSP").set_value(channel->GetNumberDSP());
            CComBSTR guidStr(channel->GetID());
            CString sValue = guidStr;
            hSignal.append_attribute(L"id").set_value(sValue);
            hSignal.append_attribute(L"type").set_value(channel->GetType());
            hSignal.append_attribute(L"groupname").set_value(channel->GetGroupName());
            hSignal.append_attribute(L"descriptor_file").set_value(sDescriptorFile);
            hSignal.append_attribute(L"data_file").set_value(sDataFile);
            //hSignal.append_attribute(L"log_file").set_value(sLogFile);

            m_mCommonDescriptors[sDirectory].push_back(*channel);

            hFile.save_file( sCommonDescriptorFile, L"\t", format_default, encoding_utf8 );
        }
    }
}

CString CDescriptorFileHelper::CreateCommonDescriptorFile( CString &sDirectory, CTime tTime )
{
    CString sReturn(L"");
    CString sFileName = sDirectory + L"\\sig0000.xml";
    xml_document hFile;
    hFile.load(L"");
    if (hFile != NULL)
    {
        xml_node hRoot = hFile.append_child(L"CommonSignalDescriptor");
        if (hRoot != NULL)
        {
            hRoot.append_attribute(L"version").set_value(COMMON_DESCRIPTOR_FILE_VERSION);
            CString sDate(L"");
            sDate.Format(L"%02d.%02d.%04d", tTime.GetDay(), tTime.GetMonth(), tTime.GetYear());
            hRoot.append_attribute(L"date").set_value(sDate);
            CString sTime(L"");
            sTime.Format(L"%02d:00:00", tTime.GetHour());
            hRoot.append_attribute(L"time").set_value(sTime);
            if (hFile.save_file( sFileName, L"\t", format_default, encoding_utf8 ))
                sReturn = sFileName;
        }
    }

    return sReturn;
}

CString CDescriptorFileHelper::CreateDescriptorFile( CString &sDirectory, CTime tTime, ChannelInfo *channel )
{
    CString sReturn(L"");
    CChannelEx channelex;
    *(ChannelInfo*)(&channelex) = *channel;

    WIN32_FIND_DATA win32_find_data;
    WCHAR wcDirectory[MAX_PATH] = {0};
    wcscpy_s(wcDirectory, MAX_PATH, sDirectory);
    wcscat_s(wcDirectory, MAX_PATH, L"\\*.xml");
    long lCounter(0);

    HANDLE hFind = FindFirstFile(wcDirectory, &win32_find_data);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            CString sFileName(CString(win32_find_data.cFileName));
            if(!(win32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                if (sFileName.Find(L"sig0000.xml") == -1)
                {
                    long counter(lCounter);
                    swscanf_s(sFileName.GetString(), L"sig%04d.xml", &counter);
                    if (lCounter < counter)
                        lCounter = counter;
                }
            }
        }
        while(FindNextFile(hFind, &win32_find_data));
        FindClose( hFind );
    }

    CString sFileName(L"");

    //временные anp-файлы
    sFileName.Format(L"\\sig%04d.anp", ++lCounter);
    sFileName.Insert(0, sDirectory);
    //sFileName.Format(sDirectory + L"\\sig%04d.anp", ++lCounter);

    FILE* hANP(nullptr);
    if ((fopen_s(&hANP, "fprintf_s.out", "wt") == 0) && (hANP != nullptr))
    {
        fprintf_s(hANP, "%s\n", (CStringA(channelex.GetName())).GetString());
        fprintf_s(hANP, "COMMENT\n");
        fprintf_s(hANP, "GAIN 1.0\n");
        float absVolt = channelex.GetMinLevel();
        if (channelex.GetSense() != 0.)
            absVolt *= channelex.GetSense();
        fprintf_s(hANP, "ABSVOLT %g\n", absVolt);
        fprintf_s(hANP, "FRQ  %f\n", channelex.GetFrequency());
        fprintf_s(hANP, "TMI 0\n");
        fprintf_s(hANP, "FRL 0.0\n");
        fprintf_s(hANP, "FRH %7.2f\n", channelex.GetFrequency() * 0.5);
        fprintf_s(hANP, "FORMAT f2\n");
        fprintf_s(hANP, "START  %02d:%02d:%02d.000\n", tTime.GetHour(), tTime.GetMinute(), tTime.GetSecond());
        fprintf_s(hANP, "DATE  %02d-%02d-%04d\n", tTime.GetDay(), tTime.GetMonth(), tTime.GetYear());
        GUID guid = channelex.GetID();
        fprintf_s(hANP, "CHANNEL %d\tTypeAdc %d\tNumberAdc %d\tSerial %d\tGroupName %s\n",
            channelex.GetNumber() + 1,
            channelex.GetTypeADC(),
            channelex.GetNumberDSP(),
            (long(*(long*)((long)&guid + 12)) >> 16) & 0xffff,
            (CStringA(channelex.GetGroupName())).GetString());
        fprintf_s(hANP, "MAXLEVEL %g\n", channelex.GetMaxLevel());
        fprintf_s(hANP, "SENSE %g\n", channelex.GetSense());
        fprintf_s(hANP, "CONVERT %s\n", (CStringA(channelex.GetConversion())).GetString());
        fprintf_s(hANP, "AMPL 1.0\n");
        fprintf_s(hANP, "REFER %g\n", channelex.GetReference());
        fprintf_s(hANP, "AFCH 0\n");
        fprintf_s(hANP, "DC %g\n", channelex.GetShift());
        CComBSTR guidStr(guid);
        fprintf_s(hANP, "ID %s\n", ((CStringA)guidStr.m_str).GetString());

        fclose(hANP);
        hANP = NULL;
    }

    sFileName.Format(L"\\sig%04d.xml", lCounter);
    sFileName.Insert(0, sDirectory);
    //sFileName.Format(sDirectory + L"\\sig%04d.xml", lCounter);
    channelex.SetFileName(sFileName);
    xml_document hFile;
    hFile.load(L"");
    if (hFile != NULL)
    {
        xml_node hRoot = hFile.append_child(L"SignalDescriptor");
        if (hRoot != NULL)
        {
            hRoot.append_attribute(L"version").set_value(DESCRIPTOR_FILE_VERSION);
            xml_node hSignal = hRoot.append_child(L"Signal");
            if (hSignal != NULL)
            {
                hSignal.append_attribute(L"name").set_value(channelex.GetName());
                hSignal.append_attribute(L"comment").set_value(channelex.GetComment());
                hSignal.append_attribute(L"format").set_value(L"f2");
                hSignal.append_attribute(L"frequency").set_value(channelex.GetFrequency());
                hSignal.append_attribute(L"minlevel").set_value(channelex.GetMinLevel());
                hSignal.append_attribute(L"maxlevel").set_value(channelex.GetMaxLevel());
                hSignal.append_attribute(L"sense").set_value(channelex.GetSense());
                hSignal.append_attribute(L"reference").set_value(channelex.GetReference());
                hSignal.append_attribute(L"shift").set_value(channelex.GetShift());
                hSignal.append_attribute(L"conversion").set_value(channelex.GetConversion());
                hSignal.append_attribute(L"channel").set_value(channelex.GetNumber());
                hSignal.append_attribute(L"typeADC").set_value(channelex.GetTypeADC());
                hSignal.append_attribute(L"numberDSP").set_value(channelex.GetNumberDSP());
                CComBSTR guidStr(channelex.GetID());
                CString sValue = guidStr;
                hSignal.append_attribute(L"id").set_value(sValue);
                hSignal.append_attribute(L"type").set_value(channelex.GetType());
                hSignal.append_attribute(L"groupname").set_value(channelex.GetGroupName());
            }
        }

        if (hFile.save_file( sFileName, L"\t", format_default, encoding_utf8 ))
        {
            m_mDescriptors[sDirectory].push_back(channelex);
            sReturn = sFileName;
        }
    }
    
    return sReturn;
}

CString CDescriptorFileHelper::FindCommonCompressedDescriptorFile( CString &sDirectory )
{
    return FindCommonDescriptorFile(sDirectory);
    /*if (m_mCommonDescriptors.find(sDirectory) == m_mCommonDescriptors.end())
        AddDirectory(sDirectory);

    if (m_mCommonDescriptors[sDirectory].empty())
        return L"";
    else
        return sDirectory + L"\\sig0000.xml";*/
}

CString CDescriptorFileHelper::CreateCommonCompressedDescriptorFile( CString &sDirectory, CTime tTime )
{
    return CreateCommonDescriptorFile(sDirectory, tTime);
}

bool CDescriptorFileHelper::FindChannelInCommonCompressedDescriptorFile( CString &sDirectory, ChannelInfo *channel )
{
    return FindChannelInCommonDescriptorFile(sDirectory, channel);
}

void CDescriptorFileHelper::AppendCommonCompressedDescriptorFile(CString &sDirectory, CString &sCommonDescriptorFile, ChannelInfo *channel, CString &sDescriptorFile, CString &sDataFile, CString &sLogFile)
{
    xml_document hFile;
    if (hFile.load_file(sCommonDescriptorFile, parse_default, encoding_utf8))
    {
        xml_node hRoot = hFile.child(L"CommonSignalDescriptor");
        if (hRoot != NULL)
        {
            xml_node hSignal = hRoot.append_child(L"Signal");
            hSignal.append_attribute(L"name").set_value(channel->GetName());
            hSignal.append_attribute(L"comment").set_value(channel->GetComment());
            hSignal.append_attribute(L"format").set_value(L"f2");
            hSignal.append_attribute(L"frequency").set_value(channel->GetFrequency());
            hSignal.append_attribute(L"minlevel").set_value(channel->GetMinLevel());
            hSignal.append_attribute(L"maxlevel").set_value(channel->GetMaxLevel());
            hSignal.append_attribute(L"sense").set_value(channel->GetSense());
            hSignal.append_attribute(L"reference").set_value(channel->GetReference());
            hSignal.append_attribute(L"shift").set_value(channel->GetShift());
            hSignal.append_attribute(L"conversion").set_value(channel->GetConversion());
            hSignal.append_attribute(L"channel").set_value(channel->GetNumber());
            hSignal.append_attribute(L"typeADC").set_value(channel->GetTypeADC());
            hSignal.append_attribute(L"numberDSP").set_value(channel->GetNumberDSP());
            CComBSTR guidStr(channel->GetID());
            CString sValue = guidStr;
            hSignal.append_attribute(L"id").set_value(sValue);
            hSignal.append_attribute(L"type").set_value(channel->GetType());
            hSignal.append_attribute(L"groupname").set_value(channel->GetGroupName());
            hSignal.append_attribute(L"descriptor_file").set_value(sDescriptorFile);
            hSignal.append_attribute(L"data_file").set_value(sDataFile);
            //hSignal.append_attribute(L"log_file").set_value(sLogFile);

            m_mCommonDescriptors[sDirectory].push_back(*channel);

            hFile.save_file( sCommonDescriptorFile, L"\t", format_default, encoding_utf8 );
        }
    }
}

CString CDescriptorFileHelper::GetLastSignalDirectory( CString sDirectory )
{
    long lBeginYear(-1), lEndYear(-1);
    FindMinMaxDirectory(sDirectory, &lBeginYear, &lEndYear);
    if (lEndYear > 0)
    {
        long lBeginMonth(-1), lEndMonth(-1);
        sDirectory.AppendFormat(L"%04d\\", lEndYear);
        FindMinMaxDirectory(sDirectory, &lBeginMonth, &lEndMonth);
        if (lEndMonth > 0)
        {
            long lBeginDay(-1), lEndDay(-1);
            sDirectory.AppendFormat(L"%02d\\", lEndMonth);
            FindMinMaxDirectory(sDirectory, &lBeginDay, &lEndDay);
            if (lEndDay > 0)
            {
                long lBeginHour(-1), lEndHour(-1);
                sDirectory.AppendFormat(L"%02d\\", lEndDay);
                FindMinMaxDirectory(sDirectory, &lBeginHour, &lEndHour);
                if (lEndHour > 0)
                    sDirectory.AppendFormat(L"%02d\\", lEndHour);
            }
        }
    }
    return sDirectory;
}

void CDescriptorFileHelper::FindMinMaxDirectory(CString sDirecory, long *pMin, long *pMax)
{
    long lMin(-1), lMax(-1);
    WIN32_FIND_DATA win32_find_data;
    WCHAR wcDirectory[MAX_PATH] = {0};
    wcscpy_s(wcDirectory, MAX_PATH, sDirecory);
    wcscat_s(wcDirectory, MAX_PATH, L"*.*");
    HANDLE hFind = FindFirstFile(wcDirectory, &win32_find_data);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            CString sFileName(CString(win32_find_data.cFileName));
            if (win32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                long lFileName;
                if (swscanf_s(sFileName, L"%d", &lFileName) == 1)
                {
                    if (lMin < 0)
                        lMin = lFileName;
                    else
                    {
                        if (lMin > lFileName)
                            lMin = lFileName;
                    }
                    if (lMax < 0)
                        lMax = lFileName;
                    else
                    {
                        if (lMax < lFileName)
                            lMax = lFileName;
                    }
                }
            }
        } while (FindNextFile(hFind, &win32_find_data));
        FindClose(hFind);
    }

    if (pMin != NULL)
        *pMin = lMin;
    if (pMax != NULL)
        *pMax = lMax;
}