//------------------------------------------------------------------------------
// DGrid.h : ���� ���������
//------------------------------------------------------------------------------
#pragma once
#include "math.h"
#include "C:\ZETTools\Interface2012\Draw\CustomDraw.h"
#include "C:\ZETTools\Interface2012\Custom\FunctionForAll.h"
#include "C:\ZETTools\Interface2012\Draw\FunctionsForGraph.h"
#include <string>

#define QNT_STEPS_X 11 //���������� �����
#define QNT_STEPS_Y 9 //���������� �����
#define  MIN_STEP 25 //� ��������
#define  MAX_STEP 40
#define  MIN_STEP_LOG 5
#define ZDEPTH 1
#define EPS 0.00001 //��������

//const float StepConstants[]={0.05, 0.05, 0.06, 0.07, 0.08, 0.09, 0.13, 0.17, 0.30}; //��������� ��� ����������� ��������. �������� ����� ����������� �������� ����� � �������� �������
const float StepConstants[]={0.0, log10(2.0), log10(3.0), log10(4.0), log10(5.0), log10(6.0), log10(7.0), log10(8.0), log10(9.0), 1.0};

struct LineStyle{
	LineType lpLineType; //��� ����� (��������, ����������)
	float lpWidth;
	GLfloat lpColor[3];
};

struct AxisParam{
    double apMaxCurrent, apMinCurrent; //������� ������� ������� �� ���
    double apBegin, apEnd; //������� �����������
    double apPxStep, apStep; //��� ����� (�������� ������� � �������)
    double apScale; //����������� ���������������
    double apShift; //��� ������
    double apMarkingArray[MAX_QNT_MARKS]; //������ �����-�����
    double apMaxTextLength; //������������ ����� ������, ���������� �����
    double apMaxTexthHeight; //������������ ������
    int apQntMarks, apQntBoldMarks, apQntTextMarks; //���������� �����
    ScaleType apScaleType; //��� �����
    GridType apGridType; //��� ���
    CString apTitle;
};

struct Dimention{
    double dWidth;
    double dHeight;
};

struct ParamGrid : public ParamCustomDraw 
{
	CRect m_rect; //�������, ���������� ���������
    CRect m_rectLegend; //�������, ���������� ��������
	CRect m_rectRightShift, m_rectLeftShift, m_rectTopShift, m_rectBottomShift, m_rectGrid,
        m_rectRightScale, m_rectLeftScale, m_rectBottomScale, m_rectTopScale, m_centerAutoScale; //������� ��� ������ � ����� (�������� ����, ���������������, �����������)
    CPoint m_cursorPosition; //������� ������� ����
    CString m_legend; //�������
    CString m_textureName; //���� � ����� � ���������
	double m_topBorder, m_bottomBorder, m_leftBorder, m_rightBorder; //������� �� ���� �������� �� ����� � ��������
	bool m_crosshair; //���� ��������� �����������
	LineStyle m_borderStyle, m_boldVertGridStyle, m_boldHorGridStyle,
        m_fineHorGridStyle, m_fineVertGridStyle, m_marksStyle,
        m_axisStyle, m_crosshairStyle, m_legendStyle; //����� ����� (���, �������, ����)
    CDFonts m_font, m_fontH, m_fontV; //������
    AxisParam m_axisX, m_axisY; //��������� ����
    Settings m_setting; //����� ������� ������� (������, ��������������)
    FitType m_fitTypeX, m_fitTypeY; //������� ���������������
    CDetail m_pBorders[8];
    GLfloat m_gridColor[3];
    GLfloat m_textColor[3];
    GLfloat m_legendColor[3];
    GLfloat mat_ambient[4], mat_ambient1[4]; 	//	{r, g, b, a}
    GLfloat mat_diffuse[4], mat_diffuse1[4];	//	{r, g, b, a}
    GLfloat mat_specular[4], mat_specular1[4];	//	{r, g, b, a}
    GLfloat mat_emission[4], mat_emission1[4];	//	{r, g, b, a}
    GLfloat mat_shininess, mat_shininess1;		//	����������� �����
	int Number_Texture;
    ParamGrid()
	{
        Size = sizeof(ParamGrid);
        m_axisX.apGridType = gtHorizontal;
        m_axisY.apGridType = gtVertical;
        m_axisX.apScaleType = stLinear;
        m_axisY.apScaleType = stLinear;
        m_fitTypeX = ftLeft;
        m_fitTypeY = ftCenter;
        m_axisX.apStep = 0;
        m_axisY.apStep = 0;
        m_axisX.apMinCurrent = 0;
        m_axisY.apMinCurrent = 0;
        m_axisX.apMaxCurrent = 100;
        m_axisY.apMaxCurrent = 100;
        m_axisX.apShift = 10;
        m_axisY.apShift = 10;
        m_axisX.apBegin = -200;
        m_axisX.apEnd = 200;
        m_axisY.apBegin = -200;
        m_axisY.apEnd = 200;
        m_axisX.apTitle = L"";
        m_axisY.apTitle = L"";
        m_cursorPosition.x = 0;
        m_cursorPosition.y = 0;
        m_crosshair = true;
        m_setting = sAuto;
		m_topBorder = 0.0;
		m_bottomBorder = 0.0;
		m_leftBorder = 0.0;
		m_rightBorder = 0.0;
		memset(&m_borderStyle, 0, sizeof(LineStyle));
		memset(&m_boldHorGridStyle, 0, sizeof(LineStyle));
		memset(&m_boldVertGridStyle, 0, sizeof(LineStyle));
		memset(&m_fineVertGridStyle, 0, sizeof(LineStyle));
		memset(&m_fineHorGridStyle, 0, sizeof(LineStyle));
		memset(&m_marksStyle, 0, sizeof(LineStyle));
		memset(&m_axisStyle, 0, sizeof(LineStyle));	
		memset(&m_crosshairStyle, 0, sizeof(LineStyle));	
		memset(&m_legendStyle, 0, sizeof(LineStyle));	
		Number_Texture = 0;

        m_gridColor[0] = 255;
        m_gridColor[1] = 255;
        m_gridColor[2] = 255;

        m_textColor[0] = 255;
        m_textColor[1] = 255;
        m_textColor[2] = 255;

        mat_ambient[0] = 0.2;
        mat_ambient[1] = 0.2;
        mat_ambient[2] = 0.2;
        mat_ambient[3] = 1.0;

        mat_diffuse[0] = 0.5;
        mat_diffuse[1] = 0.5;
        mat_diffuse[2] = 0.5;
        mat_diffuse[3] = 1.0;

        mat_specular[0] = 0.296648;
        mat_specular[1] = 0.296648;
        mat_specular[2] = 0.296648;
        mat_specular[3] = 1.0;

        mat_emission[0] = 0.2;
        mat_emission[1] = 0.2;
        mat_emission[2] = 0.2;
        mat_emission[3] = 1.0;

        mat_shininess = 100.24f;

        mat_ambient1[0] = 0.2;
        mat_ambient1[1] = 0.2;
        mat_ambient1[2] = 0.2;
        mat_ambient1[3] = 0.0;

        mat_diffuse1[0] = 1.0;
        mat_diffuse1[1] = 1.0;
        mat_diffuse1[2] = 1.0;
        mat_diffuse1[3] = 1.0;

        mat_specular1[0] = 0.1;
        mat_specular1[1] = 0.1;
        mat_specular1[2] = 0.1;
        mat_specular1[3] = 0.1;

        mat_emission1[0] = 0.0;
        mat_emission1[1] = 0.0;
        mat_emission1[2] = 0.0;
        mat_emission1[3] = 0.0;

        mat_shininess1 = 1.0f;
    }
	~ParamGrid()
	{
		m_font.Clear();
		m_fontH.Clear();
		m_fontV.Clear();

	}
};

class CDGrid : public CCustomDraw{
	public:
		CDGrid(CRect rect, bool *pFlagIsChange, double *pTimeAnimation);
		virtual ~CDGrid();
	
	protected:

	private:
		ParamGrid m_param;
        bool m_bFlag;
        GLuint Texture;
        int PicSize[2];
        CDetail m_pButton[9];
        //������� ���������
		void DrawGrid();
		void DrawBorder();
		void DrawArrows();
        void DrawCrosshair(CPoint point);
        void DrawTextMarks();
        void DrawLegend();
        void DrawSignature();
        void DrawBackground();
        void DrawButtons();

		void FillTextMarking(GridType axis);
		void ShiftGrid(zRect val);
        //auto
        AxisParam FillStruct(double maxCurrent, double minCurrent, GridType gridType, ScaleType scaleType);
        //manually
        AxisParam FillStruct(double maxCurrent, double minCurrent, double step, GridType gridType, ScaleType scaleType);
	
	public:
        //������� ���������� �����������
		void SetGridType(GridType val);
        void SetImageFileName(CString str);
		LineStyle SetLineParam(LineType lineType, float Width, GLfloat *Color);
		void SetMinMaxCurrent(float MinX, float MaxX, float MinY, float MaxY);
		void SetCursorPosition(CPoint point);
        void SetBorder(double leftBorder, double rightBorder, double topBorder, double bottomBorder);
        void SetParam();
        void SetTextColor(COLORREF color);
        void SetGridColor(COLORREF color);
        void SetLegendColor(COLORREF color);

        void SetStep(double stepX, double stepY);
        double GetStep(GridType gridType);

        void SetScaleType(ScaleType scaleTypeX, ScaleType scaleTypeY);

        void SetScale(double scaleX, double scaleY);
        double GetScale(GridType gridType);

        void SetShift(double shiftX, double shiftY);
        double GetShift(GridType gridType);

        void SetRectAll(CRect leftShiftRect, CRect rightShiftRect,
            CRect bottomShiftRect, CRect topShiftRect, CRect leftScaleRect,
            CRect rightScaleRect, CRect topScaleRect, CRect bottomScaleRect,
            CRect centerAutoScaleRect);

        void SetAxis(double minX, double maxX, double minY, double maxY);
        TwoDouble GetAxis(GridType gridType);

        void SetBounds(double minX, double maxX, double minY, double maxY);
        void SetFit(FitType fitTypeX, FitType fitTypeY);
        void SetAxisTitle(CString titleX, CString titleY);
        Dimention TextDimention(double val);
        Dimention TextDimention(CString val);
		void MakeGrid();
        double FromWorldToScreen(double val, AxisParam axis);
		double FromScreenToWorld(double pixel, AxisParam axis);
		zRect IsMouseInRect(CPoint point);
		void Shift(zRect val);
        void Scale(zRect val);
		void EnableStyle(LineStyle Object);
		void DisableStyle(LineStyle Object);

        // ����������� �������, ���������������� � ������
        virtual void OnDraw();
        virtual void SaveParameters(SimpleXML *pSimpleXML);
        virtual void LoadParameters(SimpleXML *pSimpleXML);
        virtual void OnLButtonDown(UINT nFlags, CPoint point);
        virtual void OnMouseMove(UINT nFlags, CPoint point);
        virtual void OnMouseEnter();
        virtual void OnMouseLeave();
};