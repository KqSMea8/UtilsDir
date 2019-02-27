#pragma once
#include "..\channel\Channel.h"

#include <ZETSimple\include\singleton\singleton_auto_link.h>
#include <ZETSimple\include\singleton\singleton.h>

#include <boost\noncopyable.hpp>
#include <boost\date_time\posix_time\posix_time.hpp>

#include <map>
#include <vector>

using namespace boost::posix_time;

class CDescriptorFileHelperImpl : private boost::noncopyable
{
public:
	CDescriptorFileHelperImpl(void);
	~CDescriptorFileHelperImpl(void);

	std::wstring findDescriptorFile(_In_ const std::wstring& sDirectory, _In_ const CChannel& channel);
	std::wstring createDescriptorFile(_In_ const std::wstring& sDirectory, _In_ const CChannel &channel, _In_ const ptime& tTime);
	double getFrequencyFromDescriptorFile(_In_ const std::wstring& sDirectory, _In_ const std::wstring& sDescriptorFile);

	std::wstring findCommonDescriptorFile(_In_ const std::wstring& sDirectory);
	std::wstring createCommonDescriptorFile(_In_ const std::wstring& sDirectory, _In_ const ptime& tTime);
	void appendCommonDescriptorFile(_In_ const std::wstring& sDirectory, _In_ const std::wstring& sCommonDescriptorFile,
		_In_ const CChannel& channel, _In_ const std::wstring& sDescriptorFile, _In_ const std::wstring& sDataFile);
	bool channelExistInCommonDescriptorFile(_In_ const std::wstring& sDirectory, _In_ const CChannel& channel);

	std::wstring findCommonCompressedDescriptorFile(_In_ const std::wstring& sDirectory);
	std::wstring createCommonCompressedDescriptorFile(_In_ const std::wstring& sDirectory, _In_ const ptime& tTime);
	void appendCommonCompressedDescriptorFile(_In_ const std::wstring& sDirectory, _In_ const std::wstring& sCommonDescriptorFile,
		_In_ const CChannel& channel, _In_ const std::wstring& sDescriptorFile, _In_ const std::wstring& sDataFile);
	bool channelExistInCommonCompressedDescriptorFile(_In_ const std::wstring& sDirectory, _In_ const CChannel& channel);

	std::wstring getLastSignalDirectory(_In_ const std::wstring& sDirectory);

protected:
	//ключ - директория, значение - вектор файлов-описателей каналов, которые есть в директории
	std::map <std::wstring, std::vector <CChannelEx>> m_descriptors;		
	//ключ - директория, значение - вектор каналов, которые описаны в общем файле описателе
	std::map <std::wstring, std::vector <CChannel>> m_commonDescriptors;	

	void addDirectory(_In_ const std::wstring& sDirectory);
	void findMinMaxDirectory(_In_ const std::wstring& sDirecory, _Out_ int32_t& min, _Out_ int32_t& max);

private:
	std::wstring GUIDToWstring(_In_ const GUID& guid);
};

typedef CSingleton<CDescriptorFileHelperImpl> CDescriptorFileHelper;