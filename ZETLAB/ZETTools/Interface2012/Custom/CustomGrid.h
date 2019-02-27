//------------------------------------------------------------------------------
// CustomGrid.h : ���� ���������
//------------------------------------------------------------------------------
#pragma once
#include <Interface2012\Custom\CustomElement.h>
#include <Interface2012\Custom\Custom_ZETi12.h>
#include <Interface2012\Draw\FunctionsForGraph.h>
#include <vector>
//------------------------------------------------------------------------------
typedef void (CCustom_ZETi12::*NewDoubleEvent)(double);
//------------------------------------------------------------------------------

struct z_float{
	z_float()
	{
		zfInteger = 0;
		zfFractional = 0;
		zfSign = false;
	}
    long zfInteger; //�����
    long zfFractional; //�������
    bool zfSign; //���� (true - ���� ���� "-", false - ��� �����)
};


struct AxisParameters{
    double apMaxCurrent, apMinCurrent; //������� ������� ������� �� ���
    int apMaxCurrentMS, apMinCurrentMS; //�����������
                                        //(��������� ������ ����� � ������� ���� ������ � ������� COleDataTime,
                                        //���������� ��������� �� ��������)
    z_float apZMaxCurrent, apZMinCurrent; //������� ������� ������� �� ��� � ���� x*10^y
    double apBegin, apEnd; //������� �����������
    double apPxStep, //��� ����� � ��������
        apStep; //��� ����� � �������� ��������
    TimeInterval apTimeStep; //��� ����� �� ��������� ��������
    z_float apZStep; //��� ����� � ���� x*10^y
    double apMinStep; //����������� ��� �����
    double apScale; //����������� ���������������
    double apShift; //��� ������
	std::vector<double> apMarkingArray; //������ ��������������� �����-�����
	std::vector<double> apBoldMarkingArray; //������ �������� �����-�����
    double apMaxTextLength; //������������ ����� ������, ���������� �����
    double apMaxTexthHeight; //������������ ������
    int apQntMarks, apQntBoldMarks, apQntTextMarks; //���������� �����
    ScaleType apScaleType; //��� �����
    GridType apGridType; //��� ���
    CString apTitle; //������� ���
    std::vector<CString> apUnderTextArray, apOverTextArray; //������ �� ���������� �����
    std::vector<CRect> apUnderTextRect, apOverTextRect; //������ � ������������ �������� ��� ������ �����
    FitType apFitType;
    double apCurrentTime;
#ifdef _GDIPLUS
#else
    CDFonts apFont;
#endif
	~AxisParameters()
	{
#ifdef _GDIPLUS
#else
		//apFont.Clear();
#endif
		Clear();
	}
	void Clear()
	{
		if (!apUnderTextArray.empty())
			apUnderTextArray.clear();
		if (!apUnderTextRect.empty())
			apUnderTextRect.clear();
	}
};

struct GridParam
{
    CRect m_rect;
    AxisParameters m_axis;
    CPoint m_cursorPosition;
    std::vector<double> pVertexs;
    std::vector<double> pBoldVertexs;

	void Clear()
	{
		m_axis.Clear();
		if (!pVertexs.empty())
			pVertexs.clear();
		if (!pBoldVertexs.empty())
			pBoldVertexs.clear();
	}
};

class CCustomGrid : public CCustomElement
{
public:
    CCustomGrid(CCustomContainer *owner);
    virtual ~CCustomGrid();

private:

protected:

public:
    GridParam *m_pGridParam;
    
    virtual double FromWorldToScreen(double val, const AxisParameters &axis, const CRect &rect);
    virtual CPoint GetTextDimention(CDFonts &font, wchar_t *String);

    virtual void SetBounds(double Begin, double End);

    virtual void SetMinStep(double MinStep);
    virtual void SetFit(FitType fitType);
    virtual void SetAxisTitle(CString title, GridType type);
    virtual void SetCurrentTime(double NewTime);

    virtual void SetAxis(double min, double max);

	virtual double GetAxisAbsMin(GridType gridType);
	virtual double GetAxisAbsMax(GridType gridType);

    virtual double GetAxisMin(GridType gridType);
    virtual double GetAxisMax(GridType gridType);
    virtual double GetStep();

    virtual void Shift(zRect rect){};
    virtual void Scale(zRect rect){};
    virtual void AutoScale(double minValue, double maxValue){};

	virtual void SetGridColor(COLORREF GridColor){};
};
//----------------------------------------