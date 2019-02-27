//------------------------------------------------------------------------------
// DGroupBox.h : ���� ���������
//------------------------------------------------------------------------------
// ����� ����������� �������� GroupBox
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Custom\Font.h"
#include "C:\ZETTools\Interface2012\Draw\CustomDraw.h"
//------------------------------------------------------------------------------
struct ParamGroupBox : public ParamCustomDraw
{// ��������� ���������� ����������� ��������� ������
// � ������������ ��������� - ����� ���������, ����� - ������� ���������
	CString Caption;		// ���������
	int Radius;				// ������ ����������� �����
	int LengthLine0;		// ����� ����� � 0 (������� ������)
	float CoordZ;
	float thickness;        // ������� �����
	StringAlignmentOGL Alignment;
    char FontName[20];
	float TextSize;
#ifdef _GDIPLUS
	CZFont *pFont;		// ��������� ������ ������
#else
	CDFonts *pFont;
	/*GLfloat mat_ambient[4]; 	//	{r, g, b, a}
	GLfloat mat_diffuse[4];		//	{r, g, b, a}
	GLfloat mat_specular[4];	//	{r, g, b, a}
	GLfloat mat_emission[4];	//	{r, g, b, a}
	GLfloat mat_shininess;		//	����������� �����
*/
#endif
	COLORREF Color;			// ���� �����
	COLORREF TextColor;		// ���� ������
	ParamGroupBox()
	{
		Size = sizeof(ParamGroupBox);
		Caption = L"";
		Radius = 4;//4;
		LengthLine0 = 1;
		thickness = 2.f;
		pFont = NULL;
		Color = RGB(255,255,255);
		TextColor = RGB(255,255,255);
		strcpy(FontName, "Arialbd");
		CoordZ = 1.5;
		Alignment = StrAlignmentCenter;
		TextSize = 8.f;
/*#ifdef _GDIPLUS
#else
		mat_ambient[0] = 0.5;
		mat_ambient[1] = 0.5;
		mat_ambient[2] = 0.5;
		mat_ambient[3] = 1.0;

		mat_diffuse[0] = 0.5;
		mat_diffuse[1] = 0.5;
		mat_diffuse[2] = 0.5;
		mat_diffuse[3] = 1.0;

		mat_specular[0] = 0.2;
		mat_specular[1] = 0.2;
		mat_specular[2] = 0.2;
		mat_specular[3] = 1.0;

		mat_emission[0] = 1.0;
		mat_emission[1] = 0.1;
		mat_emission[2] = 0.1;
		mat_emission[3] = 1.0;

		mat_shininess = 10.24f;
#endif*/
	}
};
//------------------------------------------------------------------------------
class CDGroupBox : public CCustomDraw
{
public:
#ifdef _GDIPLUS
	CDGroupBox(CRect rect, PVOID ppGraphics,
				bool *pFlagIsChange, double *pTimeAnimation);
#else
	CDGroupBox(CRect rect,
		bool *pFlagIsChange, double *pTimeAnimation);
#endif
	virtual ~CDGroupBox();

private:
	ParamGroupBox m_param;// ��������� ���� ���������� �����������

protected:

public:
	// ������� ���������� �����������
	CString GetCaption() { return m_param.Caption; }
	void SetCaption(CString val);
#ifdef _GDIPLUS
	CZFont* GetFont() { return m_param.pFont; }
	void SetFont(CZFont *pfont);
#else
	CDFonts* GetFont() { return m_param.pFont; }
	void SetFont(CDFonts *pfont);
#endif
	int GetRadius() { return m_param.Radius; }
	void SetRadius(int val);

	int GetLengthLine0() { return m_param.LengthLine0; }
	void SetLengthLine0(int val);

	float GetThickness () {return m_param.thickness;}
	void SetThickness (float val);

	COLORREF GetColor() { return m_param.Color; }
	void SetColor(COLORREF val);

	float GetCoordZ() { return m_param.CoordZ;}
	void SetCoordZ(float val);

	void SetTextColor (COLORREF val);
	COLORREF GetTextColor (void) { return m_param.pFont->GetColor(); };

	void SetTextSize (float val);
	float GetTextSize (void) { return m_param.TextSize; }

	StringAlignmentOGL GetAligment() { return m_param.Alignment; }
	void SetAligment(StringAlignmentOGL val);

	// ����������� �������, ���������������� � ������
	virtual void OnDraw();
    virtual void SaveParameters(SimpleXML *pSimpleXML);
    virtual void LoadParameters(SimpleXML *pSimpleXML);
};
//------------------------------------------------------------------------------