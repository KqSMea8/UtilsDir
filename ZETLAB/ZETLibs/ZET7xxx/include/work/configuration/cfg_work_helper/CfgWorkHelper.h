#pragma once

#include <ZET7xxx\include\base\ZET7xxxTypes.h>

using namespace zet7xxx;
using namespace modbus;

#pragma warning( disable : 4290 )

class CConfigurationWorkHelper
{
public:
	CConfigurationWorkHelper(_In_ const Configuration& configuration) throw (std::exception);
	~CConfigurationWorkHelper();

	/// Опрос серийного номера датчика из структуры DEVICE_PAR
	/**	Серийный номер датчика хранится в структуре DEVICE_PAR
	*
	*	@returns Серийный номер датчика
	*/
	uint64_t getSerialDevicePar();
	/// Опрос типа датчика из структуры DEVICE_PAR
	/**	Тип датчика хранится в структуре DEVICE_PAR
	*
	*	@returns Тип датчика
	*/
	int32_t getTypeDevicePar();
	uint32_t getAddressDevicePar();

	int32_t getBaudrateCanPar();
	int32_t getCurrentTimeCanPar();
	int64_t getSyncOffsetCanPar();
	int32_t getSyncStateCanPar();
	
	int32_t getBaudrateUartPar();
	int32_t getParityUartPar();
	int32_t getCurrentTimeUartPar();
	
	__time32_t getCompileTimeDevicePar();

	void getTimePar(_Out_ TIME_PAR& timePar);

	void getChannelPar(_In_ const uint32_t index, _Out_ CHANNEL_PAR& channelPar);
	void getValueAddressesChannelPar(_Outref_result_maybenull_ ModbusDataAddress*& pAddresses, _Out_ uint32_t& count);
	
	void getValueAddressesParamPar(_Outref_result_maybenull_ ModbusDataAddress*& pAddresses, _Out_ uint32_t& count);
	void getValueAddressesMessagePar(_Outref_result_maybenull_ ModbusDataAddress*& pAddresses, _Out_ uint32_t& count);

	float getRangeAdcPar2();

	float getSavedDutyDiagnost();
	float getCurrentDutyDiagnost();
	int32_t getPowerQualityDiagnost();
	int32_t getDataQualityDiagnost();
	int32_t getSynchronizationQualityDiagnost();
	int32_t getSensorQualityDiagnost();
	float getTemperatureDiagnost();

	int32_t getSyncStateSensorGps();
	int32_t getGpsTimeSensorGps();
	int32_t getPpsOffsetSensorGps();
	int32_t getSatellitesSensorGps();

	int32_t getPtpStateSensorPtp();
	int32_t getPtpTimeSensorPtp();
	int32_t getPtpOffsetSensorPtp();
	int32_t getPtpSyncStateSensorPtp();
	int32_t getPtpMeanDelaySensorPtp();

	uint32_t getSourceAddrMessagePar(_In_ const uint32_t index);

	void getStructure(_Out_ STRUCT_TYPE& type, _Inout_ uint16_t& offset, _Out_ uint16_t& size);
	void getStructureParams(STRUCT_TYPE type, _Inout_ uint16_t& offset, _Out_ uint16_t& size);

protected:
	template<typename T>
	T getValueFromStructure(STRUCT_TYPE type, uint16_t valueOffset, uint16_t valueSize)
	{
		uint16_t* pStructure(nullptr);
		uint16_t offset(0), size(0);
		T value(T(0));

		getStructure(type, pStructure, offset, size);
		if (pStructure != nullptr && size > 0)
		{
			if (size >= valueOffset + valueSize)
			{
				uint16_t valSize = sizeof(value) < valueSize * 2 ? sizeof(value) : valueSize * 2;
				memcpy_s(&value, valSize, pStructure + valueOffset, valSize);
			}
		}

		return value;
	}
	template<typename T>
	T getValueFromStructure(STRUCT_TYPE type, _In_ const uint32_t index, uint16_t valueOffset, uint16_t valueSize)
	{
		uint16_t* pStructure(nullptr);
		uint16_t offset(0), size(0);
		T value(T(0));
		uint16_t structureIndex(0);

		do
		{
			offset += size;
			pStructure = nullptr;
			size = 0;
			getStructure(type, pStructure, offset, size);
			if (pStructure != nullptr && size > 0)
			{
				++structureIndex;
				if (size >= valueOffset + valueSize)
				{
					uint16_t valSize = sizeof(value) < valueSize * 2 ? sizeof(value) : valueSize * 2;
					memcpy_s(&value, valSize, pStructure + valueOffset, valSize);
				}
			}
		} while (pStructure != nullptr && size > 0 && index >= structureIndex);

		return value;
	}
private:
	void getStructure(STRUCT_TYPE type, _Outref_result_maybenull_ uint16_t*& pStructure, _Inout_ uint16_t& offset, _Out_ uint16_t& size);

	Configuration m_configuration;
};

