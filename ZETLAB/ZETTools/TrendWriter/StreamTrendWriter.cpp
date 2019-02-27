#include "StdAfx.h"
#include "StreamTrendWriter.h"
//#include <ZETFile\include\ZETFile.h>

#include <Intel\ipp\ipp.h>

CStreamTrendWriter::CStreamTrendWriter()
	: m_hEvent(NULL)
{
	m_wtType = wtTrendStream;
	m_hEvent = CreateEvent(NULL, TRUE, TRUE, L"WriteDataEvent");
    m_bEnforceTrendWriter = FALSE;
}

CStreamTrendWriter::CStreamTrendWriter(const CStreamTrendWriter & writer)
	: CTrendWriter(writer)
{
	m_hEvent = CreateEvent(NULL, TRUE, TRUE, L"WriteDataEvent");
    m_bEnforceTrendWriter = FALSE;
}

CStreamTrendWriter & CStreamTrendWriter::operator = (const CStreamTrendWriter & writer)
{
	*((CTrendWriter*)this) = *(CTrendWriter*)&writer;
	m_hEvent = CreateEvent(NULL, TRUE, TRUE, L"WriteDataEvent");
	return *this;
}

CStreamTrendWriter::~CStreamTrendWriter()
{
	if (m_hEvent != NULL)
		CloseHandle(m_hEvent);
//	m_hEvent = NULL;
}

void CStreamTrendWriter::OnEnforceTrendWriterMode()
{
    m_bEnforceTrendWriter = TRUE;
}

void CStreamTrendWriter::OffEnforceTrendWriterMode()
{
    m_bEnforceTrendWriter = FALSE;
}

void CStreamTrendWriter::StopWriter()
{
	//TRACE(L"StopWriter\n");

	if (m_tLastStartTime >= 0)
	{
		for (int i = 0; i < NUMBER_OF_COMPRESSION; ++i)
		{
			CTime tStart = CTime(CTimeSpan(m_tLastStartTime.GetTime()).GetTotalSeconds() / m_CompressionPoint[i].lCompressionTime * m_CompressionPoint[i].lCompressionTime);
			WriteCompressedData(i, double(CTimeSpan(tStart.GetTime()).GetTotalSeconds()), double(CTimeSpan(tStart.GetTime()).GetTotalSeconds() + m_CompressionPoint[i].lCompressionTime));
		}
	}
	m_sCurrentDescriptorFile.Empty();
	if (m_pResamplingData != NULL)
	{
		delete[]m_pResamplingData;
		m_pResamplingData = nullptr;
	}
	if (m_hMap != NULL)
	{
		CloseHandle(m_hMap);
		m_hMap = NULL;
	}
	if (m_hFile != NULL)
	{
		CloseHandle(m_hFile);
		m_hFile = NULL;
	}
}

WRITER_ERROR_CODE CStreamTrendWriter::WriteData(const float* pData, long lSize, double dFrequency, double dStart, double dFinish)
{
//CString s;
	if (lSize <= 0)
		return wecIncorrectDataSize;

	if (pData == NULL)
		return wecNullWriteData;

	WRITER_ERROR_CODE wecWriteCompressedDataResult(wecSuccessful), wecResult(wecSuccessful);

	WaitForSingleObject(m_hEvent, INFINITE);
	ResetEvent(m_hEvent);

//s.Format(L"WriteData %d\n", m_Channel.GetNumber());
//OutputDebugString(s);

    double dDelta = double(lSize) / dFrequency / 100.;   // Принудительная запись трендов сигналов
	CTime tStart = long long(dStart);
	double dStartMS = dStart - long long(dStart);
	double dStartMSEx = 1. - dStartMS;
	if (abs(dStartMSEx) <= dDelta)
		tStart += 1;
	CTime tFinish = long long(dFinish);
	double dFinishMS = dFinish - long long(dFinish);
	double dFinishMSEx = 1. - dFinishMS;
	if (abs(dFinishMSEx) <= dDelta)
		tFinish += 1;

	if (!m_bTestMode)
	{
		CTime tCurrent = CTime::GetCurrentTime();
		CTimeSpan tsDelta = tCurrent - tFinish;
		long long llDelta = tsDelta.GetTotalSeconds();
		if (abs(llDelta) > 3600)
		{
			SetEvent(m_hEvent);
			return wecLargeTimeDifference;
		}
	}

//s.Format(L"WriteData: dStartMS %.10lf, dStartMSEx %.10lf\n", dStartMS, dStartMSEx);
//OutputDebugString(s);
	//если начало секунды, надо проверить переход через границу временного интервала сжатия
    if (abs(dStartMS) <= dDelta || abs(dStartMSEx) <= dDelta || m_bEnforceTrendWriter)
	{
		//проверка перехода временной границы периода сжатия
		for (int i = 0; i < NUMBER_OF_COMPRESSION; ++i)
		{
			bool bNeedToWriteCompressedData(false);
			//если временной интервал сжатия внутри минуты
			if (m_CompressionPoint[i].lCompressionTime < 60)
			{
				bNeedToWriteCompressedData = (tStart.GetSecond() % m_CompressionPoint[i].lCompressionTime == 0);
			}
			else
			{
				//если временной интервал сжатия внутри часа
				if (m_CompressionPoint[i].lCompressionTime < 3600)
				{
					bNeedToWriteCompressedData = ((tStart.GetSecond() == 0) && (tStart.GetMinute() % (m_CompressionPoint[i].lCompressionTime / 60) == 0));
				}
				else
				{
					//если временной интервал сжатия внутри дня
					if (m_CompressionPoint[i].lCompressionTime < 86400)
					{
						bNeedToWriteCompressedData = ((tStart.GetSecond() == 0) && (tStart.GetMinute() == 0) && (tStart.GetHour() % (m_CompressionPoint[i].lCompressionTime / 3600) == 0));
					}
					//если временной интервал сжатия больше дня
					else
					{
						bNeedToWriteCompressedData = ((tStart.GetSecond() == 0) && (tStart.GetMinute() == 0) && (tStart.GetHour() == 0));
					}
				}
			}
            if (bNeedToWriteCompressedData)
            {
//s.Format(L"Need to WriteCompressedData %d\n", i);
//OutputDebugString(s);
                wecWriteCompressedDataResult = WriteCompressedData(i, double(long long(dStart) - m_CompressionPoint[i].lCompressionTime), double(long long(dStart)));
            }
		}
	}

	//при переходе на новый час
	if ((tStart.GetYear() != m_tLastStartTime.GetYear()) ||
		(tStart.GetMonth() != m_tLastStartTime.GetMonth()) ||
		(tStart.GetDay() != m_tLastStartTime.GetDay()) ||
		(tStart.GetHour() != m_tLastStartTime.GetHour()))
	{
//s.Format(L"!!! New hour !!! " + CString(m_Channel.GetName()) + L"\n");
//OutputDebugString(s);
		//информируем родительские классы о смене часа
		m_bNewHour = true;

		//проверка директорий записи
		CString sCurrentDirectory;
		sCurrentDirectory.Format(L"%04d\\%02d\\%02d\\%02d", tStart.GetYear(), tStart.GetMonth(), tStart.GetDay(), tStart.GetHour());
		sCurrentDirectory.Insert(0, m_sSignalDirectory);
		//sCurrentDirectory.Format(m_sSignalDirectory + L"%04d\\%02d\\%02d\\%02d", tStart.GetYear(), tStart.GetMonth(), tStart.GetDay(), tStart.GetHour());
		if (m_sCurrentSignalDirectory != sCurrentDirectory)
		{
			if (m_hMap != NULL)
			{
				CloseHandle(m_hMap);
				m_hMap = NULL;
			}
			if (m_hFile != NULL)
			{
				CloseHandle(m_hFile);
				m_hFile = NULL;
			}
//s.Format(L"Another directory " + CString(m_Channel.GetName()) + L" from " + CString(m_sCurrentSignalDirectory) + L" to " + CString(sCurrentDirectory) + L"\n");
//OutputDebugString(s);
			m_sCurrentSignalDirectory = sCurrentDirectory;
		}
	}

	//если запись не производилась
	if (m_hFile == NULL)
	{
//s.Format(L"Need to write in new file " + CString(m_Channel.GetName()) + L"\n");
//OutputDebugString(s);
		//если с директорией все нормально
		if (m_FileHelper.CheckDirectory(m_sCurrentSignalDirectory) && m_pDescriptorFileHelper)
		{
			m_Channel.SetFrequency(dFrequency);
			//ищем файл-описатель
			CString sDescriptorFile = m_pDescriptorFileHelper->FindDescriptorFile(m_sCurrentSignalDirectory, m_Channel);
			//если не нашли файл-описатель
			if (sDescriptorFile.GetLength() == 0)
			{
				//создаем файл-описатель
				sDescriptorFile = m_pDescriptorFileHelper->CreateDescriptorFile(m_sCurrentSignalDirectory,
																				CTime(tStart.GetYear(),	tStart.GetMonth(), tStart.GetDay(), tStart.GetHour(), 0, 0),
																				m_Channel);
//s.Format(L"CreateDescriptorFile " + CString(m_Channel.GetName()) + L" " + CString(sDescriptorFile) + L"\n");
//OutputDebugString(s);
			}

			//проверяем частоту дискретизации
			double frequencyFromFile = m_pDescriptorFileHelper->GetFrequencyFromDescriptorFile(m_sCurrentSignalDirectory, sDescriptorFile);
			if (frequencyFromFile != 0.)
				m_Channel.SetFrequency(frequencyFromFile);
			
			//если файл-описатель есть
			if (1 == m_FileHelper.FileExists(sDescriptorFile))
			{
				//ищем файл данных
				CString sDataFile = sDescriptorFile;
				sDataFile.Replace(L".xml", L".ana");
				//если файла данных нету
				BOOL bDataFlag = TRUE;
				if (0 == m_FileHelper.FileExists(sDataFile))
				{
					//создаем файл данных
					bDataFlag = CreateDataFile(sDataFile, false);
//s.Format(L"CreateDataFile " + CString(m_Channel.GetName()) + L" " + CString(sDataFile) + L"\n");
//OutputDebugString(s);
				}
				////ищем файл лога
				//CString sLogFile = sDescriptorFile;
				//sLogFile.Replace(L".xml", L".log");
				////если файла лога нету
				//BOOL bLogFlag = TRUE;
				////создаем файл лога
				//if (!m_FileHelper.FileExists(sLogFile))
				//	bLogFlag = CreateLogFile(sLogFile);
				//если файл данных есть
				if (bDataFlag/* && bLogFlag*/)
				{
					CString sCommonDescriptorFile = m_pDescriptorFileHelper->FindCommonDescriptorFile(m_sCurrentSignalDirectory);
					if (sCommonDescriptorFile.GetLength() == 0)
					{
						sCommonDescriptorFile = m_pDescriptorFileHelper->CreateCommonDescriptorFile(m_sCurrentSignalDirectory, tStart);
//s.Format(L"CreateCommonDescriptorFile " + CString(m_Channel.GetName()) + L" " + CString(sCommonDescriptorFile) + L"\n");
//OutputDebugString(s);
					}

					if (1 == m_FileHelper.FileExists(sCommonDescriptorFile))
					{
						if (!m_pDescriptorFileHelper->FindChannelInCommonDescriptorFile(m_sCurrentSignalDirectory, m_Channel))
						{
							m_pDescriptorFileHelper->AppendCommonDescriptorFile(m_sCurrentSignalDirectory, sCommonDescriptorFile, m_Channel, sDescriptorFile, sDataFile/*, sLogFile*/);
//s.Format(L"AppendCommonDescriptorFile " + CString(m_Channel.GetName()) + L" " + CString(sCommonDescriptorFile) + L"\n");
//OutputDebugString(s);
						}
					}

					m_hFile = CreateFile(sDataFile, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
					if (INVALID_HANDLE_VALUE == m_hFile)
						m_hFile = NULL;
					if (NULL != m_hFile)
						m_hMap = CreateFileMapping(m_hFile, NULL, PAGE_READWRITE, 0x00000000, 0x00000000, NULL);
					if (NULL == m_hMap)
					{
						if (NULL != m_hFile)
						{
							CloseHandle(m_hFile);
							m_hFile = NULL;
						}
					}
//s.Format(L"CreateFileMapping " + CString(m_Channel.GetName()) + L" " + CString(sDataFile) + L"\n");
//OutputDebugString(s);
				}
			}
		}
	}

	if (m_hMap != NULL && m_hFile != NULL)
	{
		DWORD dwWritten(0);
		if (FALSE == WriteFile(m_hFile, L"Test", 0, &dwWritten, nullptr))
		{
			if (m_hMap != NULL)
			{
				CloseHandle(m_hMap);
				m_hMap = NULL;
			}
			if (m_hFile != NULL)
			{
				CloseHandle(m_hFile);
				m_hFile = NULL;
			}
		}
	}

	float* tmpData = nullptr;
	long tmpSize = 0;
	if (m_hMap != NULL && m_hFile != NULL)
	{
//OutputDebugString(L"Writing...\n");
		long lSeconds = tStart.GetMinute() * 60 + tStart.GetSecond();
		double dMilliseconds = round((dStart - CTimeSpan(tStart.GetTime()).GetTotalSeconds()) * 10000.) / 10000.;
		
		long long llFileOffset = long long((double(lSeconds) + dMilliseconds) * m_Channel.GetFrequency() + 0.5) * sizeof(float);
		long lPortion = sizeof(float) * lSize;
		if (llFileOffset % lPortion != 0)
			wecResult = wecIncorrectFileOffset;

		long long llTemp = llFileOffset / m_dwAllocationGranularity;
		llTemp *= m_dwAllocationGranularity;

		if (m_Channel.GetFrequency() != dFrequency)
		{
			if (m_pResamplingData == NULL)
			{
				m_pResamplingData = new float[MAX_SIZE];
				m_fMemoryPoint = pData[0];
			}
			long lRealSize(long((long long)(dFinish * m_Channel.GetFrequency() + 0.5) - (long long)(dStart * m_Channel.GetFrequency() + 0.5)));
			Resampling(pData, lSize, m_pResamplingData, lRealSize);
			tmpData = m_pResamplingData;
			tmpSize = lRealSize;
			//lSize = lRealSize;
			//ippsCopy_32f(m_pResamplingData, pData, lSize);
		}
		else
		{
			if (m_pResamplingData != NULL)
			{
				delete[] m_pResamplingData;
				m_pResamplingData = nullptr;
				m_fMemoryPoint = NAN;
			}
			tmpData = const_cast<float*>(pData);
			tmpSize = lSize;
		}
//TRACE(L"MapViewOfFile...\n");
		float* pView = reinterpret_cast<float*>(MapViewOfFile(m_hMap, FILE_MAP_WRITE, 0, DWORD(llTemp), SIZE_T(tmpSize * sizeof(float) + llFileOffset - llTemp)));
//TRACE(L"MapViewOfFile %d\n", pView);
		if (pView != NULL)
		{
			float fMin(NAN), fMax(NAN);
			int order(0);
			if (ippStsNanArg != ippsMaxOrder_32f(tmpData, tmpSize, &order))
				ippsMinMax_32f(tmpData, tmpSize, &fMin, &fMax);
			for (int i = 0; i < NUMBER_OF_COMPRESSION; ++i)
			{
				CheckMinimum(m_CompressionPoint[i].fMin, fMin);
				CheckMaximum(m_CompressionPoint[i].fMax, fMax);
			}
//            TRACE(L"Copying... %d\n", (llFileOffset - llTemp) / sizeof(float));
			if (ippsCopy_32f(tmpData, &pView[(llFileOffset - llTemp) / sizeof(float)], tmpSize) != ippStsNoErr)
				wecResult = wecWriteFileError;
//TRACE(L"End Copying...\n");
//TRACE("Write signal " + CStringA(m_Channel.GetName()) + " to directory " + CStringA(m_sCurrentSignalDirectory) + " position %d, size %d (from %lf to %lf)\n", long(llFileOffset), lSize, dStart, dFinish);
            FlushViewOfFile(&pView[(llFileOffset - llTemp) / sizeof(float)], tmpSize);
			if (UnmapViewOfFile(pView) == FALSE)
				wecResult = wecUnmapViewOfFileError;
		}
		else
			wecResult = wecMapViewOfFileError;
//s.Format(L"End Writing... %d\n", wecResult);
//OutputDebugString(s);
	}
	else
	{
		if (m_hMap == NULL)
		{
			wecResult = wecCreateFileMappingError;
			if (m_hFile == NULL)
				wecResult = wecCreateFileAndFileMappingError;
		}
		else
		{
			wecResult = wecCreateFileError;
		}
	}
	m_tLastStartTime = tStart;
	SetEvent(m_hEvent);

	return WRITER_ERROR_CODE(wecResult + (wecWriteCompressedDataResult << 8));
}

WRITER_ERROR_CODE CStreamTrendWriter::WriteCompressedData(long lNumber, double dStart, double dFinish)
{
	WRITER_ERROR_CODE wecResult(wecSuccessful);

	CTime tStart;
	tStart = long long(dStart);
	CTime tFinish;
	tFinish = long long(dFinish);
	CTime tMonthStart = CTime(tStart.GetYear(), tStart.GetMonth(), 1, 0, 0, 0);
	CTime tMonthEnd;
	if (tStart.GetMonth() == 12)
		tMonthEnd = CTime(tStart.GetYear() + 1, 1, 1, 0, 0, 0);
	else
		tMonthEnd = CTime(tStart.GetYear(), tStart.GetMonth() + 1, 1, 0, 0, 0);
	m_lDaysInCurrentMonth = long(CTimeSpan(tMonthEnd - tMonthStart).GetDays());

	BOOL bDirectoryFlag = TRUE;

	CTime tLastTime = m_tLastStartTime - CTimeSpan(1);
	//переход на новый месяц
	if ((tLastTime.GetYear() != tStart.GetYear()) ||
		(tLastTime.GetMonth() != tStart.GetMonth()))
	{
		//проверка директорий записи
		CString sCurrentDirectory;
		sCurrentDirectory.Format(L"%04d\\%02d", tStart.GetYear(), tStart.GetMonth());
		sCurrentDirectory.Insert(0, m_sCompressedDirectory);
		//sCurrentDirectory.Format(m_sCompressedDirectory + L"%04d\\%02d", tStart.GetYear(), tStart.GetMonth());
		if (m_sCurrentCompressedDirectory != sCurrentDirectory)
		{
			m_sCurrentCompressedDirectory = sCurrentDirectory;
			bDirectoryFlag = m_FileHelper.CheckDirectory(m_sCurrentCompressedDirectory);
			m_sCurrentDescriptorFile.Empty();
		}
	}

	//если с директорией все нормально
	if (bDirectoryFlag)
	{
		//если нет текущего файла-описателя
		if (m_sCurrentDescriptorFile.GetLength() == 0 && m_pDescriptorFileHelper)
		{
			//ищем файл-описатель
			m_sCurrentDescriptorFile = m_pDescriptorFileHelper->FindDescriptorFile(m_sCurrentCompressedDirectory, m_Channel);
			//если нет файла-описателя
			if (m_sCurrentDescriptorFile.GetLength() == 0)
				//создаем файл-описатель
				m_sCurrentDescriptorFile = m_pDescriptorFileHelper->CreateDescriptorFile(m_sCurrentCompressedDirectory, tMonthStart, m_Channel);

			CString sCommonCompressedDescriptorFile = m_pDescriptorFileHelper->FindCommonCompressedDescriptorFile(m_sCurrentCompressedDirectory);
			if (sCommonCompressedDescriptorFile.GetLength() == 0)
				sCommonCompressedDescriptorFile = m_pDescriptorFileHelper->CreateCommonCompressedDescriptorFile(m_sCurrentCompressedDirectory, tMonthStart);
			if (1 == m_FileHelper.FileExists(sCommonCompressedDescriptorFile))
			{
				if (!m_pDescriptorFileHelper->FindChannelInCommonCompressedDescriptorFile(m_sCurrentCompressedDirectory, m_Channel))
				{
					CString sDataFile = m_sCurrentDescriptorFile;
					sDataFile.Replace(L".xml", L"");
					CString sLogFile = m_sCurrentDescriptorFile;
					sLogFile.Replace(L".xml", L"");
					m_pDescriptorFileHelper->AppendCommonCompressedDescriptorFile(m_sCurrentCompressedDirectory, sCommonCompressedDescriptorFile, m_Channel, m_sCurrentDescriptorFile, sDataFile, sLogFile);
				}
			}
		}

		if (m_sCurrentDescriptorFile.GetLength() != 0)
		{
			//смотрим файл данных
			CString sDataFile = m_sCurrentDescriptorFile;
			sDataFile.Replace(L".xml", m_CompressionPoint[lNumber].sExtention);
			//если файла данных нету
			BOOL bDataFileFlag = TRUE;
			if (0 == m_FileHelper.FileExists(sDataFile))
				//создаем файл данных
				bDataFileFlag = CreateCompressedDataFile(sDataFile, lNumber);
			if (bDataFileFlag)
			{
				HANDLE hFile = CreateFile(sDataFile, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				if (INVALID_HANDLE_VALUE != hFile)
				{
					HANDLE hMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0x00000000, 0x00000000, NULL);
					if (hMap != NULL)
					{
						long long llFileOffset = long long(CTimeSpan(tStart - tMonthStart).GetTotalSeconds() / m_CompressionPoint[lNumber].lCompressionTime) * 2 * sizeof(float);
						long long llTemp = llFileOffset / m_dwAllocationGranularity;
						llTemp *= m_dwAllocationGranularity;

						float* pView = reinterpret_cast<float*>(MapViewOfFile(hMap, FILE_MAP_WRITE, 0, DWORD(llTemp), SIZE_T(2 * sizeof(float) + llFileOffset - llTemp)));
						if (nullptr != pView)
						{
							CheckMinimum(pView[(llFileOffset - llTemp) / sizeof(float)], m_CompressionPoint[lNumber].fMin);
							m_CompressionPoint[lNumber].fMin = 0.f;// NAN;
							CheckMaximum(pView[(llFileOffset - llTemp) / sizeof(float) + 1], m_CompressionPoint[lNumber].fMax);
							m_CompressionPoint[lNumber].fMax = 0.f;// NAN;

//TRACE("WriteCompressedData %d " + CStringA(m_Channel.GetName()) + " to directory " + CStringA(m_sCurrentCompressedDirectory) + " position %d (from %lf to %lf) %f %f\n", lNumber, long(llFileOffset), dStart, dFinish, pView[(llFileOffset - llTemp) / sizeof(float)], pView[(llFileOffset - llTemp) / sizeof(float) + 1]);

							if (UnmapViewOfFile(pView) == FALSE)
								wecResult = wecUnmapViewOfFileError;
						}
						else
							wecResult = wecMapViewOfFileError;
						CloseHandle(hMap);
					}
					else
						wecResult = wecCreateFileMappingError;
					CloseHandle(hFile);
				}
				else
					wecResult = wecCreateFileError;
			}
			else
				wecResult = wecDataFileError;
		}
		else
			wecResult = wecDescriptorFileError;
	}
	else
		wecResult = wecDirectoryError;

	return wecResult;
}

bool CStreamTrendWriter::DeleteOldRecords()
{
	long lBeginYear(-1);
	FindMinMaxDirectory(m_sSignalDirectory, &lBeginYear, nullptr);
	if (lBeginYear > 0)
	{
		CString sDirectory;
		sDirectory.Format(L"%04d\\", lBeginYear);
		long lBeginMonth(-1);
		FindMinMaxDirectory(m_sSignalDirectory + sDirectory, &lBeginMonth, nullptr);
		if (lBeginMonth > 0)
		{
			sDirectory.AppendFormat(L"%02d\\", lBeginMonth);
			long lBeginDay(-1);
			FindMinMaxDirectory(m_sSignalDirectory + sDirectory, &lBeginDay, nullptr);
			if (lBeginDay > 0)
			{
				sDirectory.AppendFormat(L"%02d\\", lBeginDay);
				long lBeginHour(-1);
				FindMinMaxDirectory(m_sSignalDirectory + sDirectory, &lBeginHour, nullptr);
				if (lBeginHour >= 0)
				{
					sDirectory.AppendFormat(L"%02d\\", lBeginHour);
					
					WIN32_FIND_DATA win32_find_data;
					WCHAR wcDirectory[MAX_PATH] = { 0 };
					wcscpy_s(wcDirectory, (m_sSignalDirectory + sDirectory));
					wcscat_s(wcDirectory, L"*.*");
					HANDLE hFind = FindFirstFile(wcDirectory, &win32_find_data);
					if (hFind != INVALID_HANDLE_VALUE)
					{
						FindNextFile(hFind, &win32_find_data);
						while (FindNextFile(hFind, &win32_find_data) != 0)
						{
							if ((win32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
							{
								WCHAR wcFileName[MAX_PATH] = {0};
								wcscpy_s(wcFileName, (m_sSignalDirectory + sDirectory));
								wcscat_s(wcFileName, win32_find_data.cFileName);
								if (FALSE == DeleteFile(wcFileName))
									break;
							}
						}
						FindClose(hFind);
					}
					if (FALSE == RemoveDirectory(m_sSignalDirectory + sDirectory))
						return false;
				}
				else
				{
					if (FALSE == RemoveDirectory(m_sSignalDirectory + sDirectory))
						return false;
					DeleteOldRecords();
				}
			}
			else
			{
				if (FALSE == RemoveDirectory(m_sSignalDirectory + sDirectory))
					return false;
				DeleteOldRecords();
			}
		}
		else
		{
			if (FALSE == RemoveDirectory(m_sSignalDirectory + sDirectory))
				return false;
			DeleteOldRecords();
		}
	}
	else
		return false;

	return true;
}
