//------------------------------------------------------------------------------
// OscillPlot.cpp : файл реализации
//------------------------------------------------------------------------------
#include "stdafx.h"
#include <Interface2012\ZetLab\OscillPlot.h>
//------------------------------------------------------------------------------
COscillPlot::COscillPlot(CCustomContainer *owner, ScaleType hScaleType, ScaleType vScaleType, CRect rect) :
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
        m_pDOscillPlot = new CDOscillPlot(rect, m_pOwner->GetPointerToIsChange(),
            m_pOwner->GetPointerToTime() );
        m_pDClass = m_pDOscillPlot;
    }
}
//------------------------------------------------------------------------------
void COscillPlot::Clear (void)
{
	m_pDOscillPlot->Clear();
}
//------------------------------------------------------------------------------
COscillPlot::~COscillPlot()
{
	m_param.Clear();
    if (m_pDOscillPlot)
	{ 
        delete m_pDOscillPlot;
		m_pDOscillPlot = nullptr;
	}
};
//------------------------------------------------------------------------------
void COscillPlot::Resize(CRect rect)
{
	m_param.m_rect = rect;
	FillPlotArray();
	m_pDOscillPlot->Resize(rect);
}
//------------------------------------------------------------------------------
void COscillPlot::FillPlotArray()
{
    int Counter, Counter1, Length;
	double MinCurrentValue, MaxCurrentValue, MaxCurrent, MinCurrent;
    if(!m_param.pVertexs.empty())
        m_param.pVertexs.clear();
    //SieveArray(m_param.m_axisX.paDataArray, m_param.m_axisY.paDataArray);
	SieveArrayNew(m_param.m_axisX.paDataArray, m_param.m_axisY.paDataArray);

	if (!m_param.m_invertation)
	{
		MinCurrentValue = m_param.m_axisY.paMinCurrentValue;
		MaxCurrentValue = m_param.m_axisY.paMaxCurrentValue;
		MaxCurrent = m_param.m_axisY.paMaxCurrent;
		MinCurrent = m_param.m_axisY.paMinCurrent;
	}
	else
	{
		MinCurrentValue = m_param.m_axisX.paMinCurrentValue;
		MaxCurrentValue = m_param.m_axisX.paMaxCurrentValue;
		MaxCurrent = m_param.m_axisX.paMaxCurrent;
		MinCurrent = m_param.m_axisX.paMinCurrent;
	}

    if(MinCurrentValue > MaxCurrent)
    {
        m_param.pVertexs.push_back(m_param.m_rect.left);
        m_param.pVertexs.push_back(m_param.m_rect.top+2);
        m_param.pVertexs.push_back(5);
        m_param.pVertexs.push_back(m_param.m_rect.right);
        m_param.pVertexs.push_back(m_param.m_rect.top+2);
        m_param.pVertexs.push_back(5);
    }

    if(MaxCurrentValue < MinCurrent)
    {
        m_param.pVertexs.push_back(m_param.m_rect.left);
        m_param.pVertexs.push_back(m_param.m_rect.bottom-2);
        m_param.pVertexs.push_back(5);
        m_param.pVertexs.push_back(m_param.m_rect.right);
        m_param.pVertexs.push_back(m_param.m_rect.bottom-2);
        m_param.pVertexs.push_back(5);
    }

    if(MinCurrentValue < MaxCurrent &&
        MaxCurrentValue > MinCurrent){
        if(m_param.m_axisY.paDrawArray.size()>m_param.m_axisX.paDrawArray.size())
            Length = m_param.m_axisX.paDrawArray.size();
        else 
            Length = m_param.m_axisY.paDrawArray.size();

		double minYLogValue = getWorldScreenPoint(m_param.m_axisY.paMinCurrent, m_param.m_axisY.paScaleType), maxYLogValue = getWorldScreenPoint(m_param.m_axisY.paMaxCurrent, m_param.m_axisY.paScaleType);
        m_param.pVertexs.resize(Length*3);
        Counter1 = 0;
        for(Counter=0; Counter<Length; Counter++)
        {
            m_param.pVertexs[Counter1] = m_param.m_rect.left + m_param.m_axisX.paDrawArray[Counter];
            //m_param.pVertexs[Counter1+1] = m_param.m_rect.bottom - FromWorldToScreen(getWorldScreenPoint(m_param.m_axisY.paDrawArray[Counter], m_param.m_axisY.paScaleType),
            //    minYLogValue,
            //    maxYLogValue, gtVertical,  m_param.m_axisY.paScaleType,
            //    m_param.m_rect);
			m_param.pVertexs[Counter1 + 1] = m_param.m_rect.bottom - getWorldScreenPoint(m_param.m_axisY.paDrawArray[Counter], m_param.m_axisY.paScaleType);
            m_param.pVertexs[Counter1+2] = 10;
            Counter1 += 3;
            /*m_param.pVertexs.push_back(m_param.m_rect.left + m_param.m_axisX.paDrawArray[Counter]);
            m_param.pVertexs.push_back(m_param.m_rect.bottom - FromWorldToScreen(getWorldScreenPoint(m_param.m_axisY.paDrawArray[Counter], m_param.m_axisY.paScaleType),
																				 minYLogValue,
																				 maxYLogValue, gtVertical,  m_param.m_axisY.paScaleType,
																				 m_param.m_rect));
            m_param.pVertexs.push_back(5);*/
        }
    }
    m_pDOscillPlot->SetPlotVertex(m_param.pVertexs);
}
//------------------------------------------------------------------------------
void COscillPlot::SetData(std::vector<double> &DataArrayX, std::vector<double> &DataArrayY,
    int Quality)
{
    CCustomPlot::SetData(DataArrayX, DataArrayY, Quality);
    FillPlotArray();
}
//------------------------------------------------------------------------------
void COscillPlot::SetData(std::vector<double> &DataArrayY, int Quality, bool Invertation)
{
	CCustomPlot::SetData(DataArrayY, Quality, Invertation);
	m_param.m_invertation = Invertation;
    FillPlotArray();
}
//------------------------------------------------------------------------------
void COscillPlot::SetDataFloat(std::vector<float> &DataArrayY, int Quality, bool Invertation)
{
	CCustomPlot::SetDataFloat(DataArrayY, Quality, Invertation);
	m_param.m_invertation = Invertation;
    FillPlotArray();
}
//------------------------------------------------------------------------------
void COscillPlot::SetAxis(double minX, double maxX, double minY, double maxY)
{
    CCustomPlot::SetAxis(minX, maxX, minY, maxY);
    FillPlotArray();
}
//------------------------------------------------------------------------------
void COscillPlot::SieveArray(const std::vector<double> &pArrayX, const std::vector<double> &pArrayY)
{
    double current, next, max, min, counterValue, temp;
    int Counter, Counter1 = 0, PointOfStart, QntOfPoints;
    long Length;
    bool flag = false;
    if(pArrayY.empty() || pArrayX.empty()){
        return;
    }

    m_param.m_axisX.paDrawArray.clear();
    m_param.m_axisY.paDrawArray.clear();
    
    if(m_param.m_axisY.paDataArray.size()>m_param.m_axisX.paDataArray.size())
        Length = m_param.m_axisX.paDataArray.size();
    else 
        Length = m_param.m_axisY.paDataArray.size();

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

	double minXLogValue = getWorldScreenPoint(m_param.m_axisX.paMinCurrent, m_param.m_axisX.paScaleType), maxXLogValue = getWorldScreenPoint(m_param.m_axisX.paMaxCurrent, m_param.m_axisX.paScaleType);

    current = next = FromWorldToScreen(getWorldScreenPoint(pArrayX[PointOfStart], m_param.m_axisX.paScaleType), minXLogValue, 
        maxXLogValue, gtHorizontal, m_param.m_axisX.paScaleType, m_param.m_rect);

    m_param.m_axisY.paMinCurrentValue = pArrayY[PointOfStart];
    m_param.m_axisY.paMaxCurrentValue = pArrayY[PointOfStart];

    for(Counter=PointOfStart; Counter<Length-1; Counter++)
    {
        if(_finite(pArrayY[Counter]) == 0)
            continue;
        if(pArrayY[Counter] < m_param.m_axisY.paMinCurrentValue)
            m_param.m_axisY.paMinCurrentValue = pArrayY[Counter];

        if(pArrayY[Counter] > m_param.m_axisY.paMaxCurrentValue)
            m_param.m_axisY.paMaxCurrentValue = pArrayY[Counter];

		counterValue = next;
        next = FromWorldToScreen(getWorldScreenPoint(pArrayX[Counter+1], m_param.m_axisX.paScaleType), minXLogValue, 
            maxXLogValue, gtHorizontal, m_param.m_axisX.paScaleType, m_param.m_rect);
        if((next - current) > 1.0)
        {
            if(flag)
            {
                m_param.m_axisX.paDrawArray.push_back(temp);
                m_param.m_axisX.paDrawArray.push_back(temp);
                m_param.m_axisY.paDrawArray.push_back(min);
                m_param.m_axisY.paDrawArray.push_back(max);
                flag = false;
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
                if(pArrayY[Counter] > pArrayY[Counter+1]){
                    max = pArrayY[Counter];
                    min = pArrayY[Counter+1];
                }
                else{
                    min = pArrayY[Counter];
                    max = pArrayY[Counter+1];
                }
                temp = counterValue;
                flag = true;
                continue;
            }
            else
            {
                if(pArrayY[Counter+1] > max)
                    max = pArrayY[Counter+1];
                if(pArrayY[Counter+1] < min)
                    min = pArrayY[Counter+1];
            }
        }
    }
	m_param.m_axisX.paDrawArray.push_back(FromWorldToScreen(getWorldScreenPoint(pArrayX[Length-1], m_param.m_axisX.paScaleType), minXLogValue, 
		maxXLogValue, gtHorizontal, m_param.m_axisX.paScaleType, m_param.m_rect));
    m_param.m_axisY.paDrawArray.push_back(pArrayY[Length-1]);

}
//------------------------------------------------------------------------------
void COscillPlot::SieveArrayNew(const std::vector<double> &pArrayX, const std::vector<double> &pArrayY)
{
	int Counter, Counter1 = 0, PointOfStart, QntOfPoints;
	long Length;
	bool flag = false;
	double MinCurrent, MaxCurrent, MinPx, MaxPx;
	ScaleType scaleType;
	std::vector<double> tempX, tempX1;
	std::vector<double>::iterator i;
	std::vector<double> SieveArray, DataArray;

	if (pArrayY.empty() || pArrayX.empty()){
		return;
	}

	if (!m_param.m_invertation)
	{
		SieveArray = pArrayX;
		DataArray = pArrayY;
		MinCurrent = m_param.m_axisX.paMinCurrent;
		MaxCurrent = m_param.m_axisX.paMaxCurrent;
		scaleType = m_param.m_axisX.paScaleType;
		MinPx = m_param.m_rect.left;
		MaxPx = m_param.m_rect.right;
	}

	else
	{
		SieveArray = pArrayY;
		DataArray = pArrayX;
		MinCurrent = m_param.m_axisY.paMinCurrent;
		MaxCurrent = m_param.m_axisY.paMaxCurrent;
		scaleType = m_param.m_axisY.paScaleType;
		MinPx = m_param.m_rect.top;
		MaxPx = m_param.m_rect.bottom;
	}

	m_param.m_axisX.paDrawArray.clear();
	m_param.m_axisY.paDrawArray.clear();

	//if (SieveArray.size() < int(MaxPx - MinPx))
	//{
	//	m_param.m_axisX.paDrawArray
	//}

	if (m_param.m_axisY.paDataArray.size()>m_param.m_axisX.paDataArray.size())
		Length = m_param.m_axisX.paDataArray.size();
	else
		Length = m_param.m_axisY.paDataArray.size();

	PointOfStart = (MinCurrent - m_param.m_startValue) / m_param.m_step;
	QntOfPoints = (MaxCurrent - MinCurrent) / m_param.m_step + 3;

	PointOfStart = abs(PointOfStart);

	if (MinCurrent < m_param.m_startValue)	PointOfStart = 0;

	if (PointOfStart > Length - 1)	PointOfStart = Length - 1;

	if ((PointOfStart + QntOfPoints) < Length)	Length = PointOfStart + QntOfPoints;

	if (QntOfPoints > Length) QntOfPoints = Length;

	m_param.m_firstPoint = PointOfStart;

	double minLogValue = getWorldScreenPoint(MinCurrent, scaleType), maxLogValue = getWorldScreenPoint(MaxCurrent, scaleType);

	int x = (MaxCurrent - MinCurrent) / (MaxPx - MinPx) / m_param.m_step + 1;

	if (x < 1) x = 1;

	tempX.resize(SieveArray.size());
	tempX1.resize(SieveArray.size());

	ippsSubC_64f(SieveArray.data(), minLogValue, tempX.data(), SieveArray.size());
	ippsDivC_64f(tempX.data(), (maxLogValue - minLogValue), tempX1.data(), tempX.size());
	ippsMulC_64f(tempX1.data(), (MaxPx - MinPx), tempX.data(), tempX1.size());

	m_param.m_axisX.paDrawArray.resize(int(2 * (MaxPx - MinPx)));
	m_param.m_axisY.paDrawArray.resize(int(2 * (MaxPx - MinPx)));

	for (Counter = PointOfStart; Counter < Length - 1 - x; Counter += x)
	{
		if (!m_param.m_invertation)
		{
			ippsMinMax_64f(pArrayY.data() + Counter, x, &m_param.m_axisY.paMinCurrentValue, &m_param.m_axisY.paMaxCurrentValue);
			m_param.m_axisX.paDrawArray[Counter1] = tempX[Counter];
			m_param.m_axisX.paDrawArray[Counter1 + 1] = tempX[Counter];
			m_param.m_axisY.paDrawArray[Counter1] = m_param.m_axisY.paMinCurrentValue;
			m_param.m_axisY.paDrawArray[Counter1 + 1] = m_param.m_axisY.paMaxCurrentValue;
			Counter1 += 2;
		}
		else
		{
			ippsMinMax_64f(pArrayX.data() + Counter, x, &m_param.m_axisX.paMinCurrentValue, &m_param.m_axisX.paMaxCurrentValue);
			m_param.m_axisX.paDrawArray[Counter1] = m_param.m_axisX.paMinCurrentValue;
			m_param.m_axisX.paDrawArray[Counter1 + 1] = m_param.m_axisX.paMaxCurrentValue;
			m_param.m_axisY.paDrawArray[Counter1] = tempX[Counter];
			m_param.m_axisY.paDrawArray[Counter1 + 1] = tempX[Counter];
			Counter1 += 2;
		}
	}
	if (!m_param.m_invertation)
	{
		m_param.m_axisX.paDrawArray[Counter1] = tempX[Length - 1];
		m_param.m_axisY.paDrawArray[Counter1] = pArrayY[Length - 1];
		ippsMinMax_64f(pArrayY.data() + PointOfStart, QntOfPoints, &m_param.m_axisY.paMinCurrentValue, &m_param.m_axisY.paMaxCurrentValue);
	}
	else
	{
		m_param.m_axisX.paDrawArray[Counter1] = pArrayX[Length - 1];
		m_param.m_axisY.paDrawArray[Counter1] = tempX[Length - 1];
		ippsMinMax_64f(pArrayX.data() + PointOfStart, QntOfPoints, &m_param.m_axisX.paMinCurrentValue, &m_param.m_axisX.paMaxCurrentValue);
	}
	m_param.m_axisX.paDrawArray.resize(Counter1 + 1);
	m_param.m_axisY.paDrawArray.resize(Counter1 + 1);
	tempX.clear();
	tempX1.clear();
	tempX.resize(Counter1 + 1);
	tempX1.resize(Counter1 + 1);
	if (!m_param.m_invertation)
	{
		tempX.resize(m_param.m_axisY.paDrawArray.size());
		tempX1.resize(m_param.m_axisY.paDrawArray.size());
		ippsSubC_64f(m_param.m_axisY.paDrawArray.data(), getWorldScreenPoint(m_param.m_axisY.paMinCurrent, m_param.m_axisY.paScaleType), tempX.data(), m_param.m_axisY.paDrawArray.size());
		ippsDivC_64f(tempX.data(), (getWorldScreenPoint(m_param.m_axisY.paMaxCurrent, m_param.m_axisY.paScaleType)
			- getWorldScreenPoint(m_param.m_axisY.paMinCurrent, m_param.m_axisY.paScaleType)), tempX1.data(), tempX.size());
		ippsMulC_64f(tempX1.data(), (m_param.m_rect.bottom - m_param.m_rect.top), tempX.data(), tempX1.size());
		m_param.m_axisY.paDrawArray = tempX;
	}
	else
	{
		tempX.resize(m_param.m_axisX.paDrawArray.size());
		tempX1.resize(m_param.m_axisX.paDrawArray.size());
		maxLogValue = getWorldScreenPoint(m_param.m_axisX.paMaxCurrent, m_param.m_axisX.paScaleType);
		minLogValue = getWorldScreenPoint(m_param.m_axisX.paMinCurrent, m_param.m_axisX.paScaleType);
		ippsSubC_64f(m_param.m_axisX.paDrawArray.data(), minLogValue, tempX.data(), m_param.m_axisX.paDrawArray.size());
		ippsDivC_64f(tempX.data(), maxLogValue - minLogValue, tempX1.data(), tempX.size());
		ippsMulC_64f(tempX1.data(), (m_param.m_rect.right - m_param.m_rect.left), tempX.data(), tempX1.size());
		m_param.m_axisX.paDrawArray = tempX;
	}

	/*PointOfStart = (m_param.m_axisX.paMinCurrent - m_param.m_startValue) / m_param.m_step;
	QntOfPoints = (m_param.m_axisX.paMaxCurrent - m_param.m_axisX.paMinCurrent) / m_param.m_step + 3;

	PointOfStart = abs(PointOfStart);

	if (m_param.m_axisX.paMinCurrent < m_param.m_startValue)	PointOfStart = 0;

	if (PointOfStart > Length - 1)	PointOfStart = Length - 1;

	if ((PointOfStart + QntOfPoints) < Length)	Length = PointOfStart + QntOfPoints;

	m_param.m_firstPoint = PointOfStart;

	double minXLogValue = getWorldScreenPoint(m_param.m_axisX.paMinCurrent, m_param.m_axisX.paScaleType), maxXLogValue = getWorldScreenPoint(m_param.m_axisX.paMaxCurrent, m_param.m_axisX.paScaleType);

	int x = (m_param.m_axisX.paMaxCurrent - m_param.m_axisX.paMinCurrent) / (m_param.m_rect.right - m_param.m_rect.left) / m_param.m_step + 1;

	if (x < 1) x = 1;

	tempX.resize(pArrayX.size());
	tempX1.resize(pArrayX.size());

	ippsSubC_64f(pArrayX.data(), minXLogValue, tempX.data(), pArrayX.size());
	ippsDivC_64f(tempX.data(), (maxXLogValue - minXLogValue), tempX1.data(), tempX.size());
	ippsMulC_64f(tempX1.data(), (m_param.m_rect.right - m_param.m_rect.left), tempX.data(), tempX1.size());

	m_param.m_axisX.paDrawArray.resize(int(2 * (m_param.m_rect.right - m_param.m_rect.left)));
	m_param.m_axisY.paDrawArray.resize(int(2 * (m_param.m_rect.right - m_param.m_rect.left)));

	for (Counter = PointOfStart; Counter < Length - 1 - x; Counter += x)
	{
		ippsMinMax_64f(pArrayY.data() + Counter, x, &m_param.m_axisY.paMinCurrentValue, &m_param.m_axisY.paMaxCurrentValue);
		m_param.m_axisX.paDrawArray[Counter1] = tempX[Counter];
		m_param.m_axisX.paDrawArray[Counter1 + 1] = tempX[Counter];
		m_param.m_axisY.paDrawArray[Counter1] = m_param.m_axisY.paMinCurrentValue;
		m_param.m_axisY.paDrawArray[Counter1+1] = m_param.m_axisY.paMaxCurrentValue;
		Counter1+=2;
	}
	m_param.m_axisX.paDrawArray[Counter1] = tempX[Length - 1];
	m_param.m_axisY.paDrawArray[Counter1] = pArrayY[Length - 1];
	m_param.m_axisX.paDrawArray.resize(Counter1+1);
	m_param.m_axisY.paDrawArray.resize(Counter1+1);
	ippsMinMax_64f(pArrayY.data() + PointOfStart, QntOfPoints, &m_param.m_axisY.paMinCurrentValue, &m_param.m_axisY.paMaxCurrentValue);*/
}
//------------------------------------------------------------------------------
void COscillPlot::SaveParameters(SimpleXML *pSimpleXML)
{
    CCustomElement::SaveParameters(pSimpleXML);
}
//------------------------------------------------------------------------------
//double COscillPlot::FromWorldToScreen(double val, double MinCurrent, double MaxCurrent, GridType gridType, ScaleType scaleType, const CRect &rect)
//{
//	if(scaleType == stLog)
//	{
//		double Length;
//		if(gridType == gtHorizontal) Length = rect.Width();
//		if(gridType == gtVertical) Length = rect.Height();
//
//		double logVal = getLog10OfValue(val, m_param.m_step), logMin = getLog10OfValue(MinCurrent, m_param.m_step), logMax = getLog10OfValue(MaxCurrent, m_param.m_step);
//		return (logVal - logMin)/(logMax-logMin)*(double)(Length);
//	}
//
//	return CCustomPlot::FromWorldToScreen(val, MinCurrent, MaxCurrent, gridType, scaleType, rect);
//}
//------------------------------------------------------------------------------
double COscillPlot::GetDataStep()
{
	return m_param.m_step;
}
//-------------------------------------------------------------------------------
double COscillPlot::getLog10OfValue(double val, double minInterval)
{
	if(val < minInterval)
	{
		return log10(minInterval);
	}
	return log10(val);
}
//-------------------------------------------------------------------------------
double COscillPlot::getWorldScreenPoint(double val, ScaleType scaleType)
{
	return scaleType == stLog ? getLog10OfValue(val, m_param.m_step) : val;
}
//-------------------------------------------------------------------------------
void COscillPlot::SetPlotColor(COLORREF color)
{
	m_pDOscillPlot->SetPlotColor(color);
}
//-------------------------------------------------------------------------------
void COscillPlot::SetPlotWidth(float width)
{
	m_pDOscillPlot->SetPlotWidth(width);
}
//-------------------------------------------------------------------------------
void COscillPlot::SetPlotTransparency(float transparency)
{
	m_pDOscillPlot->SetPlotTransparency(transparency);
}
//-------------------------------------------------------------------------------
bool COscillPlot::PlotInRect(CRect rect)
{
	int Counter;
	if(!m_param.m_invertation)
	{
		for(Counter = 0; Counter < m_param.m_axisX.paDrawArray.size() - 1; Counter++)
		{
			//if(PointInRect(CPoint(m_param.m_rect.left + m_param.m_axisX.paDrawArray[Counter], m_param.m_rect.bottom - m_param.m_axisY.paDrawArray[Counter]), &rect))
			if (LineCrossRect(CPoint(m_param.m_rect.left + m_param.m_axisX.paDrawArray[Counter], m_param.m_rect.bottom - m_param.m_axisY.paDrawArray[Counter]),
				CPoint(m_param.m_rect.left + m_param.m_axisX.paDrawArray[Counter + 1], m_param.m_rect.bottom - m_param.m_axisY.paDrawArray[Counter + 1]), &rect))
				return true;
		}
	}

	if (m_param.m_invertation)
	{
		for (Counter = 0; Counter < m_param.m_axisY.paDrawArray.size(); ++Counter)
		{
			if (PointInRect(CPoint(m_param.m_axisX.paDrawArray[Counter], m_param.m_axisY.paDrawArray[Counter]), &rect))
				return true;
		}
	}
	return false;
}
//-------------------------------------------------------------------------------
void COscillPlot::SetCursorPositin(int pointNumber, bool invertation){
}
//-------------------------------------------------------------------------------
void COscillPlot::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_pOwner->OnLButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------
void COscillPlot::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_pOwner->OnRButtonDown(nFlags, point);
}