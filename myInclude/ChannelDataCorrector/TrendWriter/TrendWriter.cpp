#include "StdAfx.h"
#include "TrendWriter.h"

#include <Intel\ipp\ipp.h>

const long g_lCompressions[NUMBER_OF_COMPRESSION] = {1, 10, 60, 600, 3600, 21600, 86400};

CTrendWriter::CTrendWriter(void)
: m_hMap(NULL)

, m_lDaysInCurrentMonth(0)
, m_tLastStartTime(-1)
, m_fMemoryPoint(NAN)
, m_pResamplingData(NULL)
, m_sCurrentDescriptorFile(L"")
, m_bNewHour(false)
{
	SYSTEM_INFO sinf;
	GetSystemInfo(&sinf);
	m_dwAllocationGranularity = sinf.dwAllocationGranularity;

	for (int i = 0; i < NUMBER_OF_COMPRESSION; ++i)
	{
		m_CompressionPoint[i].fMax = 0.f;// NAN;
		m_CompressionPoint[i].fMin = 0.f;// NAN;
		m_CompressionPoint[i].lCompressionTime = g_lCompressions[i];
		m_CompressionPoint[i].SetExtention();
	}
}

CTrendWriter::~CTrendWriter(void)
{
}

void CTrendWriter::StartWriter(CTime tTime)
{
	m_sCurrentSignalDirectory.Format(L"%04d\\%02d\\%02d\\%02d", tTime.GetYear(), tTime.GetMonth(), tTime.GetDay(), tTime.GetHour());
	m_sCurrentSignalDirectory.Insert(0, m_sSignalDirectory);
	//m_sCurrentSignalDirectory.Format(m_sSignalDirectory + L"%04d\\%02d\\%02d\\%02d", tTime.GetYear(), tTime.GetMonth(), tTime.GetDay(), tTime.GetHour());
	m_sCurrentCompressedDirectory.Format(L"%04d\\%02d", tTime.GetYear(), tTime.GetMonth());
	m_sCurrentCompressedDirectory.Insert(0, m_sCompressedDirectory);
	//m_sCurrentCompressedDirectory.Format(m_sCompressedDirectory + L"%04d\\%02d", tTime.GetYear(), tTime.GetMonth());
	m_FileHelper.CheckDirectory(m_sCurrentCompressedDirectory);
}

void CTrendWriter::CheckMinimum( float& fDst, float& fSrc )
{
	//если в поле назначения NAN, то так и оставляем его там
	//если в поле назначения не NAN
	if (_finite(fDst) != 0)
	{
		//если в поле назначения 0 (аналог NAN), т.е. не писалось ничего
		if (abs(fDst) < FLT_MIN)
			//пишем новое значение
			fDst = fSrc;
		//если в поле назначения какое-либо число
		else
		{
			//если в источнике не NAN
			if (_finite(fSrc) != 0 && abs(fSrc) > FLT_MIN)
			{
				//если в источнике минимум меньше того, что в поле назначения
				if (fDst > fSrc)
					fDst = fSrc;
			}
			//если в источнике NAN
			else
				fDst = NAN;
		}
	}

	/*//если в поле назначения NAN
	if ((_finite(fDst) == 0) || (abs(fDst) < FLT_MIN))
	{
		//а в источнике не NAN
		if (_finite(fSrc) != 0 && abs(fSrc) > FLT_MIN)
			fDst = fSrc;
	}
	//если в поле назначения не NAN
	else
	{
		//и в источнике не NAN
		if (_finite(fSrc) != 0 && abs(fSrc) > FLT_MIN)
		{
			//если в источнике минимум меньше того, что в поле назначения
			if (fDst > fSrc)
				fDst = fSrc;
		}
	}*/
}

void CTrendWriter::CheckMaximum( float& fDst, float& fSrc )
{
	//если в поле назначения NAN, то так и оставляем его там
	//если в поле назначения не NAN
	if (_finite(fDst) != 0)
	{
		//если в поле назначения 0 (аналог NAN), т.е. не писалось ничего
		if (abs(fDst) < FLT_MIN)
			//пишем новое значение
			fDst = fSrc;
		//если в поле назначения какое-либо число
		else
		{
			//если в источнике не NAN
			if (_finite(fSrc) != 0 && abs(fSrc) > FLT_MIN)
			{
				//если в источнике максимум больше того, что в поле назначения
				if (fDst < fSrc)
					fDst = fSrc;
			}
			//если в источнике NAN
			else
				fDst = NAN;
		}
	}
	/*//если в поле назначения NAN
	if (_finite(fDst) == 0 || abs(fDst) < FLT_MIN)
	{
		//а в источнике не NAN
		if ((_finite(fSrc) != 0) && (abs(fSrc) > FLT_MIN))
			fDst = fSrc;
	}
	//если в поле назначения не NAN
	else
	{
		//и в источнике не NAN
		if (_finite(fSrc) != 0 && abs(fSrc) > FLT_MIN)
		{
			//если в источнике максимум больше того, что в поле назначения
			if (fDst < fSrc)
				fDst = fSrc;
		}
	}*/
}

bool CTrendWriter::isNan(float value)
{
	return (_finite(value) == 0 || abs(value) < FLT_MIN);
}

void CTrendWriter::Resampling(const float* pSource, long lSourceSize, float* pDestination, long lDestinationSize)
{
	float fTemp(pSource[lSourceSize - 1]);
	//ippsCopy_32f(pSource, pSource + 1, lSourceSize - 1);
	//pSource[0] = m_fMemoryPoint;
	m_fMemoryPoint = fTemp;

	long lIndex;
	float fIndex;
	float fDeltaIndex;
	float fBase;
	float fDelta;
	for(int i = 0; i < lDestinationSize; ++i)
	{
		fIndex = float(i) * float(lSourceSize) / float(lDestinationSize);
		lIndex = long(fIndex);
		fDeltaIndex = fIndex - lIndex;
		if (lIndex < 0)
			fBase = m_fMemoryPoint * (1.f - fDeltaIndex);
		else
			fBase = pSource[lIndex] * (1.f - fDeltaIndex);
		if (lIndex + 1 == lSourceSize)
			fDelta = m_fMemoryPoint * fDeltaIndex;
		else
			fDelta = pSource[lIndex + 1] * fDeltaIndex;
		pDestination[i] = fBase + fDelta;
	}
}

BOOL CTrendWriter::CreateCompressedDataFile( CString sFileName, long lNumber )
{
	BOOL bRet = FALSE;

	HANDLE hFile = CreateFile(sFileName, GENERIC_READ | GENERIC_WRITE, NULL, NULL, CREATE_NEW, NULL, NULL);
	if (INVALID_HANDLE_VALUE != hFile)
	{		
		long long llFileSize = long long(m_lDaysInCurrentMonth * 86400 / m_CompressionPoint[lNumber].lCompressionTime) * sizeof (float) * 2;
		if (llFileSize == SetFilePointer(hFile, long(llFileSize), NULL, FILE_BEGIN))
		{
			if (FALSE != SetEndOfFile(hFile))
				bRet = TRUE;
		}
		CloseHandle(hFile);
	}

	return bRet;
}

CString CTrendWriter::GetCurrentSignalDirectory()
{
	return m_sCurrentSignalDirectory;
}

bool CTrendWriter::GetNewHourFlag()
{
	return m_bNewHour;
}

void CTrendWriter::ResetNewHourFlag()
{
	m_bNewHour = false;
}

void CTrendWriter::FindMinMaxDirectory(CString sDirecory, long *pMin, long *pMax)
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
					wchar_t wcFileName[MAX_PATH] = {0};
					swprintf_s(wcFileName, L"%02d", lFileName);
					if (CString(wcFileName) == sFileName)
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
			}
		} while (FindNextFile(hFind, &win32_find_data));
		FindClose(hFind);
	}

	if (pMin != NULL)
		*pMin = lMin;
	if (pMax != NULL)
		*pMax = lMax;
}