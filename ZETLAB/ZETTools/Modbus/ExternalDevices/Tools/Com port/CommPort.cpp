#include "Modbus\ExternalDevices\Tools\Com port\commPort.h"
#include "Modbus\ExternalDevices\Tools\Secondary functions\secondaryFunctions.h"
#include <vector>

CCommPort::CCommPort()
{
}

CCommPort::~CCommPort()
{

    // Очищаем список подключенных устройств по COM порту
    clearCommPortDevices();
}

void CCommPort::setConfig(CInterfaceModuleConfig& config_)
{
    // Запоминаем конфигурационные настройки интерфейсного модуля
    m_cInterfaceModuleCfg = config_;
}

void CCommPort::connect()
{
    // Открытие COM порта
    CString portName = findCommPortNameByInstanceId(m_cInterfaceModuleCfg.getCommPortInstanceId());

    m_hCommPort = INVALID_HANDLE_VALUE;
	if (!portName.IsEmpty())   // Если номер COM порта удалось найти
    {
        m_hCommPort = ::CreateFile(portName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, NULL, NULL);

        // Проверка на то, что файл открылся
        if (m_hCommPort != INVALID_HANDLE_VALUE)
        {
            // Установка настроек COM порта (настройки параметро передачи и таймаутов)
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
    // Закрытие COM порта
    if (m_hCommPort != INVALID_HANDLE_VALUE)
        CloseHandle(m_hCommPort);

    m_hCommPort = INVALID_HANDLE_VALUE;
}

void CCommPort::reconnect()
{
    // Переподключение COM порта
    disconnect();
    connect();
}

void CCommPort::addCommPortDevice(CDeviceModuleConfig& deviceCfg_, CChannelModuleConfig& channelCfg_, long virtChannNum_)
{
    // Добавление нового устройства на COM порт (устройства добавляем только через указатель!!! для правильной работы конструкторов и деструкторов)
    m_lCommPortDevices.push_back(new CCommPortDevice());

    // Берем указатель на только что, добавленное устройство и устанавливаем его параметры
    CCommPortDevice* device = m_lCommPortDevices.back();
    device->setConfig(deviceCfg_, channelCfg_);
    device->setVirtChannelNum(virtChannNum_);
}

void CCommPort::delCommPortDevice(CDeviceModuleConfig& deviceCfg_, CChannelModuleConfig& channelCfg_)
{
    // Удаление устройства работающего с этим COM портом (удаляем выделенную память под устройство, так как внутри хранятся указатели а не сами значения)
    std::list<CCommPortDevice*>::iterator commPortDeviceToRemove = m_lCommPortDevices.end();

    for (std::list<CCommPortDevice*>::iterator currDevice = m_lCommPortDevices.begin();
        currDevice != m_lCommPortDevices.end(); ++currDevice)
    {
        // Проверяем что это нужное устройство для удаления
        if ((*currDevice)->isConfig(deviceCfg_, channelCfg_))
        {
            commPortDeviceToRemove = currDevice;
            break;
        }
    }

    // Проверяем что мы нашли устройство с выбранными параметрами
    if (commPortDeviceToRemove != m_lCommPortDevices.end())
    {
        delete (*commPortDeviceToRemove);   // Освобождаем выделенную память под устройство
        m_lCommPortDevices.erase(commPortDeviceToRemove);   // Удаляем элемент листа
    }
}

void CCommPort::clearCommPortDevices()
{
    // Освобождаем память под все устройства
    for (std::list<CCommPortDevice*>::iterator currDevice = m_lCommPortDevices.begin();
        currDevice != m_lCommPortDevices.end(); ++currDevice)
    {
        delete (*currDevice);
    }

    // Освобождаем список всех устройств
    m_lCommPortDevices.clear();
}

void CCommPort::start()
{
    // Запоминаем время старта работы и частоту
    QueryPerformanceCounter(&m_liStartTime);
    QueryPerformanceFrequency(&m_liFreq);

    // Подключаемя к COM порту
    connect();

    // Сбрасываем флаг необходимости остановки потока
    m_bBreakStreamingThreadFlag = false;
    m_pStreamingThread = AfxBeginThread(streamingFunction, LPVOID(this));
}

void CCommPort::stop()
{
    // Выставляем флаг остановки потока
    m_bBreakStreamingThreadFlag = true;

    // Ждем пока флаг остановки потока отработает
    if (m_pStreamingThread != nullptr)
        if (WAIT_OBJECT_0 == WaitForSingleObject(m_pStreamingThread->m_hThread, INFINITE))
        {
            m_pStreamingThread = nullptr;
        }

    // Отключаемся от COM порта
    disconnect();
}

CString CCommPort::findCommPortNameByInstanceId(CString instanceId_)
{
    CString retVal = L"";
    std::vector<CString> comPortNames, comPortFriendlyNames, comPortInstanceIds;

    // Берем список всех COM портов
    SF_getCommPortsList(comPortNames, comPortFriendlyNames, comPortInstanceIds);

    // Проходимся по всем COM портам и ищем соответствия
    for (unsigned int currPort = 0; currPort < comPortInstanceIds.size(); currPort++)
        if (comPortInstanceIds[currPort] == instanceId_)
            retVal = comPortNames[currPort];

    return retVal;
}

long CCommPort::requestData(BYTE* sendData_, long sendDataSz_, BYTE* recData_, long recDataSz_)
{
    // Отправка запроса данных в COM порт
    sendData(sendData_, sendDataSz_);
    // Ожидание ответа по COM порту
    waitAnswer();
    // Чтение результата ответа и возвращение количества считанных байт
    return readData(recData_, recDataSz_);
}

void CCommPort::sendData(BYTE* data_, DWORD dataSz_)
{
    // Отправка пакета в COM порт
    DWORD dataWritedSz = 0;
    // Флаг необходимости произвести переподключение COM порта в случае возникновения проблем
    bool needReconnect = true;
    
    // На всякий случай очищаем буферы приема и передачи данных
    clearReadDataBuff();
    clearSendDataBuff();

    // Передаем данные
    if (m_hCommPort != INVALID_HANDLE_VALUE)
    {
        if (WriteFile(m_hCommPort, data_, dataSz_, &dataWritedSz, NULL) != FALSE)
        {
            if (FlushFileBuffers(m_hCommPort) != FALSE)
            {
                if (dataSz_ == dataWritedSz)   // Если отправка произведена успешно
                    needReconnect = false;
            }
        }
    }

    if (needReconnect)   // Если что то пошло не так переподключаемся
        reconnect();
}

long CCommPort::getSzToRead()
{
    long retVal = 0;

    if (m_hCommPort != INVALID_HANDLE_VALUE)
    {
        // Обновление сотсояния буфера для чтения
        if (FlushFileBuffers(m_hCommPort) != FALSE)
        {
            // Взятие размера для чтения
            COMSTAT comstat;
            if (ClearCommError(m_hCommPort, NULL, &comstat) != FALSE)
                retVal = comstat.cbInQue;
        }
    }

    return retVal;
}

void CCommPort::waitAnswer()
{
    // Ожидаем принятия хотя бы одного байта, а далее ждем пока размер принятых данных больше не будет увеличиваться
    LARGE_INTEGER startWaitTime, lastNewWaitTime, currWaitTime, freq;

    double maxAnswerTimeOut = m_cInterfaceModuleCfg.getMaxAnswerTimeOut() / 1000;    // Максимальное время ожидание ответа
    double maxNewByteTimeOut = m_cInterfaceModuleCfg.getMaxNewDataDelay() / 1000;   // Максимальное время между порциями данных

    QueryPerformanceCounter(&startWaitTime);
    lastNewWaitTime = startWaitTime;
    QueryPerformanceFrequency(&freq);

    long lastRdSz = 0;   // Размер в буфере данных при предыдущем чтении данных
    // Процесс ожидания полного ответа на запрос данных
    while (true)
    {
        // Проверяем текущее состояние према данных
        QueryPerformanceCounter(&currWaitTime);

        // Читаем текущее состояние COM порта
        long newRdSz = getSzToRead();
        if (newRdSz != lastRdSz)
        {
            lastRdSz = newRdSz;
            lastNewWaitTime = currWaitTime;
        }

        // Проверка на то, что был принят хотя бы один байт
        if (lastNewWaitTime.QuadPart == startWaitTime.QuadPart)
        {
            // Если время ответа превысило максимальное время ожиданияответа
            if (double(currWaitTime.QuadPart - startWaitTime.QuadPart) / double(freq.QuadPart) > maxAnswerTimeOut)
                break;
        }
        else
        {
            // Если время посде последнего увеличения количества байт превысило интервл между символами останавливаем процедуру чтения даннных
            if (double(currWaitTime.QuadPart - lastNewWaitTime.QuadPart) / double(freq.QuadPart) > maxNewByteTimeOut)
                break;
        }

        Sleep(1);
    }
}

long CCommPort::readData(BYTE* data_, DWORD dataSz_)
{
    // Читаем данные
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
    double currTime = 0;           // Текущее время работы COM порта
    double lastAnswerTime = 0;     // Время последнего ответа от устройтсва
    double lastRequestTime = 0;    // Время последнего запроса данных
    double streamingPeriod = 0;    // Пертод опроса устройства

    static const uint16_t inOutBuffSz = 256;
    BYTE sendBuff[inOutBuffSz] = {0};
    BYTE recBuff[inOutBuffSz] = {0};

    DWORD sendBuffSz = 0, recBuffSz = 0;

    while (true)
    {
        // Проверка необходимости остановиться
        if (commPort->m_bBreakStreamingThreadFlag)
            break;

        currTime = commPort->getWorkingTime();   // Берем текущее время работы устройства по COM порту

        // Проходим по списку всех устройств, ищем то устройтво, которое необходимо опросить
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

        // Запрашиваем данные по этому устройству
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

    // Берем список устройств, работающих на этом COM порту
    retVal = m_lCommPortDevices.size();

    return retVal;
}

long CCommPort::getCommPortDeviceVirtChannNum(unsigned long deviceNum_)
{
    long retVal = -1;

    // Берем номер виртуального канала, присвоенного данному устройству работающему по COM порту
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

    // Берем количество отсчетов, доступных к чтению по данному устройству, работающему по COM порту
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

    // Берем отсчет по данному устройству, работающему по COM порту
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

    // Берем статус устройства, работающего по COM порту
    if (m_lCommPortDevices.size() >= deviceNum_)
    {
        std::list<CCommPortDevice*>::iterator device = m_lCommPortDevices.begin();
		for (unsigned long i = 0; i < deviceNum_; ++i)
			++device;

        retVal = (*device)->checkIsLive();
    }

    return retVal;
}