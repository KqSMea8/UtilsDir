#include "TrendWriter.h"

class CSingleWriter : public CCommonWriter
{
public:
	CSingleWriter();
	~CSingleWriter();

	void SetCurrentDirectory(CString &sDirectory);

	WRITER_ERROR_CODE WriteData(const float* pData, long lSize, double dFrequency, double dStart, double dFinish) override;
	void StartWriter(CTime tTime) override;
	void StopWriter() override;

protected:
	bool DeleteOldRecords() override { return false; };
private:

};
