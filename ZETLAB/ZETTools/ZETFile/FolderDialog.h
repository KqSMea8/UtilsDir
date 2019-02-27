//------------------------------------------------------------------------------
//	Класс предназначен для вызова диалога выбора папки (не файла!).
//	Возможно расширение функциональности класса.
//------------------------------------------------------------------------------
/*	Использование класса:
	CFolderDialog cd(L"Выбор папки для ...", L"Maker", L"c:", this);
	if (cd.DoModal() == IDOK)
	{
		CString folder = cd.GetSelectedFolder();
	}	*/
//------------------------------------------------------------------------------
/*	Если задан владелец класса, то вызываемый диалог будет модальным.
	Если задана корневая папка (например: с:), то поиск будет только в
подпапках (только на диске С).
	Если задана текущая папка, то по умолчанию она будет выбранной.
	Заголовок - это не имя окна поиска. Он отображается внутри окна,
в верхней строке.

	Есть функции задания владельца, заголовка и упомянутых папок.

	Флаги класса
	m_bAddingSlash(false) - добавление к имени выбранной папки в конец слеша	  
	m_bStatusText(true) - отображение выбранной папки под заголовком.
	m_bViewFilesInFolder(true) - отображение файлов в папке. Влияет на имя окна.
	m_bCreateNewFolder(false) - кнопка создание новой папки. true подавляет
			m_bStatusText и m_bViewFilesInFolder;			*/
//------------------------------------------------------------------------------
#pragma once
#include "afxwin.h"
//------------------------------------------------------------------------------
class CFolderDialog
{
private:
	HWND m_hWndOwner;
	CString m_sTitle;
	CString m_sRoot;
	CString m_sFolder;

protected:

public:
	CFolderDialog(const CString sTitle = L"", const CString sCurFolder = L"",
		const CString sRoot = L"", const CWnd *pOwner = nullptr);
//	virtual ~CFolderDialog();

	bool m_bAddingSlash;
	bool m_bCreateNewFolder;
	bool m_bStatusText;
	bool m_bViewFilesInFolder;

	// функции свойств --------------------------------------------------------
	CString GetSelectedFolder();
	CString GetRootFolder();
	CString GetTitle();

	void SetCurFolder(const CString sCurFolder);
	void SetOwner(const CWnd *pOwner);
	void SetRootFolder(const CString root);
	void SetTitle(const CString title);

	// функции класса ---------------------------------------------------------
	INT_PTR DoModal();
};
//------------------------------------------------------------------------------