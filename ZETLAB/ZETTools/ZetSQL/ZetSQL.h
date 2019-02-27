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
	/// ������� ODBC �������
	/// </summary>
	/// <param name="hWnd">NULL</param>
	/// <param name="fRequest">ODBC_ADD_SYS_DSN</param>
	/// <param name="lpszDriver">��� ��������</param>
	/// <param name="lpszAttributes">�������������� ���������</param>
	/// <returns>TRUE - �����, FALSE - �������</returns>
	BOOL CreateODBC( HWND hWnd, WORD fRequest, LPCSTR lpszDriver, LPCSTR lpszAttributes );

	/// <summary>
	/// ������������ � MySQL �������
	/// </summary>
	/// <param name="Host">��� �����</param>
	/// <param name="Odbc">DSN �������</param>
	/// <param name="Database">�������� ��</param>
	/// <param name="User">��� ������������</param>
	/// <param name="Password">������</param>
	/// <param name="ServerType">������</param>
	/// <returns>TRUE - �����, FALSE - �������</returns>
	BOOL Connect( CString Host, const CString &Odbc, const CString &Database, const CString &User, const CString &Password, int ServerType);

	/// <summary>
	/// ��������� ���������� � ��������
	/// </summary>
	void Close();

	/// <summary>
	/// �������� ����������
	/// </summary>
	/// <returns>TRUE - ���������� �����������, FALSE - �������</returns>
	BOOL IsConnected();

private:

	/// <summary>
	/// ������ ��� ������ � ����� ������
	/// </summary>
	CDatabase mysql;

	/// <summary>
	/// ������ ��� ���������� �������� � ���� ������
	/// </summary>
	CRecordset recordSet;

public:

	/// <summary>
	/// ���������� SQL-������, �� ���������� data records
	/// ��� �������� ���� INSERT � UPDATE
	/// </summary>
	/// <param name="_Query">������ �������</param>
	void Query( const CString &_Query );

	/// <summary>
	/// ���������� SQL-������, ���������� data records
	/// ��� �������� ���� SELECT
	/// </summary>
	/// <param name="_Query">������ �������</param>
	/// <returns>TRUE - �����, FALSE - �������</returns>
	BOOL QueryEx( const CString &_Query );

	/// <summary>
	/// ��������� ������ QueryEx � ����������� ��� �������
	/// </summary>
	void EndQueryEx();

	/// <summary>
	/// ���������� ���������� �����
	/// </summary>
	/// <param name="Table">��� �������</param>
	/// <returns>����� ������</returns>
	DWORD GetDBSize( const CString &Table );

	/// <summary>
	/// ���������� ����� ��������� ������
	/// </summary>
	/// <param name="Table">��� �������</param>
	/// <returns>����� ������</returns>
	DWORD GetLastInserted( const CString &Table );

	/// <summary>
	/// ��������� ������ result ���������� �� �������
	/// </summary>
	/// <param name="result">�������� ������</param>
	void GetData( std::vector<CString> *sColumns, std::vector<std::vector<CString>> *ssResult);

	/// <summary>
	/// ����������� ����� �������
	/// </summary>
	/// <param name="timestamp">����� �������</param>
	/// <returns>����������������� ������</returns>
	CString ULongLongTime( ULONGLONG *timestamp );

	long m_dataSize;
};

typedef bool (__stdcall *ODBC)(HWND hwndParent, WORD fRequest, LPCSTR lpszDriver, LPCSTR lpszAttributes);
