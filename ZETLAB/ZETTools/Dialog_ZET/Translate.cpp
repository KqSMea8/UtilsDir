//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "Translate.h"
#include <Dialog_ZET/ConnectToZetPath.h>
//-----------------------------------------------------------------------------
CString g_exeName(L"");
CString g_sLanguageDst(L"");		// язык выходных фраз - из локали.
CString g_sLanguageSrc(L"ru-RU");	// язык исходных фраз - русский;
//-----------------------------------------------------------------------------
// Задание языков в словаре:
// язык исходных фраз - русский;
// язык выходных фраз - из реестра или из локали.
void InitLanguageDst()
{
	long err = zdSetLanguageSrc(g_sLanguageSrc);

	g_sLanguageDst.Preallocate(LOCALE_NAME_MAX_LENGTH);
	// Из реестра вычитываю язык
	if (0 != zpQueryLanguage(g_sLanguageDst.GetBuffer(), LOCALE_NAME_MAX_LENGTH))
	{	// вычитать язык из реестра не удалось, пытаюсь прочитать из локали
		GetUserDefaultLocaleName(g_sLanguageDst.GetBuffer(), LOCALE_NAME_MAX_LENGTH);
	}
	g_sLanguageDst.ReleaseBuffer();
	if (!g_sLanguageDst.IsEmpty())
	{// Язык вычитан успешно (из реестра или из локали)
		// устанавливаем в словарь этот язык
		if (0 != zdSetLanguageDst(g_sLanguageDst))
		{	// установилось не совсем то, что просили или не установилось
			err = zdGetLanguageDst(g_sLanguageDst.GetBuffer(), LOCALE_NAME_MAX_LENGTH);
			g_sLanguageDst.ReleaseBuffer();
		}
	}
	// в реестр НИЧЕГО писать НЕЛЬЗЯ!
}
//-----------------------------------------------------------------------------
CString GetLanguageDst()
{
	return g_sLanguageDst;
}
//-----------------------------------------------------------------------------
void SetLanguageSrc(CString newVal)
{
	g_sLanguageSrc = newVal;
	InitLanguageDst();
}
//-----------------------------------------------------------------------------
CString GetLanguageSrc()
{
	return g_sLanguageSrc;
}
//-----------------------------------------------------------------------------
// Эту функцию необходимо вызвать один раз в конструкторе программы.
// Задание языков в словаре:
// язык исходных фраз - русский;
// язык выходных фраз - из локалию.
// Задание параметра перевода _exeName:
// если _exeName = L"", то параметр - это имя программы.
void InitZetDictionary(_In_ const CString & _exeName)
{
	InitLanguageDst();

	g_exeName = _exeName.IsEmpty() ? (CString)AfxGetAppName() + L".exe" : _exeName;
}
//-----------------------------------------------------------------------------
// Перевод фразы. Языки перевода следует задавать заранее.
// _String - переводимая фраза;
// sParam - параметр перевода (если не задан, то имя программы).
// Возвращаемая строка: если в словаре есть исходная строка,
// то - переведенная строка,
// иначе - исходная строка.
CString TranslateString(_In_ const CString & _String, _In_opt_ const CString & sParam)
{
	DWORD dwSize(0);	
	if (2 == (sParam.IsEmpty() ?
		zdTranslate(_String, g_exeName, nullptr, &dwSize) :
		zdTranslate(_String, sParam,	nullptr, &dwSize)))
	{
		CString Result(L"");
		Result.Preallocate(dwSize);
		if (0 == zdGetResultTranslate(Result.GetBuffer(), dwSize))
		{
			Result.ReleaseBuffer();

			CString TrimString(_String);		// Временная строка
			CString LeftPart(L"");				// Левая усеченая часть
			CString RightPart(L"");				// Правая усеченая часть

			// запоминаем левую и правую часть строки, которые могут быть отрезаны через Trim
			TrimString.TrimLeft();
			LeftPart = _String.Left(_String.GetLength() - TrimString.GetLength());

			TrimString.TrimRight();
			RightPart = _String.Right(_String.GetLength() - TrimString.GetLength() - LeftPart.GetLength());

			// возвращаем переведенную строку с добавлением отрезаных частей
			return LeftPart + Result + RightPart;
		}
	}

	return _String;
}
//-----------------------------------------------------------------------------
// Перевод фразы из ресурсов. Языки перевода следует задавать заранее.
// nID - дескриптор переводимой фраза в ресурсах.
// Параметр перевода - имя программы.
// Возвращаемая строка: если в словаре есть исходная строка
// то - переведенная строка,
// иначе - исходная строка.
// ВНИМАНИЕ!!! В ресурсах не может быть WCHAR.
CString TranslateResourse(_In_ const UINT & nID, _In_opt_ const CString & sParam)
{
	CString Stroka(L"");
	if (Stroka.LoadString(nID) != FALSE)
		Stroka = TranslateString(Stroka, sParam);
	return Stroka;
}
//-----------------------------------------------------------------------------
// Принудительное копирование непереведенных фраз из буфера словаря в текстовый файл.
// Функция предназначена для программ, которые закрываются с помощью exit(0).
void ExitProgramm(_In_ const int code)
{
	zdCopyNotFindPhraseToTxt();
	exit(code);
}
//-----------------------------------------------------------------------------
// Перевод фразы в которой используются единицы измерения. Языки перевода следует задавать заранее.
// _String - переводимая фраза;
// sParam - параметр перевода (если не задан, то имя программы).
// Возвращаемая строка: если в словаре есть исходная строка и она имеет единицы измерения,
// то - разделяем и переводим строку,
// иначе - возвращаем исходную строку.
CString TranslateDividedString(_In_ const CString & _String, _In_opt_ const CString & sParam)
{
	CString result(L"");
	if (!_String.IsEmpty())
	{
		bool bFlag(true);
		int index = _String.ReverseFind(L',');
		if (_String[index + 1] == L' ')
		{	//Теперь разделяем строку и переводим
			wchar_t * pTmpL = (wchar_t *)_String.GetString();
			pTmpL[index] = 0;

			DWORD sizeL(0), sizeR(0);
			if (2 ==
				(sParam.IsEmpty() ?
				zdTranslate(pTmpL, g_exeName, nullptr, &sizeL) :
				zdTranslate(pTmpL, sParam, nullptr, &sizeL)))
			{
				result.Preallocate(sizeL);
				if (0 == zdGetResultTranslate(result.GetBuffer(), sizeL))
				{
					if (2 == (zdTranslate(_String.GetString() + index + 2, nullptr, nullptr, &sizeR)))
					{
						result.Preallocate(sizeL + sizeR + 1);
						wchar_t * pTmp = result.GetBuffer() + sizeL - 1;
						*pTmp++ = L',';
						*pTmp++ = L' ';
						if (0 == zdGetResultTranslate(pTmp, sizeR))
						{
							bFlag = false;
							result.ReleaseBuffer();
						}
					}
				}
			}
			pTmpL[index] = L',';
		}
		if (bFlag)
			result = TranslateString(_String, sParam);
	}
	return result;
}
//-----------------------------------------------------------------------------