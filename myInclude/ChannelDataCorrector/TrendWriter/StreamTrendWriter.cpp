#include "StdAfx.h"
#include <algorithm>
#include "StreamTrendWriter.h"

#include <Intel\ipp\ipp.h>

CStreamTrendWriter::CStreamTrendWriter()
	: m_hEvent(NULL)
{
	m_hEvent = CreateEvent(NULL, TRUE, TRUE, L"WriteDataEvent");
}

CStreamTrendWriter::CStreamTrendWriter(const CStreamTrendWriter & writer)
	: CTrendWriter(writer)
{
	m_hEvent = CreateEvent(NULL, TRUE, TRUE, L"WriteDataEvent");
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

//s.Format(L"WriteData %d\n", m_Channel->GetNumber());
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

	/*if (!m_bTestMode)
	{
		CTime tCurrent = CTime::GetCurrentTime();
		CTimeSpan tsDelta = tCurrent - tFinish;
		long long llDelta = tsDelta.GetTotalSeconds();
		if (abs(llDelta) > 3600)
		{
			SetEvent(m_hEvent);
			return wecLargeTimeDifference;
		}
	}*/

//s.Format(L"WriteData: dStartMS %.10lf, dStartMSEx %.10lf\n", dStartMS, dStartMSEx);
//OutputDebugString(s);
	//если начало секунды, надо проверить переход через границу временного интервала сжатия
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

	//при переходе на новый час
	if ((tStart.GetYear() != m_tLastStartTime.GetYear()) ||
		(tStart.GetMonth() != m_tLastStartTime.GetMonth()) ||
		(tStart.GetDay() != m_tLastStartTime.GetDay()) ||
		(tStart.GetHour() != m_tLastStartTime.GetHour()))
	{
//s.Format(L"!!! New hour !!! " + CString(m_Channel->GetName()) + L"\n");
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
//s.Format(L"Another directory " + CString(m_Channel->GetName()) + L" from " + CString(m_sCurrentSignalDirectory) + L" to " + CString(sCurrentDirectory) + L"\n");
//OutputDebugString(s);
			m_sCurrentSignalDirectory = sCurrentDirectory;
		}
	}

	//если запись не производилась
	if (m_hFile == NULL)
	{
//s.Format(L"Need to write in new file " + CString(m_Channel->GetName()) + L"\n");
//OutputDebugString(s);
		//если с директорией все нормально
		if (m_FileHelper.CheckDirectory(m_sCurrentSignalDirectory) && m_pDescriptorFileHelper)
		{
			m_Channel->SetFrequency(dFrequency);
			//ищем файл-описатель
			CString sDescriptorFile = m_pDescriptorFileHelper->FindDescriptorFile(m_sCurrentSignalDirectory, m_Channel.get());
			//если не нашли файл-описатель
			if (sDescriptorFile.GetLength() == 0)
			{
				//создаем файл-описатель
				sDescriptorFile = m_pDescriptorFileHelper->CreateDescriptorFile(m_sCurrentSignalDirectory,
																				CTime(tStart.GetYear(),	tStart.GetMonth(), tStart.GetDay(), tStart.GetHour(), 0, 0),
																				m_Channel.get());
//s.Format(L"CreateDescriptorFile " + CString(m_Channel->GetName()) + L" " + CString(sDescriptorFile) + L"\n");
//OutputDebugString(s);
			}

			//проверяем частоту дискретизации
			double frequencyFromFile = m_pDescriptorFileHelper->GetFrequencyFromDescriptorFile(m_sCurrentSignalDirectory, sDescriptorFile);
			if (frequencyFromFile != 0.)
				m_Channel->SetFrequency(frequencyFromFile);
			
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
//s.Format(L"CreateDataFile " + CString(m_Channel->GetName()) + L" " + CString(sDataFile) + L"\n");
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
//s.Format(L"CreateCommonDescriptorFile " + CString(m_Channel->GetName()) + L" " + CString(sCommonDescriptorFile) + L"\n");
//OutputDebugString(s);
					}

					if (1 == m_FileHelper.FileExists(sCommonDescriptorFile))
					{
						if (!m_pDescriptorFileHelper->FindChannelInCommonDescriptorFile(m_sCurrentSignalDirectory, m_Channel.get()))
						{
							m_pDescriptorFileHelper->AppendCommonDescriptorFile(m_sCurrentSignalDirectory, sCommonDescriptorFile, m_Channel.get(), sDescriptorFile, sDataFile/*, sLogFile*/);
//s.Format(L"AppendCommonDescriptorFile " + CString(m_Channel->GetName()) + L" " + CString(sCommonDescriptorFile) + L"\n");
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
//s.Format(L"CreateFileMapping " + CString(m_Channel->GetName()) + L" " + CString(sDataFile) + L"\n");
//OutputDebugString(s);
				}
			}
		}
	}

	//if (m_hMap != NULL && m_hFile != NULL)
	//{
	//	DWORD dwWritten(0);
	//	if (FALSE == WriteFile(m_hFile, L"Test", 0, &dwWritten, nullptr))
	//	{
	//		if (m_hMap != NULL)
	//		{
	//			CloseHandle(m_hMap);
	//			m_hMap = NULL;
	//		}
	//		if (m_hFile != NULL)
	//		{
	//			CloseHandle(m_hFile);
	//			m_hFile = NULL;
	//		}
	//	}
	//}

	float* tmpData = nullptr;
	long tmpSize = 0;
	if (m_hMap != NULL && m_hFile != NULL)
	{
//OutputDebugString(L"Writing...\n");
		long lSeconds = tStart.GetMinute() * 60 + tStart.GetSecond();
		double dMilliseconds = round((dStart - CTimeSpan(tStart.GetTime()).GetTotalSeconds()) * 10000.) / 10000.;
		
		long long llFileOffset = long long((double(lSeconds) + dMilliseconds) * m_Channel->GetFrequency() + 0.5) * sizeof(float);
		long lPortion = sizeof(float) * lSize;
		if (llFileOffset % lPortion != 0)
			wecResult = wecIncorrectFileOffset;

		long long llTemp = llFileOffset / m_dwAllocationGranularity;
		llTemp *= m_dwAllocationGranularity;

		if (m_Channel->GetFrequency() != dFrequency)
		{
			if (m_pResamplingData == NULL)
			{
				m_pResamplingData = new float[MAX_SIZE];
				m_fMemoryPoint = pData[0];
			}
			long lRealSize(long((long long)(dFinish * m_Channel->GetFrequency() + 0.5) - (long long)(dStart * m_Channel->GetFrequency() + 0.5)));
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
//TRACE("Write signal " + CStringA(m_Channel->GetName()) + " to directory " + CStringA(m_sCurrentSignalDirectory) + " position %d, size %d (from %lf to %lf)\n", long(llFileOffset), lSize, dStart, dFinish);
            FlushViewOfFile(&pView[(llFileOffset - llTemp) / sizeof(float)], tmpSize);
			if (UnmapViewOfFile(pView) == FALSE)
				wecResult = wecUnmapViewOfFileError;
		}
		else
		{
			DWORD error = GetLastError();
			wecResult = wecMapViewOfFileError;
		}
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
			m_sCurrentDescriptorFile = m_pDescriptorFileHelper->FindDescriptorFile(m_sCurrentCompressedDirectory, m_Channel.get());
			//если нет файла-описателя
			if (m_sCurrentDescriptorFile.GetLength() == 0)
				//создаем файл-описатель
				m_sCurrentDescriptorFile = m_pDescriptorFileHelper->CreateDescriptorFile(m_sCurrentCompressedDirectory, tMonthStart, m_Channel.get());

			CString sCommonCompressedDescriptorFile = m_pDescriptorFileHelper->FindCommonCompressedDescriptorFile(m_sCurrentCompressedDirectory);
			if (sCommonCompressedDescriptorFile.GetLength() == 0)
				sCommonCompressedDescriptorFile = m_pDescriptorFileHelper->CreateCommonCompressedDescriptorFile(m_sCurrentCompressedDirectory, tMonthStart);
			if (1 == m_FileHelper.FileExists(sCommonCompressedDescriptorFile))
			{
				if (!m_pDescriptorFileHelper->FindChannelInCommonCompressedDescriptorFile(m_sCurrentCompressedDirectory, m_Channel.get()))
				{
					CString sDataFile = m_sCurrentDescriptorFile;
					sDataFile.Replace(L".xml", L"");
					CString sLogFile = m_sCurrentDescriptorFile;
					sLogFile.Replace(L".xml", L"");
					m_pDescriptorFileHelper->AppendCommonCompressedDescriptorFile(m_sCurrentCompressedDirectory, sCommonCompressedDescriptorFile, m_Channel.get(), m_sCurrentDescriptorFile, sDataFile, sLogFile);
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
							pView[(llFileOffset - llTemp) / sizeof(float)] = m_CompressionPoint[lNumber].fMin;
							pView[(llFileOffset - llTemp) / sizeof(float) + 1] = m_CompressionPoint[lNumber].fMax;
							//CheckMinimum(pView[(llFileOffset - llTemp) / sizeof(float)], m_CompressionPoint[lNumber].fMin);
							m_CompressionPoint[lNumber].fMin = 0.f;// NAN;
							//CheckMaximum(pView[(llFileOffset - llTemp) / sizeof(float) + 1], m_CompressionPoint[lNumber].fMax);
							m_CompressionPoint[lNumber].fMax = 0.f;// NAN;

//TRACE("WriteCompressedData %d " + CStringA(m_Channel->GetName()) + " to directory " + CStringA(m_sCurrentCompressedDirectory) + " position %d (from %lf to %lf) %f %f\n", lNumber, long(llFileOffset), dStart, dFinish, pView[(llFileOffset - llTemp) / sizeof(float)], pView[(llFileOffset - llTemp) / sizeof(float) + 1]);

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

/*
WRITER_ERROR_CODE CStreamTrendWriter::ReadData(float* pData, long lSize, double dFrequency, CTime startTime, CTime finishTime)
{
	//CString s;
	if (lSize <= 0)
		return wecIncorrectDataSize;

	if (pData == NULL)
		return wecNullWriteData;

	WRITER_ERROR_CODE wecWriteCompressedDataResult(wecSuccessful), wecResult(wecSuccessful);

	WaitForSingleObject(m_hEvent, INFINITE);
	ResetEvent(m_hEvent);

	SYSTEMTIME st;
	startTime.GetAsSystemTime(st);
	st.wMinute = 0;
	st.wSecond = 0;

	// выравниваем по часу
	CTime endReadDataTime = st;
	endReadDataTime += 3600;

	double dFinish = (double)CTimeSpan(finishTime.GetTime()).GetTotalSeconds();
	
	double curReadTime = (double)CTimeSpan(startTime.GetTime()).GetTotalSeconds();
	double endReadTime = (double)CTimeSpan(endReadDataTime.GetTime()).GetTotalSeconds();
	endReadTime = std::min<double>(endReadTime + 3600., dFinish);

	long indexShift = 0;
	while (curReadTime < endReadTime)
	{
		lSize = endReadTime - curReadTime;

		double dDelta = double(lSize) / dFrequency / 100.;   // Принудительная запись трендов сигналов
		CTime tStart = long long(curReadTime);
		double dStartMS = curReadTime - long long(curReadTime);
		double dStartMSEx = 1. - dStartMS;
		if (abs(dStartMSEx) <= dDelta)
			tStart += 1;
		CTime tFinish = long long(dFinish);
		double dFinishMS = dFinish - long long(dFinish);
		double dFinishMSEx = 1. - dFinishMS;
		if (abs(dFinishMSEx) <= dDelta)
			tFinish += 1;

		//при переходе на новый час
		if ((tStart.GetYear() != m_tLastStartTime.GetYear()) ||
			(tStart.GetMonth() != m_tLastStartTime.GetMonth()) ||
			(tStart.GetDay() != m_tLastStartTime.GetDay()) ||
			(tStart.GetHour() != m_tLastStartTime.GetHour()))
		{
			//s.Format(L"!!! New hour !!! " + CString(m_Channel->GetName()) + L"\n");
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
				//s.Format(L"Another directory " + CString(m_Channel->GetName()) + L" from " + CString(m_sCurrentSignalDirectory) + L" to " + CString(sCurrentDirectory) + L"\n");
				//OutputDebugString(s);
				m_sCurrentSignalDirectory = sCurrentDirectory;
			}
		}

		//если запись не производилась
		if (m_hFile == NULL)
		{
			//s.Format(L"Need to write in new file " + CString(m_Channel->GetName()) + L"\n");
			//OutputDebugString(s);
			//если с директорией все нормально
			if (m_FileHelper.CheckDirectory(m_sCurrentSignalDirectory) && m_pDescriptorFileHelper)
			{
				m_Channel->SetFrequency(dFrequency);
				//ищем файл-описатель
				CString sDescriptorFile = m_pDescriptorFileHelper->FindDescriptorFile(m_sCurrentSignalDirectory, m_Channel.get());
				//если не нашли файл-описатель
				if (sDescriptorFile.GetLength() == 0)
				{
					//создаем файл-описатель
					sDescriptorFile = m_pDescriptorFileHelper->CreateDescriptorFile(m_sCurrentSignalDirectory,
																					CTime(tStart.GetYear(), tStart.GetMonth(), tStart.GetDay(), tStart.GetHour(), 0, 0),
																					m_Channel.get());
					//s.Format(L"CreateDescriptorFile " + CString(m_Channel->GetName()) + L" " + CString(sDescriptorFile) + L"\n");
					//OutputDebugString(s);
				}

				//проверяем частоту дискретизации
				double frequencyFromFile = m_pDescriptorFileHelper->GetFrequencyFromDescriptorFile(m_sCurrentSignalDirectory, sDescriptorFile);
				if (frequencyFromFile != 0.)
					m_Channel->SetFrequency(frequencyFromFile);

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

						// файла нету, данных тоже, пропускаем
						m_tLastStartTime = tStart;
						curReadTime += 3600;
						endReadTime = std::min<double>(endReadTime + 3600., dFinish);
						indexShift += 3600 * dFrequency;
						continue;

						//s.Format(L"CreateDataFile " + CString(m_Channel->GetName()) + L" " + CString(sDataFile) + L"\n");
						//OutputDebugString(s);
					}

					//если файл данных есть
					if (bDataFlag/ * && bLogFlag* /)
					{
						CString sCommonDescriptorFile = m_pDescriptorFileHelper->FindCommonDescriptorFile(m_sCurrentSignalDirectory);
						if (sCommonDescriptorFile.GetLength() == 0)
						{
							sCommonDescriptorFile = m_pDescriptorFileHelper->CreateCommonDescriptorFile(m_sCurrentSignalDirectory, tStart);
							//s.Format(L"CreateCommonDescriptorFile " + CString(m_Channel->GetName()) + L" " + CString(sCommonDescriptorFile) + L"\n");
							//OutputDebugString(s);
						}

						if (1 == m_FileHelper.FileExists(sCommonDescriptorFile))
						{
							if (!m_pDescriptorFileHelper->FindChannelInCommonDescriptorFile(m_sCurrentSignalDirectory, m_Channel.get()))
							{
								m_pDescriptorFileHelper->AppendCommonDescriptorFile(m_sCurrentSignalDirectory, sCommonDescriptorFile, m_Channel.get(), sDescriptorFile, sDataFile/ *, sLogFile* /);
								//s.Format(L"AppendCommonDescriptorFile " + CString(m_Channel->GetName()) + L" " + CString(sCommonDescriptorFile) + L"\n");
								//OutputDebugString(s);
							}
						}
						m_hFile = CreateFile(sDataFile, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
						if (INVALID_HANDLE_VALUE == m_hFile)
							m_hFile = NULL;
						if (NULL != m_hFile)
							m_hMap = CreateFileMapping(m_hFile, NULL, PAGE_READONLY, 0x00000000, 0x00000000, NULL);
						if (NULL == m_hMap)
						{
							if (NULL != m_hFile)
							{
								CloseHandle(m_hFile);
								m_hFile = NULL;
							}
						}
						//s.Format(L"CreateFileMapping " + CString(m_Channel->GetName()) + L" " + CString(sDataFile) + L"\n");
						//OutputDebugString(s);
					}
				}
			}
		}

		float* tmpData = nullptr;
		long tmpSize = 0;
		if (m_hMap != NULL && m_hFile != NULL)
		{
			//OutputDebugString(L"Writing...\n");
			long lSeconds = tStart.GetMinute() * 60 + tStart.GetSecond();
			double dMilliseconds = round((curReadTime - CTimeSpan(tStart.GetTime()).GetTotalSeconds()) * 10000.) / 10000.;

			long long llFileOffset = long long((double(lSeconds) + dMilliseconds) * m_Channel->GetFrequency() + 0.5) * sizeof(float);
			long lPortion = sizeof(float) * lSize;
			if (llFileOffset % lPortion != 0)
				wecResult = wecIncorrectFileOffset;

			long long llTemp = llFileOffset / m_dwAllocationGranularity;
			llTemp *= m_dwAllocationGranularity;

			if (m_Channel->GetFrequency() != dFrequency)
			{
				if (m_pResamplingData == NULL)
				{
					m_pResamplingData = new float[MAX_SIZE];
					m_fMemoryPoint = pData[0];
				}
				long lRealSize(long((long long)(dFinish * m_Channel->GetFrequency() + 0.5) - (long long)(curReadTime * m_Channel->GetFrequency() + 0.5)));
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
			float* pView = reinterpret_cast<float*>(MapViewOfFile(m_hMap, FILE_MAP_READ, 0, DWORD(llTemp), SIZE_T(tmpSize * sizeof(float) + llFileOffset - llTemp)));
			//TRACE(L"MapViewOfFile %d\n", pView);
			if (pView != NULL)
			{
				//            TRACE(L"Copying... %d\n", (llFileOffset - llTemp) / sizeof(float));
				if (ippsCopy_32f(&pView[(llFileOffset - llTemp) / sizeof(float)], &pData[indexShift], tmpSize) != ippStsNoErr)
					wecResult = wecWriteFileError;
				//FlushViewOfFile(&pView[(llFileOffset - llTemp) / sizeof(float)], tmpSize);
				if (UnmapViewOfFile(pView) == FALSE)
					wecResult = wecUnmapViewOfFileError;
			}
			else
			{
				DWORD error = GetLastError();
				wecResult = wecMapViewOfFileError;
			}
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


		curReadTime += 3600;
		endReadTime = std::min<double>(endReadTime + 3600., dFinish);
		indexShift += 3600 * dFrequency;
	}
	m_tLastStartTime = 0;
	SetEvent(m_hEvent);

	return WRITER_ERROR_CODE(wecResult + (wecWriteCompressedDataResult << 8));
}*/
