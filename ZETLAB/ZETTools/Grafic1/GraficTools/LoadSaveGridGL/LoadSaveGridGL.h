#pragma once
#include <Struct_grid.h>
#include <pugixml/include/pugiautolink.hpp>
#include <list>
// возвращаемые ошибки при чтении xml
namespace LoadSaveGridSetting
{
enum Errors
{
	ERROR_LOAD_FILE = -1,
	ERROR_NO_CONFIG = -2,
};
};

class CLoadSaveGridGL
{
public:
	CLoadSaveGridGL();
	~CLoadSaveGridGL();

public:
	/// <summary>Сохранение структуры GridGL в файл в виде xml.</summary>
	/// <returns> возвращает true при успешном сохранении</returns>
	/// <param name="File">Путь к файлу</param>
	/// <param name="GridGLParams">Параметры сетки</param>
	/// <param name="Comments">Массив комментариев, дописываемых в файл</param>
	/// <param name="CommentsCount">Количество дописываемых комментариев</param>
	bool SaveGridParams(_In_ CString File, _In_  grid *GridGLParams,
						_In_opt_ CString *Comments = nullptr, 
						_In_opt_ size_t   CommentsCount = 0);
	/// <summary>Загрузка структуры GridGL из xml файла.</summary>
	/// <returns> возвращает S_OK при успешном считывании файла, в противном случае код ошибки из LoadSaveGridSetting::Errors.</returns>
	/// <param name="File">Путь к файлу</param>
	/// <param name="GridGLParams">Параметры сетки</param>
	/// <param name="Comments">Список комментариев к файлу</param>
	long LoadGridParams(_In_ CString File, _Out_ grid *GridGLParams, 
						_Out_opt_ std::list<CString> &Comments = std::list<CString>());
};

