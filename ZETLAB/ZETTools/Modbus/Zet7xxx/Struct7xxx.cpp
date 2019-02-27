#include "stdafx.h"
#include "struct7xxx.h"

Struct7xxx::Struct7xxx()
{
	_mStructData = new char[MAX_SIZE_STRUCT_DATA];
	ZeroMemory(_mStructData, MAX_SIZE_STRUCT_DATA);
	if (_mStructData)
	{
		memset(&_mStructHeader, 0x00, sizeof(_mStructHeader));
		memset(&_mStructData, 0x00, sizeof(_mStructData));
	}
}

Struct7xxx::~Struct7xxx()
{
	if (_mStructData)
	{
		delete [] _mStructData;
		_mStructData = nullptr;

	}
}

Struct7xxx::Struct7xxx(void *pStruct_)
{
	_mStructData = new char[MAX_SIZE_STRUCT_DATA];
	ZeroMemory(_mStructData, MAX_SIZE_STRUCT_DATA);
	if (_mStructData)
	{
		memset(&_mStructHeader, 0x00, sizeof(_mStructHeader));
		memset(&_mStructData, 0x00, sizeof(_mStructData));
		readStruct(pStruct_);
	}
}

void Struct7xxx::readStruct(void *pStruct_)
{
	if (_mStructData)
	{
		_mStructHeader = *(STRUCT_HEAD*)pStruct_;
		memcpy_s(_mStructData, this->getSizeStruct(), (char*)pStruct_ + sizeof(STRUCT_HEAD), this->getSizeStruct());
	}
}

long Struct7xxx::getSizeStruct()
{
	return _mStructHeader.size;
}

long Struct7xxx::getStructType()
{
	return _mStructHeader.struct_type;
}

long Struct7xxx::getStatus()
{
	return _mStructHeader.status;
}

long Struct7xxx::getWriteEnable()
{
	return _mStructHeader.write_enable;
}

long Struct7xxx::getCRC16()
{
	return _mStructHeader.usCRC16;
}

void Struct7xxx::setStatus(long status_)
{
	_mStructHeader.status = status_;
}

void Struct7xxx::setWriteEnable(long writeEnable_)
{
	_mStructHeader.write_enable = writeEnable_;
}

void Struct7xxx::calcCRC16(long long serialNum_)
{
	if (_mStructData)
	{// Calculation and set CRC16 current struct
		unsigned short crc16_(0);

		crc16_ = CRC16(reinterpret_cast<unsigned char*>(&serialNum_), sizeof(serialNum_), short(0xffff));
		crc16_ = CRC16(reinterpret_cast<unsigned char*>(_mStructData), sizeof(STRUCT_HEAD) - 2, crc16_);
		crc16_ = CRC16(reinterpret_cast<unsigned char*>(_mStructData + sizeof(STRUCT_HEAD)), this->getSizeStruct() - sizeof(STRUCT_HEAD), crc16_);

		_mStructHeader.usCRC16 = crc16_;
	}
}

unsigned short CRC16(unsigned char *pBuff_, int len_, short start_value_)
{
	unsigned char	Hi_ = (start_value_ >> 8) & 0xFF;
	unsigned char	Lo_ =  start_value_       & 0xFF;
	unsigned		TableIdx_;
	
	while(len_--)
	{
		TableIdx_ = Hi_ ^ *pBuff_++;
		Hi_ = Lo_ ^ CRCHi[TableIdx_];
		Lo_ = CRCLo[TableIdx_];
	}
	return (Hi_ << 8 | Lo_);
}