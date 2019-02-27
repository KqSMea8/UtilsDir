#ifndef types_h_
#define types_h_

#include <ZETSimple\include\time\time_auto_link.h>
#include <ZETSimple\include\trends\trends_auto_link.h>
#include <ZETSimple\include\debugtools\debugtools_auto_link.h>

#include <ZET7xxx\include\base\ZET7xxxTypes.h>
#include <ZET7xxx\include\base\ZET7xxxDefines.h>

//#define _TRACE_IO_TX_BUFFER_
//#define _TRACE_IO_TX_INFO_
//#define _TRACE_IO_TX_PARAMS_
//#define _TRACE_IO_RX_BUFFER_
//#define _TRACE_IO_RX_INFO_
//#define _TRACE_IO_RIGHT_ANSWER_
//#define _TRACE_IO_TIMEOUT_

using namespace zet7xxx::io;

namespace zet7xxx
{
	/** ��� - �������*/
	typedef std::pair<SYSTEMTIME, std::wstring> LogEvent;
	/** ��� - ������ ������� */
	typedef std::vector<LogEvent> LogEventList;
	/** ��� - �������������� ������ */
	typedef uint8_t FunctionModuleType;
	/** �������������� ������	*/
	typedef enum : FunctionModuleType
	{
		fmtNoModule = 0,
		fmtIdle,
		fmtSlaveFinder,
		fmtErrorReader,
		fmtFirmwareWriter,
		fmtFirmwareReader,
		fmtDeviceMarker,
		fmtDeviceRestarter,
		fmtDescriptionReader,
		fmtHiddenProtocol,
		fmtConfigurationWriter,
		fmtDataExchangeDiagnostician,
		fmtLineSynchronizationDiagnostician,
		fmtNetSynchronizationDiagnostician,
		fmtDataQualityDiagnostician,
		fmtValuesReader,
		fmtDataReader,
	} FunctionModuleTypes;
	/** ��� - ��������� ��������������� ������ */
	typedef uint8_t FunctionModuleState;
	/** ��������� ��������������� ������	*/
	typedef enum : FunctionModuleState
	{
		fmsError = 0,
		fmsStopped,
		fmsStarted,
		fmsNeedToStop,
		fmsNeedToStart,
	} FunctionModuleStates;

	/** ��� - ��������� ��������������� ������ */
	typedef uint32_t Option;
	/** ��������� ��������������� ������	*/
	typedef enum : Option
	{
		oStopDataStream = 0,
		oEventLog,
		oNoStorage,
		oNoPinger,
	} Options;

#pragma pack (push, 1)

	typedef struct _MODBUS_PACKAGE_STRUCT
	{
		FunctionModuleType functionModuleType;
		unsigned char waitAnswerUntilTimeout;
		unsigned char node;
		unsigned char command;
		unsigned short address;
		unsigned short quantity;
		unsigned short size;
		unsigned char buffer[g_maxDataSize];
		unsigned short crc16;
		unsigned long maxWaitTimeout;
						
		_MODBUS_PACKAGE_STRUCT()
		{
			clear();
		};

		_MODBUS_PACKAGE_STRUCT(_In_ const _MODBUS_PACKAGE_STRUCT& value)
		{
			*this = value;
		}
		
		void operator=(_In_ const _MODBUS_PACKAGE_STRUCT &value)
		{
			functionModuleType = value.functionModuleType;
			waitAnswerUntilTimeout = value.waitAnswerUntilTimeout;
			node = value.node;
			command = value.command;
			address = value.address;
			quantity = value.quantity;
			size = value.size;
			crc16 = value.crc16;
			maxWaitTimeout = value.maxWaitTimeout;
			memcpy_s(buffer, g_maxDataSize, value.buffer, g_maxDataSize);
		}
		
		void clear()
		{
			functionModuleType = fmtNoModule;
			waitAnswerUntilTimeout = 0;
			node = 0;
			command = 0;
			address = 0;
			quantity = 0;
			size = 0;
			crc16 = 0;
			maxWaitTimeout = 0;
			memset(buffer, 0, g_maxDataSize);
		}

	} MODBUS_PACKAGE_STRUCT, *PMODBUS_PACKAGE_STRUCT;

#pragma pack(pop)

	namespace diagnostic
	{
		/** ��� - ��������������� ���� */
		typedef uint8_t DiagnosticParameter;
		/** ��������������� ���� */
		typedef enum : DiagnosticParameter
		{
			dfExchangeCount = 0,
			dfNoError,
			dfNoResponse,
			dfShortResponse,
			dfNodeError,
			dfCommandError,
			dfCrcError,
			dfMasterError,
			dfDataStream,
			dfStreamDataRate,
			dfCommandDataRate,
			dfDataRate,
			dfExchangeRate,
			dfCurrentTimeUart,
			dfCurrentTimeCan,
			dfSyncOffset,
			dfSyncOffsetMean,
			dfSyncOffsetStdDev,
			dfSyncState,
			dfSavedDuty,
			dfCurrentDuty,
			dfTemperature,
			dfGpsState,
			dfGpsTime,
			dfGpsOffset,
			dfGpsSatellites,
			dfPtpState,
			dfPtpTime,
			dfPtpOffset,
			dfPtpSyncState,
			dfPtpMeanPathDelay,
			dfPowerQuality,
			dfDataQuality,
			dfSynchronizationQuality,
			dfSensorQuality,

			diagnosticParametersSize,
		} DiagnosticParameters;

		/** ��� - ��������� ���������������� ��������� */
		typedef uint8_t DiagnosticParameterState;
		/** ��������������� ���� */
		typedef enum : DiagnosticParameterState
		{
			dpsGood = 0,
			dpsBad,
		} DiagnosticParameterStates;

		#define DIAGNOSTIC_PARAMETER_NAME(parameterName)				\
		static const TCHAR *parameterName[diagnosticParametersSize] = { \
			_T("��������"),												\
			_T("�������"),												\
			_T("��� ������"),											\
			_T("�������� �����"),										\
			_T("������ ������"),										\
			_T("������ �������"),										\
			_T("������ CRC"),											\
			_T("������ �������"),										\
			_T("����� ������, ��"),										\
			_T(""),														\
			_T(""),														\
			_T(""),														\
			_T(""),														\
			_T("���. ����� (RS-485)"),									\
			_T("���. ����� (CAN)"),										\
			_T("��������, �� (CAN)"),									\
			_T("����. ����, �� (CAN)"),									\
			_T("��� ����, �� (CAN)"),									\
			_T("����. �����. (CAN)"),									\
			_T("����. ���. ���, %"),									\
			_T("���. ���. ���, %"),										\
			_T("�����������, �C"),										\
			_T("��������� GPS"),										\
			_T("����� �� GPS"),											\
			_T("�������� PPS (GPS), ��"),								\
			_T("�������� (GPS)"),										\
			_T("��������� PTP"),										\
			_T("����� �� PTP"),											\
			_T("�������� (PTP), ��"),									\
			_T("����. �����. (PTP)"),									\
			_T("����. ������. ���� (PTP), ��"),							\
			_T("�������� �������"),										\
			_T("�������� ������"),										\
			_T("�������� �������������"),								\
			_T("��������� ������. ��������")							\
		};

		#define syncStateCanParSize										4
		#define SYNC_STATE_NAME_CAN_PAR(stateName)						\
		static const TCHAR *stateName[syncStateCanParSize] = {			\
			_T("��������"),												\
			_T("�����������"),											\
			_T("�����������..."),										\
			_T("���������"),											\
		};

		#define gpsStateSensorGpsSize									5
		#define GPS_STATE_NAME_SENSOR_GPS(stateName)					\
		static const TCHAR *stateName[gpsStateSensorGpsSize] = {		\
			_T("��������"),												\
			_T("������ NMEA"),											\
			_T("����� ���������"),										\
			_T("�������������"),										\
			_T("���������������"),										\
		};

		#define ptpStateSensorPtpSize									11
		#define PTP_STATE_NAME_SENSOR_PTP(stateName)					\
		static const TCHAR *stateName[ptpStateSensorPtpSize] = {		\
			_T("�� ��������"),											\
			_T("�������������"),										\
			_T("������"),												\
			_T("��������"),												\
			_T("��������"),												\
			_T("������� ����"),											\
			_T("������� ����"),											\
			_T("��������"),												\
			_T("��������"),												\
			_T("������� ����"),											\
			_T("��������� �����"),										\
		};

		#define ptpSyncStateSensorPtpSize								4
		#define PTP_SYNC_STATE_NAME_SENSOR_PTP(stateName)				\
		static const TCHAR *stateName[ptpSyncStateSensorPtpSize] = {	\
			_T("�����������"),											\
			_T("�����������..."),										\
			_T("���������"),											\
			_T("�� ���� CAN"),											\
		};

		#define powerQualityDiagnostSize								4
		#define POWER_QUALITY_NAME_DIAGNOST(name)						\
		static const TCHAR *name[powerQualityDiagnostSize] = {			\
			_T("�� �������."),											\
			_T("������"),												\
			_T("�������"),												\
			_T("����������"),											\
		};

		#define dataQualityDiagnostSize									4
		#define DATA_QUALITY_NAME_DIAGNOST(name)						\
		static const TCHAR *name[dataQualityDiagnostSize] = {			\
			_T("�� �������."),											\
			_T("�����"),												\
			_T("������"),												\
			_T("�������"),												\
		};

		#define syncQualityDiagnostSize									4
		#define SYNC_QUALITY_NAME_DIAGNOST(name)						\
		static const TCHAR *name[syncQualityDiagnostSize] = {			\
			_T("�� �������."),											\
			_T("�����"),												\
			_T("������"),												\
			_T("�������"),												\
		};

		#define sensorQualityDiagnostSize								3
		#define SENSOR_QUALITY_NAME_DIAGNOST(name)						\
		static const TCHAR *name[sensorQualityDiagnostSize] = {			\
			_T("�� �������."),											\
			_T("������"),												\
			_T("������"),												\
		};
	}

	namespace firmware
	{
		/** ��� - ��������� ���������� */
		typedef uint8_t UpdateState;
		/** ��������� ���������� */
		typedef enum : UpdateState
		{
			ufsUpdateSuccessful = 0,
			ufsWasNoUpdated,
			ufsNoAccessToUpdateDirectory,
			ufsUpdateNotFound,
			ufsUpdateCorrupted,
			ufsAnotherHardware,
			ufsDataExchangeError,
			ufsFlashEraseError,
			ufsFlashWriteError,
			ufsFlashCompareError,
			ufsFlashReadError,

			ZET7XXX_UPDATE_FIRMWARE_STATE_COUNT,
		} UpdateStates;

		#define ZET7XXX_UPDATE_FIRMWARE_STATE(state) 						\
		static const TCHAR *state[ZET7XXX_UPDATE_FIRMWARE_STATE_COUNT] = { 	\
			_T("�� ������� ���������"),										\
			_T("�� �� �����������"),										\
			_T("��� ������� � ���������� ����������"),						\
			_T("���� ���������� �� ������"),								\
			_T("���� ���������� ���������"),								\
			_T("���������� ��� ������ ������ �����"),						\
			_T("������ ���������� ��� ������ ������� � �����������"),		\
			_T("������ ���������� ��� �������� flash"),						\
			_T("������ ���������� ��� ������ �� flash"),					\
			_T("������ ���������� ��� ��������� ������ �� flash"),			\
			_T("������ ���������� ��� ������ � flash"),						\
		};
	}
}

#endif // types_h_