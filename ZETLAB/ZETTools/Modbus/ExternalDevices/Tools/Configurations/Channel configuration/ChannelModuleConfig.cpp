#include "Modbus\ExternalDevices\Tools\Configurations\Channel configuration\channelModuleConfig.h"
#include "Modbus\ExternalDevices\Tools\Secondary functions\secondaryFunctions.h"

#include "ZETFile\include\ZETFile.h"

CChannelModuleConfig::CChannelModuleConfig(CString configAdr_)
{
    m_sConfigAdr = configAdr_;
}

CChannelModuleConfig::~CChannelModuleConfig()
{

}

void CChannelModuleConfig::save(CString interfaceModuleName_, CString deviceModuleName_, CString channelModuleName_)
{
    CString channelModuleDir = m_sConfigAdr + L"\\" + interfaceModuleName_ + L"\\" + deviceModuleName_ + L"\\" + channelModuleName_;
    CString channelModulePath = channelModuleDir + L"\\" + channelModuleName_ + L".cfg";

    long retVal = zfCreateNewDirectory(channelModuleDir);
    retVal = zfWriteFileBinary(channelModulePath, &m_cChannelModule, sizeof(m_cChannelModule));
}

void CChannelModuleConfig::load(CString interfaceModuleName_, CString deviceModuleName_, CString channelModuleName_)
{
    CString channelModuleDir = m_sConfigAdr + L"\\" + interfaceModuleName_ + L"\\" + deviceModuleName_ + L"\\" + channelModuleName_;
    CString channelModulePath = channelModuleDir + L"\\" + channelModuleName_ + L".cfg";

    long retVal = zfCreateNewDirectory(channelModuleDir);
    retVal = zfReadFileBinary(channelModulePath, &m_cChannelModule, sizeof(m_cChannelModule));
}

void CChannelModuleConfig::remove(CString interfaceModuleName_, CString deviceModuleName_, CString channelModuleName_)
{
    long retVal = zfDeletePath(m_sConfigAdr + L"\\" + interfaceModuleName_ + L"\\" + deviceModuleName_ + L"\\" + channelModuleName_);
}

void CChannelModuleConfig::setChannelName(CString channelName_)
{
    SF_convertCStringToChar(channelName_, m_cChannelModule.name, sizeof(m_cChannelModule.name));
}

CString CChannelModuleConfig::getChannelName()
{
    return CString(m_cChannelModule.name);
}

void CChannelModuleConfig::setRegAddress(uint16_t regAddress_)
{
    m_cChannelModule.regAddr = regAddress_;
}

uint16_t CChannelModuleConfig::getRegAddress()
{
    return m_cChannelModule.regAddr;
}

void CChannelModuleConfig::setRegCommand(uint8_t regCommand_)
{
    m_cChannelModule.regComm = regCommand_;
}

uint8_t CChannelModuleConfig::getRegCommand()
{
    return m_cChannelModule.regComm;
}

void CChannelModuleConfig::setRegDataType(uint16_t regDataType_)
{
    m_cChannelModule.regDataType = regDataType_;
}

uint16_t CChannelModuleConfig::getRegDataType()
{
    return m_cChannelModule.regDataType;
}

uint16_t CChannelModuleConfig::getRegDataTypeSz()
{
    uint16_t dataTypeSz = 0;

    switch (getRegDataType())
    {
        case Float_AB_CD_Type:
        case Float_CD_AB_Type:
        case Float_DC_BA_Type:
        case Int32_AB_CD_Type:
        case Int32_CD_AB_Type:
        case Int32_DC_BA_Type:
            dataTypeSz = 4;
            break;
        case Int16_A_B_Type:
        case Int16_B_A_Type:
            dataTypeSz = 2;
            break;
        default:
            break;
    }

    return dataTypeSz;
}

double CChannelModuleConfig::getRegDataByDouble(BYTE* data_, DWORD dataSz_)
{
    double retVal = 0;

    if (dataSz_ >= getRegDataTypeSz())
    {
        float tmpFloatVal = 0;
        int32_t tmpInt32Val = 0;
        int16_t tmpInt16Val = 0;
        switch (getRegDataType())
        {
            // ¬ устройстве хранитс€ как DC BA (задом на перед)
            case Float_AB_CD_Type:
                memcpy_s(&tmpFloatVal, 1, &data_[3], 1);
                memcpy_s(((BYTE*)&tmpFloatVal + 1), 1, &data_[2], 1);
                memcpy_s(((BYTE*)&tmpFloatVal + 2), 1, &data_[1], 1);
                memcpy_s(((BYTE*)&tmpFloatVal + 3), 1, &data_[0], 1);
                retVal = double(tmpFloatVal);
                break;
            case Float_CD_AB_Type:
                memcpy_s(&tmpFloatVal, 1, &data_[1], 1);
                memcpy_s(((BYTE*)&tmpFloatVal + 1), 1, &data_[0], 1);
                memcpy_s(((BYTE*)&tmpFloatVal + 2), 1, &data_[3], 1);
                memcpy_s(((BYTE*)&tmpFloatVal + 3), 1, &data_[2], 1);
                retVal = double(tmpFloatVal);
                break;
            case Float_DC_BA_Type:
                memcpy_s(&tmpFloatVal, 1, &data_[0], 1);
                memcpy_s(((BYTE*)&tmpFloatVal + 1), 1, &data_[1], 1);
                memcpy_s(((BYTE*)&tmpFloatVal + 2), 1, &data_[2], 1);
                memcpy_s(((BYTE*)&tmpFloatVal + 3), 1, &data_[3], 1);
                retVal = double(tmpFloatVal);
                break;
            case Int32_AB_CD_Type:
                memcpy_s(&tmpInt32Val, 1, &data_[3], 1);
                memcpy_s(((BYTE*)&tmpInt32Val + 1), 1, &data_[2], 1);
                memcpy_s(((BYTE*)&tmpInt32Val + 2), 1, &data_[1], 1);
                memcpy_s(((BYTE*)&tmpInt32Val + 3), 1, &data_[0], 1);
                retVal = double(tmpInt32Val);
                break;
            case Int32_CD_AB_Type:
                memcpy_s(&tmpInt32Val, 1, &data_[1], 1);
                memcpy_s(((BYTE*)&tmpInt32Val + 1), 1, &data_[0], 1);
                memcpy_s(((BYTE*)&tmpInt32Val + 2), 1, &data_[3], 1);
                memcpy_s(((BYTE*)&tmpInt32Val + 3), 1, &data_[2], 1);
                retVal = double(tmpInt32Val);
                break;
            case Int32_DC_BA_Type:
                memcpy_s(&tmpInt32Val, 1, &data_[0], 1);
                memcpy_s(((BYTE*)&tmpInt32Val + 1), 1, &data_[1], 1);
                memcpy_s(((BYTE*)&tmpInt32Val + 2), 1, &data_[2], 1);
                memcpy_s(((BYTE*)&tmpInt32Val + 3), 1, &data_[3], 1);
                retVal = double(tmpInt32Val);
                break;
            case Int16_A_B_Type:
                memcpy_s(&tmpInt16Val, 1, &data_[1], 1);
                memcpy_s(((BYTE*)&tmpInt16Val + 1), 1, &data_[0], 1);
                retVal = double(tmpInt16Val);
                break;
            case Int16_B_A_Type:
                memcpy_s(&tmpInt16Val, 1, &data_[0], 1);
                memcpy_s(((BYTE*)&tmpInt16Val + 1), 1, &data_[1], 1);
                retVal = double(tmpInt16Val);
                break;
            default:
                break;
        }
    }

    return retVal;
}

void CChannelModuleConfig::setRegReadFreq(float regReadFreq_)
{
    m_cChannelModule.regReadFreq = regReadFreq_;
}

float CChannelModuleConfig::getReadFreq()
{
    return m_cChannelModule.regReadFreq;
}

void CChannelModuleConfig::setRegMeasure(CString regMeasure_)
{
    SF_convertCStringToChar(regMeasure_, m_cChannelModule.regMeasure, sizeof(m_cChannelModule.regMeasure));
}

CString CChannelModuleConfig::getRegMeasure()
{
    return CString(m_cChannelModule.regMeasure);
}

void CChannelModuleConfig::setRegResolution(float regResolution_)
{
    m_cChannelModule.regResolution = regResolution_;
}

float CChannelModuleConfig::getRegResolutoin()
{
    return m_cChannelModule.regResolution;
}

void CChannelModuleConfig::setRegMaxLevel(float regMaxLevel_)
{
    m_cChannelModule.regMaxLevel = regMaxLevel_;
}

float CChannelModuleConfig::getRegMaxLevel()
{
    return m_cChannelModule.regMaxLevel;
}

void CChannelModuleConfig::setRegReference(float regReference_)
{
    m_cChannelModule.regReference = regReference_;
}

float CChannelModuleConfig::getRegReference()
{
    return m_cChannelModule.regReference;
}

void CChannelModuleConfig::setRegMultiplicat(float regMultiplicat_)
{
    m_cChannelModule.regMultiplicat = regMultiplicat_;
}

float CChannelModuleConfig::getRegMultiplicat()
{
    return m_cChannelModule.regMultiplicat;
}

void CChannelModuleConfig::setRegAdditive(float regAdditive_)
{
    m_cChannelModule.regAdditive = regAdditive_;
}

float CChannelModuleConfig::getRegAdditive()
{
    return m_cChannelModule.regAdditive;
}