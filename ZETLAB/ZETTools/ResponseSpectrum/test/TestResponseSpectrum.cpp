// TestResponseSpectrum.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "math.h"
#include "atlstr.h"
#include "ResponseSpectrum.h"
#include "test/Accelogramm.h"


void SaveToDTU(CString filename, long size, double *freq, double *data1, double *data2, double *data3)
{
	FILE* fout;
	if (_wfopen_s(&fout, filename.GetString(), L"w, ccs=UTF-8") != 0)
		return;
	CString str;
	fwprintf(fout, L"Частота\tОбщий\tПоложительный\tОтрицительный\n");
	fwprintf(fout, L"Гц\tg\tg\tg\n");
	for (long i = 0; i < size; i++)
	{
		fwprintf(fout, L"%f\t%f\t%f\t%f\n", freq[i], data1[i], data2[i], data3[i]);
	}
	fclose(fout);
}

void Test1(CString sDir)
{
	CResponseSpectrum rs;
	SResponseIn srin;
	SResponseOut srout;
	CString sFileName;

	srin.dSampling = 20000.;
	srin.dFreqBegin = 10.;
	srin.dFreqEnd = 5000.;
	srin.lOctaveNum = 192;
	srin.dDecrement = 0.05;
	srin.lSize = 5000;
	double *pTest = new double[srin.lSize];
	srin.pData = pTest;

	double freq = 100.0;

	// полпериода синуса
	long num = static_cast<long>(srin.dSampling / freq + 0.5) / 2;
	ZeroMemory(srin.pData, srin.lSize * sizeof(srin.pData[0]));
	for (long i = 0; i < num; i++)
		srin.pData[i] = sin(2 * M_PI * freq * i / srin.dSampling);

	rs.Calculate(&srin, &srout, Overall);

	double *pfreq = new double[srout.lSize];
	double *resp1 = new double[srout.lSize];
	double *resp2 = new double[srout.lSize];
	double *resp3 = new double[srout.lSize];
	memcpy_s(pfreq, srout.lSize * sizeof(pfreq[0]), srout.pFreqs, srout.lSize * sizeof(pfreq[0]));
	memcpy_s(resp1, srout.lSize * sizeof(resp1[0]), srout.pReps, srout.lSize * sizeof(resp1[0]));

	rs.Calculate(&srin, &srout, Positive);
	memcpy_s(resp2, srout.lSize * sizeof(resp2[0]), srout.pReps, srout.lSize * sizeof(resp2[0]));

	rs.Calculate(&srin, &srout, Negative);
	memcpy_s(resp3, srout.lSize * sizeof(resp3[0]), srout.pReps, srout.lSize * sizeof(resp3[0]));

	sFileName = sDir + L"RespSin0.dtu";
	SaveToDTU(sFileName, srout.lSize, pfreq, resp1, resp2, resp3);
	printf("Test finished " + CStringA(sFileName) + "\n");

	// 10 периодов синуса
	num = static_cast<long>(srin.dSampling / freq + 0.5) * 10;
	ZeroMemory(srin.pData, srin.lSize * sizeof(srin.pData[0]));
	for (long i = 0; i < num; i++)
		srin.pData[i] = sin(2 * M_PI * freq * i / srin.dSampling);

	rs.Calculate(&srin, &srout, Overall);
	memcpy_s(pfreq, srout.lSize * sizeof(pfreq[0]), srout.pFreqs, srout.lSize * sizeof(pfreq[0]));
	memcpy_s(resp1, srout.lSize * sizeof(resp1[0]), srout.pReps, srout.lSize * sizeof(resp1[0]));

	rs.Calculate(&srin, &srout, Positive);
	memcpy_s(resp2, srout.lSize * sizeof(resp2[0]), srout.pReps, srout.lSize * sizeof(resp2[0]));

	rs.Calculate(&srin, &srout, Negative);
	memcpy_s(resp3, srout.lSize * sizeof(resp3[0]), srout.pReps, srout.lSize * sizeof(resp3[0]));

	sFileName = sDir + L"RespSin10.dtu";
	SaveToDTU(sFileName, srout.lSize, pfreq, resp1, resp2, resp3);
	printf("Test finished " + CStringA(sFileName) + "\n");

	delete[] pTest;
	delete[] pfreq;
	delete[] resp1;
	delete[] resp2;
	delete[] resp3;
}



void Test2(CString sTxt, CString sDtu)
{
	float *pData = nullptr;
	double *pFreq = nullptr;
	double *pResp[6] = { nullptr };
	long size = 0;
	long freq = 0;
	long outsize = 0;

	ReadFileAA(CStringA(sTxt), &pData, size, freq);
	CString filename = sDtu;

	CResponseSpectrum rs;
	SResponseIn srin;
	SResponseOut srout;

	srin.dSampling = (double)freq;
	srin.dFreqBegin = 1.0;
	srin.dFreqEnd = freq / 4;
	srin.lOctaveNum = 16;
	srin.dDecrement = 0.05;
	srin.lSize = size;
	double *pTest = new double[srin.lSize];
	srin.pData = pTest;

	float demp[6] = { 0.0f, 0.005f, 0.020f, 0.050f, 0.100f, 0.150f };
	for (long i = 0; i < 6; i++)
	{
		srin.dDecrement = demp[i];
		for (long j = 0; j < size; j++)
			srin.pData[j] = (double)pData[j];

		rs.Calculate(&srin, &srout, Overall);

		if (pFreq == nullptr)
		{
			outsize = srout.lSize;
			pFreq = new double[outsize];
			memcpy_s(pFreq, outsize * sizeof(double), srout.pFreqs, outsize * sizeof(double));
		}

		pResp[i] = new double[outsize];
		memcpy_s(pResp[i], outsize * sizeof(double), srout.pReps, outsize * sizeof(double));
	}


	FILE* fout;
	if (_wfopen_s(&fout, filename.GetString(), L"w, ccs=UTF-8") != 0)
		return;
	CString str;
	fwprintf(fout, L"Частота\t0.000\t0.005\t0.020\t0.050\t0.100\t0.150\n");
	fwprintf(fout, L"Гц\tg\tg\tg\tg\tg\tg\n");
	for (long i = 0; i < outsize; i++)
		fwprintf(fout, L"%f\t%f\t%f\t%f\t%f\t%f\t%f\n", pFreq[i], pResp[0][i], pResp[1][i], pResp[2][i], pResp[3][i], pResp[4][i], pResp[5][i]);
	fclose(fout);

	printf("Test finished " + CStringA(filename) + "\n");

	for (long i = 0; i < 6; i++)
		delete[] pResp[i];
	delete[] pTest;
	delete[] pFreq;
	delete[] pData;
}

int _tmain(int argc, _TCHAR* argv[])
{
	Test1(L"c:/ZETTools/ResponseSpectrum/test/");
	Test2(L"c:/ZETTools/ResponseSpectrum/test/sa-482.txt",    L"c:/ZETTools/ResponseSpectrum/test/sa-482.dtu");
	Test2(L"c:/ZETTools/ResponseSpectrum/test/Iran-1978.txt", L"c:/ZETTools/ResponseSpectrum/test/Iran-1978.dtu");
	Test2(L"c:/ZETTools/ResponseSpectrum/test/Iran-1981.txt", L"c:/ZETTools/ResponseSpectrum/test/Iran-1981.dtu");

	return 0;
}

