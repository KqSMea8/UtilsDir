//-----------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
#include <windows.h>

#define SOURSE_NAME_MAX_SIZE		400	// ������������ ������ ����� ���������, ���������� ������
#define	MAX_ERROR_SOURCE_COUNT		7	// ���������� ���������� ������
#define	MAX_ERROR_SOLUTIONS_COUNT	3	// ���������� ���������� ������
//-----------------------------------------------------------------------------
enum ErrorSourses		// ������������ ���� ���������� ������	 
{
	USB_Error = 0,		// 0 - ������ � ����������� ����������� �� USB 
	Ethernet_Error,		// 1 - ������ � ����������� ����������� �� Ethernet 
	ZetKey_Error,		// 2 - ������ � ZetKey
	ZeroVID_Error,		// 3 - ������ � ����������� �� ����������� �������� (VID/PID = 0) 
	Modbus_Error,		// 4 - ������ ��������� � Modbus
	ZetServer_Error,	// 5 - ������ ��������� � ZetServer
	Other_Error			// 6 - ������ �� ������ ���������� 
};
//-----------------------------------------------------------------------------
struct ErrorInfoStruct
{
	size_t				Struct_Size;			// ������ ���������
	/*	���� ��� ��� ������ ��������
	true	- ������ ���������
	false	- ���� ������ ������ ���*/
	BOOL				Error_Arose;

	wchar_t				Error_Sourse_Name	[SOURSE_NAME_MAX_SIZE];	// ��� ��������� ���������� ������ (���� �����)

	wchar_t				Solution_Helpstring	[MAX_ERROR_SOLUTIONS_COUNT][MAX_PATH];	// ������� �������� ������� ������� ��������
	wchar_t				Solution_ExePathName[MAX_ERROR_SOLUTIONS_COUNT][MAX_PATH];	// ������ ���� � �������������� ��������� 
	wchar_t				Solution_CommandLine[MAX_ERROR_SOLUTIONS_COUNT][MAX_PATH];	// ��������� ������ ��� ������� ���������

	ErrorSourses		Error_Sourse_ID;				// ������������� ��������� ������
	long				Error_Number;					// ����� ��������� ������
	/*	���� ��� ���������� �������� �������� 7xxx
	TRUE	- ������ �������� � ���������� 7���
	FALSE	- ������ �������� � ������� ����������
	-1		- ���������� ���������� ��������� � ������� �������� ������ (�� ����� ������������ ��������� �� ����)*/
	BOOL				Is7xxxDevice;							
	long				Device_Type;					// (����������� ���� ���� Is7xxxDevice �� ����� -1) ��� ���������� � ������� �������� ������
	
	unsigned long long	Device_Serial;					// (����������� ���� ���� Is7xxxDevice �� ����� -1) �������� ����� ���������� � ������� �������� ������ 
	wchar_t				Device_Name	[MAX_PATH];			// �������� ����������

	ErrorInfoStruct()
		: Struct_Size		(sizeof(ErrorInfoStruct))
		, Error_Sourse_ID	(USB_Error)
		, Error_Number		(NULL)
		, Error_Arose		(TRUE)
		, Is7xxxDevice		(-1)
		, Device_Type		(-1)
		, Device_Serial		(-1)
	{
		ZeroMemory(Error_Sourse_Name, SOURSE_NAME_MAX_SIZE * sizeof(wchar_t));
		ZeroMemory(Device_Name, MAX_PATH * sizeof(wchar_t));

		for (int Solution = 0; Solution < MAX_ERROR_SOLUTIONS_COUNT; Solution++)
		{
			ZeroMemory(Solution_Helpstring[Solution], MAX_PATH * sizeof(wchar_t));
			ZeroMemory(Solution_CommandLine[Solution], MAX_PATH * sizeof(wchar_t));
			ZeroMemory(Solution_ExePathName[Solution], MAX_PATH * sizeof(wchar_t));			
		}
	}
	bool ErrorInfoStruct::operator==(const ErrorInfoStruct& Val)
	{
		bool res = false;
		for (int Solution = 0; Solution < MAX_ERROR_SOLUTIONS_COUNT; Solution++)
		{
			if ((wcscmp(Solution_Helpstring[Solution], Val.Solution_Helpstring[Solution]) == 0) 
				&& (wcscmp(Solution_CommandLine[Solution], Val.Solution_CommandLine[Solution]) == 0)
				&& (wcscmp(Solution_ExePathName[Solution], Val.Solution_ExePathName[Solution]) == 0))
				res = true;
			else
			{
				res = false;
				break;
			}
		}
		if (res)
			return Struct_Size == Val.Struct_Size	 && Error_Sourse_ID == Val.Error_Sourse_ID	&& (wcscmp(Error_Sourse_Name, Val.Error_Sourse_Name) == 0)
				&& Error_Number == Val.Error_Number	 && Error_Arose == Val.Error_Arose			&& Is7xxxDevice == Val.Is7xxxDevice
				&& Device_Type == Val.Device_Type	 && Device_Serial == Val.Device_Serial		&& (wcscmp(Device_Name, Val.Device_Name) == 0);
		else
			return false;
	}

};
//-----------------------------------------------------------------------------
/**���� ������. �� ������ � ErrorCode �� NuGet*/
enum ErrorCodes : long
{
	NoError = 0,	/** ������ ��� */
	/** ��������� ������. ����������� ������ ��������� �� ��������. */
	FatalError = -1,
	/** ���������� �������� */
	InvalidParams = -2,
	/** ������ ������������� */
	Err_Initialization = -3,
	/** ������������ ������ ��������� */
	Err_Size_Structure = -4,
	/** ������������ ID ��������� */
	Err_ID_Structure = -5,
	/** ������ ��� ����������� ��������� */
	Err_Copy_Structure = -6,
	/** ������ ��� ����������� ������ */
	Err_Copy_Data = -7,
	/** ������ ��� �������� ������� */
	Err_Create_Array = -8,
	/** ������ ��� �������� */
	Err_Calc = -9,
	/** ������� ��������� */
	Err_NULL_Pointer = -10,
	/** ������ ��� �������� ���������*/
	Err_Create_Structure = -11,
	/** ���������� �������� ������ �����������*/
	Err_Value_Less = -12,
	/** ���������� �������� ������ �����������*/
	Err_Value_More = -13,
	/** ������ ������*/
	Err_String_Empty = -14,
	/** ������ ����� ������� ��������� ������*/
	Err_String_Less = -15,
	/** ������ ����� ������� ������� ������*/
	Err_String_More = -16,
	/** ��� �������*/
	Err_Access_Denied = -17,
	/** ������� ������*/
	Err_Size_Zero = -18,
	/** �������� IP-�����, �.�. 255.255.255.255 */
	Err_Invalid_IP = -19,
	/** ��� �����*/
	Err_Not_Ping = -20,
	/** ������� �������� �� ����������*/
	Err_Function_Not_Defined = -21,
	/** ������ ��� �������� �������� ������*/
	Err_Create_Thread = -22,
	/** ������� ����� �� �������*/
	Err_Thread_Not_Running = -23,
	/** ������ ������ ������ �� ��������*/
	Err_Read_Resource = -24,
	/** �������������� ������������ �����*/
	Err_Cmp_Type = -25,
	/** �������� ����� 0*/
	Err_Value_Is_Null = -26,
	/** �������� �� ����� 0*/
	Err_Value_IsNot_Null = -27,
	/** �������� < 0*/
	Err_Value_Is_Negative = -28,
	/** �������� > 0*/
	Err_Value_Is_Positive = -29,
	/** ������������ ������*/
	Err_Size_Invalid = -30,
	/** ������ ��� ��������� ������*/
	Err_Allocate_Memory = -31,
	/** ������������� ������*/
	Err_Size_Less__Zero = -32,
	/** ��������� �����*/
	Err_UnSuccess_Find = -33,
	/** ������������ ����������� ��������*/
	Err_Cmp_Value = -34,

	/** ���� �� ����������*/
	Err_File_Not_Exists = -100,
	/** ���������� �� ����������*/
	Err_Directory_Not_Exists = -101,
	/** ������ �������� �����*/
	Err_File_Create = -102,
	/** ������ �������� ����������*/
	Err_Directory_Create = -103,
	/** �� ������������ ���*/
	Err_Drive_Not_Exists = -104,
	/** ������ ������ ��������� �����/����������*/
	Err_File_Attributes = -105,
	/** ������ �������� �����*/
	Err_File_Open = -106,
	/** ������ �������� �����*/
	Err_File_Close = -107,
	/** ������ ������ �����*/
	Err_File_Write = -108,
	/** ������ ������ �����*/
	Err_File_Read = -109,
	/** ��� ����*/
	Err_Path_Not_Exists = -110,
	/** ���� ����� ������� ������� ������*/
	Err_File_Is_Large = -111,
	/** ���� ����� �� ������ ������*/
	Err_File_Incorrect_Size = -112,
	/** ����� �����*/
	Err_File_End = -113,
	/** ������������ ������� � �����*/
	Err_File_Simbol_In_Name = -114,
	/** ������ �������� �����*/
	Err_File_Delete = -115,
	/** ������ �������� �����*/
	Err_Dir_Delete = -116,
	/** ����� �� �����*/
	Err_Dir_mot_Empty = - 117,
	/** ������������ ��� ������� � �����*/
	Err_File_Open_Mode = -118,
	/** ������ �������������� ������*/
	Err_File_Write_Flush = -119,
	


	/** ������� ��������� ��� ������ �����*/
	Err_WriteData_NULL_Pointer = -200,
	/** ������� ������ ������ ��� ������ �����*/
	Err_WriteData_NULL_Size = -201,
	/** ���������� ������� ���� ��� ����������*/
	Err_WriteData_Cannot_Create_Path = -202,
	/** ������ �������� anp �����*/
	Err_WriteData_Anp_Error = -203,
	/** ������ �������� ana �����*/
	Err_WriteData_Ana_Error = -204,
	/** ������ �������� sgy �����*/
	Err_WriteData_Sgy_Error = -205,
	/** ������ �������� csv �����*/
	Err_WriteData_Csv_Error = -206,
	/** ������ ������ �������*/
	Err_WriteData_GetFileSize = -207,
	/** ������ ������ �����*/
	Err_WriteData_ReadFile = -208,
	/** ������������ ������ �����*/
	Err_WriteData_Sgy_BadSize = -209,

	/** ������ ��� ��������� ������� �����*/
	Err_Time_Get_Time = -300,
	/** ������ ��� ��������� ������� ����*/
	Err_Time_Get_TimeZone = -301,
	/** ������������ �������� �������*/
	Err_Time_Value = -302,
	/** ������������ �������� ��������� ����*/
	Err_Time_Zone = -303,
	/** ������������ ������ �������*/
	Err_Time_Format = -304,
	/** ������������ ������ ����*/
	Err_Date_Format = -305,
	/** ������������ ������ ���� ��� �������*/
	Err_Date_Time_Format = -306,
	/** �������������� ������ � ������� ����/�������*/
	Err_Incorrect_Data_Format = -307,
	/** ����� ������ 01.01.1970 ��� ������ 01.01.3000*/
	Err_ZetTime_Value = -308,

	/** ������������ ��� ���������*/
	Err_Structure_Type = -310,
	/** ������������ ������ ���������*/
	Err_Structure_Size = -311,
	/** ������������ ����������� �����*/
	Err_Structure_CRC = -312,

	/** ������ ��� ����������� � ��*/
	Err_DB_Connect = -400,
	/** ������ ��� ������ � ��*/
	Err_DB_Write = -401,
	/** ������ ��� ������ �� ��*/
	Err_DB_Read = -402,
	/** ������ ��� �������� ���������� ��*/
	Err_DB_Begin_Transaction = -403,
	/** ������ ��� �������� ���������� ��*/
	Err_DB_End_Transaction = -404,
	/** ����� ����� �������� ����������� ��*/
	Err_DB_TimeOut = -405,
	/** ������ ��� �������� ����������� ��*/
	Err_DB_Mutex = -406,

	/** ������� �� ������������ ������ ����*/
	Err_Dictionary_Not_Lang = -500,
	/** � ������� ��� ����������� �����*/
	Err_Dictionary_Not_Phrase = -501,
	/** �������� ����� �� �������� ����*/
	Err_Dictionary_No_Letters = -502,
};
//-----------------------------------------------------------------------------