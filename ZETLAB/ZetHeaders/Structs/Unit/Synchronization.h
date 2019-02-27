#pragma once
//*************************************************************************************************
#define DEVICE_NAME_SIZE			128
//*************************************************************************************************
// структура которая передается в Unit из Synchronization
struct SynchronizationUnitStruct
{
	wchar_t wcDeviceName[DEVICE_NAME_SIZE];	// название устройства в формате "%s №%I64x" или "%s №%d" Name, serial
	long long llSerial;		// серийный номер устройства
	double dTime;			// время
	double dLatitude;		// широта
	double dLongitude;		// долгота
	double dAltitude;		// высота
	double dVelosity;		// скорсоть
	int iSatellites;		// количество спутников
	bool bMasterPTP;		// флаг того что устройство является ведущим PTP
	bool bSlavePTP;			// флаг того что устройство является ведомым PTP
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