#pragma once
#include <afx.h>
#include "Modbus\ExternalDevices\ModbusExternalModules.zschema1.h"

// Модуль для работы с конфигурационным файлом стороннего устройства

class CDeviceModuleConfig
{
private:
    CString m_sConfigAdr = L"";   // Адрес расположения конфигурационного файла
    Modbus_External_Device_Module m_cDeviceModule;

public:
    CDeviceModuleConfig(CString configAdr_ = L"");
    ~CDeviceModuleConfig();
public:
    // Сохранение
    void save(CString interfaceModuleName_, CString deviceModuleName_);
    // Загрузка
    void load(CString interfaceModuleName_, CString deviceModuleName_);
    // Удаление
    void remove(CString interfaceModuleName_, CString deviceModuleName_);

    // Установка имени устройства
    void setDeviceName(CString deviceName_);
    // Взятие имени устройства
    CString getDeviceName();

    // Установка адреса устройства
    void setDeviceNode(uint8_t deviceNode_);
    // Взятие адреса устройства
    uint8_t getDeviceNode();

    // Установка типа используемого протокола
    void setProtocolType(uint8_t protocolType_);
    // Взятие типа используемого протокола
    uint8_t getProtocolType();

    // Переопределение оператора равно
    friend bool operator==(CDeviceModuleConfig& left_, CDeviceModuleConfig& right_)
    {
        return (left_.getDeviceName() == right_.getDeviceName() &&
            left_.getDeviceNode() == right_.getDeviceNode() &&
            left_.getProtocolType() == right_.getProtocolType());
    };
};