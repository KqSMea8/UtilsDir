#include "StdAfx.h"
#include "CommonWriter.h"
#include <ZETFile/include/ZETFile.h>

CCommonWriter::CCommonWriter()
: m_sSignalDirectory(L"")
, m_sCompressedDirectory(L"")
, m_sCurrentSignalDirectory(L"")
, m_pDriveChecker(NULL)
, m_pDescriptorFileHelper(NULL)
, m_wtType(wtUnknown)
, m_hFile(NULL)
, m_bTestMode(false)
{

}

CCommonWriter::~CCommonWriter()
{
}

void CCommonWriter::SetSignalDirectory(CString sDirectory)
{
	m_sSignalDirectory = sDirectory;
}

void CCommonWriter::SetCompressedDirectory(CString sDirectory)
{
	m_sCompressedDirectory = sDirectory;
}

void CCommonWriter::SetDriveChecker(CDriveChecker* pDriveChecker)
{
	m_pDriveChecker = pDriveChecker;
}

void CCommonWriter::SetDescriptorFileHelper(CDescriptorFileHelper* pDescriptorFileHelper)
{
	m_pDescriptorFileHelper = pDescriptorFileHelper;
}

CChannel* CCommonWriter::GetChannelInfo()
{
	return &m_Channel;
}

double CCommonWriter::GetDuration()
{
	double dDuration(0.);

	if (m_hFile != NULL)
	{
		DWORD dwSizeHigh(0);
		dDuration = GetFileSize(m_hFile, &dwSizeHigh) / sizeof(float) / m_Channel.GetFrequency();
	}

	return dDuration;
}

void CCommonWriter::StartWriter(double dTime)
{
	CTime tTime = long long(dTime);
	StartWriter(tTime);
}

BOOL CCommonWriter::CreateDataFile(CString &sFileName, bool bEmpty)
{
	BOOL bRet = FALSE;

	m_hFile = CreateFile(sFileName, GENERIC_READ | GENERIC_WRITE, NULL, NULL, CREATE_NEW, NULL, NULL);
	if (INVALID_HANDLE_VALUE != m_hFile)
	{
		BOOL bCheckDiskSpace(true);
		if (m_pDriveChecker != NULL)
		{
			bCheckDiskSpace = m_pDriveChecker->GetFreeDiskSpace(m_sSignalDirectory);
			while (!bCheckDiskSpace)
			{
				if (!DeleteOldRecords())
					break;
				bCheckDiskSpace = m_pDriveChecker->GetFreeDiskSpace(m_sSignalDirectory);
			}
		}

		if (!bEmpty)
		{
			long long llFileSize = long long(m_Channel.GetFrequency() * 60. * 60. + 0.5) * sizeof(float);
			long lFileSizeLow = llFileSize & 0xffffffff;
			long lFileSizeHigh = (llFileSize >> 32) & 0xffffffff;
			if (llFileSize == SetFilePointer(m_hFile, lFileSizeLow, &lFileSizeHigh, FILE_BEGIN))
			{
				if (FALSE != SetEndOfFile(m_hFile))
					bRet = TRUE;
			}
		}
		else
			bRet = TRUE;
			
		CloseHandle(m_hFile);
		m_hFile = NULL;
	}

	return bRet;
}

BOOL CCommonWriter::CreateLogFile(CString &sFileName)
{
	BOOL bRet = FALSE;

	HANDLE hFile = CreateFile(sFileName, GENERIC_READ | GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, NULL, NULL);
	if (INVALID_HANDLE_VALUE != hFile)
	{
		bRet = TRUE;
		CloseHandle(hFile);
	}
	else
	{
		bRet = FALSE;
	}

	return bRet;
}

WRITER_ERROR_CODE CCommonWriter::WriteLog(double dTime, CString &sMessage)
{
    // Лямбда функция создания записи для лога
    auto createLogText = [](CTime seconds, double nanoseconds, CString msgtext)->CString
    {
		CString sLogText(L"");

        if (nanoseconds == 0)
            sLogText = seconds.Format(L"[%Y-%m-%d  %H:%M:%S]") + L" " + msgtext + L"\n";
        else
        {
            sLogText = seconds.Format(L"[%Y-%m-%d  %H:%M:%S");
            sLogText.AppendFormat(L".%03d] ", long(nanoseconds * 1000));
            sLogText += msgtext + L"\n";
        }

        return sLogText;
    };

    if (dTime <= 0)
        return wecInvalidTime;

    CTime  sec  = long long(dTime);      // Секундная составляющая времени
    double nano = dTime - long(dTime);   // Наносекундная составляющая времени

    // Запись исходных данных
	CString sCurrentDirectory(L"");
	sCurrentDirectory.Format(L"%04d\\%02d\\%02d\\%02d", sec.GetYear(), sec.GetMonth(), sec.GetDay(), sec.GetHour());
	sCurrentDirectory.Insert(0, m_sSignalDirectory);
    //sCurrentDirectory.Format(m_sSignalDirectory + L"%04d\\%02d\\%02d\\%02d", sec.GetYear(), sec.GetMonth(), sec.GetDay(), sec.GetHour());
	long err(0);
    if (m_FileHelper.CheckDirectory(sCurrentDirectory))
    {
        CString sLogFile = sCurrentDirectory + L"\\sig0000.log";

        err = zfAddTextToFile(sLogFile, createLogText(sec, nano, sMessage));
    }


    // Запись сжатых данных
	sCurrentDirectory.Format(L"%04d\\%02d", sec.GetYear(), sec.GetMonth());
	sCurrentDirectory.Insert(0, m_sCompressedDirectory);
	//sCurrentDirectory.Format(m_sCompressedDirectory + L"%04d\\%02d", sec.GetYear(), sec.GetMonth());
    if (m_FileHelper.CheckDirectory(sCurrentDirectory))
    {
        CString sLogFile = sCurrentDirectory + L"\\sig0000.log";

        err = zfAddTextToFile(sLogFile, createLogText(sec, nano, sMessage));
    }

    return wecSuccessful;
}