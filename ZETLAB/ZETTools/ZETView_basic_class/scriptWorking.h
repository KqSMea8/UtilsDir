#pragma once

// TODO : перевод строк в общих классах требует единых идентификаторов

#import "C:\WINDOWS\System32\msscript.ocx"
using namespace MSScriptControl;

struct CScriptWork
{
	CScriptWork::CScriptWork(CWnd* parent)
		: spScriptCtl(__uuidof(ScriptControl))
	{
		//сбрасываем флаг занятости скрипта
		m_bScriptExecuting = false;

		pParent = parent;
		//просто, пример по-умолчанию
		m_progText = L"Function Initialization(param)\n"
					 L"Dim Input\n"
					 L"Input = InputBox(\"\",param,\"Текст комментария\")\n" 
					 L"Initialization = Input\n"
					 L"end Function\n";

		//выставляем язык по-умолчанию
		bstrLanguage = L"VBScript";
	}
private:
	CWnd* pParent;
	//текст, загружаемый в скрипт
	CString m_progText;
	//язык исполнения кода
	_bstr_t bstrLanguage;
	//собственно, сам скрипт
	IScriptControlPtr spScriptCtl;
public:

	//загрузить текст скрипта
	void setProgText(CString text);
	//выставляем текущий язык
	void setLanguage_Basic();
	void setLanguage_Java();
	//делаем текущий язык активным
	void acceptCurrentLanguage();

	//выполняем скрипт
	VARIANT executeScript(CString functionName, CString paramStr);
	VARIANT executeScript(IDispatch* objDist, CString objName, CString functionName, CString paramStr, int& bError);

	//флаг занятости скрипта
	bool m_bScriptExecuting;
};