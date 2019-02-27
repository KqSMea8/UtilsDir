//-----------------------------------------------------------------------------------------
// Оконные весовые функции для вычисления преобразования Фурье : header file
//-----------------------------------------------------------------------------------------
#pragma once

#include "afxwin.h"
//#include <mathZet.h>
//#include <Intel\ipp\ipps.h>

void Hanning_32f(float *wdst, int len);
void Hanning_64f(double *wdst, int len);

void RifeVincent4_32f(float *wdst, int len);
void RifeVincent4_64f(double *wdst, int len);

void BlackmanHarris_3_67_32f(float *wdst, int len);
void BlackmanHarris_3_67_64f(double *wdst, int len);

void BlackmanHarris_4_94_32f(float *wdst, int len);
void BlackmanHarris_4_94_64f(double *wdst, int len);

void Nuttal_32f(float *wdst, int len);
void Nuttal_64f(double *wdst, int len);

void BlackmanNuttal_32f(float *wdst, int len);
void BlackmanNuttal_64f(double *wdst, int len);

void Flattop_32f(float *wdst, int len);
void Flattop_64f(double *wdst, int len);

float EffectiveNoiseBandwidth_32f(float* data, int len);
double EffectiveNoiseBandwidth_64f(double* data, int len);

float WindowWidth_32f(float* data, int len);
double WindowWidth_64f(double* data, int len);

float WindowEnergy_32f(float* data, int len);
double WindowEnergy_64f(double* data, int len);
