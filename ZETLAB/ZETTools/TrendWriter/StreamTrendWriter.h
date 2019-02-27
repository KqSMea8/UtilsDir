#include "TrendWriter.h"

class CStreamTrendWriter: public CTrendWriter
{
public:
	CStreamTrendWriter();
	virtual ~CStreamTrendWriter();
	CStreamTrendWriter(const CStreamTrendWriter & writer);
	CStreamTrendWriter & operator = (const CStreamTrendWriter & writer);

    void OnEnforceTrendWriterMode();   // Включение принудительной записи трендов
    void OffEnforceTrendWriterMode();  // Отключение принудительной записи трендов

	void StopWriter();
    WRITER_ERROR_CODE WriteData(const float* pData, long lSize, double dFrequency, double dStart, double dFinish) override;

protected:
	bool DeleteOldRecords() override;

	HANDLE m_hEvent;

private:
    BOOL m_bEnforceTrendWriter = FALSE; // Принудительная запись трендов
	WRITER_ERROR_CODE WriteCompressedData(long lNumber, double dStart, double dFinish);
};
