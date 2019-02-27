//------------------------------------------------------------------------------
// DNumIndicator.h : ���� ���������
//------------------------------------------------------------------------------
// ����� ����������� ���������� ������
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Draw\CustomDraw.h"
//------------------------------------------------------------------------------
struct ParamNumIndicator : public ParamCustomDraw
{// ��������� ���������� ����������� ��������� ������
	// � ������������ ��������� - ����� ���������, ����� - ������� ���������
	TypePosition PositionType;	// ��� ��������� (�����. ��� ������.)
	long LinLog;				// �������� - 0, ��������������� - 1
	float Amplitude;			// �������� �� 0 �� 1
	float ThresholdDanger;		// ����� ���������
	int Distance;				// ��������� �/� ����������� � �������������
	int BorderWidth;			// ������ �������
	int Precision;				// ����������� ������������ ������ ����� �������
	int Integer_part;            // ���������� ��������� ������ ����� ����� �����
	COLORREF BorderColor;		// ���� �������
	COLORREF BackColor;			// ���� ����
    COLORREF TextColor;         //���� ������
	CString FileName;			// ������ ��� ����� � ��������� ������������
	double dTimeAnimation;		// ����� ��������
    char FontName[20];
	CString Text;	
	float TextSize;
	CString TextValue;
	long postcomma;            // ���������� ���� ����� �������
	bool TypeView;
#ifdef _GDIPLUS
	CZFont *pFont;		// ��������� ������ ������
	StringAlignment AlignmentHorizontal;	// ������������ �� �����������
	StringAlignment AlignmentVertical;		// ������������ �� ���������
	//TypeThreeDots ThreeDots;				// ����������
#else
	StringAlignmentOGL AlignmentHorizontal;	// ������������ �� �����������
	StringAlignmentOGL AlignmentVertical;	// ������������ �� ���������
	CDFonts *pFont;
#endif
	ParamNumIndicator()
	{
		Size = sizeof(ParamNumIndicator);
		LinLog = 1;
		TypeView = false;
		TextSize = 8.f;
		Amplitude = 0;
		ThresholdDanger = 0.93f;
		Distance = 5;
		BorderWidth = 2;
		BorderColor = RGB(82, 82, 82);
		BackColor = RGB(103, 103, 103);
        TextColor = RGB(40,90,40);
        sprintf(FontName, "Arialbd");
		FileName = L"";
		dTimeAnimation = 0.2;	// 200 ����
		pFont = NULL;
		Text = L"";
		Precision = 7;
		Integer_part = 5;
		PositionType = tpVertical;
		postcomma = 0;
#ifdef _GDIPLUS
		AlignmentHorizontal = StringAlignmentCenter;
		AlignmentVertical = StringAlignmentCenter;
		//ThreeDots = no;
#else
		AlignmentHorizontal = StrAlignmentCenter;
		AlignmentVertical = StrAlignmentCenter;
#endif
	}
};
//------------------------------------------------------------------------------
class CDNumIndicator : public CCustomDraw
{
public:
#ifdef _GDIPLUS
	CDNumIndicator(CRect rect, PVOID ppGraphics,
		bool *pFlagIsChange, double *pTimeAnimation);
#else
	CDNumIndicator(CRect rect,
		bool *pFlagIsChange, double *pTimeAnimation);
#endif
	virtual ~CDNumIndicator();

private:
	ParamNumIndicator m_param;	// ��������� ���� ���������� �����������
	COLORREF m_ColorCur;		// ������� ���� ����������
	double m_dTimeStart;		// ����� ������ ��������
	CRect m_rectT;				// ������������� ������������
	CRect m_rect1;				// ������������� ������ ����������
	CRect m_rect2;				// ������������� ��������� ����
	CRect m_rect3;
	int m_LengthRect2;			// ������������ ����� ��������� ��������������
	bool m_bDanger;				// �������� ��������� ������� ���������
	int m_SizeRectT;			// ������ Rect ������������
	void DefineRectAll();
	char vOut [20];
	bool flag;
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

	double GetTimeAnimation() { return m_param.dTimeAnimation; }
	void SetTimeAnimation(double val);

	long GetSizeString () { return m_param.postcomma; }
	void SetSizeString (long postcomma) { m_param.postcomma = postcomma; }

	CString GetText() { return m_param.Text; }
	void SetText(CString val);

	int GetPrecision() { return m_param.Precision; }
	void SetPrecision(int val);

	void SetIntegerPart (int val);
	int GetIntegerPart (void) {return m_param.Integer_part;}

	void SetTextColor(COLORREF colortext);
	COLORREF GetTextColor (void);

	void SetTextSize (float val);
	float GetTextSize (void);

	void SetTypeView (bool val) {m_param.TypeView = val; }
	bool GetTypeView (void) { return m_param.TypeView; }

	void SetTextValue (CString val);
	CString GetTextVal (void);

	void SetTextCeils (bool val);
	bool GetTextCeils (void);

#ifdef _GDIPLUS
#else
	CDFonts* GetFont() { return m_param.pFont; }
	void SetFont(CDFonts *pfont);
#endif

	// ����������� �������, ���������������� � ������
	virtual void OnDraw();
	virtual void DoProcess();
	virtual void OnLButtonClk(UINT nFlags, CPoint point);
	virtual void OnMouseEnter();
	virtual void OnMouseLeave();
    virtual void SaveParameters(SimpleXML *pSimpleXML);
    virtual void LoadParameters(SimpleXML *pSimpleXML);
};
//------------------------------------------------------------------------------