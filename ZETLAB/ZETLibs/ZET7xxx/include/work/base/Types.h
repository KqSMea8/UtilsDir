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
	/** Тип - событие*/
	typedef std::pair<SYSTEMTIME, std::wstring> LogEvent;
	/** Тип - список событий */
	typedef std::vector<LogEvent> LogEventList;
	/** Тип - функциональный модуль */
	typedef uint8_t FunctionModuleType;
	/** Функциональные модули	*/
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
	/** Тип - состояние функционального модуля */
	typedef uint8_t FunctionModuleState;
	/** Состояния функционального модуля	*/
	typedef enum : FunctionModuleState
	{
		fmsError = 0,
		fmsStopped,
		fmsStarted,
		fmsNeedToStop,
		fmsNeedToStart,
	} FunctionModuleStates;

	/** Тип - состояние функционального модуля */
	typedef uint32_t Option;
	/** Состояния функционального модуля	*/
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
		/** Тип - диагностическое поле */
		typedef uint8_t DiagnosticParameter;
		/** Диагностические поля */
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

		/** Тип - состояние диагностического параметра */
		typedef uint8_t DiagnosticParameterState;
		/** Диагностические поля */
		typedef enum : DiagnosticParameterState
		{
			dpsGood = 0,
			dpsBad,
		} DiagnosticParameterStates;

		#define DIAGNOSTIC_PARAMETER_NAME(parameterName)				\
		static const TCHAR *parameterName[diagnosticParametersSize] = { \
			_T("Запросов"),												\
			_T("Ответов"),												\
			_T("Нет ответа"),											\
			_T("Неполный ответ"),										\
			_T("Ошибка адреса"),										\
			_T("Ошибка команды"),										\
			_T("Ошибка CRC"),											\
			_T("Ошибка мастера"),										\
			_T("Поток данных, Гц"),										\
			_T(""),														\
			_T(""),														\
			_T(""),														\
			_T(""),														\
			_T("Тек. время (RS-485)"),									\
			_T("Тек. время (CAN)"),										\
			_T("Смещение, нс (CAN)"),									\
			_T("Сред. смещ, нс (CAN)"),									\
			_T("СКО смещ, нс (CAN)"),									\
			_T("Сост. синхр. (CAN)"),									\
			_T("Сохр. скв. ШИМ, %"),									\
			_T("Тек. скв. ШИМ, %"),										\
			_T("Температура, °C"),										\
			_T("Состояние GPS"),										\
			_T("Время по GPS"),											\
			_T("Смещение PPS (GPS), нс"),								\
			_T("Спутники (GPS)"),										\
			_T("Состояние PTP"),										\
			_T("Время по PTP"),											\
			_T("Смещение (PTP), нс"),									\
			_T("Сост. синхр. (PTP)"),									\
			_T("Сред. задерж. сети (PTP), нс"),							\
			_T("Качество питания"),										\
			_T("Качество данных"),										\
			_T("Качество синхронизации"),								\
			_T("Состояние чувств. элемента")							\
		};

		#define syncStateCanParSize										4
		#define SYNC_STATE_NAME_CAN_PAR(stateName)						\
		static const TCHAR *stateName[syncStateCanParSize] = {			\
			_T("Задатчик"),												\
			_T("Отсутствует"),											\
			_T("Выполняется..."),										\
			_T("Выполнена"),											\
		};

		#define gpsStateSensorGpsSize									5
		#define GPS_STATE_NAME_SENSOR_GPS(stateName)					\
		static const TCHAR *stateName[gpsStateSensorGpsSize] = {		\
			_T("Отключен"),												\
			_T("Ошибка NMEA"),											\
			_T("Поиск спутников"),										\
			_T("Синхронизация"),										\
			_T("Синхронизирован"),										\
		};

		#define ptpStateSensorPtpSize									11
		#define PTP_STATE_NAME_SENSOR_PTP(stateName)					\
		static const TCHAR *stateName[ptpStateSensorPtpSize] = {		\
			_T("Не доступен"),											\
			_T("Инициализация"),										\
			_T("Ошибка"),												\
			_T("Отключен"),												\
			_T("Ожидание"),												\
			_T("Ведущие часы"),											\
			_T("Ведущие часы"),											\
			_T("Пассивен"),												\
			_T("Ожидание"),												\
			_T("Ведомые часы"),											\
			_T("Установка часов"),										\
		};

		#define ptpSyncStateSensorPtpSize								4
		#define PTP_SYNC_STATE_NAME_SENSOR_PTP(stateName)				\
		static const TCHAR *stateName[ptpSyncStateSensorPtpSize] = {	\
			_T("Отсутствует"),											\
			_T("Выполняется..."),										\
			_T("Выполнена"),											\
			_T("По сети CAN"),											\
		};

		#define powerQualityDiagnostSize								4
		#define POWER_QUALITY_NAME_DIAGNOST(name)						\
		static const TCHAR *name[powerQualityDiagnostSize] = {			\
			_T("Не поддерж."),											\
			_T("Низкое"),												\
			_T("Высокое"),												\
			_T("Нормальное"),											\
		};

		#define dataQualityDiagnostSize									4
		#define DATA_QUALITY_NAME_DIAGNOST(name)						\
		static const TCHAR *name[dataQualityDiagnostSize] = {			\
			_T("Не поддерж."),											\
			_T("Плохо"),												\
			_T("Хорошо"),												\
			_T("Отлично"),												\
		};

		#define syncQualityDiagnostSize									4
		#define SYNC_QUALITY_NAME_DIAGNOST(name)						\
		static const TCHAR *name[syncQualityDiagnostSize] = {			\
			_T("Не поддерж."),											\
			_T("Плохо"),												\
			_T("Хорошо"),												\
			_T("Отлично"),												\
		};

		#define sensorQualityDiagnostSize								3
		#define SENSOR_QUALITY_NAME_DIAGNOST(name)						\
		static const TCHAR *name[sensorQualityDiagnostSize] = {			\
			_T("Не поддерж."),											\
			_T("Ошибка"),												\
			_T("Хорошо"),												\
		};
	}

	namespace firmware
	{
		/** Тип - состояние обновления */
		typedef uint8_t UpdateState;
		/** Состояния обновления */
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
			_T("ПО успешно обновлено"),										\
			_T("ПО не обновлялось"),										\
			_T("нет доступа к директории обновлений"),						\
			_T("файл обновления не найден"),								\
			_T("файл обновления поврежден"),								\
			_T("обновление для другой версии платы"),						\
			_T("ошибка обновления при обмене данными с устройством"),		\
			_T("ошибка обновления при стирании flash"),						\
			_T("ошибка обновления при записи на flash"),					\
			_T("ошибка обновления при сравнении данных на flash"),			\
			_T("ошибка обновления при чтении с flash"),						\
		};
	}
}

#endif // types_h_