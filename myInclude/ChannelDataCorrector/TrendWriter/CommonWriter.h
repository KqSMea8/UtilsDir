#include <memory>
#include "ChannelDataCorrector/ChannelsHelpHeader.h"
#include "ChannelDataCorrector/DescriptorFileHelper.h"

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
	void SetDescriptorFileHelper(CDescriptorFileHelper* pDescriptorFileHelper);
	
	ChannelInfo* GetChannelInfo();
	void SetChannelInfo(std::shared_ptr<ChannelInfo> channelInfo);
	double GetDuration();

	virtual void StartWriter(CTime tTime) = 0;
	void StartWriter(double dTime);
	virtual void StopWriter() = 0;

protected:
	CString m_sSignalDirectory;										//директори€ дл€ записи исходных сигналов
	CString m_sCompressedDirectory;									//директори€ дл€ записи сжатых сигналов
	CString m_sCurrentSignalDirectory;								//текуща€ директори€ записи исходного сигнала
	CDescriptorFileHelper* m_pDescriptorFileHelper;					//вспомогательный элемент дл€ работы с файлами-описател€ми
	std::shared_ptr<ChannelInfo> m_Channel;							//информаци€ о записываемом канале

	HANDLE m_hFile;													//дескриптор файла, в который пишетс€ исходный сигнал
	
	BOOL CreateDataFile(CString &sFileName, bool bEmpty);
};

