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
	CString m_sSignalDirectory;										//���������� ��� ������ �������� ��������
	CString m_sCompressedDirectory;									//���������� ��� ������ ������ ��������
	CString m_sCurrentSignalDirectory;								//������� ���������� ������ ��������� �������
	CDescriptorFileHelper* m_pDescriptorFileHelper;					//��������������� ������� ��� ������ � �������-�����������
	std::shared_ptr<ChannelInfo> m_Channel;							//���������� � ������������ ������

	HANDLE m_hFile;													//���������� �����, � ������� ������� �������� ������
	
	BOOL CreateDataFile(CString &sFileName, bool bEmpty);
};

