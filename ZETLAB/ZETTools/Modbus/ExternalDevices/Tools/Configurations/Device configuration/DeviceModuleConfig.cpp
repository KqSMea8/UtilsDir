#include "Modbus\ExternalDevices\Tools\Configurations\Device configuration\deviceModuleConfig.h"
#include "Modbus\ExternalDevices\Tools\Secondary functions\secondaryFunctions.h"

#include "ZETFile\include\ZETFile.h"

CDeviceModuleConfig::CDeviceModuleConfig(CString configAdr_)
{
    m_sConfigAdr = configAdr_;
}

CDeviceModuleConfig::~CDeviceModuleConfig()
{

}

void CDeviceModuleConfig::save(CString interfaceModuleName_, CString deviceModuleName_)
{
    CString deviceModuleDir = m_sConfigAdr + L"\\" + interfaceModuleName_ + L"\\" + deviceModuleName_;
    CString deviceModulePath = deviceModuleDir + L"\\" + deviceModuleName_ + L".cfg";

    long retVal = zfCreateNewDirectory(deviceModuleDir);
    retVal = zfWriteFileBinary(deviceModulePath, &m_cDeviceModule, sizeof(m_cDeviceModule));
}

void CDeviceModuleConfig::load(CString interfaceModuleName_, CString deviceModuleName_)
{
    CString deviceModuleDir = m_sConfigAdr + L"\\" + interfaceModuleName_ + L"\\" + deviceModuleName_;
    CString deviceModulePath = deviceModuleDir + L"\\" + deviceModuleName_ + L".cfg";

    long retVal = zfCreateNewDirectory(deviceModuleDir);
    retVal = zfReadFileBinary(deviceModulePath, &m_cDeviceModule, sizeof(m_cDeviceModule));
}

void CDeviceModuleConfig::remove(CString interfaceModuleName_, CString deviceModuleName_)
{
    long retVal = zfDeletePath(m_sConfigAdr + L"\\" + interfaceModuleName_ + L"\\" + deviceModuleName_);
}

void CDeviceModuleConfig::setDeviceName(CString deviceName_)
{
    SF_convertCStringToChar(deviceName_, m_cDeviceModule.name, sizeof(m_cDeviceModule.name));
}

CString CDeviceModuleConfig::getDeviceName()
{
    return CString(m_cDeviceModule.name);
}

void CDeviceModuleConfig::setDeviceNode(uint8_t deviceNode_)
{
    m_cDeviceModule.node = deviceNode_;
}

uint8_t CDeviceModuleConfig::getDeviceNode()
{
    return m_cDeviceModule.node;
}

void CDeviceModuleConfig::setProtocolType(uint8_t protocolType_)
{
    m_cDeviceModule.protocolType = protocolType_;
}

uint8_t CDeviceModuleConfig::getProtocolType()
{
    return m_cDeviceModule.protocolType;
}