#include "StdAfx.h"
#include "scriptWorking.h"
void CScriptWork::setProgText(CString text)
{
	m_progText = text;
}

void CScriptWork::setLanguage_Basic()
{
	bstrLanguage = L"VBScript";
}
void CScriptWork::setLanguage_Java()
{
	bstrLanguage = L"JScript";
}

void CScriptWork::acceptCurrentLanguage()
{
	//���������� ���� ����������������
	spScriptCtl->put_Language(bstrLanguage);
	//���������� ��� ���������
	spScriptCtl->Reset();
	//���������� ���� ���������������� ������
	spScriptCtl->put_Language(bstrLanguage);
}

VARIANT CScriptWork::executeScript(CString functionName, CString paramStr)
{
	if (!m_bScriptExecuting)
	{
		//spScriptCtl->CodeObject != NULL
		if (spScriptCtl != NULL)
		{
			m_bScriptExecuting = true;
			try  // Make sure exception handling is turned on.
			{
				acceptCurrentLanguage();

				//������� ������
				LPSAFEARRAY psa;
				SAFEARRAYBOUND rgsabound[] = { 1, 0 }; // 3-���������� ���������, 
				// 0-����� ������� ��������
				psa = SafeArrayCreate(VT_VARIANT, 1, rgsabound);

				//�������������� ��������� VARIANT
				VARIANT vFlavors;
				VariantInit(&vFlavors);

				//������� ������ BSTR, � ������� �������� ������������ ��������
				vFlavors.vt = VT_BSTR;
				vFlavors.bstrVal = SysAllocString(paramStr);

				//������ ������ ������������ � ������
				long numElem = 0;
				HRESULT hRes = SafeArrayPutElement(psa, &numElem, &vFlavors);

				//����� ������ ������� ������
				//SysFreeString(vFlavors.bstrVal);
				//������� ������ �� �����, ������� VarianClear() ���� ������� ������

				//��������� ����� ��������� � ������
				spScriptCtl->AddCode(_bstr_t(m_progText));
				//��������� ������
				_variant_t outpar = spScriptCtl->Run(_bstr_t(functionName), &psa);

				//������� ��������� VARIANT
				VariantClear(&vFlavors);

				//��������� ������������ ��������
				//_bstr_t bstrReturn = (_bstr_t)outpar;

				m_bScriptExecuting = false;
				return outpar;
			}
			catch (_com_error e)
			{
				IErrorInfo* pErr(nullptr);
				pErr = e.ErrorInfo();
				//�������� ��������� �� ��������� ������ �2 ���������� spScriptCtl
				IScriptError* pScrErr(nullptr);
				spScriptCtl->get_Error(&pScrErr);

				//����������� ���������� � ������ ������, ��� ��������� ������
				long lineNum = 0;
				pScrErr->get_Line(&lineNum);

				//����������� ������������� ������
				CStringA error_source = e.Source();			//�������� �������
				CStringA error_discr = e.Description();  //��������� ��������� ���� ������

				//��������� ������ ������ ���������� �� ������
				CStringA err;
				err.Format("Error (%08x)\t%s\t ������ %d",
					e.Error(),			//����� ������
					error_discr.GetString(),			//��������� ������� ������, �������� - "����������� ������"
					lineNum);			//����� ������, ��� ��������� ������			
				if (CString(error_source) == L"")
					pParent->MessageBox(L"������ ���������� ��������:\n��������, ��� ��� ������ ����������� ������� �������� �������.\n�� ��������� ������� �������- " + functionName, L"Zet.Assistant");
				else
					pParent->MessageBox(CString(err), L"Zet.Assistant");
				return _variant_t(L"");
			}
			m_bScriptExecuting = false;
		}
		else
			return _variant_t(L"");
	}
	return _variant_t(L"");
}

VARIANT CScriptWork::executeScript(IDispatch* objDist, CString objName, CString functionName, CString paramStr, int& bError)
{
if (!m_bScriptExecuting)
{
	m_bScriptExecuting = true;
	try  // Make sure exception handling is turned on.
	   {
		acceptCurrentLanguage();

		//������� ������
		LPSAFEARRAY psa;
		SAFEARRAYBOUND rgsabound[]  = { 1, 0 }; // 3-���������� ���������, 
											// 0-����� ������� ��������
		psa = SafeArrayCreate(VT_VARIANT, 1, rgsabound);

		//�������������� ��������� VARIANT
		VARIANT vFlavors;
		VariantInit(&vFlavors);

		//������� ������ BSTR, � ������� �������� ������������ ��������
		vFlavors.vt = VT_BSTR;
		vFlavors.bstrVal = SysAllocString(paramStr);

		//������ ������ ������������ � ������
		long numElem = 0;
		HRESULT hRes = SafeArrayPutElement(psa, &numElem, &vFlavors);

		//����� ������ ������� ������
		//SysFreeString(vFlavors.bstrVal);
		
		//��������� ����� ��������� � ������
		spScriptCtl->AddCode(_bstr_t(m_progText));
		spScriptCtl->AddObject(_bstr_t(objName), objDist, FALSE);
		//��������� ������
		_variant_t outpar = spScriptCtl->Run(_bstr_t(functionName), &psa);
		
		//������� ��������� VARIANT
		VariantClear(&vFlavors);

		//��������� ������������ ��������
		//_bstr_t bstrReturn = (_bstr_t)outpar;

		m_bScriptExecuting = false;
		//����������� ���������
		bError = 0;
		return outpar;
	}
	 catch(_com_error e)
	{
		//����������� � �������
		bError = 1;

		IErrorInfo* pErr(nullptr);
		pErr = e.ErrorInfo();
		//�������� ��������� �� ��������� ������ �2 ���������� spScriptCtl
		IScriptError* pScrErr(nullptr);
		spScriptCtl->get_Error(&pScrErr);
		
		//����������� ���������� � ������ ������, ��� ��������� ������
		long lineNum = 0;
		pScrErr->get_Line(&lineNum);
		//����������� ������������� ������
		CStringA error_source = e.Source();			//�������� �������
		CStringA error_discr = e.Description();  //��������� ��������� ���� ������

		//��������� ������ ������ ���������� �� ������
		CStringA err;
		err.Format(//"%s: Error (%08x)\t%s\t ������ %d", 
				   "%s:\"%s\"\n\t ��������� - Error (%08d)", 
				   CStringA(objName).GetString(),
				   error_discr.GetString(),			//��������� ������� ������, �������� - "����������� ������"
				   e.Error()//,			//����� ������
				   //error_discr//,			
				   /*lineNum-1*/);		//����� ������, ��� ��������� ������			
		if (CString(error_source) == L"")
			pParent->MessageBox(L"������ ���������� ��������:\n��������, ��� ��� ������ ����������� ������� �������� �������.\n�� ��������� ������� �������- " + functionName, L"Zet.Assistant");
		else
			pParent->MessageBox(CString(err), L"Zet.Assistant");
		return _variant_t(L"");
	 }
	m_bScriptExecuting = false;
} else
	return _variant_t(L"");
}