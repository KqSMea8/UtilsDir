//------------------------------------------------------------------------------
// OscillGrid.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "OscillLogGrid.h"
//------------------------------------------------------------------------------
COscillLogGrid::COscillLogGrid(CCustomContainer *owner, CRect rect, GridType gridType) :
CCustomGrid(owner)
{
    if (m_pOwner)
    {
        m_pGridParam = &m_param;
        m_param.m_rect = rect;
        m_param.m_axis.apGridType = gridType;
        unsigned int andMask=1, xorMask=0;
        m_hCursor = CreateCursor(nullptr, 0, 0, 1, 1, &andMask, &xorMask);

        m_param.m_axis.apMaxTextLength = GetTextDimention(m_param.m_axis.apFont, L"0000000").x;
        m_param.m_axis.apMaxTexthHeight = GetTextDimention(m_param.m_axis.apFont, L"0000000").y;

        if(m_param.m_axis.apGridType == gtHorizontal){
            //m_param.m_axis.apQntMarks = QNT_MARKS_X;
            //m_param.m_axis.apPxStep = rect.Width()/(double)(QNT_MARKS_X-1);
            m_param.m_axis.apQntTextMarks = m_param.m_rect.Width() / m_param.m_axis.apMaxTextLength; //количество блоков. т.е. сколько меток с интервалом 1 между ними возможно расставить
        }

        if(m_param.m_axis.apGridType == gtVertical){
            //m_param.m_axis.apQntMarks = QNT_MARKS_Y;
            //m_param.m_axis.apPxStep = rect.Height()/(double)(QNT_MARKS_Y-1);
            m_param.m_axis.apQntTextMarks = m_param.m_rect.Height() / m_param.m_axis.apMaxTexthHeight;
        }

        if(m_param.m_axis.apQntMarks - m_param.m_axis.apQntTextMarks < 0)
            m_param.m_axis.apQntTextMarks = m_param.m_axis.apQntMarks;

        FillGridArray();
		m_pDOscillGrid = new CDOscillGrid(rect, m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime());
		m_pDClass = m_pDOscillGrid;
		//m_pDOscillGrid->SetGridVertex(m_param.pVertexs);
		//m_pDOscillGrid->SetBoldGridVertex(m_param.pBoldVertexs);
		//m_pDOscillGrid->SetTextArray(m_param.m_axis.apTextArray);
		//m_pDOscillGrid->SetTextRect(m_param.m_axis.apTextRect);
    }
}
//------------------------------------------------------------------------------
void COscillLogGrid::Clear (void)
{
	m_pDOscillGrid->Clear();
}
//------------------------------------------------------------------------------

double COscillLogGrid::FromWorldToScreen(double val, const AxisParameters &axis, const CRect &rect)
{
	if(axis.apScaleType == stLog)
	{
		double Length(0.0);
		if(axis.apGridType == gtHorizontal) Length = rect.Width();
		if(axis.apGridType == gtVertical) Length = rect.Height();

		double logVal = getLogValue(val), logMin = getLogValue(axis.apMinCurrent), logMax = getLogValue(axis.apMaxCurrent);
		return (logVal - logMin)/(logMax-logMin)*(double)(Length);
	}
	return CCustomGrid::FromWorldToScreen(val, axis, rect);
}

//------------------------------------------------------------------------------
COscillLogGrid::~COscillLogGrid()
{
	if (m_pDOscillGrid)
	{
		delete m_pDOscillGrid;
		m_pDOscillGrid = nullptr;
	}
}
//------------------------------------------------------------------------------
void COscillLogGrid::SetAxis(double min, double max)
{
    z_float temp;
	double delta(0.0);

   /* if(m_param.m_axis.apGridType == gtHorizontal)
        QntOfMarks = QNT_MARKS_X;
    if(m_param.m_axis.apGridType == gtVertical)
        QntOfMarks = QNT_MARKS_Y;*/
    delta = max - min;
	min = min < m_param.m_axis.apMinStep ? m_param.m_axis.apMinStep : min;
	//max = max < m_param.m_axis.apMinStep ? m_param.m_axis.apMinStep : max;
	// !!! было //17/07/2014
//  max = min + delta; //косяк!!!! 
	// !!! стало 
	if (min >= max) 
		max = min + delta;

	CCustomGrid::SetAxis(min, max);

	/*double logMin = getMinRoundLogValue(min), logMax = getMaxRoundLogValue(max);
	double roundMin = pow(10, logMin), roundMax = pow(10, logMax);

	CCustomGrid::SetAxis(roundMin, roundMax);

	m_param.m_axis.apQntMarks = (int)(logMax - logMin) + 1;
	m_param.m_axis.apQntTextMarks = m_param.m_axis.apQntMarks;*/

    //вычисляем шаг, преобразуем его в число <10 и выбираем ближайший
    //предустановленный шаг (1, 2, 5). Шаг храним в двух вариантах - double
    //и свой формат, где первая часть - это значащие цифры, а вторая - степень
    //десяти. "Свой" формат необходим для корректного отображения цифр
    //и избежания проблем с представлением чисел в формате double
    /*stepX = (max - min)/(QntOfMarks-1);
    Power = log10(stepX);
    if(Power < 0)
        Power = (int)Power - 1;
    iStepX = GetRoundStep((int)Power, stepX);
    m_param.m_axis.apStep = iStepX*pow(10.0, (int)Power);
    m_param.m_axis.apZStep.zfInteger = iStepX;
    m_param.m_axis.apZStep.zfFractional = Power;

    if(m_param.m_axis.apFitType == ftCenter)
    {
        m_param.m_axis.apMinCurrent -= abs(stepX - m_param.m_axis.apStep)
            *(QntOfMarks-1)/2.0;
        m_param.m_axis.apMaxCurrent += abs(stepX - m_param.m_axis.apStep)
            *(QntOfMarks-1)/2.0;   
    }

    if(m_param.m_axis.apFitType == ftLeft)
    {
        m_param.m_axis.apMaxCurrent += abs(stepX - m_param.m_axis.apStep)
            *(QntOfMarks-1);
    }

    if(m_param.m_axis.apFitType == ftRight)
    {
        m_param.m_axis.apMinCurrent -= abs(stepX - m_param.m_axis.apStep)
            *(QntOfMarks-1);
    }*/


    //if(abs(m_param.m_axis.apMinCurrent) < Epsilon){
    //    m_param.m_axis.apZMinCurrent.zfInteger = 0;
    //    m_param.m_axis.apZMinCurrent.zfFractional = 0;
    //}
    //else
    //{
    //    //собираем из своего формата минимальное значение, если его разность
    //    //с текущим минимальным больше, чем константа, то вычисляем его заново.
    //    double tmp = m_param.m_axis.apZMinCurrent.zfInteger*pow(10.0, m_param.m_axis.apZMinCurrent.zfFractional);
    //    if(abs(m_param.m_axis.apMinCurrent - tmp) > Epsilon)
    //    {
    //        Power = log10(abs(m_param.m_axis.apMinCurrent));
    //        if(Power < 0)
    //            Power = (int)Power - 1;
    //        val = floor(m_param.m_axis.apMinCurrent/pow(10.0, (int)Power)+0.5);
    //        m_param.m_axis.apZMinCurrent.zfInteger = val;
    //        m_param.m_axis.apZMinCurrent.zfFractional = (int)Power;
    //        if(m_param.m_axis.apMinCurrent < 0)
    //            m_param.m_axis.apZMinCurrent.zfSign = true;
    //    }
    //}

    m_param.pVertexs.clear();
    m_param.m_axis.apUnderTextArray.clear();
    m_param.m_axis.apUnderTextRect.clear();
    FillGridArray();
    m_pDOscillGrid->SetGridVertex(m_param.pVertexs);
	m_pDOscillGrid->SetBoldGridVertex(m_param.pBoldVertexs);
    m_pDOscillGrid->SetTextArray(m_param.m_axis.apUnderTextArray);
    m_pDOscillGrid->SetTextRect(m_param.m_axis.apUnderTextRect);
}

void COscillLogGrid::SetMinStep(double MinStep)
{
	m_pGridParam->m_axis.apMinStep = MinStep;
	//m_pGridParam->m_axis.apStep = MinStep <= 0 ? 0 : floor(log10(MinStep));
	m_pGridParam->m_axis.apStep = MinStep <= 0 ? 0 : log10(MinStep);
};

//------------------------------------------------------------------------------
//double COscillGrid::GetStep(GridType gridType)
//{
//    if(gridType == gtHorizontal)
//        return m_param.m_axis.apStep;
//    if(gridType == gtVertical)
//        return m_param.m_axisY.apStep;
//}
//------------------------------------------------------------------------------
void COscillLogGrid::SetAxisTitle(CString title, GridType type)
{
    CCustomGrid::SetAxisTitle(title, type);
    m_pDOscillGrid->SetAxisTitle(title);
    if(type == gtHorizontal)
        m_pDOscillGrid->SetTitleRect(CRect(m_param.m_rect.right-40, 
        m_param.m_rect.bottom+20, m_param.m_rect.right,
        m_param.m_rect.bottom+50));

    if(type == gtVertical)
        m_pDOscillGrid->SetTitleRect(CRect(m_param.m_rect.left-50, 
        m_param.m_rect.top-20, m_param.m_rect.left,
        m_param.m_rect.top));
}
//------------------------------------------------------------------------------
//double COscillGrid::GetShift(GridType gridType)
//{
//    if(gridType == gtHorizontal)
//        return m_param.m_axis.apShift;
//    if(gridType == gtVertical)
//        return m_param.m_axisY.apShift;
//}
////------------------------------------------------------------------------------
//void COscillGrid::SetMinStep(double xMinStep, double yMinStep)
//{
//    m_param.m_axis.apMinStep = xMinStep;
//    m_param.m_axisY.apMinStep = yMinStep;
//}
////------------------------------------------------------------------------------
//double COscillGrid::GetScale(GridType gridType)
//{
//    if(gridType == gtHorizontal)
//        return m_param.m_axis.apScale;
//    if(gridType == gtVertical)
//        return m_param.m_axisY.apScale;
//}
//------------------------------------------------------------------------------
void COscillLogGrid::FillGridArray()
{
    m_param.pVertexs.clear();
	m_param.pBoldVertexs.clear();

	/*step = m_param.m_axis.apQntMarks
        / m_param.m_axis.apQntTextMarks;*/

	m_param.m_axis.apQntMarks = 0;
	m_param.m_axis.apUnderTextRect.clear();
	m_param.m_axis.apUnderTextArray.clear();

	_prevMarkTextRect.SetRectEmpty();
	//double curValue = m_param.m_axis.apMaxCurrent, curRoundLogValue = getMinRoundLogValue(curValue);
	double minLogValue = getLogValue(m_param.m_axis.apMinCurrent), maxLogValue = getLogValue(m_param.m_axis.apMaxCurrent), curDiff = m_param.m_axis.apMaxCurrent - m_param.m_axis.apMinCurrent;
	MakeMark(m_param.m_axis.apMaxCurrent, curDiff, false);
	double curLogValue = getMinRoundLogValue(m_param.m_axis.apMaxCurrent);
	if(curLogValue == maxLogValue)
	{
		curLogValue--;
	}

	double lastGridMark = pow(10, curLogValue), pow10;
	FillInnerGrid(m_param.m_axis.apMaxCurrent, lastGridMark, lastGridMark, curDiff);

	while(curLogValue > minLogValue)// && m_param.m_axis.apQntMarks < MAX_QNT_MARKS)
	{
		MakeMark(pow(10, curLogValue), curDiff, true);
		curLogValue--;

		if(curLogValue > minLogValue)
		{
			pow10 = pow(10, curLogValue);
			FillInnerGrid(lastGridMark, pow10, pow10, curDiff);
			lastGridMark = pow10;
		}
	}

	pow10 = pow(10, curLogValue);

	FillInnerGrid(lastGridMark, m_param.m_axis.apMinCurrent, pow10, curDiff);

	MakeMark(m_param.m_axis.apMinCurrent, curDiff, false);

	//for(Counter=0; Counter < m_param.m_axis.apQntMarks; Counter++)
	//{
	//    m_param.m_axis.apMarkingArray[Counter] = m_param.m_axis.apMinCurrent
	//        + m_param.m_axis.apStep*Counter;
	//    //MakeText(m_param.m_axis.apMarkingArray[Counter], gtHorizontal);
	//    MakeText(Counter, m_param.m_axis.apGridType);
	//    if(m_param.m_axis.apGridType == gtHorizontal)
	//        m_param.m_axis.apMarkingArray[Counter] = m_param.m_rect.left
	//        + m_param.m_axis.apPxStep*Counter;
	//    if(m_param.m_axis.apGridType == gtVertical)
	//        m_param.m_axis.apMarkingArray[Counter] = m_param.m_rect.bottom
	//        - m_param.m_axis.apPxStep*Counter;
	//    MakeTextRect(m_param.m_axis.apMarkingArray[Counter], m_param.m_axis.apGridType);
	//}


	//  for(Counter=0; Counter<m_param.m_axis.apQntMarks; Counter++)
	//  {
	//if(m_param.m_axis.apGridType == gtHorizontal){
	//	m_param.m_axis.apMarkingArray[Counter] = m_param.m_rect.left
	//		+ m_param.m_axis.apPxStep*Counter;
	//	m_param.pVertexs.push_back(m_param.m_axis.apMarkingArray[Counter]);
	//	m_param.pVertexs.push_back(m_param.m_rect.bottom);
	//	m_param.pVertexs.push_back(5);
	//	m_param.pVertexs.push_back(m_param.m_axis.apMarkingArray[Counter]);
	//	m_param.pVertexs.push_back(m_param.m_rect.top-1);
	//	m_param.pVertexs.push_back(5);
	//}
	//if(m_param.m_axis.apGridType == gtVertical){
	//	m_param.m_axis.apMarkingArray[Counter] = m_param.m_rect.bottom
	//		- m_param.m_axis.apPxStep*Counter;
	//	m_param.pVertexs.push_back(m_param.m_rect.left);
	//	m_param.pVertexs.push_back(m_param.m_axis.apMarkingArray[Counter]);
	//	m_param.pVertexs.push_back(5);
	//	m_param.pVertexs.push_back(m_param.m_rect.right);
	//	m_param.pVertexs.push_back(m_param.m_axis.apMarkingArray[Counter]);
	//	m_param.pVertexs.push_back(5);
	//}
	//  }
}

void COscillLogGrid::FillInnerGrid(double highVal, double lowVal, double step, double curDiff)
{
	if(step == 0 || (highVal - lowVal) <= step)
	{
		return;
	}
	
	double floorHighVal = floor(highVal / step) * step;
	if(floorHighVal == highVal)
	{
		floorHighVal -= step;
	}

	for(double val = floorHighVal; val > lowVal; val -= step)
	{
		MakeMark(val, curDiff, false);
	}
}

void COscillLogGrid::MakeMark(double val, double curDiff, bool isBold)
{
	std::vector<double> &vertexs = isBold ? m_param.pBoldVertexs : m_param.pVertexs;
	double mark(0.0);
	/*if(m_param.m_axis.apGridType == gtHorizontal) Length = m_param.m_rect.Width();
	if(m_param.m_axis.apGridType == gtVertical) Length = m_param.m_rect.Height();*/
	mark = FromWorldToScreen(val, m_param.m_axis, m_param.m_rect);
	if(m_param.m_axis.apGridType == gtHorizontal)
	{
		if(mark >= 0 && mark <= m_param.m_rect.Width())
		{
			mark += m_param.m_rect.left;
			vertexs.push_back(mark);
			vertexs.push_back(m_param.m_rect.bottom);
			vertexs.push_back(5);
			vertexs.push_back(mark);
			vertexs.push_back(m_param.m_rect.top);
			vertexs.push_back(5);
		}
	}
	if(m_param.m_axis.apGridType == gtVertical)
	{
		if(mark >= 0 && mark <= m_param.m_rect.Height())
		{
			mark = m_param.m_rect.bottom - mark;
			vertexs.push_back(m_param.m_rect.left);
			vertexs.push_back(mark);
			vertexs.push_back(5);
			vertexs.push_back(m_param.m_rect.right);
			vertexs.push_back(mark);
			vertexs.push_back(5);
		}
	}
	m_param.m_axis.apQntMarks++;

	if(MakeTextRect(mark, m_param.m_axis.apGridType))
	{
		MakeText(val, curDiff, m_param.m_axis.apGridType);
	}

	//m_param.m_axis.apMarkingArray[Counter] = m_param.m_axis.apMinCurrent + m_param.m_axis.apStep*Counter;
	//if(m_param.m_axis.apGridType == gtHorizontal)
	//	m_param.m_axis.apMarkingArray[Counter] = m_param.m_rect.left
	//	+ m_param.m_axis.apPxStep*Counter;
	//if(m_param.m_axis.apGridType == gtVertical)
	//	m_param.m_axis.apMarkingArray[Counter] = m_param.m_rect.bottom
	//	- m_param.m_axis.apPxStep*Counter;
	
}

//------------------------------------------------------------------------------
void COscillLogGrid::MakeText(double val, double curDiff, GridType type)
{
	//z_float current;
	//double Power;
	//Power = log10(val);
	//if(Power < 0)
	//	Power = (int)Power - 1;
	////val = floor(val/pow(10.0, Power)+0.5);

	////current.zfInteger = floor(val/pow(10.0, Power)+0.5);
	//current.zfInteger = val/pow(10.0, Power);
	//current.zfFractional = Power;
	//current.zfSign = val < 0 ? true : false;

    //z_float /*begin, step,*/ current;
	CString string(L"");
	
	double powerLog = floor(log10(val)), diffLog = curDiff > 0 ? floor(log10(curDiff)) : 0;
	int zeros= max(powerLog >= 0 ? 0 : -powerLog, diffLog >= 1 ? 0 : ceil(1 - diffLog));
	
	string.Format(L"%.*f", zeros, val);

 // /*  begin = m_param.m_axis.apZMinCurrent;
 //   step = m_param.m_axis.apZStep;*/
 //   
 //   //current.zfSign = false;

 //   ////если степень шага меньше, чем степень стартового значения,
 //   ////то приводим степень к наименьшей
 //   //if(begin.zfFractional > step.zfFractional)
 //   //{
 //   //    current.zfFractional = step.zfFractional;
 //   //    current.zfInteger = begin.zfInteger*pow(10.0, (begin.zfFractional -  step.zfFractional)) + step.zfInteger*Counter;
 //   //    if(begin.zfSign)
 //   //        current.zfSign = true;
 //   //}

 //   //else
 //   //{
 //   //    current.zfFractional = begin.zfFractional;
 //   //    current.zfInteger = Counter*step.zfInteger*pow(10.0, (step.zfFractional -  begin.zfFractional)) + begin.zfInteger;
 //   //    if(begin.zfSign)
 //   //        current.zfSign = true;
 //   //}

 //   //формирование строки в зависимости от значений целой части и степени
 //   //выводится только целая часть, если степень равна 0 или если целая часть
 //   //равна 0
 //   if(abs(current.zfFractional) == 0 || abs(current.zfInteger) == 0)
 //   {
 //       sprintf_s(str, "%d", current.zfInteger);
 //   }

 //   //целая часть отлична от нуля, степень полжительна, т.е. в строке целове число
 //   //с количеством нулей, равным числу степени
 //   if(current.zfFractional > 0 && abs(current.zfInteger) != 0)
 //   {
 //       sprintf_s(str, "%d%0*d", current.zfInteger, current.zfFractional, 0);
 //   }

 //   //целая часть отлична от нуля, степень отрицательна, т.е. в строке число
 //   //с плавающей запятой.
 //   if(current.zfFractional < 0 && abs(current.zfInteger) != 0)
 //   {
 //       i = current.zfInteger/(int)pow(10.0, abs(current.zfFractional)); //чило до точки
 //       f = abs(current.zfInteger%(int)pow(10.0, abs(current.zfFractional))); //число после запятой
 //       if(f%10 == 0){
 //           f = f/10;
 //           current.zfFractional += 1;
 //       }
 //       if(f == 0)
 //           sprintf_s(str, "%d", i);
 //       else
 //       {
 //           if(current.zfInteger < 0)
 //               sprintf_s(str, "-%d.%0*d", i, abs(current.zfFractional), f);
 //           else
 //               sprintf_s(str, "%d.%0*d", i, abs(current.zfFractional), f);
 //       }
 //   }
 //  
	//string = str;
    //for(int Counter2=0; Counter2<strlen(str); Counter2++)
    //    string = string + str[Counter2];
    m_param.m_axis.apUnderTextArray.push_back(string);
}
//------------------------------------------------------------------------------
bool COscillLogGrid::MakeTextRect(double val, GridType type)
{
	bool result = false;
	CRect rect(0, 0, 0, 0);
	if(type == gtHorizontal)
    {
        rect.left = val - m_param.m_axis.apMaxTextLength/2.0;
        rect.right = val + m_param.m_axis.apMaxTextLength/2.0;
        rect.top = m_param.m_rect.bottom + 8;
        rect.bottom = m_param.m_rect.bottom + 8 + m_param.m_axis.apMaxTexthHeight;

		if(_prevMarkTextRect.IsRectEmpty() || rect.left > _prevMarkTextRect.right || rect.right < _prevMarkTextRect.left)
		{
			m_param.m_axis.apUnderTextRect.push_back(rect);
			_prevMarkTextRect = rect;
			result = true;
		}
    }

    if(type == gtVertical)
    {
        rect.left = m_param.m_rect.left - m_param.m_axis.apMaxTextLength;
        rect.right = m_param.m_rect.left - 10;
        rect.top = val - m_param.m_axis.apMaxTexthHeight/2.0;
        rect.bottom = val + m_param.m_axis.apMaxTexthHeight/2.0;
       
		if(_prevMarkTextRect.IsRectEmpty() || rect.top > _prevMarkTextRect.bottom || rect.bottom < _prevMarkTextRect.top)
		{
			m_param.m_axis.apUnderTextRect.push_back(rect);
			_prevMarkTextRect = rect;
			result = true;
		}
    }

	return result;
}
//------------------------------------------------------------------------------
void COscillLogGrid::Shift(zRect rect)
{
	double min, max, minLogValue = getLogValue(m_param.m_axis.apMinCurrent), maxLogValue = getLogValue(m_param.m_axis.apMaxCurrent), diffValue = maxLogValue - minLogValue;
	double shiftValue = diffValue * (1 - LOG_GRID_SCALE_STEP);
	if(rect == gLeftShift || rect == gBottomShift)
	{
		double newMinLogValue = minLogValue - shiftValue, newMaxLogValue = maxLogValue - shiftValue;
		double beginLogValue = getLogValue(m_param.m_axis.apBegin);

		if(newMinLogValue < beginLogValue)
		{
			newMinLogValue = beginLogValue;
			newMaxLogValue = newMinLogValue + diffValue;
		}

		min = pow(10, newMinLogValue), max = pow(10, newMaxLogValue);


		/*min = m_param.m_axis.apMinCurrent - m_param.m_axis.apStep;
		if(min < m_param.m_axis.apBegin)
			return;

		if(m_param.m_axis.apZMinCurrent.zfFractional 
			>= m_param.m_axis.apZStep.zfFractional)
		{
			m_param.m_axis.apZMinCurrent.zfInteger =
				m_param.m_axis.apZMinCurrent.zfInteger
				*pow(10.0, (m_param.m_axis.apZMinCurrent.zfFractional 
				-  m_param.m_axis.apZStep.zfFractional))
				- m_param.m_axis.apZStep.zfInteger;
			m_param.m_axis.apZMinCurrent.zfFractional = m_param.m_axis.apZStep.zfFractional;
		}

		else
		{
			m_param.m_axis.apZMinCurrent.zfInteger
				= m_param.m_axis.apZMinCurrent.zfInteger -m_param.m_axis.apZStep.zfInteger
				*pow(10.0, (m_param.m_axis.apZStep.zfFractional
				-  m_param.m_axis.apZMinCurrent.zfFractional));
			m_param.m_axis.apZMinCurrent.zfFractional = m_param.m_axis.apZMinCurrent.zfFractional;
		}

		max = m_param.m_axis.apMaxCurrent - m_param.m_axis.apStep;*/

	}

	if(rect == gRightShift || rect == gTopShift)
	{
		double newMinLogValue = minLogValue + shiftValue, newMaxLogValue = maxLogValue + shiftValue;
		double endLogValue = getLogValue(m_param.m_axis.apEnd);

		if(newMaxLogValue > endLogValue)
		{
			newMaxLogValue = endLogValue;
			newMinLogValue = newMaxLogValue - diffValue;
		}

		min = pow(10, newMinLogValue), max = pow(10, newMaxLogValue);

		/*min = m_param.m_axis.apMinCurrent + m_param.m_axis.apStep;
		max = m_param.m_axis.apMaxCurrent + m_param.m_axis.apStep;
		if(max > m_param.m_axis.apEnd)
			return;

		if(m_param.m_axis.apZMinCurrent.zfFractional 
			> m_param.m_axis.apZStep.zfFractional)
		{
			m_param.m_axis.apZMinCurrent.zfInteger =
				m_param.m_axis.apZMinCurrent.zfInteger
				*pow(10.0, (m_param.m_axis.apZMinCurrent.zfFractional 
				-  m_param.m_axis.apZStep.zfFractional))
				+ m_param.m_axis.apZStep.zfInteger;
			m_param.m_axis.apZMinCurrent.zfFractional = m_param.m_axis.apZStep.zfFractional;
		}

		else
		{
			m_param.m_axis.apZMinCurrent.zfInteger
				= m_param.m_axis.apZStep.zfInteger
				*pow(10.0, (m_param.m_axis.apZStep.zfFractional
				-  m_param.m_axis.apZMinCurrent.zfFractional))
				+ m_param.m_axis.apZMinCurrent.zfInteger;
			m_param.m_axis.apZMinCurrent.zfFractional = m_param.m_axis.apZMinCurrent.zfFractional;
		}*/
	}

	if(rect == gNone)
		return;

	SetAxis(min, max);
}
//------------------------------------------------------------------------------
void COscillLogGrid::Scale(zRect rect)
{
	//double stepX, stepY, Power, temp;
	double min = m_param.m_axis.apMinCurrent, max = m_param.m_axis.apMaxCurrent;
	double minLogValue = getLogValue(m_param.m_axis.apMinCurrent), maxLogValue = getLogValue(m_param.m_axis.apMaxCurrent), diffValue = maxLogValue - minLogValue;

	//int QntOfMarks;

	/*if(m_param.m_axis.apGridType == gtHorizontal)
		QntOfMarks = QNT_MARKS_X;
	if(m_param.m_axis.apGridType == gtVertical)
		QntOfMarks = QNT_MARKS_Y;*/

	if(rect == gLeftScale || rect == gBottomScale)
	{
		double newDiffValue = diffValue / LOG_GRID_SCALE_STEP, sideDiffValue = (newDiffValue - diffValue) / 2.0;
		double newMinLogValue = minLogValue - sideDiffValue, newMaxLogValue = maxLogValue + sideDiffValue;

		double beginLogValue = getLogValue(m_param.m_axis.apBegin), endLogValue = getLogValue(m_param.m_axis.apEnd);

		if(newMinLogValue < beginLogValue)
		{
			newMinLogValue = beginLogValue;
			newMaxLogValue = newMinLogValue + newDiffValue;
			if(newMaxLogValue > endLogValue)
			{
				newMaxLogValue = endLogValue;
			}
		}
		else if(newMaxLogValue > endLogValue)
		{
			newMaxLogValue = endLogValue;
			newMinLogValue = newMaxLogValue - newDiffValue;
			if(newMinLogValue < beginLogValue)
			{
				newMinLogValue = beginLogValue;
			}
		}

		min = pow(10, newMinLogValue), max = pow(10, newMaxLogValue);

		/*stepX = m_param.m_axis.apStep;
		Power = log10(stepX);
		if(Power < 0)
			Power = (int)Power - 1;
		stepX = floor(stepX/pow(10.0, (int)Power)+0.5);
		if(stepX>5)
			stepX = stepX/10.0;
		if((int)stepX == 1)
			temp = 2*pow(10.0, (int)Power);
		if((int)stepX == 2)
			temp = 5*pow(10.0, (int)Power);
		if((int)stepX == 5)
			temp = 10*pow(10.0, (int)Power);

		if(m_param.m_axis.apFitType == ftCenter)
		{
			min = m_param.m_axis.apMinCurrent - abs(temp - m_param.m_axis.apStep)
				*(QntOfMarks-1)/2;
			max = m_param.m_axis.apMaxCurrent + abs(temp - m_param.m_axis.apStep)
				*(QntOfMarks-1)/2;
			if(min < m_param.m_axis.apBegin || max > m_param.m_axis.apEnd)
				return;
		}
		if(m_param.m_axis.apFitType == ftLeft)
		{
			max = m_param.m_axis.apMaxCurrent + abs(temp - m_param.m_axis.apStep)
				*(QntOfMarks-1);
			if(max > m_param.m_axis.apEnd)
				return;
		}
		if(m_param.m_axis.apFitType == ftRight)
		{
			min = m_param.m_axis.apMinCurrent - abs(temp - m_param.m_axis.apStep)
				*(QntOfMarks-1);
			if(min < m_param.m_axis.apBegin)
				return;
		}*/
	}

	if(rect == gRightScale || rect == gTopScale)
	{
		double newDiffValue = diffValue * LOG_GRID_SCALE_STEP, sideDiffValue = (diffValue - newDiffValue) / 2.0;

		min = pow(10, minLogValue + sideDiffValue), max = pow(10, maxLogValue - sideDiffValue);

		if(max - min < m_param.m_axis.apMinStep * 10)
		{
			return;
		}

		/*stepX = m_param.m_axis.apStep;
		Power = log10(stepX);
		if(Power < 0)
			Power = (int)Power - 1;
		stepX = floor(stepX/pow(10.0, (int)Power)+0.5);
		if(stepX>5){
			stepX = stepX/10.0;
			Power = Power + 1;
		}
		if((int)stepX == 1)
			temp = 0.5*pow(10.0, (int)Power);
		if((int)stepX == 2)
			temp = pow(10.0, (int)Power);
		if((int)stepX == 5)
			temp = 2*pow(10.0, (int)Power);

		if(temp < m_param.m_axis.apMinStep)
			return;

		if(m_param.m_axis.apFitType == ftCenter)
		{
			min = m_param.m_axis.apMinCurrent + abs(temp - m_param.m_axis.apStep)
				*(QntOfMarks-1)/2.0;
			max = m_param.m_axis.apMaxCurrent - abs(temp - m_param.m_axis.apStep)
				*(QntOfMarks-1)/2.0;

		}
		if(m_param.m_axis.apFitType == ftLeft)
		{
			max = m_param.m_axis.apMaxCurrent - abs(temp - m_param.m_axis.apStep)
				*(QntOfMarks-1);
		}
		if(m_param.m_axis.apFitType == ftRight)
		{
			min = m_param.m_axis.apMinCurrent + abs(temp - m_param.m_axis.apStep)
				*(QntOfMarks-1);
		}*/
	}

	if(rect == gCenterAutoScale)
	{
		return;
	}

	if(rect == gNone)
		return;

	SetAxis(min, max);
}
//------------------------------------------------------------------------------
void COscillLogGrid::AutoScale(double minValue, double maxValue)
{
	/*double stepX;
	double Power;
	int iStep, iMinValue, iMaxValue, temp;

	stepX = (maxValue - minValue)/(QNT_MARKS_Y-1);
	Power = log10(stepX);
	if(Power < 0)
		Power = (int)Power - 1;
	iStep = GetRoundStep((int)Power, stepX);
	if(abs(minValue) < Epsilon)
		iMinValue = 0;
	else
		iMinValue = GetRoundStep((int)Power, abs(minValue));
	if(minValue<0)
		iMinValue = 0 - iMinValue;
	switch(iStep)
	{
	case 1:
		temp = 2;
		break;

	case 2:
		temp = 5;
		break;

	case 5:
		temp = 10;
		break;

	default:
		temp = 1;
		Power=Power+1;
		break;
	}
	iMinValue = iMinValue - temp;
	iMaxValue = iMinValue + 8*temp;*/

	//SetAxis(iMinValue*pow(10.0, (int)Power), iMaxValue*pow(10.0, (int)Power));
	SetAxis(minValue, maxValue);
}
//------------------------------------------------------------------------------
//float COscillLogGrid::GetRoundStep(int Pow, double val)
//{
//    val = floor(val/pow(10.0, Pow)+0.5);
//    if(val<=1){
//        return 1;
//    }
//    if(val<=2)
//    {
//        return 2;
//    }
//    if(val<=5)
//    {
//        return 5;
//    }
//    else
//    {
//        return 10;
//    }
//}
//------------------------------------------------------------------------------

double COscillLogGrid::getLogValue(double val)
{
	double result = log10(val);
	return result < m_param.m_axis.apStep ? m_param.m_axis.apStep : result;
}
double COscillLogGrid::getMinRoundLogValue(double val)
{
	double result = floor(log10(val));
	return result < m_param.m_axis.apStep ? m_param.m_axis.apStep : result;
}
double COscillLogGrid::getMaxRoundLogValue(double val)
{
	double result = ceil(log10(val));
	return result < m_param.m_axis.apStep ? m_param.m_axis.apStep : result;
}

//double COscillLogGrid::getMinRoundValueByLog(double val)
//{
//	return pow(10, getMinRoundLogValue(val));
//}
//double COscillLogGrid::getMaxRoundValueByLog(double val)
//{
//	return pow(10, getMaxRoundLogValue(val));
//}

void COscillLogGrid::OnLButtonDown(UINT nFlags, CPoint point)
{
    if(m_param.m_axis.apGridType == gtVertical)
        m_pOwner->OnLButtonDown(nFlags, point);
}