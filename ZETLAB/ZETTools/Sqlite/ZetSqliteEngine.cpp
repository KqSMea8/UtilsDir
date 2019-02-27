//------------------------------------------------------------------------------
#include "stdafx.h"
#include <cassert>
#include <algorithm>
#include "ZetSqliteEngine.h"
//------------------------------------------------------------------------------
namespace zet
{
namespace sql
{
//------------------------------------------------------------------------------
void SqliteEngine::Connect()
{
	std::string sOutputDebug;
	try
	{
		int err = sqlite3_open(m_dbFullFileName.c_str(), &m_ptrDb);
		if (SQLITE_OK == err)
		{
			_ASSERTE(m_ptrDb != nullptr);

			char * pErrorMsg(nullptr);
			err = sqlite3_exec(m_ptrDb, m_SqlCreateScript.c_str(), 0, 0, &pErrorMsg);
			if (SQLITE_OK == err)
			{
				OutputDebugStringA("SqliteEngine Connect success\n");
			}
			else
			{
				m_ptrDb = nullptr;
				sOutputDebug = "SqliteEngine Connect failed. sqlite3_exec return code = ";
				sOutputDebug.append(std::to_string(err));
				sOutputDebug.append(". ");
				sOutputDebug.append(pErrorMsg);
				sOutputDebug.append("\n");
				OutputDebugStringA(sOutputDebug.c_str());
			}
			if (pErrorMsg != nullptr)
				sqlite3_free(pErrorMsg);	// удаление массива pErrorMsg
		}
		else
		{
			m_ptrDb = nullptr;
			sOutputDebug = "SqliteEngine Connect failed. sqlite3_open return code = ";
			sOutputDebug.append(std::to_string(err));
			sOutputDebug.append("\n");
			OutputDebugStringA(sOutputDebug.c_str());
		}
	}
	catch (std::string & strEx)
	{
		m_ptrDb = nullptr;
		sOutputDebug = "SqliteEngine Connect failed. SQLite operation failed with a reason ";
		sOutputDebug.append(std::string(strEx.begin(), strEx.end()));
		sOutputDebug.append("\n");
		OutputDebugStringA(sOutputDebug.c_str());
	}
}
//------------------------------------------------------------------------------
void SqliteEngine::DisConnect()
{
	if (m_ptrDb != nullptr)
	{
		sqlite3_close(m_ptrDb);
		m_ptrDb = nullptr;
	}
}
//------------------------------------------------------------------------------
SqliteEngine::SqliteEngine(
	const std::string sqlCreate,
	const std::string dbFilePath,
	const std::string dbFileExt,
	const PasswordType password)
	: SqlEngine(password)
{
	assert(!dbFileExt.empty());

	m_SqlCreateScript = sqlCreate;

	std::string sOutputDebug;
	// определение имени файла --------------------------------------
	if (dbFilePath.empty())
	{// путь файла пуст
		char name[MAX_PATH] = { 0 };
		const DWORD retDWORD = GetModuleFileNameA(NULL, name, MAX_PATH);
		if (retDWORD > 0)
		{
			const std::string strFullName(name);
			const std::string slash("\\");
			std::string::const_iterator iPos =
				std::find_end(strFullName.cbegin(), strFullName.cend(), slash.cbegin(), slash.cend());
			if (iPos != strFullName.end())
			{
				const size_t lastSlashPos = iPos - strFullName.begin();
				if (lastSlashPos > 0)
				{
					const std::string strAppFolder = strFullName.substr(0, lastSlashPos);
					std::string strLogName = strFullName.substr(lastSlashPos + slash.length(), strAppFolder.length());

					strLogName.replace(strLogName.find(".exe", 0), 4, dbFileExt);
					m_dbFullFileName = strAppFolder + slash + strLogName;
				}
			}
			else
			{
				OutputDebugStringA("SqliteEngine constructor failed. The full name of the process no have slash\n");
			}
		}
		else
		{
			sOutputDebug = "SqliteEngine constructor failed. GetModuleFileName return ";
			sOutputDebug.append(std::to_string(retDWORD));
			sOutputDebug.append("\n");
			OutputDebugStringA(sOutputDebug.c_str());
		}
	}
	else// if (dbFilePath.empty())
	{
		m_dbFullFileName = dbFilePath + dbFileExt;
	}

	// начинаем -----------------------------------------------------
	Connect();
}
//------------------------------------------------------------------------------
SqliteEngine::~SqliteEngine()
{
	DisConnect();
}
//------------------------------------------------------------------------------
void SqliteEngine::ReConnect()
{
	if (m_ptrDb == nullptr)
		DisConnect();
	Connect();
}
//------------------------------------------------------------------------------
}
}
//------------------------------------------------------------------------------