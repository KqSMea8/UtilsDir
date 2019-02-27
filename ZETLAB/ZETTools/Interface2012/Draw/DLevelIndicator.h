//------------------------------------------------------------------------------
// DNumIndicator.h : ���� ���������
//------------------------------------------------------------------------------
// ����� ����������� ���������� ������
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Draw\CustomDraw.h"
//------------------------------------------------------------------------------
struct ParamLevelIndicator : public ParamCustomDraw
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
	CString Text;	
	double value;
	double ur;
	long formax;
	float maxlevel;
	bool rotateindicator;
	bool setdiod;
	int BorderHeight;
	int BorderRadius;
	COLORREF BorderSolidColor;
	COLORREF BorderGradientColorStart;
	COLORREF BorderGradientColorEnd;
	int FieldRadius;
	COLORREF FieldSolidColor;
	CString FieldFileNameTexture;
	CRect FieldRect;
	float CoordZ;
#ifdef _GDIPLUS
	CZFont *pFont;		// ��������� ������ ������
	StringAlignment AlignmentHorizontal;	// ������������ �� �����������
	StringAlignment AlignmentVertical;		// ������������ �� ���������
	//TypeThreeDots ThreeDots;				// ����������
#else
	GLfloat mat_ambient[4]; 	//	{r, g, b, a}
	GLfloat mat_diffuse[4];		//	{r, g, b, a}
	GLfloat mat_specular[4];	//	{r, g, b, a}
	GLfloat mat_emission[4];	//	{r, g, b, a}
	GLfloat mat_shininess;		//	����������� �����
	StringAlignmentOGL AlignmentHorizontal;	// ������������ �� �����������
	StringAlignmentOGL AlignmentVertical;		// ������������ �� ���������
	CDFonts *pFont;
#endif
	ParamLevelIndicator()
	{
		value = 0.0;
		Size = sizeof(ParamLevelIndicator);
		LinLog = 1;
		Amplitude = 0;
		ThresholdDanger = 0.93f;
		Distance = 5;
		BorderWidth = 2;
		BorderColor = RGB(82, 82, 82);
		BackColor = RGB(103, 103, 103);
		FileName = L"";
		dTimeAnimation = 0.2;	// 200 ����
		pFont = NULL;
		Text = L"";
		BorderHeight = 4;
		BorderRadius = 1;
		BorderSolidColor = RGB(90, 90, 90);
		BorderGradientColorStart = RGB(180, 180, 180);
		BorderGradientColorEnd = RGB(90, 90, 90);
		FieldRect = CRect(0, 0, 0, 0);
		CoordZ = 0.1;
		PositionType = tpHorizontal;
		ur = 0.0;
		formax = 0;
		maxlevel = 0.f;
		rotateindicator = false;
		setdiod = false;
#ifdef _GDIPLUS
		BorderGradientMode = LinearGradientModeForwardDiagonal;
#endif
		FieldRadius = 0;
		FieldSolidColor = RGB(23, 23, 23);
		FieldFileNameTexture = L"";
#ifdef _GDIPLUS
		AlignmentHorizontal = StringAlignmentCenter;
		AlignmentVertical = StringAlignmentCenter;
		//ThreeDots = no;
#else
		AlignmentHorizontal = StrAlignmentCenter;
		AlignmentVertical = StrAlignmentCenter;
#endif
#ifdef _GDIPLUS
#else
		mat_ambient[0] = 0.3;
		mat_ambient[1] = 0.3;
		mat_ambient[2] = 0.3;
		mat_ambient[3] = 0.0;

		mat_diffuse[0] = 1.0;
		mat_diffuse[1] = 1.0;
		mat_diffuse[2] = 1.0;
		mat_diffuse[3] = 1.0;

		mat_specular[0] = 1.0;
		mat_specular[1] = 1.0;
		mat_specular[2] = 1.0;
		mat_specular[3] = 1.0;

		mat_emission[0] = 0.0;
		mat_emission[1] = 0.0;
		mat_emission[2] = 0.0;
		mat_emission[3] = 0.0;

		mat_shininess = 15.0f;
#endif
	}
};
//------------------------------------------------------------------------------
class CDLevelIndicator : public CCustomDraw
{
public:
#ifdef _GDIPLUS
	CDLevelIndicator(CRect rect, PVOID ppGraphics,
		bool *pFlagIsChange, double *pTimeAnimation);
#else
	CDLevelIndicator(CRect rect, bool *pFlagIsChange, double *pTimeAnimation, bool rotate, bool setdiod);
#endif
	virtual ~CDLevelIndicator();

private:
#ifdef _GDIPLUS
#else
//	CDFonts *font1;
//	CDetail *m_SvetoDiod;
//	CDFonts *font1;
	CDetail *m_SvetoDiodOsn;
//	CDFonts *font2;
	CDetail *m_SvetoDiodLed;
	GLuint Texture;
	int PicSize[2];
	GLUquadric *qobj;
#endif
	CRect rect_draw;
	ParamLevelIndicator m_param;// ��������� ���� ���������� �����������
	COLORREF m_ColorCur;		// ������� ���� ����������
	double m_dTimeStart;		// ����� ������ ��������
	CRect m_rectT;				// ������������� ������������
	CRect m_rect1;				// ������������� ������ ����������
	CRect m_rect2;				// ������������� ��������� ����
	int m_LengthRect2;			// ������������ ����� ��������� ��������������
	bool m_bDanger;				// �������� ��������� ������� ���������
	int m_SizeRectT;			// ������ Rect ������������
	void DefineRectAll();
	char vOut [20];
	bool flag;
	float m_bringmaxlevel;
protected:

public:
	// ������� ���������� �����������
	TypePosition GetPositionType() { return m_param.PositionType; }

	long GetLinLog() { return m_param.LinLog; }
	void SetLinLog(long val);

	long PointColor(double bit);

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

	CString GetText() { return m_param.Text; }
	void SetText(CString val);

	float GetMaxLevel () { return m_param.maxlevel; }
	void SetMaxLevel (float valleve);

	// ����������� �������, ���������������� � ������
	virtual void OnDraw();
	virtual void Clear();
    virtual void SaveParameters(SimpleXML *pSimpleXML);
    virtual void LoadParameters(SimpleXML *pSimpleXML);
	virtual void DoProcess();
	virtual void OnLButtonClk(UINT nFlags, CPoint point);
	virtual void OnMouseEnter();
	virtual void OnMouseLeave();
};
//------------------------------------------------------------------------------