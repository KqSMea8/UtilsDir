//------------------------------------------------------------------------------
// DScrollBar.h : ���� ���������
//------------------------------------------------------------------------------
// ����� ����������� ��������.
// �������� ��� ����� � ����� �����.
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Draw\CustomDraw.h"
#include "C:\ZETTools\Interface2012\Custom\STL\Detail.h"
#include "C:\ZETTools\Interface2012\Custom\STL\STLRead.h"
#define QNT_DETAILS 4
//------------------------------------------------------------------------------
struct ParamScrollBar : public ParamCustomDraw
{// ��������� ���������� ����������� ��������� ������
	// � ������������ ��������� - ����� ���������, ����� - ������� ���������
	TypePosition PositionType;	// ��� ��������� (�����. ��� ������.)
	float PositionValue;			// �������� �������
	float CurrentPosition;			//������� ������� � ��������
	int deltaX;
	int deltaY;
	CString FileNameField;		// ������ ��� ����� � ��������� ����
	CString FileNameCursor;		// ������ ��� ����� � ��������� �������
	double dTimeAnimation;		// ������������ �������� � ���
	float fValueCur;			// ������������� ������� ��������� �������
	double Rotate;
	int Number_Texture[2];
	CString Texture_1_FileName;
	CString Texture_2_FileName;
#ifdef _GDIPLUS
#else
	GLfloat mat_ambient[4]; 	//	{r, g, b, a}
	GLfloat mat_diffuse[4];		//	{r, g, b, a}
	GLfloat mat_specular[4];	//	{r, g, b, a}
	GLfloat mat_emission[4];	//	{r, g, b, a}
	GLfloat mat_shininess;		//	����������� �����

	GLfloat mat_ambient_b[4]; 	//	{r, g, b, a}
	GLfloat mat_diffuse_b[4];		//	{r, g, b, a}
	GLfloat mat_specular_b[4];	//	{r, g, b, a}
	GLfloat mat_emission_b[4];	//	{r, g, b, a}
	GLfloat mat_shininess_b;		//	����������� �����
#endif
	ParamScrollBar()
	{
		Size = sizeof(ParamScrollBar);
		PositionType = tpHorizontal;
		PositionValue = 0;
		CurrentPosition =0;
		deltaX = 3;
		deltaY = 1;
		FileNameField = L"";
		FileNameCursor = L"";
		dTimeAnimation = 0.5;	// 500 ����
		fValueCur = 0.f;
		Rotate = 0;
		Number_Texture[0] = 0;
		Number_Texture[1] = 20;
		Texture_1_FileName = L"C:\\ZETTools\\Interface2012\\Textures\\Patterns\\metal_mart14.bmp";
		Texture_2_FileName = L"C:\\ZETTools\\Interface2012\\Textures\\Seamless\\Other\\cursor_03.bmp";
#ifdef _GDIPLUS
#else
		mat_ambient_b[0] = mat_ambient[0] = 0.2;
		mat_ambient_b[1] = mat_ambient[1] = 0.2;
		mat_ambient_b[2] = mat_ambient[2] = 0.2;
		mat_ambient_b[3] = mat_ambient[3] = 1.0;

		mat_diffuse_b[0] = mat_diffuse[0] = 1.0;
		mat_diffuse_b[1] = mat_diffuse[1] = 1.0;
		mat_diffuse_b[2] = mat_diffuse[2] = 1.0;
		mat_diffuse_b[3] = mat_diffuse[3] = 1.0;

		mat_specular_b[0] = mat_specular[0] = 1.0;
		mat_specular_b[1] = mat_specular[1] = 1.0;
		mat_specular_b[2] = mat_specular[2] = 1.0;
		mat_specular_b[3] = mat_specular[3] = 1.0;

		mat_emission_b[0] = mat_emission[0] = 0.2;
		mat_emission_b[1] = mat_emission[1] = 0.2;
		mat_emission_b[2] = mat_emission[2] = 0.2;
		mat_emission_b[3] = mat_emission[3] = 0.0;

		mat_shininess_b = mat_shininess = 100.24f;
#endif
	}
};
//------------------------------------------------------------------------------
class CDScrollBar : public CCustomDraw
{
public:
	CDScrollBar(CRect rect,	bool *pFlagIsChange, double *pTimeAnimation);
	virtual ~CDScrollBar();
	virtual void Clear (void);
private:
	ParamScrollBar m_param;	// ��������� ���� ���������� �����������
	CDetail *ScrollBar[QNT_DETAILS];
	GLuint Texture, Texture1;
	int PicSize[2], PicSize1[2];
	double dTimeStart;			// ����� ������ ��������
	CRect m_rect0;				// ������ � ��������� 0
	CRect m_rectC;				// ������ � ������� ���������
	CRect m_rect1;				// ������ � ��������� 1
	int m_LengthValue;			// ����� ���������� ���� �������
	int m_SizeCursor;	
	double m_nStart;
	bool m_bEvent;
	bool m_bFlag;

	bool nowUsing; //����, ���������� ��� ������ ������ ScrollBar ������������
	void DefineRectAll();
	void DefineRectCur();

protected:

public:
	// ������� ���������� �����������
	TypePosition GetPositionType() { return m_param.PositionType;}
	//	void SetPositionType(TypePosition val);	������ ��� ������

	float GetPositionValue() { return m_param.PositionValue;}
	void SetPositionValue(float val);

	int GetDeltaX() { return m_param.deltaX;}
	void SetDeltaX(int val);

	int GetDeltaY() { return m_param.deltaY;}
	void SetDeltaY(int val);

	CString GetFileNameField() { return m_param.FileNameField;}
	void SetFileNameField(CString str, double angle);

	CString GetFileNameCursor() { return m_param.FileNameCursor;}
	void SetFileNameCursor(CString str, double angle);

	double GetTimeAnimation() { return m_param.dTimeAnimation;}
	void SetTimeAnimation(double val);

	// ����������� �������, ���������������� � ������
	virtual void OnDraw();
	virtual void SaveParameters(SimpleXML *pSimpleXML);
	virtual void LoadParameters(SimpleXML *pSimpleXML);
	virtual void DoProcess();
	virtual void OnMouseEnter();
	virtual void OnMouseLeave();
	virtual void OnLButtonClk(UINT nFlags, CPoint point);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnMouseMove(UINT nFlags, CPoint point);

	// ������� ������
	bool GetNeedEvent();
	void SetColor(GLfloat *ma, GLfloat *md,	
		GLfloat *me, 
		GLfloat *msp, GLfloat msh);

	void LiftScrollBar(float val);
	float GetValueCur() { return m_param.fValueCur; } //���������� ������� ������������� �������� �������
	void SetValueCur(float val) { m_param.fValueCur = val; DefineRectCur(); IsChange();} //������������� ������� ������������� �������� ������� (�������� � ������� [0;1])
	void (*pScroll)(void);
	void PionterForScroll(void (*val)()) { pScroll = val;}

	bool GetNowUsing() { return nowUsing;}
	void SetNowUsing(bool val) { nowUsing = val;}
	
};
//------------------------------------------------------------------------------