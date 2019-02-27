//------------------------------------------------------------------------------
// OscillPlot.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include <Interface2012\Custom\CustomPlot.h>
#include <Interface2012\Draw\DOscillPlot.h>
//------------------------------------------------------------------------------

struct OscillPlot : public PlotParam
{
    OscillPlot()
    {
        m_axisX.paScaleType = stLinear;
        m_axisX.paBegin = -1000;
        m_axisX.paEnd = 1000;
        m_axisX.paGridType = gtHorizontal;
        m_axisX.paMaxCurrent = 100;
        m_axisX.paMinCurrent = 0;
        m_axisX.paFitType = ftCenter;
       
        m_axisY.paScaleType = stLinear;
        m_axisY.paBegin = -100;
        m_axisY.paEnd = 100;
        m_axisY.paGridType = gtVertical;
        m_axisY.paMaxCurrent = 10;
        m_axisY.paMinCurrent = -10;
        m_axisY.paFitType = ftCenter;
        m_axisY.paMinCurrentValue = 0;
        m_axisY.paMaxCurrentValue = 0;

        m_startValue = 0;
        m_endValue = 100;
        m_firstPoint = 0;
        m_lastPoint = m_firstPoint + MAX_QNT_OF_POINTS; //с какой и по какую точку рисовать
        m_step = 0.1;
        m_qntPoints = (m_endValue - m_startValue)/m_step;
		m_invertation = false;
    }
};

class COscillPlot : public CCustomPlot
{
public:
    COscillPlot(CCustomContainer *owner, ScaleType hScaleType, ScaleType vScaleType, CRect rect);
    virtual ~COscillPlot();
	virtual void Resize(CRect rect) override;
	virtual void Clear (void);
private:
    CDOscillPlot *m_pDOscillPlot;
    OscillPlot m_param;

	double getLog10OfValue(double val, double minInterval = 1);
    double getWorldScreenPoint(double val, ScaleType scaleType);

protected:

public:
    //заполнение массива для отрисовки
    void FillPlotArray();
	//преобразование массива данных в массив для отрисовки
    void SieveArray(const std::vector<double> &pArrayX, const std::vector<double> &pArrayY);
	//новая версия (упрощенный алгоритм для равномерных данных и линейного типа графика)
	void SieveArrayNew(const std::vector<double> &pArrayX, const std::vector<double> &pArrayY);
    //установить видимые границы области
    void SetAxis(double minX, double maxX, double minY, double maxY);
    //записать данные (массив точек по оси Х и Y)
    void SetData(std::vector<double> &DataArrayX, std::vector<double> &DataArrayY, int Quality);
    //записать данные (массив Y)
	void SetData(std::vector<double> &DataArrayY, int Quality, bool Invertation = false);
    ///записать данные (в виде float, необходимо для совместимости)
	void SetDataFloat(std::vector<float> &DataArrayY, int Quality, bool Invertation = false);
    //сохранение параметров отображения в XML
    virtual void SaveParameters(SimpleXML *pSimpleXML);
    //получить шаг данных графика
    double GetDataStep() override;
	//установить цвет графика
	virtual void SetPlotColor(COLORREF color) override;
	//
	virtual void SetPlotWidth(float width) override;
	//
	virtual void SetPlotTransparency(float transparency);
	//
	virtual bool PlotInRect(CRect rect) override;
	//
	void SetCursorPositin(int pointNumber, bool invertation = false);
	//
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	//
	virtual void OnRButtonDown(UINT nFlags, CPoint point);
};
//------------------------------------------------------------------------------