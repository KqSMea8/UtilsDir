#pragma once
#include <afx.h>
#include "Modbus\ExternalDevices\ModbusExternalModules.zschema1.h"

class CChannelModuleConfig
{
private:
    CString m_sConfigAdr = L"";   // ����� ������������ ����������������� �����
    Modbus_External_Channel_Module m_cChannelModule;

public:
    CChannelModuleConfig(CString configAdr_ = L"");
    ~CChannelModuleConfig();

public:
    enum ProtocolTypes   // ���� ����������
    {
        ModbusRTUProtocolType = 0,    // ��� ��������� ������ ������� Modbus RTU
        ModbusASCIIProtocolType = 1   // ��� ��������� ������ ������� Modbus ASCII
    };

    enum DataTypes   // ���� ������
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

    // ����������
    void save(CString interfaceModuleName_, CString deviceModuleName_, CString channelModuleName_);
    // ��������
    void load(CString interfaceModuleName_, CString deviceModuleName_, CString channelModuleName_);
    // ��������
    void remove(CString interfaceModuleName_, CString deviceModuleName_, CString channelModuleName_);

    // ��������� ����� ������
    void setChannelName(CString channelName_);
    // ������ ����� ����������
    CString getChannelName();

    // ��������� ������ ��������, �� �������� ����� ��������� ������
    void setRegAddress(uint16_t regAddress_);
    // ������ ������ ��������
    uint16_t getRegAddress();

    // ��������� ������� ��� ������ ������ �� ��������
    void setRegCommand(uint8_t regCommand_);
    // ������ ������� ��� ������ ������
    uint8_t getRegCommand();

    // ��������� ���� ������, ���������� � ��������
    void setRegDataType(uint16_t regDataType_);
    // ������ ���� ������, ���������� � ��������
    uint16_t getRegDataType();
    // ������ ������� ���� ������
    uint16_t getRegDataTypeSz();
    // ������ �������� � ���� double
    double getRegDataByDouble(BYTE* data_, DWORD dataSz_);

    // ��������� ������� ������ ������
    void setRegReadFreq(float regReadFreq_);
    // ������ ������� ������ ������
    float getReadFreq();

    // ��������� ������� ���������, �������� ����������� � �������� ��������
    void setRegMeasure(CString regMeasure_);
    // ������ ������� ���������, �������� ����������� � �������� ��������
    CString getRegMeasure();

    // ��������� ������������ ��������, �������� ����� ��������������� ���������
    void setRegResolution(float regResolution_);
    // ������ ������������ ��������, ������� ����� ��������������� ���������
    float getRegResolutoin();

    // ��������� ������������� �������� �� ��������
    void setRegMaxLevel(float regMaxLevel_);
    // ������ ������������� �������� �� ��������
    float getRegMaxLevel();

    // ��������� �������� �������� �� ��������
    void setRegReference(float regReference_);
    // ������ �������� ��������
    float getRegReference();

    // ��������� ������������������ ������������ ��� ��������� �������� �� ��������
    void setRegMultiplicat(float regMultiplicat_);
    // ������ ������������������ ������������ ��� ��������� �������� �� ��������
    float getRegMultiplicat();

    // ��������� ����������� ������������ ��� ��������� �������� �� ��������
    void setRegAdditive(float regAdditive_);
    // ������ ����������� ������������ ��� ��������� �������� �� ��������
    float getRegAdditive();

    // ��������������� ��������� �����
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
