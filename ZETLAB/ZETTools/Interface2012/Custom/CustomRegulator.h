//------------------------------------------------------------------------------
// CustomRegulator.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Custom\CustomElement.h"
#include "C:\ZETTools\Interface2012\Custom\Custom_ZETi12.h"
//------------------------------------------------------------------------------
typedef void (CCustom_ZETi12::*NewDoubleEvent)(double);
//------------------------------------------------------------------------------
class CCustomRegulator : public CCustomElement
{
public:
	CCustomRegulator(CCustomContainer *owner);
	virtual ~CCustomRegulator();

private:	
protected:

public:
	NewDoubleEvent m_FunctionEvent;
	void NewDouble(double newVal);
};
//------------------------------------------------------------------------------