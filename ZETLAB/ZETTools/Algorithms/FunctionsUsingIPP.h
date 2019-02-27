//------------------------------------------------------------------------------
// ������� ��������� ������
#pragma once
//------------------------------------------------------------------------------
#include "afx.h"
#include <Intel\ipp\ipps.h>
#include <ZetErrorCodes.h>	// ���� ������
// #include "float.h"
// #include <mathZet.h>
// #include <ZetTools\include\ZetTools.h>
//------------------------------------------------------------------------------
// �������� ���������� ������������ �������
bool RemovingConstComponent_32f(const float *pSrc, float *pDst, int len, float *mean = nullptr);
bool RemovingConstComponent_32f_I(float *pSrcDst, int len, float *mean = nullptr);

bool RemovingConstComponent_64f(const double *pSrc, double *pDst, int len, double *mean = nullptr);
bool RemovingConstComponent_64f_I(double *pSrcDst, int len, double *mean = nullptr);
//------------------------------------------------------------------------------
// �������������� float � ��
bool ConvertAmplToDB_32f(const float referAmpl, const float *pSrc, float *pDst, int len, const bool bSrcNegative = true);
bool ConvertAmplToDB_32f_I(const float referAmpl, float *pSrcDst, int len, const bool bSrcNegative = true);

bool ConvertPowerToDB_32f(const float referAmpl, const float *pSrc, float *pDst, int len, const bool bSrcNegative = true);
bool ConvertPowerToDB_32f_I(const float referAmpl, float *pSrcDst, int len, const bool bSrcNegative = true);

// �������������� float � �� � ������������ �����
bool ConvertAmplToDB_32f(const float referAmpl, const float subLevelAmpl, const float *pSrc, float *pDst, int len, const bool bSrcNegative = true);
bool ConvertAmplToDB_32f_I(const float referAmpl, const float subLevelAmpl, float *pSrcDst, int len, const bool bSrcNegative = true);

bool ConvertPowerToDB_32f(const float referAmpl, const float subLevelPower, const float *pSrc, float *pDst, int len, const bool bSrcNegative = true);
bool ConvertPowerToDB_32f_I(const float referAmpl, const float subLevelPower, float *pSrcDst, int len, const bool bSrcNegative = true);

// �������������� double � ��
bool ConvertAmplToDB_64f(const double referAmpl, const double *pSrc, double *pDst, int len, const bool bSrcNegative = true);
bool ConvertAmplToDB_64f_I(const double referAmpl, double *pSrcDst, int len, const bool bSrcNegative = true);

bool ConvertPowerToDB_64f(const double referAmpl, const double *pSrc, double *pDst, int len, const bool bSrcNegative = true);
bool ConvertPowerToDB_64f_I(const double referAmpl,  double *pSrcDst, int len, const bool bSrcNegative = true);
//------------------------------------------------------------------------------
// ����� ��������� ����������. �������� ��� ������ ���� �������������.
// �������� - ��� �����, �������� �������� ������ ������������ ��� �� factor
bool ExtremumOverNose_32f(const float *pSrc, const int len, int & xExt, float & yExt, const float factor = 3.f);
bool ExtremumOverNose_64f(const double *pSrc, const int len, int & xExt, double & yExt, const double factor = 3.);
//------------------------------------------------------------------------------
// ����� �������� ��������. ��� ������ ���� ����������������
bool FindPeak_32f(const float *pSrc, const int len, int & xPeak);
//----------------------------------------------------------------------------------------------------------------------
// ����������� ������������ ������� ������� �� ����������� ������ ����� � �������������
const UINT getFilterOrder(_In_ const double fs, _In_ const double fadc);
//----------------------------------------------------------------------------------------------------------------------
//--------------------------------------- ������� ������ � ����������� -------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
// ����� ��������� ����������� ������ � ������ ����������� �� ������� ��������������
// ���������:
//		pSrcPhase	- ������ ������ ������� ��������������
//		size		- ������ ������� ������ ������� ��������������
//		freq		- ������� �������������
//		phase		- ��������� �������� ���� ��� ������ ����������
//		deltaPhase	- ������ �������� �������� ���� ��� ������ ����������
//		f1			- ������ ��������� ������, � ������� ����� ������
//		f2			- ����� ��������� ������, � ������� ����� ������
//		ppFreq		- ���������, ���� ����� �������� ��������� ��������� ����������� �������
//		ppQ			- ���������, ���� ����� �������� �����������
//		count		- ���������� ��������� ��������� ����������� ������
int calcPossibleResonancesByPhase(_In_ const double* pSrcPhase, _In_ const int size, _In_ const double freq,
	_In_ const double phase, _In_ const double deltaPhase, _In_ const double f1, _In_ const double f2,
	_Out_ double** ppFreq, _Out_ double** ppQ, _Out_ int& count);
//----------------------------------------------------------------------------------------------------------------------
// ������ ������ ����������� � ����������� ������� �� ���������� �������� ����������� ������� � �����������
// ���������:
//		pSrcAmpl	- ������ ������ ����������� ��������������
//		size		- ������ ������� ������ ����������� ��������������
//		freqADC		- ������� �������������
//		fResSrc		- ������ �������� ����������� �������
//		q			- ������ �������� �����������
//		f0			- ������ �������� ����������� �������
//		fResDst		- ������ �������� ����������� �������
//		amplRes		- ������ �������� ��������� ����������� �������������� �� ����������� �������
long calcResonanceFreqs(_In_ const double* pSrcAmpl, _In_ const int size, _In_ const double freqADC,
	_In_ const double fResSrc, _In_ const double q, _Out_ double& f0, _Out_ double& fResDst, _Out_ double& amplRes);
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------- ������� ������� ���������� ������������� -----------------------------------------
//----------------------------------------------------------------------------------------------------------------------
struct �alcImpRespState
{
	double freq = 0.;		// ������� ������������� �������
	int needLpf = 0;		// 0 - ��� �� �����, 1 - ��� �����
	int needHpf = 0;		// 0 - ��� �� �����, 1 - ��� �����
	double lpfFreq = 0.;	// ������� ����� ���
	double hpfFreq = 0.;	// ������� ����� ���
	int direction = 0;		// 0 - ������ ���������� ��������������, 1 - �������� ���������� ��������������
	int noPhase = 0;		// 0 - ��������� ���� ��� �������, 1 - �� ��������� ���� ��� �������
	int symm = 0;			// 0 - ��������������, 1 - ������������
};
// ������� ������� ���������� ��������������
// param - ��������� �������
// pCrossSpectrum - �������� ������
// pAmp2Spectr1 - ������� ��������� ������� ������� ������
// pAmp2Spectr2 - ������� ��������� ������� ������� ������
// pMemory - �������������� ������ ��� ������� (������ size*2)
// size - ������ ��������
// gain - ����������� �������� �� ���������� ������������
// pImpResp - ������������ ���������� ��������������
int CalcImpRespByCrossSpectrum(_In_ const �alcImpRespState& param, _In_ const Ipp64fc* pCrossSpectrum,
	_In_ const double* pAmp2Spectr1, _In_ Ipp64fc* pMemory, _In_ const int size, _In_ const Ipp64fc gain, _Out_writes_all_(size * 2) double* pImpResp);
//----------------------------------------------------------------------------------------------------------------------
int CalcImpRespByCrossSpectrum(_In_ const �alcImpRespState& param, _In_ const double* pH12, const double* pPhase,
	_In_ Ipp64fc* pMemory, _In_ const int size, _Out_writes_all_(size * 2) double* pImpResp);
//----------------------------------------------------------------------------------------------------------------------