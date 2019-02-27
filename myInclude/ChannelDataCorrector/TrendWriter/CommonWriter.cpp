#include "StdAfx.h"
#include "CommonWriter.h"

CCommonWriter::CCommonWriter()
: m_sSignalDirectory(L"")
, m_sCompressedDirectory(L"")
, m_sCurrentSignalDirectory(L"")
, m_pDescriptorFileHelper(NULL)
, m_hFile(NULL)
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

void CCommonWriter::SetDescriptorFileHelper(CDescriptorFileHelper* pDescriptorFileHelper)
{
	m_pDescriptorFileHelper = pDescriptorFileHelper;
}

ChannelInfo* CCommonWriter::GetChannelInfo()
{
	return m_Channel.get();
}

void CCommonWriter::SetChannelInfo(std::shared_ptr<ChannelInfo> channelInfo)
{
	m_Channel = channelInfo;
}

double CCommonWriter::GetDuration()
{
	double dDuration(0.);

	if (m_hFile != NULL)
	{
		DWORD dwSizeHigh(0);
		dDuration = GetFileSize(m_hFile, &dwSizeHigh) / sizeof(float) / m_Channel->GetFrequency();
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

		if (!bEmpty)
		{
			long long llFileSize = long long(m_Channel->GetFrequency() * 60. * 60. + 0.5) * sizeof(float);
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
