#include "DriveChecker.h"
#include "Channel.h"
#include "FileHelper.h"
#include "DescriptorFileHelper.h"

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

	void SetSignalDirectory(CString sDirectory);
	void SetCompressedDirectory(CString sDirectory);
	void SetDriveChecker(CDriveChecker* pDriveChecker);
	void SetDescriptorFileHelper(CDescriptorFileHelper* pDescriptorFileHelper);
	void SetTestMode(bool bTest){ m_bTestMode = bTest; }

	CChannel* GetChannelInfo();
	double GetDuration();

	virtual WRITER_ERROR_CODE WriteData(const float* pData, long lSize, double dFrequency, double dStart, double dFinish) = 0;
    WRITER_ERROR_CODE WriteLog(double dTime, CString &sMessage);
	virtual void StartWriter(CTime tTime) = 0;
	void StartWriter(double dTime);
	virtual void StopWriter() = 0;

protected:
	CString m_sSignalDirectory;										//директори€ дл€ записи исходных сигналов
	CString m_sCompressedDirectory;									//директори€ дл€ записи сжатых сигналов
	CString m_sCurrentSignalDirectory;								//текуща€ директори€ записи исходного сигнала
	CDriveChecker* m_pDriveChecker;									//элемент дл€ проверки заполненности диска
	CFileHelper m_FileHelper;
	CDescriptorFileHelper* m_pDescriptorFileHelper;					//вспомогательный элемент дл€ работы с файлами-описател€ми
	CChannel m_Channel;												//информаци€ о записываемом канале

	WRITER_TYPE m_wtType;											//тип

	HANDLE m_hFile;													//дескриптор файла, в который пишетс€ исходный сигнал

	bool m_bTestMode;

	BOOL CreateDataFile(CString &sFileName, bool bEmpty);
	BOOL CreateLogFile(CString &sFileName);
	virtual bool DeleteOldRecords() = 0;
};

