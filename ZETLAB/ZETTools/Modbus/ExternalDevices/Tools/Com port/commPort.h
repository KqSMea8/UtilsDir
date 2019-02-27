// �����, ����������� ������������� ����� ���������, ������������� �� COM �����

#pragma once
#include "afx.h"
#include "afxwin.h"
#include "Modbus\ExternalDevices\Tools\Configurations\Interface configuration\interfaceModuleConfig.h"
#include "Modbus\ExternalDevices\Tools\Com port\Com port device\commPortDevice.h"
#include <list>

class CCommPort
{
private:
    CInterfaceModuleConfig      m_cInterfaceModuleCfg;   // ���������������� ��������� ������������� ������
    std::list<CCommPortDevice*> m_lCommPortDevices;      // ������ ��������� ������������ � ������� COM �����

    LARGE_INTEGER m_liStartTime;   // ����� ������ ������ ���������� � �����
    LARGE_INTEGER m_liFreq;        // �������

    HANDLE m_hCommPort = INVALID_HANDLE_VALUE;   // ����� COM �����

    CWinThread* m_pStreamingThread = nullptr;          // ����� ������ ���� ���������, ����������� �� ���� COM �����
    bool        m_bBreakStreamingThreadFlag = false;   // ���� ��� ��������� ������

private:
    // ������� ������ ����� COM ����� �� InstanceID ����������
    CString findCommPortNameByInstanceId(CString instanceId_);

    // ������� ������������ ����������� � COM ����� ����� ��
    void connect();

    // ������� ������������ ���������� �� COM ����� ����� ��
    void disconnect();

    // ������� ������������ ��������������� � COM ����� ����� ��
    void reconnect();

    // ������� ������ ������ ��� ������ ������
    long getSzToRead();

    // ������� ������� ������ �������� ������
    void clearSendDataBuff();

    // ������� ������� ������ ������ ������
    void clearReadDataBuff();

    // ������� �������� ������
    void sendData(BYTE* data_, DWORD dataSz_);

    // ������� ������ ������� ������ �� COM ����� (����� ������� ���������� �� ������ ������� start())
    double getWorkingTime();

    // ������� �������� ������ �� ����������
    void waitAnswer();

    // ������� ������ ������
    long readData(BYTE* data_, DWORD dataSz_);

    // ������� ������� ������
    // sendData_, sendDataSz_ - ����� ������� ������� ������ � ����� ������� ������� ������
    // recData_, recDataSz_ - ����� ������� ������ � ����� ������� ������
    // ������������ �������� - ���������� ���������� ������ � �����
    long requestData(BYTE* sendData_, long sendDataSz_, BYTE* recData_, long recDataSz_);

public:
    CCommPort();
    ~CCommPort();

    // ������� ��������� ������������ ������������� ������
    void setConfig(CInterfaceModuleConfig& interfaceConfig_);

    // ������� ���������� ������ ����������, ����������� �� COM �����
    void addCommPortDevice(CDeviceModuleConfig& deviceCfg_, CChannelModuleConfig& channelCfg_, long virtChannelNum_ = 0);

    // ������� �������� ����������, ����������� �� COM �����
    void delCommPortDevice(CDeviceModuleConfig& deviceCfg_, CChannelModuleConfig& channelCfg_);

    // ������� ������� ������ ���������, ���������� �� ������ COM �����
    void clearCommPortDevices();

    // ������� ������� �������� ������ �������� ������ �� ��������� �� ������ COM �����
    void start();

    // ������� ��������� �������� ������ �������� ������ �� ��������� �� ������ COM �����
    void stop();

    // ������� ������ ���������� ���������, ���������� �� ������ COM �����
    unsigned long getCommPortDevicesCount();

    // ������� ������ ������ ������������ ������, ����������� �� ������� COM �����
    long getCommPortDeviceVirtChannNum(unsigned long deviceNum_);

    // ������� ������ ���������� ����������� �������� ���������� �� COM �����
    unsigned long getCommPortDeviceSampleCount(unsigned long deviceNum_);

    // ������� ������ ������� �� ���������� �� COM �����
    long getCommPortDeviceSample(unsigned long deviceNum_, double& sampleTime_, double& sampleVal_);

    // ������� ������ ������� ���������� (�������� �� ���)
    bool getDeviceStatus(unsigned long deviceNum_);

protected:
    // ��������� ������� ������ ���������
    static UINT streamingFunction(LPVOID param_);
};