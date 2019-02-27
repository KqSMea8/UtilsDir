#include "StdAfx.h"
#include "SingleTrendWriter.h"

#pragma comment(lib, "Intel\\ipp\\ia32\\zetipp.lib")
#include <Intel\ipp\ipp.h>

CSingleTrendWriter::CSingleTrendWriter()
{
	m_wtType = wtTrendSingle;
}

CSingleTrendWriter::~CSingleTrendWriter()
{

}

WRITER_ERROR_CODE CSingleTrendWriter::WriteData(const float* pData, long lSize, double dFrequency, double dStart, double dFinish)
{
	if (dStart <= 0)
		return wecInvalidTime;
	if (dFinish <= 0)
		return wecInvalidTime;
	CTime tStart = long long(dStart);
	CTime tFinish = long long(dFinish);
	long lTotalHour = long(CTimeSpan(tFinish - tStart).GetTotalHours() + 1);
	
	CTime tTempStart = tStart;
	double dTempStart(dStart);
	long lOffset(0);
	for (int i = 0; i < lTotalHour; ++i)
	{
		CTime tTempFinish = tTempStart + 3600;
		tTempFinish -= (tTempFinish.GetMinute() * 60 + tTempFinish.GetSecond());
		double dTempFinish = double(CTimeSpan(tTempFinish.GetTime()).GetTotalSeconds());
		if (dTempFinish > dFinish)
			dTempFinish = dFinish;

		//при переходе на новый час
		if ((tTempStart.GetYear() != m_tLastStartTime.GetYear()) ||
			(tTempStart.GetMonth() != m_tLastStartTime.GetMonth()) ||
			(tTempStart.GetDay() != m_tLastStartTime.GetDay()) ||
			(tTempStart.GetHour() != m_tLastStartTime.GetHour()))
		{
			//информируем родительские классы о смене часа
			m_bNewHour = true;

			//проверка директорий записи
			CString sCurrentDirectory(L"");
			sCurrentDirectory.Format(L"%04d\\%02d\\%02d\\%02d", tTempStart.GetYear(), tTempStart.GetMonth(), tTempStart.GetDay(), tTempStart.GetHour());
			sCurrentDirectory.Insert(0, m_sSignalDirectory);
			//sCurrentDirectory.Format(m_sSignalDirectory + L"%04d\\%02d\\%02d\\%02d", tTempStart.GetYear(), tTempStart.GetMonth(), tTempStart.GetDay(), tTempStart.GetHour());
			if (m_sCurrentSignalDirectory != sCurrentDirectory)
			{
				if (m_hFile != NULL)
				{
					CloseHandle(m_hFile);
					m_hFile = NULL;
				}
				m_sCurrentSignalDirectory = sCurrentDirectory;
			}
		}

		//если запись не производилась
		if (m_hFile == NULL)
		{
			//если с директорией все нормально
			if (m_FileHelper.CheckDirectory(m_sCurrentSignalDirectory))
			{
				//ищем файл-описатель
				CString sDescriptorFile = m_pDescriptorFileHelper->FindDescriptorFile(m_sCurrentSignalDirectory, m_Channel);
				//если не нашли файл-описатель
				if (sDescriptorFile.GetLength() == 0)
					//создаем файл-описатель
					sDescriptorFile = m_pDescriptorFileHelper->CreateDescriptorFile(m_sCurrentSignalDirectory,
																					CTime(tTempStart.GetYear(), tTempStart.GetMonth(), tTempStart.GetDay(), tTempStart.GetHour(), 0, 0),
																					m_Channel);
				//провер€ем частоту дискретизации
				double dFrequency = m_pDescriptorFileHelper->GetFrequencyFromDescriptorFile(m_sCurrentSignalDirectory, sDescriptorFile);

				//if (dFrequency != m_Channel.GetFrequency())
				//	m_Channel.SetFrequency(dFrequency);

				//если файл-описатель есть
				if (1 == m_FileHelper.FileExists(sDescriptorFile))
				{
					//ищем файл данных
					CString sDataFile = sDescriptorFile;
					sDataFile.Replace(L".xml", L".ana");
					//если файла данных нету
					BOOL bDataFlag = TRUE;
					if (0 == m_FileHelper.FileExists(sDataFile))
						//создаем файл данных
						bDataFlag = CreateDataFile(sDataFile, false);
					//если файл данных есть
					if (bDataFlag)
					{
						CString sCommonDescriptorFile = m_pDescriptorFileHelper->FindCommonDescriptorFile(m_sCurrentSignalDirectory);
						if (sCommonDescriptorFile.GetLength() == 0)
							sCommonDescriptorFile = m_pDescriptorFileHelper->CreateCommonDescriptorFile(m_sCurrentSignalDirectory, tTempStart);
						if (1 == m_FileHelper.FileExists(sCommonDescriptorFile))
						{
							if (!m_pDescriptorFileHelper->FindChannelInCommonDescriptorFile(m_sCurrentSignalDirectory, m_Channel))
								m_pDescriptorFileHelper->AppendCommonDescriptorFile(m_sCurrentSignalDirectory, sCommonDescriptorFile, m_Channel, sDescriptorFile, sDataFile/*, sLogFile*/);
						}

						m_hFile = CreateFile(sDataFile, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
						if (INVALID_HANDLE_VALUE == m_hFile)
							m_hFile = NULL;
						if (NULL != m_hFile)
							m_hMap = CreateFileMapping(m_hFile, NULL, PAGE_READWRITE, 0x00000000, 0x00000000, NULL);
					}
				}
			}
		}

		float* tmpData = nullptr;
		long tmpSize = 0;
		if (m_hMap != NULL && m_hFile != NULL && lSize > 0)
		{
			long long llFileOffset = long long(m_Channel.GetFrequency() * (double(tTempStart.GetMinute()) * 60. + double(tTempStart.GetSecond()) + dTempStart - double(CTimeSpan(tTempStart.GetTime()).GetTotalSeconds())) + 0.5) * sizeof(float);
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
			long lTempSize = long((dTempFinish - dTempStart) * m_Channel.GetFrequency());
			float* pView = reinterpret_cast<float*>(MapViewOfFile(m_hMap, FILE_MAP_WRITE, 0, DWORD(llTemp), SIZE_T(lTempSize * sizeof(float) + llFileOffset - llTemp)));
			//TRACE(L"Try to map at %d size %d\n", long(llTemp), long(lTempSize * sizeof(float) + llFileOffset - llTemp));
			if (pView != NULL)
			{
				ippsCopy_32f(pData + lOffset, &pView[(llFileOffset - llTemp) / sizeof(float)], lTempSize);
				//TRACE("Write signal " + CStringA(m_Channel.GetName()) + " to directory " + CStringA(m_sCurrentSignalDirectory) + " position %d, size %d (from %lf to %lf)\n", long(llFileOffset), lTempSize, dTempStart, dTempFinish);
				UnmapViewOfFile(pView);

				WriteCompressedData(tmpData + lOffset, lTempSize, dFrequency, dTempStart, dTempFinish);
			}
			lOffset += lTempSize;
		}
		else
		{
			if (m_hMap == NULL)
				return wecCreateFileMappingError;
			if (m_hFile == NULL)
				return wecCreateFileError;
			if (lSize <= 0)
				return wecIncorrectDataSize;
		}
		m_tLastStartTime = tTempStart;
		tTempStart = tTempFinish;
		dTempStart = double(CTimeSpan(tTempStart.GetTime()).GetTotalSeconds());
	}

	return wecSuccessful;
}

BOOL CSingleTrendWriter::WriteCompressedData(float* pData, long lSize, double dFrequency, double dStart, double dFinish)
{
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
	//переход на новый мес€ц
	if ((m_tLastStartTime.GetYear() != tStart.GetYear()) ||
		(m_tLastStartTime.GetMonth() != tStart.GetMonth()))
	{
		//проверка директорий записи
		CString sCurrentDirectory(L"");
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
		//если нет текущего файла-описател€
		if (m_sCurrentDescriptorFile.GetLength() == 0)
		{
			//ищем файл-описатель
			m_sCurrentDescriptorFile = m_pDescriptorFileHelper->FindDescriptorFile(m_sCurrentCompressedDirectory, m_Channel);
			//если нет файла-описател€
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
			for (int i = 0; i < NUMBER_OF_COMPRESSION; ++i)
			{
				//смотрим файл данных
				CString sDataFile = m_sCurrentDescriptorFile;
				sDataFile.Replace(L".xml", m_CompressionPoint[i].sExtention);
				//если файла данных нету
				BOOL bDataFileFlag = TRUE;
				if (0 == m_FileHelper.FileExists(sDataFile))
					//создаем файл данных
					bDataFileFlag = CreateCompressedDataFile(sDataFile, i);
				if (bDataFileFlag)
				{
					HANDLE hFile = CreateFile(sDataFile, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
					if (INVALID_HANDLE_VALUE != hFile)
					{
						HANDLE hMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0x00000000, 0x00000000, NULL);
						if (NULL != hMap)
						{
							//смещение в файле сжатых данных (реальное)
							long long llFileOffset = long long(CTimeSpan(tStart - tMonthStart).GetTotalSeconds() / m_CompressionPoint[i].lCompressionTime) * 2 * sizeof(float);
							//смещение в файле сжатых данных (приведенное к гранул€рности)
                            long long llTemp = llFileOffset / m_dwAllocationGranularity;
							llTemp *= m_dwAllocationGranularity;

							//количество точек, которое будет записано
							double dPeriodStart = double(long long(CTimeSpan(tStart - tMonthStart).GetTotalSeconds() / m_CompressionPoint[i].lCompressionTime) * m_CompressionPoint[i].lCompressionTime + tMonthStart.GetTime());
							long long lCount = long(ceil((dFinish - dPeriodStart) / m_CompressionPoint[i].lCompressionTime));
							unsigned long long ulSize = unsigned long(2 * sizeof(float) * lCount + llFileOffset - llTemp);
							float* pView = reinterpret_cast<float*>(MapViewOfFile(hMap, FILE_MAP_WRITE, 0, (DWORD)llTemp, (SIZE_T)ulSize));

							if (pView != NULL)
							{
								long long lPeriodStep = long long(double(m_CompressionPoint[i].lCompressionTime) * dFrequency);
								long long lOffset(0);
								long long lDelta = long long((dStart - dPeriodStart) * dFrequency);
								long long lTempSize = lPeriodStep - lDelta;
								long long lTempCounter(0);
								while (lOffset < lSize)
								{
									if (lOffset + lTempSize > lSize)
										lTempSize = lSize - lOffset;

									float fMin(NAN), fMax(NAN);
									int order(0);
									if (ippStsNanArg != ippsMaxOrder_32f(pData + lOffset, (int)lTempSize, &order))
										ippsMinMax_32f(pData + lOffset, (int)lTempSize, &fMin, &fMax);
									
									CheckMinimum(pView[(llFileOffset - llTemp) / sizeof(float)], fMin);
									CheckMaximum(pView[(llFileOffset - llTemp) / sizeof(float) + 1], fMax);

									llFileOffset += 2 * sizeof(float);
									lOffset += lTempSize;
									lTempSize = lPeriodStep;
									dPeriodStart += m_CompressionPoint[i].lCompressionTime;
									++lTempCounter;
								}

								UnmapViewOfFile(pView);
							}
							CloseHandle(hMap);
						}
						CloseHandle(hFile);
					}
				}
			}
		}
	}
	return TRUE;
}
void CSingleTrendWriter::StopWriter()
{
	//TRACE(L"StopWriter\n");

	m_sCurrentDescriptorFile.Empty();
	if (m_pResamplingData != NULL)
	{
		delete[]m_pResamplingData;
		m_pResamplingData = nullptr;
	}
	if (m_hMap != NULL)
	{
		CloseHandle(m_hMap);
	}
	if (m_hFile != NULL)
	{
		CloseHandle(m_hFile);
		m_hFile = NULL;
	}
}
