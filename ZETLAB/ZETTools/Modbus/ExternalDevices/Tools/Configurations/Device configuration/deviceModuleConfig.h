#pragma once
#include <afx.h>
#include "Modbus\ExternalDevices\ModbusExternalModules.zschema1.h"

// ������ ��� ������ � ���������������� ������ ���������� ����������

class CDeviceModuleConfig
{
private:
    CString m_sConfigAdr = L"";   // ����� ������������ ����������������� �����
    Modbus_External_Device_Module m_cDeviceModule;

public:
    CDeviceModuleConfig(CString configAdr_ = L"");
    ~CDeviceModuleConfig();
public:
    // ����������
    void save(CString interfaceModuleName_, CString deviceModuleName_);
    // ��������
    void load(CString interfaceModuleName_, CString deviceModuleName_);
    // ��������
    void remove(CString interfaceModuleName_, CString deviceModuleName_);

    // ��������� ����� ����������
    void setDeviceName(CString deviceName_);
    // ������ ����� ����������
    CString getDeviceName();

    // ��������� ������ ����������
    void setDeviceNode(uint8_t deviceNode_);
    // ������ ������ ����������
    uint8_t getDeviceNode();

    // ��������� ���� ������������� ���������
    void setProtocolType(uint8_t protocolType_);
    // ������ ���� ������������� ���������
    uint8_t getProtocolType();

    // ��������������� ��������� �����
    friend bool operator==(CDeviceModuleConfig& left_, CDeviceModuleConfig& right_)
    {
        return (left_.getDeviceName() == right_.getDeviceName() &&
            left_.getDeviceNode() == right_.getDeviceNode() &&
            left_.getProtocolType() == right_.getProtocolType());
    };
};