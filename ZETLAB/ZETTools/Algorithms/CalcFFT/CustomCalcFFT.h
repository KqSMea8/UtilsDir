//------------------------------------------------------------------------------
/* ���� CustomCalcFFT.h

	������� ����� �CustomCalcFFT �������, ��������������� ��� ����������
�������� ������� ���������� ��� (��� float � double), ����������� � �������
������� ���������� IPP 7.0.

	����� ���������� ���� ������� �������� �������� ����, � ������� �������
���������� ���������� � ���������� ������, � ����������� ����������� � ����������
���������� ���� ������� �������� � ���������� ������ ������.

	������, �������� ��� ���������� �������:
- ����������� ������������� ��������� ������, ����������� ��� �������� ���;
- ������� �������� ���������� ����������� ��������;
- ������� �������� ������� ������ ������� (���������� ������������ �������);
- ���������� ������������� �������� �������, ����� ���:
	* ��������� �������� ������;
	* ������������� ��������� ����, ������������� �� �������� ������;
	* ������ � ��������� ������� � ���� �������������� ������� (��� float).

	������ ������:
	- ������ ��� (����������, ����������� ����������� � �������� ������������);
	- ������ ������� � ��������� Get � Set;
	- ��� ������� ��������� ��������.

	������� ����� �������� ������� ������ �� ���������� ������� � �������
��������� ��������.
	�������� ������
1. int OrderFFT - ������� ���, ���������� ������ ������. ���������� ��������
�������� �� 7 �� 20 ������������. �� ��������� 7.
2. bool CenteringSrcData - ��������� �������� ������ (�������� ����������
������������ ��������� �������) ����� ����������� ������� ���. �� ���������
false.
3. ������������ TypeNormalization - ��� ���������� ������ ��� ���������� ���,
������������� ����� ���������� IPP 7.0 (��� ����������, ���������� ���� 1/N �
���������� ���� 1/sqrt(N)). �� ��������� ���������� ���� 1/N.
4. ������������ TypeWindowForFFT - ��� ������������� ���������� ����, ��. ����.
�� ��������� ��� ����.

	������� ��������� ��������
1. GetSize - ���������� �������� 2 ^ OrderFFT.
2. GetHalfSize - ���������� �������� (2 ^ OrderFFT) / 2.

	��������� ���������� � ��������� � ������� �����������.

1. ����� �������� �������� � �������� ��������� ������ ������ ������������ �������
���������� ������ ��� ��� ���������� ���, � ������ ���� ��� �������� ���������
�����, ������ �������� ������������ � ������� ��������������� ������� ����������.

2. � ������ ������ ���������� ���� 1/N ��� ������� ������������� ������� ��������,
������ ����� ����������� �� ����� ��������� re � im ������� � ������ ������
���������, ����� ��������� ��������� �������.

3. ���� ������� �������� ������������� ������ � ��������, � 8 ��� (������ � 2^m,
��� m - ����������� ����� ��������������� ��������) ������ ������� �������������,
�� ����������� �������, ���������� � ������� ���� ��� ������� ������� ��� �����
����������� ���������� �� �������� ���������, ���������� ������������ � ���������
���� �������������� ������� (� ��������� �� ���� 10^-6).

4. � ������ ������ ���������� ���� 1/N ��� 1/sqrt(N) ��� ������� �����������
������� �� �. 3 ��������� ������� CalcInv_CToR (��� � �������� ����� ����������
������� CalcInv_CToC) ����� ��������� � �������� ������������� �������� (�
��������� �� ���� 10^-6) �� �������� �� �������� ���������, ����������
������������ � ��������� ���� ��������� �������. ��� ���� �� ������� ������������
������� ��������� ���� � ������� �������������� ������� ������ ����� ���������
����� ����� �������� ��������� ������� (��������� ���������� �����������
������������� ���). ��� �������� ���������� ����������� ������� ��������
������� ������ �������.

5. �������� �� ��, ��� ������ � ���������� ������������ ���������
����������������� � ������ ��� ������ ���, � �������, ����� ����������
������������ �� �������� ������������� ����������, � ����� ������� � �������
��������� �������� CenteringSrcData � true, �.�. ��� ������ �� ������������
�������� ����������� ������� (��� ������� ��������� ���������� ������������).
*/
//------------------------------------------------------------------------------
#pragma once
#include <mathZet.h>
#include <ZetErrorCodes.h>	// ���� ������
#include <Buffer\SmartArray.h>
#include <Intel\ipp\ipps.h>
#include <Algorithms\FunctionsUsingIPP.h>
#include "TemplateWindow.h"
//------------------------------------------------------------------------------
#define OrderFFT_Min	7
#define OrderFFT_Max	20
//------------------------------------------------------------------------------
// �������� ����� ���������� ����������� ��������������
enum TypeNormalization : BYTE
{
	norm_no,				// ��� ����������
	norm_N,					// ���������� (1 / N) ��� Fwd
	norm_Sqrt_N,			// ���������� (1 / sqrt(N)) ��� Fwd � Inv
	norm_N_inv,				// ���������� (1 / N ��� Inv

	norm_size
};
//------------------------------------------------------------------------------
// �������� ����� ��������� ����.
// ����� ���� ��������� ������ � �����, ����� win_size!
enum TypeWindow : BYTE
{
	win_no,					// ��� ����
	win_Bartlett,			// ���� ��������
	win_Blackman,			// ����������� ���� ��������
	win_Hamming,			// ���� ��������
	win_Hann,				// ���� �����
	win_Kaiser,				// ����������� ���� �������

	win_Hanning,			// ���� ��������
	win_RifeVincent_4,		// ���� ����-�������� (4)
	win_BlackmanHarris_3,	// ���� ��������-������� (3)
	win_BlackmanHarris_4,	// ���� ��������-������� (4)
	win_Nuttall,			// ���� �������
	win_BlackmanNuttall,	// ���� ��������-�������
	win_Flattop,			// ���� � ������� ��������

	win_size
};
//------------------------------------------------------------------------------
class CCustomCalcFFT
{
private:

protected:

	bool m_bCenteringSrcData;		// ��������� �������� ������ Fwd 
	TypeWindow m_typeWin;			// ��� ���������� ���� ��� Fwd FFT
	TypeNormalization m_typeNorm;	// ��� ���������� ����������� FFT
	bool b4;
	int m_factorTypeSrc;			// ���������� �� ���� ������� ������

	int m_nOrderFFT;				// ������� FFT
	int m_sizeFFT;					// = 2 ^ order

	int m_sizeFFT_2;				// = m_sizeFFT / 2
	int m_sizeHilbert;				// ������ ������ � ��������� Hilbert

	double m_sqrt_N;				// = 1 / sqrt(m_sizeFFT)
	double m_factorWindow;			// ����������� �����. ���������� ����
	double m_factorEnergyWindow;	// �������������� �����. ���������� ����

	// ��������� ������
	SmartArray<Ipp8u> m_saBuffer_8u;

	// ��������� ������� ------------------------------------
	// ��������� ����� ��� ������� ��������������
	int GetFlagForFwd();

	virtual void DeleteStructure_FFT() {}
	virtual void DeleteStructure_All() {}

	virtual void ReSize_saWindow(DWORD size) {}

public:
	CCustomCalcFFT(const int order = 7);
	~CCustomCalcFFT();
	CCustomCalcFFT(const CCustomCalcFFT & val);
	CCustomCalcFFT & operator = (const CCustomCalcFFT & val);

	// ��������� ����� ������������� ��������� ������� ��� Fwd.
	// �� ��������� - false.
	bool GetCenteringSrcData();
	// ������� ����� ������������� ��������� ������� ��� Fwd
	void SetCenteringSrcData(const bool val);

	// ��������� ���� ���������� �����������. �� ��������� - 1/N.
	TypeNormalization GetTypeNormalization();
	// ������� ���� ���������� �����������
	void SetTypeNormalization(const TypeNormalization type);

	// ��������� ���� ���������� ����. �� ��������� - ��� ����.
	TypeWindow GetTypeWindowForFFT();
	// ������� ���� ���������� ����
	void SetTypeWindowForFFT(const TypeWindow type);

	// ��������� ������� FFT. �� ��������� - 7.
	int GetOrderFFT();
	// ������� ������� FFT
	void SetOrderFFT(const int order);

	// ��������� ������� ������ = 2 ^ orderFFT
	virtual int GetSize();

	// ��������� �������� ������� ������ = (2 ^ orderFFT) / 2
	virtual int GetHalfSize();

	// ��������� ������������ ������� ���������� ����
	double GetTimeWindowAmplFactor() { return m_factorWindow; }
	// ��������� ���������������  ������� ���������� ����
	double GetTimeWindowEnergyFactor() { return m_factorEnergyWindow; }
};
//------------------------------------------------------------------------------