//------------------------------------------------------------------------------
//	���� BufferForDataWithTime.h
//	����������, ��� "����", �������� �.�.
//------------------------------------------------------------------------------
/*		��������� ����� ��������� ����� ��� �������� � ������ ������,
������� �������� � �����		*/
//------------------------------------------------------------------------------
#pragma once
#include "afx.h"
#include <list>
#include <mathZet.h>
#include <CodeQuality.h>
#include <WideResult\WideResult.h>
//------------------------------------------------------------------------------
template <class T>
class CBufferForDataWithTime
{
private:
	bool m_bInit;
	double m_dt;
	DWORD m_sizeMax;
	std::list<WideResult<T>> m_list;
	WideResult<T> m_wr_SkipData;

	void AddOneNoTest(WideResult<T> &newWR);
	void AddOneWithTest(WideResult<T> &newWR);

protected:
public:
	CBufferForDataWithTime();
	CBufferForDataWithTime(const CBufferForDataWithTime &val);
	virtual ~CBufferForDataWithTime();
	// size - ������ ������, frequency - ������� ������������� ������
	bool Create(DWORD size, T frequency);

	// ������ ������/������ ������� -----------------
	DWORD GetSize() { return m_sizeMax; }
	double CurrentTime();
	double GetFirstTime();

	// ������ ������ ----------------------------
	bool AddOneData(double time, T val, long quality);
	bool AddData(double time, int number, T *pData, long quality);
	long GetData(double time, int number, T *pData, long *pQuality = NULL);
};
//------------------------------------------------------------------------------
template <class T>
CBufferForDataWithTime<T>::CBufferForDataWithTime()
	: m_bInit(false)
	, m_sizeMax(0)
	, m_dt(0.)
{
	CodeQuality skipData;
	skipData.Synchro = 1;
	m_wr_SkipData.quality = skipData;
	m_wr_SkipData.value = std::numeric_limits<T>::quiet_NaN();
}
//------------------------------------------------------------------------------
template <class T>
CBufferForDataWithTime<T>::~CBufferForDataWithTime()
{
	if (m_sizeMax > 0)
	{
		m_list.clear();
	}
}
//------------------------------------------------------------------------------
template <class T>
bool CBufferForDataWithTime<T>::Create(
	DWORD size,			// ������ ������ ��� �������� ��������
	T frequency)	// ������� ������������� �������� ��������
{
	m_bInit = (size > 0) && (frequency > (T)0);
	if (m_bInit && (m_sizeMax < size))
	{
		m_dt = 1.f / frequency;
		try
		{
			m_sizeMax = size;
			m_list.resize(m_sizeMax);
		}
		catch (CMemoryException*)
		{
			m_sizeMax = 0;
			m_list.clear();
			m_bInit = false;
		}
	}
	return m_bInit;
}
//------------------------------------------------------------------------------
template <class T>
// ��������� ������� ���������� ������ ������ � ����� ������
void CBufferForDataWithTime<T>::AddOneNoTest(WideResult<T> &newWR)
{// ������� �������, ����� ��������, ����� resize �� ���������
	if (m_list.size() == m_sizeMax)
		m_list.pop_front();
	m_list.push_back(newWR);
}
//------------------------------------------------------------------------------
template <class T>
// ��������� ������� ���������� ������ ������ � �����. ����� � ������
// ������������ � ������������ �� �������� �������.
void CBufferForDataWithTime<T>::AddOneWithTest(WideResult<T> &newWR)
{
	double lastTime = m_list.back().time;
	// �������� �����
	double timeApproximate = lastTime + m_dt;
	if (IsEqual(timeApproximate, newWR.time, 0.001))
	{// ������� ��������
		AddOneNoTest(newWR);
	}
	else
	{/* �� ������� ��������, �������� 3 ��������:
	 1. ����� ������� ������ �������, ���� �������� NaN-�;
	 2. � ������ ��� ���� ����� �����, ���� �������� ������;
	 3. ����� ������� ������ ������, ����� ��� ������ �� �����		*/
		if (newWR.time > timeApproximate)
		{// ������� 1.
			WideResult<T> temp = m_wr_SkipData;
			temp.time = timeApproximate;
			int n = (int)floor((newWR.time - lastTime) / m_dt + 0.5) - 1;
			for (int i = 0; i < n; ++i)
			{
				AddOneNoTest(temp);	// �������� NaN � ��������� ��������
				temp.time += m_dt;
			}
			AddOneNoTest(newWR);
		}
		else
		{
			if (newWR.time >= m_list.front().time)
			{// ������� 2.
				int n = (int)floor((lastTime - newWR.time) / m_dt + 0.5);
				// �����������, ��� newWR.time ����� � GetLastTime, ����,
				// ������� ����� � �����
				auto it = m_list.end(), ie = m_list.begin();
				--it;
				for (int i = 0; i < n; ++i, --it)
				{
					if (it != ie)
						--it;
					else
						break;
				}
				it->value = newWR.value;
				it->quality = newWR.quality;
			}
			// else ������� 3. - ������ ������ �� ����
		}
	}
}
//------------------------------------------------------------------------------
template <class T>
double CBufferForDataWithTime<T>::CurrentTime()
{
	return (m_list.size() > 0) ? m_list.back().time : 0.;
}
//------------------------------------------------------------------------------
template <class T>
double CBufferForDataWithTime<T>::GetFirstTime()
{
	return (m_list.size() > 0) ? m_list.front().time : 0.;
}
//------------------------------------------------------------------------------
template <class T>
// ��������� ������� ���������� � ����� ������ �������
bool CBufferForDataWithTime<T>:: AddOneData(double time, T val, long quality)
{
	static WideResult<T> temp;
	bool ret = m_bInit;
	if (ret)
	{
		temp.quality = quality;
		temp.time = time;
		temp.value = val;
		AddOneWithTest(temp);
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
// ��������� ������� ���������� � ����� ������� ��������
bool CBufferForDataWithTime<T>::AddData(
	double time,	// ����� ���������� �������
	int number,		// ������ �������
	T *pData,		// ������
	long quality)	// �������� ������
{
	static WideResult<T> temp;
	bool ret = m_bInit && (number > 0);
	if (ret)
	{
		temp.quality = quality;
		int n_1 = number - 1;
		int i = 0;
		// ������ ����� ��� ���������
		temp.time = time - m_dt * n_1;
		for (; i < n_1; ++i)
		{
			temp.value = pData[i];
			AddOneWithTest(temp);
			temp.time += m_dt;
		}
		// ��������� �����
		temp.time = time;
		temp.value = pData[i];
		AddOneWithTest(temp);
	}
	return ret;
}
//------------------------------------------------------------------------------
template <class T>
// ������ �� ������ ������� ������
long CBufferForDataWithTime<T>::GetData(
	double time,	// ����� ���������� �������
	int number,		// ������ �������
	T *pData,		// ������
	long *pQuality)	// �������� ������
{// ������������� ���� �������� ������ ��� � ���������� ������� ZetServer
	long ret(0);
	if (m_bInit)
	{
		if (pData == NULL)
			ret = -13;
		if (ret == 0)
		{
			if ((0 <= number) || (number > (int)m_sizeMax))
				ret = -8;
			if (ret == 0)
			{
				if (time < 0.)
					ret = -12;
				if (ret == 0)
				{
					if (time < (m_list.front().time + number * m_dt))
						ret = -14;
					if (ret == 0)
					{
						if (time > m_list.back().time)
							ret = -11;
						if (ret == 0)
						{// ������ ����, ��� �� � ���������
							auto it = m_list.cend(), ie = m_list.cbegin();
							--it;
							while (true)
							{// ���� ��������� ������ � ������
								if ((time <= it->time) || (it == ie))
									break;
								else
									--it;
							}
							if (pQuality)
								*pQuality = 0;
							T *padd = pData + number;
							for (int i = number - 1; i <= 0; --i, --padd)
							{
								if (it != ie)
								{
									*padd = it->value;
									if (pQuality)
										*pQuality |= it->quality;
									--it;
								}
								else
								{
									*padd = std::numeric_limits<T>::quiet_NaN();
								}
							}
						}
					}
				}
			}
		}
	}
	else ret = -1;	// ��� ������ � �������
	return ret;
}
//------------------------------------------------------------------------------