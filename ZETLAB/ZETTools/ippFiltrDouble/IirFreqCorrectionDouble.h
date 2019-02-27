#pragma once
#include "afx.h"
#include <ippFiltrDouble\CustomIirFiltrDouble.h>

// Перечень IIR фильтров, реализуемых какими-либо классами
enum FreqCorrection
{
	fcWb,
	fcWc,
	fcWd,
	fcWe,
	fcWf,
	fcWh,
	fcWj,
	fcWk,
	fcWm,
	fcFh,
	fcFk,
	fcFm,
	fcA,
	fcB,
	fcC,
	fcD,
};

class CIirFreqCorrectionDouble :public CCustomIirFiltrDouble
{
private:
	double pTapsD[6];
	IppsIIRState_64f *pStateHPF;
	Ipp8u* pStateHPFBuf;
	IppsIIRState_64f *pStateLPF;
	Ipp8u* pStateLPFBuf;
	IppsIIRState_64f *pStateTF;
	Ipp8u* pStateTFBuf;
	IppsIIRState_64f *pStateSF1;
	Ipp8u* pStateSF1Buf;
	IppsIIRState_64f *pStateSF2;
	Ipp8u* pStateSF2Buf;
	bool _Init();
	bool InitVibroCorrection();
	bool InitAcousticCorrection();

protected:
	FreqCorrection FCorr;

public:
	CIirFreqCorrectionDouble();
	virtual ~CIirFreqCorrectionDouble();
	// методы чтения/записи свойств -----------------

	// методы класса --------------------------------
	bool InitFiltr(TypeFiltr type, FreqCorrection corr);

	// Функции фильтрации
	virtual bool Filtration(double *pInpData, double *pOutData, int num);
	virtual bool Filtration_I(double *pInpOutData, int num);

	// Переинициализация фильтра со старыми параметрами
	virtual bool ReStart();
};

