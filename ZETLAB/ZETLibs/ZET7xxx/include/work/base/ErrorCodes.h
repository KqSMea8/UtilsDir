#ifndef errors_h_
#define errors_h_

#include "stdint.h"

/** Пространство имен modbus. */
namespace zet7xxx
{
	namespace errors
	{
		/** Тип - код ошибки. */
		typedef int32_t ErrorCode, ResponseErrorCode, ExchangeErrorCode;

		/** Коды ошибок. */
		typedef enum : ErrorCode
		{
			/** Ошибок нет. */
			NoError = 0,
			/** Нет подключения к модулю работы с устройствами. */
			ModbusNotConnected = -1,
			/** Неправильный тип устройства. */
			IncorrectDeviceType = -2,
			/** Данная операция уже была вызвана. Повторный вызов невозможен. */
			AlreadyDone = -3,
			/** Устройство такого типа с таким серийным номером отсутствует. */
			DeviceNotFound = -4,
			/** Ошибка при работе с COM-портом. */
			ComPortError = -5,
			/** Ошибка при подключении мастера. */
			ConnectMasterError = -6,
			/** Ошибка при отключении мастера. */
			DisconnectMasterError = -7,
			/** Ошибка при рестарте мастера. */
			RestartMasterError = -8,
			/** Ошибка установки скорости. */
			SetBaudrateError = -9,
			/** Ошибка чтения скорости. */
			GetBaudrateError = -10,
			/**	Мастер не активирован. */
			MasterNotConnected = -11,
			/**	Модуль не запущен.	*/
			ModuleNotActivated = -12,
			/**	Отсутствуют устройства.	*/
			NoDevices = -13,
			/** Ошибка диагностики. */
			DiagnosticError = -14,
			/** Не поддерживается. */
			NotSupported = -15,
			/** Ошибка конфигурации. */
			BadConfiguration = -16,
			/** Ошибка установки контроля четности. */
			SetParityError = -17,
			/** Ошибка чтения контроля четности. */
			GetParityError = -18,
		} ErrorCodes;

		/**	Коды ошибок при получении ответа на MODBUS запрос. */
		typedef enum : ResponseErrorCode
		{
			/** Ошибок нет. */
			recNoError = 0,
			/**	Неполный ответ.	*/
			recShortResponse = -1,
			/**	Несовпадение адреса устройства запроса и ответа. */
			recErrorNodes = -2,
			/**	Несовпадение команд запроса и ответа. */
			recErrorCommands = -3,
			/**	Несовпадение адреса записи запроса и ответа. */
			recErrorWriteAddress = -4,
			/**	Несовпадение количкества записанных байт запроса и ответа. */
			recErrorWriteQuantity = -5,
			/**	Ошибка контрольной суммы ответа. */
			recErrorCrc = -6,
			recTimeout = -7,
		} ResponseErrorCodes;

		/**	Коды ошибок при обмене данными с мастером. */
		typedef enum : ExchangeErrorCode
		{
			/** Ошибок нет. */
			eecNoError = 0,
			/** Таймаут получения ответа. */
			eecTimeout = -1,
			/**	Ошибка в ответе от устройства. */
			eecResponseError = -2,
			/** Ошибка при работе с мастером. */
			eecMasterError = -3,
			/** Прерывание обмена данными пользователем. */
			eecUserBreak = -4,
		} ExchangeErrorCodes;


		/** Тип - код ошибки. */
		typedef int32_t DiagnosticErrorCode;
		/**	Коды ошибок при диагностике. */
		typedef enum : DiagnosticErrorCode
		{
			/** Ошибок нет. */
			decNoError = 0,
			/** Диагностический параметр отсутствует. */
			decNoParameter = -1,
			/** Диагностируемое устройство отсутствует. */
			decNoDevice = -2,
		} DiagnosticErrorCodes;

		/** Тип - код ошибки. */
		typedef int32_t RegistersStorageErrorCode;
		/**	Коды ошибок при чтении регистров. */
		typedef enum : RegistersStorageErrorCode
		{
			/** Ошибок нет. */
			rsecNoError = 0,
			/** Регистр отсутствует. */
			rsecNoRegister = -1,
			/** Устройство отсутствует. */
			rsecNoDevice = -2,
		} RegistersStorageErrorCodes;
	}
}

#endif // errors_h_