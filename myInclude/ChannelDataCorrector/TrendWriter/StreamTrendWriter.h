#include "TrendWriter.h"

class CStreamTrendWriter: public CTrendWriter
{
public:
	CStreamTrendWriter();
	virtual ~CStreamTrendWriter();
	CStreamTrendWriter(const CStreamTrendWriter & writer);
	CStreamTrendWriter & operator = (const CStreamTrendWriter & writer);
	
	void StopWriter();
    WRITER_ERROR_CODE WriteData(const float* pData, long lSize, double dFrequency, double dStart, double dFinish);
	WRITER_ERROR_CODE ReadData(float* pData, long lSize, double dFrequency, CTime startTime, CTime finishTime);
protected:

	HANDLE m_hEvent;

private:
	WRITER_ERROR_CODE WriteCompressedData(long lNumber, double dStart, double dFinish);
};
