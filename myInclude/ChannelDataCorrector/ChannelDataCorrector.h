#pragma once
#include <list>
#include <memory>
#include "ChannelsHelpHeader.h"
#include "DescriptorFileHelper.h"

#ifdef USE_STREAMWRITER
#include "ChannelDataCorrector/TrendWriter/StreamTrendWriter.h"
#endif

//*****************************************************************************
// ��� ��������� ������
enum CorrectionMode
{
    eFillFullInterval = 0,	// ���������� ������� ����� ���������
    eCorrectLevelCurData,	// ����������������� ������� ������� ������
    eSaveIntervalLevels		// ��������� ������ �� ����������
};

//*****************************************************************************
// ����� ������������� ������ �� ������������� ������ � �������� ���������� �������
class CChannelDataCorrector
{
public:
    CChannelDataCorrector(CString channelName, const CTime& begin, const CTime& end);
    ~CChannelDataCorrector();

    // �������� ������ �� ������
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

    // �������� ������ �� ������
    void getSourceChannelData(const CString& channelName,
                              const CTime& begin, const CTime& end,
                              std::vector<float>& data);


    void getRandomCorrectData(CTime timeStart, CTime timeEnd, double startIntervalVal,
                              double endIntervalVal, double rangeVal,
                              std::vector<float>& data);

#ifdef USE_STREAMWRITER
    /*
    * ������ ������
    * @param timeStart - ��������� �������� ��������� �������������� ������
    * @param timeEnd   - ��������  �������� ��������� �������������� ������
    * @param startIntervalVal - ��������� �������� ����� � �������� ���������
    * @param endIntervalVal   - ��������  �������� ����� � �������� ���������
    * @param rangeVal - ������� �������� � ��������� ���������
    */
    bool correctChannelData(CTime timeStart, CTime timeEnd, double startIntervalVal,
                            double endIntervalVal, double rangeVal);


    bool correctChannelDataNew(CTime timeStart, CTime timeEnd, float *data, long lSize);

    // ������ � ����� ������
    void startWriting();
    void stopWriting();
#endif

    // ��������� ������ �������
    static void FillChannelList(CString sDirectory,
                                std::list<std::pair<CString, CString>>& channelList);

    ChannelInfo* getChannelInfo();
private:
    // ��������� ������ 
    void calcChannelInfo(ChannelInfo* chanInfo, CString channelName,
                         const CTime& begin, const CTime& end);

    // ��������� ���������� � ������
    bool getChannelInfo(const CString& xmlFileName);
    // �������� ���������� � ������
    ChannelFromFile getFilesInfo(const CString& channelName, const CString& xmlFileName);

    // �������� ���������� � ������ ������� ���������� ����������
    ChannelsFromFileByYear GetSourceFilesInfoByInterval(const CString& channelName, CTime Begin, CTime End);

    void GetSourceChannelDataByInterval(const CString& channelName,
                                        const CTime& tStart, const CTime& tEnd,
                                        long uiLength, float* pData, double freq);
private:
    // ��������� ����������
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
