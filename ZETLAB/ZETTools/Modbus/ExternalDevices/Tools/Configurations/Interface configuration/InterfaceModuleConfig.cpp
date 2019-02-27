#include "Modbus\ExternalDevices\Tools\Configurations\Interface configuration\interfaceModuleConfig.h"
#include "Modbus\ExternalDevices\Tools\Secondary functions\secondaryFunctions.h"

#include "ZETFile\include\ZETFile.h"

CInterfaceModuleConfig::CInterfaceModuleConfig(CString configAdr_)
{
    m_sConfigAdr = configAdr_;
}

CInterfaceModuleConfig::~CInterfaceModuleConfig()
{

}

void CInterfaceModuleConfig::setCommPortInstanceId(CString commPortInstanceId_)
{
    SF_convertCStringToChar(commPortInstanceId_, m_cInterfaceModule.instanceId, sizeof(m_cInterfaceModule.instanceId));
}

CString CInterfaceModuleConfig::getCommPortInstanceId()
{
    return CString(m_cInterfaceModule.instanceId);
}

void CInterfaceModuleConfig::setCommPortUserName(CString commPortUserName_)
{
    SF_convertCStringToChar(commPortUserName_, m_cInterfaceModule.name, sizeof(m_cInterfaceModule.name));
}

CString CInterfaceModuleConfig::getCommPortUserName()
{
    return CString(m_cInterfaceModule.name);
}

void CInterfaceModuleConfig::setCommPortBaudRate(uint32_t baudRate_)
{
    m_cInterfaceModule.baudRate = baudRate_;
}

uint32_t CInterfaceModuleConfig::getCommPortBaudRate()
{
    return m_cInterfaceModule.baudRate;
}

void CInterfaceModuleConfig::setCommPortByteSize(uint8_t byteSize_)
{
    m_cInterfaceModule.byteSize = byteSize_;
}

uint8_t CInterfaceModuleConfig::getCommPortByteSize()
{
    return m_cInterfaceModule.byteSize;
}

void CInterfaceModuleConfig::setCommPortParity(uint8_t parity_)
{
    m_cInterfaceModule.parity = parity_;
}

uint8_t CInterfaceModuleConfig::getCommPortParity()
{
    return m_cInterfaceModule.parity;
}

void CInterfaceModuleConfig::setCommPortStopBits(uint8_t stopBits_)
{
    m_cInterfaceModule.stopBits = stopBits_;
}

uint8_t CInterfaceModuleConfig::getCommPortStopBits()
{
    return m_cInterfaceModule.stopBits;
}

void CInterfaceModuleConfig::setMaxAnswerTimeOut(float maxAnswerTimeOut_)
{
    m_cInterfaceModule.maxAnswerTimeOut = maxAnswerTimeOut_;
}

float CInterfaceModuleConfig::getMaxAnswerTimeOut()
{
    return m_cInterfaceModule.maxAnswerTimeOut;
}

void CInterfaceModuleConfig::setMaxNewDataDelay(float maxNewDataDelay_)
{
    m_cInterfaceModule.maxNewDataDelay = maxNewDataDelay_;
}

float CInterfaceModuleConfig::getMaxNewDataDelay()
{
    return m_cInterfaceModule.maxNewDataDelay;
}

void CInterfaceModuleConfig::save(CString interfaceModuleName_)
{
    CString interfaceModuleDir = m_sConfigAdr + L"\\" + interfaceModuleName_;
    CString interfaceModulePath = interfaceModuleDir + L"\\" + interfaceModuleName_ + L".cfg";

    long retVal = zfCreateNewDirectory(interfaceModuleDir);
    retVal = zfWriteFileBinary(interfaceModulePath, &m_cInterfaceModule, sizeof(m_cInterfaceModule));
}

void CInterfaceModuleConfig::load(CString interfaceModuleName_)
{
    CString interfaceModuleDir = m_sConfigAdr + L"\\" + interfaceModuleName_;
    CString interfaceModulePath = interfaceModuleDir + L"\\" + interfaceModuleName_ + L".cfg";

    long retVal = zfCreateNewDirectory(interfaceModuleDir);
    retVal = zfReadFileBinary(interfaceModulePath, &m_cInterfaceModule, sizeof(m_cInterfaceModule));
}

void CInterfaceModuleConfig::remove(CString interfaceModuleName_)
{
    long retVal = zfDeletePath(m_sConfigAdr + L"\\" + interfaceModuleName_);
}
