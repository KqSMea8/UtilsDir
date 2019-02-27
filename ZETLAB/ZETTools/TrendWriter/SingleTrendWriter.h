#pragma once
#include "TrendWriter.h"



class CSingleTrendWriter : public CTrendWriter
{
public:
	CSingleTrendWriter();
	~CSingleTrendWriter();

	WRITER_ERROR_CODE WriteData(const float* pData, long lSize, double dFrequency, double dStart, double dFinish) override;

	void StopWriter();

protected:
	bool DeleteOldRecords() override { return false; };

private:
	BOOL WriteCompressedData(float* pData, long lSize, double dFrequency, double dStart, double dFinish);
};
