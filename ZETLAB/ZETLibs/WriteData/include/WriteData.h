#include <stdint.h>
#include <rpc.h>

#include <ZetErrorCodes.h>
#include <ZetTools\include\ZetTools.h>

#ifdef WRITEDATA_EXPORTS
#define WRITEDATA_API __declspec(dllexport)
#else
#define WRITEDATA_API __declspec(dllimport)
#endif

//----------------------------------------------------------------------------------------------------------------------
struct wdDataStructA
{
	char sName[128];			// название канала
	char sComment[128];			// комментарий по каналу (пока не используется)
	char sGroupName[128];		// групповое имя (принадлежность устройству)
	char sConversion[32];		// единица измерения по каналу
	GUID id;					// идентификатор канала
	float frequency;			// частота дискретизации (в Гц)
	float minlevel;				// минимальное измеряемое значение (в ед. изм.)
	float maxlevel;				// максимальное измеряемое значение (в ед. изм.)
	float threshold;			// порог чувствительности (в ед. изм.)
	float sense;				// чувствительность (В/ед. изм.)
	float reference;			// опорное значение (в ед. изм.) 
	float shift;				// смещение постоянной составляющей (в ед. изм.)
	int typeAdc;				// тип платы (необязательно)
	int numberDSP;				// номер сигнального процессора (необязательно)
	int type;					// статус канала (АЦП, ЦАП, виртуальный и т.д.)
	int number;					// порядковый номер канала
	CZetTime tTime;				// время начала данных
	float* pData;				// указатель на данные
	uint32_t size;				// размер массива данных
};
//----------------------------------------------------------------------------------------------------------------------
struct wdDataStructW
{
	wchar_t sName[128];			// название канала
	wchar_t sComment[128];		// комментарий по каналу (пока не используется)
	wchar_t sGroupName[128];	// групповое имя (принадлежность устройству)
	wchar_t sConversion[32];	// единица измерения по каналу
	GUID id;					// идентификатор канала
	float frequency;			// частота дискретизации (в Гц)
	float minlevel;				// минимальное измеряемое значение (в ед. изм.)
	float maxlevel;				// максимальное измеряемое значение (в ед. изм.)
	float threshold;			// порог чувствительности (в ед. изм.)
	float sense;				// чувствительность (В/ед. изм.)
	float reference;			// опорное значение (в ед. изм.) 
	float shift;				// смещение постоянной составляющей (в ед. изм.)
	int typeAdc;				// тип платы (необязательно)
	int numberDSP;				// номер сигнального процессора (необязательно)
	int type;					// статус канала (АЦП, ЦАП, виртуальный и т.д.)
	int number;					// порядковый номер канала
	CZetTime tTime;				// время начала данных
	float* pData;				// указатель на данные
	uint32_t size;				// размер массива данных
};
//----------------------------------------------------------------------------------------------------------------------
#ifdef UNICODE
#define wdDataStruct  wdDataStructW
#else
#define wdDataStruct  wdDataStructA
#endif
//----------------------------------------------------------------------------------------------------------------------
_Check_return_
long WRITEDATA_API wdWriteAnaA(_In_ wdDataStructA* pData,	//указатель на массив структур данных для записи
									_In_ const uint32_t size,	//размерность массива структур
									_In_ LPCSTR pszPath);		//путь (директория) для сохранения

_Check_return_
long WRITEDATA_API wdWriteAnaW(_In_ wdDataStructW* pData,	//указатель на массив структур данных для записи
									_In_ const uint32_t size,	//размерность массива структур
									_In_ LPCWSTR pszPath);		//путь (директория) для сохранения
#ifdef UNICODE
#define wdWriteAna wdWriteAnaW
#else
#define wdWriteAna wdWriteAnaA
#endif
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
_Check_return_
long WRITEDATA_API wdWriteSgyA(_In_ wdDataStructA* pData,	//указатель на массив структур данных для записи
									_In_ const uint32_t size,	//размерность массива структур
									_In_ LPCSTR pszPath);		//путь (имя файла) для сохранения

_Check_return_
long WRITEDATA_API wdWriteSgyW(_In_ wdDataStructW* pData,	//указатель на массив структур данных для записи
									_In_ const uint32_t size,	//размерность массива структур
									_In_ LPCWSTR pszPath);		//путь (имя файла) для сохранения

#ifdef UNICODE
#define wdWriteSgy wdWriteSgyW
#else
#define wdWriteSgy wdWriteSgyA
#endif
//----------------------------------------------------------------------------------------------------------------------
_Check_return_
long WRITEDATA_API wdChangeLengthSgyA(_In_ LPCSTR pszPath,	//путь(имя файла)
										_In_ const float length);	//новая длительность трасс

_Check_return_
long WRITEDATA_API wdChangeLengthSgyW(_In_ LPCWSTR pszPath,	//путь(имя файла)
										_In_ const float length);	//новая длительность трасс

#ifdef UNICODE
#define wdChangeLengthSgy wdChangeLengthSgyW
#else
#define wdChangeLengthSgy wdChangeLengthSgyA
#endif
//----------------------------------------------------------------------------------------------------------------------
_Check_return_
long WRITEDATA_API wdChangeFreqSgyA(_In_ LPCSTR pszPath,	//путь(имя файла)
									_In_ const float freq);	//новая частота дисктеризации

_Check_return_
long WRITEDATA_API wdChangeFreqSgyW(_In_ LPCWSTR pszPath,	//путь(имя файла)
									_In_ const float freq);	//новая частота дисктеризации

#ifdef UNICODE
#define wdChangeFreqSgy wdChangeFreqSgyW
#else
#define wdChangeFreqSgy wdChangeFreqSgyA
#endif
//----------------------------------------------------------------------------------------------------------------------
_Check_return_
long WRITEDATA_API wdMergeSgyA(_In_ LPCSTR* ppszPath,	//указатель на массив имен файлов для объединения
								_In_ const uint32_t size,	//размерность массива файлов
								_In_ LPCSTR pszPath);	//путь(имя файла) для сохранения

_Check_return_
long WRITEDATA_API wdMergeSgyW(_In_ LPCWSTR* ppszPath,	//указатель на массив имен файлов для объединения
								_In_ const uint32_t size,	//размерность массива файлов
								_In_ LPCWSTR pszPath);	//путь(имя файла) для сохранения

#ifdef UNICODE
#define wdMergeSgy wdMergeSgyW
#else
#define wdMergeSgy wdMergeSgyA
#endif
//----------------------------------------------------------------------------------------------------------------------
_Check_return_
long WRITEDATA_API wdAverageSgyA(_In_ LPCSTR* ppszPath,	//указатель на массив имен файлов для объединения
								 _In_ const uint32_t size,	//размерность массива файлов
								 _In_ LPCSTR pszPath);	//путь(имя файла) для сохранения

_Check_return_
long WRITEDATA_API wdAverageSgyW(_In_ LPCWSTR* ppszPath,	//указатель на массив имен файлов для объединения
								 _In_ const uint32_t size,	//размерность массива файлов
								 _In_ LPCWSTR pszPath);	//путь(имя файла) для сохранения

#ifdef UNICODE
#define wdAverageSgy wdAverageSgyW
#else
#define wdAverageSgy wdAverageSgyA
#endif
//----------------------------------------------------------------------------------------------------------------------
_Check_return_
long WRITEDATA_API wdWriteCsvA(_In_ wdDataStructA* pData,	//указатель на массив структур данных для записи
									_In_ const uint32_t size,	//размерность массива структур
									_In_ LPCSTR pszPath);		//путь (имя файла) для сохранения

_Check_return_
long WRITEDATA_API wdWriteCsvW(_In_ wdDataStructW* pData,	//указатель на массив структур данных для записи
									_In_ const uint32_t size,	//размерность массива структур
									_In_ LPCWSTR pszPath);		//путь (имя файла) для сохранения
#ifdef UNICODE
#define wdWriteCsv wdWriteCsvW
#else
#define wdWriteCsv wdWriteCsvA
#endif
//----------------------------------------------------------------------------------------------------------------------
_Check_return_
long WRITEDATA_API wdSgy2AnaA(_In_ LPCSTR pszPath);			//путь (имя файла) для сохранения

_Check_return_
long WRITEDATA_API wdSgy2AnaW(_In_ LPCWSTR pszPath);		//путь (имя файла) для сохранения

#ifdef UNICODE
#define wdSgy2Ana wdSgy2AnaW
#else
#define wdSgy2Ana wdSgy2AnaA
#endif
//----------------------------------------------------------------------------------------------------------------------