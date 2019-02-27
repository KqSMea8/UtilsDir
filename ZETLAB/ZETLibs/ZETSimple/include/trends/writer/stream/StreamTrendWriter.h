#include "..\TrendWriter.h"

class CStreamTrendWriter: public CTrendWriter
{
public:
	CStreamTrendWriter();
	CStreamTrendWriter(CStreamTrendWriter &writer);
	~CStreamTrendWriter();

	void stop() override;
	WRITER_ERROR_CODE write(_In_ float* pData, _In_ const uint32_t size, _In_ const double frequency,
		_In_ const ptime& tStart, _In_ const ptime& tFinish, _In_ const bool bCompressed) override;

	WRITER_ERROR_CODE write(_In_ const bool bCompressed);

protected:
	bool deleteOldRecords() override;

	HANDLE m_hEvent;

private:
	WRITER_ERROR_CODE writeCompressedData(_In_ CCompressionInfo& info);
};
