#include "Modbus\ExternalDevices\Tools\Secondary functions\secondaryFunctions.h"
#include <windows.h>
#include <Setupapi.h>
#include <devguid.h>

#pragma comment(lib,"SetupAPI")

CString SF_convertFloatValToCString(float val_)
{
    CString tmpVal = L"";
    tmpVal.AppendFormat(L"%f", val_);

    return tmpVal;
}

CString SF_convertLongValToCString(long val_)
{
    CString tmpVal = L"";
    tmpVal.AppendFormat(L"%i", val_);

    return tmpVal;
}

void SF_convertCStringToChar(CString str_, char* charStr_, long charSz_)
{
    CStringA tmpVal = CStringA(str_);

    long tmpValSz = tmpVal.GetLength();
    if (tmpValSz > charSz_)
        tmpValSz = charSz_;

    memset(charStr_, 0x00, charSz_);
    memcpy_s(charStr_, charSz_, tmpVal.GetString(), tmpValSz);
}

void SF_getCommPortsList(std::vector<CString>& comPortNames_, std::vector<CString>& comPortFriendlyNames_,
    std::vector<CString>& comPortInstanceIds_)
{
    comPortNames_.clear();
    comPortFriendlyNames_.clear();
    comPortInstanceIds_.clear();

    // Проходимся по всем подключенным COM портам
    HDEVINFO deviceInfoSet = NULL;
    GUID *guidDev = (GUID*)&GUID_DEVCLASS_PORTS;   // GUID COM портов
    deviceInfoSet = SetupDiGetClassDevs(guidDev, NULL, NULL, DIGCF_PRESENT | DIGCF_PROFILE);

    int memberIndex = 0;
    while (true)
    {
        // Проходим по всем COM устройствам
        SP_DEVINFO_DATA deviceInfoData;
        ZeroMemory(&deviceInfoData, sizeof(SP_DEVINFO_DATA));
        deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
        if (SetupDiEnumDeviceInfo(deviceInfoSet, memberIndex, &deviceInfoData) == FALSE) {
            if (GetLastError() == ERROR_NO_MORE_ITEMS) {
                break;
            }
        }

        CString instanceId = L"", friendlyName = L"", comPortName = L"";

        // Вычитываем InstanceId для каждого COM порта
        DWORD nSize = 0;
        DWORD buffersize = 4000;
        TCHAR buffer[4000] = { 0 };
        SetupDiGetDeviceInstanceId(deviceInfoSet, &deviceInfoData, buffer, sizeof(buffer), &nSize);
        instanceId = CString(buffer);

        BYTE regBuffer[4000] = { 0 };
        DWORD regRequiredSz = 0, regPropertyType = 0;
        if (SetupDiGetDeviceRegistryProperty(deviceInfoSet, &deviceInfoData, SPDRP_FRIENDLYNAME, &regPropertyType, regBuffer, sizeof(regBuffer), &regRequiredSz))
        {
            friendlyName = CString((TCHAR*)regBuffer);

            // Приводим FriendlyName к имени порта (последнее, то что в скобках)
            int firstComPortNameInd = friendlyName.ReverseFind('(') + 1;
            int LastComPortNameInd = friendlyName.ReverseFind(')') - 1;
            comPortName = friendlyName.Mid(firstComPortNameInd, LastComPortNameInd - firstComPortNameInd + 1);
        }

        comPortNames_.push_back(comPortName);
        comPortFriendlyNames_.push_back(friendlyName);
        comPortInstanceIds_.push_back(instanceId);

        memberIndex++;
    }

    if (deviceInfoSet != NULL)
        SetupDiDestroyDeviceInfoList(deviceInfoSet);
}
