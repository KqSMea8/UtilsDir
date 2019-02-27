#pragma once
#include <ZET7xxx\include\base\ZET7xxxDefines.h>
#include "iohelper\ftdi\libFTDI.h"

static const unsigned int g_uiSPIDataSize = 2048;

typedef enum _COMMAND_TYPE : unsigned char
{
	TEST_PACKET = 0,
	MODBUS_PACKET,
	SD_READ_SECTOR,
	SD_WRITE_SECTOR,
} COMMAND_TYPE;

class CSPIWork
{
public:
	CSPIWork();
	~CSPIWork();

	FT_STATUS FT232_SPI_Restart(FT_HANDLE ftHandle);
	FT_STATUS FT232_SPI_Write(FT_HANDLE ftHandle, unsigned short usSize, unsigned char* pucBuffer);
	FT_STATUS FT232_SPI_Read(FT_HANDLE ftHandle, unsigned short* pusSize, unsigned char* pucBuffer);
	FT_STATUS FT232_SPI_WaitReady(FT_HANDLE ftHandle);

protected:
	DWORD m_dwTXCounter;
	unsigned char* m_pucTXBuffer;
	unsigned char* m_pucRXBuffer;

private:
	LibFTDI* m_pFtdi;

	FT_STATUS SPI_Write(FT_HANDLE handle, uint8* buffer, uint32 sizeToTransfer, uint32* sizeTransfered, uint32 options);
	FT_STATUS SPI_Read(FT_HANDLE handle, uint8* buffer, uint32 sizeToTransfer, uint32* sizeTransfered, uint32 options);
	FT_STATUS FT_WriteGPIO(FT_HANDLE handle, uint8 dir, uint8 value);
	FT_STATUS FT_ReadGPIO(FT_HANDLE handle, uint8* value);
};

