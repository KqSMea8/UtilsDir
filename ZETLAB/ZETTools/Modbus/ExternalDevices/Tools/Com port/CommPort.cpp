#include "Modbus\ExternalDevices\Tools\Com port\commPort.h"
#include "Modbus\ExternalDevices\Tools\Secondary functions\secondaryFunctions.h"
#include <vector>

CCommPort::CCommPort()
{
}

CCommPort::~CCommPort()
{

    // ������� ������ ������������ ��������� �� COM �����
    clearCommPortDevices();
}

void CCommPort::setConfig(CInterfaceModuleConfig& config_)
{
    // ���������� ���������������� ��������� ������������� ������
    m_cInterfaceModuleCfg = config_;
}

void CCommPort::connect()
{
    // �������� COM �����
    CString portName = findCommPortNameByInstanceId(m_cInterfaceModuleCfg.getCommPortInstanceId());

    m_hCommPort = INVALID_HANDLE_VALUE;
	if (!portName.IsEmpty())   // ���� ����� COM ����� ������� �����
    {
        m_hCommPort = ::CreateFile(portName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, NULL, NULL);

        // �������� �� ��, ��� ���� ��������
        if (m_hCommPort != INVALID_HANDLE_VALUE)
        {
            // ��������� �������� COM ����� (��������� ��������� �������� � ���������)
            DCB dcbSerialParams = { 0 };
            dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

            BOOL retVal = GetCommState(m_hCommPort, &dcbSerialParams);
            if (retVal)
            {
                dcbSerialParams.BaudRate = m_cInterfaceModuleCfg.getCommPortBaudRate();
                dcbSerialParams.ByteSize = m_cInterfaceModuleCfg.getCommPortByteSize();
                dcbSerialParams.StopBits = m_cInterfaceModuleCfg.getCommPortStopBits();
                dcbSerialParams.Parity   = m_cInterfaceModuleCfg.getCommPortParity();

                retVal = SetCommState(m_hCommPort, &dcbSerialParams);
            }
        }
    }
}

void CCommPort::disconnect()
{
    // �������� COM �����
    if (m_hCommPort != INVALID_HANDLE_VALUE)
        CloseHandle(m_hCommPort);

    m_hCommPort = INVALID_HANDLE_VALUE;
}

void CCommPort::reconnect()
{
    // ��������������� COM �����
    disconnect();
    connect();
}

void CCommPort::addCommPortDevice(CDeviceModuleConfig& deviceCfg_, CChannelModuleConfig& channelCfg_, long virtChannNum_)
{
    // ���������� ������ ���������� �� COM ���� (���������� ��������� ������ ����� ���������!!! ��� ���������� ������ ������������� � ������������)
    m_lCommPortDevices.push_back(new CCommPortDevice());

    // ����� ��������� �� ������ ���, ����������� ���������� � ������������� ��� ���������
    CCommPortDevice* device = m_lCommPortDevices.back();
    device->setConfig(deviceCfg_, channelCfg_);
    device->setVirtChannelNum(virtChannNum_);
}

void CCommPort::delCommPortDevice(CDeviceModuleConfig& deviceCfg_, CChannelModuleConfig& channelCfg_)
{
    // �������� ���������� ����������� � ���� COM ������ (������� ���������� ������ ��� ����������, ��� ��� ������ �������� ��������� � �� ���� ��������)
    std::list<CCommPortDevice*>::iterator commPortDeviceToRemove = m_lCommPortDevices.end();

    for (std::list<CCommPortDevice*>::iterator currDevice = m_lCommPortDevices.begin();
        currDevice != m_lCommPortDevices.end(); ++currDevice)
    {
        // ��������� ��� ��� ������ ���������� ��� ��������
        if ((*currDevice)->isConfig(deviceCfg_, channelCfg_))
        {
            commPortDeviceToRemove = currDevice;
            break;
        }
    }

    // ��������� ��� �� ����� ���������� � ���������� �����������
    if (commPortDeviceToRemove != m_lCommPortDevices.end())
    {
        delete (*commPortDeviceToRemove);   // ����������� ���������� ������ ��� ����������
        m_lCommPortDevices.erase(commPortDeviceToRemove);   // ������� ������� �����
    }
}

void CCommPort::clearCommPortDevices()
{
    // ����������� ������ ��� ��� ����������
    for (std::list<CCommPortDevice*>::iterator currDevice = m_lCommPortDevices.begin();
        currDevice != m_lCommPortDevices.end(); ++currDevice)
    {
        delete (*currDevice);
    }

    // ����������� ������ ���� ���������
    m_lCommPortDevices.clear();
}

void CCommPort::start()
{
    // ���������� ����� ������ ������ � �������
    QueryPerformanceCounter(&m_liStartTime);
    QueryPerformanceFrequency(&m_liFreq);

    // ����������� � COM �����
    connect();

    // ���������� ���� ������������� ��������� ������
    m_bBreakStreamingThreadFlag = false;
    m_pStreamingThread = AfxBeginThread(streamingFunction, LPVOID(this));
}

void CCommPort::stop()
{
    // ���������� ���� ��������� ������
    m_bBreakStreamingThreadFlag = true;

    // ���� ���� ���� ��������� ������ ����������
    if (m_pStreamingThread != nullptr)
        if (WAIT_OBJECT_0 == WaitForSingleObject(m_pStreamingThread->m_hThread, INFINITE))
        {
            m_pStreamingThread = nullptr;
        }

    // ����������� �� COM �����
    disconnect();
}

CString CCommPort::findCommPortNameByInstanceId(CString instanceId_)
{
    CString retVal = L"";
    std::vector<CString> comPortNames, comPortFriendlyNames, comPortInstanceIds;

    // ����� ������ ���� COM ������
    SF_getCommPortsList(comPortNames, comPortFriendlyNames, comPortInstanceIds);

    // ���������� �� ���� COM ������ � ���� ������������
    for (unsigned int currPort = 0; currPort < comPortInstanceIds.size(); currPort++)
        if (comPortInstanceIds[currPort] == instanceId_)
            retVal = comPortNames[currPort];

    return retVal;
}

long CCommPort::requestData(BYTE* sendData_, long sendDataSz_, BYTE* recData_, long recDataSz_)
{
    // �������� ������� ������ � COM ����
    sendData(sendData_, sendDataSz_);
    // �������� ������ �� COM �����
    waitAnswer();
    // ������ ���������� ������ � ����������� ���������� ��������� ����
    return readData(recData_, recDataSz_);
}

void CCommPort::sendData(BYTE* data_, DWORD dataSz_)
{
    // �������� ������ � COM ����
    DWORD dataWritedSz = 0;
    // ���� ������������� ���������� ��������������� COM ����� � ������ ������������� �������
    bool needReconnect = true;
    
    // �� ������ ������ ������� ������ ������ � �������� ������
    clearReadDataBuff();
    clearSendDataBuff();

    // �������� ������
    if (m_hCommPort != INVALID_HANDLE_VALUE)
    {
        if (WriteFile(m_hCommPort, data_, dataSz_, &dataWritedSz, NULL) != FALSE)
        {
            if (FlushFileBuffers(m_hCommPort) != FALSE)
            {
                if (dataSz_ == dataWritedSz)   // ���� �������� ����������� �������
                    needReconnect = false;
            }
        }
    }

    if (needReconnect)   // ���� ��� �� ����� �� ��� ����������������
        reconnect();
}

long CCommPort::getSzToRead()
{
    long retVal = 0;

    if (m_hCommPort != INVALID_HANDLE_VALUE)
    {
        // ���������� ��������� ������ ��� ������
        if (FlushFileBuffers(m_hCommPort) != FALSE)
        {
            // ������ ������� ��� ������
            COMSTAT comstat;
            if (ClearCommError(m_hCommPort, NULL, &comstat) != FALSE)
                retVal = comstat.cbInQue;
        }
    }

    return retVal;
}

void CCommPort::waitAnswer()
{
    // ������� �������� ���� �� ������ �����, � ����� ���� ���� ������ �������� ������ ������ �� ����� �������������
    LARGE_INTEGER startWaitTime, lastNewWaitTime, currWaitTime, freq;

    double maxAnswerTimeOut = m_cInterfaceModuleCfg.getMaxAnswerTimeOut() / 1000;    // ������������ ����� �������� ������
    double maxNewByteTimeOut = m_cInterfaceModuleCfg.getMaxNewDataDelay() / 1000;   // ������������ ����� ����� �������� ������

    QueryPerformanceCounter(&startWaitTime);
    lastNewWaitTime = startWaitTime;
    QueryPerformanceFrequency(&freq);

    long lastRdSz = 0;   // ������ � ������ ������ ��� ���������� ������ ������
    // ������� �������� ������� ������ �� ������ ������
    while (true)
    {
        // ��������� ������� ��������� ����� ������
        QueryPerformanceCounter(&currWaitTime);

        // ������ ������� ��������� COM �����
        long newRdSz = getSzToRead();
        if (newRdSz != lastRdSz)
        {
            lastRdSz = newRdSz;
            lastNewWaitTime = currWaitTime;
        }

        // �������� �� ��, ��� ��� ������ ���� �� ���� ����
        if (lastNewWaitTime.QuadPart == startWaitTime.QuadPart)
        {
            // ���� ����� ������ ��������� ������������ ����� ��������������
            if (double(currWaitTime.QuadPart - startWaitTime.QuadPart) / double(freq.QuadPart) > maxAnswerTimeOut)
                break;
        }
        else
        {
            // ���� ����� ����� ���������� ���������� ���������� ���� ��������� ������� ����� ��������� ������������� ��������� ������ �������
            if (double(currWaitTime.QuadPart - lastNewWaitTime.QuadPart) / double(freq.QuadPart) > maxNewByteTimeOut)
                break;
        }

        Sleep(1);
    }
}

long CCommPort::readData(BYTE* data_, DWORD dataSz_)
{
    // ������ ������
    DWORD retVal = 0;
    DWORD szToRead = getSzToRead();
    if (szToRead > dataSz_)
        szToRead = dataSz_;

    if (ReadFile(m_hCommPort, data_, szToRead, &retVal, NULL) == FALSE)
        reconnect();

    return retVal;
}

void CCommPort::clearReadDataBuff()
{
    char tmpChr = 0;
    DWORD sz = getSzToRead();

    for (DWORD i = 0; i < sz; i++)
        readData((BYTE*)&tmpChr, 1);
}

void CCommPort::clearSendDataBuff()
{
    FlushFileBuffers(m_hCommPort);
}

double CCommPort::getWorkingTime()
{
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);

    return double(currentTime.QuadPart - m_liStartTime.QuadPart) / m_liFreq.QuadPart;
}

UINT CCommPort::streamingFunction(LPVOID param_)
{
    CCommPort* commPort = (CCommPort*)param_;
    double currTime = 0;           // ������� ����� ������ COM �����
    double lastAnswerTime = 0;     // ����� ���������� ������ �� ����������
    double lastRequestTime = 0;    // ����� ���������� ������� ������
    double streamingPeriod = 0;    // ������ ������ ����������

    static const uint16_t inOutBuffSz = 256;
    BYTE sendBuff[inOutBuffSz] = {0};
    BYTE recBuff[inOutBuffSz] = {0};

    DWORD sendBuffSz = 0, recBuffSz = 0;

    while (true)
    {
        // �������� ������������� ������������
        if (commPort->m_bBreakStreamingThreadFlag)
            break;

        currTime = commPort->getWorkingTime();   // ����� ������� ����� ������ ���������� �� COM �����

        // �������� �� ������ ���� ���������, ���� �� ���������, ������� ���������� ��������
        std::list<CCommPortDevice*>::iterator commPortDevice = commPort->m_lCommPortDevices.end();
        for (std::list<CCommPortDevice*>::iterator currCommPortDevice = commPort->m_lCommPortDevices.begin();
               currCommPortDevice != commPort->m_lCommPortDevices.end(); ++currCommPortDevice)
        {
            if ((*currCommPortDevice)->chekIsNeedNewData(currTime))
            {
                commPortDevice = currCommPortDevice;
                break;
            }
        }

        // ����������� ������ �� ����� ����������
        if (commPortDevice != commPort->m_lCommPortDevices.end())
        {
            sendBuffSz = (*commPortDevice)->getRequestBuff(sendBuff, inOutBuffSz, currTime);
            recBuffSz = commPort->requestData(sendBuff, sendBuffSz, recBuff, inOutBuffSz);

            if (recBuffSz > 0)
                (*commPortDevice)->setAnswerBuff(recBuff, recBuffSz, currTime);
        }

        Sleep(1);
    }

    return 0;
}

unsigned long CCommPort::getCommPortDevicesCount()
{
    long retVal = 0;

    // ����� ������ ���������, ���������� �� ���� COM �����
    retVal = m_lCommPortDevices.size();

    return retVal;
}

long CCommPort::getCommPortDeviceVirtChannNum(unsigned long deviceNum_)
{
    long retVal = -1;

    // ����� ����� ������������ ������, ������������ ������� ���������� ����������� �� COM �����
    if (m_lCommPortDevices.size() >= deviceNum_)
    {
        std::list<CCommPortDevice*>::iterator device = m_lCommPortDevices.begin();
        for (unsigned long i = 0; i < deviceNum_; ++i)
			++device;
        retVal = (*device)->getVirtChannelNum();
    }

    return retVal;
}

unsigned long CCommPort::getCommPortDeviceSampleCount(unsigned long deviceNum_)
{
    long retVal = 0;

    // ����� ���������� ��������, ��������� � ������ �� ������� ����������, ����������� �� COM �����
    if (m_lCommPortDevices.size() >= deviceNum_)
    {
        std::list<CCommPortDevice*>::iterator device = m_lCommPortDevices.begin();
        for (unsigned long i = 0; i < deviceNum_; ++i)
			++device;
        retVal = (*device)->getSamplesCount();

    }

    return retVal;
}

long CCommPort::getCommPortDeviceSample(unsigned long deviceNum_, double& sampleTime_, double& sampleVal_)
{
    long retVal = -1;

    // ����� ������ �� ������� ����������, ����������� �� COM �����
    if (m_lCommPortDevices.size() >= deviceNum_)
    {
        std::list<CCommPortDevice*>::iterator device = m_lCommPortDevices.begin();
        for (unsigned long i = 0; i < deviceNum_; ++i)
			++device;

        retVal = (*device)->getSample(sampleTime_, sampleVal_);
    }

    return retVal;
}

bool CCommPort::getDeviceStatus(unsigned long deviceNum_)
{
    bool retVal = false;

    // ����� ������ ����������, ����������� �� COM �����
    if (m_lCommPortDevices.size() >= deviceNum_)
    {
        std::list<CCommPortDevice*>::iterator device = m_lCommPortDevices.begin();
		for (unsigned long i = 0; i < deviceNum_; ++i)
			++device;

        retVal = (*device)->checkIsLive();
    }

    return retVal;
}