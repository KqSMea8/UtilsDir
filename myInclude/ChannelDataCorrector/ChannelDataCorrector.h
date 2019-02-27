#pragma once
#include <list>
#include <memory>
#include "ChannelsHelpHeader.h"
#include "DescriptorFileHelper.h"

#ifdef USE_STREAMWRITER
#include "ChannelDataCorrector/TrendWriter/StreamTrendWriter.h"
#endif

//*****************************************************************************
// тип коррекции данных
enum CorrectionMode
{
    eFillFullInterval = 0,	// заполнение данными всего интервала
    eCorrectLevelCurData,	// подкорректировать уровень текущих данных
    eSaveIntervalLevels		// сохранить уровни на интервалах
};

//*****************************************************************************
// класс корректировки данных по определенному каналу в заданный промежуток времени
class CChannelDataCorrector
{
public:
    CChannelDataCorrector(CString channelName, const CTime& begin, const CTime& end);
    ~CChannelDataCorrector();

    // получить данные из канала
    void getCorrectedSourceChannelData(const CString& sourceChannelName,
                                       const CString& correctionChannelName,
                                       const CTime& timeStart,
                                       const CTime& timeEnd,
                                       CTime correctionIntervalStart,
                                       CTime correctionIntervalEnd,
                                       double correctionStartVal,
                                       double correctionEndVal,
                                       double correctionRangeVal,
                                       CorrectionMode correctionMode,
                                       std::vector<float>& data);

    // получить данные по канала
    void getSourceChannelData(const CString& channelName,
                              const CTime& begin, const CTime& end,
                              std::vector<float>& data);


    void getRandomCorrectData(CTime timeStart, CTime timeEnd, double startIntervalVal,
                              double endIntervalVal, double rangeVal,
                              std::vector<float>& data);

#ifdef USE_STREAMWRITER
    /*
    * запись данных
    * @param timeStart - начальное значение интервала преобразования данных
    * @param timeEnd   - конечное  значение интервала преобразования данных
    * @param startIntervalVal - начальное значение точки в заданном интервале
    * @param endIntervalVal   - конечное  значение точки в заданном интервале
    * @param rangeVal - разброс значений в выбранном интервале
    */
    bool correctChannelData(CTime timeStart, CTime timeEnd, double startIntervalVal,
                            double endIntervalVal, double rangeVal);


    bool correctChannelDataNew(CTime timeStart, CTime timeEnd, float *data, long lSize);

    // начало и конец записи
    void startWriting();
    void stopWriting();
#endif

    // запонение списка каналов
    static void FillChannelList(CString sDirectory,
                                std::list<std::pair<CString, CString>>& channelList);

    ChannelInfo* getChannelInfo();
private:
    // получение данных 
    void calcChannelInfo(ChannelInfo* chanInfo, CString channelName,
                         const CTime& begin, const CTime& end);

    // получение информации о канале
    bool getChannelInfo(const CString& xmlFileName);
    // получаем информацию о файлах
    ChannelFromFile getFilesInfo(const CString& channelName, const CString& xmlFileName);

    // получаем информацию о файлах которые необходимо переписать
    ChannelsFromFileByYear GetSourceFilesInfoByInterval(const CString& channelName, CTime Begin, CTime End);

    void GetSourceChannelDataByInterval(const CString& channelName,
                                        const CTime& tStart, const CTime& tEnd,
                                        long uiLength, float* pData, double freq);
private:
    // получение директорий
    CString getSourceFilesDir();
    CString getCompressedDir();

    void FillFullInterval(std::vector<float>& data,
                          std::vector<float>& dataFromSource,
                          double correctionStartVal,
                          double correctionEndVal,
                          double correctionRangeVal);
    void CorrectCurentLevel(std::vector<float>& data,
                            std::vector<float>& dataFromSource,
                            double correctionStartVal,
                            double correctionEndVal,
                            double correctionRangeVal);
    void SaveCurentLevel(const CString& correctionChannelName,
                         CTime correctionIntervalStart,
                         CTime correctionIntervalEnd,
                         double correctionRangeVal,
                         std::vector<float>& data,
                         std::vector<float>& dataFromSource);
private:
    CString m_channelName;
    TimeInterval m_interval;

#ifdef USE_STREAMWRITER
    CStreamTrendWriter m_streamTrendWriter;
#endif
    CDescriptorFileHelper m_descriptorFileHelper;

    std::shared_ptr<ChannelInfo> m_channelInfo;
};
