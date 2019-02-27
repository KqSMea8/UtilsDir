//-----------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
#include <windows.h>

#define SOURSE_NAME_MAX_SIZE		400	// максимальный размер имени программы, приславшей ошибку
#define	MAX_ERROR_SOURCE_COUNT		7	// количество источников ошибок
#define	MAX_ERROR_SOLUTIONS_COUNT	3	// количество источников ошибок
//-----------------------------------------------------------------------------
enum ErrorSourses		// перечисление всех источников ошибок	 
{
	USB_Error = 0,		// 0 - ошибки в устройствах подключеннх по USB 
	Ethernet_Error,		// 1 - ошибки в устройствах подключеннх по Ethernet 
	ZetKey_Error,		// 2 - ошибки в ZetKey
	ZeroVID_Error,		// 3 - ошибки в устройствах не определнных системой (VID/PID = 0) 
	Modbus_Error,		// 4 - ошибки возникшие в Modbus
	ZetServer_Error,	// 5 - ошибки возникшие в ZetServer
	Other_Error			// 6 - ошибки из других источников 
};
//-----------------------------------------------------------------------------
struct ErrorInfoStruct
{
	size_t				Struct_Size;			// размер структуры
	/*	флаг что эта ошибка возникла
	true	- ошибка появилась
	false	- этой ошибки больше нет*/
	BOOL				Error_Arose;

	wchar_t				Error_Sourse_Name	[SOURSE_NAME_MAX_SIZE];	// имя программы приславшей ошибку (если важно)

	wchar_t				Solution_Helpstring	[MAX_ERROR_SOLUTIONS_COUNT][MAX_PATH];	// краткое описание способа решения проблемы
	wchar_t				Solution_ExePathName[MAX_ERROR_SOLUTIONS_COUNT][MAX_PATH];	// полный путь к местоположению программы 
	wchar_t				Solution_CommandLine[MAX_ERROR_SOLUTIONS_COUNT][MAX_PATH];	// командная строка для запуска программы

	ErrorSourses		Error_Sourse_ID;				// идентификатор источника ошибки
	long				Error_Number;					// номер возникшей ошибки
	/*	флаг что устройство является датчиком 7xxx
	TRUE	- ошибка возникла в устройстве 7ххх
	FALSE	- ошибка возникла в обычном устройстве
	-1		- невозможно определить устроство в котором возникла ошибка (не будет отображаться инфомация по нему)*/
	BOOL				Is7xxxDevice;							
	long				Device_Type;					// (заполняется если флаг Is7xxxDevice не равен -1) тип устройства в котором возникла ошибка
	
	unsigned long long	Device_Serial;					// (заполняется если флаг Is7xxxDevice не равен -1) серийный номер устройства в котором возникла ошибка 
	wchar_t				Device_Name	[MAX_PATH];			// название устройства

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
/**Коды ошибок. Не путать с ErrorCode из NuGet*/
enum ErrorCodes : long
{
	NoError = 0,	/** Ошибок нет */
	/** Фатальная ошибка. Продолжение работы программы не возможно. */
	FatalError = -1,
	/** Невалидный параметр */
	InvalidParams = -2,
	/** ошибка инициализации */
	Err_Initialization = -3,
	/** неправильный размер структуры */
	Err_Size_Structure = -4,
	/** неправильный ID структуры */
	Err_ID_Structure = -5,
	/** ошибка при копировании структуры */
	Err_Copy_Structure = -6,
	/** ошибка при копировании данных */
	Err_Copy_Data = -7,
	/** ошибка при создании массива */
	Err_Create_Array = -8,
	/** ошибка при расчётах */
	Err_Calc = -9,
	/** нулевой указатель */
	Err_NULL_Pointer = -10,
	/** ошибка при создании структуры*/
	Err_Create_Structure = -11,
	/** Задаваемое значение меньше допустимого*/
	Err_Value_Less = -12,
	/** Задаваемое значение больше допустимого*/
	Err_Value_More = -13,
	/** Пустая строка*/
	Err_String_Empty = -14,
	/** Строка имеет слишком маленький размер*/
	Err_String_Less = -15,
	/** Строка имеет слишком большой размер*/
	Err_String_More = -16,
	/** Нет доступа*/
	Err_Access_Denied = -17,
	/** нулевой размер*/
	Err_Size_Zero = -18,
	/** Неверный IP-адрес, т.е. 255.255.255.255 */
	Err_Invalid_IP = -19,
	/** Нет пинга*/
	Err_Not_Ping = -20,
	/** Функция временно не определена*/
	Err_Function_Not_Defined = -21,
	/** Ошибка при создании рабочего потока*/
	Err_Create_Thread = -22,
	/** Рабочий поток не запущен*/
	Err_Thread_Not_Running = -23,
	/** Ошибка чтения данных из ресурсов*/
	Err_Read_Resource = -24,
	/** Несоответствие сравниваемых типов*/
	Err_Cmp_Type = -25,
	/** Значение равно 0*/
	Err_Value_Is_Null = -26,
	/** Значение не равно 0*/
	Err_Value_IsNot_Null = -27,
	/** Значение < 0*/
	Err_Value_Is_Negative = -28,
	/** Значение > 0*/
	Err_Value_Is_Positive = -29,
	/** Неправильный размер*/
	Err_Size_Invalid = -30,
	/** Ошибка при выделении памяти*/
	Err_Allocate_Memory = -31,
	/** Отрицательный размер*/
	Err_Size_Less__Zero = -32,
	/** Неудачный поиск*/
	Err_UnSuccess_Find = -33,
	/** Неправильное соотнощение значений*/
	Err_Cmp_Value = -34,

	/** Файл не существует*/
	Err_File_Not_Exists = -100,
	/** Директория не существует*/
	Err_Directory_Not_Exists = -101,
	/** Ошибка создания файла*/
	Err_File_Create = -102,
	/** Ошибка создания директории*/
	Err_Directory_Create = -103,
	/** Не существующий том*/
	Err_Drive_Not_Exists = -104,
	/** Ошибка чтения атрибутов файла/директории*/
	Err_File_Attributes = -105,
	/** Ошибка открытия файла*/
	Err_File_Open = -106,
	/** Ошибка закрытия файла*/
	Err_File_Close = -107,
	/** Ошибка записи файла*/
	Err_File_Write = -108,
	/** Ошибка чтения файла*/
	Err_File_Read = -109,
	/** Нет пути*/
	Err_Path_Not_Exists = -110,
	/** Файл имеет слишком большой размер*/
	Err_File_Is_Large = -111,
	/** Файл имеет не верный размер*/
	Err_File_Incorrect_Size = -112,
	/** Конец файла*/
	Err_File_End = -113,
	/** Недопустимые символы в имени*/
	Err_File_Simbol_In_Name = -114,
	/** Ошибка удаления файла*/
	Err_File_Delete = -115,
	/** Ошибка удаления папки*/
	Err_Dir_Delete = -116,
	/** Папка не пуста*/
	Err_Dir_mot_Empty = - 117,
	/** Недопустимый тип доступа к файлу*/
	Err_File_Open_Mode = -118,
	/** Ошибка принудительной записи*/
	Err_File_Write_Flush = -119,
	


	/** Нулевой указатель при записи файла*/
	Err_WriteData_NULL_Pointer = -200,
	/** Нулевой размер данных при записи файла*/
	Err_WriteData_NULL_Size = -201,
	/** Невозможно создать путь для сохранения*/
	Err_WriteData_Cannot_Create_Path = -202,
	/** Ошибка создания anp файла*/
	Err_WriteData_Anp_Error = -203,
	/** Ошибка создания ana файла*/
	Err_WriteData_Ana_Error = -204,
	/** Ошибка создания sgy файла*/
	Err_WriteData_Sgy_Error = -205,
	/** Ошибка создания csv файла*/
	Err_WriteData_Csv_Error = -206,
	/** Ошибка чтения размера*/
	Err_WriteData_GetFileSize = -207,
	/** Ошибка чтения файла*/
	Err_WriteData_ReadFile = -208,
	/** Неправильный размер файла*/
	Err_WriteData_Sgy_BadSize = -209,

	/** Ошибка при получении времени компа*/
	Err_Time_Get_Time = -300,
	/** Ошибка при получении свойств зоны*/
	Err_Time_Get_TimeZone = -301,
	/** Неправильное значение времени*/
	Err_Time_Value = -302,
	/** Неправильное значение временной зоны*/
	Err_Time_Zone = -303,
	/** Неправильный формат времени*/
	Err_Time_Format = -304,
	/** Неправильный формат даты*/
	Err_Date_Format = -305,
	/** Неправильный формат даты или времени*/
	Err_Date_Time_Format = -306,
	/** Несоответствие данных и формата даты/времени*/
	Err_Incorrect_Data_Format = -307,
	/** Время меньше 01.01.1970 или больше 01.01.3000*/
	Err_ZetTime_Value = -308,

	/** Неправильный тип структуры*/
	Err_Structure_Type = -310,
	/** Неправильный размер структуры*/
	Err_Structure_Size = -311,
	/** Неправильная контрольная сумма*/
	Err_Structure_CRC = -312,

	/** Ошибка при подключении к БД*/
	Err_DB_Connect = -400,
	/** Ошибка при записи в БД*/
	Err_DB_Write = -401,
	/** Ошибка при чтении из БД*/
	Err_DB_Read = -402,
	/** Ошибка при открытии транзакции БД*/
	Err_DB_Begin_Transaction = -403,
	/** Ошибка при закрытии транзакции БД*/
	Err_DB_End_Transaction = -404,
	/** Вышло время ожидания доступности БД*/
	Err_DB_TimeOut = -405,
	/** Ошибка при проверке доступности БД*/
	Err_DB_Mutex = -406,

	/** Словарь не поддерживает данный язык*/
	Err_Dictionary_Not_Lang = -500,
	/** В словаре нет запрошенной фразы*/
	Err_Dictionary_Not_Phrase = -501,
	/** Исходная фраза не содержит букв*/
	Err_Dictionary_No_Letters = -502,
};
//-----------------------------------------------------------------------------