//------------------------------------------------------------------------------
#pragma once
#include <string>
#include <sqlite3\include\sqlite3.h>
//------------------------------------------------------------------------------
namespace zet
{
namespace sql
{
//------------------------------------------------------------------------------
typedef std::string PasswordType;
class SqlEngine
{
	private:
		const PasswordType m_password;
	public:
		SqlEngine(const PasswordType password = "");
		virtual ~SqlEngine();
};
//------------------------------------------------------------------------------
}
}
//------------------------------------------------------------------------------