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
	//выставляем язык программирования
	spScriptCtl->put_Language(bstrLanguage);
	//сбрасываем все параметры
	spScriptCtl->Reset();
	//выставляем язык программирования заново
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

				//создаем массив
				LPSAFEARRAY psa;
				SAFEARRAYBOUND rgsabound[] = { 1, 0 }; // 3-количество элементов, 
				// 0-номер нижнего элемента
				psa = SafeArrayCreate(VT_VARIANT, 1, rgsabound);

				//инициализируем структуру VARIANT
				VARIANT vFlavors;
				VariantInit(&vFlavors);

				//создаем строку BSTR, в которой хранится передаваемый параметр
				vFlavors.vt = VT_BSTR;
				vFlavors.bstrVal = SysAllocString(paramStr);

				//данная строка записывается в массив
				long numElem = 0;
				HRESULT hRes = SafeArrayPutElement(psa, &numElem, &vFlavors);

				//после записи удаляем строку
				//SysFreeString(vFlavors.bstrVal);
				//удалять строку не нужно, функция VarianClear() сама очистит память

				//добавляем текст программы в скрипт
				spScriptCtl->AddCode(_bstr_t(m_progText));
				//выполняем скрипт
				_variant_t outpar = spScriptCtl->Run(_bstr_t(functionName), &psa);

				//очищаем структуру VARIANT
				VariantClear(&vFlavors);

				//формируем возвращиемое значение
				//_bstr_t bstrReturn = (_bstr_t)outpar;

				m_bScriptExecuting = false;
				return outpar;
			}
			catch (_com_error e)
			{
				IErrorInfo* pErr(nullptr);
				pErr = e.ErrorInfo();
				//получаем указатель на интерфейс ошибок №2 компонента spScriptCtl
				IScriptError* pScrErr(nullptr);
				spScriptCtl->get_Error(&pScrErr);

				//вытаскиваем информацию о номере строки, где произошла ошибка
				long lineNum = 0;
				pScrErr->get_Line(&lineNum);

				//определение характеристик ошибки
				CStringA error_source = e.Source();			//основная причина
				CStringA error_discr = e.Description();  //небольшое пояснение типа ошибки

				//формируем строку текста информации об ошибке
				CStringA err;
				err.Format("Error (%08x)\t%s\t Строка %d",
					e.Error(),			//номер ошибки
					error_discr.GetString(),			//возможной причины ошибки, например - "неизвестный символ"
					lineNum);			//номер строки, где произошла ошибка			
				if (CString(error_source) == L"")
					pParent->MessageBox(L"Ошибка выполнения сценария:\nВозможно, что при вызове неправильно указано название функции.\nВы пытаетесь вызвать функцию- " + functionName, L"Zet.Assistant");
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

		//создаем массив
		LPSAFEARRAY psa;
		SAFEARRAYBOUND rgsabound[]  = { 1, 0 }; // 3-количество элементов, 
											// 0-номер нижнего элемента
		psa = SafeArrayCreate(VT_VARIANT, 1, rgsabound);

		//инициализируем структуру VARIANT
		VARIANT vFlavors;
		VariantInit(&vFlavors);

		//создаем строку BSTR, в которой хранится передаваемый параметр
		vFlavors.vt = VT_BSTR;
		vFlavors.bstrVal = SysAllocString(paramStr);

		//данная строка записывается в массив
		long numElem = 0;
		HRESULT hRes = SafeArrayPutElement(psa, &numElem, &vFlavors);

		//после записи удаляем строку
		//SysFreeString(vFlavors.bstrVal);
		
		//добавляем текст программы в скрипт
		spScriptCtl->AddCode(_bstr_t(m_progText));
		spScriptCtl->AddObject(_bstr_t(objName), objDist, FALSE);
		//выполняем скрипт
		_variant_t outpar = spScriptCtl->Run(_bstr_t(functionName), &psa);
		
		//очищаем структуру VARIANT
		VariantClear(&vFlavors);

		//формируем возвращиемое значение
		//_bstr_t bstrReturn = (_bstr_t)outpar;

		m_bScriptExecuting = false;
		//выполнилось нормально
		bError = 0;
		return outpar;
	}
	 catch(_com_error e)
	{
		//выполнилось с ошибкой
		bError = 1;

		IErrorInfo* pErr(nullptr);
		pErr = e.ErrorInfo();
		//получаем указатель на интерфейс ошибок №2 компонента spScriptCtl
		IScriptError* pScrErr(nullptr);
		spScriptCtl->get_Error(&pScrErr);
		
		//вытаскиваем информацию о номере строки, где произошла ошибка
		long lineNum = 0;
		pScrErr->get_Line(&lineNum);
		//определение характеристик ошибки
		CStringA error_source = e.Source();			//основная причина
		CStringA error_discr = e.Description();  //небольшое пояснение типа ошибки

		//формируем строку текста информации об ошибке
		CStringA err;
		err.Format(//"%s: Error (%08x)\t%s\t Строка %d", 
				   "%s:\"%s\"\n\t пояснение - Error (%08d)", 
				   CStringA(objName).GetString(),
				   error_discr.GetString(),			//возможной причины ошибки, например - "неизвестный символ"
				   e.Error()//,			//номер ошибки
				   //error_discr//,			
				   /*lineNum-1*/);		//номер строки, где произошла ошибка			
		if (CString(error_source) == L"")
			pParent->MessageBox(L"Ошибка выполнения сценария:\nВозможно, что при вызове неправильно указано название функции.\nВы пытаетесь вызвать функцию- " + functionName, L"Zet.Assistant");
		else
			pParent->MessageBox(CString(err), L"Zet.Assistant");
		return _variant_t(L"");
	 }
	m_bScriptExecuting = false;
} else
	return _variant_t(L"");
}