//-----------------------------------------------------------------------------
#include "stdafx.h"
#include <mathZet.h>
#include "TestValueCFG.h"
//-----------------------------------------------------------------------------
bool Convert_TypeTimeAver_TimeInSec(const TypeTimeAver type, float & timeInSec)
{
	bool ret = type < tta_size;
	if (ret)
	{
		switch (type)
		{
		case tta_0_01:	timeInSec = 0.01f;	break; 
		case tta_0_02:	timeInSec = 0.02f;	break;
		case tta_0_04:	timeInSec = 0.04f;	break;
		case tta_0_05:	timeInSec = 0.05f;	break;
		case tta_0_1:	timeInSec = 0.1f;	break;
		case tta_0_2:	timeInSec = 0.2f;	break;
		case tta_0_4:	timeInSec = 0.4f;	break;
		case tta_0_5:	timeInSec = 0.5f;	break;
		case tta_1:		timeInSec = 1.f;	break;
		case tta_2:		timeInSec = 2.f;	break;
		case tta_4:		timeInSec = 4.f;	break;
		case tta_5:		timeInSec = 5.f;	break;
		case tta_10:	timeInSec = 10.f;	break;
		case tta_20:	timeInSec = 20.f;	break;
		case tta_40:	timeInSec = 40.f;	break;
		case tta_50:	timeInSec = 50.f;	break;
		case tta_100:	timeInSec = 100.f;	break;
		case tta_200:	timeInSec = 200.f;	break;
		default:
			break;
		}
	}
	else
		timeInSec = 0.f;
	return ret;
}
//-----------------------------------------------------------------------------
bool Convert_TypeFreqResolutionFFT_OrderFFT(const TypeFreqResolution_FFT type, int & order)
{
	bool ret = type < tfrFFT_size;
	if (ret)
		order = (int)(BYTE)type + 7;
	else
		order = 0;
	return ret;
}
//-----------------------------------------------------------------------------
bool Convert_TypeFreqResolutionDFT_K(const TypeFreqResolution_DFT type, int & k)
{
	bool ret = type < tfrDFT_size;
	if (ret)
	{
		switch (type)
		{
		case tfrDFT_100:	k = 100;		break;
		case tfrDFT_125:	k = 125;		break;
		case tfrDFT_200:	k = 200;		break;
		case tfrDFT_250:	k = 250;		break;
		case tfrDFT_400:	k = 400;		break;
		case tfrDFT_500:	k = 500;		break;
		case tfrDFT_800:	k = 800;		break;
		case tfrDFT_1000:	k = 1000;		break;
		case tfrDFT_1250:	k = 1250;		break;
		case tfrDFT_2000:	k = 2000;		break;
		case tfrDFT_2500:	k = 2500;		break;
		case tfrDFT_4000:	k = 4000;		break;
		case tfrDFT_5000:	k = 5000;		break;
		case tfrDFT_8000:	k = 8000;		break;
		case tfrDFT_10000:	k = 10000;		break;
		case tfrDFT_12500:	k = 12500;		break;
		case tfrDFT_20000:	k = 20000;		break;
		case tfrDFT_25000:	k = 25000;		break;
		case tfrDFT_40000:	k = 40000;		break;
		case tfrDFT_50000:	k = 50000;		break;
		case tfrDFT_80000:	k = 80000;		break;
		case tfrDFT_100000:	k = 100000;		break;
		default:
			break;
		}
	}
	else
		k = 0;
	return ret;
}
//-----------------------------------------------------------------------------
bool Convert_TypeFreqResolutionDFT_dF(const TypeFreqResolution_DFT type, const float freqADC, float & dF)
{
	int k(0);
	bool ret = Convert_TypeFreqResolutionDFT_K(type, k);
	if (ret)
		dF = freqADC / k;
	else
		dF = 0.f;
	return ret;
}
//-----------------------------------------------------------------------------
bool Convert_TypeTimeWindows_TypeWindow(const TypeTimeWindow ttw, TypeWindow & tw)
{
	bool ret = ttw < ttw_size;
	if (ret)
	{
		switch (ttw)
		{
		case ttw_no:				tw = win_no;				break;
		case ttw_Hann:				tw = win_Hann;				break;
		case ttw_Hanning:			tw = win_Hanning;			break;
		case ttw_Hamming:			tw = win_Hamming;			break;
		case ttw_Blackman:			tw = win_Blackman;			break;
		case ttw_Bartlett:			tw = win_Bartlett;			break;
		case ttw_Kaiser:			tw = win_Kaiser;			break;
		case ttw_RifeVincent_4:		tw = win_RifeVincent_4;		break;
		case ttw_BlackmanHarris_3:	tw = win_BlackmanHarris_3;	break;
		case ttw_BlackmanHarris_4:	tw = win_BlackmanHarris_4;	break;
		case ttw_Nuttall:			tw = win_Nuttall;			break;
		case ttw_BlackmanNuttall:	tw = win_BlackmanNuttall;	break;
		case ttw_Flattop:			tw = win_Flattop;			break;
		default:					tw = win_no;				break;
		}
	}
	else
		tw = win_no;
	return ret;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool Test_Decada(BYTE & newVal, const bool bChange)
{
	return TestAndChangeValue(newVal, (BYTE)0, (BYTE)g_decada_Max, bChange);
}
//-----------------------------------------------------------------------------
bool Test_FreqResolution_FFT(TypeFreqResolution_FFT & newVal, const bool bChange)
{
	return TestAndChangeValueOfEnum(newVal, tfrFFT_size, bChange);
}
//-----------------------------------------------------------------------------
bool Test_FreqResolution_DFT(TypeFreqResolution_DFT & newVal, const bool bChange)
{
	return TestAndChangeValueOfEnum(newVal, tfrDFT_size, bChange);
}
//-----------------------------------------------------------------------------
bool Test_TypeTimeAver(TypeTimeAver & newVal, const bool bChange)
{
	return TestAndChangeValueOfEnum(newVal, tta_size, bChange);
}
//-----------------------------------------------------------------------------
bool Test_TypeViewValue(TypeViewValue & newVal, const bool bChange)
{
	return TestAndChangeValueOfEnum(newVal, tvv_size, bChange);
}
//-----------------------------------------------------------------------------
bool Test_TypeWindow(TypeWindow & newVal, const bool bChange)
{
	return TestAndChangeValueOfEnum(newVal, win_size, bChange);
}
//-----------------------------------------------------------------------------
bool Test_TypeSignalProcessing(TypeSignalProcessing & newVal, const bool bChange)
{
	return TestAndChangeValueOfEnum(newVal, tsp_size, bChange);
}
//-----------------------------------------------------------------------------
bool Test_TypeTransform(TypeTransform & newVal, const bool bChange)
{
	return TestAndChangeValueOfEnum(newVal, tt_size, bChange);
}
//-----------------------------------------------------------------------------
bool Test_TimeAver_sec(float & newVal, const bool bChange)
{
	return TestAndChangeValue(newVal, g_timeAver_Min, g_timeAver_Max, bChange);
}
//-----------------------------------------------------------------------------
bool Test_IntervalGraf_Min(float & newVal, const bool bChange)
{
	return TestAndChangeValue(newVal, g_intervalGraf_Minute_Min, g_intervalGraf_Minute_Max, bChange);
}
//-----------------------------------------------------------------------------
// template <class T>				// T - это float, double
// bool Test_dF_DFT_Hz(T & newVal, const bool bChange, const T freqADC)
// {
// 	T valMin((T)0), valMax((T)0);
// 	Convert_TypeFreqResolutionDFT_dF(tfrDFT_100000, freqADC, valMin);
// 	Convert_TypeFreqResolutionDFT_dF(tfrDFT_100, freqADC, valMax);
// 
// 	return TestAndChangeValue(newVal, valMin, valMax, bChange);
// }
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
Struct_ParamLineGridGL::Struct_ParamLineGridGL()
{	// Инициализацию членов структуры не делаю, чтобы значения по умолчанию
	// были только в одном месте - в функции SetValueDefault
	SetValueDefault();
}
//-----------------------------------------------------------------------------
void Struct_ParamLineGridGL::SetValueDefault()
{
	m_valid = 1;		// отображение линии в GridGL
	m_type_fill = 0;	// заполнение под линией: 0 - без, 1 - с заполнением
	m_width = 2;		// толщина линии
	m_color = ZETColors::Graph_Colors[0];	// цвет линии
}
//-----------------------------------------------------------------------------
// Копирование параметров из GridGL в структуру
void Struct_ParamLineGridGL::GetParamFromGrid(CGridglctrl1 * pGridGL, const long indexLine)
{
	pGridGL->SetInd(indexLine);
	m_valid = pGridGL->GetValid();
	m_type_fill = pGridGL->GetTypeFill();
	m_width = pGridGL->GetLineWidth();
	m_color = pGridGL->GetClrGrf();
}
//-----------------------------------------------------------------------------
// Установка параметров из структуры в GridGL
void Struct_ParamLineGridGL::SetParamIntoGrid(CGridglctrl1 * pGridGL, const long indexLine)
{
	pGridGL->SetInd(indexLine);
	pGridGL->SetValid(m_valid);
	pGridGL->SetTypeFill(m_type_fill);
	pGridGL->SetLineWidth(m_width);
	pGridGL->SetClrGrf(m_color);
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
Struct_ParamGridGL::Struct_ParamGridGL()
{	// Инициализацию членов структуры не делаю, чтобы значения по умолчанию
	// были только в одном месте - в функции SetValueDefault
	SetValueDefault();
}
//-----------------------------------------------------------------------------
void Struct_ParamGridGL::SetValueDefault()
{
	m_bDefault = true;		// это означает, что значения установлены по умолчанию
	m_bBoundaries_X = false;
	m_bBoundaries_Y = false;
	m_numLines = 1;			// количество графиков в GridGL
	m_param.SetValueDefault();
}
//-----------------------------------------------------------------------------
// Копирование параметров из GridGL в структуру
void Struct_ParamGridGL::GetParamFromGrid(CGridglctrl1 * pGridGL)
{
	m_bDefault = false;
	m_numLines = pGridGL->GetNumber();
	m_param.GetParamFromGrid(pGridGL);
}
//-----------------------------------------------------------------------------
// Установка параметров из структуры в GridGL
void Struct_ParamGridGL::SetParamIntoGrid(CGridglctrl1 * pGridGL)
{
	pGridGL->SetNumber(m_numLines);
	m_param.SetParamIntoGrid(pGridGL);
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
Struct_ParamGramma::Struct_ParamGramma()
{	// Инициализацию членов структуры не делаю, чтобы значения по умолчанию
	// были только в одном месте - в функции SetValueDefault
	SetValueDefault();
}
//-----------------------------------------------------------------------------
void Struct_ParamGramma::SetValueDefault()
{
	m_bDefault = true;	// это означает, что значения установлены по умолчанию
	m_bBoundaries_X = false;
	m_bBoundaries_Y = false;
	m_bBoundaries_Z = false;
	m_black = 0;		// 0 - цветное изображение, 1 - чёрно-белое изображение {0}
	m_Px = 0;			// координата курсора {0}
	m_Py = 0;			// координата курсора {0}
	m_type_Abs = 0;		// type_Abs - тип осей {0}:
	m_log = 0;			// отображение в дБ {0}

	m_color_Crs = ZETColors::CursorColor;
	m_color_Dig = ZETColors::DigitsColor;
	m_color_Fon = ZETColors::BackgroundColor;
	m_color_Grd = ZETColors::GridColor;
	m_color_Leg = ZETColors::LegendColor;

	m_make_Upor = 0;	// использование границ полного отображения по Y {0}

	m_upor_Nis = 0.;	// нижняя граница полного отображения по Y {0}
	m_upor_Verh = 0.;	// верхняя граница полного отображения по Y {0}
	m_math_ly = 0.;		// нижняя граница текущего отображения по Y {0}
	m_math_dy = 0.;		// длина текущего отображения по Y {0}

	m_Xfirst = 0.;		// левая граница полного отображения по Х {0}
	m_Xend = 0.;			// правая граница полного отображения по Х {0}
	m_math_lx = 0.;		// левая граница текущего отображения по Х {0}
	m_math_dx = 0.;		// длина текущего отображения по Х {0}

	m_deltaT = 0.;		// шаг по времени
}
//-----------------------------------------------------------------------------
// Копирование параметров из Gramma в структуру
void Struct_ParamGramma::GetParamFromGramma(CGrammactrl1 * pGramma)
{
	m_bDefault = false;

	m_black = pGramma->GetBlack();
	m_Px = pGramma->GetPx();
	m_Py = pGramma->GetPy();
	m_type_Abs = pGramma->GetTypeAbs();
	m_log = pGramma->Getlog();
	m_color_Fon = pGramma->GetClrFon();
	m_color_Grd = pGramma->GetClrGrd();
	m_color_Crs = pGramma->GetClrCrs();
	m_color_Dig = pGramma->GetClrDig();
	m_color_Leg = pGramma->GetClrLeg();
	m_make_Upor = pGramma->GetMakeUpor();
	m_upor_Nis = pGramma->GetUporNis();
	m_upor_Verh = pGramma->GetUporVerh();
	m_math_ly = pGramma->GetMathly();
	m_math_dy = pGramma->GetMathdy();
	m_Xfirst = pGramma->GetXfirst();
	m_Xend = pGramma->GetXend();
	m_math_lx = pGramma->GetMathlx();
	m_math_dx = pGramma->GetMathdx();
	m_deltaT = pGramma->GetDeltaT();
}
//-----------------------------------------------------------------------------
// Установка параметров из структуры в Gramma
void Struct_ParamGramma::SetParamIntoGramma(CGrammactrl1 * pGramma)
{
	pGramma->SetBlack(m_black);
	pGramma->SetPx(m_Px);
	pGramma->SetPy(m_Py);
	pGramma->SetTypeAbs(m_type_Abs);
	pGramma->Setlog(m_log);
	pGramma->SetClrFon(m_color_Fon);
	pGramma->SetClrGrd(m_color_Grd);
	pGramma->SetClrCrs(m_color_Crs);
	pGramma->SetClrDig(m_color_Dig);
	pGramma->SetClrLeg(m_color_Leg);
	pGramma->SetMakeUpor(m_make_Upor);
	pGramma->SetUporNis(m_upor_Nis);
	pGramma->SetUporVerh(m_upor_Verh);
	pGramma->SetMathly(m_math_ly);
	pGramma->SetMathdy(m_math_dy);
	pGramma->SetXfirst(m_Xfirst);
	pGramma->SetXend(m_Xend);
	pGramma->SetMathlx(m_math_lx);
	pGramma->SetMathdx(m_math_dx);
	pGramma->SetDeltaT(m_deltaT);
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
Struct_ParamGrammaGL::Struct_ParamGrammaGL()
{	// Инициализацию членов структуры не делаю, чтобы значения по умолчанию
	// были только в одном месте - в функции SetValueDefault
	SetValueDefault();
}
//-----------------------------------------------------------------------------
void Struct_ParamGrammaGL::SetValueDefault()
{
	m_bDefault = true;		// значения заданы по умолчанию
	m_bBoundaries_X = false;
	m_bBoundaries_Y = false;
	m_bBoundaries_Z = false;

	m_black = 0;			// 0 - цветное изображение, 1 - чёрно-белое изображение
	m_Px = 0;				// координата курсора
	m_Py = 0;				// координата курсора
	m_altCoords = 0;
	m_type_Abs = 0;
	m_ordinateData = 0;

	m_colorMin = 0.f;
	m_colorWidth = 0.f;

	m_color_Dig = ZETColors::DigitsColor;		// цвет цифр на сетке
	m_color_Fon = ZETColors::BackgroundColor;	// цвет фона
	m_color_Grd = ZETColors::GridColor;			// цвет сетки
	m_color_Leg = ZETColors::LegendColor;		// цвет легенды

	m_typeYAxis = 0;
	m_typeXAxis = 0;
	m_typeYData = 0;
	m_make_Upor = 0;		// использование границ полного отображения по Y {0}

	m_upor_Nis = 0.;	// нижняя граница полного отображения по Y {0}
	m_upor_Verh = 0.;	// верхняя граница полного отображения по Y {0}
	m_math_ly = 0.;		// нижняя граница текущего отображения по Y {0}
	m_math_dy = 0.;		// длина текущего отображения по Y {0}

	m_Xfirst = 0.;		// левая граница полного отображения по Х {0}
	m_Xend = 0.;		// правая граница полного отображения по Х {0}
	m_math_lx = 0.;		// левая граница текущего отображения по Х {0}
	m_math_dx = 0.;		// длина текущего отображения по Х {0}

	m_deltaT = 0.;		// шаг по времени
	m_reference = 1.;
}
//-----------------------------------------------------------------------------
// Копирование параметров из GrammaGL в структуру
void Struct_ParamGrammaGL::GetParamFromGramma(CGrammaglctrl1 * pGrammaGL)
{
	m_bDefault = false;

	m_black = pGrammaGL->GetBlack();
	m_Px = pGrammaGL->GetPx();
	m_Py = pGrammaGL->GetPy();

	m_altCoords = pGrammaGL->GetAltCoords();
	m_type_Abs = pGrammaGL->GetTypeAbs();
	m_ordinateData = pGrammaGL->GetOrdinateData();

	m_colorMin = pGrammaGL->GetColorMin();
	m_colorWidth = pGrammaGL->GetColorWidth();

	m_color_Fon = pGrammaGL->GetClrFon();
	m_color_Grd = pGrammaGL->GetClrGrd();
	m_color_Dig = pGrammaGL->GetClrDig();
	m_color_Leg = pGrammaGL->GetClrLeg();

	m_typeYAxis = pGrammaGL->GetTypeYAxis();
	m_typeXAxis = pGrammaGL->GetTypeXAxis();
	m_typeYData = pGrammaGL->GetTypeYData();
	m_make_Upor = pGrammaGL->GetMakeUpor();

	m_upor_Nis = pGrammaGL->GetUporNis();
	m_upor_Verh = pGrammaGL->GetUporVerh();
	m_math_ly = pGrammaGL->GetMathly();
	m_math_dy = pGrammaGL->GetMathdy();
	m_Xfirst = pGrammaGL->GetXfirst();
	m_Xend = pGrammaGL->GetXend();
	m_math_lx = pGrammaGL->GetMathlx();
	m_math_dx = pGrammaGL->GetMathdx();
	m_deltaT = pGrammaGL->GetDeltaT();
	m_reference = pGrammaGL->GetReference();
}
//-----------------------------------------------------------------------------
// Установка параметров из структуры в GrammaGL
void Struct_ParamGrammaGL::SetParamIntoGramma(CGrammaglctrl1 * pGrammaGL)
{
	pGrammaGL->SetBlack(m_black);
	pGrammaGL->SetPx(m_Px);
	pGrammaGL->SetPy(m_Py);

	pGrammaGL->SetAltCoords(m_altCoords);
	pGrammaGL->SetTypeAbs(m_type_Abs);
	pGrammaGL->SetOrdinateData(m_ordinateData);

	pGrammaGL->SetColorMin(m_colorMin);
	pGrammaGL->SetColorWidth(m_colorWidth);

	pGrammaGL->SetClrFon(m_color_Fon);
	pGrammaGL->SetClrGrd(m_color_Grd);
	pGrammaGL->SetClrDig(m_color_Dig);
	pGrammaGL->SetClrLeg(m_color_Leg);

	pGrammaGL->SetTypeYAxis(m_typeYAxis);
	pGrammaGL->SetTypeXAxis(m_typeXAxis);
	pGrammaGL->SetTypeYData(m_typeYData);
	pGrammaGL->SetMakeUpor(m_make_Upor);

	pGrammaGL->SetUporNis(m_upor_Nis);
	pGrammaGL->SetUporVerh(m_upor_Verh);
	pGrammaGL->SetMathly(m_math_ly);
	pGrammaGL->SetMathdy(m_math_dy);
	pGrammaGL->SetXfirst(m_Xfirst);
	pGrammaGL->SetXend(m_Xend);
	pGrammaGL->SetMathlx(m_math_lx);
	pGrammaGL->SetMathdx(m_math_dx);
	pGrammaGL->SetDeltaT(m_deltaT);
	pGrammaGL->SetReference(m_reference);
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
CfgOneKit::CfgOneKit()
{
	SetValueDefault();
}
//-----------------------------------------------------------------------------
void CfgOneKit::SetValueDefault()
{
	m_bDecade = 0;
	m_bType_dF_FFT = tfrFFT_10;
	m_bType_Aver = ta_line;
	m_bType_ViewValue = tvv_skz;
	m_bValueIn_dB = false;
	m_bWriteToLogger = false;
	m_bType_TimeWindow = ttw_Hanning;
	m_bType_SigProc = tsp_no;
	m_bType_Transform = tt_DFT;
	m_bFilteringDiscrete = false;

	m_indexKit = 0;
	m_fdF_DFT_Hz = 1.f;
	m_fTimeAver_sec = 1.f;
	ZeroMemory(m_sWriteComment, sizeof(m_sWriteComment));

	m_lineInMCM.SetValueDefault();
	m_par_addGraf.SetValueDefault();

}
//-----------------------------------------------------------------------------
void SaveStructure::SetValueDefault(CDSRV * psrv)
{
	m_bMCM = false;
	m_bTypeGraf_mcm = tvv_skz;

	m_num_kits = 1;
	m_indx_kitCur = 0;

	m_parWin_Main.SetValueDefault();
	m_parWin_Kepstr.SetValueDefault();
	m_parWin_2D.SetValueDefault();
	m_parWin_3D.SetValueDefault();
	m_parWin_Prohod.SetValueDefault();
	m_parWin_Reson.SetValueDefault();

	m_par_kit_0.SetValueDefault();

	size = sizeof(SaveStructure);
	wcscpy_s(nameProgram, L"spectr.exe");
	
	// стартовый канал сервера
	if (psrv != nullptr)
	{
		WORD numExe(0);		// номер запущенной Ехе, счет с 1
		ztGetNumberSelfExe(&numExe);
		long сhan = (long)numExe - 1;	// канал, с которым будет работать программа

		long quanChan = psrv->QuanChan();	// кол-во каналов сервера
		if (сhan >= quanChan)
			сhan = 0;

		bool bOk = psrv->Frequency(сhan) >= 1.f;

		if (!bOk)
		{
			long chCur(сhan);
			for (++сhan; сhan < quanChan; ++сhan)
			{
				bOk = psrv->Frequency(сhan) >= 1.f;
				if (bOk)
					break;
			}
			if (!bOk)
			{
				for (сhan = 0; сhan < chCur; ++сhan)
				{
					bOk = psrv->Frequency(сhan) >= 1.f;
					if (bOk)
						break;
				}
			}
		}
		if (bOk)
			psrv->IdChan(сhan, &m_par_kit_0.m_guidChan);
	}
}
//-----------------------------------------------------------------------------
// Проверка размера
bool SaveStructure::IsSizeOk()
{
	return (DWORD)size == CalcSizeFileCFG(m_num_kits);
}
//-----------------------------------------------------------------------------
// Получение указателя на CFG без проверки размера
ParamOneKit * SaveStructure::GetPointer(DWORD indxKit)
{
	ParamOneKit * pret = (indxKit < m_num_kits) ? &m_par_kit_0 : nullptr;
	if (pret != nullptr)
	{
		pret += indxKit;
	}
	return pret;
}
//-----------------------------------------------------------------------------
// Вычисление размера CFG файла
DWORD CalcSizeFileCFG(const DWORD numKits)
{
	DWORD ret((DWORD)sizeof(SaveStructure));
	if (numKits > 1)
		ret += (numKits - 1) * (DWORD)sizeof(ParamOneKit);
	return ret;
}
//-----------------------------------------------------------------------------
// Проверка размера CFG файла.
// Возвращает true если файл с таким размером может быть CFG-файлом
bool TestSizeFileCFG(const DWORD sizeFile)
{
	DWORD len1 = (DWORD)sizeof(ParamOneKit);
	DWORD num = (sizeFile - (DWORD)sizeof(SaveStructure)) / len1 + 1;
	return sizeFile == CalcSizeFileCFG(num);
}
//-----------------------------------------------------------------------------
// Расчет параметров DFT:
// freqADC - частота дискретизации с учётом декады;
// df - требуемое частотное разрешение DFT;
// sizeDFT - полный размер массива спектра DFT (от 0 до freqADC);
// sizeFFT - размер исходных данных при расчёте FFT;
// orderFFT - порядок, выполняемого FFT
//
// void GetParamDFT(const float freqADC, const float df, int & sizeDFT, int & sizeFFT, int & orderFFT)
// {
// 	sizeDFT = RoundToInt(freqADC / df);
// 	double d1 = log2(sizeDFT);
// 	orderFFT = (int)d1;
// 	if (d1 > (double)orderFFT)
// 		++orderFFT;
// 	orderFFT += 2;
// 	sizeFFT = RoundToInt(pow(2, orderFFT));
// }
//-----------------------------------------------------------------------------