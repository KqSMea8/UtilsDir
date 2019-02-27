#pragma once
#include <afx.h>
#include "Modbus\ExternalDevices\ModbusExternalModules.zschema1.h"

class CChannelModuleConfig
{
private:
    CString m_sConfigAdr = L"";   // Адрес расположения конфигурационного файла
    Modbus_External_Channel_Module m_cChannelModule;

public:
    CChannelModuleConfig(CString configAdr_ = L"");
    ~CChannelModuleConfig();

public:
    enum ProtocolTypes   // Типы протоколов
    {
        ModbusRTUProtocolType = 0,    // Тип протокола обмена данными Modbus RTU
        ModbusASCIIProtocolType = 1   // Тип протокола обмена данными Modbus ASCII
    };

    enum DataTypes   // Типы данных
    {
        Float_AB_CD_Type = 0,
        Float_CD_AB_Type = 1,
        Float_DC_BA_Type = 2,
        Int32_AB_CD_Type = 3,
        Int32_CD_AB_Type = 4,
        Int32_DC_BA_Type = 5,
        Int16_A_B_Type = 6,
        Int16_B_A_Type = 7
    };

    // Сохранение
    void save(CString interfaceModuleName_, CString deviceModuleName_, CString channelModuleName_);
    // Загрузка
    void load(CString interfaceModuleName_, CString deviceModuleName_, CString channelModuleName_);
    // Удаление
    void remove(CString interfaceModuleName_, CString deviceModuleName_, CString channelModuleName_);

    // Установка имени канала
    void setChannelName(CString channelName_);
    // Взятие имени устройства
    CString getChannelName();

    // Установка адреса регистра, по которому будем считывать данные
    void setRegAddress(uint16_t regAddress_);
    // Взятие адреса регистра
    uint16_t getRegAddress();

    // Установка команды для чтения данных из регистра
    void setRegCommand(uint8_t regCommand_);
    // Чтение команды для чтения данных
    uint8_t getRegCommand();

    // Установка типа данных, хранящихся в регистре
    void setRegDataType(uint16_t regDataType_);
    // Чтение типа данных, хранящихся в регистре
    uint16_t getRegDataType();
    // Взятие размера типа данных
    uint16_t getRegDataTypeSz();
    // Взятие значения в типе double
    double getRegDataByDouble(BYTE* data_, DWORD dataSz_);

    // Установка частоты опроса данных
    void setRegReadFreq(float regReadFreq_);
    // Взятие частоты опроса данных
    float getReadFreq();

    // Установка единицы измерения, значения хранящегося в заданном регистре
    void setRegMeasure(CString regMeasure_);
    // Взятие единицы измерения, значения хранящегося в заданном регистре
    CString getRegMeasure();

    // Установка минимального значения, котороем можед зафиксироваться регистром
    void setRegResolution(float regResolution_);
    // Взятие минимального значения, которое может зафиксироваться регистром
    float getRegResolutoin();

    // Установка максимального значения по регистру
    void setRegMaxLevel(float regMaxLevel_);
    // Взятие максимального значения по регистру
    float getRegMaxLevel();

    // Установка опорного значения по регистру
    void setRegReference(float regReference_);
    // Взятие опорного значения
    float getRegReference();

    // Установка мультипликативного коэффициента при обработке значения из регистра
    void setRegMultiplicat(float regMultiplicat_);
    // Взятие мультипликативного коэффициента при обработке значения из регистра
    float getRegMultiplicat();

    // Установка аддитивного коэффициента при обработке значения из регистра
    void setRegAdditive(float regAdditive_);
    // Взятие аддитивного коэффициента при обработке значения из регистра
    float getRegAdditive();

    // Переопределение оператора равно
    friend bool operator==(CChannelModuleConfig& left_, CChannelModuleConfig& right_)
    {
        return (left_.getChannelName() == right_.getChannelName() &&
            left_.getReadFreq() == right_.getReadFreq() &&
            left_.getRegAdditive() == right_.getRegAdditive() &&
            left_.getRegAddress() == right_.getRegAddress() &&
            left_.getRegCommand() == right_.getRegCommand() &&
            left_.getRegDataType() == right_.getRegDataType() &&
            left_.getRegMaxLevel() == right_.getRegMaxLevel() &&
            left_.getRegMeasure() == right_.getRegMeasure() &&
            left_.getRegMultiplicat() == right_.getRegMultiplicat() &&
            left_.getRegReference() == right_.getRegReference() &&
            left_.getRegResolutoin() == right_.getRegResolutoin());
    };
};
