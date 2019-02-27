#include "circBuffBasicZetLogger.h"
#include <memory.h>

CCircBuffBasicZetLogger::CCircBuffBasicZetLogger()
{
	_circBuffStruct = nullptr;
}

CCircBuffBasicZetLogger::CCircBuffBasicZetLogger(CircBuffBasicZetLoggerStruct* circBuffStruct_)
{
	_circBuffStruct = circBuffStruct_;
}

CCircBuffBasicZetLogger::~CCircBuffBasicZetLogger()
{
}

bool CCircBuffBasicZetLogger::isValid()
{
	return (_circBuffStruct != nullptr);
}

void CCircBuffBasicZetLogger::reset()
{
	if (isValid())
	{
		_circBuffStruct->_posWriteCircBuff = 0;
		_circBuffStruct->_posReadCircBuff = 0;
	}
}

long CCircBuffBasicZetLogger::writeStructures(char* structures_, long size_)
{
	long wrSize = ErrorCodes::FatalError;

	if (isValid())
	{
		wrSize = 0;
		if (getSizeWrite() >= size_)   // Если в циклическом буфере есть необходимое количество свободного места для ввода данных
		{
			wrSize = size_;
			long sizeToEndBuff = ZETLOGGER_CIRC_BUFF_SIZE - _circBuffStruct->_posWriteCircBuff;   // Размер свободного места в цикл. буфере до записи по новому кругу

			if (sizeToEndBuff >= wrSize)	// Если данные можно положить последовательно за раз
			{
				memcpy_s(&_circBuffStruct->_buffer[_circBuffStruct->_posWriteCircBuff], wrSize, &structures_[0], wrSize);
				_circBuffStruct->_posWriteCircBuff += wrSize;
			}
			else   // если данные необходимо поделить (часть записывается по новому кругу)
			{
				memcpy_s(&_circBuffStruct->_buffer[_circBuffStruct->_posWriteCircBuff], sizeToEndBuff, &structures_[0], sizeToEndBuff);
				memcpy_s(&_circBuffStruct->_buffer[0], wrSize - sizeToEndBuff, &structures_[sizeToEndBuff], wrSize - sizeToEndBuff);
				_circBuffStruct->_posWriteCircBuff = wrSize - sizeToEndBuff;
			}
		}
	}

	return wrSize;
}

long CCircBuffBasicZetLogger::readStructures(char* structures_, long size_)
{
	long rdSize = ErrorCodes::FatalError;

	if (isValid())
	{
		rdSize = 0;
		if (getSizeRead() >= size_)	// Если в циклическом буфере есть необходимое количество данных для чтения
		{
			rdSize = size_;
			long sizeToEndBuff = ZETLOGGER_CIRC_BUFF_SIZE - _circBuffStruct->_posReadCircBuff;   // Размер данных в цикл. буфере до чтения по новому кругу

			if (sizeToEndBuff >= rdSize)   // Если данные можно взять последовательно за раз
			{
				memcpy_s(&structures_[0], rdSize, &_circBuffStruct->_buffer[_circBuffStruct->_posReadCircBuff], rdSize);
				_circBuffStruct->_posReadCircBuff += rdSize;
			}
			else   // Если данные необходимо брать кусками (часть была записана по новому кругу)
			{
				memcpy_s(&structures_[0], sizeToEndBuff, &_circBuffStruct->_buffer[_circBuffStruct->_posReadCircBuff], sizeToEndBuff);
				memcpy_s(&structures_[sizeToEndBuff], rdSize - sizeToEndBuff, &_circBuffStruct->_buffer[0], rdSize - sizeToEndBuff);
				_circBuffStruct->_posReadCircBuff = rdSize - sizeToEndBuff;
			}
		}
	}

	return rdSize;
}

long CCircBuffBasicZetLogger::readTempStructures(char* structures_, long size_)
{
	long rdSize = ErrorCodes::FatalError;

	if (isValid())
	{
		rdSize = 0;
		if (getSizeRead() >= size_)	// Если в циклическом буфере есть необходимое количество данных для чтения
		{
			rdSize = size_;
			long sizeToEndBuff = ZETLOGGER_CIRC_BUFF_SIZE - _circBuffStruct->_posReadCircBuff;   // Размер данных в цикл. буфере до чтения по новому кругу

			if (sizeToEndBuff >= rdSize)   // Если данные можно взять последовательно за раз
			{
				memcpy_s(&structures_[0], rdSize, &_circBuffStruct->_buffer[_circBuffStruct->_posReadCircBuff], rdSize);
			}
			else   // Если данные необходимо брать кусками (часть была записана по новому кругу)
			{
				memcpy_s(&structures_[0], sizeToEndBuff, &_circBuffStruct->_buffer[_circBuffStruct->_posReadCircBuff], sizeToEndBuff);
				memcpy_s(&structures_[sizeToEndBuff], rdSize - sizeToEndBuff, &_circBuffStruct->_buffer[0], rdSize - sizeToEndBuff);
			}
		}
	}

	return rdSize;
}

long CCircBuffBasicZetLogger::getSizeRead()
{
	long rdSize = 0;

	if (isValid())
	{
		rdSize = _circBuffStruct->_posWriteCircBuff - _circBuffStruct->_posReadCircBuff;
		if (rdSize < 0)
			rdSize = rdSize + ZETLOGGER_CIRC_BUFF_SIZE;
	}

	return rdSize;
}

long CCircBuffBasicZetLogger::getSizeWrite()
{
	long wrSize = 0;

	if (isValid())
		wrSize = ZETLOGGER_CIRC_BUFF_SIZE - getSizeRead() - 1;

	return wrSize;
}