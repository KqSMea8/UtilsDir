//------------------------------------------------------------------------------
//	���� RotateSystem_2D.cpp
//	������ �� 27.11.2012
//	����������, ��� "����", �������� �.�.
//------------------------------------------------------------------------------
#include "afx.h"
#include "StdAfx.h"
#include <FunctionSeismic\RotateSystem_2D.h>
//------------------------------------------------------------------------------
CRotateSystem_2D::CRotateSystem_2D()
{
	m_BearingY = 0;
	m_cos = 1;
	m_sin = 0;
	m_bNotZero = false;
}
//------------------------------------------------------------------------------
//CRotateSystem_2D::~CRotateSystem_2D()
//{
//}
//------------------------------------------------------------------------------
bool CRotateSystem_2D::SetBearingY(float val)
{	// ���� �������� ���� - ��� � ���� ������ ��� Y
	bool ret;
	if ( (0.f <= val) && (val < 360.f) )
	{
		if (m_BearingY != val)
		{
			m_BearingY = val;
			m_cos = cos(m_BearingY);
			m_sin = sin(m_BearingY);
			m_bNotZero = m_BearingY != 0.f;
		}
		ret = true;	// ������ m_BearingY = val ���� ������
	} 
	else
		ret = false;
	return ret;
}
//------------------------------------------------------------------------------
// ������� ��������� �������������� �������� ��� �������� ���� ���������
// �� X,Y � E,N (East,North, �.�. ������ �� ������, ����� �� ���������)
bool CRotateSystem_2D::Rotate_32f_I(float *pX, float *pY, long num)
{
	bool ret = true;
	if (m_bNotZero)
	{// ���� ������ = 0, �� ������ ������ �� ����
		if ( (pX != NULL) && (pY != NULL) && (num > 0) )
		{
			float *_px = pX;
			float *_py = pY;
			for (long i=0; i<num; ++i)
			{
				float x = *_px;
				float y = *_py;
				*_px++ =  x * m_cos + y * m_sin;
				*_py++ = -x * m_sin + y * m_cos;
			}
		}
		else
			ret = false;
	}
	return ret;
}
//------------------------------------------------------------------------------