#ifndef mysqlcore_h_
#define mysqlcore_h_

#include <ZETSimple\include\logger\logger_auto_link.h>
#include <ZETSimple\include\logger\Logger.h>

#include "afxdb.h"
#include "odbcinst.h"

#include <map>
#include <vector>

typedef std::map<std::wstring, std::vector<std::wstring>> DataBaseInfo;

class CMysqlCore
{
public:
	CMysqlCore(_In_ const std::wstring& sHost, const std::wstring& sOdbc, const std::wstring& sDatabase,
		const std::wstring& sUser, const std::wstring& sPassword);
	~CMysqlCore();

	bool execute_query(const std::wstring& sQuery);
	bool execute_query_select(const std::wstring& sQuery, DataBaseInfo& info);

protected:
	bool connect();
	void disconnect();

	bool isConnected();

	std::wstring m_sHost;
	std::wstring m_sOdbc;
	std::wstring m_sDatabase;
	std::wstring m_sUser;
	std::wstring m_sPassword;

private:
	bool createOdbc(HWND hWnd, WORD request, LPCSTR lpszDriver, LPCSTR lpszAttributes);

	CDatabase* m_pDatabase;
	CRecordset* m_pRecordset;

	CLogger* m_pLogger;
};

#endif // mysqlcore_h_

