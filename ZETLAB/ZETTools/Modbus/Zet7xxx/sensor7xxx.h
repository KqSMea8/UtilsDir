#pragma once
#include "struct7xxx.h"
#include <vector>

class Sensor7xxx
{
public:
	std::vector <Struct7xxx> _mSensorDevStructs;
	std::vector <Struct7xxx> _mSensorDefStructs;

	long _sensorType;
	long long _sensorSerialNum;
	long _sensorNode;

	long _virtchan;	// номер virtchan в MODBUSZETLAB
	long _ftdind;	// номер линии FTDI в MODBUSZETLAB
	long _numChannels;

public:
	Sensor7xxx();
	Sensor7xxx(void *pDevSensor_, long devLen_, void *pDefSensor_, long defLen_);

	void readStructs(void *pDevSensor_, long devLen_, void *pDefSensor_, long defLen_);
	void writeStructs(void *pDevSensor_, long *devLen_, void *pDefSensor, long *defLen);
	void setVirtChan(long virtchan_);
	long getVirtChan(void);
	void setFTDIindex(long ftdindex_);
	long getFTDIindex(void);
	void* getDefStruct(long typeStruct_, long num_, Struct7xxx *structure_);
	void* getDevStruct(long typeStruct_, long num_, Struct7xxx *structure_);
	long getSensorType();
	long long getSensorSerialNum();
	long getSensorNode();
	void clearStructs();

	long getNumChannels(void);

	~Sensor7xxx();

};