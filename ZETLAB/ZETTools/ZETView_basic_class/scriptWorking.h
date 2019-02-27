#pragma once

// TODO : ������� ����� � ����� ������� ������� ������ ���������������

#import "C:\WINDOWS\System32\msscript.ocx"
using namespace MSScriptControl;

struct CScriptWork
{
	CScriptWork::CScriptWork(CWnd* parent)
		: spScriptCtl(__uuidof(ScriptControl))
	{
		//���������� ���� ��������� �������
		m_bScriptExecuting = false;

		pParent = parent;
		//������, ������ ��-���������
		m_progText = L"Function Initialization(param)\n"
					 L"Dim Input\n"
					 L"Input = InputBox(\"\",param,\"����� �����������\")\n" 
					 L"Initialization = Input\n"
					 L"end Function\n";

		//���������� ���� ��-���������
		bstrLanguage = L"VBScript";
	}
private:
	CWnd* pParent;
	//�����, ����������� � ������
	CString m_progText;
	//���� ���������� ����
	_bstr_t bstrLanguage;
	//����������, ��� ������
	IScriptControlPtr spScriptCtl;
public:

	//��������� ����� �������
	void setProgText(CString text);
	//���������� ������� ����
	void setLanguage_Basic();
	void setLanguage_Java();
	//������ ������� ���� ��������
	void acceptCurrentLanguage();

	//��������� ������
	VARIANT executeScript(CString functionName, CString paramStr);
	VARIANT executeScript(IDispatch* objDist, CString objName, CString functionName, CString paramStr, int& bError);

	//���� ��������� �������
	bool m_bScriptExecuting;
};