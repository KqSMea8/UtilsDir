// Поиск конфигурационных файлов в директории для хранения конфигурационных файлов

#pragma once
#include <afx.h>
#include <vector>

// Класс для работы со списком устройств, описанных в конфигурационных файлах

class CDeviceFinder
{
private:
	// Список устройств в указанной директории конфигурационных файлов
	std::vector<CString> m_vDevices;
	// Указанная директория конфигурационных файлов
	CString m_sDevicesPath = L"";

public:
	CDeviceFinder(CString path_ = L"");
	~CDeviceFinder();

public:
	unsigned long getDevicesCount();                       // Взятие количества устройств
	CString getDeviceName(unsigned long deviceNumber_);    // Взятие имени устройства
	CString getDevicePath(unsigned long deviceNumber_);    // Взятие директории устройства
	CString getDeviceCfgAdr(unsigned long deviceNumber_);  // Взятие адреса конфигурационного файла устройства
};
