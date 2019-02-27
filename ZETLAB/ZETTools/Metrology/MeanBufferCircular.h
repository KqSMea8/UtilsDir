//--------------------------------------------------------------------------
//	����������� ����� ��� ������� �������� � ���.
// ������������ � ��� - ������� ��������.
//--------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <list>
#include <Metrology\SpaceProbability.h>
//--------------------------------------------------------------------------
template <class T>		// � - ��� ����� ����� (float ��� double)
class CMeanBufferCircular : public CSpaceProbability<T>
{
private:
	bool m_bFull;		// ����� �����
	bool m_bChange;		// ����� ���������� ������� �������� ��������� ������
	UINT m_sizeBuf;		// ������ ������
	UINT m_counter;		// ������� ������
	double m_summ;		// ����� ����� � ������				// �����������
	double m_summ2;		// ����� ��������� ����� � ������	// double !!!
	std::list<T> m_list;// ����� �������� ������ � ������

protected:

public:
	bool m_bStatus;		// ������ ������
	UINT m_counterMax;

	CMeanBufferCircular();
	CMeanBufferCircular(const CMeanBufferCircular<T> &buf);
	CMeanBufferCircular(const UINT sizeBuf);
	virtual ~CMeanBufferCircular() {}

	// ������� �������
	UINT GetSizeBuf() { return m_sizeBuf; }
	bool SetSizeBuf(const UINT size, const UINT counterMax = 0);

	// ������� ������
	void Clear();
	bool IsFull() { return m_bFull; }
	UINT FillingBuf() { return m_list.size(); }
	bool MeanCalc();
	void RemovalLastData(const UINT count);
	void Write(const T val, bool *pCounterFull = NULL);
};
//--------------------------------------------------------------------------
template <class T>
CMeanBufferCircular<T>::CMeanBufferCircular() : CSpaceProbability<T>()
, m_bFull(false)
, m_bChange(false)
, m_bStatus(true)
, m_sizeBuf(0)
, m_counter(0)
, m_counterMax(0)
, m_summ(0.)
, m_summ2(0.)
{
}
//--------------------------------------------------------------------------
template <class T>
CMeanBufferCircular<T>::CMeanBufferCircular(
	const CMeanBufferCircular<T> &buf) : CSpaceProbability<T>(buf)
{
	m_bFull	  = buf.m_bFull;
	m_bChange = buf.m_bChange;
	m_bStatus = buf.m_bStatus;
	m_sizeBuf = buf.m_sizeBuf;
	m_counter = buf.m_counter;
	m_counterMax = buf.m_counterMax;
	m_summ    = buf.m_summ;
	m_summ2   = buf.m_summ2;
	m_list    = buf.m_list;
}
//--------------------------------------------------------------------------
template <class T>
CMeanBufferCircular<T>::CMeanBufferCircular(const UINT sizeBuf)
	: CSpaceProbability<T>()
, m_bFull(false)
, m_bChange(false)
, m_bStatus(true)
, m_sizeBuf(0)
, m_counter(0)
, m_counterMax(0)
, m_summ(0.)
, m_summ2(0.)
{
	SetSizeBuf(sizeBuf);
}
//--------------------------------------------------------------------------
template <class T>
bool CMeanBufferCircular<T>::SetSizeBuf(const UINT size, const UINT counterMax)
{// ��������� ������� ������
	bool ret = size > 0;
	if (ret)
	{
		m_sizeBuf = size;
		Clear();
		if (counterMax > 0)
			m_counterMax = counterMax;
		else
			m_counterMax = size;
	}
	return ret;
}
//--------------------------------------------------------------------------
template <class T>
void CMeanBufferCircular<T>::Clear()
{// ������� ����������� ������
	CSpaceProbability::Clear();
	m_list.clear();
	m_bFull = m_bChange = false;
	m_counter = 0;
	m_summ = m_summ2 = 0.;
}
//--------------------------------------------------------------------------
template <class T>
bool CMeanBufferCircular<T>::MeanCalc()
{// ������ �������� � ���
	bool ret = !m_list.empty();
	if (ret)
	{
		if (m_bChange)
		{
			ret = m_size > 0;
			if (ret)
			{
				double _stdDev;
				double _mean = m_summ / m_size;
				if (m_size > 1)
				{
					_stdDev = (m_summ2 - m_size * _mean * _mean);
					if (_stdDev > 0.)
						_stdDev = sqrt(_stdDev / (m_size - 1));
					else
						_stdDev = 0;
				}
				else
				{
					_stdDev = 0;
				}
				m_mean  = (T)_mean;
				m_stdDev = (T)_stdDev;

				m_bChange = false;
				SetMemberSpaceProb(m_mean, m_stdDev, m_size);
				//IsSpaceProbOk();
				//m_radius = (int)m_probab * m_stDef * m_factor;
				//if (m_bUncertaintyResult && (m_size > 0))
				//	m_radius /= sqrt((T)m_size);
				//m_left  = m_mean - m_radius;
				//m_right = m_mean + m_radius;
			}
		}
	}
	return ret;
}
//--------------------------------------------------------------------------
template <class T>
void CMeanBufferCircular<T>:: RemovalLastData(const UINT count)
{// ��������� �� ������ ��������� ������.
	if ( !m_list.empty() )
	{
		int rest = (int)m_list.size() - (int)count;	// �������
		if (rest > 0)
		{// ������ ��������� ������
			auto it = m_list.rbegin();
			for (UINT i=0; i<count; ++i, ++it)
			{
				double x = (double)*it;
				m_summ  -= x;
				m_summ2 -= x * x;
			}
			m_size = rest;		// m_sizeBuf �������� ������
			m_list.resize(rest);
			m_bChange = true;
			m_bFull = false;
		}
		else
			Clear();
	}
}
//--------------------------------------------------------------------------
template <class T>
void CMeanBufferCircular<T>::Write(const T val, bool *pCounterFull)
{// ������ � ����� ������ ��������
	double x = (double)val;
	m_summ  += x;
	m_summ2 += x * x;
	m_list.push_back(val);
	m_bChange = true;
	if (m_bFull)
	{
		x = (double)m_list.front();
		m_summ  -= x;
		m_summ2 -= x * x;
		m_list.pop_front();
	}
	else
		m_bFull = m_sizeBuf == ++m_size;
	// ������ �� ���������
	if (pCounterFull != NULL)
		*pCounterFull = false;
	if (m_bStatus && (++m_counter == m_counterMax))
	{
		m_counter = 0;
		if (pCounterFull != NULL)
			*pCounterFull = true;
	}
}
//--------------------------------------------------------------------------