//------------------------------------------------------------------------------
// SpectrBar.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include <Interface2012\Custom\CustomPlot.h>
#include <Interface2012\Draw\DSpectrBar.h>
//------------------------------------------------------------------------------

struct SpectrBar : public PlotParam
{
    //массив для отображения кромки столбцов
    std::vector<double> pOutlineVertexs;
    SpectrBar()
    {
        m_axisX.paScaleType = stLinear;
        m_axisX.paBegin = -1000;
        m_axisX.paEnd = 1000;
        m_axisX.paGridType = gtHorizontal;
        m_axisX.paMaxCurrent = 100;
        m_axisX.paMinCurrent = 0;
        m_axisX.paFitType = ftLeft;

        m_axisY.paScaleType = stLinear;
        m_axisY.paBegin = -100;
        m_axisY.paEnd = 100;
        m_axisY.paGridType = gtVertical;
        m_axisY.paMaxCurrent = 10;
        m_axisY.paMinCurrent = -10;
        m_axisY.paFitType = ftCenter;

        m_startValue = 0;
        m_endValue = 100;
        m_firstPoint = 0;
        m_lastPoint = m_firstPoint + MAX_QNT_OF_POINTS;
        m_step = 0.1;
        m_qntPoints = (m_endValue - m_startValue)/m_step;
    }
};

class CSpectrBar : public CCustomPlot
{
public:
    CSpectrBar(CCustomContainer *owner, ScaleType hScaleType, ScaleType vScaleType, CRect rect);
    virtual ~CSpectrBar();
	virtual void Clear (void);
private:
    CDSpectrBar *m_pDSpectrBar;
    SpectrBar m_param;

	double getLog10OfValue(double val, double minInterval = 1);
    double getWorldScreenPoint(double val, ScaleType scaleType);

protected:

public:
    //заполнение массива для отрисовки
    void FillPlotArray();
    //фильтр точек
    void SieveArray(const std::vector<double> &pArrayX, const std::vector<double> &pArrayY);
    //установить видимую область по осям Х и Y
    void SetAxis(double minX, double maxX, double minY, double maxY);
    //установить границы по осям Х и Y
    void SetBounds(double minX, double maxX, double minY, double maxY);
    //установить тип шкалы (логарифмическая или линейная)
    void SetScaleType(ScaleType scaleTypeX, ScaleType scaleTypeY);
    //записать данные (массив точек по оси Х и Y)
    void SetData(std::vector<double> &DataArrayX, std::vector<double> &DataArrayY, int Quality);
     //записать данные (массив Y)
	void SetData(std::vector<double> &DataArrayY, int Quality, bool Invertation = false);
    //записать данные (массив Y) тип float
	void SetDataFloat(std::vector<float> &DataArrayY, int Quality, bool Invertation = false);
    //установить шаг графика
    void SetStep(double stepX);
    //установить привязку при масштабировании
    void SetFit(FitType fitTypeX, FitType fitTypeY);
    //получить минимальные и максимальные значения графика по оси Y
    TwoDouble GetMinMaxAxisY();
    //сдвинуть график
    void Shift(zRect rectType);
    //масштабировать график
    void Scale(zRect rectType);
    //сохранить параментры осображения в XML
    virtual void SaveParameters(SimpleXML *pSimpleXML);
	virtual void Resize(CRect rect) override;
    double GetDataStep() override;
};
//------------------------------------------------------------------------------