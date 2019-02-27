//------------------------------------------------------------------------------
// OscillGrid.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include <Interface2012\ZetLab\OscillGrid.h>
//------------------------------------------------------------------------------
COscillGrid::COscillGrid(CCustomContainer *owner, CRect rect, GridType gridType) :
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
            m_param.m_axis.apQntMarks = QNT_MARKS_X;
            m_param.m_axis.apPxStep = rect.Width()/(double)(QNT_MARKS_X-1);
			m_param.m_axis.apQntTextMarks = QNT_MARKS_X;//m_param.m_rect.Width() / m_param.m_axis.apMaxTextLength; //количество блоков. т.е. сколько меток с интервалом 1 между ними возможно расставить
            
        }

        if(m_param.m_axis.apGridType == gtVertical){
            m_param.m_axis.apQntMarks = QNT_MARKS_Y;
            m_param.m_axis.apPxStep = rect.Height()/(double)(QNT_MARKS_Y-1);
            m_param.m_axis.apQntTextMarks = QNT_MARKS_Y; //m_param.m_rect.Height() / m_param.m_axis.apMaxTexthHeight;
        }

        if(m_param.m_axis.apQntMarks - m_param.m_axis.apQntTextMarks < 0)
            m_param.m_axis.apQntTextMarks = m_param.m_axis.apQntMarks;

        FillGridArray();
        m_pDOscillGrid = new CDOscillGrid(rect, m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime());
        m_pDClass = m_pDOscillGrid;
        m_pDOscillGrid->SetGridVertex(m_param.pVertexs);
        m_pDOscillGrid->SetTextArray(m_param.m_axis.apUnderTextArray);
        m_pDOscillGrid->SetTextRect(m_param.m_axis.apUnderTextRect);
        if(m_param.m_axis.apGridType == gtVertical){
            m_pDOscillGrid->SetAligmentHorizontal(StrAlignmentFar);
        }
    }
}
//------------------------------------------------------------------------------
void COscillGrid::Clear (void)
{
	m_pDOscillGrid->Clear();
}
//------------------------------------------------------------------------------
COscillGrid::~COscillGrid()
{
	if (m_pDOscillGrid)
	{ 
		delete m_pDOscillGrid;
		m_pDOscillGrid = nullptr;
	}
}
//------------------------------------------------------------------------------
void COscillGrid::Resize(CRect rect)
{
	m_param.m_rect = rect;
	m_pDOscillGrid->Resize(rect);
	if (m_param.m_axis.apGridType == gtHorizontal){
		m_param.m_axis.apPxStep = rect.Width() / (double)(QNT_MARKS_X - 1);
	}

	if (m_param.m_axis.apGridType == gtVertical){
		m_param.m_axis.apPxStep = rect.Height() / (double)(QNT_MARKS_Y - 1);
	}
	m_param.pVertexs.clear();
	m_param.m_axis.apUnderTextArray.clear();
	m_param.m_axis.apUnderTextRect.clear();
	FillGridArray();
	m_pDOscillGrid->SetGridVertex(m_param.pVertexs);
	m_pDOscillGrid->SetTextArray(m_param.m_axis.apUnderTextArray);
	m_pDOscillGrid->SetTextRect(m_param.m_axis.apUnderTextRect);
	
	if (m_param.m_axis.apGridType == gtHorizontal)
		m_pDOscillGrid->SetTitleRect(CRect(m_param.m_rect.right - 40,
		m_param.m_rect.bottom + 20, m_param.m_rect.right,
		m_param.m_rect.bottom + 50));

	if (m_param.m_axis.apGridType == gtVertical)
		m_pDOscillGrid->SetTitleRect(CRect(m_param.m_rect.left - 50,
		m_param.m_rect.top - 20, m_param.m_rect.left,
		m_param.m_rect.top));
}
//------------------------------------------------------------------------------
void COscillGrid::SetAxis(double min, double max)
{
	double stepX(0.0), Power(0.0);
    int iStepX(0), val(0), QntOfMarks(0);
    z_float temp;

    CCustomGrid::SetAxis(min, max);

    if(m_param.m_axis.apGridType == gtHorizontal)
        QntOfMarks = QNT_MARKS_X;
    if(m_param.m_axis.apGridType == gtVertical)
        QntOfMarks = QNT_MARKS_Y;
    
    //вычисляем шаг, преобразуем его в число <10 и выбираем ближайший
    //предустановленный шаг (1, 2, 5). Шаг храним в двух вариантах - double
    //и свой формат, где первая часть - это значащие цифры, а вторая - степень
    //десяти. "Свой" формат необходим для корректного отображения цифр
    //и избежания проблем с представлением чисел в формате double
	stepX = (m_param.m_axis.apMaxCurrent - m_param.m_axis.apMinCurrent) / (QntOfMarks - 1);
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
    }


    //if(abs(m_param.m_axis.apMinCurrent) < Epsilon){
    if(abs(m_param.m_axis.apMinCurrent) < pow(10.0, int(Power))){
        m_param.m_axis.apZMinCurrent.zfInteger = 0;
        m_param.m_axis.apZMinCurrent.zfFractional = 0;
        m_param.m_axis.apMinCurrent = 0;
    }
    else
    {
        //собираем из своего формата минимальное значение, если его разность
        //с текущим минимальным больше, чем константа, то вычисляем его заново.
        double tmp = m_param.m_axis.apZMinCurrent.zfInteger*pow(10.0, m_param.m_axis.apZMinCurrent.zfFractional);
		/*
		при перемещении графика с помощью функции shift при прохождении графика через "0" имелись искажения подписей в сетке 
		(т.е. в место 0 писалось значение 0.000006) в связи с чем был введен дополнительный код для сравнени порядков шага сетки 
		и разницей m_param.m_axis.apMinCurrent - tmp. 
		*/
        if(abs(m_param.m_axis.apMinCurrent - tmp) > Epsilon && log10(abs(m_param.m_axis.apMinCurrent - tmp)) >= m_param.m_axis.apZStep.zfFractional)//
        {
            Power = log10(abs(m_param.m_axis.apMinCurrent));
            if(Power < 0)
                Power = (int)Power - 1;
            /////////////////////////////////////////////
            if(log10(abs(m_param.m_axis.apStep)) < Power)
                Power = log10(abs(m_param.m_axis.apStep));
            if(Power < 0)
                Power = (int)Power - 1;
            ////////////////////////////////////////////
            val = floor(m_param.m_axis.apMinCurrent/pow(10.0, (int)Power)+0.5);
            m_param.m_axis.apZMinCurrent.zfInteger = val;
            m_param.m_axis.apZMinCurrent.zfFractional = (int)Power;
            if(m_param.m_axis.apMinCurrent < 0)
                m_param.m_axis.apZMinCurrent.zfSign = true;
            m_param.m_axis.apMinCurrent = m_param.m_axis.apZMinCurrent.zfInteger*pow(10.0, m_param.m_axis.apZMinCurrent.zfFractional); //правка(!)
            m_param.m_axis.apMaxCurrent = m_param.m_axis.apZMinCurrent.zfInteger*pow(10.0, m_param.m_axis.apZMinCurrent.zfFractional)
                + (QntOfMarks-1)*m_param.m_axis.apZStep.zfInteger*pow(10.0, m_param.m_axis.apZStep.zfFractional);
        }
    }

    m_param.pVertexs.clear();
    m_param.m_axis.apUnderTextArray.clear();
    m_param.m_axis.apUnderTextRect.clear();
    FillGridArray();
    m_pDOscillGrid->SetGridVertex(m_param.pVertexs);
    m_pDOscillGrid->SetTextArray(m_param.m_axis.apUnderTextArray);
    m_pDOscillGrid->SetTextRect(m_param.m_axis.apUnderTextRect);
}
//------------------------------------------------------------------------------
void COscillGrid::SetAxisTitle(CString title, GridType type)
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
//void COscillGrid::SetMinStep(double xMinStep, double yMinStep)
//{
//    m_param.m_axis.apMinStep = xMinStep;
//    m_param.m_axisY.apMinStep = yMinStep;
//}
//------------------------------------------------------------------------------
void COscillGrid::FillGridArray()
{
    int Counter, step;
    m_param.pVertexs.clear();


    step = m_param.m_axis.apQntMarks
        / m_param.m_axis.apQntTextMarks;
	m_param.m_axis.apMarkingArray.resize(m_param.m_axis.apQntMarks);
    for(Counter=0; Counter<m_param.m_axis.apQntMarks; Counter=Counter+step)
    {
        m_param.m_axis.apMarkingArray[Counter] = m_param.m_axis.apMinCurrent
            + m_param.m_axis.apStep*Counter;
        //MakeText(m_param.m_axis.apMarkingArray[Counter], gtHorizontal);
        MakeText(Counter, m_param.m_axis.apGridType);
        if(m_param.m_axis.apGridType == gtHorizontal)
            m_param.m_axis.apMarkingArray[Counter] = m_param.m_rect.left
            + m_param.m_axis.apPxStep*Counter;
        if(m_param.m_axis.apGridType == gtVertical)
            m_param.m_axis.apMarkingArray[Counter] = m_param.m_rect.bottom
            - m_param.m_axis.apPxStep*Counter;
        MakeTextRect(m_param.m_axis.apMarkingArray[Counter], m_param.m_axis.apGridType);
    }


    for(Counter=0; Counter<m_param.m_axis.apQntMarks; Counter++)
    {
        if(m_param.m_axis.apGridType == gtHorizontal){
            m_param.m_axis.apMarkingArray[Counter] = m_param.m_rect.left
            + m_param.m_axis.apPxStep*Counter;
            m_param.pVertexs.push_back(m_param.m_axis.apMarkingArray[Counter]);
            m_param.pVertexs.push_back(m_param.m_rect.bottom);
            m_param.pVertexs.push_back(1);
            m_param.pVertexs.push_back(m_param.m_axis.apMarkingArray[Counter]);
            m_param.pVertexs.push_back(m_param.m_rect.top);
            m_param.pVertexs.push_back(1);
        }
        if(m_param.m_axis.apGridType == gtVertical){
            m_param.m_axis.apMarkingArray[Counter] = m_param.m_rect.bottom
            - m_param.m_axis.apPxStep*Counter;
            m_param.pVertexs.push_back(m_param.m_rect.left);
            m_param.pVertexs.push_back(m_param.m_axis.apMarkingArray[Counter]);
            m_param.pVertexs.push_back(1);
            m_param.pVertexs.push_back(m_param.m_rect.right);
            m_param.pVertexs.push_back(m_param.m_axis.apMarkingArray[Counter]);
            m_param.pVertexs.push_back(1);
        }
    }

	SieveTextMarks();
}
//------------------------------------------------------------------------------
void COscillGrid::MakeText(int Counter, GridType type)
{
    z_float begin, step, current;
	char str[32] =  "" ;
	CString string(L"");
    int i, f;

    begin = m_param.m_axis.apZMinCurrent;
    step = m_param.m_axis.apZStep;
    
	current.zfFractional = 1;
	current.zfInteger = 1;
    current.zfSign = false;

    //если степень шага меньше, чем степень стартового значения,
    //то приводим степень к наименьшей
    if(begin.zfFractional > step.zfFractional)
    {
        current.zfFractional = step.zfFractional;
        current.zfInteger = begin.zfInteger*pow(10.0, (begin.zfFractional -  step.zfFractional)) + step.zfInteger*Counter;
        if(begin.zfSign)
            current.zfSign = true;
    }

    else
    {
        current.zfFractional = begin.zfFractional;
        current.zfInteger = Counter*step.zfInteger*pow(10.0, (step.zfFractional -  begin.zfFractional)) + begin.zfInteger;
        if(begin.zfSign)
            current.zfSign = true;
    }

    //формирование строки в зависимости от значений целой части и степени
    //выводится только целая часть, если степень равна 0 или если целая часть
    //равна 0
    if(abs(current.zfFractional) == 0 || abs(current.zfInteger) == 0)
    {
        sprintf_s(str, "%d", current.zfInteger);
    }

    //целая часть отлична от нуля, степень полжительна, т.е. в строке целове число
    //с количеством нулей, равным числу степени
    if(current.zfFractional > 0 && abs(current.zfInteger) != 0)
    {
        sprintf_s(str, "%d%0*d", current.zfInteger, current.zfFractional, 0);
    }

    //целая часть отлична от нуля, степень отрицательна, т.е. в строке число
    //с плавающей запятой.
    if(current.zfFractional < 0 && abs(current.zfInteger) != 0)
    {
        i = current.zfInteger/(int)pow(10.0, abs(current.zfFractional)); //чило до точки
        f = abs(current.zfInteger%(int)pow(10.0, abs(current.zfFractional))); //число после запятой
        if(f%10 == 0){
            f = f/10;
            current.zfFractional += 1;
        }
        if(f == 0)
            sprintf_s(str, "%d", i);
        else
        {
			if ((i == 0) && current.zfInteger < 0)
                sprintf_s(str, "-%d.%0*d", i, abs(current.zfFractional), f);
            else
                sprintf_s(str, "%d.%0*d", i, abs(current.zfFractional), f);
        }
    }
   
    for(int Counter2 = 0, Size = strlen(str); Counter2 < Size; Counter2++)
        string = string + str[Counter2];
    m_param.m_axis.apUnderTextArray.push_back(string);
}
//------------------------------------------------------------------------------
void COscillGrid::MakeTextRect(double val, GridType type)
{
	CRect rect(0, 0, 0, 0);

    if(type == gtHorizontal)
    {
        rect.left = val
        - m_param.m_axis.apMaxTextLength/2.0;
        rect.right = val
        + m_param.m_axis.apMaxTextLength/2.0;
        rect.top = m_param.m_rect.bottom + 8;
        rect.bottom = m_param.m_rect.bottom + 8 + m_param.m_axis.apMaxTexthHeight;
        m_param.m_axis.apUnderTextRect.push_back(rect);
    }

    if(type == gtVertical)
    {
        rect.left = m_param.m_rect.left - m_param.m_axis.apMaxTextLength;
        rect.right = m_param.m_rect.left - 10;
        rect.top = val
        - m_param.m_axis.apMaxTexthHeight/2.0;
        rect.bottom = val
        + m_param.m_axis.apMaxTexthHeight/2.0;
        m_param.m_axis.apUnderTextRect.push_back(rect);
    }
}
//------------------------------------------------------------------------------
void COscillGrid::SieveTextMarks()
{
	unsigned int Counter, Coeff = 1, Start = 0;
	std::vector<CRect> tmpRect, _tmpRect;
	std::vector<CString> tmpText, _tmpText;
	z_float begin, step, current;
	char str[32] =  "" ;
	CString string(L"");
	int i(0), f(0);

	begin = m_param.m_axis.apZMinCurrent;
	step = m_param.m_axis.apZStep;

	current.zfSign = false;

	for (Counter = 0; Counter < m_param.m_axis.apUnderTextRect.size(); ++Counter)
	{
		if (m_param.m_axis.apUnderTextRect[Counter].left - m_param.m_axis.apUnderTextRect[0].right > 5)
		{
			Coeff = Counter;
			break;
		}
	}
	for (Counter = 0; Counter < m_param.m_axis.apUnderTextRect.size(); ++Counter)
	{
		//если степень шага меньше, чем степень стартового значения,
		//то приводим степень к наименьшей
		if (begin.zfFractional > step.zfFractional)
		{
			current.zfFractional = step.zfFractional;
			current.zfInteger = begin.zfInteger*pow(10.0, (begin.zfFractional - step.zfFractional)) + step.zfInteger*Counter;
			if (begin.zfSign)
				current.zfSign = true;
		}

		else
		{
			current.zfFractional = begin.zfFractional;
			current.zfInteger = Counter*step.zfInteger*pow(10.0, (step.zfFractional - begin.zfFractional)) + begin.zfInteger;
			step.zfInteger = step.zfInteger*pow(10.0, (step.zfFractional - begin.zfFractional));
			step.zfFractional = begin.zfFractional;
			if (begin.zfSign)
				current.zfSign = true;
		}
		if (abs(current.zfInteger) % (step.zfInteger*Coeff) != 0)
			continue;

		Start = Counter;
		break;
	}

	for (Counter = Start; Counter < m_param.m_axis.apUnderTextRect.size(); Counter = Counter + Coeff)
	{
		tmpRect.push_back(m_param.m_axis.apUnderTextRect[Counter]);
		tmpText.push_back(m_param.m_axis.apUnderTextArray[Counter]);
		if (!m_param.m_axis.apUnderTextRect.empty())
		{
			_tmpRect.push_back(m_param.m_axis.apUnderTextRect[Counter]);
			_tmpText.push_back(m_param.m_axis.apUnderTextArray[Counter]);
		}
	}

	m_param.m_axis.apOverTextRect = tmpRect;
	m_param.m_axis.apUnderTextRect = _tmpRect;
	m_param.m_axis.apOverTextArray = tmpText;
	m_param.m_axis.apUnderTextArray = _tmpText;
}
//------------------------------------------------------------------------------
void COscillGrid::Shift(zRect rect)
{
    double min, max;
    if(rect == gLeftShift || rect == gBottomShift)
    {
        min = m_param.m_axis.apMinCurrent - m_param.m_axis.apStep;
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
            m_param.m_axis.apZMinCurrent.zfFractional = m_param.m_axis.apZMinCurrent.zfFractional;		//	TODO: потенциальная ошибка
        }

        max = m_param.m_axis.apMaxCurrent - m_param.m_axis.apStep;

    }

    if(rect == gRightShift || rect == gTopShift)
    {
        min = m_param.m_axis.apMinCurrent + m_param.m_axis.apStep;
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
            m_param.m_axis.apZMinCurrent.zfFractional = m_param.m_axis.apZMinCurrent.zfFractional;		//	TODO: потенциальная ошибка
        }
        
    }

    if(rect == gNone)
        return;

    SetAxis(min, max);
}
//------------------------------------------------------------------------------
void COscillGrid::Scale(zRect rect)
{
	double stepX(0.0), Power(0.0), temp(0.0);
    double min = m_param.m_axis.apMinCurrent, max = m_param.m_axis.apMaxCurrent;

    int QntOfMarks;

    if(m_param.m_axis.apGridType == gtHorizontal)
        QntOfMarks = QNT_MARKS_X;
    if(m_param.m_axis.apGridType == gtVertical)
        QntOfMarks = QNT_MARKS_Y;

    if(rect == gLeftScale || rect == gBottomScale)
    {
        stepX = m_param.m_axis.apStep;
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
        }
    }

    if(rect == gRightScale || rect == gTopScale)
    {
        stepX = m_param.m_axis.apStep;
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

		double temp_val = temp - m_param.m_axis.apMinStep;

        if(temp - m_param.m_axis.apMinStep < 0)//temp < m_param.m_axis.apMinStep
			if (abs(temp_val) > pow(10.0, (int)Power - 1))
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
        }
    }

    if(rect == gCenterAutoScale)
    {

    }

    if(rect == gNone)
        return;

    SetAxis(min, max);
}
//------------------------------------------------------------------------------
void COscillGrid::AutoScale(double minValue, double maxValue)
{

	SetAxis(minValue, maxValue);
 //   double iMinValue, iMaxValue, difference, scale, current_min, current_max;
 //   double Power, stepX;
 //   int iStep, temp, i = 2;
 //   
 //   stepX = (maxValue - minValue)/(QNT_MARKS_Y-1);
 //   Power = log10(stepX);
 //   if(Power < 0)
 //       Power = (int)Power - 1;
 //   iStep = GetRoundStep((int)Power, stepX);
 //   if(abs(minValue) < Epsilon)
 //       iMinValue = 0;
 //   else
 //       iMinValue = GetRoundStep((int)Power, abs(minValue));
 //   if(minValue<0)
 //       iMinValue = 0 - iMinValue;
 //   switch(iStep)
 //   {
 //   case 1:
 //       temp = 2;
 //       break;

 //   case 2:
 //       temp = 5;
 //       break;

 //   case 5:
 //       temp = 10;
 //       break;

 //   default:
 //       temp = 1;
 //       Power=Power+1;
 //       break;
 //   }
 //   iMinValue = (maxValue + minValue)/2.0 - 4.0*temp*pow(10.0, (int)Power);
 //   iMaxValue = (maxValue + minValue)/2.0 + 4.0*temp*pow(10.0, (int)Power);
 //   //iMinValue = iMinValue - temp*pow(10.0, (int)Power);
 //   //iMaxValue = iMinValue + 8*temp*pow(10.0, (int)Power);

	////double tpow10 = pow(10.0, (int)Power);
 //   //SetAxis(iMinValue*tpow10, iMaxValue*tpow10);
	//if (iMinValue > minValue){
	//	if (minValue != 0)
	//		iMinValue = (int)(minValue / abs(minValue)) + minValue;
	//	else
	//		iMinValue = 0;
	//}

	//if (iMaxValue < maxValue)
	//	if (maxValue != 0)
	//		iMaxValue = (int)(maxValue / abs(maxValue)) + maxValue;
	//	else
	//		iMaxValue = 0;

    //SetAxis(iMinValue, iMaxValue);
}
//------------------------------------------------------------------------------
float COscillGrid::GetRoundStep(int Pow, double val)
{
    val = floor(val/pow(10.0, Pow)+0.5);
    if(val<=1){
        return 1;
    }
    if(val<=2)
    {
        return 2;
    }
    if(val<=5)
    {
        return 5;
    }
    else
    {
        return 10;
    }
}
//------------------------------------------------------------------------------
void COscillGrid::OnLButtonDown(UINT nFlags, CPoint point)
{
    if(m_param.m_axis.apGridType == gtVertical)
        m_pOwner->OnLButtonDown(nFlags, point);
}
//------------------------------------------------------------------------------
void COscillGrid::SetGridColor(COLORREF GridColor)
{
	m_pDOscillGrid->SetGridColor(GridColor);
}