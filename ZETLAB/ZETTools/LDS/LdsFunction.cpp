//------------------------------------------------------------------------------
#include "StdAfx.h"
#include <LDS\LdsFunction.h>
//------------------------------------------------------------------------------
bool SearchTimeBegin(CDSRV *psrv, MultiChannelSelf *pReadParam,
	double &Tx, bool bTimeInDate)
{
	int n = 10;
	int size = pReadParam->NumberCounts * n;
	int sizePreHistory = pReadParam->NumberCounts * (n - 2);
	double time = pReadParam->DataTime.GetTime();
	float *pData = new float[size];
	bool ret = (psrv->GetData(pReadParam->Channel, 0,
		time, size, pData) == 0) &&
		SearchPositionBeginUsingFDWT(pData, NULL, 1,
		size, sizePreHistory, &n, NULL);
	if (ret)
	{
		time -= (size - n) / pReadParam->freqADC;
		if (bTimeInDate)
			Tx = psrv->GetTimeSRVasDATE(time);
		else
			Tx = time;
	}
	delete[] pData;
	return ret;
}
//------------------------------------------------------------------------------