#include "stdafx.h"
#include "DeviceInfoFromChan.h"
#include <ModbusDefinitions.h>
#include <Zet7xxxDeviceTypes.h>

#define MAX_ZET7XXX_CHANNELS_ONE_DEVICE	4

CDeviceInfoFromChan::CDeviceInfoFromChan()
	: m_sTypeName		(_T(""))	// ��� ����������
	, m_sDeviceSerial	(_T(""))	// �������� ����� ����������
	, m_lDeviceType		(0)			// ����� ���� ����������
	, m_ullDeviceSerial	(0)			// �������� ����� ����������
	, m_ChannelData		()			// ����������� ���������
{
}

CDeviceInfoFromChan::CDeviceInfoFromChan(_In_ chanInfoS &channel)
	: m_sTypeName		(_T(""))	// ��� ����������
	, m_sDeviceSerial	(_T(""))	// �������� ����� ����������
	, m_lDeviceType		(0)			// ����� ���� ����������
	, m_ullDeviceSerial	(0)			// �������� ����� ����������
	, m_ChannelData		()			// ����������� ���������
{
	SetChannelData(channel);
}

CDeviceInfoFromChan::~CDeviceInfoFromChan()
{
}

void CDeviceInfoFromChan::SetChannelData(_In_ chanInfoS &channel)
{
	m_ChannelData = channel;
	m_lDeviceType		= 0;
	m_ullDeviceSerial	= 0;
	m_sTypeName		.Empty();
	m_sDeviceSerial	.Empty();

	switch (m_ChannelData.typeADC)
	{
		// 0: ���(����� ���� �������)
		// 1: ���
		// 4: ����������� ����� ���
		// 5: ����� ������������ ���
		case 0:
		case 1:
		case 4:
		case 5:
			m_lDeviceType = GetADCDACDeviceType(m_ChannelData.ID);
			m_ullDeviceSerial = GetADCDACDeviceSerial(m_ChannelData.ID);
			//*****************************************************************
			switch (m_lDeviceType)
			{
				case 10: m_sTypeName = L"ZET210"; break;
				case 14: m_sTypeName = L"ZET017-U4/8"; break;
				case 15: m_sTypeName = L"ZET017-U2"; break;
				case 16: m_sTypeName = L"ZET220"; break;
				case 17: m_sTypeName = L"ZET230"; break;
				case 19: m_sTypeName = L"ZET240"; break;
				default: m_sTypeName = L"";	break;
			}
			m_sDeviceSerial.Format(L"%llu", m_ullDeviceSerial);
			break;
			// 2: �����������
		case 2:
			break;
			// 3: ��������
		case 3:
			break;
			//6 : ���������������� ������(����� 7000, FreqADC ����� ���� ������ 100 ��)
			//7 : ����������� ���������������� ������
			//8 : ������� �����(� ������� FreqADC, ������� ������ ����� �������, �� ������� ��� �� �������)
		case 6:
		case 7:
		case 8:
		{
			Get7xxxDeviceInfo(channel.ID, channel.Name, &m_lDeviceType, &m_ullDeviceSerial);
			ZET7XXX_DEFINE_DEVICE_NAMES(DeviceList);
			m_sTypeName = DeviceList[m_lDeviceType];
			m_sDeviceSerial.Format(L"%I64x", m_ullDeviceSerial);
			break;
		}
		default:
			break;
	}
}

CString CDeviceInfoFromChan::sGetName()
{
	return m_sTypeName;
}

CString CDeviceInfoFromChan::sGetsSerial()
{
	return m_sDeviceSerial;
}

uint64_t CDeviceInfoFromChan::ullGetSerial()
{
	return m_ullDeviceSerial;
}

long CDeviceInfoFromChan::lGetType()
{
	return m_lDeviceType;
}

long CDeviceInfoFromChan::GetADCDACDeviceSerial(_In_ long lID)
{
	return (lID >> 16) & 0xffffffff;
}

long CDeviceInfoFromChan::GetADCDACDeviceType(_In_ long lID)
{
	return (lID & 0x0000ff00) >> 8;
}

unsigned short CalcCrc16(char *pcBlock, unsigned short RS_crc)
{
	long i;

	RS_crc ^= (*pcBlock) & 0xff;
	for (i = 0; i < 8; i++)
		RS_crc = (RS_crc & 0x01) ? (RS_crc >> 1) ^ 0xA001 : RS_crc >> 1;

	return RS_crc;
}

void CDeviceInfoFromChan::Get7xxxDeviceInfo(_In_ long lID, _In_ CString Name, 
										   _Out_opt_ long *Type /*= nullptr*/, 
										   _Out_opt_ uint64_t *Serial /*= nullptr*/)
{
	// ���� �� ����� ���������� ��������� ������ �� ������
	if ((Type == nullptr) && (Serial == nullptr))
		return;

	if (Type != nullptr)
		*Type = 0;
	if (Serial != nullptr)
		*Serial = 0;

	HANDLE hFile = OpenFileMapping(FILE_MAP_READ, TRUE, L"MyData7000");
	if (hFile != NULL)
	{
		STRUCT7000EX* m_pModbus = reinterpret_cast<STRUCT7000EX*>(MapViewOfFile(hFile, FILE_MAP_READ, 0, 0, 0));
		if (m_pModbus != NULL)
		{
			for (int i = 0; i < NUMCELLS; ++i)
			{
				unsigned short usSerialCRC = 0xffff;
				unsigned long long ullSerial = m_pModbus->devinfo[i].serial_num;
				for (int j = 0; j < sizeof(unsigned long long); ++j)
					usSerialCRC = CalcCrc16(&((char*)&ullSerial)[j], usSerialCRC);
				long lDeviceID(((m_pModbus->devinfo[i].type + 20) << 8) + ((long)usSerialCRC << 16));

				long lDelta = lID - lDeviceID;
				if (lDelta >= 0 && lDelta < MAX_ZET7XXX_CHANNELS_ONE_DEVICE)
				{
					CString sAddress;
					sAddress.Format(L"(%d)", m_pModbus->devinfo[i].node);
					if (Name.Find(sAddress) != -1)
					{
						if (Serial != nullptr)
							*Serial = m_pModbus->devinfo[i].serial_num;
						if (Type != nullptr)
							*Type = m_pModbus->devinfo[i].type;
					}
				}

			}

			UnmapViewOfFile(m_pModbus);
		}
		CloseHandle(hFile);
	}
}
