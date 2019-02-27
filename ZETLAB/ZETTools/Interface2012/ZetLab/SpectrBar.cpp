//------------------------------------------------------------------------------
// OscillPlot.cpp : файл реализации
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "SpectrBar.h"
//------------------------------------------------------------------------------
CSpectrBar::CSpectrBar(CCustomContainer *owner, ScaleType hScaleType, ScaleType vScaleType, CRect rect) :
CCustomPlot(owner)
{
    if (m_pOwner)
    {
        m_pPlotParam = &m_param;
		m_param.m_axisX.paScaleType = hScaleType;
		m_param.m_axisY.paScaleType = vScaleType;
        m_param.m_rect = rect;
        m_param.m_pxStep = FromWorldToScreen(getWorldScreenPoint(m_param.m_step, m_param.m_axisX.paScaleType),
            getWorldScreenPoint(m_param.m_axisX.paMinCurrent, m_param.m_axisX.paScaleType), getWorldScreenPoint(m_param.m_axisX.paMaxCurrent, m_param.m_axisX.paScaleType),
            gtHorizontal, m_param.m_axisX.paScaleType, m_param.m_rect);
        m_pDSpectrBar = new CDSpectrBar(rect, m_pOwner->GetPointerToIsChange(),
            m_pOwner->GetPointerToTime() );
        m_pDClass = m_pDSpectrBar;
    }
}
//------------------------------------------------------------------------------
void CSpectrBar::Clear (void)
{
	m_pDSpectrBar->Clear();
}
//------------------------------------------------------------------------------
CSpectrBar::~CSpectrBar()
{
    if (m_pDSpectrBar)
	{ 
        delete m_pDSpectrBar;
		m_pDSpectrBar = nullptr;
	}
};
//------------------------------------------------------------------------------
void CSpectrBar::Resize(CRect rect)
{
	m_param.m_rect = rect;
	FillPlotArray();
	m_pDSpectrBar->Resize(rect);
}
//------------------------------------------------------------------------------
void CSpectrBar::FillPlotArray()
{
    int Counter, Counter1, Length;
    m_param.pVertexs.clear();
    m_param.pOutlineVertexs.clear();

    double pxStep(0), lastWidth(0);

//  if(m_param.m_axisY.paDataArray.size() == 0)
	if (m_param.m_axisY.paDataArray.empty())
        return;

    SieveArray(m_param.m_axisX.paDataArray, m_param.m_axisY.paDataArray);

    if(m_param.m_axisY.paDrawArray.size()>m_param.m_axisX.paDrawArray.size())
        Length = m_param.m_axisX.paDrawArray.size();
    else 
        Length = m_param.m_axisY.paDrawArray.size();

	m_param.pVertexs.resize(Length*12);

	double minYLogValue = getWorldScreenPoint(m_param.m_axisY.paMinCurrent, m_param.m_axisY.paScaleType), maxYLogValue = getWorldScreenPoint(m_param.m_axisY.paMaxCurrent, m_param.m_axisY.paScaleType);
	double currentX(0), currentY(0), outlineShift(0);
	bool useOutlineShift = true, invisibleInterval, firstVisible = true;

	int leftEdge = m_param.m_rect.left, rightEdge = m_param.m_rect.left + m_param.m_rect.Width();
    Counter1 = 0;
	for(Counter = Length - 1; Counter >= 0; Counter--)
	{
		pxStep = Counter > 0 ?  (m_param.m_axisX.paDrawArray[Counter] - m_param.m_axisX.paDrawArray[Counter - 1]) / 2.0 : pxStep;
		currentX = m_param.m_rect.left + m_param.m_axisX.paDrawArray[Counter];

		invisibleInterval = currentX - lastWidth >= rightEdge || currentX + pxStep <= leftEdge;

		if(!invisibleInterval)
		{
			if(firstVisible)
			{
				if(pxStep >= 10)
				{
					outlineShift = 1;
				}
				else if(pxStep < 2)
				{
					useOutlineShift = false;
				}
				firstVisible = false;
			}

			if(_finite(m_param.m_axisY.paDrawArray[Counter]))
			{
				currentY = m_param.m_rect.bottom - FromWorldToScreen(getWorldScreenPoint(m_param.m_axisY.paDrawArray[Counter], m_param.m_axisY.paScaleType),
																	 minYLogValue, maxYLogValue, gtVertical, m_param.m_axisY.paScaleType, m_param.m_rect);

                m_param.pVertexs[Counter1] = currentX - pxStep + outlineShift;
                m_param.pVertexs[Counter1 + 1] = m_param.m_rect.bottom;
                m_param.pVertexs[Counter1 + 2] = 5;

                m_param.pVertexs[Counter1 + 3] = currentX - pxStep + outlineShift;
                m_param.pVertexs[Counter1 + 4] = currentY;
                m_param.pVertexs[Counter1 + 5] = 5;

                m_param.pVertexs[Counter1 + 6] = currentX + lastWidth - outlineShift;
                m_param.pVertexs[Counter1 + 7] = currentY;
                m_param.pVertexs[Counter1 + 8] = 5;

                m_param.pVertexs[Counter1 + 9] = currentX + lastWidth - outlineShift;
                m_param.pVertexs[Counter1 + 10] = m_param.m_rect.bottom;
                m_param.pVertexs[Counter1 + 11] = 5;

                Counter1 += 12;
			}
		}

		lastWidth = pxStep;
	}
   
    m_pDSpectrBar->SetBarVertex(m_param.pVertexs);
	m_pDSpectrBar->SetOutlineVertex(useOutlineShift? m_param.pVertexs : std::vector<double>());
}
//------------------------------------------------------------------------------
void CSpectrBar::SieveArray(const std::vector<double> &pArrayX, const std::vector<double> &pArrayY)
{
	double current(0.0), next(0.0), max, counterValue(0.0), temp(0.0);
	int Counter(0), Counter1(0), PointOfStart(0), QntOfPoints(0);
	long Length(0);
    bool flag(false);
//  if(pArrayY.size() == 0){
	if (pArrayY.empty()){
        return;
    }

    m_param.m_axisX.paDrawArray.clear();
    m_param.m_axisY.paDrawArray.clear();

    if(pArrayX.size() < pArrayY.size())
        Length = pArrayX.size();
    else 
        Length = pArrayY.size();

    double minXLogValue = getWorldScreenPoint(m_param.m_axisX.paMinCurrent, m_param.m_axisX.paScaleType), maxXLogValue = getWorldScreenPoint(m_param.m_axisX.paMaxCurrent, m_param.m_axisX.paScaleType);

	current = next = FromWorldToScreen(getWorldScreenPoint(pArrayX[0], m_param.m_axisX.paScaleType), minXLogValue, maxXLogValue, gtHorizontal, m_param.m_axisX.paScaleType, m_param.m_rect);

    PointOfStart = (m_param.m_axisX.paMinCurrent - m_param.m_startValue) / m_param.m_step;
    QntOfPoints = (m_param.m_axisX.paMaxCurrent - m_param.m_axisX.paMinCurrent) / m_param.m_step+3;

    PointOfStart = abs(PointOfStart);

    if(m_param.m_axisX.paMinCurrent < m_param.m_startValue)
    {
        PointOfStart = 0;
    }

    if(PointOfStart > Length-1)
        PointOfStart = Length-1;

    if((PointOfStart + QntOfPoints) < Length)
        Length = PointOfStart + QntOfPoints;

    m_param.m_firstPoint = PointOfStart;

    m_param.m_axisY.paMinCurrentValue = pArrayY[PointOfStart];
    m_param.m_axisY.paMaxCurrentValue = pArrayY[PointOfStart];

    for(Counter=PointOfStart; Counter<Length-1; Counter++)
    {
		/*if(_finite(pArrayY[Counter]) == 0)
			continue;*/

		if(_finite(pArrayY[Counter]))
		{
			if(_finite(m_param.m_axisY.paMinCurrentValue) == 0 || pArrayY[Counter] < m_param.m_axisY.paMinCurrentValue)
			{
				m_param.m_axisY.paMinCurrentValue = pArrayY[Counter];
			}
			if(_finite(m_param.m_axisY.paMaxCurrentValue) == 0 || pArrayY[Counter] > m_param.m_axisY.paMaxCurrentValue)
			{
				m_param.m_axisY.paMaxCurrentValue = pArrayY[Counter];
			}
		}

    /*    if(pArrayY[Counter] < m_param.m_axisY.paMinCurrentValue)
            m_param.m_axisY.paMinCurrentValue = pArrayY[Counter];

        if(pArrayY[Counter] > m_param.m_axisY.paMaxCurrentValue)
            m_param.m_axisY.paMaxCurrentValue = pArrayY[Counter];*/

		counterValue = next;
		next = FromWorldToScreen(getWorldScreenPoint(pArrayX[Counter+1], m_param.m_axisX.paScaleType), minXLogValue, maxXLogValue, gtHorizontal, m_param.m_axisX.paScaleType, m_param.m_rect);
        if((next - current) > 1.0)
        {
            if(flag)
            {
                m_param.m_axisX.paDrawArray.push_back(temp);
                m_param.m_axisY.paDrawArray.push_back(max);
                flag = false;
				max = NAN;
                current = next;
                Counter1++;
                continue;
            }
            else
            {
                m_param.m_axisX.paDrawArray.push_back(counterValue);
                m_param.m_axisY.paDrawArray.push_back(pArrayY[Counter]);
                Counter1++;
            }
        }
        else
        {
            if(!flag)
            {
				if(_finite(pArrayY[Counter]) && (_finite(pArrayY[Counter + 1]) == 0 || pArrayY[Counter] > pArrayY[Counter+1]))
                //if(pArrayY[Counter] > pArrayY[Counter+1])
				{
                    max = pArrayY[Counter];
                }
                else
				{
                    max = pArrayY[Counter+1];
                }
                temp = counterValue;
                flag = true;
                continue;
            }
            else
            {
                //if(pArrayY[Counter+1] > max)
				if(_finite(pArrayY[Counter + 1]) && (_finite(max) == 0 || pArrayY[Counter + 1] > max))
                {
					max = pArrayY[Counter+1];
				}
            }
        }
    }
	m_param.m_axisX.paDrawArray.push_back(FromWorldToScreen(getWorldScreenPoint(pArrayX[Length-1], m_param.m_axisX.paScaleType), minXLogValue, 
										  maxXLogValue, gtHorizontal, m_param.m_axisX.paScaleType, m_param.m_rect));
    m_param.m_axisY.paDrawArray.push_back(pArrayY[Length-1]);
}
//------------------------------------------------------------------------------
void CSpectrBar::SetScaleType(ScaleType scaleTypeX, ScaleType scaleTypeY){
    m_param.m_axisX.paScaleType = scaleTypeX;
    m_param.m_axisY.paScaleType = scaleTypeY;
}
//------------------------------------------------------------------------------
void CSpectrBar::SetData(std::vector<double> &DataArrayX, std::vector<double> &DataArrayY,
    int Quality)
{
    CCustomPlot::SetData(DataArrayX, DataArrayY, Quality);
    FillPlotArray();
}
//------------------------------------------------------------------------------
void CSpectrBar::SetData(std::vector<double> &DataArrayY, int Quality, bool Invertation)
{
    /*SetSpace(m_param.m_axisX.paMinCurrent, m_param.m_endValue,
        m_param.m_step);*/
    CCustomPlot::SetData(DataArrayY, Quality);
    FillPlotArray();
}
//------------------------------------------------------------------------------
void CSpectrBar::SetDataFloat(std::vector<float> &DataArrayY, int Quality, bool Invertation)
{
    CCustomPlot::SetDataFloat(DataArrayY, Quality);
    FillPlotArray();
}
//------------------------------------------------------------------------------
void CSpectrBar::SetStep(double stepX){
    m_param.m_step = stepX;
    m_param.m_pxStep = FromWorldToScreen(getWorldScreenPoint(m_param.m_step, m_param.m_axisX.paScaleType),
            getWorldScreenPoint(m_param.m_axisX.paMinCurrent, m_param.m_axisX.paScaleType), getWorldScreenPoint(m_param.m_axisX.paMaxCurrent, m_param.m_axisX.paScaleType),
            gtHorizontal, m_param.m_axisX.paScaleType, m_param.m_rect);
}
//------------------------------------------------------------------------------
void CSpectrBar::SetAxis(double minX, double maxX, double minY, double maxY)
{
    CCustomPlot::SetAxis(minX, maxX, minY, maxY);
    FillPlotArray();
}
//------------------------------------------------------------------------------
void CSpectrBar::SetBounds(double minX, double maxX, double minY, double maxY)
{
    
}
//------------------------------------------------------------------------------
TwoDouble CSpectrBar::GetMinMaxAxisY()
{
    TwoDouble temp;
    temp.Min = m_param.m_axisY.paMinCurrentValue;
    temp.Max = m_param.m_axisY.paMaxCurrentValue;
    return temp;
}
//------------------------------------------------------------------------------
void CSpectrBar::SetFit(FitType fitTypeX, FitType fitTypeY)
{
    
}
//------------------------------------------------------------------------------
void CSpectrBar::Shift(zRect rectType){
}
//------------------------------------------------------------------------------
void CSpectrBar::Scale(zRect rectType){
    
}
//------------------------------------------------------------------------------
void CSpectrBar::SaveParameters(SimpleXML *pSimpleXML)
{
    CCustomElement::SaveParameters(pSimpleXML);
}
//-------------------------------------------------------------------------------
double CSpectrBar::GetDataStep()
{
	return m_param.m_step;
}
//-------------------------------------------------------------------------------
double CSpectrBar::getLog10OfValue(double val, double minInterval)
{
	if(val < minInterval)
	{
		return log10(minInterval);
	}
	return log10(val);
}
//-------------------------------------------------------------------------------
double CSpectrBar::getWorldScreenPoint(double val, ScaleType scaleType)
{
	return scaleType == stLog ? getLog10OfValue(val, m_param.m_step) : val;
}
//-------------------------------------------------------------------------------