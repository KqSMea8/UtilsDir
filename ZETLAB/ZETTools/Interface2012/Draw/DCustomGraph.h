//------------------------------------------------------------------------------
// DButtonHMME.h : ���� ��������� (Draw Graph)
//------------------------------------------------------------------------------
// ������� ����� ����������� ��������.
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Draw\CustomDraw.h"
//------------------------------------------------------------------------------
struct ParamCustomGraph : public ParamCustomDraw
{// ��������� ���������� ����������� ��������
	// � ������������ ��������� - ����� ���������, ����� - ������� ���������
	int iBorderHeight;
	COLORREF clrBorderGradientColorStart;
	COLORREF clrBorderGradientColorEnd;
	ParamCustomGraph()
	{
		Size = sizeof(ParamCustomGraph);
		iBorderHeight = 3;
		clrBorderGradientColorStart = RGB(90, 90, 90);
		clrBorderGradientColorEnd = RGB(180, 180, 180);
	}
};
//------------------------------------------------------------------------------
class CDCustomGraph : public CCustomDraw
{
public:
	CDCustomGraph(CRect rect, PVOID ppGraphics,
		bool *pFlagIsChange, double *pTimeAnimation);
	virtual ~CDCustomGraph();

private:
	ParamCustomGraph m_param;// ��������� ���� ���������� �����������

protected:

public:
	// ������� ���������� �����������
	int GetBorderHeight() { return m_param.iBorderHeight; }
	void SetBorderHeight(int iValue);

	COLORREF GetBorderGradientColorStart() { return m_param.clrBorderGradientColorStart; }
	void SetBorderGradientColorStart(COLORREF clrValue);

	COLORREF GetBorderGradientColorEnd() { return m_param.clrBorderGradientColorEnd; }
	void SetBorderGradientColorEnd(COLORREF clrValue);
};
//------------------------------------------------------------------------------