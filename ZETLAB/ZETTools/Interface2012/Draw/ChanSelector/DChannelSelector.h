//------------------------------------------------------------------------------
// DchanSelector.h : ���� ���������
//------------------------------------------------------------------------------
// ����� ����������� ChanSelector.
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Draw\CustomDraw.h"
#define DETAILS 9
//------------------------------------------------------------------------------
struct ParamChannelSelector : public ParamCustomDraw
{// � ������������ ��������� - ����� ���������, ����� - ������� ���������
	int BorderWidth;			// ������ �������
	BOOL PositionValue;			// ������� ������
	COLORREF BorderColor;		// ���� ������� ��� ������
	COLORREF BorderColorFocus;	// ���� ������� � �������
	COLORREF BackColor;			// ���� ���� ��� ����
	COLORREF BackColorMouse;	// ���� ���� ��� ��������� ����
	COLORREF BackColorEnter;	// ���� ���� ��� ������� ������
	COLORREF CheckColorMouse;
	COLORREF CheckColor;
	CString Caption;			// �����, ������� ��������� �� ������
	CString CaptionChange;			// �����, ������� ��������� �� ������ ��� ������� ��� ������������� ������
	CString fileName;		// ������� ��� ������ � ����������
#ifdef _GDIPLUS
	CZFont *pFont;		// ��������� ������ ������
	StringAlignment AlignmentHorizontal;	// ������������ �� �����������
	StringAlignment AlignmentVertical;		// ������������ �� ���������
	LinearGradientMode GradientMode;	// ��� ���������
#else
	CDFonts *pFont;
	GLfloat mat_ambient[4]; 	//	{r, g, b, a}
	GLfloat mat_diffuse[4];		//	{r, g, b, a}
	GLfloat mat_specular[4];	//	{r, g, b, a}
	GLfloat mat_emission[4];	//	{r, g, b, a}
	GLfloat mat_shininess;		//	����������� �����
#endif
	double dTimeAnimation;	// ������������ �������� ������
	double dTimeCur;		// ������� ����������������� ��������
	double dTimeStart;		// ����� ������ ��������
	int SizeText;			// ������ ���������� ���� (��� CheckBox)
	Point pt[3];	
	ParamChannelSelector()
	{
		Size = sizeof(ParamChannelSelector);
		BorderWidth = 0;		// � ������������ ������������ 1
		BorderColor = RGB(66, 66, 66);
		BorderColorFocus = RGB(66, 66, 66);	// 164, 210, 244;  150 210 255
		BackColor = RGB(240, 240, 240);
		BackColorMouse = RGB(223, 243, 255);
		BackColorEnter = RGB(240, 240, 240);
		//ButtonType = false;
		//CheckBox = false;
		SizeText = 60;
		Caption = L"";
		CaptionChange = L"";
		pFont = NULL;
		dTimeAnimation = 0.5;
		dTimeCur = 0.;
		dTimeStart = 0.;
#ifdef _GDIPLUS
		AlignmentHorizontal = StringAlignmentCenter;
		AlignmentVertical = StringAlignmentCenter;
		GradientMode = LinearGradientModeForwardDiagonal;
#else
		mat_ambient[0] = 0.1;
		mat_ambient[1] = 0.1;
		mat_ambient[2] = 0.1;
		mat_ambient[3] = 1.0;

		mat_diffuse[0] = 0.5;
		mat_diffuse[1] = 0.5;
		mat_diffuse[2] = 0.5;
		mat_diffuse[3] = 1.0;

		mat_specular[0] = 0.2;
		mat_specular[1] = 0.2;
		mat_specular[2] = 0.2;
		mat_specular[3] = 1.0;

		mat_emission[0] = 0.2;
		mat_emission[1] = 0.2;
		mat_emission[2] = 0.2;
		mat_emission[3] = 1.0;

		mat_shininess = 10.24f;
#endif
		fileName = L"";
	}
};
//------------------------------------------------------------------------------
class CDChannelSelector : public CCustomDraw
{
public:
#ifdef _GDIPLUS
	CDChannelSelector(CRect rect, PVOID ppGraphics,
		bool *pFlagIsChange, double *pTimeAnimation);
#else
	CDChannelSelector(CRect rect,
		bool *pFlagIsChange, double *pTimeAnimation);
#endif
	virtual ~CDChannelSelector();

private:
	ParamChannelSelector m_param;	// ��������� ���� ���������� �����������
	COLORREF m_BackColorCur;
	COLORREF m_BorderColorCur;
	CRect m_rect1;			// ������������� ���������� ����
	bool m_bEvent;			// ���� �������
	bool m_bFixation;		// ���� �������
	CString m_Text;
	double dTimeStart;			// ����� ������ ��������
	int Width;
	int size;
#ifdef _GDIPLUS
	CZFont *m_pFont;
	Image *m_pImage_Cur;	// ��. �� ������� ��������
	Image *m_pImage_s;		// ����������� �����������
	Image *m_pImage_m;		// ����������� � �����
	Image *m_pImage_p;		// ����������� � ������� ���������
	Image *m_pImage_pm;		// ����������� � ����� ��� checkbox � ������� ���������
#else
	CDFonts *m_pFont;
	GLuint Texture;
	CDetail *m_ChSelector[DETAILS];
	int PicSize[2];
	CRect  rect_panel;
#endif
protected:

public:
	// ������� ���������� �����������
	BOOL GetPositionValue() { return m_param.PositionValue;}
	void SetPositionValue(BOOL val);

	int GetBorderWidth() { return m_param.BorderWidth; }
	void SetBorderWidth(int val);

	COLORREF GetBorderColor() { return m_param.BorderColor; }
	void SetBorderColor(COLORREF val);

	COLORREF GetBorderColorFocus() { return m_param.BorderColorFocus; }
	void SetBorderColorFocus(COLORREF val);

	CString GetCaption() { return m_param.Caption; }
	void SetCaption(CString str);

	int GetSizeText() { return m_param.SizeText; }
	void SetSizeText(int val);

	CString GetCaptionChange() { return m_param.CaptionChange; }
	void SetCaptionChange(CString str);

	double GetTimeAnimation() { return m_param.dTimeAnimation;}
	void SetTimeAnimation(double val);

	bool GetAnimation()	 { return m_param.bAnimation; }
	void SetAnimation(bool val) { m_param.bAnimation = val; }
#ifdef _GDIPLUS
	CZFont* GetFont() { return m_pFont; }
	void SetFont(CZFont *pFont) { m_pFont = pFont; }

	StringAlignment GetAlignmentHorizontal() {return m_param.AlignmentHorizontal; }
	void SetAlignmentHorizontal(StringAlignment val);

	StringAlignment GetAlignmentVertical() {return m_param.AlignmentVertical; }
	void SetAlignmentVertical(StringAlignment val);
#else
	CDFonts* GetFont() { return m_pFont; }
	void SetFont(CDFonts *pFont) { m_pFont = pFont; }
#endif
	bool GetFixsation() { return m_bFixation; }

	CString GetImageFileName() { return m_param.fileName;}
	void SetImageFileName(CString str);

	bool GetNeedEvent();

	// ����������� �������, ���������������� � ������
	virtual void OnDraw();
	virtual void GetFocus();
	virtual void KillFocus();
	virtual void OnMouseEnter();
	virtual void OnMouseLeave();
	//virtual void DoProcess();
	virtual void OnLButtonClk(UINT nFlags, CPoint point);

};
//------------------------------------------------------------------------------