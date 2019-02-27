#include "StdAfx.h"
#include "ZetSQL.h"

ZetSQL::ZetSQL()
{
	m_dataSize = 0;
}

ZetSQL::~ZetSQL()
{
}


BOOL ZetSQL::CreateODBC( HWND hWnd, WORD fRequest, LPCSTR lpszDriver, LPCSTR lpszAttributes )
{
	HINSTANCE dllHandle = NULL;
	FARPROC farProc = NULL;

	dllHandle = LoadLibrary( L"odbccp32.dll" );

	if (NULL != dllHandle)
	{
		farProc = GetProcAddress( dllHandle, "SQLConfigDataSource" );

		if (farProc != NULL)
		{
			ODBC MyFunction;
			MyFunction = ODBC( farProc );

			MyFunction( hWnd, fRequest, lpszDriver, lpszAttributes );
		}

		FreeLibrary( dllHandle );
	}

	return farProc != NULL;
}

BOOL ZetSQL::Connect( CString Host, const CString &Odbc, const CString &Database, const CString &User, const CString &Password, int ServerType )
{
	CStringA driverString;
	
	switch ( ServerType )
	{
	case 0:
		driverString = "MySQL ODBC 5.1 Driver";		
		break;
	case 1:
		driverString = "Firebird/InterBase(r) driver";
		break;
	case 2:
		driverString = "MariaDB ODBC 1.0 Driver";
		break;
	}

	OutputDebugStringW((CString)driverString);
	try
	{
		if (Host.IsEmpty()) Host = L"127.0.0.1"; // localhost
		if (Odbc.IsEmpty() || Database.IsEmpty() || User.IsEmpty()) return FALSE;

		USES_CONVERSION;
		CStringA creatingString;
		creatingString.Format( "DSN=%s;SERVER=%s;DATABASE=%s;UID=%s;PWD=%s;",
			W2A( LPCWSTR( Odbc ) ),
			W2A( LPCWSTR( Host ) ),
			W2A( LPCWSTR( Database ) ),
			W2A( LPCWSTR( User ) ),
			W2A( LPCWSTR( Password ) ) );

		if (!CreateODBC( NULL, ODBC_ADD_SYS_DSN, driverString, creatingString ))
			return FALSE;

		CString connectionString;
		connectionString.Format(L"DSN=%s;SERVER=%s;UID=%s;PWD=%s;", Odbc.GetString(), Host.GetString(), User.GetString(), Password.GetString());

		recordSet.m_pDatabase = &mysql;

		mysql.SetLoginTimeout( 2 );
		mysql.SetQueryTimeout( 5 );
		return mysql.OpenEx( connectionString, CDatabase::noOdbcDialog );
	}
	catch (CException* e)
	{
		e->Delete();
	}

	return FALSE;
}

void ZetSQL::Close()
{
	try
	{
		if (recordSet.IsOpen())
			recordSet.Close();

		if (IsConnected())
			mysql.Close();
	}
	catch (CException* e)
	{
		e->Delete();
	}
}

BOOL ZetSQL::IsConnected()
{
	try
	{
		return mysql.IsOpen();
	}
	catch (CException* e)
	{
		e->Delete();
	}

	return FALSE;
}

void ZetSQL::Query( const CString &_Query )
{
	if (!IsConnected()) return;

	try
	{
		mysql.ExecuteSQL( _Query );
	}
	catch (CDBException* e)
	{
		Close();
		e->Delete();
	}
	catch (CMemoryException* e)
	{
		e->Delete();
	}
	catch (CException* e)
	{
		e->Delete();
	}
}

BOOL ZetSQL::QueryEx( const CString &_Query )
{
	if (!IsConnected()) return FALSE;
	if (recordSet.IsOpen()) return FALSE;

	try
	{
		return recordSet.Open( CRecordset::snapshot, _Query, CRecordset::readOnly );
	}
	catch (CDBException* e)
	{
		e->Delete();
		Close();
	}
	catch (CMemoryException* e)
	{
		e->Delete();
	}
	catch (CException* e)
	{
		e->Delete();
	}

	return FALSE;
}

void ZetSQL::EndQueryEx()
{
	recordSet.Close();
}

DWORD ZetSQL::GetLastInserted( const CString &Table )
{
	DWORD ID = 0;
	CString query = L"SELECT `#` FROM `" + Table + L"` ORDER BY `#` DESC LIMIT 1";

	if (QueryEx( query ))
	{
		if (recordSet.GetRecordCount() && recordSet.GetODBCFieldCount())
		{
			CDBVariant cbdVarValue;
			recordSet.GetFieldValue( /*oO*/(short)0, cbdVarValue );
			ID = cbdVarValue.m_lVal;
		}

		EndQueryEx();
	}

	return ID;
}

DWORD ZetSQL::GetDBSize( const CString &Table_ )
{
	DWORD size = 0;
	CString query = L"SELECT COUNT(*) FROM " + Table_ + L";";

	if (QueryEx( query ))
	{
		if (recordSet.GetRecordCount() && recordSet.GetODBCFieldCount())
		{
			CDBVariant cbdVarValue;
			recordSet.GetFieldValue( /*oO*/(short)0, cbdVarValue );

			switch (cbdVarValue.m_dwType)
			{
			case DBVT_STRING:
			case DBVT_ASTRING:
			case DBVT_WSTRING:
				swscanf_s( cbdVarValue.m_pstring->GetBuffer(), _T("%d"), &size );
				break;
			case DBVT_LONG:
				size = cbdVarValue.m_lVal;
				break;
			default:
				break;
			}

		}

		EndQueryEx();
	}

	return size;
}

void ZetSQL::GetData( std::vector<CString> *sColumns, std::vector<std::vector<CString>> *ssResult )
{
	sColumns->clear();
	ssResult->clear();

	m_dataSize = 0;

	LONG rows = recordSet.GetRecordCount();
	SHORT fields = recordSet.GetODBCFieldCount();

	CDBVariant cbdVarValue;
	CODBCFieldInfo fieldInfo;
	INT j = -1;
	short i = 0;

	for (i = 0; i < fields; i++)
	{	// Пройдемся по всем столбцам

		recordSet.GetODBCFieldInfo( i, fieldInfo );
		sColumns->push_back( fieldInfo.m_strName );
	}

	try 
	{
		while (!recordSet.IsEOF())
		{	// Пройдемся по всем строкам

		std::vector<CString>sResult;
			CString strTemp;

			for (i = 0; i < fields; i++)
			{	// Пройдемся по всем столбцам строки

				recordSet.GetFieldValue( i, cbdVarValue );

				switch (cbdVarValue.m_dwType)
				{
				case 2: // tinyint
					strTemp.Format( L"%i", cbdVarValue.m_chVal );
					break;

				case 3: // smallint, year
					strTemp.Format( L"%i", cbdVarValue.m_iVal );
					break;

				case 4: // int, mediumint
					strTemp.Format( L"%i", cbdVarValue.m_lVal );
					break;

				case 5: // float
					strTemp.Format( L"%f", cbdVarValue.m_fltVal );
					break;

				case 6: // double
					strTemp.Format( L"%f", cbdVarValue.m_dblVal );
					break;

				case 7: // date, datetime, time (incorrect), timestamp
					strTemp.Format( L"%04i-%02i-%02i %02i:%02i:%02i.%03u",
						cbdVarValue.m_pdate->year, cbdVarValue.m_pdate->month,
						cbdVarValue.m_pdate->day, cbdVarValue.m_pdate->hour,
						cbdVarValue.m_pdate->minute, cbdVarValue.m_pdate->second,
						cbdVarValue.m_pdate->fraction / 1000000 );
					break;

				case 10: // bigint, decimal, char, varchar, tinytext, text, mediumtext, longtext 
					strTemp = CString(*cbdVarValue.m_pstringA);
					break;

				case 11: // bigint, decimal, char, varchar, tinytext, text, mediumtext, longtext 
					strTemp.Format(L"%s", (*cbdVarValue.m_pstringW).GetString());
					break;

				default:
					strTemp.Empty();
					break;
				}

				sResult.push_back( strTemp );
				cbdVarValue.Clear();
			}

			ssResult->push_back( sResult );
			m_dataSize++;
			recordSet.MoveNext();
		}
	}
	catch (CDBException *e)
	{
		e->Delete();
	}
	long b = 0;
}

CString ZetSQL::ULongLongTime( ULONGLONG *timestamp )
{
	WORD mSeconds = (WORD)( *timestamp        & 0x3FF );
	WORD Second   = (WORD)((*timestamp >> 10) & 0x3F  );
	WORD Minute   = (WORD)((*timestamp >> 16) & 0x3F  );
	WORD Hour     = (WORD)((*timestamp >> 22) & 0x1F  );
	WORD Day      = (WORD)((*timestamp >> 27) & 0x1F  );
	WORD Month    = (WORD)((*timestamp >> 32) & 0xF   );
	WORD Year     = (WORD)((*timestamp >> 36) & 0x3FFF);

	CString result;
	result.Format( L"%02i.%02i.%04i %02i:%02i:%02i:%03i",
		Day, Month, Year, Hour, Minute, Second, mSeconds );

	return result;
}
