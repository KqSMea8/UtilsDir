//------------------------------------------------------------------------------
#include "stdafx.h"
#include "ZetSqlEngine.h"
//------------------------------------------------------------------------------
namespace zet
{
namespace sql
{
//------------------------------------------------------------------------------
SqlEngine::SqlEngine(const PasswordType password)
	: m_password(password)
{
}
//------------------------------------------------------------------------------
SqlEngine::~SqlEngine()
{}
//------------------------------------------------------------------------------
}
}
//------------------------------------------------------------------------------