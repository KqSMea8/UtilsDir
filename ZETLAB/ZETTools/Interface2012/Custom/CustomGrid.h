//------------------------------------------------------------------------------
// CustomGrid.h : файл заголовка
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
    long zfInteger; //число
    long zfFractional; //степень
    bool zfSign; //знак (true - есть знак "-", false - нет знака)
};


struct AxisParameters{
    double apMaxCurrent, apMinCurrent; //границы видимой области по оси
    int apMaxCurrentMS, apMinCurrentMS; //милисекунды
                                        //(поскольку внутри сетки и графика идет работа с классом COleDataTime,
                                        //необходимо учитывать их отдельно)
    z_float apZMaxCurrent, apZMinCurrent; //границы видимой области по оси в виде x*10^y
    double apBegin, apEnd; //границы отображения
    double apPxStep, //шаг сетки в пикселях
        apStep; //шаг сетки в условных единицах
    TimeInterval apTimeStep; //шаг сетки во временных единицах
    z_float apZStep; //шаг сетки в виде x*10^y
    double apMinStep; //минимальный шаг сетки
    double apScale; //коэффициент масштабирования
    double apShift; //шаг сдвига
	std::vector<double> apMarkingArray; //массив вспомогательных точек-меток
	std::vector<double> apBoldMarkingArray; //массив основных точек-меток
    double apMaxTextLength; //максимальная длина строки, содержащей число
    double apMaxTexthHeight; //максимальная высота
    int apQntMarks, apQntBoldMarks, apQntTextMarks; //количество меток
    ScaleType apScaleType; //тип шкалы
    GridType apGridType; //тип оси
    CString apTitle; //подпись оси
    std::vector<CString> apUnderTextArray, apOverTextArray; //вектор со значениями меток
    std::vector<CRect> apUnderTextRect, apOverTextRect; //вектор с координатами областей для вывода меток
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