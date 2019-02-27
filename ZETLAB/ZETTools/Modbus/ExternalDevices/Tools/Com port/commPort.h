// Класс, выполняющий периодический опрос устройств, расположенных на COM порту

#pragma once
#include "afx.h"
#include "afxwin.h"
#include "Modbus\ExternalDevices\Tools\Configurations\Interface configuration\interfaceModuleConfig.h"
#include "Modbus\ExternalDevices\Tools\Com port\Com port device\commPortDevice.h"
#include <list>

class CCommPort
{
private:
    CInterfaceModuleConfig      m_cInterfaceModuleCfg;   // конфигурационные настройки интерфейсного модуля
    std::list<CCommPortDevice*> m_lCommPortDevices;      // список устройств подключенных к данному COM порту

    LARGE_INTEGER m_liStartTime;   // Время старта работы устройства в тиках
    LARGE_INTEGER m_liFreq;        // Частота

    HANDLE m_hCommPort = INVALID_HANDLE_VALUE;   // Хендл COM порта

    CWinThread* m_pStreamingThread = nullptr;          // Поток опроса всех устройств, находящихся на этом COM порту
    bool        m_bBreakStreamingThreadFlag = false;   // Флаг для остоновки потока

private:
    // Функция поиска имени COM порта по InstanceID устройства
    CString findCommPortNameByInstanceId(CString instanceId_);

    // Функция программного подключения к COM порту через ОС
    void connect();

    // Функция программного отключения от COM порта через ОС
    void disconnect();

    // Функция программного переподключения к COM порту через ОС
    void reconnect();

    // Функция взятия размра для чтения данных
    long getSzToRead();

    // Функция очистки буфера отправки данных
    void clearSendDataBuff();

    // Функция очистки буфера чтения данных
    void clearReadDataBuff();

    // Функция отправки данных
    void sendData(BYTE* data_, DWORD dataSz_);

    // Функция взятия времени работы по COM порту (старт времени начинается от вызова команды start())
    double getWorkingTime();

    // Функция ожидания ответа от устройства
    void waitAnswer();

    // Функция чтения данных
    long readData(BYTE* data_, DWORD dataSz_);

    // Функция запроса данных
    // sendData_, sendDataSz_ - буфер команды запроса данных и длина команды запроса данных
    // recData_, recDataSz_ - буфер команды ответа и длина команды ответа
    // возвращаемое значение - количество полученных данных в ответ
    long requestData(BYTE* sendData_, long sendDataSz_, BYTE* recData_, long recDataSz_);

public:
    CCommPort();
    ~CCommPort();

    // Функция установки конфигурации интерфейсного модуля
    void setConfig(CInterfaceModuleConfig& interfaceConfig_);

    // Функция добавления нового устройства, работающего на COM порту
    void addCommPortDevice(CDeviceModuleConfig& deviceCfg_, CChannelModuleConfig& channelCfg_, long virtChannelNum_ = 0);

    // Функция удаления устройства, работающего по COM порту
    void delCommPortDevice(CDeviceModuleConfig& deviceCfg_, CChannelModuleConfig& channelCfg_);

    // Функция очистки списка устройств, работающих на данном COM порту
    void clearCommPortDevices();

    // Функция запуска процесса приема передачи данных от устройств на данном COM порту
    void start();

    // Функция остановки процесса приема передачи данных от устройств на данном COM порту
    void stop();

    // Функция взятия количества устройств, работающих на данном COM порту
    unsigned long getCommPortDevicesCount();

    // Функция взятия номера виртуального канала, работающего по данному COM порту
    long getCommPortDeviceVirtChannNum(unsigned long deviceNum_);

    // Функция взятие количества накопленных отсчетов устройства на COM порту
    unsigned long getCommPortDeviceSampleCount(unsigned long deviceNum_);

    // Функция взятия отсчета от устройства на COM порту
    long getCommPortDeviceSample(unsigned long deviceNum_, double& sampleTime_, double& sampleVal_);

    // Функция взятия статуса устройства (работает ли ещё)
    bool getDeviceStatus(unsigned long deviceNum_);

protected:
    // Потоковая функция опроса устройств
    static UINT streamingFunction(LPVOID param_);
};