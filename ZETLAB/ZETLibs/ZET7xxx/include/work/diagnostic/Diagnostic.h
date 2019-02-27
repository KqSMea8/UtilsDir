#pragma once
#include "base\ErrorCodes.h"
#include "base\Types.h"
#include "DiagnosticInfo.h"
#include <ZETSimple\include\time\timer\difftimer\DiffTimer2.h>

#include <boost\noncopyable.hpp>

using namespace zet7xxx::diagnostic;
using namespace zet7xxx::errors;

SYNC_STATE_NAME_CAN_PAR(syncStateName);
GPS_STATE_NAME_SENSOR_GPS(gpsStateName);
PTP_STATE_NAME_SENSOR_PTP(ptpStateName);
PTP_SYNC_STATE_NAME_SENSOR_PTP(ptpSyncStateName);
POWER_QUALITY_NAME_DIAGNOST(powerName);
DATA_QUALITY_NAME_DIAGNOST(dataName);
SYNC_QUALITY_NAME_DIAGNOST(syncName);
SENSOR_QUALITY_NAME_DIAGNOST(sensorName);

inline static bool isParameterStatisticable(DiagnosticParameter parameter)
{
	switch (parameter)
	{
	case dfCurrentTimeUart:
	case dfCurrentTimeCan:
	case dfSyncState:
	case dfGpsState:
	case dfGpsTime:
	case dfSavedDuty:
	case dfSyncOffsetMean:
	case dfSyncOffsetStdDev:
	case dfPtpState:
	case dfPtpTime:
	case dfPtpSyncState:
	case dfPowerQuality:
	case dfDataQuality:
	case dfSynchronizationQuality:
	case dfSensorQuality:
		return false;
	default:
		return true;
		break;
	}
}
template <typename T>
inline static std::wstring Parameter2Text(DiagnosticParameter parameter, const T& value)
{
	wchar_t buffer[128];
	wcscpy_s(buffer, _T("-"));
				
	switch (parameter)
	{
		case dfExchangeCount:
		case dfNoError:
		case dfNoResponse:
		case dfShortResponse:
		case dfNodeError:
		case dfCommandError:
		case dfCrcError:
		case dfMasterError:
		case dfDataStream:
		case dfSyncOffsetStdDev:
		case dfGpsSatellites:
		case dfPtpMeanPathDelay:
			swprintf_s(buffer, _T("%u"), uint32_t(value));
			break;

		case dfCurrentTimeUart:
		case dfCurrentTimeCan:
		case dfGpsTime:
		case dfPtpTime:
		{
			time_t tTime = time_t(value);
			tm tmTime;
			localtime_s(&tmTime, &tTime);
			swprintf_s(buffer, _T("%02d.%02d.%04d %02d:%02d:%02d"), tmTime.tm_mday, tmTime.tm_mon + 1,
						tmTime.tm_year + 1900, tmTime.tm_hour, tmTime.tm_min, tmTime.tm_sec);
		}
			break;
				
		case dfSyncOffset:
		case dfSyncOffsetMean:
		case dfGpsOffset:
		case dfPtpOffset:
			swprintf_s(buffer, _T("%d"), int32_t(value));
			break;

		case dfSyncState:
			if (uint32_t(value) < syncStateCanParSize)
				wcscpy_s(buffer, syncStateName[uint32_t(value)]);
			break;

		case dfSavedDuty:
		case dfCurrentDuty:
		case dfTemperature:
			swprintf_s(buffer, _T("%.1f"), float(value));
			break;

		case dfGpsState:
			if (uint32_t(value) < gpsStateSensorGpsSize)
				wcscpy_s(buffer, gpsStateName[uint32_t(value)]);
			break;

		case dfPtpState:
			if (uint32_t(value) < ptpStateSensorPtpSize)
				wcscpy_s(buffer, ptpStateName[uint32_t(value)]);
			break;

		case dfPtpSyncState:
			if (uint32_t(value) < ptpSyncStateSensorPtpSize)
				wcscpy_s(buffer, ptpSyncStateName[uint32_t(value)]);
			break;

		case dfPowerQuality:
			if (uint32_t(value) < powerQualityDiagnostSize)
				wcscpy_s(buffer, powerName[uint32_t(value)]);
			break;

		case dfDataQuality:
			if (uint32_t(value) < dataQualityDiagnostSize)
				wcscpy_s(buffer, dataName[uint32_t(value)]);
			break;

		case dfSynchronizationQuality:
			if (uint32_t(value) < syncQualityDiagnostSize)
				wcscpy_s(buffer, syncName[uint32_t(value)]);
			break;

		case dfSensorQuality:
			if (uint32_t(value) < sensorQualityDiagnostSize)
				wcscpy_s(buffer, sensorName[uint32_t(value)]);
			break;

		default:
			swprintf_s(buffer, _T("%d"), int32_t(value));
			break;
	}

	std::wstring sValue(buffer);
	return sValue;
}

template <typename T>
inline static DiagnosticParameterState Parameter2State(DiagnosticParameter parameter, const T& value)
{
	DiagnosticParameterState state(dpsBad);
			
	switch (parameter)
	{
		case dfExchangeCount:
		case dfNoError:
		case dfDataStream:
			if (uint32_t(value) != 0)
				state = dpsGood;
			break;
		case dfNoResponse:
		case dfShortResponse:
		case dfNodeError:
		case dfCommandError:
		case dfCrcError:
		case dfMasterError:
			if (uint32_t(value) == 0)
				state = dpsGood;
			break;
		case dfCurrentTimeUart:
		case dfCurrentTimeCan:
		case dfGpsTime:
		case dfPtpTime:
			if (time_t(value) > 31536000)
				state = dpsGood;
			break;
		case dfSavedDuty:
		case dfCurrentDuty:
			if (value <= 80. && value >= 20.)
				state = dpsGood;
			break;
		case dfSyncOffset:
		case dfGpsOffset:
		case dfPtpOffset:
			if (abs(int64_t(value)) < 2000)
				state = dpsGood;
			break;
		case dfSyncOffsetMean:
			if (abs(int64_t(value)) < 100)
				state = dpsGood;
			break;
		case dfSyncOffsetStdDev:
			if (uint64_t(value) < 400)
				state = dpsGood;
			break;
		case dfSyncState:
			if (uint32_t(value) == 0 || 
				uint32_t(value) == 3)
				state = dpsGood;
			break;
		case dfTemperature:
			if (value != 0.)
				state = dpsGood;
			break;
		case dfGpsState:
			if (uint32_t(value) == 4)
				state = dpsGood;
			break;
		case dfGpsSatellites:
			if (uint32_t(value) >= 2)
				state = dpsGood;
			break;
		case dfPtpState:
			if (uint32_t(value) == 5 ||
				uint32_t(value) == 6 ||
				uint32_t(value) == 9)
				state = dpsGood;
			break;
		case dfPtpSyncState:
			if (uint32_t(value) == 2 ||
				uint32_t(value) == 3)
				state = dpsGood;
			break;
		case dfPowerQuality:
			if (uint32_t(value) == 3)
				state = dpsGood;
			break;
		case dfDataQuality:
			if (uint32_t(value) == 3)
				state = dpsGood;
			break;
		case dfSynchronizationQuality:
			if (uint32_t(value) == 3)
				state = dpsGood;
			break;
		case dfSensorQuality:
			if (uint32_t(value) == 2)
				state = dpsGood;
			break;
		default:
			state = dpsGood;
			break;
	}

	return state;
}

class CDiagnostic : private boost::noncopyable
{
public:
	CDiagnostic();
	virtual ~CDiagnostic();

	virtual void addInfo(_In_ CDiagnosticInfo* pInfo, bool bCount = true);
	virtual void clear();

	void getAddressParameters(_Outref_result_maybenull_ DiagnosticParameter*& pParameters, _Out_ uint32_t& count);

	uint32_t getStatisticSize();
	const DATE* getStatisticTime();
	const float* getStatisticData(uint8_t address, DiagnosticParameter field);

	template <typename T>
	DiagnosticErrorCode getDiagnosticInfo(uint8_t address, DiagnosticParameter field, _Out_ T& info)
	{
		info = T(0);
		auto itNodeInfo = m_nodeInfo.find(address);
		if (itNodeInfo == m_nodeInfo.end())
			return decNoDevice;
		auto itDiagnosticField = itNodeInfo->second.find(field);
		if (itDiagnosticField == itNodeInfo->second.end())
			return decNoParameter;
		info = T(itDiagnosticField->second);
		return decNoError;
	}
	template <typename T>
	DiagnosticErrorCode getDiagnosticInfo(DiagnosticParameter field, _Out_ T& info)
	{
		info = T(0);
		auto itDiagnosticField = m_baseInfo.find(field);
		if (itDiagnosticField == m_baseInfo.end())
			return decNoParameter;
		info = T(itDiagnosticField->second);
		return decNoError;
	}

protected:
	void checkTime();
	void initStatistic();
	void updateStatistic();
	void clearStatistic();

	typedef std::map<DiagnosticParameter, double> NodeInfo, BaseInfo;
	typedef	std::map<uint8_t, NodeInfo> Info;
	Info m_nodeInfo;
			
	typedef std::pair<uint8_t, DiagnosticParameter> StatisticKey;
	std::map<StatisticKey, std::vector<float>> m_nodeStatistic;
			
	std::vector<DiagnosticParameter> m_addressParameters;
			
	double m_tCurrentTime;
	CRITICAL_SECTION m_csInfo;

private:
	void resetHistory();

	BaseInfo m_baseInfo;
	std::vector<DATE> m_statisticTime;
			
	CDiffTimer2 m_timer;

	uint64_t m_exchangeCounter;
	uint64_t m_bytesCounter;
	uint64_t m_streamDataBytesCounter;
	uint64_t m_commandDataBytesCounter;

	double m_deltaTime;
};



