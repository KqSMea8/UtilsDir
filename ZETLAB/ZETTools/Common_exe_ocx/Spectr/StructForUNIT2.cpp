//------------------------------------------------------------------------------
#include "stdafx.h"
#include "StructForUNIT2.h"
//------------------------------------------------------------------------------
StructSpectrForUNIT2::StructSpectrForUNIT2()
	: size(sizeof(StructSpectrForUNIT2))
	, indxKit(0)
	, numArray(1)
	, bOldData(false)
	, bGraf_Max(false)
	, bGraf_Aver(false)
	, sizeOneArray(0)
	, ztTime()
	{}
//------------------------------------------------------------------------------
void StructSpectrForUNIT2::SetSizeOfStruct()
{
	size = SizeItemsInByte + SizeAllArraysInByte(sizeOneArray, numArray);
}
//------------------------------------------------------------------------------
bool StructSpectrForUNIT2::TestSize(DWORD * pShift)
{
	DWORD shift = SizeItemsInByte;
	if (pShift != nullptr)
		*pShift = shift;

	return size == (shift + SizeAllArraysInByte(sizeOneArray, numArray));
}
//------------------------------------------------------------------------------
bool StructSpectrForUNIT2::GetPointers(const int indxArray, float ** ppDataArray)
{
	bool ret = (ppDataArray != nullptr) && ((BYTE)indxArray < numArray);
	if (ret)
	{
		DWORD shift(0);
		ret = TestSize(&shift);
		if (ret)
		{
			*ppDataArray = (float *)((BYTE*)this + shift);
			*ppDataArray += indxArray * sizeOneArray;
		}
		else
			*ppDataArray = nullptr;
	}
	return ret;
}
//------------------------------------------------------------------------------