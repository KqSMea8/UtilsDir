#include "calculation_spectr.h"
#include "window_funcs.h"
#include "ZetErrorCodes.h"
#include "mathZet.h"

#include <ipps.h>
#include <ippvm.h>
#include <ippcore.h>

#include <vector>

using namespace calculation;

long generate_transform(std::vector<Ipp32fc> &prm3, long sizep, std::vector<Ipp32fc> &prm4, long sizez)
{
	for (int i = 0; i < sizep; i++)
	{
		prm3[i].re = (float)cos(i * M_PI / sizep*(double)(i));
		prm3[i].im = (float)sin(i * M_PI / sizep*(double)(i));
	}
	for (int i = 0, size = prm4.size() / 2; i <= size; i++)
	{
		prm4[i].re = (float)cos(i * M_PI / sizep*(double)(i));
		prm4[i].im = -(float)sin(i * M_PI / sizep*(double)(i));
		if (i != 0)
			prm4[sizez - i] = prm4[i];
	}
	
	long order = (int)(log((double)sizez) / log((double)2.));

	IppStatus ippReturn;
	IppsFFTSpec_C_32fc* ippsSpec;
	ippReturn = ippsFFTInitAlloc_C_32fc(&ippsSpec, order, IPP_FFT_DIV_INV_BY_N, ippAlgHintNone);
	ippReturn = ippsFFTFwd_CToC_32fc_I(prm4.data(), ippsSpec, NULL);
	ippsFFTFree_C_32fc(ippsSpec);

	return NoError;
}

long calculate_fft(float *pin, float *pout, long size)
{
	long order = (int)(log((double)size) / log((double)2.));
	long order_size = 1 << order;

	if (size != order_size)
		return Err_Size_Invalid;

	Ipp32fc zeroc;
	zeroc.re = 0;
	zeroc.im = 0;
	std::vector<float> temp(order_size, 0);
	std::vector<Ipp32fc> spec(order_size, zeroc);

	IppStatus ippReturn;

	// ‘ормируем комплексный массив с нулевой мнимой частью
	ippReturn = ippsRealToCplx_32f(pin, temp.data(), spec.data(), order_size);
	if (ippReturn != ippStsNoErr)
		return ippReturn;

	// »нициализируем необходимую структуру IPP
	IppsFFTSpec_C_32fc *ippsSpec;
	ippReturn = ippsFFTInitAlloc_C_32fc(&ippsSpec, order, IPP_FFT_DIV_INV_BY_N, ippAlgHintNone);
	if (ippReturn != ippStsNoErr)
		return ippReturn;
	if (ippsSpec == nullptr)
		return Err_Calc;

	// ¬ыполн€ем преобразование 
	ippReturn = ippsFFTFwd_CToC_32fc_I(spec.data(), ippsSpec, NULL);
	if (ippReturn != ippStsNoErr)
		return ippReturn;

	// ¬ычисл€ем спектр мощности
	ippReturn = ippsPowerSpectr_32fc(spec.data(), pout, order_size);
	if (ippReturn != ippStsNoErr)
		return ippReturn;

	ippsFFTFree_C_32fc(ippsSpec);

	return NoError;
}

long calculate_dft(float *pin, float *pout, long size)
{
	// ¬ычисл€ем достаточный дл€ преобразовани€ ‘урье пор€док массива
	long order = (int)(log((double)size) / log((double)2.)) + 1;
	long order_size = 1 << order;

	Ipp32fc zeroc;
	zeroc.re = 0;
	zeroc.im = 0;
	std::vector<float> temp(order_size, 0);
	std::vector<Ipp32fc> prm2(order_size, zeroc);
	std::vector<Ipp32fc> prm3(order_size, zeroc);
	std::vector<Ipp32fc> prm4(order_size, zeroc);
	std::vector<Ipp32fc> spec(order_size, zeroc);

	IppStatus ippReturn;

	generate_transform(prm3, size, prm4, order_size);

	// ‘ормируем комплексный массив с нулевой мнимой частью
	ippReturn = ippsRealToCplx_32f(pin, temp.data(), prm2.data(), size);
	if (ippReturn != ippStsNoErr)
		return ippReturn;
	// ќбнул€ем элементы комплексного массива после необходимого количества отсчЄтов
	if (order_size > size)
		ippsZero_32fc(prm2.data() + size, order_size - size);

	ippsMul_32fc_I(prm3.data(), prm2.data(), size);

	IppsFFTSpec_C_32fc* ippsSpec;

	// ¬ыполн€ем хитрое преобразование дл€ изменени€ частотного разрешени€ при преобразовании
	ippReturn = ippsFFTInitAlloc_C_32fc(&ippsSpec, order, IPP_FFT_DIV_INV_BY_N, ippAlgHintNone);
	ippReturn = ippsFFTFwd_CToC_32fc_I(prm2.data(), ippsSpec, NULL);

	ippsMul_32fc_I(prm4.data(), prm2.data(), order_size);

	ippsFFTFree_C_32fc(ippsSpec);

	ippReturn = ippsFFTInitAlloc_C_32fc(&ippsSpec, order, IPP_FFT_DIV_FWD_BY_N, ippAlgHintNone);
	ippReturn = ippsFFTFwd_CToC_32fc_I((Ipp32fc*)prm2.data(), ippsSpec, NULL);

	ippsPowerSpectr_32fc(prm2.data(), pout, size);

	ippsFFTFree_C_32fc(ippsSpec);

	return NoError;
}

long calculate_ft(float *pin, float *pout, FT_PARAMETERS *params)
{
	if (pin == nullptr)
		return Err_NULL_Pointer;

	if (pout == nullptr)
		return Err_NULL_Pointer;

	if (params->_size < 2)
		return Err_Size_Zero;

	// ”даление посто€нной составл€ющей из входного массива
	float mean(0);
	ippsMean_32f(pin, params->_size, &mean, ippAlgHintAccurate);
	ippsAddC_32f_I(-mean, pin, params->_size);

	// ”множение на весовое окно
	std::vector<float> win(params->_size, 0);
	switch (params->_win)
	{
	case WIN_RECTANGLE:
		ippsSet_32f(1.f, win.data(), params->_size);
		break;
	case WIN_HANN:
		ippsWinHann_32f_I(win.data(), params->_size);
		break;
	case WIN_HANNING:
		Hanning_32f(win.data(), params->_size);
		break;
	case WIN_HAMMING:
		ippsWinHamming_32f_I(win.data(), params->_size);
		break;
	case WIN_BLACKMAN:
		ippsWinBlackmanOpt_32f_I(win.data(), params->_size);
		break;
	case WIN_BARTLETT:
		ippsWinBartlett_32f_I(win.data(), params->_size);
		break;
	case WIN_KAIZER:
		ippsWinKaiser_32f_I(win.data(), params->_size, 1.0f);
		break;
	case WIN_RIFE_VINCENT_4:
		RifeVincent4_32f(win.data(), params->_size);
		break;
	case WIN_BLACKMAN_HARRIS_3:
		BlackmanHarris_3_67_32f(win.data(), params->_size);
		break;
	case WIN_BLACKMAN_HARRIS_4:
		BlackmanHarris_4_94_32f(win.data(), params->_size);
		break;
	case WIN_NUTTALL:
		Nuttal_32f(win.data(), params->_size);
		break;
	case WIN_BLACKMAN_NUTTALL:
		BlackmanNuttal_32f(win.data(), params->_size);
		break;
	case WIN_FLAT_TOP:
		Flattop_32f(win.data(), params->_size);
		break;
	}

	// ¬ыполн€ем нужное преобразование
	long result(0);
	switch (params->_type)
	{
	case FFT: 
		result = calculate_fft(pin, pout, params->_size); 
		break;
	case DFT: 
		result = calculate_dft(pin, pout, params->_size); 
		break;
	default:
		break;
	}

	return result;
}

long calculate_spectr(float* data, long data_size, double data_time, 
	float *spectr, long &spectr_size, double &spectr_time, SPECTR_PARAMETERS params)
{
	if (data_time <= 0 || data_size <= 0 || data == nullptr || spectr == nullptr)
		return InvalidParams;

	// считаем размер массива спектра или частотное разрешение в зависимости от типа преобразовани€
	switch (params._type)
	{
	case FFT:
		params._resolution = params._sampling / params._size;
		break;
	case DFT:
		params._size = long(params._sampling / params._resolution + 0.5);
		break;
	default:
		break;
	}

	spectr_size = params._size / 2;

	// врем€ одного усреднени€
	double one_time = (double)params._size / (double)params._resolution;
	
	// нарезаем входной массив на участки дл€ вычислени€ и усредн€ем результаты
	if (params._overlay > 0.9f)
		params._overlay = 0.9f;
	if (params._overlay < 0.0f)
		params._overlay = 0.0f;

	// провер€ем достаточность входных данных
	if (spectr_time < data_time)
		return Err_Value_Less;

	// минимальный требуемый размер данных дл€ выборки всех интервалов и вычислени€ спектра
	long required_size = long((spectr_time - data_time + params._average + one_time) * params._sampling + 0.5);
	if (required_size < data_size)
		return Err_Value_Less;

	long index(0);		// индекс элемента в массиве
	long count(0);		// количество вычисленных спектров
	long result(0);
	long first_point = long((spectr_time - data_time) * params._sampling + 0.5);
	long last_point = long((spectr_time - data_time + params._average) * params._sampling + 0.5);

	index = first_point;
	ippsZero_32f(spectr, spectr_size);
	std::vector<float> temp(params._size, 0);
	while (index < last_point)
	{
		calculation::FT_PARAMETERS ftparam;
		ftparam._size = params._size;
		ftparam._type = params._type;
		ftparam._win = params._win;
		result = calculation::calculate_ft(&data[index], temp.data(), &ftparam);
		if (result != NoError)
			return result;
		count++;
		index += long(one_time * params._sampling);
	}

	ippsDivC_32f_I((float)count, spectr, spectr_size);

	return NoError;
}