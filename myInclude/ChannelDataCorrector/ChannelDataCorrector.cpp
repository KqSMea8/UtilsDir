#include "stdafx.h"

#include <pugixml-1.4/src/pugixml.hpp>
#include <vector>
#include <filesystem>
#include <functional>
#include <numeric>

#include "ChannelDataCorrector.h"
#include "ZetDirs.h"

#pragma comment(lib, "Intel\\ipp\\ia32\\zetipp.lib")

using namespace pugi;


void deleteFiles(ChannelsFromFileByYear filesByDate)
{
    for (auto &yearFiles : filesByDate)
    {
        for (auto &monthFiles : yearFiles.second.ChannelsFromFileByMonth)
        {
            for (auto &dayFiles : monthFiles.second.ChannelsFromFileByDay)
            {
                for (auto &hourFiles : dayFiles.second.ChannelsFromFileByHour)
                {
                    if (std::experimental::filesystem::is_regular_file(hourFiles.second.sDataFileName.GetString()))
                    {
                        std::experimental::filesystem::remove(hourFiles.second.sDataFileName.GetString());
                    }
                }
            }
        }
    }
}

//*****************************************************************************
CChannelDataCorrector::CChannelDataCorrector(CString channelName, const CTime& begin, const CTime& end)
    : m_channelName(channelName)
    , m_interval(begin, end)
    , m_channelInfo(new ChannelInfo())
{
    calcChannelInfo(m_channelInfo.get(), channelName, begin, end);

#ifdef USE_STREAMWRITER
    m_streamTrendWriter.SetChannelInfo(m_channelInfo);

    m_streamTrendWriter.SetSignalDirectory(getSourceFilesDir());
    m_streamTrendWriter.SetCompressedDirectory(getCompressedDir());
#endif
}

CChannelDataCorrector::~CChannelDataCorrector()
{
#ifdef USE_STREAMWRITER
    stopWriting();
#endif
}

void fillData(double correctionStartVal, double correctionEndVal,
              double correctionRangeVal,
              float* resultData, int resDataSize,
              float* sourseData, int srcDataSize)
{
    if (resDataSize == 0)
        return;

    // на единицу индекса корректируем на это число
    double correctOnIndex = (correctionEndVal - correctionStartVal) / (double)resDataSize;
    double currentCorrectionVal = correctionStartVal;

    int curentCorrection = 0;
    float curResultVal = correctionStartVal;
    /*for (int index = 0; index < resDataSize; ++index)
    {
    int currentSourceData = index % srcDataSize;
    // если число вошло в разрешенный интервал
    if (abs(sourseData[currentSourceData] - curResultVal) < correctionRangeVal)
    {
    resultData[index] = sourseData[currentSourceData];
    curentCorrection = 0;
    }
    else if (abs(sourseData[currentSourceData] + curentCorrection - curResultVal) < correctionRangeVal)
    {
    resultData[index] = sourseData[currentSourceData] + curentCorrection;
    curentCorrection = curResultVal - resultData[index];
    }
    else
    {
    curentCorrection = curResultVal - resultData[index];
    }
    curResultVal += currentCorrectionVal;

    }*/

    for (int index = 0; index < resDataSize; ++index)
    {
        int currentSourceData = index % srcDataSize;
        // если число вошло в разрешенный интервал
        if (abs(sourseData[currentSourceData] - curResultVal) < correctionRangeVal)
        {
            resultData[index] = sourseData[currentSourceData];
            curentCorrection = 0;
        }
        else if (abs(sourseData[currentSourceData] + curentCorrection - curResultVal) <= correctionRangeVal)
        {
            resultData[index] = sourseData[currentSourceData] + curentCorrection;
            curentCorrection = curResultVal - sourseData[currentSourceData];
        }
        else
        {
            resultData[index] = curResultVal - (correctionRangeVal / 2.) / 10. + (correctionRangeVal < 0.001 ?
                (rand() % 1 == 0 ? correctionRangeVal : -correctionRangeVal) :
                                                                                  (rand() % int(correctionRangeVal * 100) / 100)) / 10.;
            curentCorrection = curResultVal - sourseData[currentSourceData];
        }
        curResultVal += correctOnIndex;
    }
}

typedef std::function<bool(const float& value)> Callback;

float deleteBadValues(std::vector<float>& datavect, Callback&& callback)
{
    bool bNotDataExist = false;
    int firstNoDataIndex = 0;
    int lastDataExistedIndex = 0;
    int dataSize = datavect.size();

    float sum = 0.f;
    for (int i = 0; i < dataSize; ++i)
    {
        if (callback(datavect[i]))
        {
            if (!bNotDataExist)
            {
                firstNoDataIndex = i;
                bNotDataExist = true;
            }
        }
        else
        {
            // если есть пропуски данных
            if (bNotDataExist)
            {
                // сдвигаем данные и заполняем конец нулями
                memcpy_s(&datavect[firstNoDataIndex], (dataSize - i) * sizeof(float),
                         &datavect[i], (dataSize - i) * sizeof(float));

                memset(&datavect[firstNoDataIndex + dataSize - i], 0, (i - firstNoDataIndex) * sizeof(float));
                bNotDataExist = false;
                
                // сдвигаем наш индекс чтобы потому что данные сдвинулись тоже
                i = firstNoDataIndex;
            }

            lastDataExistedIndex = i;
            sum += datavect[i];
        }
    }

    datavect.resize(lastDataExistedIndex);
    return sum / (lastDataExistedIndex == 0 ? 1 : lastDataExistedIndex);
}

float calculateAverageVal(std::vector<float>& datavect, Callback&& callback)
{
    float summ(0);
    UINT goodDataCount(0);
    for (int i = 0, dataSize = (int)datavect.size(); i < dataSize; ++i)
    {
        if (callback(datavect[i]))
        {
            summ += datavect[i];
            ++goodDataCount;
        }
    }

    return summ / (goodDataCount == 0 ? 1 : goodDataCount);
}

float replaceValues(std::vector<float>& datavect, float newValue, Callback&& callback)
{
    float summ(0);
    UINT goodDataCount(0);
    for (int i = 0, dataSize = (int)datavect.size(); i < dataSize; ++i)
    {
        if (callback(datavect[i]))
        {
            datavect[i] =
                summ += datavect[i];
            ++goodDataCount;
        }
    }

    return summ / (goodDataCount == 0 ? 1 : goodDataCount);
}

void CChannelDataCorrector::getCorrectedSourceChannelData(const CString& sourceChannelName,
                                                          const CString& correctionChannelName,
                                                          const CTime& timeStart,
                                                          const CTime& timeEnd,
                                                          CTime correctionIntervalStart,
                                                          CTime correctionIntervalEnd,
                                                          double correctionStartVal,
                                                          double correctionEndVal,
                                                          double correctionRangeVal,
                                                          CorrectionMode correctionMode,
                                                          std::vector<float>& data)
{
    std::vector<float> dataFromSource;
    getSourceChannelData(sourceChannelName, timeStart, timeEnd, dataFromSource);

    CTimeSpan dt = correctionIntervalEnd - correctionIntervalStart;
    data.resize(dt.GetTotalSeconds() * m_channelInfo->GetFrequency());

    try
    {
        switch (correctionMode)
        {
            case eFillFullInterval:
                FillFullInterval(data, dataFromSource,
                                 correctionStartVal, correctionEndVal,
                                 correctionRangeVal);

                break;
            case eCorrectLevelCurData:
                CorrectCurentLevel(data, dataFromSource,
                                   correctionStartVal, correctionEndVal,
                                   correctionRangeVal);
                break;
            case eSaveIntervalLevels:
                SaveCurentLevel(correctionChannelName,
                                correctionIntervalStart,
                                correctionIntervalEnd,
                                correctionRangeVal,
                                data, dataFromSource);
                break;
            default:
                assert(false);
                break;
        }
    }
    catch (const CString& str)
    {
        throw CString(sourceChannelName + L": " + str);
    }
}

void CChannelDataCorrector::getSourceChannelData(const CString& channelName,
                                                 const CTime& timeStart, const CTime& timeEnd,
                                                 std::vector<float>& data)
{
    // получаем инфу по каналу
    ChannelInfo chanInfo;
    calcChannelInfo(&chanInfo, channelName, timeStart, timeEnd);

    // вычисляем сколько надо данных
    CTimeSpan tsDelta = timeEnd - timeStart;
    double freq = m_channelInfo->GetFrequency();

    int dataSize = tsDelta.GetTotalSeconds() * freq;

    if (tsDelta.GetTotalSeconds() <= 0)
        throw CString(L"Неверный временной интервал для загрузки исходных данных " + channelName);

    data.resize(dataSize, 0.f);

    GetSourceChannelDataByInterval(channelName, timeStart, timeEnd,
                                   dataSize, data.data(), freq);

    // заменяем наны на нули
    for (int i = 0; i < dataSize; ++i)
    {
        if (_finite(data[i]) == 0)
        {
            data[i] = 0;
        }
    }
}

void CChannelDataCorrector::getRandomCorrectData(CTime timeStart, CTime timeEnd,
                                                 double startIntervalVal, double endIntervalVal, double rangeVal,
                                                 std::vector<float>& data)
{
    float freq = (float)m_channelInfo->GetFrequency();

    CTimeSpan tsDelta = timeEnd - timeStart;
    long long llDelta = tsDelta.GetTotalSeconds();

    int pointsCount = int(llDelta * freq);
    data.resize(pointsCount);
    double curVal = startIntervalVal;		// текущее значение в точке
    double deltaBetweenVals = (endIntervalVal - startIntervalVal) / pointsCount;	// интервал между точками
    for (int index = 0; index < pointsCount; ++index, curVal += deltaBetweenVals)
    {
        float range = rangeVal == 0 ? 0 : float(rand() % int(rangeVal * 100) / 100.f);
        data[index] = float(curVal + range - rangeVal);
    }
}

#ifdef USE_STREAMWRITER
bool CChannelDataCorrector::correctChannelDataNew(CTime timeStart, CTime timeEnd,
                                                  float *data, long lSize)
{
    float freq = (float)m_channelInfo->GetFrequency();

    return wecSuccessful == m_streamTrendWriter.WriteData(data, lSize, freq,
                                                          double(CTimeSpan(timeStart.GetTime()).GetTotalSeconds()),
                                                          double(CTimeSpan(timeEnd.GetTime()).GetTotalSeconds()));
}

bool CChannelDataCorrector::correctChannelData(CTime timeStart, CTime timeEnd, double startIntervalVal,
                                               double endIntervalVal, double rangeVal)
{
    float freq = (float)m_channelInfo->GetFrequency();

    CTimeSpan tsDelta = timeEnd - timeStart;
    long long llDelta = tsDelta.GetTotalSeconds();

    int pointsCount = int(llDelta * freq);
    std::vector<float> channelData(pointsCount);
    double curVal = startIntervalVal;		// текущее значение в точке
    double deltaBetweenVals = (endIntervalVal - startIntervalVal) / pointsCount;	// интервал между точками
    for (int index = 0; index < pointsCount; ++index, curVal += deltaBetweenVals)
    {
        float range = rangeVal == 0 ? 0 : float(rand() % int(rangeVal * 100) / 100.f);
        channelData[index] = float(curVal + range - rangeVal);
    }

    return wecSuccessful == m_streamTrendWriter.WriteData(channelData.data(), channelData.size(), freq,
                                                          double(CTimeSpan(timeStart.GetTime()).GetTotalSeconds()),
                                                          double(CTimeSpan(timeEnd.GetTime()).GetTotalSeconds()));
}

//*****************************************************************************
void CChannelDataCorrector::startWriting()
{
    m_streamTrendWriter.SetDescriptorFileHelper(&m_descriptorFileHelper);
    m_streamTrendWriter.StartWriter(m_interval.Begin);
}

void CChannelDataCorrector::stopWriting()
{
    m_streamTrendWriter.StopWriter();
}
#endif

//*****************************************************************************
void CChannelDataCorrector::FillChannelList(CString sDirectory,
                                            std::list<std::pair<CString, CString>>& channelList)
{
    WIN32_FIND_DATA win32_find_data;
    WCHAR wcDirectory[MAX_PATH];
    wcscpy_s(wcDirectory, MAX_PATH, sDirectory);
    wcscat_s(wcDirectory, MAX_PATH, L"*.*");

    HANDLE hFind = FindFirstFile(wcDirectory, &win32_find_data);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        bool bEndOfFind = false;
        do
        {
            CString sFileName(CString(win32_find_data.cFileName));
            if (!(win32_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                if (sFileName.Find(L"sig0000.xml") != -1)
                {
                    sFileName = sDirectory + sFileName;
                    xml_document hFile;
                    if (hFile.load_file(sFileName, parse_default, encoding_utf8))
                    {
                        xml_node hRoot = hFile.child(L"CommonSignalDescriptor");
                        if (hRoot != NULL)
                        {
                            xml_node hSignal = hRoot.child(L"Signal");
                            while (hSignal != NULL)
                            {
                                if (hSignal.attribute(L"name") != NULL)
                                {
                                    std::pair<CString, CString> channel;
                                    channel.first = hSignal.attribute(L"name").value();
                                    if (!channel.first.IsEmpty())
                                    {
                                        if (hSignal.attribute(L"conversion") != NULL)
                                        {
                                            channel.second = hSignal.attribute(L"conversion").value();
                                            if (!channel.second.IsEmpty())
                                            {
                                                if (std::find(channelList.begin(), channelList.end(), channel) == channelList.end()/* && channelList.size() < 1600 && channel.first == L"NPS_10-1X"*/)
                                                    channelList.push_back(channel);
                                            }
                                        }
                                    }
                                }
                                hSignal = hSignal.next_sibling();
                            }
                            bEndOfFind = true;
                        }
                    }
                }
            }
            else
                if (sFileName != L"." && sFileName != L"..")
                {
                    FillChannelList(sDirectory + sFileName + L"\\", channelList);
                }
        } while (FindNextFile(hFind, &win32_find_data) && !bEndOfFind);
        FindClose(hFind);
    }
}

//*****************************************************************************
void CChannelDataCorrector::calcChannelInfo(ChannelInfo* chanInfo, CString channelName,
                                            const CTime& begin, const CTime& end)
{
    CTime tBegin = CTime(begin.GetYear(), begin.GetMonth(), begin.GetDay(), begin.GetHour(), 0, 0);
    int offset = end.GetMinute() * 60 + end.GetSecond();
    CTime tEnd = offset > 0 ? (end - CTimeSpan(offset - 3600)) : end;

    // флаг что удалось вытащить информацию о канале
    bool bChannelInfoExist(false);

    CTime tTimeNext = tBegin;
    while (tTimeNext < tEnd)
    {
        CString sFileName(L"");
        ChannelFromFile tempChannel;
        sFileName.Format(L"%s%d\\%02d\\%02d\\%02d\\sig0000.xml", getSourceFilesDir().GetString(), tTimeNext.GetYear(), tTimeNext.GetMonth(), tTimeNext.GetDay(), tTimeNext.GetHour());

        // пытаемся вытащить информацию о канале
        if (getChannelInfo(sFileName))
            return;

        tTimeNext += 3600;
    }

    // если не нашли информацию о канале вытаскиваем ее из файла с информацией
    if (!bChannelInfoExist)
    {
        CString reserveFileInfo = get_service<ZetDirsService>().getCurrentDir() + L"sig0000.xml";
        if (!getChannelInfo(reserveFileInfo))
        {
            CString errorText;
            errorText.Format(L"Не удалось вытащить информацию о канале, нету исходных файлов и не подошел резервный файл, имя канала: %s\n\
Резевный файл(нужен sig0000.xml с названием канала): %s", m_channelName.GetString(), reserveFileInfo.GetString());

            throw errorText;
        }
    }
}

//*****************************************************************************
ChannelsFromFileByYear CChannelDataCorrector::GetSourceFilesInfoByInterval(const CString& channelName, CTime Begin, CTime End)
{
    // перечень всех файлов которые должны быть 
    ChannelsFromFileByYear filesByDate;

    CTime tBegin = CTime(Begin.GetYear(), Begin.GetMonth(), Begin.GetDay(), Begin.GetHour(), 0, 0);
    int offset = End.GetMinute() * 60 + End.GetSecond();
    CTime tEnd = offset > 0 ? (End - CTimeSpan(offset - 3600)) : End;

    // флаг что удалось вытащить информацию о канале
    bool bChannelInfoExist(false);

    CTime tTimeNext = tBegin;
    while (tTimeNext < tEnd)
    {
        CString sFileName(L"");
        ChannelFromFile tempChannel;
        sFileName.Format(L"%s%d\\%02d\\%02d\\%02d\\sig0000.xml", getSourceFilesDir().GetString(), tTimeNext.GetYear(), tTimeNext.GetMonth(), tTimeNext.GetDay(), tTimeNext.GetHour());

        // пытаемся вытащить информацию о файлах с данными
        filesByDate[tTimeNext.GetYear()].ChannelsFromFileByMonth[tTimeNext.GetMonth()].ChannelsFromFileByDay[tTimeNext.GetDay()].ChannelsFromFileByHour[tTimeNext.GetHour()] =
            getFilesInfo(channelName, sFileName);

        tTimeNext += 3600;
    }

    return filesByDate;
}

//*****************************************************************************
bool CChannelDataCorrector::getChannelInfo(const CString& xmlFileName)
{
    bool bRes(false);
    xml_document hFile;
    if (hFile.load_file(xmlFileName, parse_default, encoding_utf8))
    {
        xml_node hRoot = hFile.child(L"CommonSignalDescriptor");
        if (hRoot != NULL)
        {
            xml_node hSignal = hRoot.find_child_by_attribute(L"name", m_channelName);
            bRes = hSignal != NULL;
            if (bRes)
            {
                m_channelInfo->m_sName = m_channelName;
                m_channelInfo->m_sComment = hSignal.attribute(L"comment").value();
                m_channelInfo->m_dFrequency = hSignal.attribute(L"frequency").as_double();
                m_channelInfo->m_fMinlevel = hSignal.attribute(L"minlevel").as_float();
                m_channelInfo->m_fMaxlevel = hSignal.attribute(L"maxlevel").as_float();
                m_channelInfo->m_fSense = hSignal.attribute(L"sense").as_float();
                m_channelInfo->m_fReference = hSignal.attribute(L"reference").as_float();
                m_channelInfo->m_fShift = hSignal.attribute(L"shift").as_float();
                m_channelInfo->m_sConversion = hSignal.attribute(L"conversion").value();
                m_channelInfo->m_iTypeAdc = hSignal.attribute(L"typeADC").as_int();
                m_channelInfo->m_iNumberDSP = hSignal.attribute(L"numberDSP").as_int();
                m_channelInfo->m_iChannel = hSignal.attribute(L"channel").as_int();

                GUID guid = { 0 };
                //запрашиваем CLSID компонента Список переменных
                HRESULT hRes = CLSIDFromString(hSignal.attribute(L"id").value(), &guid);
                assert(hRes == S_OK);// все оч плохо

                m_channelInfo->m_ID = guid;
                m_channelInfo->m_iType = hSignal.attribute(L"type").as_int();
                m_channelInfo->m_sGroupName = hSignal.attribute(L"groupname").value();
            }
        }
    }

    return bRes;
}

ChannelInfo* CChannelDataCorrector::getChannelInfo()
{
    return m_channelInfo.get();
}

ChannelFromFile CChannelDataCorrector::getFilesInfo(const CString& channelName, const CString& xmlFileName)
{
    ChannelFromFile files;
    files.sDataFileName.Empty();
    files.sDescriptorFileName.Empty();

    xml_document hFile;
    if (hFile.load_file(xmlFileName, parse_default, encoding_utf8))
    {
        xml_node hRoot = hFile.child(L"CommonSignalDescriptor");
        if (hRoot != NULL)
        {
            xml_node hSignal = hRoot.find_child_by_attribute(L"name", channelName);
            if (hSignal != NULL)
            {
                files.sDataFileName = hSignal.attribute(L"data_file").value();
                files.sDescriptorFileName = hSignal.attribute(L"descriptor_file").value();
            }
        }
    }

    return files;
}

//*****************************************************************************
CString CChannelDataCorrector::getSourceFilesDir()
{
    return get_service<ZetDirsService>().getSignalsDir();
}

CString CChannelDataCorrector::getCompressedDir()
{
    return get_service<ZetDirsService>().getCompressedDir();
}

void fillDataKoeff(double correctionStartVal, double correctionEndVal,
                   float* resultData, long resDataSize,
                   float* sourseData, long srcDataSize)
{
    // вычисляем К и B для прямой результата
    // К = Y0 - YN / X0 - XN
    // B = Y0 - K * X0
    double Kresult = (correctionStartVal - correctionEndVal) / (/*0*/ -resDataSize);
    double Bresult = correctionStartVal - Kresult * 0;

    // вычисляем К и B для прямой c исходными данными
    double Ksourse = (sourseData[0] - sourseData[std::min<long>(resDataSize, srcDataSize) - 1]) / (/*0*/-std::min<long>(resDataSize, srcDataSize));
    double Bsourse = sourseData[0] - Ksourse * 0;

    long currentIndex = 0;
    for (long index = 0; index < resDataSize; ++index)
    {
        currentIndex = index % srcDataSize;
        
        resultData[index] = sourseData[currentIndex] + float((double)index * (Kresult - Ksourse) + Bresult - Bsourse);
    }
}

void fillData(double correctionStartVal, double correctionEndVal,
              float* resultData, long resDataSize,
              float* sourseData, long srcDataSize)
{
    int startIndex(0);
    int endIndex(0);
    if (srcDataSize >= resDataSize)
    {
        // если исходных данных больше чем нужно
        fillDataKoeff(correctionStartVal, correctionEndVal,
                      resultData, resDataSize,
                      sourseData, srcDataSize);
    }
    else
    {
        // если исходных данных меньше чем нужно

        startIndex = std::max<int>(0, float(resDataSize - srcDataSize) / 2.f);
        endIndex = startIndex + srcDataSize;

        // вычисляем К и B для прямой результата
        // К = Y0 - YN / X0 - XN
        // B = Y0 - K * X0
        double Kresult = (correctionStartVal - correctionEndVal) / (0 - resDataSize);
        double Bresult = correctionStartVal - Kresult * 0;
        double startVal = Kresult * startIndex + Bresult;
        double endVal = Kresult * (endIndex - 1) + Bresult;
        // переносим данные
        fillData(startVal, endVal,
                 &resultData[startIndex], srcDataSize,
                 sourseData, srcDataSize);

        // заполняем начальный интервал
        fillData(correctionStartVal, startVal,
                 resultData, startIndex,
                 sourseData, srcDataSize);

        // заполняем конечную прямую
        fillData(endVal, correctionEndVal,
                 &resultData[endIndex], resDataSize - endIndex,
                 sourseData, srcDataSize);
    }
}

void CChannelDataCorrector::FillFullInterval(std::vector<float>& data,
                                             std::vector<float>& dataFromSource,
                                             double correctionStartVal,
                                             double correctionEndVal,
                                             double correctionRangeVal)
{
    float averageVal(0);
    
    // удаляем нули и наны
    averageVal = deleteBadValues(dataFromSource,
                                 [](const float& value) mutable
    {
        return abs(value) <= FLT_MIN;
    });

    // удаляем выделяющиеся данные
    averageVal = deleteBadValues(dataFromSource,
                                 [&averageVal](const float& value) mutable
    {
        return abs(value - averageVal) > 30;
    });

    if (dataFromSource.empty())
        throw CString(L"Исходные данные не обнаружены для.");

    fillData(correctionStartVal, correctionEndVal,
             data.data(), data.size(),
             dataFromSource.data(), dataFromSource.size());
}

void CChannelDataCorrector::CorrectCurentLevel(std::vector<float>& data,
                                               std::vector<float>& dataFromSource,
                                               double correctionStartVal,
                                               double correctionEndVal,
                                               double correctionRangeVal)
{
    float averageVal(0);

    // cчитаем среднее значение
    averageVal = calculateAverageVal(dataFromSource,
                                     [](const float& value) mutable
    {
        return abs(value) > FLT_MIN;
    });

    // заменяем все выделяющиеся числа на NAN
    for (auto &value : dataFromSource)
    {
        if (abs(value) > FLT_MIN && abs(value - averageVal) > correctionRangeVal)
            value = 0;
    }

    // cчитаем среднее значение
    averageVal = calculateAverageVal(dataFromSource,
                                     [](const float& value) mutable
    {
        return abs(value) > FLT_MIN;
    });

    if (dataFromSource.empty())
        throw CString(L"Исходные данные не обнаружены.");

    int dataSize = data.size();
    int dataSourceSize = dataFromSource.size();
    // ровняем по нашей прямой
    for (int i = 0; i < dataSourceSize; ++i)
    {
        // в режиме корректировки уровня в данных есть нули
        if (abs(dataFromSource[i]) > FLT_MIN)
            dataFromSource[i] = dataFromSource[i] - averageVal + (correctionEndVal + correctionStartVal) / 2.;
    }

    int startIndex(0);
    int endIndex(0);
    if (dataSourceSize > dataSize)
    {
        // если исходных данных больше чем нужно
        memcpy_s(data.data(), dataSize * sizeof(float),
                 &dataFromSource[std::max<int>(0, (int)(float(dataSourceSize - dataSize) / 2.f - 0.5))], dataSize * sizeof(float));
        return;
    }
    else
    {
        // если исходных данных меньше чем нужно
        startIndex = std::max<int>(0, float(dataSize - dataSourceSize) / 2.f);
        memcpy_s(&data[startIndex], dataSourceSize * sizeof(float),
                 dataFromSource.data(), dataSourceSize * sizeof(float));
        endIndex = startIndex + dataSourceSize;

        if (!data.empty())
            ZeroMemory(&data[0], startIndex * sizeof(float));

        // заполняем конечную прямую
        if (endIndex > 0)
            ZeroMemory(&data[endIndex], (dataSize - endIndex) * sizeof(float));
    }
}

void CChannelDataCorrector::SaveCurentLevel(const CString& correctionChannelName,
                                            CTime correctionIntervalStart,
                                            CTime correctionIntervalEnd,
                                            double correctionRangeVal,
                                            std::vector<float>& data,
                                            std::vector<float>& dataFromSource)
{
    float averageVal(0);

    // удаляем нули и наны
    averageVal = deleteBadValues(dataFromSource,
                                 [](const float& value) mutable
    {
        return abs(value) <= FLT_MIN;
    });

    // удаляем выделяющиеся данные
    averageVal = deleteBadValues(dataFromSource,
                                 [&averageVal](const float& value) mutable
    {
        return abs(value - averageVal) > 30;
    });

    if (dataFromSource.empty())
        throw CString(L"Исходные данные не обнаружены для.");

    // забираем текущие данные по нашему корректируемому каналу
    std::vector<float> currentDataInChannel;
    getSourceChannelData(correctionChannelName,
                         correctionIntervalStart,
                         correctionIntervalEnd,
                         currentDataInChannel);

    // удаляем нули и наны
    averageVal = deleteBadValues(currentDataInChannel,
                                 [](const float& value) mutable
    {
        return abs(value) <= FLT_MIN;
    });

    // удаляем выделяющиеся данные
    averageVal = deleteBadValues(currentDataInChannel,
                                 [&averageVal](const float& value) mutable
    {
        return abs(value - averageVal) > 30;
    });

    if (currentDataInChannel.empty())
        throw CString(L"Исходные данные не обнаружены для текущего канала.");

    fillData((double)currentDataInChannel.front(), (double)currentDataInChannel.back(),
             data.data(), data.size(),
             dataFromSource.data(), dataFromSource.size());
}

CString GetAnotherPath(CString sSoursePath, CString sPath, unsigned int depthLevel)
{
    ++depthLevel;
    std::vector<CString> vPath;
    int iCurPos(0);
    CString sRes = sPath.Tokenize(_T("\\"), iCurPos);
    while (!sRes.IsEmpty())
    {
        vPath.push_back(sRes);
        sRes = sPath.Tokenize(_T("\\"), iCurPos);
    };

    CString sRet(L"");
    if (vPath.size() > depthLevel)
    {
        sRet = sSoursePath;
        sRet.TrimRight(L"\\");

        std::for_each(vPath.end() - depthLevel, vPath.end(), [&](CString &s)
        {
            sRet += L"\\";
            sRet += s;
        });
    }

    return sRet;
}

#include <Intel\ipp\ipp.h>
void Resampling(float* pSource, long lSourceSize, float* pDestination, long lDestinationSize)
{
    bool bFlag(lSourceSize > lDestinationSize);
    if (bFlag)
    {
        for (long i = 0; i < lDestinationSize; i += 2)
        {
            long lIndex = (long)(round(double(i) / double(lDestinationSize) * double(lSourceSize)));
            long lIndex2 = (long)(round(double(i + 2) / double(lDestinationSize) * double(lSourceSize)));
            if (lIndex > lSourceSize - 1)
                lIndex = lSourceSize - 1;
            if (lIndex2 > lSourceSize - 1)
                lIndex2 = lSourceSize - 1;

            float fMin(NAN), fMax(NAN);
            int iMin(lIndex), iMax(lIndex);

            int order(0);
            if (ippStsNanArg == ippsMaxOrder_32f(pSource + lIndex, lIndex2 - lIndex, &order))
            {
                /*for (long j = lIndex; j < lIndex2; ++j)
                {
                if (_finite(pSource[j]) != 0 && abs(pSource[j]) >= FLT_MIN)
                {
                if (_finite(fMax) != 0)
                {
                if (fMax < pSource[j])
                {
                fMax = pSource[j];
                iMax = j;
                }
                }
                else
                {
                fMax = pSource[j];
                iMax = j;
                }

                if (_finite(fMin) != 0)
                {
                if (fMin > pSource[j])
                {
                fMin = pSource[j];
                iMin = j;
                }
                }
                else
                {
                fMin = pSource[j];
                iMin = j;
                }
                }
                }*/
            }
            else
            {
                ippsMinMaxIndx_32f(pSource + lIndex, lIndex2 - lIndex, &fMin, &iMin, &fMax, &iMax);
            }

            if (iMin > iMax)
            {
                pDestination[i] = fMax;
                pDestination[i + 1] = fMin;
                //TRACE(L"From %d to %d: %d = %f, %d = %f\n", lIndex, lIndex2, i, fMax, i + 1, fMin);
            }
            else
            {
                pDestination[i + 1] = fMax;
                pDestination[i] = fMin;
                //TRACE(L"From %d to %d: %d = %f, %d = %f\n", lIndex, lIndex2, i, fMin, i + 1, fMax);
            }
        }
    }
    else
    {
        for (int i = 0; i < lDestinationSize; ++i)
        {
            double index = double(i) / double(lDestinationSize) * double(lSourceSize);
            long lIndex = (long)(floor(index));

            double deltaIndex = index - lIndex;
            if (lIndex + 1 == lSourceSize)
                pDestination[i] = pSource[lIndex];
            else
            {
                if (abs(pSource[lIndex]) < FLT_MIN && abs(pSource[lIndex + 1]) >= FLT_MIN)
                    pDestination[i] = pSource[lIndex + 1];
                else if (abs(pSource[lIndex]) >= FLT_MIN && abs(pSource[lIndex + 1]) < FLT_MIN)
                    pDestination[i] = pSource[lIndex];
                else
                    pDestination[i] = (float)(pSource[lIndex] * (1. - deltaIndex) + pSource[lIndex + 1] * deltaIndex);
            }
        }
    }
}

void CChannelDataCorrector::GetSourceChannelDataByInterval(const CString& channelName,
                                                           const CTime& tStart, const CTime& tEnd,
                                                           long uiLength, float* pData,
                                                           double freq)
{
    ChannelsFromFileByYear channelMap = GetSourceFilesInfoByInterval(channelName, tStart, tEnd);

    long lLength(uiLength);


    double dLength = (1 / freq) * uiLength;
    //округленное в меньшую сторону до целых секунд время начала
    //CZetTime tBegin(tStart);
    //double begin = double(tBegin.GetNanoseconds()) / 1.e9;
    //tBegin.RoundSecondsDown();
    CTime tIntervalBegin = tStart;

    double begin = 0;

    //округленное в большую сторону до целых секунд время конца
    //CZetTime tFinish(tEnd);
    //double end = 1. - double(tFinish.GetNanoseconds()) / 1.e9;
    //tFinish.RoundSecondsUp();
    CTime tIntervalEnd = tEnd;
    double end = 1;

    //время начала часовой записи
    CTime timeBegin = CTime(tIntervalBegin.GetYear(), tIntervalBegin.GetMonth(), tIntervalBegin.GetDay(), tIntervalBegin.GetHour(), 0, 0);
    CString sBegin(timeBegin.Format(L"%Y-%m-%d-%H-%M-%S"));

    //время окончания часовой записи
    int offset = tIntervalEnd.GetMinute() * 60 + tIntervalEnd.GetSecond();
    CTime timeEnd = offset > 0 ? (tIntervalEnd - CTimeSpan(offset - 3600)) : tIntervalEnd;
    CString sEnd(timeEnd.Format(L"%Y-%m-%d-%H-%M-%S"));

    SYSTEM_INFO sinf;
    GetSystemInfo(&sinf);

    DWORD dwAllocationGranularity;								//гранулярность для начального адреса, в котором может быть выделена виртуальная память
    dwAllocationGranularity = sinf.dwAllocationGranularity;

    CTime tTimeNext = timeBegin;
    CTime tTimeCurrent = tIntervalBegin;
    CTime tCurrentBegin = CTime(tStart);
    double currentBegin = begin;
    double currentEnd = 0.;
    long lPosition(0);
    while (tTimeNext < timeEnd)
    {
        //текущее время начала часовой записи
        timeBegin = tTimeNext;
        //время начала следующей часовой записи
        tTimeNext += 3600;

        if (tTimeNext > tIntervalEnd)
        {
            tTimeNext = tIntervalEnd;
            timeEnd = tIntervalEnd;
            currentEnd = end;
        }
        CString sTimeCurrent(tTimeCurrent.Format(L"%Y-%m-%d-%H-%M-%S"));
        CString sTimeNext(tTimeNext.Format(L"%Y-%m-%d-%H-%M-%S"));

        CString sFileName = channelMap[tTimeCurrent.GetYear()].ChannelsFromFileByMonth[tTimeCurrent.GetMonth()].ChannelsFromFileByDay[tTimeCurrent.GetDay()].ChannelsFromFileByHour[tTimeCurrent.GetHour()].sDataFileName;

        struct __stat64 st;
        int iStatRes = _tstat64(sFileName, &st);
        if (iStatRes != 0)
        {
            sFileName = GetAnotherPath(getSourceFilesDir(), sFileName, 4);
            iStatRes = _tstat64(sFileName, &st);
        }
        if (iStatRes == 0)
        {
            if (st.st_size > 0)
            {
                DWORD dwFileSize = DWORD(st.st_size / sizeof(float));
                double dFileDelta = 3600. / double(dwFileSize);
                HANDLE hFile(CreateFile(sFileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL));
                if (hFile != INVALID_HANDLE_VALUE)
                {
                    HANDLE hMap(CreateFileMapping(hFile, NULL, PAGE_READONLY, 0x00000000, 0x00000000, NULL));
                    if (hMap)
                    {
                        //смещение в файле исходных данных (которое нужно)
                        long long llFileOffset = long((double(CTimeSpan(tCurrentBegin - timeBegin).GetTotalSeconds()) + currentBegin) / dFileDelta) * sizeof(float);
                        long long llTail = dwFileSize * sizeof(float);
                        //смещение в файле сжатых данных (приведенное к гранулярности)
                        long long llFileOffsetReal = llFileOffset / dwAllocationGranularity * dwAllocationGranularity;
                        long lDeltaFileOffset(long(llFileOffset - llFileOffsetReal));
                        //количество точек
                        double dFactor = freq * dFileDelta;
                        long lCount = long(lLength * dFactor);
                        if (llTail - llFileOffset < lCount * sizeof(float))
                            lCount = long((llTail - long(llFileOffset)) / sizeof(float));

                        unsigned long ulSize = unsigned long(sizeof(float)* lCount + lDeltaFileOffset);
                        float* pView = reinterpret_cast<float*>(MapViewOfFile(hMap, FILE_MAP_READ, 0, DWORD(llFileOffsetReal), ulSize));
                        if (pView != NULL)
                        {
                            float* pSrcData = &pView[lDeltaFileOffset / sizeof(float)];
                            float* pDestData = pData + lPosition;

                            // проверяем что при загрузке данных не будет выхода за пределы диапазона
                            if (lPosition + lCount > (long)uiLength)
                                lCount = uiLength - lPosition;
                            if (lCount < 0)
                                break;

                            if (dFactor == 1.)
                            {
                                memcpy_s(pDestData, sizeof(float)* lCount, pSrcData, sizeof(float)* lCount);
                                //TRACE("Fill %d points by source\n", lCount);
                            }
                            else
                            {
                                //Прореживание исходных данных
                                if (dFactor > 1.)
                                {
                                    if (dFactor - floor(dFactor) > 0)
                                    {
                                        long lQuantity = long(double(lCount) / dFactor);
                                        if (lPosition + lQuantity > long(uiLength))
                                            lQuantity = long(uiLength) - lPosition;
                                        Resampling(pSrcData, lCount, pDestData, lQuantity);
                                        //TRACE("Fill %d points by resampling\n", lQuantity);
                                    }
                                    else
                                    {
                                        int iIndex(0);
                                        for (int i = 0; i < lCount; i += int(dFactor) * 2)
                                        {
                                            float fMin, fMax;
                                            fMin = fMax = pSrcData[i];
                                            for (int j = 0; j < int(dFactor); ++j)
                                            {
                                                //CheckMinimum(fMin, pSrcData[i + j]);
                                                //CheckMaximum(fMax, pSrcData[i + j]);
                                            }
                                            if (lPosition + iIndex < long(uiLength))
                                            {
                                                pDestData[iIndex] = fMin;
                                                ++iIndex;
                                                if (lPosition + iIndex < long(uiLength))
                                                {
                                                    pDestData[iIndex] = fMax;
                                                    ++iIndex;
                                                }
                                            }
                                        }
                                        //TRACE("Fill %d points by decimation\n", iIndex);
                                    }
                                }
                                //Передискретизация исходных данных в более высокую частоту дискретизации
                                else
                                {
                                    long lQuantity = long(double(lCount) / dFactor);
                                    if (lPosition + lQuantity > long(uiLength))
                                        lQuantity = long(uiLength) - lPosition;
                                    Resampling(pSrcData, lCount, pDestData, lQuantity);
                                    //TRACE("Fill %d points by resampling\n", lQuantity);
                                }
                            }
                            UnmapViewOfFile(pView);
                        }
                        else
                        {

                        }
                        lPosition += long(lCount / dFactor);
                        lLength -= long(lCount / dFactor);
                        CloseHandle(hMap);
                    }
                    CloseHandle(hFile);
                }
            }
        }
        else
        {
            CString sTimeCurrent(tTimeCurrent.Format(L"%Y-%m-%d-%H-%M-%S"));
            CString sTimeNext(tTimeNext.Format(L"%Y-%m-%d-%H-%M-%S"));
            long lCount = long(CTimeSpan(tTimeNext - tTimeCurrent).GetTotalSeconds() * freq);
            lPosition += lCount;
            lLength -= lCount;
            //TRACE("Fill %d points by empty\n", lCount);
        }
        tTimeCurrent = tTimeNext;
        tCurrentBegin = tTimeCurrent;
        currentBegin = 0.;
    }
}