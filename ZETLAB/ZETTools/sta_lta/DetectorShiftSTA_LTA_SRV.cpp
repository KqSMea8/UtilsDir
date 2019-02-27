//------------------------------------------------------------------------------
//	Модуль CDetectorShiftSTA_LTA_SRV.cpp
//	Версия от 10.11.2011
//	Зеленоград, ЗАО "ЭТМС", Савостин Ю.М.
//------------------------------------------------------------------------------
//	Класс предназначен для создания детектора STA_LTA, основанного на
// многоканальном (N >= 1) временном сигнале
//------------------------------------------------------------------------------
#include "stdafx.h"
#include <sta_lta\DetectorShiftSTA_LTA_SRV.h>
//------------------------------------------------------------------------------
CDetectorShiftSTA_LTA_SRV::CDetectorShiftSTA_LTA_SRV() : CDetectorShiftSTA_LTA()
{
	pSRV = NULL;
}
//------------------------------------------------------------------------------
/*CDetectorShiftSTA_LTA_SRV::~CDetectorShiftSTA_LTA_SRV()
{
}*/
//------------------------------------------------------------------------------
bool CDetectorShiftSTA_LTA_SRV::Create(int numberSignal, int size, int sizePage,
				int numberSTA, int numberLTA,
				HWND hwnd, UINT wm_EVENT_START, UINT wm_EVENT_END,
				bool needVirtChan, int numVirtChan,
				CDSRV *psrv, float *pfreqADC,
				CShiftBufferDATE *pbufDate)
{
	bool ret = CDetectorShiftSTA_LTA::Create(numberSignal, size, sizePage,
					numberSTA, numberLTA, hwnd, wm_EVENT_START, wm_EVENT_END,
					pbufDate);
	if (ret)
	{
		FbNeedVirtChannel = needVirtChan;
		NumberVirtChannel = numVirtChan;
		pSRV = psrv;
		pFreqADC = pfreqADC;
		FStartPointVirtChannel = size - sizePage - numberSTA;
		FdTimeVirtChannel = double(numberSTA) / *pFreqADC;
	}
	return ret;
}
//------------------------------------------------------------------------------
void CDetectorShiftSTA_LTA_SRV::SetNumberSTA(int val)
{
	CDetectorShiftSTA_LTA::SetNumberSTA(val);
	FStartPointVirtChannel = m_pSTA_LTA->GetSize() -
								m_pSTA_LTA->GetSizePage() - FNumberSTA;
	FdTimeVirtChannel = double(FNumberSTA) / *pFreqADC;
}
//------------------------------------------------------------------------------
bool CDetectorShiftSTA_LTA_SRV::SetSize(int size)
{
	bool ret = CDetectorShiftSTA_LTA::SetSize(size);
	FStartPointVirtChannel = m_pSTA_LTA->GetSize() -
					m_pSTA_LTA->GetSizePage() - FNumberSTA;
	return ret;
}
//------------------------------------------------------------------------------
void CDetectorShiftSTA_LTA_SRV::SetSizePage(int val)
{
	CDetectorShiftSTA_LTA::SetSizePage(val);
	FStartPointVirtChannel = m_pSTA_LTA->GetSize() -
					m_pSTA_LTA->GetSizePage() - FNumberSTA;
}
//------------------------------------------------------------------------------
void CDetectorShiftSTA_LTA_SRV::Calculation(float **ppData,
			double localTimeLastPage)
{	//	Расчеты значений детектора для данных последнего буфера
	CDetectorShiftSTA_LTA::Calculation(ppData);

	// расчёты значений виртуального канала ---------------------
	if (FbNeedVirtChannel)
		pSRV->PutData( NumberVirtChannel, localTimeLastPage,
				m_pSTA_LTA->GetSizePage(),
				m_pSTA_LTA->GetGlobalAddres() + FStartPointVirtChannel );
}
//------------------------------------------------------------------------------