//-----------------------------------------------------------------------------------------
// Оконные весовые функции для вычисления преобразования Фурье : implementation file
//-----------------------------------------------------------------------------------------
#include "stdafx.h"
#include "window_funcs.h"
#include "mathZet.h"
#include <Intel\ipp\ipps.h>

void Hanning_32f(float *wdst, int len)
{
	ippsZero_32f(wdst, len);
	for (int i = 0; i < len; i++)
	{
		wdst[i] = 0.5f * (1.0f - (float)(cos(2*M_PI*i/(len-1))));
	}
}

void Hanning_64f(double *wdst, int len)
{
	ippsZero_64f(wdst, len);
	for (int i = 0; i < len; i++)
	{
		wdst[i] = 0.5 * (1.0 - (cos(2 * M_PI*i / (len - 1))));
	}
}

void RifeVincent4_32f(float *wdst, int len)
{
	float a[5] = {35.0f, -56.0f, 28.0f, -8.0f, 1.0f};
	ippsZero_32f(wdst, len);
	for (int i = 0; i < len; i++)
	{
		for (int k = 0; k < 5; k++)
			wdst[i] += a[k]/128.0f * (float)cos(k*2*M_PI*i/(len-1));
	}
}

void RifeVincent4_64f(double *wdst, int len)
{
	double a[5] = { 35.0, -56.0, 28.0, -8.0, 1.0 };
	ippsZero_64f(wdst, len);
	for (int i = 0; i < len; i++)
		for (int k = 0; k < 5; k++)
			wdst[i] += a[k] / 128.0 * cos(k * 2 * M_PI*i / (len - 1));
}

void BlackmanHarris_3_67_32f(float *wdst, int len)
{
	//	3-х членное окно Блэкмана-Харриса с уровнем боковых лепестко -67 дБ
	double a[3] = {0.42323, -0.49364, 0.05677};
	ippsZero_32f(wdst, len);
	for (int i = 0; i < len; i++)
	{
		for (int k = 0; k < 3; k++)
			wdst[i] += (float)a[k] * (float)cos(k*2*M_PI*i/(len-1));
	}
}

void BlackmanHarris_3_67_64f(double *wdst, int len)
{
	//	3-х членное окно Блэкмана-Харриса с уровнем боковых лепестко -67 дБ
	double a[3] = { 0.42323, -0.49364, 0.05677 };
	ippsZero_64f(wdst, len);
	for (int i = 0; i < len; i++)
	{
		for (int k = 0; k < 3; k++)
			wdst[i] += a[k] * cos(k * 2 * M_PI*i / (len - 1));
	}
}

void BlackmanHarris_4_94_32f(float *wdst, int len)
{
	//	4-х членное окно Блэкмана-Харриса с уровнем боковых лепестко -92 дБ
	double a[4] = {0.35875, -0.48829, 0.14128, -0.01168};
	ippsZero_32f(wdst, len);
	for (int i = 0; i < len; i++)
	{
		for (int k = 0; k < 4; k++)
			wdst[i] += (float)a[k] * (float)cos(k*2*M_PI*i/(len-1));
	}
}

void BlackmanHarris_4_94_64f(double *wdst, int len)
{
	//	4-х членное окно Блэкмана-Харриса с уровнем боковых лепестко -92 дБ
	double a[4] = { 0.35875, -0.48829, 0.14128, -0.01168 };
	ippsZero_64f(wdst, len);
	for (int i = 0; i < len; i++)
	{
		for (int k = 0; k < 4; k++)
			wdst[i] += a[k] * cos(k * 2 * M_PI*i / (len - 1));
	}
}

void Nuttal_32f(float *wdst, int len)
{
	double a[4] = {0.355768, -0.487396, 0.144232, -0.012604};
	ippsZero_32f(wdst, len);
	for (int i = 0; i < len; i++)
	{
		for (int k = 0; k < 4; k++)
			wdst[i] += (float)a[k] * (float)cos(k*2*M_PI*i/(len-1));
	}
}

void Nuttal_64f(double *wdst, int len)
{
	double a[4] = { 0.355768, -0.487396, 0.144232, -0.012604 };
	ippsZero_64f(wdst, len);
	for (int i = 0; i < len; i++)
	{
		for (int k = 0; k < 4; k++)
			wdst[i] += a[k] * cos(k * 2 * M_PI*i / (len - 1));
	}
}

void BlackmanNuttal_32f(float *wdst, int len)
{
	double a[4] = {0.3635819, -0.4891755, 0.1365995, -0.0106411};
	ippsZero_32f(wdst, len);
	for (int i = 0; i < len; i++)
	{
		for (int k = 0; k < 4; k++)
			wdst[i] += (float)a[k] * (float)cos(k*2*M_PI*i/(len-1));
	}
}

void BlackmanNuttal_64f(double *wdst, int len)
{
	double a[4] = { 0.3635819, -0.4891755, 0.1365995, -0.0106411 };
	ippsZero_64f(wdst, len);
	for (int i = 0; i < len; i++)
	{
		for (int k = 0; k < 4; k++)
			wdst[i] += a[k] * cos(k * 2 * M_PI*i / (len - 1));
	}
}

void Flattop_32f(float *wdst, int len)
{
	//double a[5] = {0.21557895, -0.41663158, 0.277263158, -0.083578947, 0.006947368};
	double a[5] = {1.0, -1.93, 1.29, -0.388, 0.028};
	ippsZero_32f(wdst, len);
	for (int i = 0; i < len; i++)
	{
		for (int k = 0; k < 5; k++)
			wdst[i] += (float)a[k] * (float)cos(k*2*M_PI*i/(len-1));
	}
}

void Flattop_64f(double *wdst, int len)
{
	//double a[5] = {0.21557895, -0.41663158, 0.277263158, -0.083578947, 0.006947368};
	double a[5] = { 1.0, -1.93, 1.29, -0.388, 0.028 };
	ippsZero_64f(wdst, len);
	for (int i = 0; i < len; i++)
	{
		for (int k = 0; k < 5; k++)
			wdst[i] += a[k] * cos(k * 2 * M_PI*i / (len - 1));
	}
}

float EffectiveNoiseBandwidth_32f(float* data, int len)
{
	float* temp = ippsMalloc_32f(len);
	ippsSqr_32f(data, temp, len);
	float energy(0.f);
	float norma(0.f);
	float result(0.f);
	ippsSum_32f(temp, len, &energy, ippAlgHintAccurate);
	ippsSum_32f(data, len, &norma, ippAlgHintAccurate);
	norma = norma * norma;
	result = energy / norma * len;
	ippsFree(temp);
	temp = nullptr;
	return result;
}

double EffectiveNoiseBandwidth_64f(double* data, int len)
{
	double* temp = ippsMalloc_64f(len);
	ippsSqr_64f(data, temp, len);
	double energy(0.);
	double norma(0.);
	double result(0.);
	ippsSum_64f(temp, len, &energy);
	ippsSum_64f(data, len, &norma);
	norma = norma * norma;
	result = energy / norma * len;
	ippsFree(temp);
	temp = nullptr;
	return result;
}

float WindowWidth_32f(float* data, int len)
{
	float sum(0.);
	ippsSum_32f(data, len, &sum, ippAlgHintAccurate);
	return sum / len;
}

double WindowWidth_64f(double* data, int len)
{
	double sum(0.);
	ippsSum_64f(data, len, &sum);
	return sum / len;
}

float WindowEnergy_32f(float* data, int len)
{
	float* temp = ippsMalloc_32f(len);
	ippsSqr_32f(data, temp, len);
	float energy(0.);
	ippsSum_32f(temp, len, &energy, ippAlgHintAccurate);
	energy = energy / len;
	ippsFree(temp);
	temp = nullptr;
	return energy;
}

double WindowEnergy_64f(double* data, int len)
{
	double* temp = ippsMalloc_64f(len);
	ippsSqr_64f(data, temp, len);
	double energy(0.);
	ippsSum_64f(temp, len, &energy);
	energy = energy / len ;
	ippsFree(temp);
	temp = nullptr;
	return energy;
}