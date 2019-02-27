//------------------------------------------------------------------------------
// DSimpleSlider.h : ���� ���������
//------------------------------------------------------------------------------
// ����� ����������� �������� ��������.
// �������� ��� ����� � ��������������� ����.
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Draw\CustomDraw.h"
#ifdef _GDIPLUS
#else
#define QNT_DETAILS 4
#endif
//------------------------------------------------------------------------------
struct ParamSimpleSlider : public ParamCustomDraw
{// ��������� ���������� ����������� ��������� ������
// � ������������ ��������� - ����� ���������, ����� - ������� ���������
	TypePosition PositionType;	// ��� ��������� (�����. ��� ������.)
	BOOL PositionValue;			// �������� �������
	int deltaX;
	int deltaY;
	CString FileNameField;		// ������ ��� ����� � ��������� ����
	CString FileNameCursor1;		// ������ ��� ����� � ��������� �������
	CString FileNameCursor2;		// ������ ��� ����� � ��������� �������
	double dTimeAnimation;		// ������������ �������� � ���
	float fValueCur;			// ������������� ������� ��������� �������
    double Rotate;
	int Number_Texture[3];
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
	ParamSimpleSlider()
	{
		Size = sizeof(ParamSimpleSlider);
		PositionType = tpHorizontal;
		PositionValue = 0;
		deltaX = 3;
		deltaY = 1;
		FileNameField = L"C:\\ZETTools\\Interface2012\\Textures\\Patterns\\metal_mart14.bmp";
		FileNameCursor1 = L"C:\\ZETTools\\Interface2012\\Textures\\Seamless\\Other\\cursor_1.bmp";
		FileNameCursor2 = L"C:\\ZETTools\\Interface2012\\Textures\\Seamless\\Other\\cursor_2.bmp";
		dTimeAnimation = 0.5;	// 500 ����
		fValueCur = 0.f;
        Rotate = 0;
		Number_Texture[0] = 0;
		Number_Texture[1] = 4;
		Number_Texture[2] = 5;
#ifdef _GDIPLUS
#else
        mat_ambient[0] = 0.25;
        mat_ambient[1] = 0.25;
        mat_ambient[2] = 0.25;
        mat_ambient[3] = 0.0;

        mat_diffuse[0] = 1.0;
        mat_diffuse[1] = 1.0;
        mat_diffuse[2] = 1.0;
        mat_diffuse[3] = 1.0;

        mat_specular[0] = 1.0;
        mat_specular[1] = 1.0;
        mat_specular[2] = 1.0;
        mat_specular[3] = 1.0;

        mat_emission[0] = 0.2;
        mat_emission[1] = 0.2;
        mat_emission[2] = 0.2;
        mat_emission[3] = 0.0;

        mat_shininess = 100.24f;

        mat_ambient_b[0] = 0.25;
        mat_ambient_b[1] = 0.25;
        mat_ambient_b[2] = 0.25;
        mat_ambient_b[3] = 0.0;

        mat_diffuse_b[0] = 1.0;
        mat_diffuse_b[1] = 1.0;
        mat_diffuse_b[2] = 1.0;
        mat_diffuse_b[3] = 1.0;

        mat_specular_b[0] = 1.0;
        mat_specular_b[1] = 1.0;
        mat_specular_b[2] = 1.0;
        mat_specular_b[3] = 1.0;

        mat_emission_b[0] = 0.2;
        mat_emission_b[1] = 0.2;
        mat_emission_b[2] = 0.2;
        mat_emission_b[3] = 0.0;

        mat_shininess_b = 100.24f;
#endif
	}
};
//------------------------------------------------------------------------------
class CDSimpleSlider : public CCustomDraw
{
public:
#ifdef _GDIPLUS
	CDSimpleSlider(CRect rect, PVOID ppGraphics,
		bool *pFlagIsChange, double *pTimeAnimation);
#else
	CDSimpleSlider(CRect rect,	bool *pFlagIsChange, double *pTimeAnimation);
#endif
	virtual ~CDSimpleSlider();

private:
	ParamSimpleSlider m_param;	// ��������� ���� ���������� �����������
#ifdef _GDIPLUS
	Image *m_pImage_Field;		// �������� ���� ��������
	Image *m_pImage_Cursor;		// ������ ��������� �� �������� �������� �������
	Image *m_pImage_Cursor_s;	// �������� ������� ��������, ��������
	Image *m_pImage_Cursor_m;	// �������� ������� ��������, � �����
	Image *m_pImage_Cursor_p;	// �������� ������� ��������, "������" �����
#else
	CDetail *Slider[QNT_DETAILS];
	GLuint Texture;
    GLuint Texture1;
	int PicSize[2];
    int PicSize1[2];
#endif
	double dTimeStart;			// ����� ������ ��������
	CRect m_rect0;				// ������ � ��������� 0
	CRect m_rectC;				// ������ � ������� ���������
	CRect m_rect1;				// ������ � ��������� 1
	CRect m_rect2;
	int m_LengthValue;			// ����� ���������� ���� �������
	int m_SizeCursor;	
	int m_nStart;
	bool m_bEvent;
	bool m_bFlag;

	void DefineRectAll();
	void DefineRectCur();
	bool CenterAroundZero();
	void NotPull();

protected:

public:
	// ������� ���������� �����������
	TypePosition GetPositionType() { return m_param.PositionType;}
//	void SetPositionType(TypePosition val);	������ ��� ������

	BOOL GetPositionValue() { return m_param.PositionValue;}
	void SetPositionValue(BOOL val);

	int GetDeltaX() { return m_param.deltaX;}
	void SetDeltaX(int val);
	
	int GetDeltaY() { return m_param.deltaY;}
	void SetDeltaY(int val);

	CString GetFileNameField() { return m_param.FileNameField;}
	void SetFileNameField(CString str, double angle);

	CString GetFileNameCursor() { return L"";}
	void SetFileNameCursor(CString str, double angle);
	
	double GetTimeAnimation() { return m_param.dTimeAnimation;}
	void SetTimeAnimation(double val);

	// ����������� �������, ���������������� � ������
	virtual void OnDraw();
	virtual void Clear();
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
#ifdef _GDIPLUS
#else
	void SetColor(GLfloat *ma, GLfloat *md,	
		GLfloat *me, 
		GLfloat *msp, GLfloat msh);
#endif
};
//------------------------------------------------------------------------------