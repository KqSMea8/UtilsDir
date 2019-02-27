//------------------------------------------------------------------------------
// Spectre_Base.cpp : implementation file
//------------------------------------------------------------------------------
//	Чтение данных каждый канал выполняет за 0,1 сек, это одна порция.
// А кол-во порций для усредненния берётся в соответствии с заданным временем.
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "COMDEF.H"
#include "share.h"
#include "Spectre_Base.h"
#include <Intel\ipp\ipps.h>
#include <Intel\ipp\ippvm.h>
#include <Intel\ipp\ippcore.h>
#include "MMSystem.h"
#include <stdio.h>
#include <Interpolation/LinearInterpolation.h>
#include <Buffer\SmartArray.h>
//------------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
CString strGL;
#define TARGET_RESOLUTION 1         // 1-millisecond target resolution
#define MAX_SPECTR_LOG_SIZE  4096
UINT wTimerRes;
//------------------------------------------------------------------------------
UINT ThreadShield(LPVOID pParam)
{
	char lerr = 0;
	try 
	{
		while( Shield(1, &lerr) < 0 )
			Sleep(100);
	}
	catch (char *str)
	{
		UNREFERENCED_PARAMETER(str);
	}

	if ( lerr & 1 )
	{
		Sleep( 20 * abs(lerr) );
		::PostMessage(*((HWND*)pParam), WM_CLOSE, 0, 0);
	}
	return 0;
}
//------------------------------------------------------------------------------
void FormVes(MultiParamSelf* pmp)
{
	int j, order;

	if(pmp->analis == 0) 
		return;
	for(j = 0; j < pmp->size; j++)
	{
		pmp->prm3[j].re = (float)cos(j * M_PI / pmp->size*(double)(j));
		pmp->prm3[j].im = (float)sin(j * M_PI / pmp->size*(double)(j));
		pmp->dPrm3[j].re = cos(j * M_PI / pmp->size*(double)(j));
		pmp->dPrm3[j].im = sin(j * M_PI / pmp->size*(double)(j));
	}
	for(j = 0; j <= pmp->sizeforz/2; j++)
	{
		pmp->prm4[j].re = (float)cos(j * M_PI / pmp->size*(double)(j));
		pmp->prm4[j].im =-(float)sin(j * M_PI / pmp->size*(double)(j));
		pmp->dPrm4[j].re = cos(j * M_PI / pmp->size*(double)(j));
		pmp->dPrm4[j].im = -sin(j * M_PI / pmp->size*(double)(j));
		if (j != 0) 
		{
			pmp->prm4[pmp->sizeforz - j] = pmp->prm4[j];
			pmp->dPrm4[pmp->sizeforz - j] = pmp->dPrm4[j];
		}
	}
	order = (int)(log((double)pmp->sizeforz)/log((double)2.));

	//IppsFFTSpec_C_32fc* spec;
	IppsFFTSpec_C_32fc* pFftSpec_32fc(nullptr);
	Ipp8u* pFFTSpecBuf(nullptr), *pFFTInitBuf(nullptr), *pFFTWorkBuf(nullptr);
	int sizeFFTSpec(0), sizeFFTInitBuf(0), sizeFFTWorkBuf(0);
	//status = ippsFFTInitAlloc_C_32fc(&spec, order, IPP_FFT_DIV_INV_BY_N, ippAlgHintNone);
	ippsFFTGetSize_C_32fc(order, IPP_DIV_INV_BY_N, ippAlgHintNone, &sizeFFTSpec, &sizeFFTInitBuf, &sizeFFTWorkBuf);
	pFFTSpecBuf = ippsMalloc_8u(sizeFFTSpec);
	pFFTInitBuf = ippsMalloc_8u(sizeFFTInitBuf);
	pFFTWorkBuf = ippsMalloc_8u(sizeFFTWorkBuf);
	ippsFFTInit_C_32fc(&pFftSpec_32fc, order, IPP_DIV_INV_BY_N, ippAlgHintNone, pFFTSpecBuf, pFFTInitBuf);
	if (pFFTInitBuf)
		ippsFree(pFFTInitBuf);
	//status = ippsFFTFwd_CToC_32fc_I((Ipp32fc*)pmp->prm4.data(), spec, NULL);
	ippsFFTFwd_CToC_32fc_I((Ipp32fc*)pmp->prm4.data(), pFftSpec_32fc, pFFTWorkBuf);
	if (pFFTWorkBuf)
		ippsFree(pFFTWorkBuf);
	if (pFFTSpecBuf)
		ippsFree(pFFTSpecBuf);

	//IppsFFTSpec_C_64fc* dspec;
	IppsFFTSpec_C_64fc* pFftdspec_64fc(nullptr);
	//status = ippsFFTInitAlloc_C_64fc(&dspec, order, IPP_FFT_DIV_INV_BY_N, ippAlgHintNone);
	ippsFFTGetSize_C_64fc(order, IPP_DIV_INV_BY_N, ippAlgHintNone, &sizeFFTSpec, &sizeFFTInitBuf, &sizeFFTWorkBuf);
	pFFTSpecBuf = ippsMalloc_8u(sizeFFTSpec);
	pFFTInitBuf = ippsMalloc_8u(sizeFFTInitBuf);
	pFFTWorkBuf = ippsMalloc_8u(sizeFFTWorkBuf);
	ippsFFTInit_C_64fc(&pFftdspec_64fc, order, IPP_DIV_INV_BY_N, ippAlgHintNone, pFFTSpecBuf, pFFTInitBuf);
	if (pFFTInitBuf)
		ippsFree(pFFTInitBuf);
	//status = ippsFFTFwd_CToC_64fc_I((Ipp64fc*)pmp->dPrm4.data(), dspec, NULL);
	ippsFFTFwd_CToC_64fc_I((Ipp64fc*)pmp->dPrm4.data(), pFftdspec_64fc, pFFTWorkBuf);
	if (pFFTWorkBuf)
		ippsFree(pFFTWorkBuf);
	if (pFFTSpecBuf)
		ippsFree(pFFTSpecBuf);
}
//------------------------------------------------------------------------------
void FormIntegr(MultiParamSelf* pmp, double channelFreq)
{
	int i(0);
	double freq(0.);

	if (pmp->integr.empty())
		return;

	if (pmp->dIntegr.empty())
		return;

	pmp->integr[0] = 1.;
	pmp->dIntegr[0] = 1.;

	freq = channelFreq * pow(0.1, pmp->decade) / pmp->size * 2. * M_PI; // изначально channelFreq = mypr->spServer->Frequency(mypr->channel)
	for(i = 1; i < pmp->size/2 + 1; i++)
	{
		pmp->integr[i] = 1.;		//case 2
		switch(pmp->integral)
		{
		case 0:
			pmp->integr[i] = (float)pow(freq*i,4.);
			pmp->dIntegr[i] = pow(freq*i, 4.);
			break;
		case 1:
			pmp->integr[i] = (float)pow(freq*i,2.);
			pmp->dIntegr[i] = pow(freq*i, 2.);
			break;
		case 3:
			pmp->integr[i] = (float)(1./pow(freq*i,2.));
			pmp->dIntegr[i] = 1. / pow(freq*i, 2.);
			break;
		case 4:
			pmp->integr[i] = (float)(1./pow(freq*i,4.));
			pmp->dIntegr[i] = 1. / pow(freq*i, 4.);
			break;
		}
	}
	pmp->integr[0] = pmp->integr[1]/10000000.f;
	pmp->dIntegr[0] = pmp->dIntegr[1] / 10000000.f;
}
//------------------------------------------------------------------------------
int InitWind(MultiParamSelf* pmp)
{
	int i, size;

	size = pmp->size;
	if (size <= 0)
	{
		size = 2;
	}
	ippsSet_32f(1.0f, pmp->win.data(), size);
	ippsSet_64f(1.0f, pmp->dWin.data(), size);
	switch (pmp->wind)
	{
	case 0:			//прямое окно
		ippsSet_32f(1.f, pmp->win.data(), size);
		ippsSet_64f(1.0, pmp->dWin.data(), size);
		break;
	case 1:
		ippsWinHann_32f_I(pmp->win.data(), size);
		ippsWinHann_64f_I(pmp->dWin.data(), size);
		break;
	case 2:
		Hanning_32f(pmp->win.data(), size);
		Hanning_64f(pmp->dWin.data(), size);
		break;
	case 3:
		ippsWinHamming_32f_I(pmp->win.data(), size);
		ippsWinHamming_64f_I(pmp->dWin.data(), size);
		break;
	case 4:
		ippsWinBlackmanOpt_32f_I(pmp->win.data(), size);
		ippsWinBlackmanOpt_64f_I(pmp->dWin.data(), size);
		break;
	case 5:
		ippsWinBartlett_32f_I(pmp->win.data(), size);
		ippsWinBartlett_64f_I(pmp->dWin.data(), size);
		break;
	case 6:
		ippsWinKaiser_32f_I(pmp->win.data(), size, 1.0f);
		ippsWinKaiser_64f_I(pmp->dWin.data(), size, 1.0f);
		break;
	case 7 :
		RifeVincent4_32f(pmp->win.data(), size);
		RifeVincent4_64f(pmp->dWin.data(), size);
		break;
	case 8 :
		BlackmanHarris_3_67_32f(pmp->win.data(), size);
		BlackmanHarris_3_67_64f(pmp->dWin.data(), size);
		break;
	case 9 :
		BlackmanHarris_4_94_32f(pmp->win.data(), size);
		BlackmanHarris_4_94_64f(pmp->dWin.data(), size);
		break;
	case 10 :
		Nuttal_32f(pmp->win.data(), size);
		Nuttal_64f(pmp->dWin.data(), size);
		break;
	case 11 :
		BlackmanNuttal_32f(pmp->win.data(), size);
		BlackmanNuttal_64f(pmp->dWin.data(), size);
		break;
	case 12 :
		Flattop_32f(pmp->win.data(), size);
		Flattop_64f(pmp->dWin.data(), size);
		break;
	}

	pmp->ewind = 0.;
	pmp->swind = 0.;
	for(i = 0; i < size; i++)
	{
		pmp->swind += pmp->dWin[i];
		pmp->ewind += pmp->dWin[i] * pmp->dWin[i];
	}
	pmp->swind = pmp->swind / size;
	pmp->ewind = pmp->ewind / size / pmp->swind / pmp->swind;
	
	return 0;
}
//------------------------------------------------------------------------------
int SpectrFFT( MultiParamSelf* pmp, float* mass, CalcDataStruct *calcData, SaveStruct* saveData, WorkParams* workData, double channelFreq, double channelMinLevel, double channeltime, bool isCurrentChannel)
	//				входной ,для кепстра, линейный масштаб для сред., результат текущий
{
	LARGE_INTEGER qpFreq;
	LARGE_INTEGER qpCounter1, qpCounter2;
	QueryPerformanceCounter(&qpCounter1);
	QueryPerformanceFrequency(&qpFreq);

	int order(0);
	int realsize(0);
	int size(0), sizeforz(0);
	int result(0);
	int i(0), numusr(0);
	Ipp64f dPopravka, dLogRef;
	float dt(0.f), srednee(0.f);
	double dsrednee(0.f);
	double adel(0.), del(0.), freq(0.);
	IppStatus ippReturn;

	realsize = pmp->size / 2;
	size = pmp->size;					//круглое число < sizeforz/2
	sizeforz = pmp->sizeforz;			//2 в степени n
	freq = channelFreq * pow(0.1, pmp->decade); // надо определить channelFreq = mypr->spServer->Frequency(mypr->channel)
	dt = (float)(pmp->sdvig / freq);
	numusr = (int)(pmp->usred * freq / pmp->sdvig + 0.5);
	if (numusr == 0) 
		numusr = 1;

	CString strspam(L"");

	if (pmp->typeusred == 0)		//линейное усреднение
	{
		adel = 1.0;
		del = 1.0 / numusr;
		if (pmp->tempsred == 0) 
		{
			//ippsZero_32f(pmp->uspectr.data(), realsize); // заменил цикл на вызов функции, посмотрим, будет ли работать
			ippsZero_64f(pmp->dUsrSpectr.data(), realsize); // заменил цикл на вызов функции, посмотрим, будет ли работать
		}
		pmp->tempsred += dt;
		strspam.Format(L"Spectr calcFFT   time %f   usred %.0f / %.0f ", channeltime, pmp->tempsred, pmp->usred);
		if (pmp->tempsred + dt / 2.f >= pmp->usred)
		{
			result = 1;
			pmp->tempsred = 0;
			strspam.AppendFormat(L"result\n");
		}
		else 
		{
			result = 0;
			strspam.AppendFormat(L"waiting\n");
		}
		//OutputDebugString(strspam);
	}
	else							//экспоненциальное усреднение
	{
		adel = pow(0.1, 1 / (pmp->usred * freq / pmp->sdvig));
		del = 1 - adel;
		result = 1;
	}

	ippsConvert_32f64f(mass, pmp->dSignal.data(), size);

	//удаление постоянной составляющей
	//ippsMean_32f(mass, size, &srednee, ippAlgHintFast);
	//ippsAddC_32f_I(-srednee, mass, size);		//весовое окно
	//ippsMul_32f_I(pmp->win.data(), mass, size);

	ippsMean_64f(pmp->dSignal.data(), size, &dsrednee);
	ippsAddC_64f_I(-dsrednee, pmp->dSignal.data(), size);		//весовое окно
	ippsMul_64f_I(pmp->dWin.data(), pmp->dSignal.data(), size);

	if(pmp->analis == 0)				//FFT, то есть БПФ
	{
		order = (int)(log((double)pmp->size) / log((double)2.));
		TRACE("SPECTR FFT order %d size %d\n", order, pmp->size);
		//формируем комплексный массив с нулевой мнимой частью
		//ippsRealToCplx_32f(mass, calcData->massivImag.data(), pmp->spectr.data(), pmp->size);
		ippsRealToCplx_64f(pmp->dSignal.data(), calcData->dMassivImag.data(), pmp->dSpectr.data(), pmp->size);
		//Ipp8u* pFFTSpecBuf(nullptr), *pFFTInitBuf(nullptr), *pFFTWorkBuf(nullptr);
		//int sizeFFTSpec(0), sizeFFTInitBuf(0), sizeFFTWorkBuf(0);
		//если specSpectr нужно проинициализировать
		if (pmp->pFftSpecSpectr_64fc == nullptr)
		{
			Ipp8u* pFFTInitBuf(nullptr);
			int sizeFFTSpec(0), sizeFFTInitBuf(0), sizeFFTWorkBuf(0);
			//ippsFFTInitAlloc_C_64fc(&pmp->pFftSpecSpectr_64fc, order, IPP_FFT_DIV_INV_BY_N, ippAlgHintNone);
			ippsFFTGetSize_C_64fc(order, IPP_DIV_INV_BY_N, ippAlgHintNone, &sizeFFTSpec, &sizeFFTInitBuf, &sizeFFTWorkBuf);
			pmp->pFFTSpecSpectrBuf = ippsMalloc_8u(sizeFFTSpec);
			pFFTInitBuf = ippsMalloc_8u(sizeFFTInitBuf);
			pmp->pFFTWorkSpectrBuf = ippsMalloc_8u(sizeFFTWorkBuf);
			ippsFFTInit_C_64fc(&pmp->pFftSpecSpectr_64fc, order, IPP_DIV_INV_BY_N, ippAlgHintNone, pmp->pFFTSpecSpectrBuf, pFFTInitBuf);
			if (pFFTInitBuf)
				ippsFree(pFFTInitBuf);
		}
		if (pmp->pFftSpecSpectr_64fc != nullptr)
		{ 
			//ippReturn = ippsFFTFwd_CToC_64fc_I((Ipp64fc*)pmp->dSpectr.data(), pmp->pFftSpecSpectr_64fc, NULL);
			ippReturn = ippsFFTFwd_CToC_64fc_I((Ipp64fc*)pmp->dSpectr.data(), pmp->pFftSpecSpectr_64fc, pmp->pFFTWorkSpectrBuf);
		}
		//спектр мощности
		//ippsPowerSpectr_32fc(pmp->spectr.data(), mass, realsize);
		ippsPowerSpectr_64fc(pmp->dSpectr.data(), pmp->dSignal.data(), realsize);
	}             
	else        //Z-transform, то есть ДПФ
	{
		//	делаем из двух вещественных массивов один комплексный
		//ippsRealToCplx_32f(mass, calcData->massivImag.data(), pmp->prm2.data(), size);
		ippsRealToCplx_64f(pmp->dSignal.data(), calcData->dMassivImag.data(), pmp->dPrm2.data(), size);

		//	обнуляем элементы комплексного массива после необходимого количества отсчётов
		if(sizeforz > size)
		{
			//ippsZero_32fc(pmp->prm2.data() + size, sizeforz - size);
			ippsZero_64fc(pmp->dPrm2.data() + size, sizeforz - size);
		}

		//ippsMul_32fc_I(pmp->prm3.data(), pmp->prm2.data(), size);
		ippsMul_64fc_I(pmp->dPrm3.data(), pmp->dPrm2.data(), size);

		//	вычисляем достаточный для преобразования Фурье порядок массива
		order = (int)(log((double)pmp->sizeforz)/log((double)2.));

		//	подготавливаем к вычилсению алгоритм преобразования Фурье
		//status1 = ippsFFTInitAlloc_C_32fc(&spec, order, IPP_FFT_DIV_INV_BY_N, ippAlgHintNone );
		if (pmp->pFftSpecSpectr_64fc == nullptr)
		{
			Ipp8u* pFFTInitBuf(nullptr);
			int sizeFFTSpec(0), sizeFFTInitBuf(0), sizeFFTWorkBuf(0);
			//status1 = ippsFFTInitAlloc_C_64fc(&dspec, order, IPP_FFT_DIV_INV_BY_N, ippAlgHintNone);
			ippsFFTGetSize_C_64fc(order, IPP_DIV_INV_BY_N, ippAlgHintNone, &sizeFFTSpec, &sizeFFTInitBuf, &sizeFFTWorkBuf);
			pmp->pFFTSpecSpectrBuf = ippsMalloc_8u(sizeFFTSpec);
			pFFTInitBuf = ippsMalloc_8u(sizeFFTInitBuf);
			pmp->pFFTWorkSpectrBuf = ippsMalloc_8u(sizeFFTWorkBuf);
			ippsFFTInit_C_64fc(&pmp->pFftSpecSpectr_64fc, order, IPP_DIV_INV_BY_N, ippAlgHintNone, pmp->pFFTSpecSpectrBuf, pFFTInitBuf);
			if (pFFTInitBuf)
				ippsFree(pFFTInitBuf);
		}

		//	выполнение преобразования Фурье
		//status2 = ippsFFTFwd_CToC_64fc_I((Ipp64fc*)pmp->dPrm2.data(), dspec, NULL);
		if (pmp->pFftSpecSpectr_64fc != nullptr)
			ippsFFTFwd_CToC_64fc_I((Ipp64fc*)pmp->dPrm2.data(), pmp->pFftSpecSpectr_64fc, pmp->pFFTWorkSpectrBuf);
		
		//ippsMul_32fc_I(pmp->prm4.data(), pmp->prm2.data(), sizeforz);
		ippsMul_64fc_I(pmp->dPrm4.data(), pmp->dPrm2.data(), sizeforz);

		//	подготавливаем к вычислению алгоритм преобразования Фурье
		//status1 = //очищаем то что выделили под fftippsFFTInitAlloc_C_64fc(&pFftdspec_64fc, order, IPP_FFT_DIV_FWD_BY_N, ippAlgHintNone);
		if (pmp->pFftSpecSpectr_64fc2 == nullptr)
		{
			Ipp8u* pFFTInitBuf(nullptr);
			int sizeFFTSpec(0), sizeFFTInitBuf(0), sizeFFTWorkBuf(0);
			ippsFFTGetSize_C_64fc(order, IPP_DIV_FWD_BY_N, ippAlgHintNone, &sizeFFTSpec, &sizeFFTInitBuf, &sizeFFTWorkBuf);
			pmp->pFFTSpecSpectrBuf2 = ippsMalloc_8u(sizeFFTSpec);
			pFFTInitBuf = ippsMalloc_8u(sizeFFTInitBuf);

			ippsFFTInit_C_64fc(&pmp->pFftSpecSpectr_64fc2, order, IPP_DIV_FWD_BY_N, ippAlgHintNone, pmp->pFFTSpecSpectrBuf2, pFFTInitBuf);
			if (pFFTInitBuf)
				ippsFree(pFFTInitBuf);
		}
		//	выполнение преобрзования Фурье (ошибка происходит где-то здесь)
		//status2 = ippsFFTFwd_CToC_64fc_I((Ipp64fc*)pmp->dPrm2.data(), pFftdspec_64fc, NULL);
		if (pmp->pFftSpecSpectr_64fc2 != nullptr)
			ippsFFTFwd_CToC_64fc_I((Ipp64fc*)pmp->dPrm2.data(), pmp->pFftSpecSpectr_64fc2, pmp->pFFTWorkSpectrBuf);
		
		//ippsPowerSpectr_32fc(pmp->prm2.data(), mass, realsize);
		ippsPowerSpectr_64fc(pmp->dPrm2.data(), pmp->dSignal.data(), realsize);
	}	
	//popravka = size * (float)pmp->swind;
	//popravka = popravka * popravka;
	//popravka = (float)(1./popravka*del);
	dPopravka = size * pmp->swind;
	dPopravka = dPopravka * dPopravka;
	dPopravka = (1. / dPopravka * del);
	switch (pmp->spm)
	{
	case 2:
		//popravka = (float)(2.*popravka / (pmp->ewind*freq / size));
		dPopravka = 2.*dPopravka / (pmp->ewind * freq / size);
		break;
	case 3:
		//popravka = (float)(2.*popravka / (pmp->ewind*freq / size));
		dPopravka = 2.*dPopravka / (pmp->ewind * freq / size);
		break;
	case 0:
		//popravka = (float)2.*popravka;
		dPopravka = 2.*dPopravka;
		break;
	case 1:
		//popravka = (float)4.*popravka;
		dPopravka = 4.*dPopravka;
		break;
	}

	if (pmp->integral != 2)					//интегрирование или дифференцирование
	{
		//ippsMul_32f_I(pmp->integr.data(), mass, realsize);
		ippsMul_64f_I(pmp->dIntegr.data(), pmp->dSignal.data(), realsize);
		//popravka = pmp->mnoj*popravka;
		dPopravka = pmp->mnoj * dPopravka;
	}
	//ippsMulC_32f_I(popravka, mass, realsize);
	ippsMulC_64f_I(dPopravka, pmp->dSignal.data(), realsize);

	//ippsMulC_32f_I((float)adel, pmp->uspectr.data(), realsize);
	ippsMulC_64f_I(adel, pmp->dUsrSpectr.data(), realsize);

	//uspectr текущий средний спектр
	//ippsAdd_32f_I(mass, pmp->uspectr.data(), realsize);
	ippsAdd_64f_I(pmp->dSignal.data(), pmp->dUsrSpectr.data(), realsize);

	if (result != 0)
	{
		//ippsMul_32f_I(pmp->popspectr.data(), pmp->uspectr.data(), realsize);
		ippsMul_64f_I(pmp->dPopSpectr.data(), pmp->dUsrSpectr.data(), realsize);

		// TODO : необходимо разобраться с вычислением средненго спектра и сделать его аналогично максимальному

		if (pmp->bStartInterval)
			pmp->indexsred++;

		if (pmp->linlog == 0)
		{
			if (pmp->spm != 3)
				//ippsSqrt_32f(pmp->uspectr.data(), pmp->pspectr.data(), realsize);
				ippsSqrt_64f(pmp->dUsrSpectr.data(), pmp->dPrmSpectr.data(), realsize);
			else
				//ippsCopy_32f(pmp->uspectr.data(), pmp->pspectr.data(), realsize);
				ippsCopy_64f(pmp->dUsrSpectr.data(), pmp->dPrmSpectr.data(), realsize);
		}
		else
		{
			//ippsThreshold_32f_I(pmp->uspectr.data(), realsize, (float)(channelMinLevel/100000000000000000.), ippCmpLess); // здесь значение переменной channelMinLevel = mypr->spServer->MinLevel(mypr->channel)
			//ippsLn_32f(pmp->uspectr.data(), pmp->pspectr.data(), realsize);
			//ippsMulC_32f_I(10. * log10(M_E) / log10(10.0), pmp->pspectr.data(), realsize);
			ippsThreshold_64f_I(pmp->dUsrSpectr.data(), realsize, channelMinLevel / 1e+18, ippCmpLess); // здесь значение переменной channelMinLevel = mypr->spServer->MinLevel(mypr->channel)
			ippsLn_64f(pmp->dUsrSpectr.data(), pmp->dPrmSpectr.data(), realsize);
			ippsMulC_64f_I(10. * log10(M_E) / log10(10.0), pmp->dPrmSpectr.data(), realsize);

			//logref = pmp->reference;
			//if (logref <= 0.) 
			//	logref = (float)0.000001;
			//logref = -(float)(log10(logref)*20.);
			//ippsAddC_32f_I(logref, pmp->pspectr.data(), realsize);
			dLogRef = pmp->reference;
			if (dLogRef <= 0.)
				dLogRef = 0.000001;
			dLogRef = -(log10(dLogRef)*20.);
			ippsAddC_64f_I(dLogRef, pmp->dPrmSpectr.data(), realsize);
		}
	}

	if (workData->GRAPH_UPOR && pmp->unis)
	{
		if (pmp->integral == 2)
		{
			for (i = 0; i < realsize; i++)
			{
				//if (pmp->pspectr[i] < pmp->unis)
				//	pmp->pspectr[i] = pmp->unis;
				if (pmp->dPrmSpectr[i] < pmp->unis)
					pmp->dPrmSpectr[i] = pmp->unis;
			}
		}
	}



	QueryPerformanceCounter(&qpCounter2);
	//TRACE("SPECTR FFT: time = %d\t size = %d\t calc = %6.6f\n", GetTickCount(), size, (qpCounter2.LowPart - qpCounter1.LowPart)/(float)qpFreq.LowPart);

	return result;
}
//------------------------------------------------------------------------------
int SredSpectr(MultiParamSelf* pmp, WorkParams* workData, double channelMinLevel)
{
	int realsize;

	if (pmp->bStartInterval)
	{
		realsize = pmp->size/2+1;

		if (pmp->linlog == 0)
		{
			//ippsAdd_32f_I(pmp->pspectr.data(), pmp->sspectr.data(), realsize);
			//ippsMulC_32f(pmp->sspectr.data(), (float)(1. / pmp->indexsred), pmp->stspectr.data(), realsize);
			ippsAdd_64f_I(pmp->dPrmSpectr.data(), pmp->dSrdSpectr.data(), realsize);
			ippsMulC_64f(pmp->dSrdSpectr.data(), (float)(1. / pmp->indexsred), pmp->dStsSpectr.data(), realsize);
		}
		else
		{
			//ippsDivC_32f(pmp->pspectr.data(), 20.f, pmp->uspectr.data(), realsize);
			//ippsExp_32f_I(pmp->uspectr.data(), realsize);
			//ippsAdd_32f_I(pmp->uspectr.data(), pmp->sspectr.data(), realsize);
			//ippsMulC_32f(pmp->sspectr.data(), (float)(1. / pmp->indexsred), pmp->stspectr.data(), realsize);
			//ippsLn_32f_I(pmp->stspectr.data(), realsize);
			//ippsMulC_32f_I(20.f, pmp->stspectr.data(), realsize);
			ippsDivC_64f(pmp->dPrmSpectr.data(), 20.f, pmp->dUsrSpectr.data(), realsize);
			ippsExp_64f_I(pmp->dUsrSpectr.data(), realsize);
			ippsAdd_64f_I(pmp->dUsrSpectr.data(), pmp->dSrdSpectr.data(), realsize);
			ippsMulC_64f(pmp->dSrdSpectr.data(), (float)(1. / pmp->indexsred), pmp->dStsSpectr.data(), realsize);
			ippsLn_64f_I(pmp->dStsSpectr.data(), realsize);
			ippsMulC_64f_I(20.f, pmp->dStsSpectr.data(), realsize);
		}
		// TODO : всвязи с исправлениями логики работы линейные или децибельные значения вычисляются самим спектром
		// поэтому в децибеллах мы на самом деле получаем среднее гармоническое
		return 1;
	}
	return 0;
}
//------------------------------------------------------------------------------
int MaxSpectr( MultiParamSelf* pmp, SaveStruct* saveData)
{
	if (pmp->bStartInterval)
	{
		for (int i = 0; i < pmp->size/2; i++)
		{
			//if (pmp->mspectr[i] < pmp->pspectr[i]) 
			//	pmp->mspectr[i] = pmp->pspectr[i];
			if (pmp->dMaxSpectr[i] < pmp->dPrmSpectr[i])
				pmp->dMaxSpectr[i] = pmp->dPrmSpectr[i];
		}
		return 1;
	}
	return 0;
}
//------------------------------------------------------------------------------
int InverseCalc(MultiParamSelf* pmp, CalcDataStruct *calcData, SaveStruct* saveData, WorkParams* workData)
{
	ippsZero_32f(calcData->kspectr.data(), calcData->kspectr.size());
	ippsSet_32f(1.f, calcData->kspectr.data(), pmp->size / 2);
	ippsDiv_32f(pmp->uspectr.data(), calcData->kspectr.data(), calcData->massivImag.data(), pmp->size / 2);
	ippsSqr_32f(calcData->massivImag.data(), calcData->kspectr.data() + pmp->size / 2, pmp->size / 2);
	ippsSqr_32f(calcData->freqm.data(), calcData->massivImag.data() + pmp->size / 2, pmp->size / 2);
	float dx = calcData->freqm[1] + calcData->freqm[pmp->size / 2 - 1];
	
	for (long i = 0; i < pmp->size / 2; i++)
		calcData->massivImag[i] = dx * dx * i / pmp->size * 2;;

	CLinearInterpolation<float> interpol;
	interpol.InitInterpolation(tiLinear);
	interpol.Interpolation(calcData->massivImag.data() + pmp->size / 2, calcData->kspectr.data() + pmp->size / 2, pmp->size / 2,
		calcData->massivImag.data(), calcData->kspectr.data(), pmp->size / 2);
	ippsSqrt_32f_I(calcData->kspectr.data(), pmp->size / 2);

	return 0;
}
//------------------------------------------------------------------------------
int KepstrCalc(MultiParamSelf* pmp, CalcDataStruct *calcData, SaveStruct* saveData, WorkParams* workData, double channelMinLevel)
{
	int order,i;
	float logref;

	ippsZero_32f(calcData->kspectr.data(), calcData->kspectr.size());
	ippsThreshold_32f_I(pmp->uspectr.data(), pmp->size/2, (float)(channelMinLevel/100000000000000.), ippCmpLess); // значение переменной channelMinLevel = mypr->spServer->MinLevel(mypr->channel)

	ippsLn_32f(pmp->uspectr.data(), calcData->kspectr.data(), pmp->size/2);
	ippsMulC_32f_I(10.f, calcData->kspectr.data(), pmp->size/2);

	logref = pmp->reference;
	if (logref <= 0.f) 
		logref = 0.000001f;
	logref = -(float)(log10(logref)*20.);

	ippsAddC_32f_I(logref, calcData->kspectr.data(), pmp->size/2);

	if (pmp->analis == 0)				//FFT
	{
		order = (int)(log((double)pmp->size/2.)/log((double)2.)+0.5);

		//формируем комплексный массив с нулевой мнимой частью
		ippsRealToCplx_32f(calcData->kspectr.data(), calcData->massivImag.data(), pmp->spectr.data(), pmp->size);
		//ippsFFTInitAlloc_C_32fc(&pFftkepstr_spec_32fc, order, IPP_FFT_DIV_FWD_BY_N, ippAlgHintNone);
		if (pmp->pFftSpecKepstr_32fc != nullptr)
		{
			Ipp8u* pFFTInitBuf(nullptr);
			int sizeFFTSpec(0), sizeFFTInitBuf(0), sizeFFTWorkBuf(0);
			ippsFFTGetSize_C_32fc(order, IPP_DIV_FWD_BY_N, ippAlgHintNone, &sizeFFTSpec, &sizeFFTInitBuf, &sizeFFTWorkBuf);
			pmp->pFFTSpecKepstrBuf = ippsMalloc_8u(sizeFFTSpec);
			pFFTInitBuf = ippsMalloc_8u(sizeFFTInitBuf);
			pmp->pFFTWorkKepstrBuf = ippsMalloc_8u(sizeFFTWorkBuf);
			ippsFFTInit_C_32fc(&pmp->pFftSpecKepstr_32fc, order, IPP_DIV_FWD_BY_N, ippAlgHintNone, pmp->pFFTSpecKepstrBuf, pFFTInitBuf);
			if (pFFTInitBuf)
				ippsFree(pFFTInitBuf);
		}
		//ippsFFTFwd_CToC_32fc_I((Ipp32fc*)pmp->spectr.data(), pFftkepstr_spec_32fc, NULL);
		if (pmp->pFftSpecKepstr_32fc != nullptr)
			ippsFFTFwd_CToC_32fc_I((Ipp32fc*)pmp->spectr.data(), pmp->pFftSpecKepstr_32fc, pmp->pFFTWorkKepstrBuf);
		
		ippsPowerSpectr_32fc(pmp->spectr.data(), calcData->kspectr.data(), pmp->size/4);
	}             
	else        //Z-transform  
	{
		for(i = 0; i < pmp->size/4; i++)
		{
			pmp->prm2[i].re = calcData->kspectr[i];
			pmp->prm2[i].im = 0;
		}
		for(i = pmp->size/4; i < pmp->sizeforz/2; i++) 
		{
			pmp->prm2[i].re = 0;
			pmp->prm2[i].im = 0;
		}
		ippsMul_32fc_I(pmp->prm3.data(), pmp->prm2.data(), pmp->size/2);

		order = (int)(log((double)pmp->sizeforz/2.)/log((double)2.)+0.5);

		//ippsFFTInitAlloc_C_32fc(&spec, order, IPP_FFT_DIV_INV_BY_N, ippAlgHintNone );
		if (pmp->pFftSpecKepstr_32fc == nullptr)
		{
			Ipp8u* pFFTInitBuf(nullptr);
			int sizeFFTSpec(0), sizeFFTInitBuf(0), sizeFFTWorkBuf(0);
			ippsFFTGetSize_C_32fc(order, IPP_DIV_INV_BY_N, ippAlgHintNone, &sizeFFTSpec, &sizeFFTInitBuf, &sizeFFTWorkBuf);
			pmp->pFFTSpecKepstrBuf = ippsMalloc_8u(sizeFFTSpec);
			pFFTInitBuf = ippsMalloc_8u(sizeFFTInitBuf);
			pmp->pFFTWorkKepstrBuf = ippsMalloc_8u(sizeFFTWorkBuf);
			ippsFFTInit_C_32fc(&pmp->pFftSpecKepstr_32fc, order, IPP_DIV_INV_BY_N, ippAlgHintNone, pmp->pFFTSpecKepstrBuf, pFFTInitBuf);
			if (pFFTInitBuf)
				ippsFree(pFFTInitBuf);
		}
		//ippsFFTFwd_CToC_32fc_I((Ipp32fc*)pmp->prm2.data(), spec, NULL);
		if (pmp->pFftSpecKepstr_32fc != nullptr)
			ippsFFTFwd_CToC_32fc_I((Ipp32fc*)pmp->prm2.data(), pmp->pFftSpecKepstr_32fc, pmp->pFFTWorkKepstrBuf);
		
		ippsMul_32fc_I(pmp->prm4.data(), pmp->prm2.data(), pmp->sizeforz/2);
				
		//ippsFFTInitAlloc_C_32fc(&spec, order, IPP_FFT_DIV_FWD_BY_N, ippAlgHintNone );
		if (pmp->pFftSpecKepstr_32fc2 == nullptr)
		{
			Ipp8u* pFFTInitBuf(nullptr);
			int sizeFFTSpec(0), sizeFFTInitBuf(0), sizeFFTWorkBuf(0);
			ippsFFTGetSize_C_32fc(order, IPP_DIV_FWD_BY_N, ippAlgHintNone, &sizeFFTSpec, &sizeFFTInitBuf, &sizeFFTWorkBuf);
			pmp->pFFTSpecKepstrBuf2 = ippsMalloc_8u(sizeFFTSpec);
			pFFTInitBuf = ippsMalloc_8u(sizeFFTInitBuf);
			ippsFFTInit_C_32fc(&pmp->pFftSpecKepstr_32fc2, order, IPP_DIV_FWD_BY_N, ippAlgHintNone, pmp->pFFTSpecKepstrBuf2, pFFTInitBuf);
			if (pFFTInitBuf)
				ippsFree(pFFTInitBuf);
		}
		//ippsFFTFwd_CToC_32fc_I((Ipp32fc*)pmp->prm2.data(), spec, NULL);
		if (pmp->pFftSpecKepstr_32fc2 != nullptr)
			ippsFFTFwd_CToC_32fc_I((Ipp32fc*)pmp->prm2.data(), pmp->pFftSpecKepstr_32fc2, pmp->pFFTWorkKepstrBuf);
		
		ippsPowerSpectr_32fc(pmp->prm2.data(), calcData->kspectr.data(), pmp->size / 4);
	}

	ippsThreshold_32f_I(calcData->kspectr.data(), pmp->size/4, (float)(channelMinLevel/100000000000000000.), ippCmpLess);// значение переменной channelMinLevel = mypr->spServer->MinLevel(mypr->channel)

	ippsLn_32f_I(calcData->kspectr.data(), pmp->size / 4);
	ippsMulC_32f_I((float)(10. * log10(M_E)), calcData->kspectr.data(), pmp->size / 4);

	// обнуляем первое значение, чтобы не зашкаливал график
	calcData->kspectr[0] = 0;

	// обнуляем все значения после pmp->size / 4, так как они на графике не рисуются, но мешают ему масштабироваться
	ippsZero_32f(calcData->kspectr.data() + pmp->size / 4, calcData->kspectr.size() - pmp->size / 4);

	logref = pmp->reference;
	if (logref <= 0.f) 
		logref = 0.000001f;
	logref = -(float)(log10(logref)*20.);

	return 0;
}
//------------------------------------------------------------------------------
int Calculation(MultiChannelSelf* pmChan, MultiParamSelf *pmp, SaveStruct* saveData, WorkParams* workData, CalcDataStruct* calcData, CDSRV* server, bool isCurChannel, bool &all_nan)
{
	int res;
	_bstr_t cmnt;
	int channel = pmChan->Channel;
	int decade = pmChan->Decade;
	long size = pmChan->NumberCounts;
	double sdvig = pmp->sdvig;
	float minLevel = server->MinLevel(channel);
	float* massiv = pmChan->pData;

	pmp->resultStatus.ResetStatus();

	int order;
	IppStatus st = ippsMaxOrder_32f(massiv, size, &order);

	if (pmp->bStartInterval)		
		pmp->timer += pmp->timedelay;

	if (st != ippStsNanArg)
	{
		all_nan = false;
		pmp->resultStatus.spectreSuccess = SpectrFFT(pmp, massiv, calcData, saveData, workData, pmChan->freqADC, minLevel, pmChan->DataTime.GetTime(), isCurChannel) != 0; 
	}
	else
	{
		all_nan = true;
	}

	pmp->cycle += pmp->timedelay;
	if (st != ippStsNanArg) // если все-таки в сигнале нанов нет, то все отрабатываем по старой схеме
	if (pmp->resultStatus.spectreSuccess)
	{
		if (pmp->clean != 0) 
		{
			//ippsFilterMedian_32f_I(pmp->pspectr.data(), pmp->size/2, 17);
			int sizeBuf(0);
			int maskSize(17);
			static SmartArray<Ipp8u> sa_8u;

			ippsFilterMedianGetBufferSize(maskSize, ipp32f, &sizeBuf);
			sa_8u.ReSize(sizeBuf);
			ippsFilterMedian_32f_I(pmp->pspectr.data(), pmp->size / 2, 17, nullptr, nullptr, sa_8u.pData);

		}

		if (pmp->cycle >= 0.099)
		{
			pmp->cycle = 0;
				
			if (pmp->showMaxGrf != 0)
			{
				pmp->resultStatus.maxSpectreSuccess = MaxSpectr(pmp, saveData) != 0; 
			}

			if (pmp->showSredGrf != 0)
			{
				pmp->resultStatus.sredSpectreSuccess = SredSpectr(pmp, workData, minLevel) != 0; 
			}
				
			if (pmp->bStartInterval)
			{
				if (pmp->timer + pmp->timedelay/2 >= pmp->interval)
				{
					pmp->timer = pmp->interval;
					pmp->bStartInterval = false;
					//TRACE("%d Calculation bStartInterval %d\n", pmp->linlog, pmp->bStartInterval);
				}
			}
		}	

		// пробразуем массивы типа double в тип float
		ippsConvert_64f32f(pmp->dMaxSpectr.data(), pmp->mspectr.data(), pmp->size);
		ippsConvert_64f32f(pmp->dSrdSpectr.data(), pmp->sspectr.data(), pmp->size);
		ippsConvert_64f32f(pmp->dStsSpectr.data(), pmp->stspectr.data(), pmp->size);
		ippsConvert_64f32f(pmp->dPrmSpectr.data(), pmp->pspectr.data(), pmp->size);
		ippsConvert_64f32f(pmp->dPopSpectr.data(), pmp->popspectr.data(), pmp->size);
		ippsConvert_64f32f(pmp->dUsrSpectr.data(), pmp->uspectr.data(), pmp->size);

		if(isCurChannel)
		{
			if(saveData->showParams.showKepstrWindow != 0)
			{
				res = KepstrCalc(pmp, calcData, saveData, workData, minLevel);
				//res = InverseCalc(pmp, calcData, saveData, workData);
			}
			if(saveData->showParams.showProhodWindow != 0)			//	отображение проходной характеристики
			{
				if (workData->prohReset == 1)						//	если установлен флаг сброса проходной характеристики
				{
					workData->prohindex = 0;						//	текущий индекс массива проходной сбрасываем в 0
					workData->prohReset = 0;						//	флаг сброса проходной характеристики тоже сбрасываем
					ippsZero_32f(calcData->prohspectr.data(), saveData->showParams.prohsize);		//	обнуляем массив проходной характеристики
				}
				calcData->prohspectr[workData->prohindex] = pmp->pspectr[workData->prohx];		//	записываем в массив проходной текущее значение из указанного элемента
				workData->prohindex++;													//	берём следующий индекс
				if (workData->prohindex >= saveData->showParams.prohsize)				//	если индекс элемента массива проходной больше размера массива
				{
					for (int i = 1; i < workData->prohindex; i++) calcData->prohspectr[i - 1] = calcData->prohspectr[i];
					workData->prohindex--;
					//workData->prohindex = 0;											//	то сбрасываем индекс в 0
				}
			}
		}

	}

	return 0;
}
//------------------------------------------------------------------------------
// Функция обработки считанных с сервера данных.
// Данная программа не поддерживает каналы серии 7000!
long GlobalFunctionMulti(PVOID pParam)
{
	long ret = 0;

	// определяю указатели на параметры канала, у которого прочитаны данные
	SendToSecGlobFunction *ppar = (SendToSecGlobFunction*)pParam;
	MultiChannelSelf *pMChan = ppar->pMChan;

	ThreadParams* params = (ThreadParams*)(ppar->pParamNext);	
	MultiParamSelf *pm = (params->multiChannels->begin() + ppar->index)._Ptr;

	pm->time = pMChan->DataTime.GetTime();
	pMChan->freqADC = params->server->Frequency(pMChan->Channel);

	// TODO : необходимо организовать правильный циклический буффер, который бы дописывал в конец регулярно читаеые куски данных
	// часто возможен случай, когда поток рассчитывает читать куски данных меньшие чем требуется для преобразования

	Calculation(pMChan, pm, params->saveParams, params->workParams, params->calcData, params->server, ppar->isCurrentChannel, params->workParams->allnan);
	
	return ret;
}
//------------------------------------------------------------------------------
UINT GenerateFileMassiveBase(LPVOID pParam)
{
	FileArrayStruct* pFAS = (FileArrayStruct*)pParam;
	CString stringTempl, stringTempl1, m_Text;
	for (int k = pFAS->size - 1; k > -1; k--)
	{
		stringTempl.Format(L"%f", pFAS->pF[k]);
		stringTempl1.Format(L"%i", pFAS->size - k);
		m_Text = m_Text + stringTempl1 + L"   -   " + L" " + stringTempl + L"\r\n";
	}
	if (!strGL.Find(L".txt"))
	{
		CStdioFile File(L"C:\\" + strGL, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
		File.WriteString((LPCTSTR)m_Text);
	}
	else
	{
		CStdioFile File(L"C:\\" + strGL + L".txt", CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
		File.WriteString((LPCTSTR)m_Text);
	}
	::PostMessage(GetForegroundWindow(), WM_END_CRFILE_UNIT, 0, 0);
	return 0;
}
//------------------------------------------------------------------------------
CSpectre_Base::CSpectre_Base( UINT nIDTemplate, CWnd* pParent, CString exeName,	CString cfgFilePreamble, ULONG nZPriority)

	: SpectreBase(
	nIDTemplate,		// идентификатор программы
	pParent,			// указатель на владельца
	exeName,			// имя exe-файла программы
	cfgFilePreamble,	// имя преамбулы конфигурационного файла				
	nZPriority)			// приоритет программы в Zet-панели (от 0 до 3)
//	, m_bEndInitNI(false)
	, m_bMulti(false)
	, g_pThread(nullptr)
	, m_pFAS(nullptr)
	, m_ElapseTimerReStart(200)
	, m_IDtimerReStart(1)
	, m_ElapseTimerAutoscale(1000)
	, m_IDtimerAutoscale(2)
	, m_isStarted(true)		// флаг работы программы
	, m_pmChanCur(nullptr)		// указатель на структуру параметров чтения текущего канала
	, m_pmParCur(nullptr)		// указатель на структуру параметров обработки текущего канала
	, m_sizeWcharTmp_400(400)
	, m_sizeWcharTmp_400_ts(400)
	, m_sizeWcharTmp_100_prom(100)
{
	m_sParam = new SaveStruct;
	m_sParamNew = new SaveStruct;
	m_sParamReader = new ShowParams;
	pSaveData = m_sParam;
	pSaveDataNew = m_sParamNew;
	m_sizeSaveData = sizeof(SaveStruct);
	ZeroMemory(pSaveData, m_sizeSaveData);
	ZeroMemory(pSaveDataNew, m_sizeSaveData);
	m_bPlacementCFG = true;
	m_sParam->SetName(m_sExeName + L".exe");
	m_sParamNew->SetName(m_sExeName + L".exe");
	m_sParam->size = m_sParamNew->size = sizeof(SaveStruct);
	m_sCommentAndOrder = GetLocalString(StringTitle);

	//ippStaticInit();			//	для статической линковки библиотек IPP
	ippInit();			        //	для динамической линковки библиотек IPP

	m_pwcharTmp_400 = new wchar_t[m_sizeWcharTmp_400];
	ZeroMemory(m_pwcharTmp_400, m_sizeWcharTmp_400 * sizeof(wchar_t));
	m_pwcharTmp_400_ts = new wchar_t[m_sizeWcharTmp_400_ts];
	ZeroMemory(m_pwcharTmp_400_ts, m_sizeWcharTmp_400_ts * sizeof(wchar_t));
	m_pwcharTmp_100_prom = new wchar_t[m_sizeWcharTmp_100_prom];
	ZeroMemory(m_pwcharTmp_100_prom, m_sizeWcharTmp_100_prom * sizeof(wchar_t));
}
//------------------------------------------------------------------------------
CSpectre_Base::~CSpectre_Base()
{
	if (m_pwcharTmp_400 != nullptr)
	{ 
		delete[] m_pwcharTmp_400;
		m_pwcharTmp_400 = nullptr;
	}
	if (m_pwcharTmp_400_ts != nullptr)
	{ 
		delete[] m_pwcharTmp_400_ts;
		m_pwcharTmp_400_ts = nullptr;
	}
	if (m_pwcharTmp_100_prom != nullptr)
	{
		delete[] m_pwcharTmp_100_prom;
		m_pwcharTmp_100_prom = nullptr;
	}

	if (g_pThread)
	{
		delete g_pThread;
		g_pThread = nullptr;
	}

	if (m_pFAS)
	{
		if (m_pFAS->pF)
		{
			delete[] m_pFAS->pF;
		delete m_pFAS;
		m_pFAS = nullptr;
		}
	}

	// синхронное завершение работы рабочего потока и удаление экз. класса
	m_multi.clear();	// вектор параметров обработки каналов

	if (m_sParamReader != nullptr)
	{ 
		delete m_sParamReader;
		m_sParamReader = nullptr;
	}
	if (m_sParamNew != nullptr)
	{ 
	 	delete m_sParamNew;
		m_sParamNew = nullptr;
	}
	if (m_sParam != nullptr)
	{ 
	 	delete m_sParam;
		m_sParam = nullptr;
	}
	//не исправлять
	m_pmChanCur = nullptr;
	m_pmParCur = nullptr;
}
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CSpectre_Base, SpectreBase)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_MESSAGE(WM_NEW_DATA_SRV, OnSetUpdate)
	ON_MESSAGE(WM_END_CRFILE_UNIT, OnEndGenerateFileUnit)
END_MESSAGE_MAP()
//------------------------------------------------------------------------------
BEGIN_EVENTSINK_MAP(CSpectre_Base, SpectreBase)
	ON_EVENT(CSpectre_Base, IDC_SRV, 1, _OnModifySRV, VTS_I4)
END_EVENTSINK_MAP()
//------------------------------------------------------------------------------
BOOL CSpectre_Base::OnInitDialog()
{
	SpectreBase::OnInitDialog();
	CreateElementInterface();
	Set_Icon_iw();

	m_sParam->numberProgram = m_sParamNew->numberProgram = m_nOrder;

	ResetOutData_iw();

	// изменяем шаг системного таймера для увеличения частоты обработки данных
	TIMECAPS tc;
	wTimerRes = 0;
	if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) == TIMERR_NOERROR)
	{
		wTimerRes = __min(__max(tc.wPeriodMin, TARGET_RESOLUTION), tc.wPeriodMax);
		timeBeginPeriod(wTimerRes);
	}
	
	// По умолчанию задаётся fast=0 => TimeInterval = 0.1 сек
	// Вначале new (текущий индекс = 0),
	// затем Create (вектор MultiChannel получает один элемент),
	// потом SetTimeInterval и только потом SetChannelNumber!!!
	// Тогда параметры канала заданы полностью и правильно

	m_threadParams.server = m_psrv;
	m_threadParams.saveParams = m_sParam;
	m_threadParams.workParams = &m_workParams;
	m_threadParams.calcData = &m_calcData;
	m_threadParams.multiChannels = &m_multi;

	g_pThread = new CThreadSRVmultiSelf();
	g_pThread->Create(L"Thread SRV", m_hWnd, m_psrv, 1,
		&GlobalFunctionMulti, &m_threadParams);
	g_pThread->SetSupport7000(false);
	g_pThread->SetMinFreqADC(1.f);
	g_pThread->SetChannelNumber(m_sParam->showParams.channel);
	m_pmChanCur = g_pThread->GetPointerMultiChannelCur();

	MultiParamSelf mulCur;
	m_multi.push_back(mulCur);		// добавил элемент
	m_pmParCur = &m_multi[0];

	*(AnalysisParams*)m_pmParCur = m_sParam->analysisParams;

	InitializeCurrentChannel();

	// отображение текстов слева
	if (m_pmChanCur->b7000)
		Show7000();
	else
		ResetOutData_iw();

	ZeroMemory(m_sParamReader, sizeof(ShowParams));
	
	m_pmChanCur->DataTime.SetStartTime( m_psrv->CurrentTime(m_sParam->showParams.channel) );
	m_isStarted = true;
	//------------------------------------------------
	if (m_sParamNew->wPlace.showCmd == SW_SHOWNORMAL)
	{
		RECT rect;
		GetClientRect(&rect);
		OnSize(SIZE_RESTORED, rect.right - rect.left, rect.bottom - rect.top);
	}

	//CreateLogger();
	// регистрируем структуры логгера ----------------------------------------
	if (!m_bJobUnit && CreateLogger())
	{
		WORD numberExe(0);
		long err = ztGetNumberSelfExe(&numberExe);

		err = ztOpenStructure(&m_saConfigLogger.zet_header, 
			ZETLAB_SPECTR_ANALYSIS_CFG_TYPE, ZETLAB_SPECTR_ANALYSIS_CFG_SIZE);
		err = ztSetProgramValuesInHeader(&m_saConfigLogger.zet_header, m_crc32ByNameExe, numberExe, false);

		m_saBufferLogger.ReSize(ZETLOGGER_MAXIMUM_STRUCTURE_LENGTH);
		m_psaResultLogger = (Spectr_Analysis_Result*)m_saBufferLogger.pData;

		err = ztOpenStructure(&m_psaResultLogger->zet_header, 
			ZETLAB_SPECTR_ANALYSIS_RESULT_TYPE, m_saBufferLogger.size);
		err = ztSetProgramValuesInHeader(&m_psaResultLogger->zet_header, m_crc32ByNameExe, numberExe, false);

		m_dwSizeArrayLogger = (m_psaResultLogger->zet_header.size - ZETLAB_SPECTR_ANALYSIS_RESULT_MIN_SIZE)
			/ sizeof(float);
		m_pResultArrayLogger = (float*)m_psaResultLogger->_;
	}

	AfxBeginThread(ThreadShield, &m_hWnd);
	g_pThread->Start();
	return TRUE;	// return TRUE  unless you set the focus to a control
}
//------------------------------------------------------------------------------
void CSpectre_Base::OnDestroy()
{
	if (wTimerRes != 0)
		timeEndPeriod(wTimerRes);

	KillTimer(m_IDtimerReStart);

	if (g_pThread)
	{
		delete g_pThread;
		g_pThread = NULL;
	}

	if (m_pFAS)
	{
		if (m_pFAS->pF)
		{
			delete[] m_pFAS->pF;
		delete m_pFAS;
		m_pFAS = nullptr;
		}
	}

	if (m_pLogger)
	{
		delete m_pLogger;
		m_pLogger = nullptr;
	}

	if (m_pResultDescription)
	{
		delete m_pResultDescription;
		m_pResultDescription = nullptr;
	}

	// синхронное завершение работы рабочего потока и удаление экз. класса
	m_multi.clear();	// вектор параметров обработки каналов

	//не исправлять
	m_pmChanCur = nullptr;
	m_pmParCur = nullptr;

	DeleteElementInterface();

	// убираем информацию об открытых дополнительных окнах
	m_sParam->showParams.showGrammaWindow = 0;
	m_sParam->showParams.showTDGrammaWindow = 0;
	m_sParam->showParams.showKepstrWindow = 0;
	m_sParam->showParams.showProhodWindow = 0;

	/*if (m_bBackupChanMode)
	{
		m_sParam->showParams.channel = m_BackupChan;
		m_sParam->showParams.IdChan  = m_IdBackupChan;
	}*/
	SpectreBase::OnDestroy();
}
//------------------------------------------------------------------------------
// Определение виртуальной функции, вызываемой в CDialog_ZET после чтения
// конфигурационного файла или после получения конфигурационных данных от
// Zet-панели. CDialog_ZET ничего не знает о структуре SaveStructure, поэтому
// эта функция должна определяться здесь!
void CSpectre_Base::TestBackupChanMode()
{// режим подмены канала после чтения конфигурационного файла
	long n = m_psrv->detectChanNum(m_sParamNew->showParams.IdChan);
	if (n != -1)
		m_sParamNew->showParams.channel = n;
	else
	{
		// работать буду с другим каналом
		long m = m_psrv->QuanChan();
		if (m_sParamNew->showParams.channel < 0)
			m_sParamNew->showParams.channel = 0;
		else if (m_sParamNew->showParams.channel >= m)
			m_sParamNew->showParams.channel = m - 1;
		m_psrv->IdChan(m_sParamNew->showParams.channel, &m_sParamNew->showParams.IdChan);
	}
}
//------------------------------------------------------------------------------
void CSpectre_Base::OnStructureDefault()
{
	InitializeDefaultParams();

	ZeroMemory(m_sParam->showParams.placement, sizeof(m_sParam->showParams.placement));
	UINT size = sizeof(WINDOWPLACEMENT);
	for (auto & it : m_sParam->showParams.placement)
		it.length = size;

	SpectreBase::OnStructureDefault();
}
//------------------------------------------------------------------------------
void CSpectre_Base::OnStructureNew()
{
	StopAllThread();
	ResetOutData_iw();
	UpdateCurrentChannelParams(true);
	UINT size = sizeof(m_sParam->showParams.placement);
	memcpy_s(m_sParam->showParams.placement, size, m_sParamNew->showParams.placement, size);

	if (m_isStarted)
		ReStart(false);
}
//------------------------------------------------------------------------------
bool CSpectre_Base::CreateLogger()
{
	/*wchar_t nameExe[MAX_PATH] = { 0 };
	if (GetModuleFileName(NULL, nameExe, MAX_PATH) > 0)
	{
		m_pLogger = new CZETLoggerClient();
	}

	HINSTANCE hInstance = AfxGetInstanceHandle();
	LPCTSTR lpType = MAKEINTRESOURCEW(7777);
	LPCTSTR lpName = MAKEINTRESOURCEW(1001);
	HRSRC myResource = ::FindResourceW(hInstance, lpName, lpType);
	if (myResource)
	{
		HGLOBAL myResourceData = ::LoadResource(NULL, myResource);
		if (myResourceData)
		{
			ZetHeader* pzh = (ZetHeader*)::LockResource(myResourceData);
			if (pzh)
			{
				try
				{
					if (ztIsStructureValid(pzh) == 0)
					{
						SZetLoggerStructureId slogger(m_crc32ByNameExe, 0, m_numberSelfExe, 0);
						m_pLogger->saveStructureDefinition(pzh, slogger);
					}
				}
				catch (...)
				{
					TRACE(L"Load descriptor error\n");
				}				
			}
		}
	}*/

	m_pLogger = new CZETLoggerClient();
	bool ret = m_pLogger != nullptr;
	if (ret)
	{
		SZetLoggerStructureId slogger(m_crc32ByNameExe, 0, m_numberSelfExe, 0);

		HINSTANCE hInstance = AfxGetInstanceHandle();
		LPCTSTR lpType = MAKEINTRESOURCEW(7777);
		WORD intRes[2] = { 1001, 1002 };
		for (auto it : intRes)
		{
			HRSRC myResource = ::FindResourceW(hInstance, MAKEINTRESOURCEW(it), lpType);
			if (myResource)
			{
				HGLOBAL myResourceData = ::LoadResource(NULL, myResource);
				if (myResourceData)
				{
					ZetHeader * pzh = (ZetHeader*)::LockResource(myResourceData);
					if (0 == ztIsStructureValid(pzh))
					{
						m_pLogger->saveStructureDefinition(pzh, slogger);
					}
				}
			}
		}
	}

	return m_pLogger != nullptr;
}
//------------------------------------------------------------------------------
void CSpectre_Base::SaveConfigToLogger()
{
	if (m_pLogger)
	{
		GUID *guid = (GUID*)m_saConfigLogger.channelID;
		m_psrv->IdChan(m_sParam->showParams.channel, guid);

		m_saConfigLogger.averagingType = m_sParam->analysisParams.typeusred;
		m_saConfigLogger.averaging = m_sParam->analysisParams.usred;
		m_saConfigLogger.calculationType = m_sParam->analysisParams.spm;
		m_saConfigLogger.decade = m_sParam->analysisParams.decade;
		m_saConfigLogger.integration = m_sParam->analysisParams.integral;
		m_saConfigLogger.logarithm = m_sParam->analysisParams.linlog;
		m_saConfigLogger.resolution = m_sParam->analysisParams.size;
		m_saConfigLogger.spectrType = m_sParam->analysisParams.analis;
		m_saConfigLogger.windowType = m_sParam->analysisParams.wind;


		if ((0 == ztSetStructureTimeLocal(&m_saConfigLogger.zet_header, m_psrv->GetTimeSRVasDATE(m_psrv->WorkingTime()))) &&
			(0 == ztCloseStructure(&m_saConfigLogger.zet_header)))
		{
			m_pLogger->writeStructureToBuff(&m_saConfigLogger.zet_header);
		}
	}
}
//------------------------------------------------------------------------------
void CSpectre_Base::ViewNewChanInWindows()
{
	Set_Title_iw(m_pmChanCur->Channel);
}
//------------------------------------------------------------------------------
void CSpectre_Base::DefaultMultiParam()
{// Задание параметров обработки по умолчанию в соответствии с OnStructureDefault

}
//------------------------------------------------------------------------------
void CSpectre_Base::Show7000()
{

}
//------------------------------------------------------------------------------
bool CSpectre_Base::StartSwitch()
{
	//TRACE("Spectr StartSwitch\n");
	bool bRet = false;
	if (!m_bMulti)		// если программа работает не в многоканальном режиме, то управляем флагом
	{
		m_isStarted = !m_isStarted;
		m_pmChanCur->bJob = !m_pmChanCur->bJob;
		bRet = m_isStarted;
	}
	else				// иначе управляем отдельно флагм каждого канала
	{
		m_pmChanCur->bJob = !m_pmChanCur->bJob;
		m_pmChanCur->DataTime.SetStartTime(m_psrv->CurrentTime(m_pmChanCur->Channel));
		StopAllThread();
		m_pmParCur->bStartInterval = true;
		m_pmParCur->timer = 0;
		m_pmParCur->tempsred = 0;
		m_pmParCur->indexsred = 0;
		ReStart(false);
		bRet = m_pmChanCur->bJob;
	}
	return bRet;
}
//------------------------------------------------------------------------------
LRESULT CSpectre_Base::OnSetUpdate(WPARAM wParam, LPARAM lParam)
{
	double time(0.);
	if ( g_pThread)	// если не была выполнена функция OnDestroy
	{
		// параметры обработанного канала -----------------------------
		int index = (int)wParam;
		MultiParamSelf *pmp = &m_multi[index];
		MultiChannelSelf *pmc = (g_pThread->GetIteratorBegin() + index)._Ptr;

		time = pmp->time;

		if (pmp->resultStatus.spectreSuccess)
		{
			// передача в UNIT всегда ---------------------------
			long i, sz;
			float x1, dx, fsize;
			double resolution;

			//TRACE("Spectr %d job UNIT %d\n", index, pmp->typeValUnit);
			sz = pmp->size / 2;
			x1 = 0; // GetGridStart();
			dx = pmc->freqADC / (float)pow(10.f, pmc->Decade) / 2; // GetGridInterval();
			resolution = (double)dx / sz;
			for (i = 0; i < sz; i++)
				m_calcData.freqm[i] = x1 + dx / (float)sz * (float)i;

			if (m_bJobUnit)
			{
				switch(pmp->typeValUnit)
				{
				case 5:	//	размер массива спектра
					fsize = (float)pmp->size;
					m_punitParent->UnitWrite(1, &fsize, pmp->typeValUnit);
					break;
				case 6:	//	массив частота
					m_punitParent->UnitWrite(sz, m_calcData.freqm.data(), pmp->typeValUnit);
					break;
				case 7:	//	мгновенный спектр
					//sz = pmp->size/2;
					m_punitParent->UnitWrite(sz, pmp->pspectr.data(), pmp->typeValUnit);
					break;
				case 19:	//	последовательная передача массивов частота, спктра, максимального и среднего
					m_punitParent->UnitWrite(sz, m_calcData.freqm.data(), 0 + index*100);
					m_punitParent->UnitWrite(sz, pmp->pspectr.data(), 1 + index*100);
					if (m_sParam->analysisParams.showMaxGrf)
						m_punitParent->UnitWrite(sz, pmp->mspectr.data(), 2 + index*100);
					if (m_sParam->analysisParams.showSredGrf)
						m_punitParent->UnitWrite(sz, pmp->stspectr.data(), 3 + index*100);
					break;
				case 26:
					//	для передачи данных через юнит без проблем соберём четыре массива в один
					double time = pmc->DataTime.GetTime();
					double delta = time - floor(time);
					if ((time > pmp->dLastUnitTime + 0.99 && delta < 0.01) || !_finite(pmp->dLastUnitTime))
					{
						pmp->dLastUnitTime = time;
						long sz4 = sz * 4;
						if (m_calcData.massivUnit.size() != sz4)
							m_calcData.massivUnit.resize(sz4);
						ippsZero_32f(m_calcData.massivUnit.data(), sz4);
						ippsCopy_32f(m_calcData.freqm.data(), m_calcData.massivUnit.data(), sz);
						ippsCopy_32f(pmp->pspectr.data(), &m_calcData.massivUnit.data()[sz], sz);
						if (m_sParam->analysisParams.showMaxGrf)
							ippsCopy_32f(pmp->mspectr.data(),  &m_calcData.massivUnit.data()[sz*2], sz);
						if (m_sParam->analysisParams.showSredGrf)
							ippsCopy_32f(pmp->stspectr.data(), &m_calcData.massivUnit.data()[sz*3], sz);
						m_punitParent->UnitWrite(sz4, m_calcData.massivUnit.data(), index * 100);
						CString outstr;
						outstr.AppendFormat(L"Spectr UnitWrite index %4d size %6d time %12.6f data[0]=%12.6f\n", index, sz4, time, pmp->pspectr[0]);
						OutputDebugString(outstr);
					}
					break;
				}
			}
			if (m_bJobUnit2)// результат в UNIT-2
			{
				switch (pmp->typeValUnit)
				{
				case 5:
					fsize = (float)pmp->size;
					//m_punitParent->UnitWrite(1, &fsize, pmp->typeValUnit);
					m_punitSrv->WriteResult_32s(pmp->typeValUnit, (long)floor(fsize));
					break;
				case 6:
					//m_punitParent->UnitWrite(sz, m_calcData.freqm.data(), pmp->typeValUnit);
					m_punitSrv->WriteResult_32f_arr(pmp->typeValUnit, sz, m_calcData.freqm.data());
					break;
				case 7:
					//m_punitParent->UnitWrite(sz, pmp->pspectr.data(), pmp->typeValUnit);
					m_punitSrv->WriteResult_32f_arr(pmp->typeValUnit, sz, pmp->pspectr.data());
					break;
				case 19:
					//m_punitParent->UnitWrite(sz, m_calcData.freqm.data(), 0 + index * 100);
					m_punitSrv->WriteResult_32f_arr(pmp->typeValUnit, sz, m_calcData.freqm.data());
					//m_punitParent->UnitWrite(sz, pmp->pspectr.data(), 1 + index * 100);
					m_punitSrv->WriteResult_32f_arr(pmp->typeValUnit, sz, pmp->pspectr.data());
					if (m_sParam->analysisParams.showMaxGrf)
						//m_punitParent->UnitWrite(sz, pmp->mspectr.data(), 2 + index * 100);
						m_punitSrv->WriteResult_32f_arr(pmp->typeValUnit, sz, pmp->mspectr.data());
					if (m_sParam->analysisParams.showSredGrf)
						//m_punitParent->UnitWrite(sz, pmp->stspectr.data(), 3 + index * 100);
						m_punitSrv->WriteResult_32f_arr(pmp->typeValUnit, sz, pmp->stspectr.data());
					break;
				case 26:
					//	для передачи данных через юнит без проблем соберём четыре массива в один
					double time = pmc->DataTime.GetTime();
					double delta = time - floor(time);
					delta = min(delta, 1. - delta);
					CString outstr;
					//outstr.Format(L"Spectr UnitWrite time %f\n", time);
					if ((time > pmp->dLastUnitTime + 0.99 && delta < 0.01) || !_finite(pmp->dLastUnitTime))
					{
						pmp->dLastUnitTime = time;
						long sz4 = sz * 4;
						if (m_calcData.massivUnit.size() != sz4)
							m_calcData.massivUnit.resize(sz4);
						ippsZero_32f(m_calcData.massivUnit.data(), sz4);
						ippsCopy_32f(m_calcData.freqm.data(), m_calcData.massivUnit.data(), sz);
						ippsCopy_32f(pmp->pspectr.data(), &m_calcData.massivUnit.data()[sz], sz);
						if (m_sParam->analysisParams.showMaxGrf)
							ippsCopy_32f(pmp->mspectr.data(), &m_calcData.massivUnit.data()[sz * 2], sz);
						if (m_sParam->analysisParams.showSredGrf)
							ippsCopy_32f(pmp->stspectr.data(), &m_calcData.massivUnit.data()[sz * 3], sz);
						//m_punitParent->UnitWrite(sz4, m_calcData.massivUnit.data(), index * 100);
						//m_punitSrv->WriteResult_32f_arr(index * 100, sz4, m_calcData.massivUnit.data());
						pmp->wr.value = m_calcData.massivUnit.data();
						pmp->wr.time = time;
						pmp->wr.quality = 0;
						m_punitSrv->WriteResult_wrs_32f_arr(index * 100 + 1, sz4, &pmp->wr);
						outstr.AppendFormat(L"Spectr UnitWrite index %d size %d time %f data[0]=%f\n", index, sz, time, pmp->pspectr[0]);
					}
					OutputDebugString(outstr);
					break;
				}
			}

			// запись спектра в логгер
			if (m_pLogger && m_sParam->showParams.logger.logSpectr)
			{
				DATE dtime = pmc->DataTime.GetTime();
				dtime = m_psrv->GetTimeSRVasDATE(dtime);
				if (ztSetStructureTimeLocal(&m_psaResultLogger->zet_header, dtime) == 0)
				{
					uint16_t process_id = (uint16_t)(m_numberSelfExe * 256 + index);
					CHANNEL_STRUCT tmpChan;
					m_psrv->GetChannelPar(pmc->Channel, (BYTE*)&tmpChan, SIZE_CHANNEL_STRUCT);
					memcpy_s(&m_psaResultLogger->channelIdentificator, 16, &tmpChan.id, sizeof(GUID));

					m_psaResultLogger->numberOfChannels = 1;
					ZeroMemory(m_psaResultLogger->spectrUnits, 8);
					m_psaResultLogger->resolution = resolution;
					m_psaResultLogger->peakValue = m_sParam->showParams.maxLevel;
					m_psaResultLogger->peakFrequency = (float)m_sParam->showParams.maxFreq;

					// вычисляем размер и позицию сохраняемого спектра
					long logindex = 0;
					long logsize = sz;
					long sar_size = sz * sizeof(float);
					// если размер в байтах массива со спектром больше буфера структуры, 
					// то выбираем только область 
					if (sz > (long)m_dwSizeArrayLogger)
					{
						logindex = (long)(m_sParam->showParams.logger.centralFreq / m_psaResultLogger->resolution + 0.5f)
							- m_dwSizeArrayLogger / 2;
						if (logindex < 0)
							logindex = 0;
						if (logindex + (long)m_dwSizeArrayLogger > sz)
							logindex = sz - m_dwSizeArrayLogger;
						logsize = m_dwSizeArrayLogger;
					}

					sar_size = ZETLAB_SPECTR_ANALYSIS_RESULT_MIN_SIZE + logsize * sizeof(float);
					m_psaResultLogger->zet_header.size = sar_size;
					m_psaResultLogger->minFrequncy = (float)(m_psaResultLogger->resolution * logindex);
					m_psaResultLogger->maxFrequncy = (float)(m_psaResultLogger->resolution * (logindex + logsize));

					// копируем выбранные массив в область структуры сразу псле завершения заголовка
					memcpy_s(m_pResultArrayLogger, logsize * sizeof(float),
						  &pmp->pspectr[logindex], logsize * sizeof(float));

					if (ztCloseStructure(&m_psaResultLogger->zet_header) == 0)
					{
						long ret = m_pLogger->writeStructureToBuff(&m_psaResultLogger->zet_header);
					}
				}
			}
		}

		// отображение результатов ----------------------------------		
		// для текущего канала отображение в окне программы
		if ( index == g_pThread->GetIndexCur() )
		{
			ShowResults();
		}
		// установка события View -----------------------------------
		g_pThread->SetSynchro();
	} 
	return 0;
}
//------------------------------------------------------------------------------
void CSpectre_Base::OnTimer(UINT_PTR nIDEvent)
{// Таймер нужен для того, чтобы при программировании программы через UNIT в
	// многоканальном режиме не передёргивать поток сервера
	if ( (nIDEvent == m_IDtimerReStart) && m_bProgramInitialized )
	{
		KillTimer(m_IDtimerReStart);
		OnModifySRV();		// обновление параметров каналов
	}
	// таймер отложенного автомасштаба нужен для того, чтобы отмасштабировать график 
	// не сразу после применения новых параметров, когда ещё не рассчитан спектр,
	// а через некоторое время после расчёта
	if ( (nIDEvent == m_IDtimerAutoscale) && m_bProgramInitialized)
	{
		KillTimer(m_IDtimerAutoscale);
		AutoscaleGrid();
	}
	SpectreBase::OnTimer(nIDEvent);
}
//------------------------------------------------------------------------------
BOOL CSpectre_Base::PreTranslateMessage(MSG* pMsg)
{
	return SpectreBase::PreTranslateMessage(pMsg);
}
//------------------------------------------------------------------------------
void CSpectre_Base::StopAllThread()
{	// останавливаю поток сервера -----------------------------
	//CString str;
	//str.Format(L"Spectr STOP all threads channel %d\n", m_pmChanCur->Channel);
	//OutputDebugString(str);
	g_pThread->StopAndWait();
}
//------------------------------------------------------------------------------
void CSpectre_Base::ReStart(bool bStopThread)
{
	if (bStopThread)
		StopAllThread();
	OnSetTimeSRV(0);
	*m_sParam = *m_sParamNew;
	//CString str;
	//str.Format(L"Spectr START thread channel %d\n", m_pmChanCur->Channel);
	//OutputDebugString(str);
	g_pThread->Start();
}
//------------------------------------------------------------------------------
void CSpectre_Base::OnFileModeSRV(long GlobalPar)
{
	StopAllThread();
	if (GlobalPar == 1 && m_sParamReader->IdChan == 0)

		memcpy_s(m_sParamReader, sizeof(ShowParams), &m_sParam->showParams, sizeof(ShowParams));
	if (GlobalPar == 0)
		memcpy_s(&m_sParam->showParams, sizeof(ShowParams), m_sParamReader, sizeof(ShowParams));
	OnModifySRV();		// надо обновить лист каналов
}
//------------------------------------------------------------------------------
void CSpectre_Base::OnSetTimeSRV(long GlobalPar)
{
	Sleep(100);
	auto it = g_pThread->GetIteratorBegin();
	for (auto itm=m_multi.begin(); itm!=m_multi.end(); ++itm, ++it)
	{
		if ( it->bExist && it->bJob && it->bEnabled)
		{
			resetAdditionalGraphs(&*itm);
			m_psrv->NumFileUsed(it->Channel);
			double curtime = m_psrv->CurrentTime(it->Channel);
			it->DataTime.SetStartTime(curtime);
			itm->dLastUnitTime = curtime;
		}
	}
}
//------------------------------------------------------------------------------
void CSpectre_Base::OnStartFileSRV(long GlobalPar)
{// При GlobalPar=0 старт чтения из файла (работа с ридером)
	// При этом время может != 0
	if (GlobalPar == 0)
	{
		OnSetTimeSRV(0);
	}
}
//------------------------------------------------------------------------------
void CSpectre_Base::_OnModifySRV(long GlobalPar)
{
	// при окончании воспроизведения файла ничего обновлять не нужно
	if (GlobalPar == 3)
		return;
	if (m_bProgramInitialized && m_bInterruptSRV)
	{
		SetTimer(m_idTimerModify, m_ElapseTimerModify, NULL);
		if (!m_bModifySRV)
		{// первый Modify
			m_bModifySRV = true;
			OnFirstModifySRV_0();
		}
	}
}
//------------------------------------------------------------------------------
void CSpectre_Base::OnModifySRV()
{// последний Modify или просто надо обновить информацию о каналах
	long n = 0;
	bool wasChange(false);
	if (g_pThread)
	{	// могли измениться каналы
		if (m_bJobManual || m_bJobPanel/* || m_bJobUnit || m_bJobUnit2*/)
		{// Одноканальный режим.
			n = m_psrv->detectChanNum(m_sParam->showParams.IdChan);// m_IdBackupChan);
			TRACE("Channel %d   id %d\n", n, m_sParam->showParams.IdChan);
			if (n == -1)
			{// не появился, работаю с прежним
				g_pThread->Modify();	// здесь учитывается возможное изменение FreqADC
			}
			else
			{// появился! перехожу на него
				if (g_pThread->SetChannelNumber(n))	// здесь учитывается возможное изменение FreqADC
				{
					m_sParamNew->showParams.channel = n;
					CHANNEL_STRUCT tmpStruct;
					m_psrv->GetChannelPar(m_sParamNew->showParams.channel, (BYTE*)&tmpStruct, SIZE_CHANNEL_STRUCT);
					if (memcmp(&m_sParam->showParams.ChanStruct, &tmpStruct, SIZE_CHANNEL_STRUCT) != 0)
					{
						wasChange = true;
						memcpy_s(&m_sParamNew->showParams.ChanStruct, SIZE_CHANNEL_STRUCT, &tmpStruct, SIZE_CHANNEL_STRUCT);
					}
				}
				else
					if ((m_psrv->GetStatus(n) == 0) || (m_psrv->GetStatus(n) == 1) || 
						(m_psrv->GetStatus(n) == 2) || (m_psrv->GetStatus(n) == 3) || 
						(m_psrv->GetStatus(n) == 6) || (m_psrv->GetStatus(n) == 8) )
						g_pThread->Start();
			}
			Set_Title_iw(n);
		} 
		else
		{// UNIT - без подмены канала
			g_pThread->Modify();	// здесь учитывается возможное изменение FreqADC
			if (IsCurrentChannelExist() && m_pmChanCur->bEnabled)
			{
				ViewNewChanInWindows();
			}
			g_pThread->Start();
		}
		if(wasChange)
		{
			UpdateCurrentChannelParams(true, true);
			//ReStart(false);
			UpdateGrid();
			UpdateGramma();
		}
		// перенёс сюда рестарт, чтобы спектр применял свои свойства сразу после запуска скады
		ReStart(false);
		
	}
}
//------------------------------------------------------------------------------
void CSpectre_Base::OnFirstModifySRV_0()
{// Первый Modify c GlobalPar == 0, могли измениться каналы
	StopAllThread();
}
//------------------------------------------------------------------------------
void CSpectre_Base::OnReadyUnitParent(long par)
{
//	long param, err, size; не использованная переменная исправлена из-за предупреждений studio
	long param(0);
	double value(0.);
	CString str = L"";
	m_punitParent->UnitParam(&param, &value);
	long val = (long)value;
	if (param == 3 || param == 22 || param == 24 || param == 25 || param == 27 || param == 28)
		TRACE("OnReadyUnitParent %d, %f\n", param, value);
	else
		TRACE("OnReadyUnitParent %d, %d\n", param, val);
	switch(param)
	{
	case 0:	 SetUnitParam_p0(val);	break;	// Номер канала
	case 1:	 SetUnitParam_p1(val);	break;  // установка декады
	case 3:	 SetUnitParam_p3(value);	break;  // установка усреднения в сек
	case 4:	 SetUnitParam_p4(val);	break;	// установка типа представления
	case 5:	 SetUnitParam_p5(val);	break;	// установка на получение размера спектра
	case 6:	 SetUnitParam_p6(val);	break;	// установка на получение массива частот
	case 7:	 SetUnitParam_p7(val);	break;	// установка на получение значений спектра
	case 8:	 SetUnitParam_p8(val);	break;	// установка частотного разрешения, размер ПФ
	case 9:	 SetUnitParam_p9(val);	break;	// очистка спектра
	case 10: SetUnitParam_p10(val);	break;	// тип анализа БПФ ДПФ
	case 11: SetUnitParam_p11(val);	break;	// тип весовой функции
	case 12: SetUnitParam_p12(val);	break;	// установка на запрет передачи данных
	case 13: SetUnitParam_p13(val);	break;	// интегрирование или дифференцирование
	case 14: SetUnitParam_p14(val);	break;	// линейно или логарифмического расчета
	case 15: SetUnitParam_p15(val);	break;	// чтение нормы и прорисовка
	case 16: SetUnitParam_p16(val);	break;	// запрет рисования нормы
	case 17: SetUnitParam_p17(val);	break;	// установка идентификатора номера канала
	case 18: SetUnitParam_p18(val);	break;	// установка курсора
	case 19: SetUnitParam_p19(val);	break;	// установка на последовательное получение данных (сетка частот(0), спектр(1), максимальный спектр(2), средний спектр(3))
	case 20: SetUnitParam_p20(val);	break;	// установка на разрешение максимального спектра
	case 21: SetUnitParam_p21(val);	break;	// установка на разрешение среднего спектра
	case 22: SetUnitParam_p22(value);	break;	// установка интервала расчета дополнительных графиков в сек
	case 23: SetUnitParam_p23(str);	break;	// 
	case 24: SetUnitParam_p24(value);	break;	// 
	case 25: SetUnitParam_p25(value);	break;	// 
	case 26: SetUnitParam_p26(val); break;	// установка на одновременное получение данных в массиве учетверённого размера
	case 27: SetUnitParam_p27(value);	break;	// установка частотного разрешения в герцах
	case 28: SetUnitParam_p28((long)value);	break;	// установка типа усреднения
	case 127: SetUnitParam_p127(val);	break;	// Stop
	case 255: SetUnitParam_p255(val);	break;	// Start
	case -1: SetUnitParam_m1(val);	break;
	case -2: SetUnitParam_m2(val);	break;
	case -3: SetUnitParam_m3(val);	break;
	case -4: SetUnitParam_m4(val);	break;
	case -5: SetUnitParam_m5(val);	break;
	}
	//if (param < 0)
	//	TRACE("Multi param=%d value=%d\n", param, val);
}
//------------------------------------------------------------------------------
void CSpectre_Base::SetUnitParam_p0(const long val)
{//установка номера канала
	if (m_bMulti)
	{
		if (m_pmChanCur->Channel != val)
		{
			CString str;
			str.Format(L"Spectr UNIT set channel %d\n", val);
			OutputDebugString(str);
			StopAllThread();
			bool hasChannel = IsCurrentChannelExist();
			g_pThread->SetChannelNumber(val);
			ViewNewChanInWindows();
			ResetOutData_iw();
			UpdateCurrentChannelParams(true);
			if (m_pmChanCur->b7000)
			{
				Show7000();
				m_pmChanCur->bJob = false;
			}
			else
			{
				if (!hasChannel && IsCurrentChannelExist())
				{
					InitializeCurrentChannel(true);
				}
			}
			if (m_isStarted)
				SetTimer(m_IDtimerReStart, m_ElapseTimerReStart, NULL);
		}
	}
	else
	{
		m_sParamNew->showParams.channel = val;
		OnStructureNewIfInit();
	}
}
//------------------------------------------------------------------------------
void CSpectre_Base::SetUnitParam_p1(const long val)	// установка декады
{
	if (m_bMulti)
	{
		if (m_pmParCur->decade != val)
		{
			StopAllThread();
			ResetOutData_iw();
			m_sParamNew->analysisParams.decade = val;
			setDecade();
			if(IsCurrentChannelExist())
			{
				InitializeDefaultChannel(m_pmParCur, m_pmChanCur);
			}
			SetTimer(m_IDtimerReStart, m_ElapseTimerReStart, NULL);
		}
	}
	else
	{
		m_sParamNew->analysisParams.decade = val;
		OnStructureNewIfInit();
	}
}
//------------------------------------------------------------------------------
void CSpectre_Base::SetUnitParam_p3(const double val)	// установка усреднения в сек
{
	//TRACE("SetUnitParam_p3 value=%f\n", val);
	if (m_bMulti)
	{
		if (IsNotEqual (m_pmParCur->usred, (float)val, 0.0001f))
		{
			StopAllThread();
			ResetOutData_iw();
			m_sParamNew->analysisParams.usred = (float)val;
			setUsredValue();
			if(IsCurrentChannelExist())
			{
				InitializeDefaultChannel(m_pmParCur, m_pmChanCur);
			}
			SetTimer(m_IDtimerReStart, m_ElapseTimerReStart, NULL);
		}
	}
	else
	{
		m_sParamNew->analysisParams.usred = (float)val;
		OnStructureNewIfInit();
	}
}
//------------------------------------------------------------------------------
void CSpectre_Base::SetUnitParam_p4(const long val)	// установка типа представления
{
	// из-за изменения порядка следования вкладок в окне параметров изменяем значение параметра
	long param(0);
	switch (val)
	{
	case 0: param = 2; break;
	case 1: param = 3; break;
	case 2: param = 0; break;
	case 3: param = 1; break;
	}
	if (m_bMulti)
	{
		if (m_pmParCur->spm != param)
		{
			StopAllThread();
			ResetOutData_iw();
			m_sParamNew->analysisParams.spm = param;
			setSpm();
			if(IsCurrentChannelExist())
			{
				InitializeDefaultChannel(m_pmParCur, m_pmChanCur);
			}
			SetTimer(m_IDtimerReStart, m_ElapseTimerReStart, NULL);
		}
	}
	else
	{
		m_sParamNew->analysisParams.spm = param;
		OnStructureNewIfInit();
	}
}
//------------------------------------------------------------------------------
void CSpectre_Base::SetUnitParam_p5(const long val)	// установка на получение размера спектра
{
	m_pmParCur->typeValUnit = 5;
}
//------------------------------------------------------------------------------
void CSpectre_Base::SetUnitParam_p6(const long val)	// установка на получение массива частот
{
	m_pmParCur->typeValUnit = 6;
}
//------------------------------------------------------------------------------
void CSpectre_Base::SetUnitParam_p7(const long val)	// установка на получение значений спектра
{
	m_pmParCur->typeValUnit = 7;
}
//------------------------------------------------------------------------------
void CSpectre_Base::SetUnitParam_p8(const long val)	// установка частотного разрешения, размер ПФ
{
	if (m_bMulti)
	{
		if (m_pmParCur->size != val * 2)
		{
			StopAllThread();
			ResetOutData_iw();
			m_sParamNew->analysisParams.size = val * 2;
			setSizeAndWindType();
			if(IsCurrentChannelExist())
			{
				InitializeDefaultChannel(m_pmParCur, m_pmChanCur);
			}
			SetTimer(m_IDtimerReStart, m_ElapseTimerReStart, NULL);
		}
	}
	else
	{
		m_sParamNew->analysisParams.size = val * 2;
		OnStructureNewIfInit();
	}
}
//------------------------------------------------------------------------------
void CSpectre_Base::SetUnitParam_p9(const long val)	// очистка спектра
{
	if (m_bMulti)
	{
		if (m_pmParCur->clean != val)
		{
			StopAllThread();
			ResetOutData_iw();
			m_sParamNew->analysisParams.clean = val;
			setClean();
			if(IsCurrentChannelExist())
			{
				InitializeDefaultChannel(m_pmParCur, m_pmChanCur);
			}
			SetTimer(m_IDtimerReStart, m_ElapseTimerReStart, NULL);
		}
	}
	else
	{
		m_sParamNew->analysisParams.clean = val;
		OnStructureNewIfInit();
	}
}
//------------------------------------------------------------------------------
void CSpectre_Base::SetUnitParam_p10(const long val)	// тип анализа БПФ ДПФ
{
	if (m_bMulti)
	{
		if (m_pmParCur->analis != val)
		{
			StopAllThread();
			ResetOutData_iw();
			m_sParamNew->analysisParams.analis = val;
			setAnalis();
			if(IsCurrentChannelExist())
			{
				InitializeDefaultChannel(m_pmParCur, m_pmChanCur);
			}
			SetTimer(m_IDtimerReStart, m_ElapseTimerReStart, NULL);
		}
	}
	else
	{
		m_sParamNew->analysisParams.analis = val;
		setAnalis();
		OnStructureNewIfInit();
	}
}
//------------------------------------------------------------------------------
void CSpectre_Base::SetUnitParam_p11(const long val)	// тип весовой функции
{
	if (m_bMulti)
	{
		if (m_pmParCur->wind != val)
		{
			StopAllThread();
			ResetOutData_iw();
			m_sParamNew->analysisParams.wind = val;
			setSizeAndWindType(false);
			if(IsCurrentChannelExist())
			{
				InitializeDefaultChannel(m_pmParCur, m_pmChanCur);
			}
			SetTimer(m_IDtimerReStart, m_ElapseTimerReStart, NULL);
		}
	}
	else
	{
		m_sParamNew->analysisParams.wind = val;
		OnStructureNewIfInit();
	}
}
//------------------------------------------------------------------------------
void CSpectre_Base::SetUnitParam_p12(const long val)	// установка на запрет передачи данных
{
	m_pmParCur->typeValUnit = 0;
}
//------------------------------------------------------------------------------
void CSpectre_Base::SetUnitParam_p13(const long val)	// интегрирование или дифференцирование
{
	if (m_bMulti)
	{
		if (m_pmParCur->integral != val)
		{
			StopAllThread();
			ResetOutData_iw();
			m_sParamNew->analysisParams.integral = val;
			setIntegral();
			if(IsCurrentChannelExist())
			{
				InitializeDefaultChannel(m_pmParCur, m_pmChanCur);
			}
			SetTimer(m_IDtimerReStart, m_ElapseTimerReStart, NULL);
		}
	}
	else
	{
		m_sParamNew->analysisParams.integral = val;
		OnStructureNewIfInit();
	}
}
//------------------------------------------------------------------------------
void CSpectre_Base::SetUnitParam_p14(const long val)	// линейно или логарифмического расчета
{
	if (m_bMulti)
	{
		if (m_pmParCur->linlog != val)
		{
			StopAllThread();
			ResetOutData_iw();
			m_sParamNew->analysisParams.linlog = val;
			setLinlog();
			if(IsCurrentChannelExist())
			{
				InitializeDefaultChannel(m_pmParCur, m_pmChanCur);
			}
			SetTimer(m_IDtimerReStart, m_ElapseTimerReStart, NULL);
		}
	}
	else
	{
		m_sParamNew->analysisParams.linlog = val;
		OnStructureNewIfInit();
	}
}
//------------------------------------------------------------------------------
void CSpectre_Base::SetUnitParam_p15(const long val)	// чтение нормы и прорисовка
{

}
//------------------------------------------------------------------------------
void CSpectre_Base::SetUnitParam_p16(const long val)	// запрет рисования нормы
{

}
//------------------------------------------------------------------------------
void CSpectre_Base::SetUnitParam_p17(const long val)	// установка идентификатора номера канала
{
	// В компоненте не используется.
}
//------------------------------------------------------------------------------
void CSpectre_Base::SetUnitParam_p18(const long val)	// установка курсора
{
	SetGridCursorPosition(val);
}
//------------------------------------------------------------------------------
void CSpectre_Base::SetUnitParam_p19(const long val)	// установка на последовательное получение данных (сетка частот(0), спектр(1), максимальный спектр(2), средний спектр(3))
{
	m_pmParCur->typeValUnit = 19;
}
//------------------------------------------------------------------------------
void CSpectre_Base::SetUnitParam_p20(const long val)	// установка на разрешение максимального спектра
{
	if (m_bMulti)
	{
		if (m_pmParCur->showMaxGrf != val)
		{
			StopAllThread();
			ResetOutData_iw();
			m_sParamNew->analysisParams.showMaxGrf = val;
			setShowMaxGrf();
			SetTimer(m_IDtimerReStart, m_ElapseTimerReStart, NULL);
		}
	}
	else
	{
		m_sParamNew->analysisParams.showMaxGrf = val;
		OnStructureNewIfInit();
	}
}
//------------------------------------------------------------------------------
void CSpectre_Base::SetUnitParam_p21(const long val)	// установка на разрешение среднего спектра
{
	if (m_bMulti)
	{
		if (m_pmParCur->showSredGrf != val)
		{
			StopAllThread();
			ResetOutData_iw();
			m_sParamNew->analysisParams.showSredGrf = val;
			setShowSredGrf();
			SetTimer(m_IDtimerReStart, m_ElapseTimerReStart, NULL);
			//TRACE("Spectr UNIT show sred graph %d\n", m_pmParCur->showSredGrf);
		}
	}
	else
	{
		m_sParamNew->analysisParams.showSredGrf = val;
		OnStructureNewIfInit();
	}
}
//------------------------------------------------------------------------------
void CSpectre_Base::SetUnitParam_p22(const double val)	// установка интервала расчета дополнительных графиков
{
	if (m_bMulti)
	{
		if (m_pmParCur->interval != val)
		{
			StopAllThread();
			ResetOutData_iw();
			m_sParamNew->analysisParams.interval = val;
			setInterval();
			if(m_isStarted)
			{
				InitializeDefaultChannel(m_pmParCur, m_pmChanCur);
			}
			SetTimer(m_IDtimerReStart, m_ElapseTimerReStart, NULL);
		}
	}
	else
	{
		m_sParamNew->analysisParams.interval = val;
		OnStructureNewIfInit();
	}
}
//------------------------------------------------------------------------------
void CSpectre_Base::SetUnitParam_p23(const CString val)	// 
{
	if (m_pFAS)
	{
		if (m_pFAS->pF)
		{
			delete[] m_pFAS->pF;
		delete m_pFAS;
		m_pFAS = nullptr;
		}
	}
//	pF = new float[m_pmParCur->pspectr.size()];
	m_pFAS = new FileArrayStruct;	// TODO: не забыть удалить
	m_pFAS->size = m_pmParCur->pspectr.size();
	ippsCopy_32f(m_pmParCur->pspectr.data(), m_pFAS->pF, m_pmParCur->pspectr.size());
	m_pFAS->strGL = val;
	AfxBeginThread(*GenerateFileMassiveBase, NULL);
/*	FILE* fp = fopen("C:\\my_file.txt", "w");
	fwrite((void *)m_Text.GetString(), 1, lstrlenW(m_Text.GetString()) << 1, fp);
	fclose(fp);//*/
	// В компоненте не используется.
}
//------------------------------------------------------------------------------
void CSpectre_Base::SetUnitParam_p24(const double val)	// 
{
	SetGridStartPosition(val);
}
//------------------------------------------------------------------------------
void CSpectre_Base::SetUnitParam_p25(const double val)	// 
{
	SetGridInterval(val);
}
//------------------------------------------------------------------------------
void CSpectre_Base::SetUnitParam_p26(const long val)	// 
{
	m_pmParCur->typeValUnit = 26;
}
//------------------------------------------------------------------------------
void CSpectre_Base::SetUnitParam_p27(const double val)	// 
{
	long newRes = (long)(m_pmChanCur->freqADC / pow(10., m_pmChanCur->Decade) / val + 0.5);
	if (m_bMulti)
	{
		if (m_pmParCur->size != newRes)
		{
			StopAllThread();
			ResetOutData_iw();
			m_sParamNew->analysisParams.size = newRes;
			setSizeAndWindType();
			if (IsCurrentChannelExist())
			{
				InitializeDefaultChannel(m_pmParCur, m_pmChanCur);
			}
			SetTimer(m_IDtimerReStart, m_ElapseTimerReStart, NULL);
		}
	}
	else
	{
		m_sParamNew->analysisParams.size = newRes;
		OnStructureNewIfInit();
	}
}
//------------------------------------------------------------------------------
void CSpectre_Base::SetUnitParam_p28(const long val)
{
	if (m_bMulti)
	{
		if (m_pmParCur->typeusred != val)
		{
			StopAllThread();
			ResetOutData_iw();
			m_sParamNew->analysisParams.typeusred = val;
			setAveragingType();
			SetTimer(m_IDtimerReStart, m_ElapseTimerReStart, NULL);
			//TRACE("Spectr UNIT show sred graph %d\n", m_pmParCur->showSredGrf);
		}
	}
	else
	{
		m_sParamNew->analysisParams.typeusred = val;
		OnStructureNewIfInit();
	}
}
//------------------------------------------------------------------------------
void CSpectre_Base::SetUnitParam_p255(const long val)	// Start
{
	if (m_bMulti)
	{
		if (!m_pmChanCur->bJob)
			StartSwitch();
	}	
	else
	{
		if (!m_isStarted)
			StartSwitch();
	}
	TRACE("SPECTR START %d\n", val);
}
//------------------------------------------------------------------------------
void CSpectre_Base::SetUnitParam_p127(const long val)	// Stop
{
	if (m_bMulti)
	{
		if (m_pmChanCur->bJob)
			StartSwitch();
	}
	else
	{
		if (m_isStarted)
			StartSwitch();
	}
	TRACE("SPECTR STOP %d\n", val);
}
//------------------------------------------------------------------------------
void CSpectre_Base::SetUnitParam_m1(const long val)
{// Переход в многоканальный режим. Первоначальное задание кол-ва каналов
	if (!m_bMulti)
	{
		if (val > 0)
		{
			StopAllThread();
			m_bMulti = true;
			m_multi.resize(val);
			for(long i=0; i < val-1; ++i)	// один канал уже есть
				g_pThread->AddChannel(-1);
			g_pThread->SetIndexCur(0);
			g_pThread->SetChannelNumber(-1);
			m_pmChanCur = g_pThread->GetPointerMultiChannelCur();
			m_pmParCur = &m_multi[ g_pThread->GetIndexCur() ];
			ViewNewChanInWindows();
			ResetOutData_iw();
			//Многоканальный режим работы программы. Кол-во каналов
			stringTempl.Format(GetLocalString(StringMultiChannelMode) + L" = %d", val);
			MessageInformation(stringTempl);
			SetTimer(m_IDtimerReStart, m_ElapseTimerReStart, NULL);
		}
	}

}
//------------------------------------------------------------------------------
void CSpectre_Base::SetUnitParam_m2(const long val)
{// задание текущего индекса канала для будущего изменения параметров
	if (m_bMulti)
	{
		long iOld = g_pThread->GetIndexCur();
		if ( g_pThread->SetIndexCur(val) )
		{
			long num = g_pThread->GetIndexCur();
			if ( iOld != num )
			{
				//L"Установка текущего индекса канала"
				stringTempl.Format(GetLocalString(StringSetCurrentChannel) + L" = %d",
					num);
				MessageInformation(stringTempl);
				m_pmChanCur = g_pThread->GetPointerMultiChannelCur();
				m_pmParCur = &m_multi[ g_pThread->GetIndexCur() ];
				// применияем текущие параметры канала
				memcpy_s(&m_sParamNew->analysisParams, sizeof(AnalysisParams), m_pmParCur, sizeof(AnalysisParams));
				ViewNewChanInWindows();
				if (m_pmChanCur->b7000)
					Show7000();
				else
					ResetOutData_iw();
			}
		}
	}
}
//------------------------------------------------------------------------------
void CSpectre_Base::SetUnitParam_m3(const long val)
{// остановка канала
	if (m_bMulti)
	{
		if ( (0 <= val) && (val < (long)m_multi.size() ) )
		{
		    StopAllThread();
		    if ( g_pThread->StopChannel(val) )
		    {// L"Остановка канала с индексом"
			    stringTempl.Format(GetLocalString(StringStopChannel) + L" = %d", val);
			    MessageInformation(stringTempl);
		    }
		    SetTimer(m_IDtimerReStart, m_ElapseTimerReStart, NULL);
		}
	}

}
//------------------------------------------------------------------------------
void CSpectre_Base::SetUnitParam_m4(const long val)
{// запуск канала
	if (m_bMulti)
	{
		if ( (0 <= val) && (val < (long)m_multi.size() ) )
		{
		    StopAllThread();
		    if ( g_pThread->StartChannel(val) )
		    {// L"Запуск канала с индексом"
			    stringTempl.Format(GetLocalString(StringStartChannel) + L" = %d", val);
			    MessageInformation(stringTempl);
		    }
		    SetTimer(m_IDtimerReStart, m_ElapseTimerReStart, NULL);
		}
	}

}
//------------------------------------------------------------------------------
void CSpectre_Base::SetUnitParam_m5(const long val)
{ // добавление канала, он станет текущим
	if (m_bMulti)
	{// добавление канала № num
		if ( (0 <= val) && (val < m_psrv->QuanChan()) )
		{
		    StopAllThread();
		    if ( g_pThread->AddChannel(val) != -1)
		    {// добавление удачное (0) или добавлен не существующий канал (-2)
			    // функция SetChannelNumber будет вызвана внутри AddChannel
			    MultiParamSelf mlt;
			    m_multi.push_back(mlt);
			    m_pmParCur = &m_multi[ g_pThread->GetIndexCur() ];
			    m_pmChanCur = g_pThread->GetPointerMultiChannelCur();

			    if (m_pmChanCur->b7000)
			    {
				    Show7000();
				    m_pmChanCur->bJob = false;
			    }
			    else if (!IsCurrentChannelExist())
				    ResetOutData_iw();
			    else
			    {
					InitializeCurrentChannel(true);
			    }		
			    stringTempl.Format( GetLocalString(StringAddChannel) + L"%s", m_psrv->Commentary(val) );
			    MessageInformation(stringTempl);
		    }
		    else
		    {
				m_pmChanCur = g_pThread->GetPointerMultiChannelCur();
				m_pmParCur = &m_multi[ g_pThread->GetIndexCur() ];
		    }

		    ViewNewChanInWindows();
		    SetTimer(m_IDtimerReStart, m_ElapseTimerReStart, NULL);
		}
	}
}
//------------------------------------------------------------------------------
void CSpectre_Base::InitializeDefaultParams()
{
	double freq = 25000.;

	m_sParam->analysisParams.decade = 0;

	m_sParam->analysisParams.size = 2000;
	m_sParam->analysisParams.sizeforz = 4096;
	m_sParam->analysisParams.analis = 1;
	m_sParam->analysisParams.wind = 2;
	m_sParam->analysisParams.spm = 0;
	m_sParam->analysisParams.integral = 2;
	m_sParam->analysisParams.interval = 100.;
	m_sParam->analysisParams.usred = 1.;
	m_sParam->analysisParams.typeusred = 0;
	m_sParam->analysisParams.linlog = 1;
	m_sParam->analysisParams.clean = 0;

	m_sParam->analysisParams.showMaxGrf = 0;
	m_sParam->analysisParams.showSredGrf = 0;

	m_sParam->showParams.channel = 0;
	m_psrv->IdChan(0, &m_sParam->showParams.IdChan);

	m_sParam->showParams.showGrammaWindow = 0;
	m_sParam->showParams.colorgrm = 1;
	m_sParam->showParams.grammaStart = 1;
	m_sParam->showParams.showTDGrammaWindow = 0;
	m_sParam->showParams.tdGrammaStart = 1;
	m_sParam->showParams.showKepstrWindow = 0;
	m_sParam->showParams.showNormaGrf = 0;
	m_sParam->showParams.showProhodWindow = 0;
	m_workParams.prohindex = 0;
	
	m_sParam->showParams.myComment[0] = 0;
	m_workParams.prohReset = 1;
	m_sParam->showParams.prohsize = 0;
}
//------------------------------------------------------------------------------
void CSpectre_Base::InitializeDefaultData()
{
	initializeProhod();
}
//------------------------------------------------------------------------------
void CSpectre_Base::InitializeDefaultChannel(MultiParamSelf* pmp, MultiChannelSelf* mChan)
{
	ippsZero_32f(m_pmParCur->uspectr.data(), m_pmParCur->size);
	ippsSet_32f(1.0f, m_pmParCur->popspectr.data(), pmp->size);
	ippsZero_64f(m_pmParCur->dUsrSpectr.data(), m_pmParCur->size);
	ippsSet_64f(1.0f, m_pmParCur->dPopSpectr.data(), pmp->size);
	resetAdditionalGraphs(pmp);

	double frequency(0.);
	frequency = m_psrv->Frequency(mChan->Channel) * pow(0.1, mChan->Decade);
	if(pmp->size / frequency / 4 > 0.1) 
		pmp->timedelay = 0.1;
	else 
		if (pmp->usred < 0.1)
			pmp->timedelay = 0.01;
		else
			pmp->timedelay = 0.1;
	if(pmp->size / frequency / 4 > 1) 
		pmp->timedelay = 1.;
	if((frequency >= 1000000.)&&(pmp->size > 50000l))
		pmp->timedelay = 1.;
	if (pmp->typeusred)
		m_pmChanCur->DataTime.SetAverageCodeMode(1, frequency);
	else
		m_pmChanCur->DataTime.SetUserParamMode(m_pmParCur->timedelay, 0, m_pmParCur->usred);

	pmp->sdvig = frequency * pmp->timedelay;
}
//------------------------------------------------------------------------------
void CSpectre_Base::UpdateCurrentChannelParams(bool setDefaultForced, bool channelChangedByServer)
{
	bool changed(setDefaultForced), usredChanged(false), rereadpopr(setDefaultForced), rereadnorm(false), redrawGramma(false), redrawTDGramma(false);

	int i(0), err(0), flusred(0), chch(0), chm(0);
	float fr(0.f), ur(0.f), frold(0.f), urold(0.f), tfr(0.f), tur(0.f), frp(0.f);

	FILE *input(nullptr);
	CString stringTempl_;
	
	flusred = 0;
	
	chch = 0;
	chm = 0;

	m_pmParCur->bParamsChanged = true;

	if (setChannel(channelChangedByServer))
	{
		changed = true;
		rereadpopr = true;
		rereadnorm = true;
		redrawGramma = true;
		redrawTDGramma = true;
	}

	if (setDecade())
	{
		changed = true;
		rereadpopr = true;
		rereadnorm = true;
		redrawGramma = true;
		redrawTDGramma = true;
		if((m_sParam->showParams.showGrammaWindow == 0) && (m_sParam->showParams.showTDGrammaWindow == 0))
		{
			m_pmParCur->tempsred=0;
		}
	}

	if (setUsredValue())
	{
		changed = true;
		rereadpopr = true;
		rereadnorm = true;
		redrawGramma = true;
		redrawTDGramma = true;
		usredChanged = true;
	}

	if (setIntegral())
	{
		changed = true;
	}

	if (setLinlog())
	{
		changed = true;
		rereadpopr = true;
		rereadnorm = true;
		redrawGramma = true;
		redrawTDGramma = true;
	}

	if (setInterval())
	{
		//changed = true;
	}
	if (setUsredType())
	{
		//changed = true;
	}
	if (setAnalis())
	{
		//changed = true;
	}

	if (setSpm())
	{
		changed = true;
	}

	if (setSizeAndWindType())
	{
		changed = true;
		rereadpopr = true;
		rereadnorm = true;
		redrawGramma = true;
		redrawTDGramma = true;
	}

	if (m_sParam->showParams.showKepstrWindow != m_sParamNew->showParams.showKepstrWindow)
	{
		if (m_sParamNew->showParams.showKepstrWindow != 0)
		{
			ShowKepstrWindow(true);
			UpdateKepstr();
		}
		else
		{
			ShowKepstrWindow(false);
			m_sParamNew->showParams.kepstrStart = 0;
		}
	}

	setShowMaxGrf();

	if ((m_sParam->showParams.showNormaGrf != m_sParamNew->showParams.showNormaGrf) || wcscmp(m_sParam->showParams.normaFile, m_sParam->showParams.normaFile))
	{
		rereadnorm = 1;
	}

	if (m_sParam->showParams.showProhodWindow != m_sParamNew->showParams.showProhodWindow)
	{
		if (m_sParamNew->showParams.showProhodWindow != 0)
		{
			ShowProhodWindow(true);
			UpdateProhod();
		}
		else
		{
			ShowProhodWindow(false);
			m_sParamNew->showParams.prohStart = 0;
		}
	}

	setShowSredGrf();

	if (changed)
	{
		InitializeDefaultChannel(m_pmParCur, m_pmChanCur);
		m_pmChanCur->DataTime.SetStartTime(m_psrv->CurrentTime(m_sParam->showParams.channel));
		UpdateGrid();
		// SetTimer(m_IDtimerAutoscale, m_ElapseTimerAutoscale, NULL);
	}

	if (redrawGramma)
	{
		UpdateGramma();
	}

	if (m_sParam->showParams.showTDGrammaWindow != m_sParamNew->showParams.showTDGrammaWindow)
	{
		if (m_sParamNew->showParams.showTDGrammaWindow != '\0')
		{
			ShowTDGrammaWindow(true);
			redrawTDGramma = false;
		}
		else
		{
			ShowTDGrammaWindow(false);
		}
	}

	if (redrawTDGramma)
	{
		UpdateTDGramma();
	}

	if (m_sParam->showParams.showGrammaWindow != m_sParamNew->showParams.showGrammaWindow)
	{
		if (m_sParamNew->showParams.showGrammaWindow != 0)
		{
			ShowGrammaWindow(true);
			redrawGramma = false;
		}
		else
		{
			ShowGrammaWindow(false);
		}
	}

	if (usredChanged)
	{
		if (m_sParamNew->showParams.showGrammaWindow != 0)
		{
			ResetGramma();
		}
		if (m_sParamNew->showParams.showTDGrammaWindow != 0)
		{
			ResetTDGramma();
		}
		if (m_sParamNew->showParams.showProhodWindow != 0)
		{
			ResetProhod();
			initializeProhod();
		}
	}

	setClean();

	if (rereadnorm)
	{
		if (m_sParamNew->showParams.showNormaGrf != 0) //включить
		{
			if (wcslen(m_sParamNew->showParams.normaFile) != 0)
			{
				//input = (FILE*)_wfsopen(m_sParamNew->showParams.normaFile, L"r+, ccs=UTF-8", SH_DENYNO);
				if (zfOpenFile(m_sParamNew->showParams.normaFile, L"r+, ccs=UTF-8", &input) == 0)
//				if(input != NULL)
				{
					err = 2;
					frold = 0.f;
					urold = 0.f;
					if (err == 2)
					{
						err = 0;
						err += fwscanf_s(input, L"%f", &fr);
						err += fwscanf_s(input, L"%f", &ur);
					}
					if (fr != '\0')
					{
						m_calcData.nspectr[0] = ur;
					}
					else
					{
						m_calcData.nspectr[0] = urold;
					}
					for(i = 1; i < m_pmParCur->size/2; i++)
					{
						tfr = (float)((float)i * m_pmChanCur->freqADC * pow(0.1, m_pmChanCur->Decade) / (float)m_pmParCur->size);
						if(tfr > fr)
						{
							if (err == 2)
							{
								urold = ur;
								frp = fr;
								while ((fr <= tfr) && (err == 2))
								{
									err = 0;
									err += fwscanf_s(input,L"%f",&fr);
									err += fwscanf_s(input,L"%f",&ur);
								}
								if(IsNotEqual (frp, fr, 0.0001f)) frold = frp;
							}
							else fr = tfr;
						}
						tur = urold + (ur-urold)/(fr-frold)*(tfr-frold);
						m_calcData.nspectr[i] = tur;
					}
					ShowNormaGrf(true);
					PaintNormaGrf();
					zfCloseFile(input);
					//fclose(input);
				}
			}
		}
		else
		{
			ShowNormaGrf(false);
		}
	}

	if (rereadpopr == 1)
	{
		if (wcslen(m_psrv->AFCH(m_pmChanCur->Channel)) != 0)
		{
			//input = (FILE*)_wfsopen(m_psrv->AFCH(m_pmChanCur->Channel), L"r+, ccs=UTF-8", SH_DENYNO);
			if (zfOpenFile(m_psrv->AFCH(m_pmChanCur->Channel), L"r+, ccs=UTF-8", &input) == 0)
			//if (input != NULL)
			{
				memset(m_pwcharTmp_400, 0, m_sizeWcharTmp_400 * sizeof(wchar_t));
				fgetws(m_pwcharTmp_400, m_sizeWcharTmp_400, input);
				stringTempl_ = GetLocalString(StringPopravkaFile);
				if (wcsstr(m_pwcharTmp_400, stringTempl_) == NULL)
				{
					zfCloseFile(input);
					//fclose(input);
					return;
				}
				stringTempl_ = GetLocalString(StringArgHz);
				while (wcsstr(m_pwcharTmp_400, stringTempl_) == NULL)
				{
					fgetws(m_pwcharTmp_400, m_sizeWcharTmp_400, input);
				}
				m_pmParCur->linlog_popr = 0;

				stringTempl_ = GetLocalString(StringArgDecibel);
				if (wcsstr(m_pwcharTmp_400, stringTempl_) != NULL)
				{
					m_pmParCur->linlog_popr = 1;
				}

				err = 2;
				frold = 0;
				urold = 0;
				if (err == 2)
				{
					err = 0;
					err += fwscanf_s(input, L"%f", &fr);
					err += fwscanf_s(input, L"%f", &ur);
				}
				if (IsEqual(fr, 0.f, 0.0001f)) tur=ur; 
				else tur=urold;

				i = 0;
				if (m_pmParCur->linlog_popr == 1)
				{
					m_pmParCur->popspectr[i] = (float)pow(10.0,tur/20.0);
				}
				else
				{
					if (tur>0)
						m_pmParCur->popspectr[i] = tur;
					else
						m_pmParCur->popspectr[i] = 1.0f;
				}
				float frTemp = m_pmChanCur->freqADC * (float)pow(0.1, m_pmChanCur->Decade) / (float)m_pmParCur->size;
				for (i = 1; i < m_pmParCur->size/2; i++)
				{
					tfr = (float)i * frTemp;
					if (tfr > fr)
					{
						if (err == 2)
						{
							urold = ur;
							frp = fr;
							while ((fr < tfr) && (err == 2))
							{
								err = 0;
								err += fwscanf_s(input, L"%f", &fr);
								err += fwscanf_s(input, L"%f", &ur);
							}
							if(IsNotEqual (frp, fr, 0.0001f)) frold = frp; 
						}
						else fr = tfr;
					}
					tur = urold + (ur-urold)/(fr-frold)*(tfr-frold);

					if (m_pmParCur->linlog_popr == 1)
					{
						m_pmParCur->popspectr[i] = (float)pow(10.0,tur/20.0);
					}
					else
					{
						if (tur > 0)
							m_pmParCur->popspectr[i] = tur;
						else
							m_pmParCur->popspectr[i] = 1.0f;
					}
				}
				zfCloseFile(input);
				//fclose(input); 
				//ippsMul_32f_I(m_pmParCur->popspectr.data(), m_pmParCur->popspectr.data(), m_pmParCur->size / 2); //	обновляем массив спектра поправки
				ippsZero_32f(m_pmParCur->popspectr.data(), m_pmParCur->size); //	обнуляем массив спектра поправки
			}
		}
	}

	CString tmpstr(L"");
	tmpstr.Format(L"Spectr updateParams ");
	tmpstr.AppendFormat(L"channel %d ", m_sParamNew->showParams.channel);
	tmpstr.AppendFormat(L"size %d ", m_sParamNew->analysisParams.size);
	tmpstr.AppendFormat(L"usred %d ", m_sParamNew->analysisParams.usred);
	tmpstr.Append(L"\n");
	OutputDebugString(tmpstr);
}
//------------------------------------------------------------------------------
void CSpectre_Base::ShowResults()
{
	if(m_pmParCur->resultStatus.spectreSuccess)
	{
		PaintSpectr();
		if(m_pmParCur->showMaxGrf && m_pmParCur->resultStatus.maxSpectreSuccess)
		{
			PaintMaxGrf();
		}
		if(m_pmParCur->showSredGrf && m_pmParCur->resultStatus.sredSpectreSuccess)
		{
			PaintSredGrf();
		}
		if(m_sParam->showParams.showKepstrWindow && m_sParam->showParams.kepstrStart)
		{
			PaintKepstr();
		}
		if(m_sParam->showParams.showGrammaWindow && m_sParam->showParams.grammaStart)
		{
			PaintGramma();
		}
		if(m_sParam->showParams.showTDGrammaWindow && m_sParam->showParams.tdGrammaStart)
		{
			PaintTDGramma();
		}
		if(m_sParam->showParams.showProhodWindow && m_sParam->showParams.prohStart)
		{
			PaintProhod();
		}

		UpdateMaxValues();
		SetCurAmplitude_iw((double)m_psrv->CurLevel(m_pmChanCur->Channel));
	}
	else
	{
		if (m_workParams.allnan)
		{
			memset(m_pmParCur->pspectr.data(), 0, m_pmParCur->pspectr.size() * sizeof(float));
			PaintSpectr();

			if (m_pmParCur->showMaxGrf && !m_pmParCur->resultStatus.maxSpectreSuccess)
			{
				memset(m_pmParCur->mspectr.data(), 0, m_pmParCur->mspectr.size() * sizeof(float));
				PaintMaxGrf();
			}
			if (m_pmParCur->showSredGrf && !m_pmParCur->resultStatus.sredSpectreSuccess)
			{
				memset(m_pmParCur->stspectr.data(), 0, m_pmParCur->stspectr.size() * sizeof(float));
				PaintSredGrf();
			}
			if (m_sParam->showParams.showKepstrWindow && !m_sParam->showParams.kepstrStart)
			{
				memset(m_calcData.kspectr.data(), 0, m_calcData.kspectr.size() * sizeof(float));

				PaintKepstr();
			}
			if (m_sParam->showParams.showGrammaWindow && !m_sParam->showParams.grammaStart)
			{
				memset(m_pmParCur->pspectr.data(), 0, m_pmParCur->pspectr.size() * sizeof(float));
				PaintGramma();
			}
			if (m_sParam->showParams.showTDGrammaWindow && !m_sParam->showParams.tdGrammaStart)
			{
				memset(m_pmParCur->pspectr.data(), 0, m_pmParCur->pspectr.size() * sizeof(float));
				PaintTDGramma();
			}
			if (m_sParam->showParams.showProhodWindow && !m_sParam->showParams.prohStart)
			{
				memset(m_calcData.prohspectr.data(), 0, m_calcData.prohspectr.size() * sizeof(float));
				PaintProhod();
			}

			UpdateMaxValues();
			SetCurAmplitude_iw((double)m_psrv->CurLevel(m_pmChanCur->Channel));
		}
	}
	if ((m_pmParCur->showMaxGrf != 0) || (m_pmParCur->showSredGrf != 0))
	{
		SetCurInterval_iw(m_pmParCur->timer);
	}
}
//------------------------------------------------------------------------------
void CSpectre_Base::UpdateMaxValues()
{
	int maxInd(0);
	float maxLevel(0.f);
	double maxFreq(0.);
	ippsMaxIndx_32f(m_pmParCur->pspectr.data(), m_pmParCur->size / 2, &maxLevel, &maxInd);
	maxFreq = GetGridStart() + GetGridInterval();
	maxFreq /= (double)m_pmParCur->size;
	maxFreq *= 2.0f;
	maxFreq *= (double)maxInd;
	m_sParam->showParams.maxFreq = maxFreq;
	m_sParam->showParams.maxLevel = maxLevel;
	UpdateMaxValues_iw(maxLevel, maxFreq);
}
//------------------------------------------------------------------------------
template<typename T> T getParamByLimits(T val, T min, T max)
{
	return val < min ? min : val > max ? max : val;
}
//------------------------------------------------------------------------------
bool CSpectre_Base::setChannel(bool channelChangedByServer)
{
	bool result = m_sParam->showParams.channel != m_sParamNew->showParams.channel;
	if(channelChangedByServer)
	{
		// новые данные в диалоговое окно
		ResetOutData_iw();
		ViewNewChanInWindows();
		result = true;
	}
	else if (result)
	{
		if( g_pThread->SetChannelNumber(m_sParamNew->showParams.channel) )
		{
			//Переходим на канал -
			MessageInformation(GetLocalString(StringChangeChannel) + L" " + m_psrv->Commentary(m_sParamNew->showParams.channel));
			// новые данные в диалоговое окно
			ResetOutData_iw();
			m_psrv->IdChan(m_sParamNew->showParams.channel, &m_sParamNew->showParams.IdChan);
			UpdateKepstr();
			UpdateProhod();
		}
		else
		{
			ResetOutData_iw();
		}
		ViewNewChanInWindows();
	}
	return result;
}
//------------------------------------------------------------------------------
bool CSpectre_Base::setDecade()
{
	bool result = m_pmParCur->decade != m_sParamNew->analysisParams.decade;
	if(result)
	{
		g_pThread->SetChannelDecade(g_pThread->GetIndexCur(), m_sParamNew->analysisParams.decade);
		m_pmParCur->decade = m_sParamNew->analysisParams.decade = m_pmChanCur->Decade;
		if(IsCurrentChannelExist())
		{
			FormIntegr(m_pmParCur, m_pmChanCur->freqADC);
			UpdateKepstr();
			UpdateProhod();
		}
	}
	return result;
}
//------------------------------------------------------------------------------
bool CSpectre_Base::setIntegral()
{
	m_sParamNew->analysisParams.integral = getParamByLimits(m_sParamNew->analysisParams.integral, 0, 4);
	bool result = m_pmParCur->integral != m_sParamNew->analysisParams.integral;
	if(result)
	{
		m_pmParCur->integral = m_sParamNew->analysisParams.integral;
		if(IsCurrentChannelExist())
		{
			FormIntegr(m_pmParCur, m_pmChanCur->freqADC);
		}
	}
	return result;
}
//------------------------------------------------------------------------------
bool CSpectre_Base::setUsredType()
{
	m_sParamNew->analysisParams.typeusred = getParamByLimits(m_sParamNew->analysisParams.typeusred, 0, 1);
	bool result = m_pmParCur->typeusred != m_sParamNew->analysisParams.typeusred;
	if(result)
	{
		m_pmParCur->typeusred = m_sParamNew->analysisParams.typeusred;
	}
	return result;
}
//------------------------------------------------------------------------------
bool CSpectre_Base::setUsredValue()
{
	m_sParamNew->analysisParams.usred = getParamByLimits(m_sParamNew->analysisParams.usred, 0.01f, 100000.f);
//	bool result = m_pmParCur->usred != m_sParamNew->analysisParams.usred; исправлена из-за исправления ошибки найденной 2014.06.09 pvs studio
	bool result = IsNotEqual (m_pmParCur->usred, m_sParamNew->analysisParams.usred, 0.0001f);
	if (result)
	{
		m_pmParCur->usred = m_sParamNew->analysisParams.usred;
	}
	return result;
}
//------------------------------------------------------------------------------
bool CSpectre_Base::setAnalis()
{
	m_sParamNew->analysisParams.analis = getParamByLimits(m_sParamNew->analysisParams.analis, 0, 1);
	bool result = m_pmParCur->analis != m_sParamNew->analysisParams.analis;
	if(result)
	{
		m_pmParCur->analis = m_sParamNew->analysisParams.analis;
	}
	return result;
}
//------------------------------------------------------------------------------
bool CSpectre_Base::setSpm()
{
	m_sParamNew->analysisParams.spm = getParamByLimits(m_sParamNew->analysisParams.spm, 0, 3);
	bool result = m_pmParCur->spm != m_sParamNew->analysisParams.spm;
	if(result)
	{
		m_pmParCur->spm = m_sParamNew->analysisParams.spm;
	}
	return result;
}
//------------------------------------------------------------------------------
bool CSpectre_Base::setInterval()
{
	m_sParamNew->analysisParams.interval = getParamByLimits(m_sParamNew->analysisParams.interval, (double)5, (double)100000);
	bool result = IsNotEqual (m_pmParCur->interval, m_sParamNew->analysisParams.interval, 0.0001);
	if(result)
	{
		m_pmParCur->interval = m_sParamNew->analysisParams.interval;
	}	
	return result;
}
//------------------------------------------------------------------------------
bool CSpectre_Base::setLinlog()
{
	m_sParamNew->analysisParams.linlog = getParamByLimits(m_sParamNew->analysisParams.linlog, 0, 1);
	bool result = m_pmParCur->linlog != m_sParamNew->analysisParams.linlog;
	if(result)
	{
		m_pmParCur->linlog = m_sParamNew->analysisParams.linlog;
	}
	return result;
}
//------------------------------------------------------------------------------
bool CSpectre_Base::setClean()
{
	m_sParamNew->analysisParams.clean = getParamByLimits(m_sParamNew->analysisParams.clean, 0, 1);
	bool result = m_pmParCur->clean != m_sParamNew->analysisParams.clean;
	if(result)
	{
		m_pmParCur->clean = m_sParamNew->analysisParams.clean;
	}
	return result;
}
//------------------------------------------------------------------------------
bool CSpectre_Base::setSizeAndWindType(bool byUser)
{
	bool result = false;
	long bSize = m_psrv->DecadeBufferSize(m_sParamNew->analysisParams.decade) / 2;
	m_sParamNew->analysisParams.size = getParamByLimits(m_sParamNew->analysisParams.size, 100l, bSize);
	
	// из-за неразмной организации базового класса и непродуманного взаимодействия с функцией потока иногда случаются "дикие" ошибки
	// например, при смене канала в потоке обновляется количество считываемых данных, а следующие функции по-идее должны были сделать размер
	// соответствующим преобразованию БПФ
	//if(m_pmParCur->size != m_sParamNew->analysisParams.size)
	{
		result = true;
		if(IsCurrentChannelExist())
		{
		    m_pmParCur->ReSize(m_sParamNew->analysisParams.size);
		    m_calcData.Resize(m_sParamNew->analysisParams.size);
		    //  задаём количество считываемых точек за один период работы потоковой функции
			if (m_sParamNew->analysisParams.size > (long)m_pmParCur->sdvig * 2)
				g_pThread->SetChannelNumberCounts(m_sParamNew->analysisParams.size);
			else
				g_pThread->SetChannelNumberCounts((long)m_pmParCur->sdvig * 2);
		}
	}
	m_sParamNew->analysisParams.wind = getParamByLimits(m_sParamNew->analysisParams.wind, 0, 12);
	if (m_pmParCur->wind != m_sParamNew->analysisParams.wind)
	{
		result = true;
		m_pmParCur->wind = m_sParamNew->analysisParams.wind;
	}

	if (result && IsCurrentChannelExist())
	{
		InitWind(m_pmParCur);
		FormVes(m_pmParCur);
		m_pmParCur->tempsred = 0;
		FormIntegr(m_pmParCur, m_pmChanCur->freqADC);

		// теперь при любом изменении параметров грамма сбрасывается

		/*if(!byUser || ((m_sParamNew->showParams.showGrammaWindow == 0) && (m_sParamNew->showParams.showTDGrammaWindow == 0)))
		{
			InitWind(m_pmParCur);
			FormVes(m_pmParCur);
			m_pmParCur->tempsred = 0;
			FormIntegr(m_pmParCur, m_pmChanCur->freqADC);
		}
		else
		{
			//MessageBox(GetLocalString(StringCantChangeSizeWhileShowGramma), 0, MB_OK);
			MessageInformation(GetLocalString(StringCantChangeSizeWhileShowGramma));
			result = false;
		}*/

		//if (m_sParamNew->showParams.showGrammaWindow == 1 || m_sParamNew->showParams.showTDGrammaWindow == 1)
		//	MessageInformation(GetLocalString(StringCantChangeSizeWhileShowGramma));
	}
	return result;
}
//------------------------------------------------------------------------------
bool CSpectre_Base::setShowMaxGrf()
{
	bool result = false;
	m_sParamNew->analysisParams.showMaxGrf = getParamByLimits(m_sParamNew->analysisParams.showMaxGrf, 0, 1);
	//if (m_pmParCur->showMaxGrf != m_sParamNew->analysisParams.showMaxGrf)
	{
		result = true;
		m_pmParCur->showMaxGrf = m_sParamNew->analysisParams.showMaxGrf;
		if (m_pmParCur->showMaxGrf != 0)
		{
			ShowMaxGrf(true); //включить
			//if (!m_pmParCur->bStartInterval)
			{
				m_pmParCur->bStartInterval = true;
				m_pmParCur->timer = 0;
				m_pmParCur->tempsred = 0;
			}
			if (IsCurrentChannelExist())
			{
			    initializeMaxGraph(m_pmParCur);
			    PaintMaxGrf();
			}
		}
		else
		{
			ShowMaxGrf(false); //выключить
		}
	}
	return result;
}
//------------------------------------------------------------------------------
bool CSpectre_Base::setShowSredGrf()
{
	bool result = false;
	m_sParamNew->analysisParams.showSredGrf = getParamByLimits(m_sParamNew->analysisParams.showSredGrf, 0, 1);
	//if (m_pmParCur->showSredGrf != m_sParamNew->analysisParams.showSredGrf)
	{
		result = true;
		m_pmParCur->showSredGrf = m_sParamNew->analysisParams.showSredGrf;
		if (m_pmParCur->showSredGrf != 0)
		{
			ShowSredGrf(true);
			m_pmParCur->indexsred = 0;
			//if (!m_pmParCur->bStartInterval)
			{
				m_pmParCur->bStartInterval = true;
				m_pmParCur->timer = 0;
				m_pmParCur->tempsred = 0;
			}
			if (IsCurrentChannelExist())
			{
			    initializeSredGraph(m_pmParCur);
			    PaintSredGrf();
			}
		}
		else
		{
			ShowSredGrf(false);
		}
	}
	return result;
}
//------------------------------------------------------------------------------
bool CSpectre_Base::setAveragingType()
{
	bool result = false;
	m_sParamNew->analysisParams.typeusred = getParamByLimits(m_sParamNew->analysisParams.typeusred, 0, 1);

	result = true;
	m_pmParCur->typeusred = m_sParamNew->analysisParams.typeusred;

	return result;
}
//------------------------------------------------------------------------------
void CSpectre_Base::initializeMaxGraph(MultiParamSelf* pmp)
{
	ippsSet_32f(-1000.f, m_pmParCur->mspectr.data(), pmp->size);
	ippsSet_64f(-1000.f, m_pmParCur->dMaxSpectr.data(), pmp->size);
}
//------------------------------------------------------------------------------
void CSpectre_Base::initializeSredGraph(MultiParamSelf* pmp)
{
	ippsZero_32f(m_pmParCur->stspectr.data(), pmp->size);
	ippsZero_32f(m_pmParCur->sspectr.data(), pmp->size);
	ippsZero_64f(m_pmParCur->dStsSpectr.data(), pmp->size);
	ippsZero_64f(m_pmParCur->dSrdSpectr.data(), pmp->size);
}
//------------------------------------------------------------------------------
void CSpectre_Base::initializeProhod()
{
	ippsZero_32f(m_calcData.prohspectr.data(), m_sParam->showParams.prohsize);
	m_workParams.prohindex = 0;
}
//------------------------------------------------------------------------------
void CSpectre_Base::resetAdditionalGraphs()
{
	resetAdditionalGraphs(m_pmParCur);
}
//------------------------------------------------------------------------------
void CSpectre_Base::resetAdditionalGraphs(MultiParamSelf* pmp)
{
	initializeMaxGraph(pmp);
	initializeSredGraph(pmp);
	pmp->bStartInterval = true;
	pmp->timer = 0;
	pmp->tempsred = 0;
	pmp->indexsred = 0;
}
//------------------------------------------------------------------------------
void CSpectre_Base::WriteFile(const CString &fileName)
{
	FILE *out(nullptr);
	float freq(0.f);
	double val(0.),logref(0.);
	int i(0);
	CTime ti = CTime::GetCurrentTime();
	//CString tmpString(L""); не используется.

	//out = _wfsopen(fileName.GetString(), L"w+, ccs=UTF-8", SH_DENYNO);
	//if (out != NULL)
	if (zfOpenFile(fileName.GetString(), L"w+, ccs=UTF-8", &out) == 0)
	{
		CString str_s3 = GetLocalString(StringNameOut);
		fwprintf_s(out, L"%s", str_s3.GetString());
		CString str_s4 = m_psrv->Commentary(m_pmChanCur->Channel);
		fwprintf_s(out, L"%s\n", str_s4.GetString());
		fwprintf_s(out, L"%s\n", m_sParam->showParams.myComment);
		freq = (float)(m_pmChanCur->freqADC * pow(0.1, m_pmChanCur->Decade));
		fwprintf_s(out, GetLocalString(StringWriteFileStringFreq), freq / m_pmParCur->size, freq / 2, freq / m_pmParCur->size, freq / m_pmParCur->size * m_pmParCur->ewind);
		fwprintf_s(out, GetLocalString(StringWriteFileStringDate) + L"\n",ti.GetDay(), ti.GetMonth(), ti.GetYear());
		fwprintf_s(out, GetLocalString(StringWriteFileStringTime) + L"\n",ti.GetHour(), ti.GetMinute(), ti.GetSecond());
		//21.02.2005 - куча значений не всегда нужна /Сергей
		memset(m_pwcharTmp_400_ts, 0, m_sizeWcharTmp_400_ts * sizeof(wchar_t)); //идеальное зануление строки		
		WriteGridParams(out);
		swprintf_s(m_pwcharTmp_400_ts, m_sizeWcharTmp_400_ts, L"%s\t%s", TranslateString(L"Частота").GetString(), TranslateString(L"Уровень").GetString());
		if(m_sParam->analysisParams.showMaxGrf != 0)
		{
			wcscat_s(m_pwcharTmp_400_ts, m_sizeWcharTmp_400_ts, GetLocalString(StringWrtieFileStringMax));
		}
		if(m_sParam->analysisParams.showSredGrf != 0)
		{
			wcscat_s(m_pwcharTmp_400_ts, m_sizeWcharTmp_400_ts, GetLocalString(StringWrtieFileStringSred));
		}
		if(m_sParam->showParams.showNormaGrf != 0)
		{
			wcscat_s(m_pwcharTmp_400_ts, m_sizeWcharTmp_400_ts, GetLocalString(StringWrtieFileStringNorma));
		}

		fwprintf_s(out, L"%s\n", m_pwcharTmp_400_ts);

		if(m_pmParCur->linlog == 0)
		{
			memset(m_pwcharTmp_400_ts, 0, m_sizeWcharTmp_400_ts *sizeof(wchar_t));
			swprintf_s(m_pwcharTmp_400_ts, m_sizeWcharTmp_400_ts, GetLocalString(StringWrtieFileStringHz), m_workParams.linstr);
			if(m_sParam->analysisParams.showMaxGrf != 0)
			{
				wcscat_s(m_pwcharTmp_400_ts, m_sizeWcharTmp_400_ts, L"\t");
				wcscat_s(m_pwcharTmp_400_ts, m_sizeWcharTmp_400_ts, m_workParams.linstr);
			}
			if(m_sParam->analysisParams.showSredGrf != 0)
			{
				wcscat_s(m_pwcharTmp_400_ts, m_sizeWcharTmp_400_ts, L"\t");
				wcscat_s(m_pwcharTmp_400_ts, m_sizeWcharTmp_400_ts, m_workParams.linstr);
			}
			if(m_sParam->showParams.showNormaGrf != 0)
			{
				wcscat_s(m_pwcharTmp_400_ts, m_sizeWcharTmp_400_ts, L"\t");
				wcscat_s(m_pwcharTmp_400_ts, m_sizeWcharTmp_400_ts, m_workParams.linstr);
			}
			fwprintf_s(out, L"%s\n", m_pwcharTmp_400_ts);
		}
		else
		{		
			memset(m_pwcharTmp_400_ts, 0, m_sizeWcharTmp_400_ts * sizeof(wchar_t));
			swprintf_s(m_pwcharTmp_400_ts, m_sizeWcharTmp_400_ts, GetLocalString(StringWrtieFileStringHz), m_workParams.logstr);
			if(m_sParam->analysisParams.showMaxGrf != 0)
			{
				wcscat_s(m_pwcharTmp_400_ts, m_sizeWcharTmp_400_ts, L"\t");
				wcscat_s(m_pwcharTmp_400_ts, m_sizeWcharTmp_400_ts, m_workParams.logstr);
			}
			if(m_sParam->analysisParams.showSredGrf != 0)
			{
				wcscat_s(m_pwcharTmp_400_ts, m_sizeWcharTmp_400_ts, L"\t");
				wcscat_s(m_pwcharTmp_400_ts, m_sizeWcharTmp_400_ts, m_workParams.logstr);
			}
			if(m_sParam->showParams.showNormaGrf != 0)
			{
				wcscat_s(m_pwcharTmp_400_ts, m_sizeWcharTmp_400_ts, L"\t");
				wcscat_s(m_pwcharTmp_400_ts, m_sizeWcharTmp_400_ts, m_workParams.logstr);
			}
			fwprintf_s(out, L"%s\n", m_pwcharTmp_400_ts);
			logref = m_pmParCur->reference;
			if(logref <= 0.) logref = 0.000001;
			logref=-log10(logref)*20.;
		}
		for(i = 0; i < m_pmParCur->size / 2; i++)
		{
			swprintf_s(m_pwcharTmp_400, m_sizeWcharTmp_400, L"%g", i * freq / m_pmParCur->size);
			swprintf_s(m_pwcharTmp_100_prom, m_sizeWcharTmp_100_prom, L"\t%g", m_pmParCur->pspectr[i]);
			wcscat_s(m_pwcharTmp_400, m_sizeWcharTmp_400, m_pwcharTmp_100_prom);
			if(m_sParam->analysisParams.showMaxGrf != 0)
			{
				swprintf_s(m_pwcharTmp_100_prom, m_sizeWcharTmp_400, L"\t%g", m_pmParCur->mspectr[i]);
				wcscat_s(m_pwcharTmp_400, m_sizeWcharTmp_400, m_pwcharTmp_100_prom);
			}
			if(m_sParam->analysisParams.showSredGrf != 0)
			{
				val = m_pmParCur->stspectr[i];
				swprintf_s(m_pwcharTmp_100_prom, m_sizeWcharTmp_400, L"\t%g", m_pmParCur->stspectr[i]);
				wcscat_s(m_pwcharTmp_400, m_sizeWcharTmp_400, m_pwcharTmp_100_prom);
			}
			if(m_sParam->showParams.showNormaGrf != 0)
			{
				swprintf_s(m_pwcharTmp_400, m_sizeWcharTmp_400, L"\t%g", m_calcData.nspectr[i]);
				wcscat_s(m_pwcharTmp_400, m_sizeWcharTmp_400, m_pwcharTmp_100_prom);
			}
			fwprintf_s(out, L"%s\n", m_pwcharTmp_400);
		}
		zfCloseFile(out);
		//fclose(out);
	}
}
//------------------------------------------------------------------------------
bool CSpectre_Base::IsCurrentChannelExist()
{
	return m_pmChanCur->Channel != -1 && m_pmChanCur->bExist;
}
//------------------------------------------------------------------------------
void CSpectre_Base::InitializeCurrentChannel(bool withUI)
{
	m_pmParCur->ReSize(m_pmParCur->size);

	m_calcData.Resize(m_pmParCur->size);
	InitializeDefaultChannel(m_pmParCur, m_pmChanCur);

	g_pThread->SetChannelDecade(g_pThread->GetIndexCur(), m_pmParCur->decade);
	
	double frequency(0.);
	frequency = m_psrv->Frequency(m_pmChanCur->Channel) * pow(0.1, m_pmChanCur->Decade);
	if (m_pmParCur->typeusred)
		m_pmChanCur->DataTime.SetAverageCodeMode(1, frequency);
	else
		m_pmChanCur->DataTime.SetUserParamMode(m_pmParCur->timedelay, 0, m_pmParCur->usred);

	g_pThread->SetChannelNumberCounts(m_pmParCur->size);

	InitWind(m_pmParCur);
	FormVes(m_pmParCur);
	FormIntegr(m_pmParCur, m_pmChanCur->freqADC);

	if (withUI)
	{
		UpdateGrid();
		SetTimer(m_IDtimerAutoscale, m_ElapseTimerAutoscale, NULL);
	}
}
//------------------------------------------------------------------------------
void CSpectre_Base::OnReadyUnit2Param (void)
{
	long val(0);
	float fval(0);
	CString str(L"");
	switch(m_paramUnit2.number)
	{
	case 0:	if (m_paramUnit2.type == tdu_32s)
			if (m_paramUnit2.Extract_val(val) == NOERROR) 
				SetUnitParam_p0(val);	break;	// Номер канала
	case 1:	if (m_paramUnit2.type == tdu_32s)
			if (m_paramUnit2.Extract_val(val) == NOERROR)
				SetUnitParam_p1(val);	break;  // установка декады
	case 3:	if (m_paramUnit2.type == tdu_32f)
			if (m_paramUnit2.Extract_val(fval) == NOERROR)	 
				SetUnitParam_p3(fval);	break;  // установка усреднения в сек
	case 4:	if (m_paramUnit2.type == tdu_32s)
			if (m_paramUnit2.Extract_val(val) == NOERROR)
				SetUnitParam_p4(val);	break;	// установка типа представления
	case 5: if (m_paramUnit2.type == tdu_32s)
			if (m_paramUnit2.Extract_val(val) == NOERROR)	
				SetUnitParam_p5(val);	break;	// установка на получение размера спектра
	case 6:	if (m_paramUnit2.type == tdu_32s)
			if (m_paramUnit2.Extract_val(val) == NOERROR) 
				SetUnitParam_p6(val);	break;	// установка на получение массива частот
	case 7:	if (m_paramUnit2.type == tdu_32s)
			if (m_paramUnit2.Extract_val(val) == NOERROR) 
				SetUnitParam_p7(val);	break;	// установка на получение значений спектра
	case 8:	if (m_paramUnit2.type == tdu_32s)
			if (m_paramUnit2.Extract_val(val) == NOERROR) 
				SetUnitParam_p8(val);	break;	// установка частотного разрешения, размер ПФ
	case 9:	if (m_paramUnit2.type == tdu_32s)
			if (m_paramUnit2.Extract_val(val) == NOERROR) 
				SetUnitParam_p9(val);	break;	// очистка спектра
	case 10: if (m_paramUnit2.type == tdu_32s)
			 if (m_paramUnit2.Extract_val(val) == NOERROR) 
				SetUnitParam_p10(val);	break;	// тип анализа БПФ ДПФ
	case 11: if (m_paramUnit2.type == tdu_32s)
			 if (m_paramUnit2.Extract_val(val) == NOERROR) 
				SetUnitParam_p11(val);	break;	// тип весовой функции
	case 12: if (m_paramUnit2.type == tdu_32s)
			 if (m_paramUnit2.Extract_val(val) == NOERROR) 
				SetUnitParam_p12(val);	break;	// установка на запрет передачи данных
	case 13: if (m_paramUnit2.type == tdu_32s)
			 if (m_paramUnit2.Extract_val(val) == NOERROR) 
				SetUnitParam_p13(val);	break;	// интегрирование или дифференцирование
	case 14: if (m_paramUnit2.type == tdu_32s)
			 if (m_paramUnit2.Extract_val(val) == NOERROR) 
				SetUnitParam_p14(val);	break;	// линейно или логарифмического расчета
	case 15: if (m_paramUnit2.type == tdu_32s)
			 if (m_paramUnit2.Extract_val(val) == NOERROR) 
				SetUnitParam_p15(val);	break;	// чтение нормы и прорисовка
	case 16: if (m_paramUnit2.type == tdu_32s)
			 if (m_paramUnit2.Extract_val(val) == NOERROR) 
				SetUnitParam_p16(val);	break;	// запрет рисования нормы
	case 17: if (m_paramUnit2.type == tdu_32s)
			 if (m_paramUnit2.Extract_val(val) == NOERROR) 
				SetUnitParam_p17(val);	break;	// установка идентификатора номера канала
	case 18: if (m_paramUnit2.type == tdu_32s)
			 if (m_paramUnit2.Extract_val(val) == NOERROR) 
				SetUnitParam_p18(val);	break;	// установка курсора
	case 19: if (m_paramUnit2.type == tdu_32s)
			 if (m_paramUnit2.Extract_val(val) == NOERROR) 
				SetUnitParam_p19(val);	break;	// установка на последовательное получение данных (сетка частот(0), спектр(1), максимальный спектр(2), средний спектр(3))
	case 20: if (m_paramUnit2.type == tdu_32s)
			 if (m_paramUnit2.Extract_val(val) == NOERROR) 
				SetUnitParam_p20(val);	break;	// установка на разрешение максимального спектра
	case 21: if (m_paramUnit2.type == tdu_32s)
			 if (m_paramUnit2.Extract_val(val) == NOERROR) 
				SetUnitParam_p21(val);	break;	// установка на разрешение среднего спектра
	case 22: if (m_paramUnit2.type == tdu_32f)
			 if (m_paramUnit2.Extract_val(fval) == NOERROR) 
				SetUnitParam_p22(fval);	break;	// установка интервала расчета дополнительных графиков
	case 23: if (m_paramUnit2.type == tdu_txt)
			 if (m_paramUnit2.Extract_txt(str) == NOERROR)
				 SetUnitParam_p23(str); break;
/*	case 24: if (m_paramUnit2.type == tdu_32s)
			 if (m_paramUnit2.Extract_val(val) == NOERROR) 
			   	SetUnitParam_p24(value);	break;	// 
	case 25: if (m_paramUnit2.type == tdu_32s)
			 if (m_paramUnit2.Extract_val(val) == NOERROR) 
				SetUnitParam_p25(value);	break;	// */
	case 26: if (m_paramUnit2.type == tdu_32s)
			 if (m_paramUnit2.Extract_val(val) == NOERROR)
				 SetUnitParam_p26(val); break;
	case 27: if (m_paramUnit2.type == tdu_32f)
			 if (m_paramUnit2.Extract_val(fval) == NOERROR)
				 SetUnitParam_p27(fval); break;
	case 28: if (m_paramUnit2.type == tdu_32s)
			 if (m_paramUnit2.Extract_val(val) == NOERROR)
				 SetUnitParam_p28(val); break;
	case 127: if (m_paramUnit2.type == tdu_32s)
			  if (m_paramUnit2.Extract_val(val) == NOERROR) 
				 SetUnitParam_p127(val); break;	// Stop
	case 255: if (m_paramUnit2.type == tdu_32s)
			  if (m_paramUnit2.Extract_val(val) == NOERROR) 
				 SetUnitParam_p255(val); break;	// Start
	case -1: if (m_paramUnit2.type == tdu_32s)
			 if (m_paramUnit2.Extract_val(val) == NOERROR) 
				SetUnitParam_m1(val);	break;
	case -2: if (m_paramUnit2.type == tdu_32s)
			 if (m_paramUnit2.Extract_val(val) == NOERROR) 
				SetUnitParam_m2(val);	break;
	case -3: if (m_paramUnit2.type == tdu_32s)
			 if (m_paramUnit2.Extract_val(val) == NOERROR) 
				SetUnitParam_m3(val);	break;
	case -4: if (m_paramUnit2.type == tdu_32s)
			 if (m_paramUnit2.Extract_val(val) == NOERROR) 
				SetUnitParam_m4(val);	break;
	case -5: if (m_paramUnit2.type == tdu_32s)
			 if (m_paramUnit2.Extract_val(val) == NOERROR) 
				SetUnitParam_m5(val);	break;
	}

	if (m_paramUnit2.type == tdu_32s)
		TRACE(L"OnReadyUnit2Param   %d   %d\n", m_paramUnit2.number, val);
	if (m_paramUnit2.type == tdu_32f)
		TRACE(L"OnReadyUnit2Param   %d   %f\n", m_paramUnit2.number, fval);
}
//------------------------------------------------------------------------------
LRESULT CSpectre_Base::OnEndGenerateFileUnit(WPARAM wParam, LPARAM lParam)
{
	if (m_pFAS)
	{
		delete m_pFAS;
		m_pFAS = nullptr;
	}
	return 0;
}