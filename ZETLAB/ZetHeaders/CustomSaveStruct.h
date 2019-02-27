//------------------------------------------------------------------------------
// Обязательная часть структуры параметров программ ZetLab.
// Пример работы с этой структурой см. в программе "Вольтметр постоянного тока"
//------------------------------------------------------------------------------
#pragma once
#include <stdint.h>
#include <ZetTools\include\ZetTools.h>	// ztStringCopyW
//------------------------------------------------------------------------------
// 62 - для выравнивания структуры на 8 байт (sizeof(WINDOWPLACEMENT) == 44)
#define SIZE_NAME_PROGRAM	62
//------------------------------------------------------------------------------
struct CustomSaveStructure
{
	uint32_t size;					// размер структуры, которая пока не известна
	uint32_t numberProgram;			// номер запущенной копии
	WINDOWPLACEMENT wPlace;			// параметры расположения окна программы
	wchar_t nameProgram[SIZE_NAME_PROGRAM];	// имя программы

	CustomSaveStructure()
	{
		ZeroMemory(this, sizeof(CustomSaveStructure));
		wPlace.length = sizeof(WINDOWPLACEMENT);
	}

	~CustomSaveStructure() {}

	CustomSaveStructure(const CustomSaveStructure & val)
	{
		*this = val;
	}

	CustomSaveStructure & operator = (const CustomSaveStructure & val)
	{
		auto size_s = sizeof(CustomSaveStructure);
		memcpy_s(this, size_s, &val, size_s);
		return *this;
	}

	// Безопасное задание имени
	bool SetName(LPCWSTR pName)
	{
		return 0 == ztStringCopyW(nameProgram, SIZE_NAME_PROGRAM, pName);
	}

	// Позиционирование окна программы
	void SetPlacement(HWND hWnd)
	{
		if ((wPlace.length == sizeof(WINDOWPLACEMENT)) &&
			((wPlace.rcNormalPosition.right - wPlace.rcNormalPosition.left) > 0) &&
			(wPlace.rcNormalPosition.bottom - wPlace.rcNormalPosition.top) > 0)
			::SetWindowPlacement(hWnd, &wPlace);
	}

	// Сохранение параметров позиционирования окна программы
	void GetPlacement(HWND hWnd)
	{
		ZeroMemory(&wPlace, sizeof(WINDOWPLACEMENT));
		wPlace.length = sizeof(WINDOWPLACEMENT);
		::GetWindowPlacement(hWnd, &wPlace);
		if (wPlace.showCmd == SW_SHOWMINIMIZED)
			wPlace.showCmd = SW_SHOWNORMAL;
	}
};
//------------------------------------------------------------------------------