#ifndef errors_h_
#define errors_h_

#include "stdint.h"

/** ������������ ���� modbus. */
namespace zet7xxx
{
	namespace errors
	{
		/** ��� - ��� ������. */
		typedef int32_t ErrorCode, ResponseErrorCode, ExchangeErrorCode;

		/** ���� ������. */
		typedef enum : ErrorCode
		{
			/** ������ ���. */
			NoError = 0,
			/** ��� ����������� � ������ ������ � ������������. */
			ModbusNotConnected = -1,
			/** ������������ ��� ����������. */
			IncorrectDeviceType = -2,
			/** ������ �������� ��� ���� �������. ��������� ����� ����������. */
			AlreadyDone = -3,
			/** ���������� ������ ���� � ����� �������� ������� �����������. */
			DeviceNotFound = -4,
			/** ������ ��� ������ � COM-������. */
			ComPortError = -5,
			/** ������ ��� ����������� �������. */
			ConnectMasterError = -6,
			/** ������ ��� ���������� �������. */
			DisconnectMasterError = -7,
			/** ������ ��� �������� �������. */
			RestartMasterError = -8,
			/** ������ ��������� ��������. */
			SetBaudrateError = -9,
			/** ������ ������ ��������. */
			GetBaudrateError = -10,
			/**	������ �� �����������. */
			MasterNotConnected = -11,
			/**	������ �� �������.	*/
			ModuleNotActivated = -12,
			/**	����������� ����������.	*/
			NoDevices = -13,
			/** ������ �����������. */
			DiagnosticError = -14,
			/** �� ��������������. */
			NotSupported = -15,
			/** ������ ������������. */
			BadConfiguration = -16,
			/** ������ ��������� �������� ��������. */
			SetParityError = -17,
			/** ������ ������ �������� ��������. */
			GetParityError = -18,
		} ErrorCodes;

		/**	���� ������ ��� ��������� ������ �� MODBUS ������. */
		typedef enum : ResponseErrorCode
		{
			/** ������ ���. */
			recNoError = 0,
			/**	�������� �����.	*/
			recShortResponse = -1,
			/**	������������ ������ ���������� ������� � ������. */
			recErrorNodes = -2,
			/**	������������ ������ ������� � ������. */
			recErrorCommands = -3,
			/**	������������ ������ ������ ������� � ������. */
			recErrorWriteAddress = -4,
			/**	������������ ����������� ���������� ���� ������� � ������. */
			recErrorWriteQuantity = -5,
			/**	������ ����������� ����� ������. */
			recErrorCrc = -6,
			recTimeout = -7,
		} ResponseErrorCodes;

		/**	���� ������ ��� ������ ������� � ��������. */
		typedef enum : ExchangeErrorCode
		{
			/** ������ ���. */
			eecNoError = 0,
			/** ������� ��������� ������. */
			eecTimeout = -1,
			/**	������ � ������ �� ����������. */
			eecResponseError = -2,
			/** ������ ��� ������ � ��������. */
			eecMasterError = -3,
			/** ���������� ������ ������� �������������. */
			eecUserBreak = -4,
		} ExchangeErrorCodes;


		/** ��� - ��� ������. */
		typedef int32_t DiagnosticErrorCode;
		/**	���� ������ ��� �����������. */
		typedef enum : DiagnosticErrorCode
		{
			/** ������ ���. */
			decNoError = 0,
			/** ��������������� �������� �����������. */
			decNoParameter = -1,
			/** ��������������� ���������� �����������. */
			decNoDevice = -2,
		} DiagnosticErrorCodes;

		/** ��� - ��� ������. */
		typedef int32_t RegistersStorageErrorCode;
		/**	���� ������ ��� ������ ���������. */
		typedef enum : RegistersStorageErrorCode
		{
			/** ������ ���. */
			rsecNoError = 0,
			/** ������� �����������. */
			rsecNoRegister = -1,
			/** ���������� �����������. */
			rsecNoDevice = -2,
		} RegistersStorageErrorCodes;
	}
}

#endif // errors_h_