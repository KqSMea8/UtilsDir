//------------------------------------------------------------------------------
// CustomSlider.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Custom\CustomElement.h"
#include "C:\ZETTools\Interface2012\Custom\Custom_ZETi12.h"
//------------------------------------------------------------------------------
typedef void (CCustom_ZETi12::*NewDoubleEvent)(double);
//------------------------------------------------------------------------------
class CCustomSlider : public CCustomElement
{
public:
	CCustomSlider(CCustomContainer *owner);
	virtual ~CCustomSlider();

private:	
protected:

public:
	NewDoubleEvent m_FunctionEvent;
	void NewDouble(double newVal);
};
//------------------------------------------------------------------------------