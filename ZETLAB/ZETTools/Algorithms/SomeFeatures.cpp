//------------------------------------------------------------------------------
#include "StdAfx.h"
#include <Algorithms\SomeFeatures.h>
//------------------------------------------------------------------------------
//		Поиск начала события в многомерном сигнале с помощью вейвлет-фильтрации.
//		К проекту необходимо подключить три класса CCustomFDWT, CCustomPacketFDWT
// и COncePacketFDWT.
//		При dimention = 1 pointerDataInp и pointerDataOut - это float*, при
// dimention > 1 pointerDataInp и pointerDataOut - это float**.
//		Если отфильтрованные данные не нужны, то pointerDataOut = NULL. Если
// исходные данные больше не нужно, то pointerDataOut = pointerDataInp.
//		Функция может искать начало при движении по массивам как слева
// (pPositionLeft != NULL), так и справа (pPositionRight != NULL). Оба не
// могут быть нулевыми. При движении справа вначале находится максимум, и
// от него выполняется поиск влево.
//
bool SearchPositionBeginUsingFDWT(
	PVOID pointerDataInp,		//массив исходных данных
	PVOID pointerDataOut,		//массив данных после вейвлет-фильтрации
	int dimention,				//размерность сигнала
	int size,					//размер каждой из компонент сигнала
	int sizePreHistory,			//размер предыстории, где точно нет события
	int *pPositionLeft,			//найденная точка при поиске слева
	int *pPositionRight,		//найденная точка при поиске справа
	TypeWavelet typeWavelet,	//материнский вейвлет
	int levelWavelet,			//уровень вейвлет-разложения
	int numPointNotZero)		//минимальное кол-во первых не нулевых отсчётов
{
	bool ret = (pointerDataInp != NULL) && (dimention > 0) &&
		(0 < sizePreHistory) && (sizePreHistory < size) &&
		((pPositionLeft != NULL) || (pPositionRight != NULL));
	if (ret)
	{
		float **ppDataInp, **ppDataOut;
		if (dimention == 1)
		{
			ppDataInp = new float *[1];
			ppDataInp[0] = (float*)pointerDataInp;

			ppDataOut = new float *[1];
			if (pointerDataOut == NULL)
				ppDataOut[0] = new float[size];
			else
				ppDataOut[0] = (float*)pointerDataOut;
		}
		else
		{
			ppDataInp = (float**)pointerDataInp;
			if (pointerDataOut == NULL)
			{
				ppDataOut = new float*[dimention];
				for (int i = 0; i < dimention; ++i)
					ppDataOut[i] = new float[size];
			}
			else
				ppDataOut = (float**)pointerDataOut;
		}

		COncePacketFDWT *pwt = new COncePacketFDWT();
		ret = pwt->Create(typeWavelet, levelWavelet, true, size) >= 0;
		if (ret)
		{
			//преобразование, фильтрация и обратное преобразование
			for (int i = 0; i < dimention; i++)
			{
				ret = ret &&
					pwt->Forward(ppDataInp[i]) &&
					pwt->CalcThreshold(0, sizePreHistory) &&
					pwt->FactorsUnderThreshold() &&
					pwt->Inverse_I() &&
					(ippsCopy_32f(pwt->m_pSignal, ppDataOut[i], size) == ippStsNoErr);
				if ((dimention > 1) && (i < (dimention - 1)))
					ret = ret && pwt->ReStart();
			}
		}
		delete pwt;
		pwt = nullptr;
		if (ret)
		{	// создание массива с модулем ------------------------------
			float *pDataMod = new float[size];
			ret = (ippsSqr_32f(ppDataOut[0], pDataMod, size) == ippStsNoErr);
			if (dimention > 1)
			{
				float *pDataSqr = new float[size];
				if (ret)
				{
					for (int i = 1; i < dimention; i++)
					{
						ret = ret &&
							(ippsSqr_32f(ppDataOut[i], pDataSqr, size) == ippStsNoErr) &&
							(ippsAdd_32f_I(pDataSqr, pDataMod, size) == ippStsNoErr);
					}
				}
				delete[] pDataSqr;
				pDataSqr = nullptr;
			}
			// Поиск -------------------------------------------------------
			if (ret)
			{
				ret = false;
				int nNotZero = 0;
				int position = 0;
				// поиск слева
				if (pPositionLeft != NULL)
				{
					for (int i = 0; i < size; ++i)
					{
						if (pDataMod[i] == 0.f)	// IsNotEqual здесь НЕЛЬЗЯ!!!
							nNotZero = 0;
						else
						{
							if (nNotZero == 0)
								position = i;
							
							if (++nNotZero >= numPointNotZero)
							{
								*pPositionLeft = position;
								ret = true;
								break;
							}
						}
						
						//if (pDataMod[i] != 0.f)	// IsNotEqual здесь НЕЛЬЗЯ!!!
						//{
						//	*pPositionLeft = i;
						//	ret = true;
						//	break;
						//}
					}
				}
				// поиск справа (от максимального значения)
				if (pPositionRight != NULL)
				{
					int nMax;
					float vMax;
					if (ippsMaxIndx_32f(pDataMod, size, &vMax, &nMax) ==
						ippStsNoErr)
					{
						nNotZero = 0;
						position = 0;
						for (int i = nMax - 1; i > 0; --i)
						{
							if (pDataMod[i] == 0.f)	// IsEqual здесь НЕЛЬЗЯ!!!
								nNotZero = 0;
							else
							{
								if (nNotZero == 0)
									position = i;

								if (++nNotZero >= numPointNotZero)
								{	// + 1 = > возврат на не нулевую точку
									*pPositionRight = position + 1;
									ret = true;
									break;
								}

							}

							//if (pDataMod[i] == 0.f)	// IsEqual здесь НЕЛЬЗЯ!!!
							//{	// + 1 = > возврат на не нулевую точку
							//	*pPositionRight = i + 1;
							//	ret = true;
							//	break;
							//}
						}
					}
				}
			}
			delete[] pDataMod;
			pDataMod = nullptr;
		}
		// удаление созданных двумерных массивов
		if (dimention == 1)
		{
			delete[] ppDataInp;
			ppDataInp = nullptr;
			if (pointerDataOut == NULL)
			{ 
				delete[] ppDataOut[0];
				ppDataOut[0] = nullptr;
			}
			delete[] ppDataOut;
			ppDataOut = nullptr;
		}
		else
		{
			if (pointerDataOut == NULL)
			{
				for (int i = 0; i < dimention; i++)
				{ 
					delete[] ppDataOut[i];
					ppDataOut[i] = nullptr;
				}
				delete[] ppDataOut;
				ppDataOut = nullptr;
			}
		}
	}
	return ret;
}
//------------------------------------------------------------------------------
//		Проверка считанных данных с сервера на:
// fault_ErrRead, fault_NaN и fault_NoRange.
//		Возвращаемое значение: true - всё хорошо, false - есть неисправность,
// её тип см. в fault. Если заданы пороговые значения FLT_MIN или FLT_MAX, то
// данная проверка не выполняется.
bool TestDataFromSRV(
	float *pData,		// проверяемые данные
	DWORD sizeData,		// их размер
	float minValue,		// мин. значение или FLT_MIN
	float maxValue,		// макс. значение или FLT_MAX
	int &fault,			// флаги неисправностей
	bool bTestNaN,		// требуется проверка на NaN
	int *pindxMin,		// индекс отсчёта меньшего, чем minValue
	int *pindxMax)		// индекс отсчёта большего, чем maxValue
{	// проверка возможной неисправности
	bool ret = (pData != 0) && (sizeData > 0);
	if (ret)
	{
		if (bTestNaN)
		{
			IppStatus stat = ippsMaxOrder_32f(pData, sizeData, &fault);
			ret = stat == ippStsNoErr;
			if (ret)
			{
				for (DWORD i = 0; i < sizeData; ++i)
				{// проверка на чистые нули, поэтому IsEqual - нельзя
					if (pData[i] == 0.f)
					{
						ret = false;
						fault = fault_NaN;
						break;
					}
				}
			}
			else
			{
				if (stat == ippStsNanArg)
					fault = fault_NaN;
				else
					fault = fault_ErrRead;
			}
		}
		if (ret)
		{
			fault = 0;
			bool bMin = minValue != FLT_MIN;	// IsNotEqual нельзя
			bool bMax = maxValue < FLT_MAX;
			float _min, _max;
			int iMin, iMax;
			if (bMin && bMax)
				ippsMinMaxIndx_32f(pData, sizeData, &_min, &iMin, &_max, &iMax);
			else if (bMin)
				ippsMinIndx_32f(pData, sizeData, &_min, &iMin);
			else if (bMax)
				ippsMaxIndx_32f(pData, sizeData, &_max, &iMax);

			if (bMin && (_min < minValue))
			{
				ret = false;
				fault = fault_NoRangeMin | fault_NoRange;
				if (pindxMin != NULL)
					*pindxMin = iMin;
			}
			if (bMax && (_max > maxValue))
			{
				ret = false;
				fault |= fault_NoRangeMax | fault_NoRange;
				if (pindxMax != NULL)
					*pindxMax = iMax;
			}
		}
	}
	else
		fault = fault_ErrRead;
	return ret;
}
//------------------------------------------------------------------------------
bool WriteDTU(
// Запись данных в DTU-файл данных по X, Y, Z. Наличие папки не проверяется,
// считается, что она уже есть
	CString nameFileFull,	// полное имя файла
	CString title,		// заголовок данных
	int size,			// кол-во кадров
	float FreqADC,		// частота дискретизации
	int numData,		// кол-во столбцов (каналов), не более 3
	DATE T0,			// абсолютное время первого кадра
	double t0,			// время сервера данных первого кадра в сек
	bool bAmpl,			// флаг, что требуется амплитуда
	float *pdata0,		// указатель первых данных
	float *pdata1,		// указатель вторых данных
	float *pdata2,		// указатель третьих данных
	CString name0,		// заголовок первого столбца
	CString name1,		// заголовок второго столбца
	CString name2,		// заголовок третьего столбца
	CString conv0,		// размерность первого столбца
	CString conv1,		// размерность второго столбца
	CString conv2)		// размерность третьего столбца
{
	int i;
	float a, x;
	double tt = t0, dt = 1. / FreqADC;
	FILE *fl;
	CString str, s, s1;
	bool ret = _wfopen_s(&fl, nameFileFull, L"wt, ccs=UTF-8") == 0;
	if (ret && (fl != nullptr))
	{
		str = L"Временная реализация сигнала\n" + title +
			L"\nЧастотный диапазон - от 0Гц до ";
		s.Format(L"%fГц\nДата:\t", FreqADC * 0.5f);
		str += s;
		CZetTime zt(T0);
		s1.Preallocate(32);
		zt.ConvertToString(s1.GetBuffer(), 32, dt, nullptr, false);
		s1.ReleaseBuffer();
		s = s1;		// = ConvertDATEtoString(T0, dt);
		s1.Delete(0, 11);
		s.Delete(10, s.GetLength() - 10);
		str += s + L"\nВремя:\t" + s1 + L"\nВремя\t";
		str += name0;
		if (numData > 1)
			str += L"\t" + name1;
		if (numData > 2)
			str += L"\t" + name2;
		str += L"\nc\t" + conv0;
		if (numData > 1)
			str += L"\t" + conv1;
		if (numData > 2)
			str += L"\t" + conv2;
		str += L"\n";
		fputws(str, fl);

		if (bAmpl)
		{
			for (i = 0; i < size; ++i)
			{
				a = pdata0[i];
				x = a * a;
				a = pdata1[i];
				x += a * a;
				a = pdata2[i];
				x += a * a;
				x = sqrt(x);
				str.Format(L"%.2f\t%g\n", tt, x);
				fputws(str, fl);
				tt += dt;
			}
		}
		else
		{
			for (i = 0; i < size; ++i)
			{
				str.Format(L"%.2f\t%g", tt, pdata0[i]);
				if (numData > 1)
				{
					s.Format(L"\t%g", pdata1[i]);
					str += s;
				}
				if (numData > 2)
				{
					s.Format(L"\t%g", pdata2[i]);
					str += s;
				}
				str += L"\n";
				fputws(str, fl);
				tt += dt;
			}
		}
		fclose(fl);
		fl = NULL;
	}
	return ret;
}
//------------------------------------------------------------------------------
bool CalcAmpl_32f(const float *px, const float *py, const float *pz,
	float *pAmpl, int number, bool bNeedSqrt,
	float *px2, float *py2, float *pz2)	// служебные массивы
{
	bool ret = (px != nullptr) && (py != nullptr) && (pz != nullptr) &&
		(pAmpl != nullptr) && (number > 0);
	if (ret)
	{
		if ((px2 == nullptr) || (py2 == nullptr) || (pz2 == nullptr))
		{// служебные массивы не заданы
			SmartArray<float> sa[3];
			ret = sa[0].ReSize(number) && sa[1].ReSize(number) && sa[2].ReSize(number) &&
				(ippsSqr_32f(px, sa[0].pData, number) == ippStsNoErr) &&
				(ippsSqr_32f(py, sa[1].pData, number) == ippStsNoErr) &&
				(ippsSqr_32f(pz, sa[2].pData, number) == ippStsNoErr) &&
				(ippsAdd_32f(sa[0].pData, sa[1].pData, pAmpl, number) == ippStsNoErr) &&
				(ippsAdd_32f_I(sa[2].pData, pAmpl, number) == ippStsNoErr);
		}
		else
		{// служебные массивы заданы 
			ret = (ippsSqr_32f(px, px2, number) == ippStsNoErr) &&
				(ippsSqr_32f(py, py2, number) == ippStsNoErr) &&
				(ippsSqr_32f(pz, pz2, number) == ippStsNoErr) &&
				(ippsAdd_32f(px2, py2, pAmpl, number) == ippStsNoErr) &&
				(ippsAdd_32f_I(pz2, pAmpl, number) == ippStsNoErr);
		}

		if (bNeedSqrt)
			ret &= (ippsSqrt_32f_I(pAmpl, number) == ippStsNoErr);
	}
	if (!ret && (pAmpl != nullptr))
		ippsZero_32f(pAmpl, number);
	return ret;
}
//------------------------------------------------------------------------------
bool CalcAmpl_32f_I(float *px, float *py, float *pz,
	float *pAmpl, int number, bool bNeedSqrt)
{
	bool ret = (px != nullptr) && (py != nullptr) && (pz != nullptr) &&
		(pAmpl != nullptr) && (number > 0);
	if (ret)
	{
		ret = (ippsSqr_32f_I(px, number) == ippStsNoErr) &&
			(ippsSqr_32f_I(py, number) == ippStsNoErr) &&
			(ippsSqr_32f_I(pz, number) == ippStsNoErr) &&
			(ippsAdd_32f(px, py, pAmpl, number) == ippStsNoErr) &&
			(ippsAdd_32f_I(pz, pAmpl, number) == ippStsNoErr);

		if (bNeedSqrt)
			ret &= (ippsSqrt_32f_I(pAmpl, number) == ippStsNoErr);
	}
	if (!ret && (pAmpl != nullptr))
		ippsZero_32f(pAmpl, number);
	return ret;
}
//------------------------------------------------------------------------------
bool CalcAmpl_64f(const double *px, const double *py, const double *pz,
	double *pAmpl, int number, bool bNeedSqrt,
	double *px2, double *py2, double *pz2)	// служебные массивы
{
	bool ret = (px != nullptr) && (py != nullptr) && (pz != nullptr) &&
		(pAmpl != nullptr) && (number > 0);
	if (ret)
	{
		if ((px2 == nullptr) || (py2 == nullptr) || (pz2 == nullptr))
		{// служебные массивы не заданы
			SmartArray<double> sa[3];
			ret = sa[0].ReSize(number) && sa[1].ReSize(number) && sa[2].ReSize(number) &&
				(ippsSqr_64f(px, sa[0].pData, number) == ippStsNoErr) &&
				(ippsSqr_64f(py, sa[1].pData, number) == ippStsNoErr) &&
				(ippsSqr_64f(pz, sa[2].pData, number) == ippStsNoErr) &&
				(ippsAdd_64f(sa[0].pData, sa[1].pData, pAmpl, number) == ippStsNoErr) &&
				(ippsAdd_64f_I(sa[2].pData, pAmpl, number) == ippStsNoErr);
		}
		else
		{// служебные массивы заданы 
			ret = (ippsSqr_64f(px, px2, number) == ippStsNoErr) &&
				(ippsSqr_64f(py, py2, number) == ippStsNoErr) &&
				(ippsSqr_64f(pz, pz2, number) == ippStsNoErr) &&
				(ippsAdd_64f(px2, py2, pAmpl, number) == ippStsNoErr) &&
				(ippsAdd_64f_I(pz2, pAmpl, number) == ippStsNoErr);
		}

		if (bNeedSqrt)
			ret &= (ippsSqrt_64f_I(pAmpl, number) == ippStsNoErr);
	}
	if (!ret && (pAmpl != nullptr))
		ippsZero_64f(pAmpl, number);
	return ret;
}
//------------------------------------------------------------------------------
bool CalcAmpl_64f_I(double *px, double *py, double *pz,
	double *pAmpl, int number, bool bNeedSqrt)
{
	bool ret = (px != nullptr) && (py != nullptr) && (pz != nullptr) &&
		(pAmpl != nullptr) && (number > 0);
	if (ret)
	{
		ret = (ippsSqr_64f_I(px, number) == ippStsNoErr) &&
			(ippsSqr_64f_I(py, number) == ippStsNoErr) &&
			(ippsSqr_64f_I(pz, number) == ippStsNoErr) &&
			(ippsAdd_64f(px, py, pAmpl, number) == ippStsNoErr) &&
			(ippsAdd_64f_I(pz, pAmpl, number) == ippStsNoErr);

		if (bNeedSqrt)
			ret &= (ippsSqrt_64f_I(pAmpl, number) == ippStsNoErr);
	}
	if (!ret && (pAmpl != nullptr))
		ippsZero_64f(pAmpl, number);
	return ret;
}
//------------------------------------------------------------------------------