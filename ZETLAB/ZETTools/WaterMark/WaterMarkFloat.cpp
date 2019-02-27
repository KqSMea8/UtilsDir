#include "CRC.h"
#include "WatermarkFloat.h"

long WMF_PutWatermark(float* pBuffer, long lSize, long lRTC, unsigned char ucQuality, long lStartWatermark)
{
	if(lSize <= 0) 
		return -1;

	long lIndex(0), lWatermarkIndex(0);
	long lRTCbit(lRTC), lCRC8(0);
	unsigned char ucQualityBit(ucQuality);
	long* lPointer((long*)pBuffer);

	if (lStartWatermark <= lWatermarkIndex)
	{
		//стартовый бит
		lPointer[0] = lPointer[0] | 0x1;
		++lPointer;
		++lIndex;
		if (lIndex >= lSize)
			return -2;
	}
	++lWatermarkIndex;

	//метка времени
	lCRC8 = GetCrc8((char*)&lRTCbit, sizeof(long) / sizeof(char), CRC_START_VAL);
	for(int i = 0; i < SIZEOFTIMELABEL; ++i)
	{
		if (lStartWatermark <= lWatermarkIndex)
		{
			lPointer[0] = lPointer[0] & (~0x1);
			lPointer[0] = lPointer[0] | (lRTCbit & 0x1);
			++lPointer;
			++lIndex;
			if (lIndex >= lSize)
				return -3;
		}
		++lWatermarkIndex;
		lRTCbit = lRTCbit >> 1;
	}

	//качество сигнала
	lCRC8 = GetCrc8((char*)&ucQualityBit, sizeof(ucQualityBit), char(lCRC8));
	for(int i = 0; i < SIZEOFQUALITY * 4; ++i)
	{
		if (lStartWatermark <= lWatermarkIndex)
		{
			lPointer[0] = lPointer[0] & (~0x1);
			lPointer[0] = lPointer[0] | (ucQualityBit & 0x1);
			++lPointer;
			++lIndex;
			if (lIndex >= lSize)
				return -4;
		}
		++lWatermarkIndex;
		ucQualityBit = ucQualityBit >> 1;
	}
	//контрольная сумма
	for(int i = 0; i < SIZEOFCRC; ++i)
	{
		if (lStartWatermark <= lWatermarkIndex)
		{
			lPointer[0] = lPointer[0] & (~0x1);
			lPointer[0] = lPointer[0] | (lCRC8 & 0x1);
			++lPointer;
			++lIndex;
			if (lIndex >= lSize)
				return -5;
		}
		++lWatermarkIndex;
		lCRC8 = lCRC8 >> 1;
	}

	//стоповый бит
	if (lStartWatermark <= lWatermarkIndex)
	{
		lPointer[0] = lPointer[0] | 0x1;
		++lPointer;
		++lIndex;
	}
	++lWatermarkIndex;

	if (lSize - lIndex > 0)
		WMF_ClearBit(&pBuffer[lIndex], lSize - lIndex);

	return 0;
}
/////////////////////////////////////////////////////////////////
long WMF_ClearBit(float* pBuffer, long lSize)
{
	if(lSize <= 0) 
		return -1;

	long* lPointer((long*)pBuffer);

	for(int i = 0; i < lSize; ++i)
	{
		lPointer[i] = lPointer[i] & (~0x1);
	}

	return 0;
}
//////////////////////////////////////////////////////////////////
long WMF_FindPPS( float* pBuffer, long lSize, long* lAdress, long* lRTC, unsigned char* ucQuality)
{
	*lAdress = -1;
	*lRTC = -1;
	*ucQuality = 0;

	if(lSize <= 0) 
		return -1;
	//if(lSize < SIZEOFWMF * 2)
	//	return -2;

	long lPrisnak(0), lZeroCounter(0), lIndex(0), lRTCBit(0);
	char cCRC(0), cResultCRC(0), cQualityBit(0);
	long *lPointer((long*)pBuffer);

	//поиск стартового бита
	while(lPrisnak == 0)
	{
		lPrisnak = (lPointer[0] & 0x1);
		if(lPrisnak == 0) 
			++lZeroCounter;
		++lPointer;
		++lIndex;
		if(lIndex >= lSize) 
			return -3;
	}
	*lAdress = lIndex - 1;
	//TRACE("WaterMark Start bit found at %d\n", *lAdress);

	//разбор метки времени
	for(int i = 0; i < sizeof(long) * 8; ++i)
	{
		lPrisnak = lPointer[0] & 0x1;
		lPrisnak = lPrisnak << i;
		lRTCBit = lRTCBit | lPrisnak;
		++lPointer;
		++lIndex;
		if(lIndex >= lSize) 
			return -4;
	}
	*lRTC = lRTCBit;

	//разбор качества сигнала
	for(int i = 0; i < SIZEOFQUALITY * 4; ++i)
	{
		lPrisnak = lPointer[0] & 0x1;
		lPrisnak = lPrisnak << i;
		cQualityBit |= lPrisnak;
		++lPointer;
		++lIndex;
		if(lIndex >= lSize) 
			return -5;
	}
	*ucQuality = static_cast< unsigned char >(cQualityBit);

	//разбор контрольной суммы
	for(int i = 0; i < sizeof(unsigned char) * 8; ++i)
	{
		lPrisnak = lPointer[0] & 0x1;
		lPrisnak = (lPrisnak << i) & 0xff;
		cCRC = cCRC | char(lPrisnak);
		++lPointer;
		++lIndex;
		if(lIndex >= lSize)
			return -6;
	}
	//подсчет присланной контрольной суммы и проверка на совпадение контрольных сумм
	cResultCRC = GetCrc8((char*)&lRTCBit, sizeof(long) / sizeof(char), CRC_START_VAL);
	cResultCRC = GetCrc8(&cQualityBit, sizeof(char), cResultCRC);
	if(cResultCRC != cCRC) 
		return -7;
	
	//проверка стопового бита
	lPrisnak = lPointer[0] & 0x1;
	if(lPrisnak == 0) 
		return -8;

	//проверка нулей
	++lPointer;
	++lIndex;
	if(lIndex >= lSize)
	{
		if (lZeroCounter < SIZEOFWMF)
			return -9;
	}

	lPrisnak = 0;
	while(lPrisnak == 0)
	{
		lPrisnak = (lPointer[0] & 0x1);
		if(lPrisnak == 0) 
			++lZeroCounter;
		++lPointer;
		++lIndex;
		if(lIndex >= lSize)
			break;
	}
	if(lZeroCounter < SIZEOFWMF) 
		return -10;

	return 0;
}

long WMF_FindFirstPPS(float* pBuffer, long lSize, long* lAdress, long* lRTC, unsigned char* ucQuality)
{
	long lPosition(0);
	while (lPosition < lSize)
	{
		long result = WMF_FindPPS(pBuffer + lPosition, lSize - lPosition, lAdress, lRTC, ucQuality);
		if (result == 0)
		{
			*lAdress += lPosition;
			return result;
		}
		++lPosition;
	}

	return -1;
}

long WMF_FindLastPPS(float* pBuffer, long lSize, long* lAdress, long* lRTC, unsigned char* ucQuality)
{
	long lPosition(lSize);
	while (lPosition > 0)
	{
		lPosition--;
		long result = WMF_FindPPS(pBuffer + lPosition, lSize - lPosition, lAdress, lRTC, ucQuality);
		if (result == 0)
		{
			*lAdress += lPosition;
			return result;
		}
	}

	return -1;
}
