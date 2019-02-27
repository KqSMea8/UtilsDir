#pragma once

#include <ZET7xxx\include\base\ZET7xxxDefines.h>
#include <ZET7xxx\include\base\ZET7xxxTypes.h>

using namespace zet7xxx;
using namespace flash;

class CFirmwareFileHelper
{
public:
	CFirmwareFileHelper(_In_ const wchar_t* sFileName);
	CFirmwareFileHelper(_In_ const wchar_t* sPath, _In_ const Device& device, _In_ const VERSION_STRUCT& deviceVersion);
	CFirmwareFileHelper(_In_ const Device& device, _In_ const std::vector<uint8_t>& buffer);
	~CFirmwareFileHelper();

	VERSION_STRUCT getUpdateVersion() const { return m_version; }
	std::wstring getUpdateFileName() const;
	void getUpdateDataBuffer(_Inout_ uint8_t* pBuffer, _In_ const uint32_t address, _In_ const uint32_t size) const;

protected:
	long ReadUPDFile(wchar_t* sFileName);

	wchar_t m_sFileName[MAX_PATH];
	VERSION_STRUCT m_version;
	Device m_device;
	uint8_t m_buffer[g_uiFlashSectorSize];
};

