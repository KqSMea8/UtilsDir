//------------------------------------------------------------------------------
// DIndicatorLevel.h : ���� ���������
//------------------------------------------------------------------------------
// ����� ����������� ���������� ������
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Draw\CustomDraw.h"
//------------------------------------------------------------------------------
struct ParamIndicatorLevel : public ParamCustomDraw
{// ��������� ���������� ����������� ��������� ������
// � ������������ ��������� - ����� ���������, ����� - ������� ���������
	TypePosition PositionType;	// ��� ��������� (�����. ��� ������.)
	long LinLog;				// �������� - 0, ��������������� - 1
	float Amplitude;			// �������� �� 0 �� 1
	float ThresholdDanger;		// ����� ���������
	int Distance;				// ��������� �/� ����������� � �������������
	int BorderWidth;			// ������ �������
	COLORREF BorderColor;		// ���� �������
	COLORREF BackColor;			// ���� ����
	CString FileName;			// ������ ��� ����� � ��������� ������������
	double dTimeAnimation;		// ����� ��������

	ParamIndicatorLevel()
	{
		Size = sizeof(ParamIndicatorLevel);
		LinLog = 1;
		Amplitude = 0;
		ThresholdDanger = 0.93f;
		Distance = 5;
		BorderWidth = 2;
		BorderColor = RGB(82, 82, 82);
		BackColor = RGB(103, 103, 103);
		FileName = L"";
		dTimeAnimation = 0.2;	// 200 ����
	}
};
//------------------------------------------------------------------------------
class CDIndicatorLevel : public CCustomDraw
{
public:
	CDIndicatorLevel(CRect rect, PVOID ppGraphics,
				bool *pFlagIsChange, double *pTimeAnimation);
	virtual ~CDIndicatorLevel();

private:
	ParamIndicatorLevel m_param;// ��������� ���� ���������� �����������
	COLORREF m_ColorCur;		// ������� ���� ����������
	Image *m_pImage;			// ��������� �� ������� �������� ������������
	Image *m_pImage_s;			// �������� ������������, ��������
	Image *m_pImage_d;			// �������� ������������ �������
	Image *m_pImage_p;			// �������� ������������ ������, ��� ����������
	double m_dTimeStart;		// ����� ������ ��������
	CRect m_rectT;				// ������������� ������������
	CRect m_rect1;				// ������������� ������ ����������
	CRect m_rect2;				// ������������� ��������� ����
	int m_LengthRect2;			// ������������ ����� ��������� ��������������
	bool m_bDanger;				// �������� ��������� ������� ���������
	int m_SizeRectT;			// ������ Rect ������������
	void DefineRectAll();

protected:

public:
	// ������� ���������� �����������
	TypePosition GetPositionType() { return m_param.PositionType; }
	
	long GetLinLog() { return m_param.LinLog; }
	void SetLinLog(long val);

	float GetAmplitude() { return m_param.Amplitude; }
	void SetAmplitude(float val);

	float GetThresholdDanger() { return m_param.ThresholdDanger; }
	void SetThresholdDanger(float val);

	int GetDistance() { return m_param.Distance; }
	void SetDistance(int val);
	
	int GetBorderWidth() { return m_param.BorderWidth; }
	void SetBorderWidth(int val);
	
	COLORREF GetBorderColor() { return m_param.BorderColor; }
	void SetBorderColor(COLORREF val);

	COLORREF GetBackColor() { return m_param.BackColor; }
	void SetBackColor(COLORREF val);

	CString GetFileNameTriangle() { return m_param.FileName; }
	void SetFileNameTriangle(CString str);

	double GetTimeAnimation() { return m_param.dTimeAnimation; }
	void SetTimeAnimation(double val);

	// ����������� �������, ���������������� � ������
	virtual void OnDraw();
	virtual void DoProcess();
	virtual void OnLButtonClk(UINT nFlags, CPoint point);
};
//------------------------------------------------------------------------------