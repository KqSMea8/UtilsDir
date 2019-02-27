#include "StdAfx.h"
#include "SingleWriter.h"
//#include <ZETFile\include\ZETFile.h>

CSingleWriter::CSingleWriter()
{
	m_wtType = wtSingle;
}

CSingleWriter::~CSingleWriter()
{

}

void CSingleWriter::SetCurrentDirectory(CString &sDirectory)
{
	m_sCurrentSignalDirectory = sDirectory;
	m_FileHelper.CheckDirectory(m_sCurrentSignalDirectory);
}

void CSingleWriter::StartWriter(CTime tTime)
{
	if (m_FileHelper.CheckDirectory(m_sCurrentSignalDirectory) && m_pDescriptorFileHelper)
	{
		//���� ����-���������
		CString sDescriptorFile = m_pDescriptorFileHelper->FindDescriptorFile(m_sCurrentSignalDirectory, m_Channel);
		//���� �� ����� ����-���������
		if (sDescriptorFile.GetLength() == 0)
		{
			//������� ����-���������
			sDescriptorFile = m_pDescriptorFileHelper->CreateDescriptorFile(m_sCurrentSignalDirectory, tTime, m_Channel);
			//TRACE("CreateDescriptorFile " + CStringA(m_Channel.GetName()) + " " + CStringA(sDescriptorFile) + "\n");
		}

		//��������� ������� �������������
		double dFrequency = m_pDescriptorFileHelper->GetFrequencyFromDescriptorFile(m_sCurrentSignalDirectory, sDescriptorFile);
		if (dFrequency != m_Channel.GetFrequency())
			m_Channel.SetFrequency(dFrequency);
		//���� ����-��������� ����
		if (1 == m_FileHelper.FileExists(sDescriptorFile))
		{
			//���� ���� ������
			CString sDataFile = sDescriptorFile;
			sDataFile.Replace(L".xml", L".ana");
			//���� ����� ������ ����
			BOOL bDataFlag = TRUE;
			if (0 == m_FileHelper.FileExists(sDataFile))
			{
				//������� ���� ������
				bDataFlag = CreateDataFile(sDataFile, true);
				//TRACE("CreateDataFile " + CStringA(m_Channel.GetName()) + " " + CStringA(sDataFile) + "\n");
			}
			//���� ���� ������ ����
			if (bDataFlag)
			{
				m_hFile = CreateFile(sDataFile, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				if (INVALID_HANDLE_VALUE == m_hFile)
					m_hFile = NULL;
			}
		}
	}
}

void CSingleWriter::StopWriter()
{
	if (m_hFile != NULL)
	{
		CloseHandle(m_hFile);
		m_hFile = NULL;
	}
}

WRITER_ERROR_CODE CSingleWriter::WriteData(const float* pData, long lSize, double dFrequency, double dStart, double dFinish)
{
	if (m_hFile == NULL)
		CCommonWriter::StartWriter(dStart);

	if (m_hFile == NULL)
		return wecCreateFileError;

	DWORD dwWritten(0);
	if (!WriteFile(m_hFile, pData, lSize * sizeof(float), &dwWritten, NULL))
		return wecWriteFileError;

	if (dwWritten != lSize * sizeof(float))
		return wecWriteFileError;

	return wecSuccessful;
}




