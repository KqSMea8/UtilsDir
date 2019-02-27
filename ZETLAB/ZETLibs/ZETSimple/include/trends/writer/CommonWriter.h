#include "..\drive_checker\DriveChecker.h"
#include "..\channel\Channel.h"
#include "..\file_helper\DescriptorFileHelper.h"

using namespace boost::posix_time;

typedef enum _WRITER_TYPE : int
{
	wtUnknown = -1,
	wtTrendStream,
	wtTrendSingle,
	wtSingle,
} WRITER_TYPE;

typedef enum _WRITER_ERROR_CODE : unsigned long
{
	wecSuccessful = 0,
	wecLargeTimeDifference,
	wecIncorrectDataSize,
	wecNullWriteData,
	wecCreateFileError,
	wecCreateFileMappingError,
	wecMapViewOfFileError,
	wecUnmapViewOfFileError,
	wecIncorrectFileOffset,
	wecWriteFileError,
	wecDirectoryError,
	wecDescriptorFileError,
	wecDataFileError,
	wecCreateFileAndFileMappingError,
	wecInvalidTime,
} WRITER_ERROR_CODE;

class CCommonWriter
{
public:
	CCommonWriter();
	virtual ~CCommonWriter();

	void setSignalDirectory(_In_ const std::wstring& sDirectory);
	void setCompressedDirectory(_In_ const std::wstring& sDirectory);
	void SetTestMode(bool bTest){ m_bTestMode = bTest; }

	CChannel* getChannelInfo();
	
	//virtual WRITER_ERROR_CODE WriteData(float* pData, long lSize, double dFrequency, double dStart, double dFinish) = 0;
	virtual WRITER_ERROR_CODE write(_In_ float* pData, _In_ const uint32_t size, _In_ const double frequency,
		_In_ const ptime& tStart, _In_ const ptime& tFinish, _In_ const bool bCompressed) = 0;
	virtual void start(_In_ const ptime& tTime) = 0;
	//void StartWriter(double dTime);
	virtual void stop() = 0;

protected:
	std::wstring m_sSignalDirectory;								//директория для записи исходных сигналов
	std::wstring m_sCompressedDirectory;							//директория для записи сжатых сигналов
	std::wstring m_sCurrentSignalDirectory;							//текущая директория записи исходного сигнала
	CChannel m_Channel;												//информация о записываемом канале

	WRITER_TYPE m_wtType;											//тип

	HANDLE m_hFile;													//дескриптор файла, в который пишется исходный сигнал

	bool m_bTestMode;

	bool createDataFile(_In_ const std::wstring& sFileName, _In_ const bool bEmpty);
	
	virtual bool deleteOldRecords() = 0;
};

