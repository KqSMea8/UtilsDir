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
		//���� ��������� �������� ������ ������ ����
		if (lCompressionTime < 3600)
		{
			sExtention.Format(L".%02dm", lCompressionTime / 60);
		}
		else
		//���� ��������� �������� ������ ������ ���
		if (lCompressionTime < 86400)
		{
			sExtention.Format(L".%02dh", lCompressionTime / 3600);
		}
		//���� ��������� �������� ������ ������ ���
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

	//Get-��������
	CString GetCurrentSignalDirectory();
	bool GetNewHourFlag();

	//������
	void StartWriter(CTime tTime) override;
	virtual void StopWriter() {};
	virtual WRITER_ERROR_CODE WriteData(const float* pData, long lSize, double dFrequency, double dStart, double dFinish) = 0;
		
	void ResetNewHourFlag();
	
protected:
	HANDLE m_hMap;													//���������� ����������� ����� � ������
	
	CString m_sCurrentCompressedDirectory;							//������� ���������� ������ ������� �������
	DWORD m_dwAllocationGranularity;								//������������� ��� ���������� ������, � ������� ����� ���� �������� ����������� ������
	long m_lDaysInCurrentMonth;										//���������� ���� � ������� ������
	CompressionInfo m_CompressionPoint[NUMBER_OF_COMPRESSION];		//������� �������� ������ ������ �� ������ ������� ������
	CString m_sCurrentDescriptorFile;
	CTime m_tLastStartTime;											//��������� ��������� ����� ������ �������� ������
	float m_fMemoryPoint;											//������ ��� �����������������
	float* m_pResamplingData;										//������ ������ ��� �����������������
	bool m_bNewHour;
			
	BOOL CreateCompressedDataFile(CString sFileName, long lNumber);
		
	void CheckMinimum(float& fDst, float& fSrc);
	void CheckMaximum(float& fDst, float& fSrc);
	bool isNan(float value);
	
	void Resampling(const float* pSource, long lSourceSize, float* pDestination, long lDestinationSize);

	void FindMinMaxDirectory(CString sDirecory, long *pMin, long *pMax);
};