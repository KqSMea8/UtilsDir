//------------------------------------------------------------------------------
//	Файл CustomDWF.cpp
//	Зеленоград, ЗАО "ЭТМС", Савостин Ю.М.
//------------------------------------------------------------------------------
#include "StdAfx.h"
#include "CustomFDWT.h"
//------------------------------------------------------------------------------
CCustomFDWT::CCustomFDWT()
{	
	fwdFltOffs = -1;
	invFltOffs = 0;
	bInitForward = bInitInverse = false;
	FQuantityWavelet = 23;
	FLevelMaxAll = 13;
	FTypeWavelet = no;
	FLengthFilter =
		FLevel = FLevelMaxCur = FLevelOld = 
		FLengthSignalOld = FLengthSignal = 0;

	m_pNameWavelet = new CString [FQuantityWavelet];
	m_pNameWavelet[0] = L"Haar";
	m_pNameWavelet[1] = L"db1";
	m_pNameWavelet[2] = L"db2";
	m_pNameWavelet[3] = L"db3";
	m_pNameWavelet[4] = L"db4";
	m_pNameWavelet[5] = L"db5";
	m_pNameWavelet[6] = L"db6";
	m_pNameWavelet[7] = L"db7";
	m_pNameWavelet[8] = L"db8";
	m_pNameWavelet[9] = L"db9";
	m_pNameWavelet[10] = L"db10";
	m_pNameWavelet[11] = L"sym1";
	m_pNameWavelet[12] = L"sym2";
	m_pNameWavelet[13] = L"sym3";
	m_pNameWavelet[14] = L"sym4";
	m_pNameWavelet[15] = L"sym5";
	m_pNameWavelet[16] = L"sym6";
	m_pNameWavelet[17] = L"sym7";
	m_pNameWavelet[18] = L"coif1";
	m_pNameWavelet[19] = L"coif2";
	m_pNameWavelet[20] = L"coif3";
	m_pNameWavelet[21] = L"coif4";
	m_pNameWavelet[22] = L"coif5";
}
//------------------------------------------------------------------------------
CCustomFDWT::~CCustomFDWT()
{
	delete [] m_pNameWavelet;
	m_pNameWavelet = nullptr;
}
//------------------------------------------------------------------------------
long CCustomFDWT::Create(
	TypeWavelet typeWavelet,	// тип вейвлета
	int level,					// номер уровня разложения (не менее 1)
	bool needInverse,			// требуется обратное преобразование
	int lengthSignal)			// кол-во точек в сигнале
{
	FbNeedInverse = needInverse;
	
	FLengthSignalClient = lengthSignal;
	long ret = _setTypeWavelet(typeWavelet);
	FLevel = min( max(level, 1), FLevelMaxAll );
	if (lengthSignal != INFINITE)
	{
		_setLengthSignal(lengthSignal);
		_setLevelMaxCur();
		if ( FLevel > FLevelMaxCur )
		{
			FLevel = FLevelMaxCur;
			_setLengthSignal(lengthSignal);
		}
		SetpSize();
	}
	else
	{		
		ippsZero_32s(m_pSize, 32);
		FLevelMaxCur = FLevelMaxAll;
	}
	if ( FLevel != level )
		ret |= wtrvNewLevel;
	if ( FLengthSignal != lengthSignal )
		ret |= wtrvNewLengthSignal;
	return ret;
}
//------------------------------------------------------------------------------
//	Приватные функции
//------------------------------------------------------------------------------
long CCustomFDWT::_setTypeWavelet(TypeWavelet val)
{// Задание нового значения типа материнского вейвлета и, как следствие,
// новой длины фильтра
	FTypeWavelet = val;
	switch (val)
	{// Вейвлет Хаара - установка по умолчанию см. default
	case Haar : FLengthFilter = FilterLen_Haar;
		ippsCopy_32f(pTaps_fwdLo_Haar, pTaps_fwdLo, FLengthFilter);
		ippsCopy_32f(pTaps_fwdHi_Haar, pTaps_fwdHi, FLengthFilter);
		ippsCopy_32f(pTaps_invLo_Haar, pTaps_invLo, FLengthFilter);
		ippsCopy_32f(pTaps_invHi_Haar, pTaps_invHi, FLengthFilter);
		break;
	// Вейвлеты Добеши 1 - 10
	case db1 : FLengthFilter = FilterLen_db1;
		ippsCopy_32f(pTaps_fwdLo_db1, pTaps_fwdLo, FLengthFilter);
		break;
	case db2 : FLengthFilter = FilterLen_db2;
		ippsCopy_32f(pTaps_fwdLo_db2, pTaps_fwdLo, FLengthFilter);
		break;
	case db3 : FLengthFilter = FilterLen_db3;
		ippsCopy_32f(pTaps_fwdLo_db3, pTaps_fwdLo, FLengthFilter);
		break;
	case db4 : FLengthFilter = FilterLen_db4;
		ippsCopy_32f(pTaps_fwdLo_db4, pTaps_fwdLo, FLengthFilter);
		break;
	case db5 : FLengthFilter = FilterLen_db5;
		ippsCopy_32f(pTaps_fwdLo_db5, pTaps_fwdLo, FLengthFilter);
		break;
	case db6 : FLengthFilter = FilterLen_db6;
		ippsCopy_32f(pTaps_fwdLo_db6, pTaps_fwdLo, FLengthFilter);
		break;
	case db7 : FLengthFilter = FilterLen_db7;
		ippsCopy_32f(pTaps_fwdLo_db7, pTaps_fwdLo, FLengthFilter);
		break;
	case db8 : FLengthFilter = FilterLen_db8;
		ippsCopy_32f(pTaps_fwdLo_db8, pTaps_fwdLo, FLengthFilter);
		break;
	case db9 : FLengthFilter = FilterLen_db9;
		ippsCopy_32f(pTaps_fwdLo_db9, pTaps_fwdLo, FLengthFilter);
		break;
	case db10 : FLengthFilter = FilterLen_db10;
		ippsCopy_32f(pTaps_fwdLo_db10, pTaps_fwdLo, FLengthFilter);
		break;
	// Симплеты (вейвлеты ) 1 - 7
	case sym1 : FLengthFilter = FilterLen_sym1;
		ippsCopy_32f(pTaps_fwdLo_sym1, pTaps_fwdLo, FLengthFilter);
		break;
	case sym2 : FLengthFilter = FilterLen_sym2;
		ippsCopy_32f(pTaps_fwdLo_sym2, pTaps_fwdLo, FLengthFilter);
		break;
	case sym3 : FLengthFilter = FilterLen_sym3;
		ippsCopy_32f(pTaps_fwdLo_sym3, pTaps_fwdLo, FLengthFilter);
		break;
	case sym4 : FLengthFilter = FilterLen_sym4;
		ippsCopy_32f(pTaps_fwdLo_sym4, pTaps_fwdLo, FLengthFilter);
		break;
	case sym5 : FLengthFilter = FilterLen_sym5;
		ippsCopy_32f(pTaps_fwdLo_sym5, pTaps_fwdLo, FLengthFilter);
		break;
	case sym6 : FLengthFilter = FilterLen_sym6;
		ippsCopy_32f(pTaps_fwdLo_sym6, pTaps_fwdLo, FLengthFilter);
		break;
	case sym7 : FLengthFilter = FilterLen_sym7;
		ippsCopy_32f(pTaps_fwdLo_sym7, pTaps_fwdLo, FLengthFilter);
		break;
	// Койфлеты (вейвлеты Кофмана) 1 - 5
	case coif1 : FLengthFilter = FilterLen_coif1;
		ippsCopy_32f(pTaps_fwdLo_coif1, pTaps_fwdLo, FLengthFilter);
		break;
	case coif2 : FLengthFilter = FilterLen_coif2;
		ippsCopy_32f(pTaps_fwdLo_coif2, pTaps_fwdLo, FLengthFilter);
		break;
	case coif3 : FLengthFilter = FilterLen_coif3;
		ippsCopy_32f(pTaps_fwdLo_coif3, pTaps_fwdLo, FLengthFilter);
		break;
	case coif4 : FLengthFilter = FilterLen_coif4;
		ippsCopy_32f(pTaps_fwdLo_coif4, pTaps_fwdLo, FLengthFilter);
		break;
	case coif5 : FLengthFilter = FilterLen_coif5;
		ippsCopy_32f(pTaps_fwdLo_coif5, pTaps_fwdLo, FLengthFilter);
		break;
		
	default : FLengthFilter = FilterLen_Haar;	// Хаар
		ippsCopy_32f(pTaps_fwdLo_Haar, pTaps_fwdLo, FLengthFilter);
		ippsCopy_32f(pTaps_fwdHi_Haar, pTaps_fwdHi, FLengthFilter);
		ippsCopy_32f(pTaps_invLo_Haar, pTaps_invLo, FLengthFilter);
		ippsCopy_32f(pTaps_invHi_Haar, pTaps_invHi, FLengthFilter);
		FTypeWavelet = Haar;	// обязательно
	}
	if (FTypeWavelet > 0)	// не для Хаара
	{// определяю коэффициенты фильтров для данного вейвлета
		int i, n = 1;
		int m = FLengthFilter - 1;
		for(i=0; i<FLengthFilter; ++i)
		{
			pTaps_fwdHi[m--] = n * pTaps_fwdLo[i];
			n *= -1;
		}
		m = FLengthFilter - 1;
		for(i=0; i<FLengthFilter; ++i)
		{
			pTaps_invLo[i] = pTaps_fwdLo[m];
			pTaps_invHi[i] = pTaps_fwdHi[m--];
		}
	}
	long ret;
	if (FTypeWavelet == val)
		ret = wtrvNoError;
	else
		ret = wtrvNewTypeWavelet;
	return ret;
}
//------------------------------------------------------------------------------
void CCustomFDWT::_setLengthSignal(int val)
{// задание новой длины сигнала
// Делаю длину сигнала равную минимальному значению, кратному 2^Level,
//	но большему (или равному) val
	int len = 1 << FLevel;
	FLengthSignal = (val / len) * len;
	if ( FLengthSignal != val )
		FLengthSignal += len;
}
//------------------------------------------------------------------------------
void CCustomFDWT::_setLevelMaxCur()
{// Расчет максимально возможного текущего значения максимального уровня
// Исходные данные - длина сигнала
	FLevelMaxCur = 0;
	int len = FLengthSignal;
	while (len > 0)
	{
		FLevelMaxCur++;
		len = len >> 1;
	}
}
//------------------------------------------------------------------------------
void CCustomFDWT::_setLevel(int val)
{
	FLevel = min( max(val, 1), min(FLevelMaxCur, FLevelMaxAll) );
}
//------------------------------------------------------------------------------
long CCustomFDWT::SetTypeWavelet(TypeWavelet val)
{//	Задание свойства TypeWavelet.
//	Переопределение FTypeWavelet и FLengthFilter.
	long ret;
	if ( FTypeWavelet != val )
	{// Вначале установить новое значение, затем побочные свойства в наследниках
		ret = _setTypeWavelet(val);
		NewTypeWavelet();
	}
	else
		ret = 1;
	return ret;
}
//------------------------------------------------------------------------------
void CCustomFDWT::SetLevelMaxCur()
{//FLengthSignal уже должно быть задано
	int oldVal = FLevelMaxCur;
	if (FLengthSignal != INFINITE)
		_setLevelMaxCur();
	else
		FLevelMaxCur = FLevelMaxAll;
	// если значение изменилось --------------------------------
	if ( FLevelMaxCur != oldVal )
	{
		if (FLevel > FLevelMaxCur)
			SetLevel(FLevelMaxCur);
	}
}
//------------------------------------------------------------------------------
void CCustomFDWT::SetLevelMaxAll(int val)
{
	if (val < 1)
		FLevelMaxAll = 1;
	else if (val < 32)
		FLevelMaxAll = val;
	else
		FLevelMaxAll = 31;
}
//------------------------------------------------------------------------------
long CCustomFDWT::SetLengthSignal(int val, bool bNewLevel)
{//		Задание значения свойству LengthSignal (кол-во точек в сигнале).
//		Переопределение FLengthSignal и FLevelMax.
	FLengthSignalClient = val;
	int newVal, len = 1 << FLevel;
	// проверка значения ------------------------------------
	if (val != INFINITE)
		newVal = max(len, val);
	else
		newVal = INFINITE;
	// новое значение ---------------------------------------
	FLengthSignalOld = FLengthSignal;	// старое значение

	if ( (FLengthSignal != newVal) || bNewLevel )
	{// новое значение FLengthSignal
		if (newVal != INFINITE)
			_setLengthSignal(newVal);
		else
			FLengthSignal = INFINITE;
	}
	// если значение изменилось -----------------------------
	if ( FLengthSignal != FLengthSignalOld )
	{// новые параметры: максим. уровень и массив размеров
		SetLevelMaxCur();
		SetpSize();
	}
	// и вот только теперь вызов виртуальной функции для реализации
	// побочных свойств в классах-наследниках
	if (newVal != INFINITE)	// при INFINITE ничего не надо
	if ( (FLengthSignal != FLengthSignalOld) || bNewLevel )
	// если новая длина или новый уровень
		NewLengthSignal();

	// возвращаемое значение --------------------------------
	long ret;
	if ( FLengthSignal != val )
		ret = wtrvNoError;
	else
		ret = wtrvNewLengthSignal;
	return ret;
}
//------------------------------------------------------------------------------
long CCustomFDWT::SetLevel(int val)
{//	Задание значения свойству Level (уровень разложения)
// могло измениться FLevelMax
	// Возвращаемое значение - заданное значение:
	// 1 - установлено, 0 - установлено другое значение
	// проверка значения ---------------------------
	//int newVal = max(1, val);
	// новое значение ------------------------------
	FLevelOld = FLevel;
	_setLevel(val);	//	FLevel = min( max(1, val), FLevelMax );
	// если значение изменилось ----------------
	if ( FLevel != FLevelOld )
	{
		if (FLevel > FLevelOld)
			SetLengthSignal(FLengthSignalClient, true);
		SetpSize();	// после SetLengthSignal
		// И вот только теперь вызов виртуальной функции для реализации	
		NewLevel();	// побочных свойств в классах-наследниках
	}
	// возвращаемое значение ------------------------
	long ret;
	if ( FLevel != val )
		ret = wtrvNoError;
	else
		ret = wtrvNewLevel;
	return ret;
}
//------------------------------------------------------------------------------
void CCustomFDWT::SetpSize()
{// массив m_pSize содержит размеры массивов di.
// m_pSize[0] - размер исходного сигнала
	int size;
	ippsZero_32s(m_pSize, 32);
	if (FLengthSignal != INFINITE)
	{
		size = FLengthSignal;
		for(int i=0; i<=FLevel; ++i)
		{
			m_pSize[i] = size;
			size /= 2;
		}
	}
}
//------------------------------------------------------------------------------
CString CCustomFDWT::NameWavelet(int index)
{
	CString ret;
	if ( ( index >= 0 ) && ( index < FQuantityWavelet ) )
		ret = m_pNameWavelet[index];
	else
		ret.Empty();
	return ret;
}
//------------------------------------------------------------------------------