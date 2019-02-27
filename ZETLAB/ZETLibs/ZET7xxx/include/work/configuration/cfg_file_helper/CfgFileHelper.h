#pragma once
#include <HtmlReport/HtmlReport.h>

#include <ZET7xxx\include\base\ZET7xxxTypes.h>

#include <windows.h>

using namespace zet7xxx;

class CConfigurationFileHelper
{
public:
	CConfigurationFileHelper(std::wstring sFile);
	~CConfigurationFileHelper();

	int32_t save(_In_ Configuration& configuration, std::wstring sPath);
	int32_t save(_In_ Configuration& configuration, _In_ Description& description, std::wstring sPath);
	int32_t saveTable(_In_ Configuration& configuration, _In_ Description& description, std::wstring sPath, bool bMetrology);

	int32_t read(_Out_ Configuration& configuration);
	
protected:
private:
	void saveAddressTable(_In_ Configuration& configuration,
		_In_ Description& description, _In_ CHtmlReport &htmlReport, bool bMetrology);
	void saveConfigurationTable(_In_ Configuration& configuration,
		_In_ Description& description, _In_ CHtmlReport &htmlReport);
	void saveAddressPage(uint16_t addressOffset, _In_ pSTRUCT_HEAD pDescriptionHead,
		_In_ pSTRUCT_HEAD pConfigurationHead, _In_ CHtmlReport &htmlReport);
	void saveConfigurationPage(uint16_t addressOffset, _In_ pSTRUCT_HEAD pDescriptionHead,
		_In_ pSTRUCT_HEAD pConfigurationHead, _In_ CHtmlReport &htmlReport);
	std::pair<uint16_t, uint16_t> savePageElement(uint16_t offset, uint16_t addressOffset, _In_ PSTRUCT_ARGV pArgv,
		_In_ PVOID pvDescription, _In_ PVOID pvConfiguration, _In_ CHtmlReport &htmlReport);
	std::pair<uint16_t, uint16_t> saveConfigurationElement(uint16_t offset, uint16_t addressOffset, _In_ PSTRUCT_ARGV pArgv,
		_In_ PVOID pvDescription, _In_ PVOID pvConfiguration, _In_ CHtmlReport &htmlReport);
	uint16_t getDescriptionSize(_In_ PVOID pvDescription, type_of_arg typeOfArg);
	uint16_t getConfigurationSize(_In_ PSTRUCT_ARGV pArgv);
	void getElementValues(_In_ PVOID pvDescription, type_of_arg typeOfArg, _Out_ std::vector<std::wstring>& values);
	void getElementValue(_In_ PVOID pvDescription, _In_ PVOID pvConfiguration, type_of_arg typeOfArg, _Out_ std::wstring& value);

	std::wstring m_sDeviceName4Save;
	std::wstring m_sFileName4Read;
};

