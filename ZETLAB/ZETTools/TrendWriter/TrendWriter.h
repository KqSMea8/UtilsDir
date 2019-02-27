#pragma once
#include "CommonWriter.h"

#include <SimpleEvent/Event.h>

#define MAX_SIZE				500000
#define NUMBER_OF_COMPRESSION	7

struct CompressionInfo
{
	float fMin;
	float fMax;
	long lCompressionTime;
	CString sExtention;

	void SetExtention()
	{
		if (lCompressionTime < 60)
		{
			sExtention.Format(L".%02ds", lCompressionTime);
		}
		else
		//если временной интервал сжатия внутри часа
		if (lCompressionTime < 3600)
		{
			sExtention.Format(L".%02dm", lCompressionTime / 60);
		}
		else
		//если временной интервал сжатия внутри дня
		if (lCompressionTime < 86400)
		{
			sExtention.Format(L".%02dh", lCompressionTime / 3600);
		}
		//если временной интервал сжатия больше дня
		else
		{
			sExtention.Format(L".%02dd", lCompressionTime / 86400);
		}
	}
};

class CTrendWriter: public CCommonWriter
{
public:
	CTrendWriter(void);
	virtual ~CTrendWriter(void);

	//Get-свойства
	CString GetCurrentSignalDirectory();
	bool GetNewHourFlag();

	//методы
	void StartWriter(CTime tTime) override;
	virtual void StopWriter() {};
	virtual WRITER_ERROR_CODE WriteData(const float* pData, long lSize, double dFrequency, double dStart, double dFinish) = 0;
		
	void ResetNewHourFlag();
	
protected:
	HANDLE m_hMap;													//дескриптор отображения файла в память
	
	CString m_sCurrentCompressedDirectory;							//текущая директория записи сжатого сигнала
	DWORD m_dwAllocationGranularity;								//гранулярность для начального адреса, в котором может быть выделена виртуальная память
	long m_lDaysInCurrentMonth;										//количество дней в текущем месяце
	CompressionInfo m_CompressionPoint[NUMBER_OF_COMPRESSION];		//текущее значение сжатых данных по каждой степени сжатия
	CString m_sCurrentDescriptorFile;
	CTime m_tLastStartTime;											//последнее начальное время записи исходных данных
	float m_fMemoryPoint;											//память для передискретизации
	float* m_pResamplingData;										//массив данных для передискретизации
	bool m_bNewHour;
			
	BOOL CreateCompressedDataFile(CString sFileName, long lNumber);
		
	void CheckMinimum(float& fDst, float& fSrc);
	void CheckMaximum(float& fDst, float& fSrc);
	bool isNan(float value);
	
	void Resampling(const float* pSource, long lSourceSize, float* pDestination, long lDestinationSize);

	void FindMinMaxDirectory(CString sDirecory, long *pMin, long *pMax);
};