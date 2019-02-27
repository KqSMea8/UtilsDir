#pragma once
#include "ChannelsHelpHeader.h"
#include <map>
#include <vector>

class CChannelEx: public ChannelInfo
{
public:
	CChannelEx(void): m_sFileName(L""){};
	~CChannelEx(void){};

	CString GetFileName(){return m_sFileName;};
	void SetFileName(CString sFileName){m_sFileName = sFileName;};

protected:
	CString m_sFileName;
};

class CDescriptorFileHelper
{
public:
	CDescriptorFileHelper(void);
	~CDescriptorFileHelper(void);

	CString FindDescriptorFile(CString &sDirectory, ChannelInfo *channel);
	CString CreateDescriptorFile(CString &sDirectory, CTime tTime, ChannelInfo *channel);
	double GetFrequencyFromDescriptorFile(CString &sDirectory, CString &sDescriptorFile);

	CString FindCommonDescriptorFile(CString &sDirectory);
	CString CreateCommonDescriptorFile(CString &sDirectory, CTime tTime);
	void AppendCommonDescriptorFile(CString &sDirectory, CString &sCommonDescriptorFile, ChannelInfo *channel, CString &sDescriptorFile, CString &sDataFile/*, CString &sLogFile*/);
	bool FindChannelInCommonDescriptorFile(CString &sDirectory, ChannelInfo *channel);

	CString FindCommonCompressedDescriptorFile(CString &sDirectory);
	CString CreateCommonCompressedDescriptorFile(CString &sDirectory, CTime tTime);
	void AppendCommonCompressedDescriptorFile(CString &sDirectory, CString &sCommonDescriptorFile, ChannelInfo *channel, CString &sDescriptorFile, CString &sDataFile, CString &sLogFile);
	bool FindChannelInCommonCompressedDescriptorFile(CString &sDirectory, ChannelInfo *channel);

	CString GetLastSignalDirectory(CString sDirectory);

protected:
	std::map <CString, std::vector <CChannelEx>> m_mDescriptors;		//ключ - директория, значение - вектор файлов-описателей каналов, которые есть в директории
	std::map <CString, std::vector <ChannelInfo>> m_mCommonDescriptors;	//ключ - директория, значение - вектор каналов, которые описаны в общем файле описателе

	void AddDirectory(CString sDirectory);
	void FindMinMaxDirectory(CString sDirecory, long *pMin, long *pMax);
};

