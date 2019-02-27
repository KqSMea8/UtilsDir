//------------------------------------------------------------------------------
#include "stdafx.h"
#include "StructForUNIT2.h"
//------------------------------------------------------------------------------
StructCorrForUNIT2::StructCorrForUNIT2()
	: size(sizeof(StructCorrForUNIT2))
	, indxKit(0)
	, numArray(1)
	, bOldData(false)
	, xPosExtremum_1(0.f)
	, xPosExtremum_2(0.f)
	, xPosExtremum_3(0.f)
	, xPosExtremum_4(0.f)
	, xPosIntersection(0.f)
	, sizeOneArray(0)
	, ztTime()
	{}
//------------------------------------------------------------------------------
void StructCorrForUNIT2::SetSizeOfStruct()
{
	size = SizeItemsInByte + SizeAllArraysInByte(sizeOneArray, numArray);
}
//------------------------------------------------------------------------------
bool StructCorrForUNIT2::TestSize(DWORD * pShift)
{
	DWORD shift = SizeItemsInByte;
	if (pShift != nullptr)
		*pShift = shift;

	return size == (shift + SizeAllArraysInByte(sizeOneArray, numArray));
}
//------------------------------------------------------------------------------
bool StructCorrForUNIT2::GetPointers(const int indxArray, float ** ppDataArray)
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