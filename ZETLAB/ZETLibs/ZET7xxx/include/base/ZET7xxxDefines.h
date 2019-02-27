#ifndef zet7xxx_defines_h_
#define zet7xxx_defines_h_

#include "ZET7xxxTypes.h"

#include <windows.h>
#include <tchar.h>
#include <ftd2xx.h>
#include <stdint.h>

#include <map>

#define STRINGIZE_HELPER(x) #x
#define STRINGIZE(x) STRINGIZE_HELPER(x)
#define WARNING(desc) message(__FILE__ "(" STRINGIZE(__LINE__) ") : Warning: " #desc)

namespace zet7xxx
{
	ZET7XXX_DEFINE_DEVICE_NAMES(device_names);
	static const ZET7XXX_DEVICE_TYPE COM_PORT = ZET7XXX_DEVICE_TYPE(1000);
	inline static std::map<ZET7XXX_DEVICE_TYPE, std::wstring> create_devices()
	{
		std::map<ZET7XXX_DEVICE_TYPE, std::wstring> devices;
		for (uint32_t i = 0; i < ZET7XXX_DEVICE_TYPE_COUNT; ++i)
			devices[ZET7XXX_DEVICE_TYPE(i)] = device_names[i];
		devices[COM_PORT] = _T("COM");
		return devices;
	}
	#define ZET7XXX_DEFINE_DEVICES(devices) \
	static const std::map<ZET7XXX_DEVICE_TYPE, std::wstring> devices = create_devices();
	ZET7XXX_DEFINE_DEVICES(g_devices);

	#define ZET7XXX_TYPE_OF_ARG(type) 							\
	static const wchar_t* type[ZET7XXX_TYPE_OF_ARG_COUNT] = {	\
		_T(""),													\
		_T("char"),												\
		_T("short"),											\
		_T("int"),												\
		_T("ushort"),											\
		_T("uint"),												\
		_T("float"),											\
		_T("float"),											\
		_T("int"),												\
		_T("char"),												\
		_T("int"),												\
		_T("time"),												\
		_T("int"),												\
		_T("longlong"),											\
		_T("longlong"),											\
		_T("ip"),												\
		_T("mac"),												\
		_T("int"),												\
		_T("2 float")											\
	};
	ZET7XXX_TYPE_OF_ARG(g_arg_types);

	#define ZET7XXX_SIZE_OF_ARG(size) 							\
	static const uint8_t size[ZET7XXX_TYPE_OF_ARG_COUNT] = {	\
		0,														\
		1,														\
		2,														\
		4,														\
		2,														\
		4,														\
		4,														\
		4,														\
		4,														\
		1,														\
		4,														\
		4,														\
		4,														\
		8,														\
		8,														\
		4,														\
		6,														\
		4,														\
		8														\
	};
	ZET7XXX_SIZE_OF_ARG(g_arg_sizes);

	static const uint32_t g_baudRatesRs485[] = { FT_BAUD_2400,
												 FT_BAUD_4800,
												 FT_BAUD_9600,
												 FT_BAUD_14400,
												 FT_BAUD_19200,
												 FT_BAUD_38400,
												 FT_BAUD_57600,
												 FT_BAUD_115200,
												 FT_BAUD_230400 };
	static const uint32_t g_baudRatesCan[] = { 100000,
											   300000,
											   1000000 };

	static const uint8_t g_paritiesRs485[] = {  FT_PARITY_NONE,
												FT_PARITY_ODD};

	static const Device g_nullDevice = std::make_pair(UNKNOWN, 0);
	
	namespace modbus
	{
		static const ModbusAddress g_nullAddress = 0;
		static const ModbusAddress g_masterAddress = 1;
		static const ModbusAddress g_factoryAddress = 2;
		static const ModbusAddress g_minAddress = 1;
		static const ModbusAddress g_maxAddress = 63;
		static const uint32_t g_hiddenProtocolBaudrate = FT_BAUD_19200;
		static const uint8_t g_hiddenProtocolParity = FT_PARITY_ODD;

		static const ModbusDataAddress g_nullDataAddress = UINT16_MAX;
		
		static const unsigned short g_usWEWriteState = 1;
		static const unsigned short g_usWESaveState = 3;

		static const uint16_t g_valueOffset_ChannelPar = sizeof(STRUCT_HEAD) / 2;
		static const uint16_t g_valueSize_ChannelPar = sizeof(float) / 2;
		
		static const uint16_t g_nameOffset_ChannelPar = (sizeof(STRUCT_HEAD) + sizeof(float) + sizeof(float) +
			sizeof(char[8])) / 2;
		static const uint16_t g_nameSize_ChannelPar = sizeof(char[32]) / 2;
		

		static const uint16_t g_typeOffset_DevicePar = sizeof(STRUCT_HEAD) / 2;
		static const uint16_t g_typeSize_DevicePar = sizeof(int32_t) / 2;
		static const uint16_t g_serialOffset_DevicePar = (sizeof(STRUCT_HEAD) + sizeof(int32_t)) / 2;
		static const uint16_t g_serialSize_DevicePar = sizeof(uint64_t) / 2;
		static const uint16_t g_compileTimeOffset_DevicePar = (sizeof(STRUCT_HEAD) + sizeof(int32_t) + sizeof(uint64_t)) / 2;
		static const uint16_t g_compileTimeSize_DevicePar = sizeof(int32_t) / 2;
		static const uint16_t g_addressOffset_DevicePar = (sizeof(STRUCT_HEAD) + sizeof(int32_t) + sizeof(uint64_t) +
			sizeof(int32_t) + sizeof(int32_t)) / 2;
		static const uint16_t g_addressSize_DevicePar = sizeof(uint32_t) / 2;

		static const uint16_t g_nameOffset_ParamPar = sizeof(STRUCT_HEAD) / 2;
		static const uint16_t g_nameSize_ParamPar = sizeof(char[32]) / 2;

		static const uint16_t g_baudrateOffset_CanPar = sizeof(STRUCT_HEAD) / 2;
		static const uint16_t g_baudrateSize_CanPar = sizeof(int32_t) / 2;
		static const uint16_t g_currentTimeOffset_CanPar = (sizeof(STRUCT_HEAD) + sizeof(int32_t)) / 2;
		static const uint16_t g_currentTimeSize_CanPar = sizeof(int32_t) / 2;
		static const uint16_t g_syncOffsetOffset_CanPar = (sizeof(STRUCT_HEAD) + sizeof(int32_t) + sizeof(int32_t)) / 2;
		static const uint16_t g_syncOffsetSize_CanPar = sizeof(int64_t) / 2;
		static const uint16_t g_syncStateOffset_CanPar = (sizeof(STRUCT_HEAD) + sizeof(int32_t) + sizeof(int32_t) + sizeof(int64_t)) / 2;
		static const uint16_t g_syncStateSize_CanPar = sizeof(int32_t) / 2;

		static const uint16_t g_baudrateOffset_UartPar = sizeof(STRUCT_HEAD) / 2;
		static const uint16_t g_baudrateSize_UartPar = sizeof(int32_t) / 2;
		static const uint16_t g_parityOffset_UartPar = (sizeof(STRUCT_HEAD) + sizeof(int32_t)) / 2;
		static const uint16_t g_paritySize_UartPar = sizeof(int32_t) / 2;
		static const uint16_t g_currentTimeOffset_UartPar = (sizeof(STRUCT_HEAD) + sizeof(int32_t) + sizeof(int32_t)) / 2;
		static const uint16_t g_currentTimeSize_UartPar = sizeof(int32_t) / 2;

		static const uint16_t g_rangeOffset_AdcPar2 = (sizeof(STRUCT_HEAD) + sizeof(float) + sizeof(float) + sizeof(float)) / 2;;
		static const uint16_t g_rangeSize_AdcPar2 = sizeof(float) / 2;

		static const uint16_t g_savedDutyOffset_Diagnost = sizeof(STRUCT_HEAD) / 2;
		static const uint16_t g_savedDutySize_Diagnost = sizeof(float) / 2;
		static const uint16_t g_currentDutyOffset_Diagnost = (sizeof(STRUCT_HEAD) + sizeof(float)) / 2;
		static const uint16_t g_currentDutySize_Diagnost = sizeof(float) / 2;
		static const uint16_t g_powerQualityOffset_Diagnost = (sizeof(STRUCT_HEAD) + sizeof(float) + sizeof(float)) / 2;
		static const uint16_t g_powerQualitySize_Diagnost = sizeof(int32_t) / 2;
		static const uint16_t g_dataQualityOffset_Diagnost = (sizeof(STRUCT_HEAD) + sizeof(float) + sizeof(float) + sizeof(int32_t)) / 2;
		static const uint16_t g_dataQualitySize_Diagnost = sizeof(int32_t) / 2;
		static const uint16_t g_syncQualityOffset_Diagnost = (sizeof(STRUCT_HEAD) + sizeof(float) + sizeof(float) + sizeof(int32_t) +
			sizeof(int32_t)) / 2;
		static const uint16_t g_syncQualitySize_Diagnost = sizeof(int32_t) / 2;
		static const uint16_t g_sensorQualityOffset_Diagnost = (sizeof(STRUCT_HEAD) + sizeof(float) + sizeof(float) + sizeof(int32_t) +
			sizeof(int32_t) + sizeof(int32_t)) / 2;
		static const uint16_t g_sensorQualitySize_Diagnost = sizeof(int32_t) / 2;
		static const uint16_t g_temperatureOffset_Diagnost = (sizeof(STRUCT_HEAD) + sizeof(float) + sizeof(float) + sizeof(int32_t) +
			sizeof(int32_t) + sizeof(int32_t) + sizeof(int32_t)) / 2;
		static const uint16_t g_temperatureSize_Diagnost = sizeof(float) / 2;

		static const uint16_t g_gpsStateOffset_SensorGps = sizeof(STRUCT_HEAD) / 2;
		static const uint16_t g_gpsStateSize_SensorGps = sizeof(int32_t) / 2;
		static const uint16_t g_gpsTimeOffset_SensorGps = (sizeof(STRUCT_HEAD) + sizeof(int32_t)) / 2;
		static const uint16_t g_gpsTimeSize_SensorGps = sizeof(int32_t) / 2;
		static const uint16_t g_ppsOffsetOffset_SensorGps = (sizeof(STRUCT_HEAD) + sizeof(int32_t) + sizeof(int32_t)) / 2;
		static const uint16_t g_ppsOffsetSize_SensorGps = sizeof(int32_t) / 2;
		static const uint16_t g_satellitesOffset_SensorGps = (sizeof(STRUCT_HEAD) + sizeof(int32_t) + sizeof(int32_t) + sizeof(int32_t)) / 2;
		static const uint16_t g_satellitesSize_SensorGps = sizeof(int32_t) / 2;

		static const uint16_t g_ptpStateOffset_SensorPtp = sizeof(STRUCT_HEAD) / 2;
		static const uint16_t g_ptpStateSize_SensorPtp = sizeof(int32_t) / 2;
		static const uint16_t g_ptpTimeOffset_SensorPtp = (sizeof(STRUCT_HEAD) + sizeof(int32_t)) / 2;
		static const uint16_t g_ptpTimeSize_SensorPtp = sizeof(int32_t) / 2;
		static const uint16_t g_ptpOffsetOffset_SensorPtp = (sizeof(STRUCT_HEAD) + sizeof(int32_t) + sizeof(int32_t)) / 2;
		static const uint16_t g_ptpOffsetSize_SensorPtp = sizeof(int32_t) / 2;
		static const uint16_t g_ptpSyncStateOffset_SensorPtp = (sizeof(STRUCT_HEAD) + sizeof(int32_t) + sizeof(int32_t) + sizeof(int32_t)) / 2;
		static const uint16_t g_ptpSyncStateSize_SensorPtp = sizeof(int32_t) / 2;
		static const uint16_t g_ptpMeanDelayOffset_SensorPtp = (sizeof(STRUCT_HEAD) + sizeof(int32_t) + sizeof(int32_t) + sizeof(int32_t) + sizeof(int32_t)) / 2;
		static const uint16_t g_ptpMeanDelaySize_SensorPtp = sizeof(int32_t) / 2;

		static const uint16_t g_sourceAddrOffset_MessagePar = (sizeof(STRUCT_HEAD) + sizeof(uint32_t)) / 2;
		static const uint16_t g_sourceAddrSize_MessagePar = sizeof(uint32_t) / 2;
	}

	namespace io
	{
		static const unsigned short g_maxDataSize = 2048;
		static const unsigned short g_modbusPacketSize = 240;
		static const uint16_t g_maxStreamDataSize = 1000;
		static const uint16_t g_ethernetBufferSize = 1024 * 12;
		static const unsigned long g_watchDogTimeout = 40000;
		static const uint32_t g_maxRepeatCount = 5;

		static const uint16_t g_hiddenProtocolPacketSize = 10;
		static const uint8_t g_hiddenProtocolRequest[g_hiddenProtocolPacketSize]
			= { 0xeb, 0xaa, 0xfe, 0xef, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66 };
	}

	namespace flash
	{
		static const unsigned int g_uiFlashStartAddress = 0x20000000;
		static const unsigned int g_uiFlashBootloaderAddress = 0x20000000;
		static const unsigned int g_uiFlashMainProgramAddress = 0x20020000;
		static const unsigned int g_uiFlashReserveCopyAddress = 0x20070000;
		static const unsigned int g_uiFlashDefaultCopyAddress = 0x200C0000;
		static const unsigned int g_uiFlashErrorLogAddress = 0x20180000;
		static const unsigned int g_uiFlashWorkTimeAddress = 0x201A0000;
		static const unsigned int g_uiFlashCheckingLogAddress = 0x201C0000;

		static const unsigned int g_uiFlashErrorLogSize = 128 * 1024;
		static const unsigned int g_uiFlashSectorSize = 0x00050000;
		static const unsigned int g_uiFlashPacketSize = 128;

		static const unsigned long g_ulFlashEraseTimeout = 20000;

		static const unsigned int g_uiFlashFooterSize = sizeof(BIN_FOOTER);
	}

	namespace diagnostic
	{
		static const int32_t g_maxStatisticSize = 60 * 60 * 24 * 3;
	}
}

#endif // zet7xxx_defines_h_