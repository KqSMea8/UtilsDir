// Устройство на COM порте
#include "Modbus\ExternalDevices\Tools\Configurations\Device configuration\deviceModuleConfig.h"
#include "Modbus\ExternalDevices\Tools\Configurations\Channel configuration\channelModuleConfig.h"
#include <list>

typedef std::pair<double, double> Sample;   // Один отсчет данных (время, данные)
typedef std::list<Sample> Samples;          // Отсчеты данных

class CCommPortDevice
{
private:
    // Время в секундах, если в течении этого времени устройство не отвечает, оно умерло (отображаем серым цветом)
    const double DeviсeIsDeadTimeOut = 5;
    // Максимальное количество хранимых отсчетов (при переполнении удалются более старые отсчеты, таким образом хранятся самые актуальные данные)
    const unsigned long MaxDeviceSamplesCount = 100;

    CDeviceModuleConfig  m_cDeviceModuleCfg;    // Конфигурация измерительного устройства
    CChannelModuleConfig m_cChannelModuleCfg;   // Конфигурация измерительного канала

    CRITICAL_SECTION m_csSampleAccess;   // Критическая секция доступа к отсчетам
    Samples m_lSamples;                  // Отсчеты данных

    double m_tLastRequestTime = 0;    // Время отправки последнего запроса данных от старта работы устройства
    double m_tLastAnswerTime  = 0;    // Время получения последнего ответа от старта работы устройства

    long m_lVirtChannelNum = 0;   // Номер виртуального канала, соответствующего данному устройству

    uint16_t crc16(uint8_t byte_, uint16_t lastCRC_);            // Рассчет CRC16
    void putSample(double& sampleTime_, double& sampleValue_);   // Добавление отсчета от устройства
public:
    CCommPortDevice();
    ~CCommPortDevice();

    // Проверка работаспособности устройства (устройство отвечает, не позднее DeviсeIsDeadTimeOut)
    bool checkIsLive();
    // Проверка устройства на то, что необходимо запросить новую порцию данных
    bool chekIsNeedNewData(double currTime_);

    // Устанавливаем конфигурационные параметры измерительного канала
    void setConfig(CDeviceModuleConfig& deviceConfig_, CChannelModuleConfig& channelConfig_);
    // Проверка устройства на то что это устройство с указанными параметрами
    bool isConfig(CDeviceModuleConfig& deviceConfig_, CChannelModuleConfig& channelConfig_);

    // Функция взятия пакета команды на запрос данных от устройства
    DWORD getRequestBuff(BYTE* data_, DWORD dataSz_, double requestTime_);

    // Функция распознования ответного пакета от устройства на команду запроса данных
    void setAnswerBuff(BYTE* data_, DWORD dataSz_, double answerTime_);

    // Взятие количества необработанных отсчетов устройства
    long getSamplesCount();
    // Взятие необработанного отсчета устройства
    long getSample(double& sampleTime_, double& sampleValue_);

    // Установка номера виртуального канала, соответствующего данному устройству
    void setVirtChannelNum(long virtChannNum_);
    // Взятие номера виртуального канала, соответствующего данному устройству
    long getVirtChannelNum();
};