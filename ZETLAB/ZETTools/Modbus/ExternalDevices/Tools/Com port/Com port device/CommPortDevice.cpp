#include "Modbus\ExternalDevices\Tools\Com port\Com port device\commPortDevice.h"

CCommPortDevice::CCommPortDevice()
{
    // �������������� ����������� ������ ������� � ��������
    InitializeCriticalSection(&m_csSampleAccess);
}

CCommPortDevice::~CCommPortDevice()
{
    // ������� ����������� ������ ������� � ��������
    DeleteCriticalSection(&m_csSampleAccess);
}

void CCommPortDevice::setConfig(CDeviceModuleConfig& deviceConfig_, CChannelModuleConfig& channelConfig_)
{
    // ���������� ���������������� ��������� ���������� �� COM �����
    m_cDeviceModuleCfg = deviceConfig_;
    m_cChannelModuleCfg = channelConfig_;
}

bool CCommPortDevice::isConfig(CDeviceModuleConfig& deviceConfig_, CChannelModuleConfig& channelConfig_)
{
    // ��������� ���������� ���������������� ���������� ���������� �� COM �����
    return (m_cDeviceModuleCfg == deviceConfig_) && (m_cChannelModuleCfg == channelConfig_);
}

bool CCommPortDevice::checkIsLive()
{
    // ���������� ��������� ������������, ���� (����� ���������� ������� - ����� ���������� ������) < 5 ���
    return (m_tLastRequestTime - m_tLastAnswerTime) < Devi�eIsDeadTimeOut;
}

bool CCommPortDevice::chekIsNeedNewData(double currTime_)
{
    // ���������� ��������� � ����� ������, ���� (������� ����� - ����� ���������� ������� > period)
    double period = double(1) / m_cChannelModuleCfg.getReadFreq();

    return ((currTime_ - m_tLastRequestTime) > period);
}

DWORD CCommPortDevice::getRequestBuff(BYTE* data_, DWORD dataSz_, double requestTime_)
{
    // �������� ������ ��� ������� ������ �� ����������
    m_tLastRequestTime = requestTime_;

    DWORD packBytesCount = 0;    // ���������� ������ � ������ ������� ������
    BYTE  packDeviceNode = 0;    // ����� ����������, �������� ��������������� �����
    BYTE  packReadCommand = 0;   // ������� ��� ������ ������ � ����������
    uint16_t packReadAddr = 0;   // ����� �� ������� ����� ������ ������
    uint16_t packReadSize = 0;   // ���������� ������, ������� ���������� �������
    uint16_t packCRC16 = 0;      // ����������� ����� ������

    // �������� ���� ��������� ������ �������
    switch (m_cDeviceModuleCfg.getProtocolType())
    {
    case CChannelModuleConfig::ModbusRTUProtocolType:
        packCRC16 = 0xFFFF;

            // ������� ����� ����������
            packDeviceNode = m_cDeviceModuleCfg.getDeviceNode();
            if (packBytesCount + sizeof(packDeviceNode) <= dataSz_)
            {
                memcpy_s(data_, sizeof(packDeviceNode), &packDeviceNode, sizeof(packDeviceNode));
                packBytesCount = packBytesCount + sizeof(packDeviceNode);
                packCRC16 = crc16(packDeviceNode, packCRC16);
            }

            // ����� ������� ��� ������ ������
            packReadCommand = m_cChannelModuleCfg.getRegCommand();
            if (packBytesCount + sizeof(packReadCommand) <= dataSz_)
            {
                memcpy_s(&data_[packBytesCount], sizeof(packReadCommand), &packReadCommand, sizeof(packReadCommand));
                packBytesCount = packBytesCount + sizeof(packReadCommand);
                packCRC16 = crc16(packReadCommand, packCRC16);
            }

            // ������������� ����� �� �������� ����� ������ ������
            packReadAddr = m_cChannelModuleCfg.getRegAddress();
            if (packBytesCount + sizeof(packReadAddr) <= dataSz_)
            {
                memcpy_s(&data_[packBytesCount], 1, (BYTE*)&packReadAddr + 1, 1);
                memcpy_s(&data_[packBytesCount + 1], 1, &packReadAddr, 1);
                packBytesCount = packBytesCount + sizeof(packReadAddr);
                packCRC16 = crc16(*((unsigned char*)&packReadAddr + 1), packCRC16);
                packCRC16 = crc16(*((unsigned char*)&packReadAddr), packCRC16);
            }

            // ������������� ���������� ���� ������� ����� ����������
            packReadSize = m_cChannelModuleCfg.getRegDataTypeSz() / sizeof(uint16_t);
            if (packBytesCount + sizeof(packReadSize) <= dataSz_)
            {
                memcpy_s(&data_[packBytesCount], 1, (BYTE*)&packReadSize + 1, 1);
                memcpy_s(&data_[packBytesCount + 1], 1, &packReadSize, 1);
                packBytesCount = packBytesCount + sizeof(packReadSize);
                packCRC16 = crc16(*((unsigned char*)&packReadSize + 1), packCRC16);
                packCRC16 = crc16(*((unsigned char*)&packReadSize), packCRC16);
            }

            // ������������� ����������� �����
            if (packBytesCount + sizeof(packCRC16) <= dataSz_)
            {
                memcpy_s(&data_[packBytesCount], sizeof(packCRC16), &packCRC16, sizeof(packCRC16));
                packBytesCount = packBytesCount + sizeof(packCRC16);
            }
            break;
    case CChannelModuleConfig::ModbusASCIIProtocolType:
            break;
        default:
            break;
    }

    return packBytesCount;
}

void CCommPortDevice::setAnswerBuff(BYTE* data_, DWORD dataSz_, double answerTime_)
{
    // ������ ��������� ������ ������
    uint16_t packetCalculatedCRC = 0, packetCRC = 0;   // ������������ CRC � �� ������ ������
    // �������� �������, ����� ������� �� ����������� �������, ����� ������� ���������� �� �������
    double sampleVal = 0, sampleNumber = 0, sampleTime = 0;

    // �������� ���� ��������� ������ �������
    switch (m_cDeviceModuleCfg.getProtocolType())
    {
    case CChannelModuleConfig::ModbusRTUProtocolType:
        // ��������� ����������� ����� ������ ������
        if (dataSz_ > sizeof(packetCRC))
        {
            // ������������ CRC �� ������
            packetCalculatedCRC = 0xFFFF;
            for (unsigned int i = 0; i < dataSz_ - sizeof(packetCRC); i++)
                packetCalculatedCRC = crc16(data_[i], packetCalculatedCRC);

            // ����� CRC �� ������
            packetCRC = *(uint16_t*)&data_[dataSz_ - sizeof(packetCRC)];

            // ���� CRC �� ������ � ������������ CRC ���������, ������������ ������
            if (packetCalculatedCRC == packetCRC)
            {
                // ��������� �������� �������: ����� ���� (1 ����) _ ����� ������� (1 ����) _ ���������� ������ (1 ����) _ ... ������ ...
                sampleVal = m_cChannelModuleCfg.getRegDataByDouble(&data_[3], m_cChannelModuleCfg.getRegDataTypeSz());

                // ������������ ����������������� � ���������� ������������
                sampleVal = sampleVal * m_cChannelModuleCfg.getRegMultiplicat() + m_cChannelModuleCfg.getRegAdditive();

                // ��������� ����� ������� ��� ������� ������� �����
                long sampleNumber = long(answerTime_ * m_cChannelModuleCfg.getReadFreq());
                double sampleTime = sampleNumber / m_cChannelModuleCfg.getReadFreq();

                putSample(sampleTime, sampleVal);
                
                // ��� ������������ ������ ����������, ������������ ����� ���������� ������� ������ �� ����������
                m_tLastAnswerTime = sampleTime;
                m_tLastRequestTime = sampleTime;
            }
        }
        break;
    case CChannelModuleConfig::ModbusASCIIProtocolType:
        break;
    default:
        break;
    }
}

void CCommPortDevice::putSample(double& sampleTime_, double& sampleValue_)
{
    EnterCriticalSection(&m_csSampleAccess);
    // ��������� ������� ���������� ��������
    if (m_lSamples.size() > MaxDeviceSamplesCount)
        m_lSamples.pop_front();
    m_lSamples.push_back(Sample(sampleTime_, sampleValue_));
    LeaveCriticalSection(&m_csSampleAccess);
}

long CCommPortDevice::getSamplesCount()
{
    long retVal = 0;

    EnterCriticalSection(&m_csSampleAccess);
    retVal = m_lSamples.size();
    LeaveCriticalSection(&m_csSampleAccess);

    return retVal;
}

long CCommPortDevice::getSample(double& sampleTime_, double& sampleValue_)
{
    long retVal = -1;

    EnterCriticalSection(&m_csSampleAccess);
    Sample currSample;
    if (m_lSamples.size() > 0)
    {
        // ������ ������� ������� �� ������ ��������
        currSample = m_lSamples.front();
        m_lSamples.pop_front();

        sampleTime_ = currSample.first;
        sampleValue_ = currSample.second;

        retVal = 0;
    }
    LeaveCriticalSection(&m_csSampleAccess);

    return retVal;
}

void CCommPortDevice::setVirtChannelNum(long virtChannNum_)
{
    m_lVirtChannelNum = virtChannNum_;
}

long CCommPortDevice::getVirtChannelNum()
{
    return m_lVirtChannelNum;
}

uint16_t CCommPortDevice::crc16(uint8_t byte_, uint16_t lastCRC_)
{
    lastCRC_ ^= (0x00ff & byte_);
    for (uint8_t i = 0; i < 8; i++)
        lastCRC_ = (lastCRC_ & 0x01) ? (lastCRC_ >> 1) ^ 0xA001 : lastCRC_ >> 1;

    return lastCRC_;
}
