#pragma once
#include "CommonWriter.h"
#include "..\compression\CompressionInfo.h"
//#include <SimpleEvent/Event.h>

#define MAX_SIZE				500000
const uint32_t g_compressionIntervalsSize = 7;

class CTrendWriter: public CCommonWriter
{
public:
	CTrendWriter(void);
	virtual ~CTrendWriter(void);

	//Get-��������
	std::wstring getCurrentSignalDirectory() const { return m_sCurrentSignalDirectory; }
	
	//������
	void start(_In_ const ptime& tTime) override;
	virtual void stop() override {};
	
	virtual WRITER_ERROR_CODE write(_In_ float* pData, _In_ const uint32_t size, _In_ const double frequency,
		_In_ const ptime& tStart, _In_ const ptime& tFinish, _In_ const bool bCompressed) override = 0;
		
protected:
	HANDLE m_hMap;													//���������� ����������� ����� � ������
	
	uint64_t m_allocationGranularity;								//������������� ��� ���������� ������, � ������� ����� ���� �������� ����������� ������
	std::vector<CCompressionInfo> m_compressionInfo;				//������� �������� ������ ������ �� ������ ������� ������
	ptime m_lastTime;
	
	float m_fMemoryPoint;											//������ ��� �����������������
	float* m_pResamplingData;										//������ ������ ��� �����������������
				
	bool createCompressedDataFile(_In_ const std::wstring& sFileName, _In_ const time_duration& interval, _In_ const ptime& tTime);
		
	void CheckMinimum(float& fDst, float& fSrc);
	void CheckMaximum(float& fDst, float& fSrc);
	
	void Resampling(float* pSource, long lSourceSize, float* pDestination, long lDestinationSize);

	void findMinMaxDirectory(_In_ const std::wstring& sDirecory, _Out_ int32_t& min, _Out_ int32_t& max);

	template<class _Fn1>
	inline _Fn1 for_each_compression_info(_Fn1 _Func)
	{
		return std::for_each(m_compressionInfo.begin(), m_compressionInfo.end(), _Func);
	}
};