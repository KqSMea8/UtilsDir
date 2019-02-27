// Модуль для линковки DLL от FTDI в реальном времени
#pragma once

// Создать глобальные объекты (g_FT, g_FTID, g_MPSSE)
//#define LIB_FTDI_CREATE_GLOBAL_OBJECTS 1

#define FTD2XX_EXPORTS 1    // Важно для run-time линковки ftd2xx.dll
#include "ftd2xx.h"
#include "FTChipID.h"
#include "SPI/libMPSSE_spi.h"


#ifdef LIB_FTDI_CREATE_GLOBAL_OBJECTS
extern class LibFTDI g_FTDI;
#endif


#define LIB_FTDI_PROC(ReturnType, ErrorValue, DeclType, Name, Proto, Args)	\
protected:																	\
	typedef ReturnType (DeclType *Type_ ## Name) Proto;						\
	Proc m_ ## Name;														\
public:																		\
	FT_STATUS Name Proto const												\
	{																		\
		if (m_ ## Name.Addr)												\
		{																	\
			return ((Type_ ## Name) m_ ## Name.Addr) Args;					\
		}																	\
		return ErrorValue;													\
	}

#define LIB_FTD2_PROC(Name, Proto, Args)	LIB_FTDI_PROC(FT_STATUS,   FT_NOT_SUPPORTED,    WINAPI,  Name, Proto, Args)
#define LIB_FTID_PROC(Name, Proto, Args)	LIB_FTDI_PROC(FTID_STATUS, FTID_INVALID_HANDLE, WINAPI,  Name, Proto, Args)
#define LIB_SPI_PROC(Name, Proto, Args)		LIB_FTDI_PROC(FT_STATUS,   FT_NOT_SUPPORTED,    __cdecl, Name, Proto, Args)

class LibFTDI
{
public:
	LibFTDI();
	~LibFTDI();

	FT_STATUS ft_GetStatus(FT_HANDLE ftHandle, DWORD* dwRxBytes, DWORD* dwTxBytes, DWORD* dwEventDWord);

protected:
	struct Proc
	{
		FARPROC Addr;

		Proc() : Addr(NULL) {}
	};

	class Module
	{
	public:
		Module() : m_Lib(NULL) {}
		~Module() { Unload(); }

		bool Load(LPCTSTR LibName);
		void Unload();

		operator bool() const { return m_Lib != NULL; }
		bool LoadProc(const char* ProcName, Proc& Proc) const;

	protected:
		HMODULE m_Lib;
	};

protected:
	Module m_FTD2XX;
	Module m_FTCHIPID;
	Module m_MPSSE;

protected:
	LIB_FTD2_PROC(FT_CreateDeviceInfoList, (
			LPDWORD lpdwNumDevs),
			(lpdwNumDevs))
	LIB_FTD2_PROC(FT_GetDeviceInfoList, (
			FT_DEVICE_LIST_INFO_NODE *pDest,
			LPDWORD lpdwNumDevs),
			(pDest, lpdwNumDevs))
	LIB_FTD2_PROC(FT_GetDeviceInfoDetail, (
			DWORD dwIndex,
			LPDWORD lpdwFlags,
			LPDWORD lpdwType,
			LPDWORD lpdwID,
			LPDWORD lpdwLocId,
			LPVOID lpSerialNumber,
			LPVOID lpDescription,
			FT_HANDLE *pftHandle),
			(dwIndex, lpdwFlags, lpdwType, lpdwID, lpdwLocId,
			lpSerialNumber, lpDescription, pftHandle))
	LIB_FTD2_PROC(FT_Open, (
			int deviceNumber,
			FT_HANDLE *pHandle),
			(deviceNumber, pHandle))
	LIB_FTD2_PROC(FT_Close, (
		FT_HANDLE ftHandle),
		(ftHandle))
	LIB_FTD2_PROC(FT_Read, (
			FT_HANDLE ftHandle,
			LPVOID lpBuffer,
			DWORD dwBytesToRead,
			LPDWORD lpBytesReturned),
			(ftHandle, lpBuffer, dwBytesToRead, lpBytesReturned))
	LIB_FTD2_PROC(FT_Purge, (
			FT_HANDLE ftHandle,
			DWORD dMask),
			(ftHandle, dMask))
	LIB_FTD2_PROC(FT_Write, (
			FT_HANDLE ftHandle,
			LPVOID lpBuffer,
			DWORD dwBytesToWrite,
			LPDWORD lpBytesWritten),
			(ftHandle, lpBuffer, dwBytesToWrite, lpBytesWritten))
	LIB_FTD2_PROC(FT_CyclePort, (
			FT_HANDLE ftHandle),
			(ftHandle))
	LIB_FTD2_PROC(FT_ResetPort, (
			FT_HANDLE ftHandle),
			(ftHandle))
	LIB_FTD2_PROC(FT_EE_UARead, (
			FT_HANDLE ftHandle,
			PUCHAR pucData,
			DWORD dwDataLen,
			LPDWORD lpdwBytesRead),
			(ftHandle, pucData, dwDataLen, lpdwBytesRead))
	LIB_FTD2_PROC(FT_EE_UAWrite, (
			FT_HANDLE ftHandle,
			PUCHAR pucData,
			DWORD dwDataLen),
			(ftHandle, pucData, dwDataLen))
	LIB_FTD2_PROC(FT_GetStatus, (
			FT_HANDLE ftHandle,
			DWORD *dwRxBytes,
			DWORD *dwTxBytes,
			DWORD *dwEventDWord),
			(ftHandle, dwRxBytes, dwTxBytes, dwEventDWord))
	LIB_FTD2_PROC(FT_GetDeviceInfo, (
			FT_HANDLE ftHandle,
			FT_DEVICE *lpftDevice,
			LPDWORD lpdwID,
			PCHAR SerialNumber,
			PCHAR Description,
			LPVOID Dummy),
			(ftHandle, lpftDevice, lpdwID, SerialNumber, Description, Dummy))
	LIB_FTD2_PROC(FT_SetBaudRate, (
			FT_HANDLE ftHandle,
			ULONG BaudRate),
			(ftHandle, BaudRate))
	LIB_FTD2_PROC(FT_SetDataCharacteristics, (
			FT_HANDLE ftHandle,
			UCHAR WordLength,
			UCHAR StopBits,
			UCHAR Parity),
			(ftHandle, WordLength, StopBits, Parity))
	LIB_FTD2_PROC(FT_SetTimeouts, (
			FT_HANDLE ftHandle,
			ULONG ReadTimeout,
			ULONG WriteTimeout),
			(ftHandle, ReadTimeout, WriteTimeout))
	LIB_FTD2_PROC(FT_SetLatencyTimer, (
			FT_HANDLE ftHandle,
			UCHAR ucLatency),
			(ftHandle, ucLatency))
	LIB_FTD2_PROC(FT_SetUSBParameters, (
			FT_HANDLE ftHandle,
			ULONG ulInTransferSize,
			ULONG ulOutTransferSize),
			(ftHandle, ulInTransferSize, ulOutTransferSize))

	LIB_FTID_PROC(FTID_GetChipIDFromHandle, (
			FT_HANDLE ftHandle,
			unsigned long* ChipIDBuffer),
			(ftHandle, ChipIDBuffer))

	LIB_SPI_PROC(SPI_GetNumChannels, (
			uint32 *numChannels),
			(numChannels))
	LIB_SPI_PROC(SPI_GetChannelInfo, (
			uint32 index, 
			FT_DEVICE_LIST_INFO_NODE *chanInfo),
			(index, chanInfo))
	LIB_SPI_PROC(SPI_OpenChannel, (
			uint32 index,
			FT_HANDLE *handle),
			(index, handle))
	LIB_SPI_PROC(SPI_InitChannel, (
			FT_HANDLE handle,
			ChannelConfig *config),
			(handle, config))
	LIB_SPI_PROC(SPI_CloseChannel, (
			FT_HANDLE handle),
			(handle))
	LIB_SPI_PROC(SPI_Read, (
			FT_HANDLE handle,
			uint8 *buffer, 
			uint32 sizeToTransfer,
			uint32 *sizeTransfered,
			uint32 options),
			(handle, buffer, sizeToTransfer, sizeTransfered, options))
	LIB_SPI_PROC(SPI_Write, (
			FT_HANDLE handle,
			uint8 *buffer, 
			uint32 sizeToTransfer,
			uint32 *sizeTransfered,
			uint32 options),
			(handle, buffer, sizeToTransfer, sizeTransfered, options))
	LIB_SPI_PROC(SPI_IsBusy, (
			FT_HANDLE handle,
			bool *state),
			(handle, state))
	LIB_SPI_PROC(SPI_ChangeCS, (
			FT_HANDLE handle,
			uint32 configOptions),
			(handle, configOptions))
	LIB_SPI_PROC(FT_WriteGPIO, (
			FT_HANDLE handle,
			uint8 dir,
			uint8 value),
			(handle, dir, value))
	LIB_SPI_PROC(FT_ReadGPIO, (
			FT_HANDLE handle,
			uint8 *value),
			(handle, value))
};
