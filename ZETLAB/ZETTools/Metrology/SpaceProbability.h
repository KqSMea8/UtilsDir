//------------------------------------------------------------------------------
// ������������� ��������
//------------------------------------------------------------------------------
#pragma once
#include <Metrology\Space.h>
#include <Metrology\Probability.h>
//------------------------------------------------------------------------------
#define Num_WorkAbility	20
//------------------------------------------------------------------------------
template <class T>
class CSpaceProbability : public CSpace<T>
{
private:
	T m_factor;				// ��������� ��� ������� �����������

public:
	// �������������� ��������������� - m_stDef > 0 & m_size >= Num_WorkAbility
	bool m_bWorkAbility;
	// ��� ��������������� ��������� - �� ���������� ��� �� ���������
	bool m_bUncertaintyResult;

	UINT m_size;			// ������ ������, �� ������� ����������� �������
	Probability m_probab;	// �����������, M_0_95
	T m_stdDev;				// ���

	CSpaceProbability();
	CSpaceProbability(const CSpaceProbability<T> &sp);
	CSpaceProbability(const T mean, const T stDef, const UINT size = 1);
	virtual ~CSpaceProbability() {}

	T GetFactor() { return m_factor;  }
	void SetFactor(T newVal);

	void Clear();
	void ChangeUncertaintyType();
	bool IsSpaceProbOk();
	void SetMemberSpaceProb(const T mean, const T stdDev, const UINT size = 1);
};
//------------------------------------------------------------------------------
template <class T>
CSpaceProbability<T>::CSpaceProbability() : CSpace<T>()
	, m_bUncertaintyResult(true)
	, m_probab(P_0_95)
	, m_factor((T)1)
	, m_bWorkAbility(false)
	, m_size(0)
	, m_stdDev((T)0)
{}
//------------------------------------------------------------------------------
template <class T>
CSpaceProbability<T>::CSpaceProbability(
	const CSpaceProbability<T> &sp) : CSpace<T>(sp)
{// ����������� �����������
	m_bUncertaintyResult = sp.m_bUncertaintyResult;
	m_probab = sp.m_probab;
	m_size = sp.m_size;
	m_stdDev = sp.m_stdDev;
	m_factor = buf.m_factor;
	m_bWorkAbility = IsSpaceProbOk();
}
//------------------------------------------------------------------------------
template <class T>
CSpaceProbability<T>::CSpaceProbability(
	const T mean, const T stDef, const UINT size) : CSpace<T>()
	, m_bUncertaintyResult(true)
	, m_probab(P_0_95)
	, m_factor((T)1)
{// ����������� � �����������
	SetMemberSpaceProb(mean, stDef, size);
}
//------------------------------------------------------------------------------
template <class T>
void CSpaceProbability<T>::SetFactor(T newVal)
{
	if ((newVal > (T)0) && (m_factor != newVal))
	{
		m_factor = newVal;
		SetMemberSpaceProb(m_mean, m_stdDev, m_size);
	}
}
//------------------------------------------------------------------------------
template <class T>
void CSpaceProbability<T>::Clear()
{// ��������� ���������� ��������� (����� m_probab � m_bUncertaintyResult)
	CSpace::Clear();
	m_bWorkAbility = false;
	m_size = 0;
	m_stdDev = (T)0;
}
//------------------------------------------------------------------------------
template <class T>
void CSpaceProbability<T>::ChangeUncertaintyType()
{// ����� ���� ���������������
	m_bUncertaintyResult = !m_bUncertaintyResult;
	T radius = (int)m_probab * m_stdDev * m_factor;
	if (m_bUncertaintyResult && (m_size > 0))
		radius /= sqrt((T)m_size);
	SetSpaceFromHalf(m_mean, radius);
}
//------------------------------------------------------------------------------
template <class T>
bool CSpaceProbability<T>::IsSpaceProbOk()
{// ��� ������ ���� > 0
	m_bWorkAbility = (m_stdDev > (T)0) && (m_size >= Num_WorkAbility);
	return m_bWorkAbility;
}
//------------------------------------------------------------------------------
template <class T>
void CSpaceProbability<T>::SetMemberSpaceProb(
	const T mean, const T stdDev, const UINT size)
{// ������� ���������� ���������
	T radius = (int)m_probab * stdDev * m_factor;
	if (m_bUncertaintyResult && (size > 0))
		radius /= sqrt((T)size);
	SetSpaceFromHalf(mean, radius);
	m_stdDev = stdDev;
	m_size = size;
	IsSpaceProbOk();
}
//------------------------------------------------------------------------------