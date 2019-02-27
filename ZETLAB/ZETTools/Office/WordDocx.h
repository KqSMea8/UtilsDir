/*! Класс для работы с документами Microsoft Word 2007 и выше.
Читает и записывает документ. Осуществляет поиск строк и замену строк.
*/
#pragma once
#include "OfficeBase.h"

class CWordDocx : public COfficeBase
{
public:
	CWordDocx();
	~CWordDocx();

	int ReplacePictureFromFile(LPCSTR fileName);				//! функция для замены изображения или изображения в документе на изображение из файла

protected:
	int FindDocument();											//! функция для поиска основного документа
};

