//------------------------------------------------------------------------------
/*			Класс предназначен для унификации вызова справки в программах,
написанных НЕ на базовом классе CDialog_ZET. Справка может быть либо в отдельном
файле, либо в общем файле. Код взят из файла Dialog_ZET.cpp.

			Функция Create.
		Параметр pZetPath требуется для определения папки с файлами справок.
Если pZetPath = NULL, то экземпляр класса CDZetPath будет создан динамически,
но для этого требуется задать pParent. Поэтому обязательно должен быть
задан котя-бы один из параметров pZetPath и pParent.
*/
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <Dialog_ZET\CDZetPath.h>
//------------------------------------------------------------------------------
class COnHelp
{
private:
	CString m_sExeName;
	CString m_sSystemRoot;
	CString m_sFileHelp;
	PROCESS_INFORMATION m_piHelp;

protected:

public:
	COnHelp();
	~COnHelp();
	bool Create(CDZetPath *pZetPath, CWnd *pParent = NULL);
	bool Create(const CString sFolderHelp, CString chmName = L"");

	// функции свойств --------------------------------------------------------
	CString GetSystemRoot() {return m_sSystemRoot;}
	CString GetExeName() {return m_sExeName;}
	CString GetFileHelp() {return m_sFileHelp;}

	// функции класса ---------------------------------------------------------
	bool OnHelp();
};
//------------------------------------------------------------------------------