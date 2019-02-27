//----------------------------------------------------------------------
// Файл IirFiltrSmartInt.cpp
// Версия от 26.02.2012
// ЗАО "ЭТМС", Зеленоград, Савостин Ю.М.
//----------------------------------------------------------------------
#include "StdAfx.h"
#include <ippFiltr\IirFiltrSmartInt.h>
//----------------------------------------------------------------------
CIirFiltrSmartInt::CIirFiltrSmartInt()
	: ppFiltrInt(NULL)
	, ppFiltrNo(NULL)
	, bInit(false)
	, FType(tfNo)
	, pFreqADC(nullptr)
{	

	FNumFiltrNo = FNumFiltrInt = 0;
}
//----------------------------------------------------------------------
CIirFiltrSmartInt::~CIirFiltrSmartInt()
{
	DeleteFiltr();
}
//----------------------------------------------------------------------
bool CIirFiltrSmartInt::Create(
	float *pfreqADC)	// указатель на частоту дискретизации АЦП
{
	pFreqADC = pfreqADC;
	return *pFreqADC > 0.f;
}
//----------------------------------------------------------------------
bool CIirFiltrSmartInt::CreateFiltr()
{
	int i;
	bool ret = true;
	ppFiltrNo = new CIirFiltrNoDC* [FNumFiltrNo];
	for(i=0; i<FNumFiltrNo; i++)
	{
		ppFiltrNo[i] = new CIirFiltrNoDC();
		ret = ret && ppFiltrNo[i]->Create(pFreqADC);
		ret = ret && ppFiltrNo[i]->InitFiltr(tfNoDC);
	}

	ppFiltrInt = new CIirFiltrSimple* [FNumFiltrInt];
	for(i=0; i<FNumFiltrInt; i++)
	{
		ppFiltrInt[i] = new CIirFiltrSimple();
		ret = ret && ppFiltrInt[i]->Create(pFreqADC);
	}
	if (FNumFiltrInt == 2)
		ret = ret && ppFiltrInt[1]->InitFiltr(tfInt1);

	return ret;
}
//----------------------------------------------------------------------
void CIirFiltrSmartInt::DeleteFiltr()
{
	if (ppFiltrNo)
	{
		for(int i=0; i<FNumFiltrNo; i++)
		{
			delete [] ppFiltrNo[i];
			ppFiltrNo[i] = nullptr;
		}
	}
	if (ppFiltrInt)
	{
		for(int i=0; i<FNumFiltrInt; i++)
		{ 
			delete [] ppFiltrInt[i];
			ppFiltrInt[i] = nullptr;
		}
	}
}
//----------------------------------------------------------------------
bool CIirFiltrSmartInt::InitFiltr(TypeFiltr type)
{	// новый тип фильтра
	float a = *pFreqADC;
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
bool CIirFiltrSmartInt::Filtration(float *pInpData, float *pOutData, int num)
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
bool CIirFiltrSmartInt::Filtration_I(float *pInpOutData, int num)
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
bool CIirFiltrSmartInt::ReStart()
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