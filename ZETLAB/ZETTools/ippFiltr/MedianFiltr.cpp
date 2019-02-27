//----------------------------------------------------------------------
#include "StdAfx.h"
#include <ippFiltr\MedianFiltr.h>
//----------------------------------------------------------------------
IppsMdaState_32f::IppsMdaState_32f()
	: len(0)
	, len_1(0)
	, IndxMda(0)
	, bEnabledTail(false)
	, pDistIndx(nullptr)
	, pTmpIndx(nullptr)
	, pTail(nullptr)
	, bOdd(false)
{
	pDistIndx = pTmpIndx;
}
//----------------------------------------------------------------------
IppsMdaState_32f::~IppsMdaState_32f()
{
	Delete();
}
//----------------------------------------------------------------------
void IppsMdaState_32f::Delete()
{
	len = 0;
	bEnabledTail = false;
	if (pDistIndx)
	{
		delete [] pDistIndx;
		pDistIndx = nullptr;
	}
	if (pTmpIndx)
	{
		delete [] pTmpIndx;
		pTmpIndx = nullptr;
	}
	if (pTail)
	{
		delete [] pTail;
		pTail = nullptr;
	}
}
//----------------------------------------------------------------------
void IppsMdaState_32f::ReStart()
{
	bEnabledTail = false;
}
//----------------------------------------------------------------------
bool IppsMdaState_32f::Create(int lenght)
{
	bool ret;
	int val;
	if (lenght < 3)
		val = 3;
	else
		val = lenght;

	if (len != val)
	{
		Delete();
		len = val;
		len_1 = len - 1;
		IndxMda = len / 2;
		bOdd = (len % 2);
		if (bOdd == FALSE)
			--IndxMda;
		pDistIndx = new int [len];
		pTmpIndx  = new int [len];
		pTail = new float [len_1];
		ret = (pDistIndx != NULL) && (pTmpIndx != NULL)
			&& (pTail != NULL);
		if (!ret)
			Delete();
	}
	else
	{
		ret = true;
		bEnabledTail = false;
	}
	return ret;
}
//----------------------------------------------------------------------
CMedianFiltr::CMedianFiltr()
	: m_FType(tfMdaThread)	// ��� ����������
	, m_bInit(false)
	, m_pState(nullptr)
	, m_pData(nullptr)
	, m_sizeData(0)
	, m_pFreqADC(nullptr)
{

}
//----------------------------------------------------------------------
CMedianFiltr::~CMedianFiltr()
{
	if (m_pState)
	{ 
		delete m_pState;
		m_pState = nullptr;
	}
	if (m_pData)
	{ 
		delete [] m_pData;
		m_pData = nullptr;
	}
}
//----------------------------------------------------------------------
bool CMedianFiltr::Create(
	float *pfreqADC)	// ��������� �� ������� ������������� ���
{
	m_pFreqADC = pfreqADC;
	return *m_pFreqADC > 0.f;
}
//----------------------------------------------------------------------
BOOL CMedianFiltr::GetOdd()
{
	BOOL ret;
	if (m_pState)
		ret = m_pState->bOdd;
	else
		ret = -1;
	return ret;
}
//----------------------------------------------------------------------
int CMedianFiltr::GetLengthInPoint()
{
	int ret;
	if (m_pState)
		ret = m_pState->len;
	else
		ret = -1;
	return ret;
}
//----------------------------------------------------------------------
float CMedianFiltr::GetLengthInTime()
{
	float ret;
	if (m_pState && m_pFreqADC)
		ret = m_pState->len / *m_pFreqADC;
	else
		ret = 0;
	return ret;
}
//----------------------------------------------------------------------
bool CMedianFiltr::_InitFiltr(int lenght)
{// ������������� ��������� �������
	bool ret;
	if (m_pState == NULL)
		m_pState = new IppsMdaState_32f();	
	ret = m_pState->Create(lenght);
	int size8u(0);
	m_bInit = 
		(ippStsNoErr == ippsSortRadixIndexGetBufferSize(lenght, ipp32f, &size8u)) &&
		m_sa_8u.ReSize(size8u);
	return ret;
}
//----------------------------------------------------------------------
bool CMedianFiltr::InitFiltrInPoint(int lenght)
{
	return _InitFiltr(lenght);
}
//----------------------------------------------------------------------
bool CMedianFiltr::InitFiltrInTime(float lenTime)
{
	bool ret;
	if (m_pFreqADC)
	{
		float t = *m_pFreqADC * lenTime;
		int len = int(t);
		//if ( (len % 2) == 0)
		//	++len;
		if (len < 3)
			len = 3;
		ret = _InitFiltr(len);
	}
	else
		ret = false;
	return ret;
}
//----------------------------------------------------------------------
bool CMedianFiltr::Filtration(
	float *pInpData,	// ��������� �� �������� ������
	float *pOutData,	// ��������� �� ��������������� ������
	int num)			// ���-�� ��������
{
	return _Filtration(pInpData, pOutData, num);
}
//----------------------------------------------------------------------
bool CMedianFiltr::Filtration_I(
	float *pInpOutData,	// ��������� �� ��������/�������� ������
	int num)			// ���-�� ��������
{
	return _Filtration(pInpOutData, pInpOutData, num);
}
//----------------------------------------------------------------------
bool CMedianFiltr::ReStart()
{// ����������������� ��������� ������� � �������� �����������.
// ��������� ��� ��������� ������� ������������� ��� ���
// ��� ����������� ���������. ��� tfUser ������� ������� SetUserTaps().
	bool ret;
	if (m_bInit)
	{// ��������� ��� ������
		m_pState->ReStart();
		ret = true;
	}
	else	// ��������� ��� �� ������
		ret = false;
	return ret;
}
//----------------------------------------------------------------------
bool CMedianFiltr::_Filtration(
	float *pInpData,	// ��������� �� �������� ������
	float *pOutData,	// ��������� �� ��������������� ������
	int num)			// ���-�� ��������
{// ������� ���������� �� ��� ������ �����
	static int sizeFloat = sizeof(float);
	bool ret;
	if (m_bInit)
	{	// �������� ������� ��������
		if (m_pState->bEnabledTail)
			ret = true;
		else
		{// ������ ����� (�������� ���)
			ret = ippsSet_32f(*pInpData, m_pState->pTail, m_pState->len_1) == ippStsNoErr;
			m_pState->bEnabledTail = true;
		}
		// �������� ������ ��������� ������
		if (ret)
		{
			int size = num + m_pState->len_1;
			if (m_sizeData < size)
			{// ������ ������ ������
				if (m_pData)
				{ 
					delete [] m_pData;
					m_pData = nullptr;
				}
				m_pData = new float [size];
				m_sizeData = size;
			}
			if (ret)
			{	// � ��������� ������ ������� ������� �������
				ret =  (ippsCopy_32f(m_pState->pTail, m_pData, m_pState->len_1) == ippStsNoErr)
					// ����� ���� �������� ������
					&& (ippsCopy_32f(pInpData, m_pData + m_pState->len_1, num) == ippStsNoErr)
					// ����� �������� ������� (�� ��� ������, ���� ���� Filtration_I)
					// ������ ������� �� (m_pData+num), �.�. num ����� == 1!!!
					&& (ippsCopy_32f(m_pData + num, m_pState->pTail, m_pState->len_1) == ippStsNoErr);
			
				if (ret)
				{// � ��������� ������� � �����
					float *addInp = m_pData;
					float *addOut = pOutData;



					for(int i=0; i<num; ++i, ++addInp, ++addOut)
					{// � ������� ������ �������� ������
						ret = ippStsNoErr ==
							ippsSortRadixIndexAscend_32f(addInp, sizeFloat, m_pState->pDistIndx, m_pState->len, m_sa_8u.pData);
// 						ret = (ippsSortRadixIndexAscend_32f(addInp, sizeFloat,
// 							m_pState->pDistIndx, m_pState->pTmpIndx, m_pState->len) == ippStsNoErr);
						if (ret)
						{// � �������� ������ ������ ������� �����
							if (m_pState->bOdd)
								*addOut = addInp[ m_pState->pDistIndx[m_pState->IndxMda] ];
							else
								*addOut = (addInp[ m_pState->pDistIndx[m_pState->IndxMda] ] +
											addInp[ m_pState->pDistIndx[m_pState->IndxMda+1] ]) * 0.5f;
						}
						else
							break;
					}
				}
			}
		}
	}
	else
		ret = false;
	return ret;
}
//----------------------------------------------------------------------