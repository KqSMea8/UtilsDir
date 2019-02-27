// ���������� �� COM �����
#include "Modbus\ExternalDevices\Tools\Configurations\Device configuration\deviceModuleConfig.h"
#include "Modbus\ExternalDevices\Tools\Configurations\Channel configuration\channelModuleConfig.h"
#include <list>

typedef std::pair<double, double> Sample;   // ���� ������ ������ (�����, ������)
typedef std::list<Sample> Samples;          // ������� ������

class CCommPortDevice
{
private:
    // ����� � ��������, ���� � ������� ����� ������� ���������� �� ��������, ��� ������ (���������� ����� ������)
    const double Devi�eIsDeadTimeOut = 5;
    // ������������ ���������� �������� �������� (��� ������������ �������� ����� ������ �������, ����� ������� �������� ����� ���������� ������)
    const unsigned long MaxDeviceSamplesCount = 100;

    CDeviceModuleConfig  m_cDeviceModuleCfg;    // ������������ �������������� ����������
    CChannelModuleConfig m_cChannelModuleCfg;   // ������������ �������������� ������

    CRITICAL_SECTION m_csSampleAccess;   // ����������� ������ ������� � ��������
    Samples m_lSamples;                  // ������� ������

    double m_tLastRequestTime = 0;    // ����� �������� ���������� ������� ������ �� ������ ������ ����������
    double m_tLastAnswerTime  = 0;    // ����� ��������� ���������� ������ �� ������ ������ ����������

    long m_lVirtChannelNum = 0;   // ����� ������������ ������, ���������������� ������� ����������

    uint16_t crc16(uint8_t byte_, uint16_t lastCRC_);            // ������� CRC16
    void putSample(double& sampleTime_, double& sampleValue_);   // ���������� ������� �� ����������
public:
    CCommPortDevice();
    ~CCommPortDevice();

    // �������� ����������������� ���������� (���������� ��������, �� ������� Devi�eIsDeadTimeOut)
    bool checkIsLive();
    // �������� ���������� �� ��, ��� ���������� ��������� ����� ������ ������
    bool chekIsNeedNewData(double currTime_);

    // ������������� ���������������� ��������� �������������� ������
    void setConfig(CDeviceModuleConfig& deviceConfig_, CChannelModuleConfig& channelConfig_);
    // �������� ���������� �� �� ��� ��� ���������� � ���������� �����������
    bool isConfig(CDeviceModuleConfig& deviceConfig_, CChannelModuleConfig& channelConfig_);

    // ������� ������ ������ ������� �� ������ ������ �� ����������
    DWORD getRequestBuff(BYTE* data_, DWORD dataSz_, double requestTime_);

    // ������� ������������� ��������� ������ �� ���������� �� ������� ������� ������
    void setAnswerBuff(BYTE* data_, DWORD dataSz_, double answerTime_);

    // ������ ���������� �������������� �������� ����������
    long getSamplesCount();
    // ������ ��������������� ������� ����������
    long getSample(double& sampleTime_, double& sampleValue_);

    // ��������� ������ ������������ ������, ���������������� ������� ����������
    void setVirtChannelNum(long virtChannNum_);
    // ������ ������ ������������ ������, ���������������� ������� ����������
    long getVirtChannelNum();
};