//------------------------------------------------------------------------------
#pragma once
#include "ZetSqlEngine.h"
//------------------------------------------------------------------------------
namespace zet
{
namespace sql
{
//------------------------------------------------------------------------------
class SqliteEngine : public SqlEngine
{
private:
	std::string m_SqlCreateScript;
	std::string m_dbFullFileName;

	void Connect();
	void DisConnect();

protected:
	sqlite3 * m_ptrDb = nullptr;

public:
	SqliteEngine(
		const std::string sqlCreate,
		const std::string dbFilePath,
		const std::string dbFileExt,
		const PasswordType password);
	~SqliteEngine();

	void ReConnect();
};
//------------------------------------------------------------------------------
}
}
//------------------------------------------------------------------------------