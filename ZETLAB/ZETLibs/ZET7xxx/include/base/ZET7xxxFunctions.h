#ifndef zet7xxx_functions_h_
#define zet7xxx_functions_h_

#include "ZET7xxxTypes.h"

#include <tchar.h>

/** Пространство имен zet7xxx. */
namespace zet7xxx
{
	bool IsMaster(_In_ const Device& device);
	bool IsRs485Master(_In_ const Device& device);
	bool IsCanMaster(_In_ const Device& device);
	bool IsUsbMaster(_In_ const Device& device);
	bool IsEthernetMaster(_In_ const Device& device);
	bool IsComportMaster(_In_ const Device& device);
	bool IsRs485(_In_ const Device& device);
	bool IsCan(_In_ const Device& device);
	bool IsSensor(_In_ const Device& device);
	bool IsController(_In_ const Device& device);
	bool IsInterface(_In_ const Device& device);
	bool IsUpdatable(_In_ const Device& device);
	bool IsPressureSensor(_In_ const Device& device);

	const uint32_t getMasterPrior(_In_ const Device& master);
	const uint8_t getByteSize(_In_ const Device& master);

	uint32_t getNextBaudRate(_In_ const Device& master, _In_ const uint32_t baudRate);
	uint32_t getPreviousBaudRate(_In_ const Device& master, _In_ const uint32_t baudRate);
	void getBaudRateList(_In_ const Device& master, _Out_ std::vector<uint32_t>& baudRates);

	uint8_t getNextParity(_In_ const Device& master, _In_ const uint8_t parity);
	uint8_t getPreviousParity(_In_ const Device& master, _In_ const uint8_t parity);
	void getParityList(_In_ const Device& master, _Out_ std::vector<uint8_t>& parities);
	std::wstring getParity(_In_ const uint8_t parity);
	
	std::wstring getDeviceName(_In_ const Device& device);
	ZET7XXX_DEVICE_TYPE getDeviceType(_In_ const std::wstring& sDevice);
	std::wstring getFirmwareVersion(_In_ const VERSION_STRUCT& version);
	std::wstring getFirmwareVersion(_In_ const time_t& version);
	void correctDeviceVersion4Update(_Inout_ VERSION_STRUCT& version);
	void getPossibleUpdateFiles(_In_ const Device& device, _In_ const VERSION_STRUCT& version,
		_Out_ std::vector<std::wstring>& files);

	bool isDeviceCorrect(_In_ const Device& device);
	bool isBaudRateCorrect(_In_ const Device& master, _In_ const uint32_t baudRate);
	bool isParityCorrect(_In_ const Device& master, _In_ const uint8_t parity);
	bool isConfigurationCorrect(_In_ const Configuration& configuration);

	bool areSameType(_In_ const PDevice pDevices, _In_ uint32_t count);
}

#endif // zet7xxx_functions_h_