#pragma once
//*************************************************************************************************
#define DEVICE_NAME_SIZE			128
//*************************************************************************************************
// ��������� ������� ���������� � Unit �� Synchronization
struct SynchronizationUnitStruct
{
	wchar_t wcDeviceName[DEVICE_NAME_SIZE];	// �������� ���������� � ������� "%s �%I64x" ��� "%s �%d" Name, serial
	long long llSerial;		// �������� ����� ����������
	double dTime;			// �����
	double dLatitude;		// ������
	double dLongitude;		// �������
	double dAltitude;		// ������
	double dVelosity;		// ��������
	int iSatellites;		// ���������� ���������
	bool bMasterPTP;		// ���� ���� ��� ���������� �������� ������� PTP
	bool bSlavePTP;			// ���� ���� ��� ���������� �������� ������� PTP
	//*********************************************************************************************
	SynchronizationUnitStruct()
		: dTime		(0.)
		, llSerial	(0)
		, dLatitude	(0.)
		, dLongitude(0.)
		, dAltitude	(0.)
		, dVelosity	(0.)
		, iSatellites(0)
		, bMasterPTP(false)
		, bSlavePTP	(false)
	{
		ZeroMemory(wcDeviceName, DEVICE_NAME_SIZE * sizeof(wchar_t));
	}
};
//*************************************************************************************************