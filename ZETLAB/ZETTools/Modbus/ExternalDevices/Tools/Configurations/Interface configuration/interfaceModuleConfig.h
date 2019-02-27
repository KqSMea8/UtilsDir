#pragma once
#include <afx.h>
#include "Modbus\ExternalDevices\ModbusExternalModules.zschema1.h"

// Модуль для работы с конфигурационным файлом стороннего интерфейсного модуля

class CInterfaceModuleConfig
{
private:
    CString m_sConfigAdr = L"";   // Адрес расположения конфигурационного файла
    Modbus_External_Interface_Module m_cInterfaceModule;

    public:
        CInterfaceModuleConfig(CString configAdr_ = L"");
        ~CInterfaceModuleConfig();
public:
    void save(CString interfaceModuleName_);
    void load(CString interfaceModuleName_);
    void remove(CString interfaceModuleName_);

    void setCommPortInstanceId(CString commPortInstanceId_);
    CString getCommPortInstanceId();

    void setCommPortUserName(CString commPortUserName_);
    CString getCommPortUserName();

    void setCommPortBaudRate(uint32_t baudRate_);
    uint32_t getCommPortBaudRate();

    void setCommPortByteSize(uint8_t byteSize_);
    uint8_t getCommPortByteSize();

    void setCommPortParity(uint8_t parity_);
    uint8_t getCommPortParity();

    void setCommPortStopBits(uint8_t stopBits_);
    uint8_t getCommPortStopBits();

    void  setMaxAnswerTimeOut(float maxAnswerTimeOut_);
    float getMaxAnswerTimeOut();

    void  setMaxNewDataDelay(float maxNewDataDelay_);
    float getMaxNewDataDelay();

   // Переопределение оператора равно
    friend bool operator==(CInterfaceModuleConfig& left_, CInterfaceModuleConfig& right_)
    {
        return (left_.getCommPortBaudRate() == right_.getCommPortBaudRate() &&
            left_.getCommPortByteSize() == right_.getCommPortByteSize() &&
            left_.getCommPortInstanceId() == right_.getCommPortInstanceId() &&
            left_.getCommPortParity() == right_.getCommPortParity() &&
            left_.getCommPortStopBits() == right_.getCommPortStopBits() &&
            left_.getCommPortUserName() == right_.getCommPortUserName() &&
            left_.getMaxAnswerTimeOut() == right_.getMaxAnswerTimeOut() &&
            left_.getMaxNewDataDelay() == right_.getMaxNewDataDelay());
    };
};