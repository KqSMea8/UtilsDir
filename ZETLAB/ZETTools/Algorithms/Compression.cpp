//----------------------------------------------------------------------------------------------------------------------
#include "StdAfx.h"
#include <Algorithms\Compression.h>
//----------------------------------------------------------------------------------------------------------------------
int32_t getDecompressSize(const uint16_t sizeCode, _In_ const uint32_t inSize)
{
	if (sizeCode == 0)
		return -1;
	if (sizeCode > 24)
		return -2;

	return inSize * 8 / sizeCode + 1;
}
//----------------------------------------------------------------------------------------------------------------------
int32_t decompress(_In_ const COMPRESSED parameters, _In_ const uint32_t val0, _In_ const uint16_t sizeCode,
	_In_ uint8_t* pDataIn, _In_ const uint32_t inSize, _Out_ float* pDataOut, _In_ const uint32_t outSize)
{
	//TRACE("Decompress: bit_offset %d, val0 %d, sizeCode %d, inSize %d\n", parameters.bit_offset, val0, sizeCode, inSize);
	if (inSize == 0)
		return -1;
	if (nullptr == pDataIn)
		return -2;
	if (sizeCode == 0)
		return -3;
	if (sizeCode > 24)
		return -4;
	if (sizeCode + parameters.bit_offset > 24)
		return -5;
	if (outSize == 0)
		return -6;
	if (nullptr == pDataOut)
		return -7;

	uint32_t i = 0;
	int32_t prevVal = (val0 << parameters.bit_offset);
	if (prevVal & 0x800000)
	{
		prevVal &= ~0x800000;
		prevVal *= -1;
	}

	pDataOut[i++] = prevVal == 0 ? (float)(prevVal + 1) * parameters.coef_k  : (float)prevVal * parameters.coef_k;

	int32_t counter(0);
	int8_t bit = 0;
	int8_t byte = 3;
	uint8_t j = 0;
	uint32_t current(0);
	int32_t* delta = reinterpret_cast<int32_t*>(&current);
	bool bNegative(false);

	//CString sOutput;

	while (1)
	{
		uint32_t val(0);
		int32_t tail = (int32_t)inSize - counter * sizeof(uint32_t);
		if (tail <= 0)
			break;
		else if (tail < sizeof(uint32_t))
		{
			//sOutput = L"";
			uint8_t tempData[sizeof(uint32_t)];
			for (uint32_t k = 0; k < sizeof(uint32_t); ++k)
			{
				if (k < (uint32_t)tail)
				{
					tempData[sizeof(uint32_t) - k - 1] = pDataIn[counter * sizeof(uint32_t) + k];
					//sOutput.AppendFormat(L"%02x ", pDataIn[counter * sizeof(uint32_t) + k]);
				}
				else
					tempData[sizeof(uint32_t) - k - 1] = 0;
			}
			//OutputDebugString(sOutput);
			val = *(reinterpret_cast<uint32_t*>(tempData));
		}
		else
			val = *(reinterpret_cast<uint32_t*>(pDataIn)+counter);

		while (1)
		{
			//if (j == 0)
			//	sOutput.Format(L"%d Value start from\t%d bit\t%d byte of\t0x%08x\t\t\t\t", i, bit, byte, val);

			int32_t byteVal = (val >> 8 * byte) & 0xff;
			int32_t bitVal = (byteVal & (1 << bit) ? 1 : 0) << j;
			if (j == sizeCode - 1)
				bNegative = bitVal != 0;
			else
				current |= bitVal;

			if (++j >= sizeCode)
			{
				//sOutput.AppendFormat(L"%d Value ends\t%d bit\t%d byte of\t0x%08x\t\t%d\t", i, bit, byte, val, current);

				current <<= parameters.bit_offset;
				if (current == 0)
					int a = 0;
				if (bNegative)
				{
					//sOutput.AppendFormat(L"%d\n", -(*delta));
					prevVal = prevVal - (*delta);
				}
				else
				{
					prevVal = prevVal + (*delta);
					//sOutput.AppendFormat(L"%d\n", (*delta));
				}

				//OutputDebugString(sOutput);

				pDataOut[i++] = prevVal == 0 ? (float)(prevVal + 1) * parameters.coef_k : (float)prevVal * parameters.coef_k;
				if (i >= outSize)
					break;
				j = 0;
				current = 0;
			}

			if (++bit > 7)
			{
				bit = 0;
				if (--byte < 0)
				{
					byte = 3;
					break;
				}
			}
		}
		if (i >= outSize)
			break;

		++counter;
	}

	return outSize;
}
//----------------------------------------------------------------------------------------------------------------------