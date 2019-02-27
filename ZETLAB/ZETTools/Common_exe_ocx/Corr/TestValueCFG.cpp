//-----------------------------------------------------------------------------
#include "stdafx.h"
#include <mathZet.h>
#include "TestValueCFG.h"
//-----------------------------------------------------------------------------
bool Test_BandPassFiltr(float & newVal_freq_0, float & newVal_freq_1,
	const float freqADC, const long decada,
	const TypeTestFiltrBandPass typeTest, const bool bChange)
{
	float frADC = freqADC / (float)pow(10, decada);
	float f_min = frADC / g_factor_filtrBandPass;
	float f_max = frADC * 0.5f - f_min;
	bool ret(false);
	bool bFreq_0_IsOk = newVal_freq_0 > 0.f;
	bool bFreq_1_IsOk = newVal_freq_1 > 0.f;
	switch (typeTest)
	{
	case test_freq_0:
		if (bChange)
		{// ���� �������� ��������
			if (bFreq_1_IsOk)
			{// ���� �������� ��������, ���� ������ ������
				ret = TestFiltrBandPass(newVal_freq_0, newVal_freq_1, f_min, f_max, test_freq_0);
			}
			else
			{// ���� �������� ��������, ������� ������� ���
				ret = TestAndChangeValue(newVal_freq_0, f_min, f_max, true);
			}
		}
		else
		{// �� ���� �������� ��������
			if (bFreq_1_IsOk)
			{// �� ���� �������� ��������, ���� ������ ������
				ret = (f_min <= newVal_freq_0) && (newVal_freq_0 <= newVal_freq_1);
			}
			else
			{// �� ���� �������� ��������, ������� ������� ���
				ret = TestAndChangeValue(newVal_freq_0, f_min, f_max, false);
			}
		}
		break;

	case test_freq_1:
		if (bChange)
		{// ���� �������� ��������
			if (bFreq_0_IsOk)
			{// ���� �������� ��������, ���� ������ ������
				ret = TestFiltrBandPass(newVal_freq_0, newVal_freq_1, f_min, f_max, test_freq_1);
			}
			else
			{// ���� �������� �������� ������� ������� ���
				ret = TestAndChangeValue(newVal_freq_1, f_min, f_max, true);			
			}
		}
		else
		{// �� ���� �������� ��������
			if (bFreq_0_IsOk)
			{// �� ���� �������� ��������, ���� ������ ������
				ret = TestAndChangeValue(newVal_freq_0, f_min, f_max, false);
				ret = (newVal_freq_0 <= newVal_freq_1) && (newVal_freq_1 <= f_max);
			}
			else
			{// �� ���� �������� ��������, ������� ������� ���
				ret = TestAndChangeValue(newVal_freq_1, f_min, f_max, false);
			}
		}
		break;

	case test_freq_both:
		if (bChange)
		{// ���� �������� ��������
			if (bFreq_0_IsOk && bFreq_1_IsOk)
			{// ���� �������� ��������
				ret = TestFiltrBandPass(newVal_freq_0, newVal_freq_1, f_min, f_max, test_freq_both);
			}
			else if (bFreq_0_IsOk)
			{// ���� �������� ��������
				ret = TestAndChangeValue(newVal_freq_0, f_min, f_max, true);
			}
			else if (bFreq_1_IsOk)
			{// ���� �������� ��������
				ret = TestAndChangeValue(newVal_freq_1, f_min, f_max, true);
			}
			else// bNotZeroFreq_0 = 0 && bNotZeroFreq_1 = 0
			{// ���� �������� ��������
				ret = true;
			}
		}
		else
		{// �� ���� �������� ��������
			if (bFreq_0_IsOk && bFreq_1_IsOk)
			{// �� ���� �������� ��������
				ret = (f_min <= newVal_freq_0) &&
				(newVal_freq_0 <= newVal_freq_1) &&
				(newVal_freq_1 <= f_max);
			}
			else if (bFreq_0_IsOk)
			{// �� ���� �������� ��������
				ret = (f_min <= newVal_freq_0) &&
					(newVal_freq_0 <= f_max);
			}
			else if (bFreq_1_IsOk)
			{// �� ���� �������� ��������
				ret = (f_min <= newVal_freq_1) &&
					(newVal_freq_1 <= f_max);
			}
			else// bNotZeroFreq_0 = 0 && bNotZeroFreq_1 = 0
			{// �� ���� �������� ��������
				ret = true;
			}
		}
		break;
	}
	return ret;
}
//-----------------------------------------------------------------------------
bool Test_InterChanDela(float & newVal, const bool bChange)
{
	return TestAndChangeValue(newVal, g_InterChanDelay_Min, g_InterChanDelay_Max, bChange);
}
//-----------------------------------------------------------------------------
bool Test_DistanceChan(float & newVal, const bool bChange)
{
	return TestAndChangeValue(newVal, g_Distance_Min, g_Distance_Max, bChange);
}
//-----------------------------------------------------------------------------
bool Test_TimeAver(float & newVal, const bool bChange)
{
	return TestAndChangeValue(newVal, g_timeAver_Min, g_timeAver_Max, bChange);
}
//-----------------------------------------------------------------------------
bool Test_Decada(BYTE & newVal, const bool bChange)
{
	return TestAndChangeValue(newVal, (BYTE)0, (BYTE)g_decada_Max, bChange);
}
//-----------------------------------------------------------------------------
bool Test_ShiftA12(float & newVal,	// �������� � ����
	const float freqADC, const BYTE decada, const TypeTimeDuration typeTimeDuration,
	const bool bChange, float *pMaxVal)
{
	float val = 1000 / freqADC;		// �������� ������ ������ ������ � ����
	if (decada == 1)
		val *= 10;
	val *= RoundToInt(pow(2, (int)typeTimeDuration + 5));
	if (pMaxVal != nullptr)
		*pMaxVal = val;
	return TestAndChangeValue(newVal, -val, val, bChange);
}
//-----------------------------------------------------------------------------
float GetFminForFiltr(const float freqADC)
{
	return freqADC / g_factor_filtrBandPass;
}
//-----------------------------------------------------------------------------
float GetFmaxForFiltr(const float freqADC)
{
	return  freqADC * g_factor_freq_band;
//	return  freqADC * (0.5f - 1.f / g_factor_filtrBandPass);
}
//-----------------------------------------------------------------------------
StructureCFG::StructureCFG()
{
	SetValueDefault();
}
//-----------------------------------------------------------------------------
void StructureCFG::SetValueDefault()
{
	bCalcDistance = false;
	bInversion = false;
	typeFuncView = tfv_corr_envel;
	bWriteToLogger = false;
	bFiltration = false;
	bFilteringDiscrete = false;
	bDecade = 0;
	typeTimeDuration = ttd_13;
	typeModeDC = typeDC_yes;

	fTimeAver_sec = 1.f;		// ����� ����������, ���
	fFreqLowPass = 0.f;			// ������� ����� ���
	fFreqHighPass = 0.f;		// ������� ����� ���
	fInterChanDelay = 10.f;		// ������������ ��������, ��
	fDistance_0 = 0.f;			// ��������� ������� ������ 0
	fDistance_1 = 0.f;			// ��������� ������� ������ 1
	fShiftZero_funcA12 = 0.f;	// �������� ��������� �������� ��� ������� ����� �������

	ZeroMemory(sWriteComment, sizeof(sWriteComment));
}
//-----------------------------------------------------------------------------