#ifndef zet7xxx_types_h_
#define zet7xxx_types_h_

#include <Zet7xxxDefinitions.h>

#include <stdint.h>
#include <vector>
#include <map>

namespace zet7xxx
{
	/** Тип - идентификатор устройства (тип устройства + серийный номер)*/
	typedef std::pair<ZET7XXX_DEVICE_TYPE, uint64_t> Device, *PDevice;
	/** Тип - список устройств */
	typedef std::vector<Device> DeviceList;
	
	typedef std::vector<uint16_t> Configuration, Description;

	/** Тип - состояние подключения мастера */
	typedef uint8_t MasterConnectState;
	/** Состояния подключения мастера	*/
	typedef enum : MasterConnectState
	{
		mcsNotConnected = 0,
		mcsConnecting,
		mcsConnected
	} MasterConnectStates;

	/** Тип - вариант поиска датчиков */
	typedef uint8_t FindMode;
	/** Состояния подключения мастера	*/
	typedef enum : FindMode
	{
		fmNormal = 0,
		fmEx
	} FindModes;

#pragma pack (push, 1)
	typedef struct _BIN_FOOTER
	{
		uint32_t dataCrc32;
		uint32_t dataSize;
		uint16_t hardwareVersion;
		uint16_t firmwareVersion;
		int8_t deviceCode[16];
		int8_t compileTime[20];
		uint8_t reserve[8];
		uint32_t footerCrc32;
	} BIN_FOOTER;

	typedef struct _VERSION_STRUCT
	{
		uint16_t hardwareVersion;
		uint16_t firmwareVersion;
		uint8_t deviceCode[16];
	} VERSION_STRUCT, *PVERSION_STRUCT;

	typedef struct _ERROR_STRUCT
	{
		uint32_t prefix;
		uint32_t recordNumber;
		uint32_t code;
		char name[32];
		uint32_t rtc;
		uint8_t reserve[14];
		uint16_t crc16;
	} ERROR_STRUCT, *PERROR_STRUCT;
	typedef std::vector<ERROR_STRUCT> ErrorList;

	typedef struct _CAN_PACKET
	{
		union
		{
			uint16_t id;
			uint8_t address;
		};
		uint16_t length;
		union
		{
			uint8_t b[8];
			float f[2];
			int16_t s[4];
			int32_t t[2];
			int64_t tt;
		};
	} CAN_PACKET, *pCAN_PACKET;
#pragma pack(pop)

	namespace modbus
	{
		/** Тип - адрес MODBUS */
		typedef uint8_t ModbusAddress;
		/** Тип - команда MODBUS */
		typedef uint8_t ModbusCommand;
		/** MODBUS команды	*/
		typedef enum : ModbusCommand
		{
			mcUnknown = 0,
			mcReadCoilStatus = 1,				//чтение значений из нескольких регистров флагов 
			mcReadDiscreteInputs,				//чтение значений из нескольких дискретных регистров 
			mcReadHoldingRegisters,				//чтение значений из нескольких регистров хранения 
			mcReadInputRegisters,				//чтение значений из нескольких регистров ввода
			mcForceSingleCoil,					//запись значения одного флага 
			mcPresetSingleRegister,				//запись значения в один регистр хранения 
			mcForce_Multiple_Coils = 15,		//запись значений в несколько регистров флагов 
			mcPresetMultipleRegisters,			//запись значений в несколько регистров хранения 
			mcReadInputRegistersEx = 20,
			mcMarkDevice = 32,
			mcFlashErase = 66,
			mcFlashWrite,
			mcFlashRead,
			mcGetVersion,
			mcRestart,
			mcStopDataStream,
		} ModbusCommands;

		/** Тип - адрес данных MODBUS */
		typedef uint16_t ModbusDataAddress;
		/** Тип - тип данных MODBUS */
		typedef uint8_t ModbusDataType;
		/** Типы данных MODBUS */
		typedef enum : ModbusDataType
		{
			mdtUnknown = 0,
			mdtStream,
			mdtParam,
			mdtMessage,
		} ModbusDataTypes;

		/** Тип - идентификатор устройства (адрес устройства + адрес данных)*/
		typedef std::pair<ModbusAddress, ModbusDataAddress> DataChannel, *PDataChannel;

		/** Тип - структура регистра */
		typedef struct RegisterValue
		{
			uint16_t address;
			uint16_t size;
			type_of_arg type;

			bool operator == (const RegisterValue& val) const
			{
				return (address == val.address &&
					size == val.size &&
					type == val.type);
			}
		} RegisterValue;
		/** Тип - список значений регистров */
		typedef std::vector<RegisterValue> RegisterValueList;
		/** Тип - значение регистра устройства*/
		typedef struct : RegisterValue
		{
			ModbusAddress device;
		} DeviceRegisterValue;

		typedef std::vector<uint16_t> RegisterData, StreamData;	
	}

	typedef struct _DeviceInfo
	{
		Device device;
		wchar_t sDeviceName[64];
		Device master;
		wchar_t sMasterName[64];
		modbus::ModbusAddress node;
		uint32_t baudRate;
		uint8_t parity;
		uint8_t type;
		VERSION_STRUCT version;
		int32_t status;
		wchar_t sComputerIp[16];
		wchar_t sComputerName[32];
		float fChannelMaxLevel;
	} DeviceInfo;

	typedef struct _ChannelInfo
	{
		wchar_t sName[sizeof(((CHANNEL_PAR*)(0))->channel_name) +1];
		float frequency;
		float minLevel;
		float maxLevel;
		float resolution;
		float sense;
		float reference;
		wchar_t sConversion[sizeof(((CHANNEL_PAR*)(0))->measure) +1];
		modbus::ModbusDataType mdtType;
		modbus::ModbusDataAddress dataAddress;
		Device device;
		bool bMessage;
	} ChannelInfo;
	typedef struct _Representative
	{
		wchar_t full_name[64];
		wchar_t post[64];
	} Representative;
}

#endif // zet7xxx_types_h_