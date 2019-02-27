#pragma once
#include "afxdb.h"
#include <odbcinst.h>
#include <vector>
class ZetSQL
{
public:
	ZetSQL();
	~ZetSQL();

public:

	/// <summary>
	/// Создает ODBC профиль
	/// </summary>
	/// <param name="hWnd">NULL</param>
	/// <param name="fRequest">ODBC_ADD_SYS_DSN</param>
	/// <param name="lpszDriver">Имя драйвера</param>
	/// <param name="lpszAttributes">Дополнительные аттрибуты</param>
	/// <returns>TRUE - удача, FALSE - неудача</returns>
	BOOL CreateODBC( HWND hWnd, WORD fRequest, LPCSTR lpszDriver, LPCSTR lpszAttributes );

	/// <summary>
	/// Подключается к MySQL серверу
	/// </summary>
	/// <param name="Host">Имя хоста</param>
	/// <param name="Odbc">DSN сервера</param>
	/// <param name="Database">Название БД</param>
	/// <param name="User">Имя пользователя</param>
	/// <param name="Password">Пароль</param>
	/// <param name="ServerType">Сервер</param>
	/// <returns>TRUE - удача, FALSE - неудача</returns>
	BOOL Connect( CString Host, const CString &Odbc, const CString &Database, const CString &User, const CString &Password, int ServerType);

	/// <summary>
	/// Закрываем соединение с сервером
	/// </summary>
	void Close();

	/// <summary>
	/// Проверка соединения
	/// </summary>
	/// <returns>TRUE - соединение установлено, FALSE - неудача</returns>
	BOOL IsConnected();

private:

	/// <summary>
	/// Объект для работы с базой данных
	/// </summary>
	CDatabase mysql;

	/// <summary>
	/// Объект для выполнения запросов в базу данных
	/// </summary>
	CRecordset recordSet;

public:

	/// <summary>
	/// Производит SQL-запрос, не возвращает data records
	/// Для запросов типа INSERT и UPDATE
	/// </summary>
	/// <param name="_Query">Строка запроса</param>
	void Query( const CString &_Query );

	/// <summary>
	/// Производит SQL-запрос, ВОЗВРАЩАЕТ data records
	/// Для запросов типа SELECT
	/// </summary>
	/// <param name="_Query">Строка запроса</param>
	/// <returns>TRUE - удача, FALSE - неудача</returns>
	BOOL QueryEx( const CString &_Query );

	/// <summary>
	/// Завершает запрос QueryEx и освобождает все ресурсы
	/// </summary>
	void EndQueryEx();

	/// <summary>
	/// Возвращает количество строк
	/// </summary>
	/// <param name="Table">Имя таблицы</param>
	/// <returns>Номер строки</returns>
	DWORD GetDBSize( const CString &Table );

	/// <summary>
	/// Возвращает номер последней строки
	/// </summary>
	/// <param name="Table">Имя таблицы</param>
	/// <returns>Номер строки</returns>
	DWORD GetLastInserted( const CString &Table );

	/// <summary>
	/// Заполняет массив result значениями из запроса
	/// </summary>
	/// <param name="result">Выходной массив</param>
	void GetData( std::vector<CString> *sColumns, std::vector<std::vector<CString>> *ssResult);

	/// <summary>
	/// Форматирует метку времени
	/// </summary>
	/// <param name="timestamp">Метка времени</param>
	/// <returns>Отформатированная строка</returns>
	CString ULongLongTime( ULONGLONG *timestamp );

	long m_dataSize;
};

typedef bool (__stdcall *ODBC)(HWND hwndParent, WORD fRequest, LPCSTR lpszDriver, LPCSTR lpszAttributes);
