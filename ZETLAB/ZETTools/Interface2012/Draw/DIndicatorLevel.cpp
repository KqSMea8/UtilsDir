//------------------------------------------------------------------------------
// DIndicatorLevel.cpp : файл реализации
// Прямоугольник элемента в процессе работы не меняется, поэтому положение
// (горизонтальное или вертикальное) определяется один раз в конструкторе.
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "C:\ZETTools\Interface2012\Draw\DIndicatorLevel.h"

//------------------------------------------------------------------------------
int g_nIndicatorLevel = 0;		// счётчик статических картинок
//------------------------------------------------------------------------------
CDIndicatorLevel::CDIndicatorLevel(CRect rect, PVOID ppGraphics,
				bool *pFlagIsChange, double *pTimeAnimation) :
				CCustomDraw(rect, ppGraphics, pFlagIsChange, pTimeAnimation)
{
	m_pParamDraw = &m_param;	// обеспечение ОБЩИХ параметров
	m_param.nElement = ++g_nIndicatorLevel;
	SetNameWithNumber(L"IndicatorLevel");
	if ( m_rect.Width() > m_rect.Height() )
		m_param.PositionType = tpHorizontal;
	else
		m_param.PositionType = tpVertical;
	// переменные класса
	DefineRectAll();
	m_pImage = m_pImage_s = m_pImage_d = m_pImage_p = NULL;
	m_bDanger = false;
}
//------------------------------------------------------------------------------
CDIndicatorLevel::~CDIndicatorLevel()
{
	DeleteGdiPlusObject(m_pImage_s);
	DeleteGdiPlusObject(m_pImage_d);
	DeleteGdiPlusObject(m_pImage_p);
}
//------------------------------------------------------------------------------
void CDIndicatorLevel::DefineRectAll()
{
	m_rect1 = m_rectT = m_rect;
	if (m_param.PositionType == tpHorizontal)
	{
		m_rectT.left = m_rect.right - (m_rect.bottom - m_rect.top);
		m_rect1.right = m_rectT.left - m_param.Distance;
		m_SizeRectT = m_rectT.right - m_rectT.left;
	}
	else
	{
		m_rectT.bottom = m_rect.top + (m_rect.right - m_rect.left);
		m_rect1.top = m_rectT.bottom + m_param.Distance;
		m_SizeRectT = m_rectT.bottom - m_rectT.top;
	}
	DefineRect2(&m_rect1, &m_rect2, m_param.BorderWidth);
	if (m_param.PositionType == tpHorizontal)
		m_LengthRect2 = m_rect2.Width();
	else
		m_LengthRect2 = m_rect2.Height();
}
//------------------------------------------------------------------------------
void CDIndicatorLevel::SetAmplitude(float val)
{
	float newVal;
	if (val < 0.f)
		newVal = 0.f;
	else if (val > 1.f)
		newVal = 1.f;
	else
		newVal = val;
	if (m_param.Amplitude != newVal)
	{
		m_param.Amplitude = newVal;		
		if (newVal > m_param.ThresholdDanger)
		{			
			m_pImage = m_pImage_d;
			m_bDanger = true;
		}

		if (m_param.LinLog)
		{// логарифмическая шкала
			if (newVal < 0.000001f)
				newVal = 0.000001f;
			newVal = 1.f + 0.25f * log10(newVal);
		}
		if (newVal < 0.1f)
			newVal = 0.1f;
		
		int len = int(m_LengthRect2 * newVal);
		if (m_param.PositionType == tpHorizontal)
			m_rect2.right = m_rect2.left + len;
		else
			m_rect2.top = m_rect2.bottom - len;
		newVal *= 5;
		switch ( int(newVal) )
		{
		case 0:
			m_ColorCur = int(sqrt(newVal)*255.9f) * 0x10000;
			break;
		case 1:
			m_ColorCur = int(sqrt(newVal-1)*255.9f) * 0x100 + 0xff0000;
			break;
		case 2:
			m_ColorCur = int(sqrt(3-newVal)*255.9f) * 0x10000 + 0xff00;
			break;
		case 3:
			m_ColorCur = int(sqrt(newVal-3)*255.9f) + 0xff00;
			break;
		case 4:
			m_ColorCur = int(sqrt(5-newVal)*255.9f) * 0x100 + 0xff;
			break;
		case 5:
			m_ColorCur = 0xff;
		}
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDIndicatorLevel::SetThresholdDanger(float val)
{
	if ( (val > 0.8f) && (val <= 1.f) && (m_param.ThresholdDanger != val) )
	{
		m_param.ThresholdDanger = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDIndicatorLevel::SetLinLog(long val)
{
	long newVal;
	if (val != 0)
		newVal = 1;
	else
		newVal = 0;
	if (m_param.LinLog != newVal)
	{
		m_param.LinLog = newVal;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDIndicatorLevel::SetDistance(int val)
{
	int newVal;
	if (val < 2)
		newVal = 2;
	else
		newVal = val;
	if (m_param.Distance != newVal)
	{
		m_param.Distance = newVal;
		DefineRectAll();
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDIndicatorLevel::SetBorderWidth(int val)
{

	int newVal;
	if (val < 2)
		newVal = 2;
	else
		newVal = val;
	if (m_param.BorderWidth != newVal)
	{
		m_param.BorderWidth = newVal;
		DefineRect2(&m_rect1, &m_rect2, m_param.BorderWidth);
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDIndicatorLevel::SetBorderColor(COLORREF val)
{
	if (m_param.BorderColor != val)
	{
		m_param.BorderColor = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDIndicatorLevel::SetBackColor(COLORREF val)
{
	if (m_param.BackColor != val)
	{
		m_param.BackColor = val;
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDIndicatorLevel::SetFileNameTriangle(CString str)
{
//	if (m_param.FileName != str)
//	{
//		m_param.FileName = str;
//		LoadImageFromFile(&m_pImage_s, str + L"_s");
//		LoadImageFromFile(&m_pImage_d, str + L"_d");
//		LoadImageFromFile(&m_pImage_p, str + L"_p");
//		m_pImage = m_pImage_s;
//		IsChange();
//	}
}
//------------------------------------------------------------------------------
void CDIndicatorLevel::SetTimeAnimation(double val)
{
	if (val > 0.)
		m_param.dTimeAnimation = val;
}
//------------------------------------------------------------------------------
void CDIndicatorLevel::DoProcess()
{
	if (m_param.bAnimation)
	{
		if ( (GetTime() - m_dTimeStart) > m_param.dTimeAnimation )
		{
			m_param.bAnimation = false;
			if (m_param.Amplitude <= m_param.ThresholdDanger)
				m_pImage = m_pImage_s;
			else
			{
				m_bDanger = true;
				m_pImage = m_pImage_d;
			}
		}
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDIndicatorLevel::OnLButtonClk(UINT nFlags, CPoint point)
{
	if ( m_bDanger && PointInRect(point, &m_rectT) )
	{
		m_bDanger = false;
		m_pImage = m_pImage_p;
		m_param.bAnimation = true;
		m_dTimeStart = GetTime();
		IsChange();
	}
}
//------------------------------------------------------------------------------
void CDIndicatorLevel::OnDraw()
{// отображение
	if (m_param.bVisible)
	{
		Graphics *pGraphics = *((Graphics**)m_ppGraphics);
		// граница индикатора
		FillRectSolid(pGraphics, &m_rect1, m_param.BorderColor);
		// цвет индикатора
		FillRectSolid(pGraphics, &m_rect2, m_ColorCur);
		// треугольник
		pGraphics->DrawImage( m_pImage, m_rectT.left, m_rectT.top,
								m_SizeRectT, m_SizeRectT );
	}
}
//------------------------------------------------------------------------------