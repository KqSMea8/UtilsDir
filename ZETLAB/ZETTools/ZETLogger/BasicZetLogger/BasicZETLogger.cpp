#include "basicZetLogger.h"

CBasicZetLogger::CBasicZetLogger()
{
	_basicZETLogger = nullptr;
}

CBasicZetLogger::CBasicZetLogger(_In_ BasicZetLoggerStruct* basicZETLogger_)
{
	_basicZETLogger = basicZETLogger_;
	_circBuff = CCircBuffBasicZetLogger(&basicZETLogger_->_circBuff);
}

CBasicZetLogger::~CBasicZetLogger()
{

}

void CBasicZetLogger::init(_In_ const ZetHeader* structureHeader_)
{
	if (isValid())
	{
		reset();

		_basicZETLogger->_type = structureHeader_->type;
		_basicZETLogger->_size = structureHeader_->size;
		_basicZETLogger->_timeHourId = convertTimeStampToTimeHourId(structureHeader_->seconds);

		_basicZETLogger->_deviceId = structureHeader_->device_id;
		_basicZETLogger->_moduleId = structureHeader_->module_id;
		_basicZETLogger->_processId = structureHeader_->process_id;
		_basicZETLogger->_channelId = structureHeader_->channel_id;
	}
}

void CBasicZetLogger::reset()
{
	if (isValid())
	{
		_basicZETLogger->_type = 0;
		_basicZETLogger->_size = 0;
		_basicZETLogger->_timeHourId = 0;

		_basicZETLogger->_deviceId = 0;
		_basicZETLogger->_moduleId = 0;
		_basicZETLogger->_processId = 0;
		_basicZETLogger->_channelId = 0;

		updateLive();
		updateWaitTime();
		_circBuff.reset();
	}
}

void CBasicZetLogger::updateLive()
{
	if (isValid())
	{
		_basicZETLogger->_livetick = BASICZETLOGGER_MAX_LIVETICK;
	}
}

void CBasicZetLogger::updateWaitTime()
{
	if (isValid())
	{
		_basicZETLogger->_waittick = 0;
	}
}

void CBasicZetLogger::runingLiveAndWaitTime()
{
	if (isValid())
	{
		if (!isFree())	// Если ячейка не пустая, обновляем её статус жизни и время ожидания для сброса буфера накопленных данных
		{
			if (_basicZETLogger->_livetick-- < 0)
				_basicZETLogger->_livetick = 0;

			if (_basicZETLogger->_waittick++ > BASICZETLOGGER_MAX_WAITTICK)
				_basicZETLogger->_waittick = BASICZETLOGGER_MAX_WAITTICK;
		}
	}
}

void CBasicZetLogger::getType(_Out_ uint32_t & type_)
{
	type_ = isValid() ? _basicZETLogger->_type : 0;
}

void CBasicZetLogger::getUniqueId(_Out_ uint64_t & deviceId_, _Out_ uint32_t & moduleId_, _Out_ uint16_t & processId_, _Out_ uint16_t & channelId_)
{
	deviceId_ = isValid() ? _basicZETLogger->_deviceId : 0;
	moduleId_ = isValid() ? _basicZETLogger->_moduleId : 0;
	processId_ = isValid() ? _basicZETLogger->_processId : 0;
	channelId_ = isValid() ? _basicZETLogger->_channelId : 0;
}

void CBasicZetLogger::getTimeHourId(_Out_ uint64_t & timeHourId_)
{
	timeHourId_ = isValid() ? _basicZETLogger->_timeHourId : 0;
}

void CBasicZetLogger::getStructureSize(_Out_ uint32_t & structureSize_)
{
	structureSize_ = isValid() ? _basicZETLogger->_size : 0;
}

uint32_t CBasicZetLogger::getStructureSize()
{
	return isValid() ? _basicZETLogger->_size : 0;
}

bool CBasicZetLogger::isValid()
{
	return (_basicZETLogger != nullptr) && (_circBuff.isValid());
}

bool CBasicZetLogger::isLive()
{
	return !isFree() && (isValid() ? (_basicZETLogger->_livetick > 0) : false);
}

bool CBasicZetLogger::isDied()
{
	return !isFree() && (isValid() ? (_basicZETLogger->_livetick == 0) : false);
}

bool CBasicZetLogger::isWaitTimeFinished()
{
	return !isFree() && (isValid() ? (_basicZETLogger->_waittick >= BASICZETLOGGER_MAX_WAITTICK) : false);
}

bool CBasicZetLogger::isFree()
{
	return isValid() ? (_basicZETLogger->_type == 0) && (_basicZETLogger->_timeHourId == 0) && 
		      (_basicZETLogger->_deviceId == 0) && (_basicZETLogger->_moduleId == 0) &&
				 (_basicZETLogger->_processId == 0) && (_basicZETLogger->_channelId == 0) : false;
}

bool CBasicZetLogger::isNeedType(uint32_t type_)
{
	return isValid() ? (_basicZETLogger->_type == type_) : false;
}

bool CBasicZetLogger::isNeedDeviceId(uint64_t deviceId_)
{
	return isValid() ? (_basicZETLogger->_deviceId == deviceId_) : false;
}

bool CBasicZetLogger::isNeedModuleId(uint32_t moduleId_)
{
	return isValid() ? (_basicZETLogger->_moduleId == moduleId_) : false;
}

bool CBasicZetLogger::isNeedProcessId(uint16_t processId_)
{
	return isValid() ? (_basicZETLogger->_processId == processId_) : false;
}

bool CBasicZetLogger::isNeedChannelId(uint16_t channelId_)
{
	return isValid() ? (_basicZETLogger->_channelId == channelId_) : false;
}

bool CBasicZetLogger::isNeedId(uint64_t deviceId_, uint32_t moduleId_, uint16_t processId_, uint16_t channelId_)
{
	return isValid() ? (_basicZETLogger->_deviceId == deviceId_) && (_basicZETLogger->_moduleId == moduleId_) && (_basicZETLogger->_processId == processId_) &&
		      (_basicZETLogger->_channelId == channelId_) : false;
}

bool CBasicZetLogger::isNeedTypeAndId(uint32_t type_, uint64_t deviceId_, uint32_t moduleId_, uint16_t processId_, uint16_t channelId_)
{
	return isValid() ? (_basicZETLogger->_type == type_) && (_basicZETLogger->_deviceId == deviceId_) &&
		      (_basicZETLogger->_moduleId == moduleId_) && (_basicZETLogger->_processId == processId_) && (_basicZETLogger->_channelId == channelId_) : false;
}

bool CBasicZetLogger::isNeedTimeHourId(uint64_t timeHourId_)
{
	return isValid() ? _basicZETLogger->_timeHourId == convertTimeStampToTimeHourId(timeHourId_) : false;
}


bool CBasicZetLogger::isNeedStructureHeader(const ZetHeader* structureHeader_)
{
	return isValid() ? (_basicZETLogger->_type == structureHeader_->type) && (_basicZETLogger->_timeHourId == convertTimeStampToTimeHourId(structureHeader_->seconds)) &&
			  (_basicZETLogger->_deviceId == structureHeader_->type) && (_basicZETLogger->_moduleId == structureHeader_->module_id) &&
				 (_basicZETLogger->_processId == structureHeader_->process_id) && (_basicZETLogger->_channelId == structureHeader_->channel_id) : false;
}

long CBasicZetLogger::writeStructure(char* structure_, long size_)
{
	updateLive();

	return isValid() ? _circBuff.writeStructures(structure_, size_) : ErrorCodes::FatalError;
}

long CBasicZetLogger::readStructures(char* structure_, long size_)
{
	return isValid() ? _circBuff.readStructures(structure_, size_) : ErrorCodes::FatalError;
}

long CBasicZetLogger::readTempStructures(char* structure_, long size_)
{
	return isValid() ? _circBuff.readTempStructures(structure_, size_) : ErrorCodes::FatalError;
}

long CBasicZetLogger::getSizeWrite()
{
	return isValid() ? _circBuff.getSizeWrite() : 0;
}

long CBasicZetLogger::getSizeRead()
{
	return isValid() ? _circBuff.getSizeRead() : 0;
}

uint64_t CBasicZetLogger::convertTimeStampToTimeHourId(uint64_t timeStamp_)	// Так как время UTC, все нормально
{
	return (timeStamp_ / CONST_SECONDS_IN_HOUR) * CONST_SECONDS_IN_HOUR;
}