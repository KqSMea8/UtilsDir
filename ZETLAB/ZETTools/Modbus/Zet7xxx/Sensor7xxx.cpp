#include "stdafx.h"
#include "sensor7xxx.h"

Sensor7xxx::Sensor7xxx()
{
	_mSensorDevStructs.clear();
	_mSensorDefStructs.clear();

	_sensorSerialNum = 0;
	_sensorType = 0;
	_sensorNode = 0;

	_virtchan = -1;
	_ftdind = -1;

	_numChannels = 0;
}

Sensor7xxx::Sensor7xxx(void *pDevSensor_, long devLen_, void *pDefSensor_, long defLen_)
{
	_mSensorDevStructs.clear();
	_mSensorDefStructs.clear();

	_sensorSerialNum = 0;
	_sensorType = 0;
	_sensorNode = 0;

	_virtchan = -1;
	_ftdind = -1;

	_numChannels = 0;

	readStructs(pDevSensor_, devLen_, pDefSensor_, defLen_);
}

void Sensor7xxx::clearStructs()
{
	_mSensorDefStructs.clear();
	_mSensorDevStructs.clear();
}

void Sensor7xxx::readStructs(void *pDevSensor_, long devLen_, void *pDefSensor_, long defLen_)
{
	_mSensorDevStructs.clear();
	_mSensorDefStructs.clear();

	_numChannels = 0;

	long devPos_ = 0;
	long defPos_ = 0;

	Struct7xxx structure_;

	//TRACE ("Read structs: devLen: %d, defLen: %d\n", devLen_, defLen_);

	while ((devPos_ < devLen_) && (defPos_ < defLen_))
	{
		STRUCT_HEAD* pHeadDev_ = reinterpret_cast<STRUCT_HEAD*>((char*)pDevSensor_ + devPos_);
		STRUCT_HEAD* pHeadDef_ = reinterpret_cast<STRUCT_HEAD*>((char*)pDefSensor_ + defPos_);

		// Skip if types of structs differs
		if (pHeadDef_->struct_type != pHeadDev_->struct_type)
		{
			devPos_ += pHeadDev_->size ? pHeadDev_->size : sizeof(STRUCT_HEAD);

			structure_.readStruct((void*)pHeadDev_);
			_mSensorDevStructs.push_back(structure_);

			continue;
		}

		structure_.readStruct((void*)pHeadDev_);
		_mSensorDevStructs.push_back(structure_);

		if (structure_.getStructType() == DEV_PAR_STRUCT)
		{
			_sensorSerialNum = ((DEVICE_PAR*)((char*)pHeadDev_ + sizeof(STRUCT_HEAD)))->serial_num;
			_sensorNode = ((DEVICE_PAR*)((char*)pHeadDev_ + sizeof(STRUCT_HEAD)))->dev_addr;
			_sensorType = ((DEVICE_PAR*)((char*)pHeadDev_ + sizeof(STRUCT_HEAD)))->type;
		}

		if (structure_.getStructType() == CHANNEL_PAR_STRUCT)
		{
			_numChannels ++;
		}

		structure_.readStruct((void*)pHeadDef_);
		_mSensorDefStructs.push_back(structure_);

		if (pHeadDef_->size == 0)
		{
			defPos_ += sizeof(STRUCT_HEAD);
			devPos_ += sizeof(STRUCT_HEAD);
		}
		else
		{
			defPos_ += pHeadDef_->size;	// Skip to next struct
			devPos_ += pHeadDev_->size;
		}
	}

	//TRACE ("Current num dev structs: %d, current num def structs: %d\n", _mSensorDefStructs.size(), _mSensorDevStructs.size());
}

void Sensor7xxx::writeStructs(void *pDevSensor_, long *devLen_, void *pDefSensor_, long *defLen_)
{
	long devPos_ = 0;
	long defPos_ = 0;

	Struct7xxx structure_;
	long offset = 0;

	for (unsigned int currStruct = 0; currStruct < _mSensorDefStructs.size(); currStruct++)
	{
		//if (_mSensorDefStructs[currStruct].getStructType() == ADC_PAR_STRUCT)
			memcpy_s((BYTE*)pDefSensor_ + offset, sizeof(STRUCT_HEAD), &_mSensorDefStructs[currStruct]._mStructHeader, sizeof(STRUCT_HEAD));
		offset += sizeof(STRUCT_HEAD);
		
		if (_mSensorDefStructs[currStruct].getSizeStruct() != 0)
		{
			//if (_mSensorDefStructs[currStruct].getStructType() == ADC_PAR_STRUCT)
				memcpy_s((BYTE*)pDefSensor_ + offset, _mSensorDefStructs[currStruct].getSizeStruct() - sizeof(STRUCT_HEAD), _mSensorDefStructs[currStruct]._mStructData, _mSensorDefStructs[currStruct].getSizeStruct() - sizeof(STRUCT_HEAD));
			offset += (_mSensorDefStructs[currStruct].getSizeStruct() - sizeof(STRUCT_HEAD));
		}	
	}

	offset = 0;
	for (unsigned int currStruct = 0; currStruct < _mSensorDevStructs.size(); currStruct++)
	{
		//if (_mSensorDevStructs[currStruct].getStructType() == ADC_PAR_STRUCT)
			memcpy_s((BYTE*)pDevSensor_ + offset, sizeof(STRUCT_HEAD), &_mSensorDevStructs[currStruct]._mStructHeader, sizeof(STRUCT_HEAD));
		offset += sizeof(STRUCT_HEAD);
		if (_mSensorDevStructs[currStruct].getSizeStruct() != 0)
		{
			//if (_mSensorDevStructs[currStruct].getStructType() == ADC_PAR_STRUCT)
				memcpy_s((BYTE*)pDevSensor_ + offset, _mSensorDevStructs[currStruct].getSizeStruct() - sizeof(STRUCT_HEAD), _mSensorDevStructs[currStruct]._mStructData, _mSensorDevStructs[currStruct].getSizeStruct() - sizeof(STRUCT_HEAD));
			offset += (_mSensorDevStructs[currStruct].getSizeStruct() - sizeof(STRUCT_HEAD));
		}
	}
}

void* Sensor7xxx::getDefStruct(long structType_, long num_, Struct7xxx* structure_)	
{
	long numStruct_ = 0;
	void* pointer_ = NULL;

	for (unsigned int currStruct = 0; currStruct < _mSensorDefStructs.size(); currStruct ++)
	{
		if (_mSensorDefStructs[currStruct].getStructType() == structType_)
		{
			if (num_ == numStruct_)
			{
				if (structure_ != NULL)
					*structure_ = _mSensorDefStructs[currStruct];
				pointer_ = (void*)_mSensorDefStructs[currStruct]._mStructData;
				break;
			}
			else
			{
				numStruct_ ++;
			}
		}
	}

	return pointer_;
}

void* Sensor7xxx::getDevStruct(long structType_, long num_, Struct7xxx* structure_)	
{
	long numStruct_ = 0;
	void* pointer_ = 0;

	for (unsigned int currStruct = 0; currStruct < _mSensorDevStructs.size(); currStruct ++)
	{
		if (_mSensorDevStructs[currStruct].getStructType() == structType_)
		{
			if (num_ == numStruct_)
			{
				if (structure_ != NULL)
					*structure_ = &_mSensorDevStructs[currStruct];
				pointer_ = (void*)&_mSensorDevStructs[currStruct];
				break;
			}
			else
			{
				numStruct_ ++;
			}
		}
	}

	return pointer_;
}

/*long getFTDIindex(void)
{
	return _ftdind;
}*/

long Sensor7xxx::getSensorNode()
{
	return _sensorNode;
}

long long Sensor7xxx::getSensorSerialNum()
{
	return _sensorSerialNum;
}

long Sensor7xxx::getSensorType()
{
	return _sensorType;
}

Sensor7xxx::~Sensor7xxx()
{
	_mSensorDevStructs.clear();
	_mSensorDefStructs.clear();
}

void Sensor7xxx::setFTDIindex(long ftdindex_)
{
	_ftdind = ftdindex_;
}

long Sensor7xxx::getFTDIindex(void)
{
	return _ftdind;
}

void Sensor7xxx::setVirtChan(long virtchan_)
{
	_virtchan = virtchan_;
}

long Sensor7xxx::getVirtChan(void)
{
	return _virtchan;
}

long Sensor7xxx::getNumChannels(void)
{
	return _numChannels;
}