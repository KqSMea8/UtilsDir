//----------------------------------------------------------------------
// ���� CustomInterpolation.h
// ������ �� 28.03.2013
// ��� "����", ����������, ������� �.�.
//----------------------------------------------------------------------
// ������� ����� ��� �������, ����������� ������������.
//----------------------------------------------------------------------
#pragma once
//----------------------------------------------------------------------
// �������� ����� ������������, ����������� ������-���� ��������
enum TypeInterpolation
{
	tiNo,		// ��� ������������

	tiSpline,	// ������������ ���������
	tiLinear,	// �������� ������������
	tiLogarithm,// �������� ������������ � ��������������� ��������
};
//----------------------------------------------------------------------
template <class T>
class CCustomInterpolation
{
protected:
	bool bInit;					// ���� ������������� ������������
	TypeInterpolation IType;	// ��� ������������

public:
	CCustomInterpolation()
	{
		IType = tiNo;	// ��� ������������
		bInit = false;	// ������������ �� ����������������
	}
	virtual ~CCustomInterpolation(){};
	
	struct PredicateMore
	{
		PredicateMore(){};
		bool operator()(const T i, const T j){return (i >= j);}
	} predicateMore;

	// ������ ������/������ ������� -----------------
	TypeInterpolation GetTypeInterpolation() { return IType; }

	// ������ ������ --------------------------------
	// ����������� ������� ������������� ������������.
	// � ����������� ���������� ��������������
	virtual bool InitInterpolation(TypeInterpolation type) = 0;
	
	// ������� ������������
	virtual bool Interpolation(float* pInpXData, T* pInpYData, int iInpSize, float* pOutXData, T* pOutYData, int iOutSize) = 0;
};
//----------------------------------------------------------------------

