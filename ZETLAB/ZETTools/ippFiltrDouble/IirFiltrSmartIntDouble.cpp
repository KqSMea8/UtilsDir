//----------------------------------------------------------------------
// Файл IirFiltrSmartIntDouble.cpp
// Версия от 03.05.2012
// ЗАО "ЭТМС", Зеленоград, Савостин Ю.М.
//----------------------------------------------------------------------
#include "StdAfx.h"
#include <ippFiltrDouble\IirFiltrSmartIntDouble.h>
//----------------------------------------------------------------------
CIirFiltrSmartIntDouble::CIirFiltrSmartIntDouble()
	: bInit(false)
	, pFreqADC(nullptr)
	, FType(tfNo)
{	
	ppFiltrInt = NULL;
	ppFiltrNo = NULL;
	FNumFiltrNo = FNumFiltrInt = 0;
}
//----------------------------------------------------------------------
CIirFiltrSmartIntDouble::~CIirFiltrSmartIntDouble()
{
	DeleteFiltr();
}
//----------------------------------------------------------------------
bool CIirFiltrSmartIntDouble::Create(
	double *pfreqADC)	// указатель на частоту дискретизации АЦП
{
	pFreqADC = pfreqADC;
	return *pFreqADC > 0.;
}
//------------------------------------------------------------------------------
bool CIirFiltrSmartIntDouble::CreateFiltr()
{
	int i;
	bool ret = true;
	ppFiltrNo = new CIirFiltrNoDCDouble* [FNumFiltrNo];
	for(i=0; i<FNumFiltrNo; i++)
	{
		ppFiltrNo[i] = new CIirFiltrNoDCDouble();
		ret = ret && ppFiltrNo[i]->Create(pFreqADC);
		ret = ret && ppFiltrNo[i]->InitFiltr(tfNoDC);
	}

	ppFiltrInt = new CIirFiltrSimpleDouble* [FNumFiltrInt];
	for(i=0; i<FNumFiltrInt; i++)
	{
		ppFiltrInt[i] = new CIirFiltrSimpleDouble();
		ret = ret && ppFiltrInt[i]->Create(pFreqADC);
	}
	if (FNumFiltrInt == 2)
		ret = ret && ppFiltrInt[1]->InitFiltr(tfInt1);

	return ret;
}
//----------------------------------------------------------------------
void CIirFiltrSmartIntDouble::DeleteFiltr()
{
	if (ppFiltrNo)
	{
		for(int i=0; i<FNumFiltrNo; i++)
		{ 
			delete [] ppFiltrNo[i];
			ppFiltrNo[i] = nullptr;
		}
		delete ppFiltrNo;
	}
	if (ppFiltrInt)
	{
		for(int i=0; i<FNumFiltrInt; i++)
		{ 
			delete []ppFiltrInt[i];
			ppFiltrInt[i] = nullptr;
		}
		delete ppFiltrInt;
	}
}
//----------------------------------------------------------------------
bool CIirFiltrSmartIntDouble::InitFiltr(TypeFiltr type)
{	// новый тип фильтра
	double a = *pFreqADC;
	if ( (type == tfInt1NoDC)  || (type == tfInt2NoDC) ||
		 (type == tfInt1gNoDC) || (type == tfInt2gNoDC) ||
		 (type == tfInt1mNoDC) || (type == tfInt2mNoDC) )		
	{// if должен быть именно таким
		FType = type;
		switch (FType)
		{
		case tfInt1NoDC:
			DeleteFiltr();
			FNumFiltrNo = 2;
			FNumFiltrInt = 1;
			bInit = CreateFiltr() &&
					ppFiltrInt[0]->InitFiltr(tfInt1);
			break;
		case tfInt1gNoDC:
			DeleteFiltr();
			FNumFiltrNo = 2;
			FNumFiltrInt = 1;
			bInit = CreateFiltr() &&
					ppFiltrInt[0]->InitFiltr(tfInt1g);
			break;
		case tfInt1mNoDC:
			DeleteFiltr();
			FNumFiltrNo = 2;
			FNumFiltrInt = 1;
			bInit = CreateFiltr() &&
					ppFiltrInt[0]->InitFiltr(tfInt1m);
			break;
		case tfInt2NoDC:
			DeleteFiltr();
			FNumFiltrNo = 3;
			FNumFiltrInt = 2;
			bInit = CreateFiltr() &&
					ppFiltrInt[0]->InitFiltr(tfInt1);
			break;
		case tfInt2gNoDC:
			DeleteFiltr();
			FNumFiltrNo = 3;
			FNumFiltrInt = 2;
			bInit = CreateFiltr() &&
					ppFiltrInt[0]->InitFiltr(tfInt1g);
			break;
		case tfInt2mNoDC:
			DeleteFiltr();
			FNumFiltrNo = 3;
			FNumFiltrInt = 2;
			bInit = CreateFiltr() &&
					ppFiltrInt[0]->InitFiltr(tfInt1m);
			//break;
		}
	}
	else
		bInit = false;
	return bInit;
}
//----------------------------------------------------------------------
bool CIirFiltrSmartIntDouble::Filtration(double *pInpData, double *pOutData, int num)
{
	bool ret;
	if (bInit)
	{
		ret = ppFiltrNo[0]->Filtration(pInpData, pOutData, num);
		ret = ret && ppFiltrInt[0]->Filtration_I(pOutData, num);
		ret = ret && ppFiltrNo[1]->Filtration_I(pOutData, num);
		if (FNumFiltrInt == 2)
		{
			ret = ret && ppFiltrInt[1]->Filtration_I(pOutData, num);
			ret = ret && ppFiltrNo[2]->Filtration_I(pOutData, num);
		}
	}
	else
		ret = false;
	return ret;
}
//----------------------------------------------------------------------
bool CIirFiltrSmartIntDouble::Filtration_I(double *pInpOutData, int num)
{
	bool ret;
	if (bInit)
	{	
		ret = ppFiltrNo[0]->Filtration_I(pInpOutData, num);
		ret = ret && ppFiltrInt[0]->Filtration_I(pInpOutData, num);
		ret = ret && ppFiltrNo[1]->Filtration_I(pInpOutData, num);
		if (FNumFiltrInt == 2)
		{
			ret = ret && ppFiltrInt[1]->Filtration_I(pInpOutData, num);
			ret = ret && ppFiltrNo[2]->Filtration_I(pInpOutData, num);
		}
	}
	else
		ret = false;
	return ret;
}
//----------------------------------------------------------------------
bool CIirFiltrSmartIntDouble::ReStart()
{
	int i;
	bool ret;
	if (bInit)
	{
		ret = true;
		for(i=0; i<FNumFiltrNo; i++)
			ret = ret && ppFiltrNo[i]->ReStart();
		for(i=0; i<FNumFiltrInt; i++)
			ret = ret && ppFiltrInt[i]->ReStart();
	}
	else
		ret = false;
	return ret;
}
//----------------------------------------------------------------------
void CIirFiltrSmartIntDouble::SetFreqMinInHz(double freq)
{
	for (int i = 0; i < FNumFiltrNo; ++i)
		ppFiltrNo[i]->SetFreqMinInHz(freq);
	ReStart();
}
//----------------------------------------------------------------------