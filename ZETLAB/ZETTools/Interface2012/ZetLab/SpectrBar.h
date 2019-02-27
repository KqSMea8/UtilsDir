//------------------------------------------------------------------------------
// SpectrBar.h : ���� ���������
//------------------------------------------------------------------------------
#pragma once
#include <Interface2012\Custom\CustomPlot.h>
#include <Interface2012\Draw\DSpectrBar.h>
//------------------------------------------------------------------------------

struct SpectrBar : public PlotParam
{
    //������ ��� ����������� ������ ��������
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
    //���������� ������� ��� ���������
    void FillPlotArray();
    //������ �����
    void SieveArray(const std::vector<double> &pArrayX, const std::vector<double> &pArrayY);
    //���������� ������� ������� �� ���� � � Y
    void SetAxis(double minX, double maxX, double minY, double maxY);
    //���������� ������� �� ���� � � Y
    void SetBounds(double minX, double maxX, double minY, double maxY);
    //���������� ��� ����� (��������������� ��� ��������)
    void SetScaleType(ScaleType scaleTypeX, ScaleType scaleTypeY);
    //�������� ������ (������ ����� �� ��� � � Y)
    void SetData(std::vector<double> &DataArrayX, std::vector<double> &DataArrayY, int Quality);
     //�������� ������ (������ Y)
	void SetData(std::vector<double> &DataArrayY, int Quality, bool Invertation = false);
    //�������� ������ (������ Y) ��� float
	void SetDataFloat(std::vector<float> &DataArrayY, int Quality, bool Invertation = false);
    //���������� ��� �������
    void SetStep(double stepX);
    //���������� �������� ��� ���������������
    void SetFit(FitType fitTypeX, FitType fitTypeY);
    //�������� ����������� � ������������ �������� ������� �� ��� Y
    TwoDouble GetMinMaxAxisY();
    //�������� ������
    void Shift(zRect rectType);
    //�������������� ������
    void Scale(zRect rectType);
    //��������� ���������� ����������� � XML
    virtual void SaveParameters(SimpleXML *pSimpleXML);
	virtual void Resize(CRect rect) override;
    double GetDataStep() override;
};
//------------------------------------------------------------------------------