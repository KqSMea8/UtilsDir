//------------------------------------------------------------------------------
// CustomIndicator.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include "Interface2012\Custom\CustomElement.h"
//#include "NaNsupport.h"
//------------------------------------------------------------------------------
class CCustomIndicator : public CCustomElement
{
public:
	CCustomIndicator(CCustomContainer *owner);
	virtual ~CCustomIndicator();

private:	
protected:

public:
	virtual void SetAmplitude(double val) {}
	virtual double GetAmplitude() { return 0; }
};
//------------------------------------------------------------------------------