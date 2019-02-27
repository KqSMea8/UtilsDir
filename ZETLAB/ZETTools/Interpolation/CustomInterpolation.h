//----------------------------------------------------------------------
// ‘айл CustomInterpolation.h
// ¬ерси€ от 28.03.2013
// «јќ "Ё“ћ—", «еленоград, јнтонов ј.ё.
//----------------------------------------------------------------------
// Ѕазовый класс дл€ классов, реализующих интерпол€цию.
//----------------------------------------------------------------------
#pragma once
//----------------------------------------------------------------------
// ѕеречень типов интерпол€ций, реализуемых какими-либо классами
enum TypeInterpolation
{
	tiNo,		// без интерпол€ции

	tiSpline,	// интерпол€ци€ сплайнами
	tiLinear,	// линейна€ интерпол€ци€
	tiLogarithm,// линейна€ интерпол€ци€ в логарифмическом масштабе
};
//----------------------------------------------------------------------
template <class T>
class CCustomInterpolation
{
protected:
	bool bInit;					// флаг инициализации интерпол€ции
	TypeInterpolation IType;	// тип интерпол€ции

public:
	CCustomInterpolation()
	{
		IType = tiNo;	// без интерпол€ции
		bInit = false;	// интерпол€ци€ не инициализирована
	}
	virtual ~CCustomInterpolation(){};
	
	struct PredicateMore
	{
		PredicateMore(){};
		bool operator()(const T i, const T j){return (i >= j);}
	} predicateMore;

	// методы чтени€/записи свойств -----------------
	TypeInterpolation GetTypeInterpolation() { return IType; }

	// методы класса --------------------------------
	// ¬иртуальна€ функци€ инициализации интерпол€ции.
	// ¬ наследниках необходимо переопредел€ть
	virtual bool InitInterpolation(TypeInterpolation type) = 0;
	
	// ‘ункци€ интерпол€ции
	virtual bool Interpolation(float* pInpXData, T* pInpYData, int iInpSize, float* pOutXData, T* pOutYData, int iOutSize) = 0;
};
//----------------------------------------------------------------------

