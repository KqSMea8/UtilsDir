#pragma once
#include "../base/usb/USBMaster.h"
#include "../base/rs485/RS485Master.h"

class CComportMaster: public CRS485Master
{
public:
	CComportMaster(uint32_t number);
	~CComportMaster();

protected:
	ErrorCode connect() override;
	ErrorCode disconnect() override;

	void initialize() override;
	ErrorCode changeBaudRate(uint32_t baudRate) override;
	ErrorCode changeParity(uint8_t parity) override;
	ErrorCode recoverBaudRate() override;
	ErrorCode recoverParity() override;
	void updateDataExchangeParametersFromRestore() override;

	std::wstring getNameWithSerial() const override;
		
private:
	DWORD threadFunction() override;

	uint32_t m_number;
	HANDLE m_hPort;
};

