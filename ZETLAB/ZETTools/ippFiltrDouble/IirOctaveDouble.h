#pragma once
#include "afx.h"
#include <ippFiltrDouble\CustomIirFiltrDouble.h>

#define MAX_ELLIPTIC_ORDER	20

#define SizeBandInDecade_1 10
#define SizeBandInDecade_3 10
#define SizeBandInDecade_6 20
#define SizeBandInDecade_12 40
#define SizeBandInDecade_24 80

const double g_d1_All_Octave[SizeBandInDecade_1 + SizeBandInDecade_3 + SizeBandInDecade_6 +
							SizeBandInDecade_12 + SizeBandInDecade_24] {
	// 1/1
	1., 2., 4., 8., 16., 31.5, 63., 125., 250., 500.,
	// 1/3
	1., 1.25, 1.6, 2., 2.5, 3.15, 4., 5., 6.3, 8.,
	// 1/6
	1., 1.12, 1.25, 1.4, 1.6, 1.8, 2.,
		2.24, 2.5, 2.8, 3.15, 3.55, 4.,
		4.5, 5., 5.6, 6.3, 7.1, 8., 9.,
	// 1/12
	1., 1.06, 1.12, 1.19, 1.25, 1.33, 1.4, 1.5, 1.6, 1.68, 1.8, 1.9, 2.,
		2.12, 2.24, 2.38, 2.5, 2.67, 2.8, 3., 3.15, 3.36, 3.55, 3.78, 4.,
		4.24, 4.5, 4.76, 5., 5.34, 5.6, 6., 6.3, 6.73, 7.1, 7.55, 8.,
		8.48, 9., 9.5,
	// 1/24
	1., 1.03, 1.06, 1.09, 1.12, 1.16, 1.19, 1.22, 1.25, 1.3, 1.33, 1.37, 1.4, 1.46, 1.5, 1.54, 1.6, 1.63, 1.68, 1.73, 1.8, 1.83, 1.9, 1.94, 2.,
		2.06, 2.12, 2.18, 2.24, 2.31, 2.38, 2.45, 2.5, 2.59, 2.67, 2.75, 2.8, 2.91, 3., 3.08, 3.15, 3.27, 3.36, 3.46, 3.55, 3.67, 3.78, 3.89, 4.,
		4.12, 4.24, 4.36, 4.5, 4.62, 4.76, 4.9, 5., 5.19, 5.34, 5.5, 5.6, 5.82, 6., 6.12, 6.3, 6.54, 6.73, 6.92, 7.1, 7.34, 7.55, 7.77, 8.,
		8.23, 8.48, 8.72, 9., 9.24, 9.5, 9.79 };


enum TypeElliptic
{
	teLPF,	// low pass filter
	teHPF,	// high pass filter
	teBPF,	// band pass filter
	teBSF,	// band stop filter
};

enum OctavePart : BYTE
{
	op1_1,
	op1_3,
	op1_6,
	op1_12,
	op1_24,
	Number_Octave_Part,
};

const double g_factor[Number_Octave_Part] =
		{ IPP_SQRT2, pow(IPP_SQRT2, 1. / 3.), pow(IPP_SQRT2, 1. / 6.),
		pow(IPP_SQRT2, 1. / 12.), pow(IPP_SQRT2, 1. / 24.) };

const DWORD g_sizeBandInAllDecade[Number_Octave_Part] =
{ SizeBandInDecade_1, SizeBandInDecade_3, SizeBandInDecade_6, SizeBandInDecade_12, SizeBandInDecade_24 };

#define LimitPartsCount		9
#define LimitPointsCount	LimitPartsCount * 2 - 1
const double g_partPoints[LimitPartsCount] = { 0., 1. / 8., 1. / 4., 3. / 8., 1. / 2., 1., 2., 3., 4. };
const double g_partDownLimitStart[3][LimitPointsCount - 1] =   {{ -141., -141., -141., -141., -4.5, -1.1, -0.40, -0.20, -0.15, -0.2, -0.4, -1.10, -141., -141., -141., -141. },
																{ -141., -141., -141., -141., -5.0, -1.3, -0.60, -0.40, -0.30, -0.4, -0.6, -1.30, -141., -141., -141., -141. },
																{ -141., -141., -141., -141., -5.5, -1.6, -0.80, -0.60, -0.50, -0.6, -0.8, -1.60, -141., -141., -141., -141. }};
const double g_partDownLimitEnd[3][LimitPointsCount - 1] =	   {{ -141., -141., -141., -141., -1.1, -0.4, -0.20, -0.15, -0.20, -0.4, -1.1, -4.50, -141., -141., -141., -141. },
																{ -141., -141., -141., -141., -1.3, -0.6, -0.40, -0.30, -0.40, -0.6, -1.3, -5.00, -141., -141., -141., -141. },
																{ -141., -141., -141., -141., -1.6, -0.8, -0.60, -0.50, -0.60, -0.8, -1.6, -5.50, -141., -141., -141., -141. }};
const double g_partUpLimitStart[3][LimitPointsCount - 1] =	   {{ -75.0, -62.0, -42.5, -18.0,  0.15, 0.15, 0.15,  0.15,  0.15,  0.15, 0.15, 0.15, -2.3,  -18.0, -42.5, -62.0 },
																{ -70.0, -61.0, -42.0, -17.5,  0.30, 0.30, 0.30,  0.30,  0.30,  0.30, 0.3,  0.30, -2.0,  -17.5, -42.0, -61.0 },
																{ -60.0, -55.0, -41.0, -16.5,  0.50, 0.50, 0.50,  0.50,  0.50,  0.50, 0.5,  0.50, -1.6,  -16.5, -41.0, -55.0 }};
const double g_partUpLimitEnd[3][LimitPointsCount - 1] =	   {{ -62.0, -42.5, -18.0, -2.30,  0.15, 0.15, 0.15,  0.15,  0.15,  0.15, 0.15, 0.15, -18.0, -42.5, -62.0, -75.0 },
																{ -61.0, -42.0, -17.5, -2.00,  0.30, 0.30, 0.30,  0.30,  0.30,  0.30, 0.30, 0.30, -17.5, -42.0, -61.0, -70.0 },
																{ -55.0, -41.0, -16.5, -1.60,  0.50, 0.50, 0.50,  0.50,  0.50,  0.50, 0.50, 0.50, -16.5, -41.0, -55.0, -60.0 }};

class CIirOctaveDouble : public CCustomIirFiltrDouble
{
private:
	OctavePart m_OctavePart;
	double m_pTapsD[(MAX_ELLIPTIC_ORDER + 1) * 2];
	IppsIIRState_64f *m_pStateLPF, *m_pStateHPF;
	Ipp8u* m_pStateLpfBuf, *m_pStateHpfBuf;

	double m_limitFreqs[LimitPointsCount];

	bool _Init();
	bool _InitEllipticFilter(TypeElliptic teType, double dFreq1,
		double dFreq2, double dBandPassRipple, double dBandStopDrop,
		TransferFunction* tfFunction);

	const double getLimit(const double f, const double* pLimitDown, const double* pLimitUp) const;

protected:

public:
	CIirOctaveDouble();
	virtual ~CIirOctaveDouble();
	// методы чтения/записи свойств -----------------
	OctavePart GetOctavePart() { return m_OctavePart; }

	// методы класса --------------------------------
	bool InitFiltr(TypeFiltr type, double freq, OctavePart part);
	double PriorValue(double freq, OctavePart part);

	// Функции фильтрации
	virtual bool Filtration(double *pInpData, double *pOutData, int num);
	virtual bool Filtration_I(double *pInpOutData, int num);

	// Переинициализация фильтра со старыми параметрами
	virtual bool ReStart();

	const double getUpLimit(const double f, const int filtrClass) const;
	const double getDownLimit(const double f, const int filtrClass) const;
};

