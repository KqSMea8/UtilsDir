//------------------------------------------------------------------------------
// TrendGrid.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include <Interface2012\ZetLab\TrendGrid.h>
//------------------------------------------------------------------------------
CTrendGrid::CTrendGrid(CCustomContainer *owner, CRect rect, GridType gridType) :
CCustomGrid(owner)
{
    m_pGridParam = &m_param;
    m_param.m_rect = rect;
    m_param.m_axis.apGridType = gridType;

    m_param.m_axis.apMaxTextLength = GetTextDimention(m_param.m_axis.apFont, L"00:00:00.000").x;
    m_param.m_axis.apMaxTexthHeight = GetTextDimention(m_param.m_axis.apFont, L"00:00:00.000").y;

    if(m_param.m_axis.apGridType == gtHorizontal){
        m_param.m_axis.apQntTextMarks = m_param.m_rect.Width() / m_param.m_axis.apMaxTextLength; //количество блоков. т.е. сколько меток с интервалом 1 между ними возможно расставить
    }

    if(m_param.m_axis.apGridType == gtVertical){

        m_param.m_axis.apQntTextMarks = m_param.m_rect.Height() / m_param.m_axis.apMaxTexthHeight;
    }

    if(m_param.m_axis.apQntMarks - m_param.m_axis.apQntTextMarks < 0)
        m_param.m_axis.apQntTextMarks = m_param.m_axis.apQntMarks;

    m_pDTrendGrid = new CDTrendGrid(rect, m_pOwner->GetPointerToIsChange(), m_pOwner->GetPointerToTime());
    m_pDClass = m_pDTrendGrid;

    FillGridArray();
};
//------------------------------------------------------------------------------
CTrendGrid::~CTrendGrid()
{
	m_param.Clear();
	if (m_pDTrendGrid)
	{
		delete m_pDTrendGrid;
		m_pDTrendGrid = nullptr;
	}
};
//------------------------------------------------------------------------------
void CTrendGrid::Resize(CRect rect)
{
	m_param.m_rect = rect;
	m_pDTrendGrid->Resize(rect);
	m_param.pVertexs.clear();
	m_param.m_axis.apUnderTextArray.clear();
	m_param.m_axis.apUnderTextRect.clear();
	FillGridArray();
	if (m_param.m_axis.apGridType == gtHorizontal)
		m_pDTrendGrid->SetTitleRect(CRect(m_param.m_rect.right - 20,
		m_param.m_rect.bottom + 10 + 2 * m_param.m_axis.apMaxTexthHeight + 2, m_param.m_rect.right + 20,
		m_param.m_rect.bottom + 10 + 2 * m_param.m_axis.apMaxTexthHeight + 50));

	if (m_param.m_axis.apGridType == gtVertical)
		m_pDTrendGrid->SetTitleRect(CRect(m_param.m_rect.left - 50,
		m_param.m_rect.top - 20, m_param.m_rect.left,
		m_param.m_rect.top));
}
//------------------------------------------------------------------------------
void CTrendGrid::SetAxis(double min, double max)
{
    CCustomGrid::SetAxis(min, max);
    m_param.m_axis.apMinCurrentMS = 0;
    m_param.m_axis.apMaxCurrentMS = 0;
    FillGridArray();
}
//------------------------------------------------------------------------------
void CTrendGrid::SetAxisTitle(CString title, GridType type)
{
    CCustomGrid::SetAxisTitle(title, type);
    m_pDTrendGrid->SetAxisTitle(title);
    if(type == gtHorizontal)
        m_pDTrendGrid->SetTitleRect(CRect(m_param.m_rect.right-20, 
        m_param.m_rect.bottom + 10 + 2*m_param.m_axis.apMaxTexthHeight+2, m_param.m_rect.right+20,
        m_param.m_rect.bottom + 10 + 2*m_param.m_axis.apMaxTexthHeight+50));

    if(type == gtVertical)
        m_pDTrendGrid->SetTitleRect(CRect(m_param.m_rect.left-50, 
        m_param.m_rect.top-20, m_param.m_rect.left,
        m_param.m_rect.top));
}
//------------------------------------------------------------------------------
void CTrendGrid::FillGridArray()
{
    int Counter(0), Counter1(0), Counter2(0), CurrentMonth(0), millisecond(0);
    double CurrentDay(0.0);
	double delta(0.0);
    TimeInterval time;
    /////////
    COleDateTimeSpan temp;
    COleDateTime m_time;
    /////////
    time = GetInterval();
    m_param.pBoldVertexs.clear();
    m_param.m_axis.apUnderTextArray.clear();
    m_param.m_axis.apUnderTextRect.clear();
    m_param.m_axis.apOverTextArray.clear();
    m_param.m_axis.apOverTextRect.clear();
    m_param.TimeMarks.clear();

    //если отображаемый интервал год и более
    if(time.tiID == Year)
    {
        if(time.tiData >= MIN_QNT_MARKS)
        {
            m_param.m_axis.apQntBoldMarks = time.tiData + 1;
            if(m_param.m_axis.apQntBoldMarks > MAX_QNT_MARKS)
                m_param.m_axis.apQntBoldMarks = MAX_QNT_MARKS;
			m_param.m_axis.apBoldMarkingArray.resize(m_param.m_axis.apQntBoldMarks);

            for(Counter = 0; Counter < m_param.m_axis.apQntBoldMarks; Counter++)
            {
                m_time = COleDateTime(COleDateTime(m_param.m_axis.apMinCurrent).GetYear()+(Counter+1),
                    1, 1, 0, 0, 0);

				if (m_param.m_axis.apGridType == gtHorizontal)
				{
					m_param.m_axis.apBoldMarkingArray[Counter] =
						m_param.m_rect.left + FromWorldToScreen((DATE)m_time - m_param.m_axis.apMinCurrent,
						m_param.m_axis, m_param.m_rect);
				}

				else
				{
					m_param.m_axis.apBoldMarkingArray[Counter] =
						m_param.m_rect.bottom - FromWorldToScreen((DATE)m_time - m_param.m_axis.apMinCurrent,
						m_param.m_axis, m_param.m_rect);
				}

                m_param.TimeMarks.push_back(m_time);
            }
            m_param.NumberOfTimeStep = 0;
        }
    }

    //отображаемый интервал от месяца до года
    if(time.tiID == Month)
    {
        delta = COleDateTime(m_param.m_axis.apMaxCurrent).GetMonth()
            + COleDateTime(m_param.m_axis.apMaxCurrent).GetYear()*12 - COleDateTime(m_param.m_axis.apMinCurrent).GetMonth()
            - COleDateTime(m_param.m_axis.apMinCurrent).GetYear()*12 + COleDateTime(m_param.m_axis.apMaxCurrent).GetDay()/30
            - COleDateTime(m_param.m_axis.apMinCurrent).GetDay()/30;
        for(Counter = 1; Counter < 6; Counter++)
        {
            if(delta / TimeSteps[Counter] >= MIN_QNT_MARKS)
            {
                m_param.m_axis.apStep = TimeSteps[Counter];
                m_param.NumberOfTimeStep = Counter;

                m_param.m_axis.apQntBoldMarks = delta/m_param.m_axis.apStep+1;
                if(m_param.m_axis.apQntBoldMarks > MAX_QNT_MARKS)
                    m_param.m_axis.apQntBoldMarks = MAX_QNT_MARKS;
				m_param.m_axis.apBoldMarkingArray.resize(m_param.m_axis.apQntBoldMarks);

                for(Counter1 = 0; Counter1 < m_param.m_axis.apQntBoldMarks; Counter1++)
                {
                    if((COleDateTime(m_param.m_axis.apMinCurrent).GetMonth() + (Counter1+1)*TimeSteps[Counter])%12 == 0)
                    {
                        m_time = COleDateTime(COleDateTime(m_param.m_axis.apMinCurrent).GetYear()
                            +(COleDateTime(m_param.m_axis.apMinCurrent).GetMonth() + (Counter1+1)*TimeSteps[Counter])/12-1,
                            12,
                            1, 0, 0, 0);
                    }
                    else
                    {
                        m_time = COleDateTime(COleDateTime(m_param.m_axis.apMinCurrent).GetYear()
                            +(COleDateTime(m_param.m_axis.apMinCurrent).GetMonth() + (Counter1+1)*TimeSteps[Counter])/12,
                            (COleDateTime(m_param.m_axis.apMinCurrent).GetMonth() + (Counter1+1)*TimeSteps[Counter])%12,
                            1, 0, 0, 0);
                    }
					if (m_param.m_axis.apGridType == gtHorizontal)
					{
						m_param.m_axis.apBoldMarkingArray[Counter1] =
							m_param.m_rect.left + FromWorldToScreen((DATE)m_time - m_param.m_axis.apMinCurrent,
							m_param.m_axis, m_param.m_rect);
					}
					else
					{
						m_param.m_axis.apBoldMarkingArray[Counter1] =
							m_param.m_rect.bottom - FromWorldToScreen((DATE)m_time - m_param.m_axis.apMinCurrent,
							m_param.m_axis, m_param.m_rect);
					}

                    m_param.TimeMarks.push_back(m_time);
                }
                break;
            }
        }
    }

    //отображаемый интервал от суток до месяца
    if(time.tiID == Day)
    {
        temp = COleDateTime(m_param.m_axis.apMaxCurrent) - COleDateTime(m_param.m_axis.apMinCurrent);

        for(Counter = 6; Counter < 14; Counter++)
        {
            if(temp.GetTotalHours()/24.0
                / TimeSteps[Counter] >= MIN_QNT_MARKS)
            {
                m_param.m_axis.apStep = TimeSteps[Counter];
                m_param.NumberOfTimeStep = Counter;

                m_param.m_axis.apQntBoldMarks = time.tiData/m_param.m_axis.apStep+1;
                if(m_param.m_axis.apQntBoldMarks > MAX_QNT_MARKS)
                    m_param.m_axis.apQntBoldMarks = MAX_QNT_MARKS;
				m_param.m_axis.apBoldMarkingArray.resize(m_param.m_axis.apQntBoldMarks);

                m_time = COleDateTime(m_param.m_axis.apMinCurrent)
                    - COleDateTimeSpan(0, COleDateTime(m_param.m_axis.apMinCurrent).GetHour(),
                    COleDateTime(m_param.m_axis.apMinCurrent).GetMinute(),
                    COleDateTime(m_param.m_axis.apMinCurrent).GetSecond())
                    + COleDateTimeSpan(m_param.m_axis.apStep, 0, 0, 0);

                if(m_time.GetDay()%(int)(m_param.m_axis.apStep) != 0)
                    m_time -= COleDateTimeSpan((int)m_time.GetDay()%(int)(m_param.m_axis.apStep), 0, 0, 0);

				if (m_param.m_axis.apGridType == gtHorizontal)
				{
					m_param.m_axis.apBoldMarkingArray[0] =
						m_param.m_rect.left + FromWorldToScreen((DATE)m_time - m_param.m_axis.apMinCurrent,
						m_param.m_axis, m_param.m_rect);
				}

				else
				{
					m_param.m_axis.apBoldMarkingArray[0] =
						m_param.m_rect.bottom - FromWorldToScreen((DATE)m_time - m_param.m_axis.apMinCurrent,
						m_param.m_axis, m_param.m_rect);
				}

                m_param.TimeMarks.push_back(m_time);

                for(Counter1 = 1; Counter1 < m_param.m_axis.apQntBoldMarks; Counter1++)
                {
                    m_time += COleDateTimeSpan((int)m_param.m_axis.apStep,0,0,0);

					if (m_param.m_axis.apGridType == gtHorizontal)
					{
						m_param.m_axis.apBoldMarkingArray[Counter1] =
							m_param.m_rect.left + FromWorldToScreen((DATE)m_time - m_param.m_axis.apMinCurrent,
							m_param.m_axis, m_param.m_rect);
					}
					else
					{
						m_param.m_axis.apBoldMarkingArray[Counter1] =
							m_param.m_rect.bottom - FromWorldToScreen((DATE)m_time - m_param.m_axis.apMinCurrent,
							m_param.m_axis, m_param.m_rect);
					}

                    m_param.TimeMarks.push_back(m_time);
                }
                break;
            }
        }
    }

    //отображаемый интервал от часа до суток
    if(time.tiID == Hour)
    {
        temp = COleDateTime(m_param.m_axis.apMaxCurrent) - COleDateTime(m_param.m_axis.apMinCurrent);

        for(Counter = 14; Counter < 20; Counter++)
        {
            if(temp.GetTotalMinutes()/60.0
                / TimeSteps[Counter] >= MIN_QNT_MARKS)
            {
                m_param.m_axis.apStep = TimeSteps[Counter];
                m_param.NumberOfTimeStep = Counter;

                m_param.m_axis.apQntBoldMarks = time.tiData/m_param.m_axis.apStep + 1;
                if(m_param.m_axis.apQntBoldMarks > MAX_QNT_MARKS)
                    m_param.m_axis.apQntBoldMarks = MAX_QNT_MARKS;
				m_param.m_axis.apBoldMarkingArray.resize(m_param.m_axis.apQntBoldMarks);

                m_time = COleDateTime(m_param.m_axis.apMinCurrent)
                    - COleDateTimeSpan(0, 0, COleDateTime(m_param.m_axis.apMinCurrent).GetMinute(),
                    COleDateTime(m_param.m_axis.apMinCurrent).GetSecond())
                    + COleDateTimeSpan(0, m_param.m_axis.apStep, 0, 0);

                if(m_time.GetHour()%(int)(m_param.m_axis.apStep) != 0)
                    m_time -= COleDateTimeSpan(0, (int)m_time.GetHour()%(int)(m_param.m_axis.apStep), 0, 0);

				if (m_param.m_axis.apGridType == gtHorizontal)
				{
					m_param.m_axis.apBoldMarkingArray[0] =
						m_param.m_rect.left + FromWorldToScreen((DATE)m_time - m_param.m_axis.apMinCurrent,
						m_param.m_axis, m_param.m_rect);
				}
				else
				{
					m_param.m_axis.apBoldMarkingArray[0] =
						m_param.m_rect.bottom - FromWorldToScreen((DATE)m_time - m_param.m_axis.apMinCurrent,
						m_param.m_axis, m_param.m_rect);
				}

                m_param.TimeMarks.push_back(m_time);

                for(Counter1 = 1; Counter1 < m_param.m_axis.apQntBoldMarks; Counter1++)
                {
                    m_time += COleDateTimeSpan(0, m_param.m_axis.apStep, 0, 0);

					if (m_param.m_axis.apGridType == gtHorizontal)
					{
						m_param.m_axis.apBoldMarkingArray[Counter1] =
							m_param.m_rect.left + FromWorldToScreen((DATE)m_time - m_param.m_axis.apMinCurrent,
							m_param.m_axis, m_param.m_rect);
					}
					else
					{
						m_param.m_axis.apBoldMarkingArray[Counter1] =
							m_param.m_rect.bottom - FromWorldToScreen((DATE)m_time - m_param.m_axis.apMinCurrent,
							m_param.m_axis, m_param.m_rect);
					}

                    m_param.TimeMarks.push_back(m_time);
                }
                break;
            }
        }
    }

    //отображаемый интервал от минуты до часа
    if(time.tiID == Minute)
    {
        temp = COleDateTime(m_param.m_axis.apMaxCurrent) - COleDateTime(m_param.m_axis.apMinCurrent);

        if(time.tiData >= MIN_QNT_MARKS)
        {
            for(Counter = 20; Counter < 27; Counter++)
            {
                if(temp.GetTotalSeconds()/60.0
                    / TimeSteps[Counter] >= MIN_QNT_MARKS)
                {
                    m_param.m_axis.apStep = TimeSteps[Counter];
                    m_param.NumberOfTimeStep = Counter;

                    m_param.m_axis.apQntBoldMarks = time.tiData/m_param.m_axis.apStep + 1;
                    if(m_param.m_axis.apQntBoldMarks > MAX_QNT_MARKS)
                        m_param.m_axis.apQntBoldMarks = MAX_QNT_MARKS;
					m_param.m_axis.apBoldMarkingArray.resize(m_param.m_axis.apQntBoldMarks);

                    m_time = COleDateTime(m_param.m_axis.apMinCurrent)
                        - COleDateTimeSpan(0, 0, 0, COleDateTime(m_param.m_axis.apMinCurrent).GetSecond())
                        + COleDateTimeSpan(0, 0, m_param.m_axis.apStep, 0);

                    if(m_time.GetMinute()%(int)(m_param.m_axis.apStep) != 0)
                        m_time -= COleDateTimeSpan(0, 0, (int)m_time.GetMinute()%(int)(m_param.m_axis.apStep), 0);

					if (m_param.m_axis.apGridType == gtHorizontal)
					{
						m_param.m_axis.apBoldMarkingArray[0] =
							m_param.m_rect.left + FromWorldToScreen((DATE)m_time - m_param.m_axis.apMinCurrent,
							m_param.m_axis, m_param.m_rect);
					}
					else
					{
						m_param.m_axis.apBoldMarkingArray[0] =
							m_param.m_rect.bottom - FromWorldToScreen((DATE)m_time - m_param.m_axis.apMinCurrent,
							m_param.m_axis, m_param.m_rect);
					}

                    m_param.TimeMarks.push_back(m_time);

                    for(Counter = 1; Counter < m_param.m_axis.apQntBoldMarks; Counter++)
                    {
                        m_time += COleDateTimeSpan(0, 0, m_param.m_axis.apStep, 0);

						if (m_param.m_axis.apGridType == gtHorizontal)
						{
							m_param.m_axis.apBoldMarkingArray[Counter] =
								m_param.m_rect.left + FromWorldToScreen((DATE)m_time - m_param.m_axis.apMinCurrent,
								m_param.m_axis, m_param.m_rect);
						}
						else
						{
							m_param.m_axis.apBoldMarkingArray[Counter] =
								m_param.m_rect.bottom - FromWorldToScreen((DATE)m_time - m_param.m_axis.apMinCurrent,
								m_param.m_axis, m_param.m_rect);
						}

                        m_param.TimeMarks.push_back(m_time);
                    }
                    break;
                }
            }
        }
    }

    if(time.tiID == Second)
    {
        temp = COleDateTime(m_param.m_axis.apMaxCurrent) - COleDateTime(m_param.m_axis.apMinCurrent);
        if(time.tiData >= MIN_QNT_MARKS)
        {
            for(Counter = 27; Counter < 34; Counter++)
            {
                if(temp.GetTotalSeconds()
                    / TimeSteps[Counter] >= MIN_QNT_MARKS)
                {
                    m_param.m_axis.apStep = TimeSteps[Counter];
                    m_param.NumberOfTimeStep = Counter;

                    m_param.m_axis.apQntBoldMarks = time.tiData/m_param.m_axis.apStep + 1;
                    if(m_param.m_axis.apQntBoldMarks > MAX_QNT_MARKS)
                        m_param.m_axis.apQntBoldMarks = MAX_QNT_MARKS;
					m_param.m_axis.apBoldMarkingArray.resize(m_param.m_axis.apQntBoldMarks);

                    m_time = COleDateTime(m_param.m_axis.apMinCurrent)
                        + COleDateTimeSpan(0, 0, 0, m_param.m_axis.apStep);

                    if(m_time.GetSecond()%(int)(m_param.m_axis.apStep) != 0)
                        m_time -= COleDateTimeSpan(0, 0, 0, (int)m_time.GetSecond()%(int)(m_param.m_axis.apStep));

					if (m_param.m_axis.apGridType == gtHorizontal)
					{
						m_param.m_axis.apBoldMarkingArray[0] =
							m_param.m_rect.left + FromWorldToScreen((DATE)m_time - m_param.m_axis.apMinCurrent,
							m_param.m_axis, m_param.m_rect);
					}
					else
					{
						m_param.m_axis.apBoldMarkingArray[0] =
							m_param.m_rect.bottom - FromWorldToScreen((DATE)m_time - m_param.m_axis.apMinCurrent,
							m_param.m_axis, m_param.m_rect);
					}

                    m_param.TimeMarks.push_back(m_time);

                    for(Counter = 1; Counter < m_param.m_axis.apQntBoldMarks; Counter++)
                    {
                        m_time += COleDateTimeSpan(0, 0, 0, m_param.m_axis.apStep);

						if (m_param.m_axis.apGridType == gtHorizontal)
						{
							m_param.m_axis.apBoldMarkingArray[Counter] =
								m_param.m_rect.left + FromWorldToScreen((DATE)m_time - m_param.m_axis.apMinCurrent,
								m_param.m_axis, m_param.m_rect);
						}
						else
						{
							m_param.m_axis.apBoldMarkingArray[Counter] =
								m_param.m_rect.bottom - FromWorldToScreen((DATE)m_time - m_param.m_axis.apMinCurrent,
								m_param.m_axis, m_param.m_rect);
						}
                        
                        m_param.TimeMarks.push_back(m_time);
                    }
                    break;
                }
            }
        }
    }

    if(time.tiID == Millisecond)
    {
        temp = COleDateTime(m_param.m_axis.apMaxCurrent) - COleDateTime(m_param.m_axis.apMinCurrent);
        if(time.tiData >= MIN_QNT_MARKS)
        {
            for(Counter = 34; Counter < 37; Counter++)
            {
                if((temp.GetTotalSeconds()*1000
                    + m_param.m_axis.apMaxCurrentMS - m_param.m_axis.apMinCurrentMS)
                    / TimeSteps[Counter] >= MIN_QNT_MARKS)
                {
                    m_param.m_axis.apStep = TimeSteps[Counter];
                    m_param.NumberOfTimeStep = Counter;

                    m_param.m_axis.apQntBoldMarks = time.tiData/m_param.m_axis.apStep + 1;
                    if(m_param.m_axis.apQntBoldMarks > MAX_QNT_MARKS)
                        m_param.m_axis.apQntBoldMarks = MAX_QNT_MARKS;
					m_param.m_axis.apBoldMarkingArray.resize(m_param.m_axis.apQntBoldMarks);

                    millisecond = m_param.m_axis.apMinCurrentMS + m_param.m_axis.apStep;
                    if(millisecond > 999)
                    {
                        m_time = COleDateTime(m_param.m_axis.apMinCurrent) + COleDateTimeSpan(0, 0, 0, (int)millisecond/1000);
                        millisecond = millisecond%1000;
                    }
                    else
                        m_time = COleDateTime(m_param.m_axis.apMinCurrent);

                    if(millisecond%(int)m_param.m_axis.apStep != 0)
                        millisecond = millisecond - millisecond%(int)m_param.m_axis.apStep;

                    AxisParameters tmp;
                    tmp.apGridType = m_param.m_axis.apGridType;
                    tmp.apScaleType = m_param.m_axis.apScaleType;
                    tmp.apMinCurrent = 0;
                    tmp.apMaxCurrent = temp.GetTotalSeconds()*1000
                        + m_param.m_axis.apMaxCurrentMS - m_param.m_axis.apMinCurrentMS;

					if (m_param.m_axis.apGridType == gtHorizontal)
					{
						m_param.m_axis.apBoldMarkingArray[0] =
							m_param.m_rect.left + FromWorldToScreen((m_time
							- COleDateTime(m_param.m_axis.apMinCurrent)).GetTotalSeconds() * 1000
							+ millisecond - m_param.m_axis.apMinCurrentMS,
							tmp, m_param.m_rect);
					}
					else
					{
						m_param.m_axis.apBoldMarkingArray[0] =
							m_param.m_rect.bottom - FromWorldToScreen((m_time
							- COleDateTime(m_param.m_axis.apMinCurrent)).GetTotalSeconds() * 1000
							+ millisecond - m_param.m_axis.apMinCurrentMS,
							tmp, m_param.m_rect);
					}

                    m_param.TimeMarks.push_back(m_time);
                    MakeUnderText(m_param.TimeMarks[0], millisecond);
                    MakeUnderTextRect(m_param.m_axis.apBoldMarkingArray[0], m_param.m_axis.apGridType);

                    for(Counter = 1; Counter < m_param.m_axis.apQntBoldMarks; Counter++)
                    {
                        millisecond = millisecond + m_param.m_axis.apStep;
                        if(millisecond > 999)
                        {
                            m_time += COleDateTimeSpan(0, 0, 0, millisecond/1000);
                            millisecond = millisecond%1000;
                        }

                        if(millisecond%(int)m_param.m_axis.apStep != 0)
                            millisecond = millisecond - millisecond%(int)m_param.m_axis.apStep;

						if (m_param.m_axis.apGridType == gtHorizontal)
						{
							m_param.m_axis.apBoldMarkingArray[Counter] =
								m_param.m_rect.left + FromWorldToScreen((m_time
								- COleDateTime(m_param.m_axis.apMinCurrent)).GetTotalSeconds() * 1000
								+ millisecond - m_param.m_axis.apMinCurrentMS,
								tmp, m_param.m_rect);
						}
						else
						{
							m_param.m_axis.apBoldMarkingArray[Counter] =
								m_param.m_rect.bottom - FromWorldToScreen((m_time
								- COleDateTime(m_param.m_axis.apMinCurrent)).GetTotalSeconds() * 1000
								+ millisecond - m_param.m_axis.apMinCurrentMS,
								tmp, m_param.m_rect);
						}

                        m_param.TimeMarks.push_back(m_time);
                        MakeUnderText(m_param.TimeMarks[Counter], millisecond);
                        MakeUnderTextRect(m_param.m_axis.apBoldMarkingArray[Counter], m_param.m_axis.apGridType);
                    }
                    break;
                }
            }
        }
    }
    else
    {
        //m_param.m_axis.apMaxCurrentMS = 0;
        //m_param.m_axis.apMinCurrentMS = 0;
    }

    m_param.m_axis.apTimeStep.tiID = time.tiID;
    m_param.m_axis.apTimeStep.tiData = m_param.m_axis.apStep;

    for(Counter = 0; Counter < m_param.m_axis.apQntBoldMarks; Counter++){
		if (m_param.m_axis.apGridType == gtHorizontal)
		{
			m_param.pBoldVertexs.push_back(m_param.m_axis.apBoldMarkingArray[Counter]);
			m_param.pBoldVertexs.push_back(m_param.m_rect.bottom);
			m_param.pBoldVertexs.push_back(5);
			m_param.pBoldVertexs.push_back(m_param.m_axis.apBoldMarkingArray[Counter]);
			m_param.pBoldVertexs.push_back(m_param.m_rect.top - 1);
			m_param.pBoldVertexs.push_back(5);
		}
		else
		{
			m_param.pBoldVertexs.push_back(m_param.m_rect.left);
			m_param.pBoldVertexs.push_back(m_param.m_axis.apBoldMarkingArray[Counter]);
			m_param.pBoldVertexs.push_back(5);
			m_param.pBoldVertexs.push_back(m_param.m_rect.right);
			m_param.pBoldVertexs.push_back(m_param.m_axis.apBoldMarkingArray[Counter]);
			m_param.pBoldVertexs.push_back(5);
		}
        MakeOverText(m_param.TimeMarks[Counter]);
        MakeOverTextRect(m_param.m_axis.apBoldMarkingArray[Counter], m_param.m_axis.apGridType);
        if(time.tiID != Day && time.tiID != Month && time.tiID != Year && time.tiID != Millisecond)
        {
            MakeUnderText(m_param.TimeMarks[Counter]);
            MakeUnderTextRect(m_param.m_axis.apBoldMarkingArray[Counter], m_param.m_axis.apGridType);
        }
		if (time.tiID == Millisecond)
		{
			MakeUnderText(m_param.TimeMarks[Counter], millisecond);
			MakeUnderTextRect(m_param.m_axis.apBoldMarkingArray[Counter], m_param.m_axis.apGridType);
		}
    }

	SieveTextMarks();

    m_pDTrendGrid->SetBoldGridVertex(m_param.pBoldVertexs);
    //m_pDTrendGrid->SetGridVertex(m_param.pVertexs);
    m_pDTrendGrid->SetUnderTextArray(m_param.m_axis.apUnderTextArray);
    m_pDTrendGrid->SetUnderTextRect(m_param.m_axis.apUnderTextRect);
    m_pDTrendGrid->SetOverTextArray(m_param.m_axis.apOverTextArray);
    m_pDTrendGrid->SetOverTextRect(m_param.m_axis.apOverTextRect);

};
//------------------------------------------------------------------------------
void CTrendGrid::MakeOverText(COleDateTime time)
{
    m_param.m_axis.apOverTextArray.push_back(time.Format(_T("%d/%m/%Y")));
}
//------------------------------------------------------------------------------
void CTrendGrid::MakeUnderText(COleDateTime time)
{
    m_param.m_axis.apUnderTextArray.push_back(time.Format(_T("%H:%M:%S")));
}
//------------------------------------------------------------------------------
void CTrendGrid::MakeUnderText(COleDateTime time, int milliseconds)
{
	char str[6] =  "" ;
	CString string(L"");
    if(milliseconds < 100)
        sprintf_s(str, ".0%d", milliseconds);

    if(milliseconds < 10)
        sprintf_s(str, ".00%d", milliseconds);

    if(milliseconds >= 100)
        sprintf_s(str, ".%d", milliseconds);
    string = str;
    m_param.m_axis.apUnderTextArray.push_back(time.Format(_T("%H:%M:%S:")) + string);
}
//------------------------------------------------------------------------------
void CTrendGrid::MakeOverTextRect(double val, GridType type)
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
        m_param.m_axis.apOverTextRect.push_back(rect);
    }

    if(type == gtVertical)
    {
        rect.left = 0;
		rect.right = m_param.m_rect.left;
		rect.top = val - m_param.m_axis.apMaxTexthHeight;
        rect.bottom = val;
        m_param.m_axis.apOverTextRect.push_back(rect);
    }
}
//------------------------------------------------------------------------------
void CTrendGrid::MakeUnderTextRect(double val, GridType type)
{
	CRect rect(0, 0, 0, 0);

    if(type == gtHorizontal)
    {
        rect.left = val
            - m_param.m_axis.apMaxTextLength/2.0;
        rect.right = val
            + m_param.m_axis.apMaxTextLength/2.0;
        rect.top = m_param.m_rect.bottom + 10 + m_param.m_axis.apMaxTexthHeight;
        rect.bottom = m_param.m_rect.bottom + 10 + 2*m_param.m_axis.apMaxTexthHeight;
        m_param.m_axis.apUnderTextRect.push_back(rect);
    }

    if(type == gtVertical)
    {
        rect.left = 0;
        rect.right = m_param.m_rect.left;
		rect.top = val;
        rect.bottom = val
            + m_param.m_axis.apMaxTexthHeight;
        m_param.m_axis.apUnderTextRect.push_back(rect);
    }

}
//------------------------------------------------------------------------------
void CTrendGrid::SieveTextMarks()
{
    unsigned int Counter, Coeff=1, Start=0;
    std::vector<CRect> tmpRect, _tmpRect;
    std::vector<CString> tmpText, _tmpText;

    for(Counter=1; Counter < m_param.m_axis.apOverTextRect.size(); ++Counter)
    {
        if(m_param.m_axis.apOverTextRect[Counter].left - m_param.m_axis.apOverTextRect[0].right > 5)
        {
            Coeff = Counter;
            break;
        }
    }
    for(Counter=0; Counter < m_param.m_axis.apOverTextRect.size(); ++Counter)
    {
        if(m_param.m_axis.apTimeStep.tiID == Year){
            if(m_param.TimeMarks[Counter].GetYear()%(Coeff) != 0)
                continue;
        }
        
        if(m_param.m_axis.apTimeStep.tiID == Month)
        {
            if(m_param.TimeMarks[Counter].GetMonth()%(m_param.m_axis.apTimeStep.tiData*Coeff) != 0)
                continue;
        }

        if(m_param.m_axis.apTimeStep.tiID == Day)
        {
            if(m_param.TimeMarks[Counter].GetDay()%(m_param.m_axis.apTimeStep.tiData*Coeff) != 0)
                continue;
        }

        if(m_param.m_axis.apTimeStep.tiID == Hour)
        {
            if(m_param.TimeMarks[Counter].GetHour()%(m_param.m_axis.apTimeStep.tiData*Coeff) != 0)
                continue;
        }

        if(m_param.m_axis.apTimeStep.tiID == Minute)
        {
            if(m_param.TimeMarks[Counter].GetMinute()%(m_param.m_axis.apTimeStep.tiData*Coeff) != 0)
                continue;
        }

        if(m_param.m_axis.apTimeStep.tiID == Second)
        {
            int a = m_param.TimeMarks[Counter].GetSecond();
            if(m_param.TimeMarks[Counter].GetSecond()%(m_param.m_axis.apTimeStep.tiData*Coeff) != 0)
                continue;
        }
        Start = Counter;
        break;
    }

    for(Counter = Start; Counter <  m_param.m_axis.apOverTextRect.size(); Counter = Counter + Coeff)
    {
        tmpRect.push_back(m_param.m_axis.apOverTextRect[Counter]);
        tmpText.push_back(m_param.m_axis.apOverTextArray[Counter]);
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
TimeInterval CTrendGrid::GetInterval()
{
    TimeInterval ret;
    COleDateTimeSpan delta;
    delta = COleDateTime(m_param.m_axis.apMaxCurrent) - COleDateTime(m_param.m_axis.apMinCurrent);

    if(delta.GetDays()>365*5)
    {
        ret.tiData = delta.GetDays()/365;
        ret.tiID = Year;
        return ret;
    }

    if(delta.GetDays() > 5*30)
    {
        ret.tiData = delta.GetDays()/30;
        ret.tiID = Month;
        return ret;
    }

    if(delta.GetDays() > 5)
    {
        ret.tiData = delta.GetDays();
        ret.tiID = Day;
        return ret;
    }

    if(delta.GetTotalHours() > 5)
    {
        ret.tiData = delta.GetTotalHours();
        ret.tiID = Hour;
        return ret;
    }

    if(delta.GetTotalMinutes() > 5)
    {
        ret.tiData = delta.GetTotalMinutes();
        ret.tiID = Minute;
        return ret;
    }

    if(delta.GetTotalSeconds() > 5)
    {
        ret.tiData = delta.GetTotalSeconds();
        ret.tiID = Second;
        return ret;
    }

    if(delta.GetTotalSeconds() <= 5)
    {
        ret.tiData = delta.GetTotalSeconds()*1000
            + m_param.m_axis.apMaxCurrentMS - m_param.m_axis.apMinCurrentMS;
        ret.tiID = Millisecond;
        return ret;
    }

    ret.tiData = -1;
    ret.tiID = Second;

    return ret;
};
//------------------------------------------------------------------------------
void CTrendGrid::Clear(void)
{
    m_pDTrendGrid->Clear();
}
//------------------------------------------------------------------------------
void CTrendGrid::Shift(zRect rect)
{
    bool flag = false;

    int year_min, month_min, day_min, hour_min, minute_min, second_min, millisecond_min,
        year_max, month_max, day_max, hour_max, minute_max, second_max, millisecond_max;

    year_min = COleDateTime(m_param.m_axis.apMinCurrent).GetYear();
    month_min = COleDateTime(m_param.m_axis.apMinCurrent).GetMonth();
    day_min = COleDateTime(m_param.m_axis.apMinCurrent).GetDay();
    hour_min = COleDateTime(m_param.m_axis.apMinCurrent).GetHour();
    minute_min = COleDateTime(m_param.m_axis.apMinCurrent).GetMinute();
    second_min = COleDateTime(m_param.m_axis.apMinCurrent).GetSecond();
    millisecond_min = m_param.m_axis.apMinCurrentMS;

    year_max = COleDateTime(m_param.m_axis.apMaxCurrent).GetYear();
    month_max = COleDateTime(m_param.m_axis.apMaxCurrent).GetMonth();
    day_max = COleDateTime(m_param.m_axis.apMaxCurrent).GetDay();
    hour_max = COleDateTime(m_param.m_axis.apMaxCurrent).GetHour();
    minute_max = COleDateTime(m_param.m_axis.apMaxCurrent).GetMinute();
    second_max = COleDateTime(m_param.m_axis.apMaxCurrent).GetSecond();
    millisecond_max = m_param.m_axis.apMaxCurrentMS;

    if(rect == gLeftShift || rect == gBottomShift)
    {
        //если шаг измеряется в годах
        if(m_param.NumberOfTimeStep == 0)
        {
            year_min -= TimeSteps[m_param.NumberOfTimeStep];
            year_max -= TimeSteps[m_param.NumberOfTimeStep];
        }

        //шаг измеряется в месяцах
        if(m_param.NumberOfTimeStep > 0 && m_param.NumberOfTimeStep < 6)
        {
            month_min -= TimeSteps[m_param.NumberOfTimeStep];            
            month_max -= TimeSteps[m_param.NumberOfTimeStep];
        }

        //если шаг измеряется в днях
        if(m_param.NumberOfTimeStep >= 6 && m_param.NumberOfTimeStep < 14)
        {
            m_param.m_axis.apMinCurrent = (DATE)(COleDateTime(m_param.m_axis.apMinCurrent) - COleDateTimeSpan(TimeSteps[m_param.NumberOfTimeStep], 0, 0, 0));
            m_param.m_axis.apMaxCurrent = (DATE)(COleDateTime(m_param.m_axis.apMaxCurrent) - COleDateTimeSpan(TimeSteps[m_param.NumberOfTimeStep], 0, 0, 0));
            flag = true;
        }

        //шаг измеряется в часах
        if(m_param.NumberOfTimeStep >= 14 && m_param.NumberOfTimeStep < 20)
        {
            m_param.m_axis.apMinCurrent = (DATE)(COleDateTime(m_param.m_axis.apMinCurrent) - COleDateTimeSpan(0, TimeSteps[m_param.NumberOfTimeStep], 0, 0));
            m_param.m_axis.apMaxCurrent = (DATE)(COleDateTime(m_param.m_axis.apMaxCurrent) - COleDateTimeSpan(0, TimeSteps[m_param.NumberOfTimeStep], 0, 0));
            flag = true;
        }

        //шаг измеряется в минутах
        if(m_param.NumberOfTimeStep >= 20 && m_param.NumberOfTimeStep < 27)
        {
            m_param.m_axis.apMinCurrent = (DATE)(COleDateTime(m_param.m_axis.apMinCurrent) - COleDateTimeSpan(0, 0, TimeSteps[m_param.NumberOfTimeStep], 0));
            m_param.m_axis.apMaxCurrent = (DATE)(COleDateTime(m_param.m_axis.apMaxCurrent) - COleDateTimeSpan(0, 0, TimeSteps[m_param.NumberOfTimeStep], 0));
            flag = true;
        }

        //если шаг измеряется в секундах
        if(m_param.NumberOfTimeStep >= 27 && m_param.NumberOfTimeStep < 34)
        {
            m_param.m_axis.apMinCurrent = (DATE)(COleDateTime(m_param.m_axis.apMinCurrent) - COleDateTimeSpan(0, 0, 0, TimeSteps[m_param.NumberOfTimeStep]));
            m_param.m_axis.apMaxCurrent = (DATE)(COleDateTime(m_param.m_axis.apMaxCurrent) - COleDateTimeSpan(0, 0, 0, TimeSteps[m_param.NumberOfTimeStep]));
            flag = true;
        }

        //если шаг измеряется в милисекундах
        if(m_param.NumberOfTimeStep >= 34 && m_param.NumberOfTimeStep < 37)
        {
            m_param.m_axis.apMinCurrentMS -= TimeSteps[m_param.NumberOfTimeStep];
            m_param.m_axis.apMaxCurrentMS -= TimeSteps[m_param.NumberOfTimeStep];

            if(m_param.m_axis.apMinCurrentMS < 0)
            {
				m_param.m_axis.apMinCurrent -= (DATE)(COleDateTimeSpan(0, 0, 0, 1));
                m_param.m_axis.apMinCurrentMS = 1000 + m_param.m_axis.apMinCurrentMS;
            }
            
            if(m_param.m_axis.apMaxCurrentMS < 0)
            {
				m_param.m_axis.apMaxCurrent -= (DATE)(COleDateTimeSpan(0, 0, 0, 1));
                m_param.m_axis.apMaxCurrentMS = 1000 + m_param.m_axis.apMaxCurrentMS;
            }
            flag = true;
        }

    }

    if(rect == gRightShift || rect == gTopShift)
    {
        //если шаг измеряется в годах
        if(m_param.NumberOfTimeStep == 0)
        {
            year_min += TimeSteps[m_param.NumberOfTimeStep];
            year_max += TimeSteps[m_param.NumberOfTimeStep];
        }

        //если шаг измеряется в месяцах
        if(m_param.NumberOfTimeStep > 0 && m_param.NumberOfTimeStep < 6)
        {
            month_max += TimeSteps[m_param.NumberOfTimeStep];
            month_min += TimeSteps[m_param.NumberOfTimeStep];
        }
        
        //если шаг измеряется в днях
        if(m_param.NumberOfTimeStep >= 6 && m_param.NumberOfTimeStep < 14)
        {
            m_param.m_axis.apMinCurrent = (DATE)(COleDateTime(m_param.m_axis.apMinCurrent) + COleDateTimeSpan(TimeSteps[m_param.NumberOfTimeStep], 0, 0, 0));
            m_param.m_axis.apMaxCurrent = (DATE)(COleDateTime(m_param.m_axis.apMaxCurrent) + COleDateTimeSpan(TimeSteps[m_param.NumberOfTimeStep], 0, 0, 0));
            flag = true;
        }

        //шаг измеряется в часах
        if(m_param.NumberOfTimeStep >= 14 && m_param.NumberOfTimeStep < 20)
        {
            m_param.m_axis.apMinCurrent = (DATE)(COleDateTime(m_param.m_axis.apMinCurrent) + COleDateTimeSpan(0, TimeSteps[m_param.NumberOfTimeStep], 0, 0));
            m_param.m_axis.apMaxCurrent = (DATE)(COleDateTime(m_param.m_axis.apMaxCurrent) + COleDateTimeSpan(0, TimeSteps[m_param.NumberOfTimeStep], 0, 0));
            flag = true;
        }

        //если шаг измеряется в минутах
        if(m_param.NumberOfTimeStep >= 20 && m_param.NumberOfTimeStep < 27)
        {
            m_param.m_axis.apMinCurrent = (DATE)(COleDateTime(m_param.m_axis.apMinCurrent) + COleDateTimeSpan(0, 0, TimeSteps[m_param.NumberOfTimeStep], 0));
            m_param.m_axis.apMaxCurrent = (DATE)(COleDateTime(m_param.m_axis.apMaxCurrent) + COleDateTimeSpan(0, 0, TimeSteps[m_param.NumberOfTimeStep], 0));
            flag = true;
        }

        //если шаг измеряется в секундах
        if(m_param.NumberOfTimeStep >= 27 && m_param.NumberOfTimeStep < 34)
        {
            m_param.m_axis.apMinCurrent = (DATE)(COleDateTime(m_param.m_axis.apMinCurrent) + COleDateTimeSpan(0, 0, 0, TimeSteps[m_param.NumberOfTimeStep]));
            m_param.m_axis.apMaxCurrent = (DATE)(COleDateTime(m_param.m_axis.apMaxCurrent) + COleDateTimeSpan(0, 0, 0, TimeSteps[m_param.NumberOfTimeStep]));
            flag = true;
        }

        //если шаг измеряется в милисекундах
        if(m_param.NumberOfTimeStep >= 34 && m_param.NumberOfTimeStep < 37)
        {
            m_param.m_axis.apMinCurrentMS += TimeSteps[m_param.NumberOfTimeStep];
            m_param.m_axis.apMaxCurrentMS += TimeSteps[m_param.NumberOfTimeStep];

            if(m_param.m_axis.apMinCurrentMS > 999)
            {
				m_param.m_axis.apMinCurrent += (DATE)(COleDateTimeSpan(0, 0, 0, (int)m_param.m_axis.apMinCurrentMS / 1000));
                m_param.m_axis.apMinCurrentMS = m_param.m_axis.apMinCurrentMS%1000;
            }

            if(m_param.m_axis.apMaxCurrentMS > 999)
            {
				m_param.m_axis.apMaxCurrent += (DATE)(COleDateTimeSpan(0, 0, 0, (int)m_param.m_axis.apMaxCurrentMS / 1000));
                m_param.m_axis.apMaxCurrentMS = m_param.m_axis.apMaxCurrentMS%1000;
            }
            flag = true;
        }
    }

    if(rect == gNone)
        return;

    if(!flag)
    {
        if(month_min > 12)
        {
            year_min += month_min/12;
            month_min = month_min%12;
        }

        if(month_max > 12)
        {
            year_max += month_max/12;
            month_max = month_max%12;
        }

        if(month_min < 1)
        {
            year_min -= (1 + abs(month_min)/12);
            month_min = 12 + month_min%12;
        }

        if(month_max < 1)
        {
            year_max -= (1 + abs(month_max)/12);
            month_max = 12 + month_max%12;
        }

        m_param.m_axis.apMinCurrent =
            (DATE)COleDateTime(year_min, month_min, day_min, hour_min, minute_min, second_min);

        m_param.m_axis.apMaxCurrent =
            (DATE)COleDateTime(year_max, month_max, day_max, hour_max, minute_max, second_max);
    }

    if(m_param.m_axis.apMinCurrent < m_param.m_axis.apBegin){
        COleDateTimeSpan tmp = COleDateTime(m_param.m_axis.apMaxCurrent) - COleDateTime(m_param.m_axis.apMinCurrent);
        m_param.m_axis.apMinCurrent = m_param.m_axis.apBegin;
        m_param.m_axis.apMaxCurrent = (DATE)(COleDateTime(m_param.m_axis.apMinCurrent) + tmp);
    }

    if(m_param.m_axis.apMaxCurrent > m_param.m_axis.apEnd){
        COleDateTimeSpan tmp = COleDateTime(m_param.m_axis.apMaxCurrent) - COleDateTime(m_param.m_axis.apMinCurrent);
        m_param.m_axis.apMaxCurrent = m_param.m_axis.apEnd;
        m_param.m_axis.apMinCurrent = (DATE)(COleDateTime(m_param.m_axis.apMaxCurrent) - tmp);
    }

    //SetAxis(min, max);
    FillGridArray();
}
//------------------------------------------------------------------------------
void CTrendGrid::Scale(zRect rect)
{
    int step=0,
        year_min, month_min, day_min, hour_min, minute_min, second_min, millisecond_min,
        year_max, month_max, day_max, hour_max, minute_max, second_max, millisecond_max;

    bool Flag = false;

    year_min = COleDateTime(m_param.m_axis.apMinCurrent).GetYear();
    month_min = COleDateTime(m_param.m_axis.apMinCurrent).GetMonth();
    day_min = COleDateTime(m_param.m_axis.apMinCurrent).GetDay();
    hour_min = COleDateTime(m_param.m_axis.apMinCurrent).GetHour();
    minute_min = COleDateTime(m_param.m_axis.apMinCurrent).GetMinute();
    second_min = COleDateTime(m_param.m_axis.apMinCurrent).GetSecond();
    millisecond_min = m_param.m_axis.apMinCurrentMS;

    year_max = COleDateTime(m_param.m_axis.apMaxCurrent).GetYear();
    month_max = COleDateTime(m_param.m_axis.apMaxCurrent).GetMonth();
    day_max = COleDateTime(m_param.m_axis.apMaxCurrent).GetDay();
    hour_max = COleDateTime(m_param.m_axis.apMaxCurrent).GetHour();
    minute_max = COleDateTime(m_param.m_axis.apMaxCurrent).GetMinute();
    second_max = COleDateTime(m_param.m_axis.apMaxCurrent).GetSecond();
    millisecond_max = m_param.m_axis.apMaxCurrentMS;

    int QntOfMarks;
    QntOfMarks = m_param.m_axis.apQntBoldMarks;

    if(rect == gLeftScale || rect == gBottomScale)
    {
        //если шаг измеряется в годах
        if(m_param.NumberOfTimeStep == 0)
        {
            step = TimeSteps[m_param.NumberOfTimeStep];
            Flag = true;

            if(m_param.m_axis.apFitType == ftCenter)
            {
                year_max += step/2.0;
                year_min -= step/2.0;
            }

            if(m_param.m_axis.apFitType == ftLeft)
                year_max += step;

            if(m_param.m_axis.apFitType == ftRight)
                year_min -= step;
        }

        //если шаг измеряется в месяцах
        if(m_param.NumberOfTimeStep > 0 && m_param.NumberOfTimeStep < 6)
        {
            if(m_param.NumberOfTimeStep-1 < 1)
                step = (TimeSteps[m_param.NumberOfTimeStep-1]*12-TimeSteps[m_param.NumberOfTimeStep])*QntOfMarks; //считаем, насколько месяцев увеличится интервал при выборе нового шага

            else
                step = (TimeSteps[m_param.NumberOfTimeStep-1]-TimeSteps[m_param.NumberOfTimeStep])*QntOfMarks;
            Flag = true;

            if(m_param.m_axis.apFitType == ftCenter)
            {
                month_max += step/2;
                month_min -= step/2;
            }

            if(m_param.m_axis.apFitType == ftLeft)
                month_max += step;

            if(m_param.m_axis.apFitType == ftRight)
                month_min -= step;
        }

        //если шаг измеряется в днях
        if(m_param.NumberOfTimeStep >= 6 && m_param.NumberOfTimeStep < 14)
        {
            if(m_param.NumberOfTimeStep - 1 < 6)
                step = (TimeSteps[m_param.NumberOfTimeStep-1]*30-TimeSteps[m_param.NumberOfTimeStep])*QntOfMarks; //считаем, насколько дней увеличится интервал при выборе нового шага
            else
                step = (TimeSteps[m_param.NumberOfTimeStep-1]-TimeSteps[m_param.NumberOfTimeStep])*QntOfMarks; //считаем, насколько дней увеличится интервал при выборе нового шага
            
            if(m_param.m_axis.apFitType == ftCenter)
            {
                m_param.m_axis.apMinCurrent = (DATE)(COleDateTime(m_param.m_axis.apMinCurrent) - COleDateTimeSpan(step/2, 0, 0, 0));
                m_param.m_axis.apMaxCurrent = (DATE)(COleDateTime(m_param.m_axis.apMaxCurrent) + COleDateTimeSpan(step/2, 0, 0, 0));
            }

            if(m_param.m_axis.apFitType == ftLeft)
                m_param.m_axis.apMaxCurrent = (DATE)(COleDateTime(m_param.m_axis.apMaxCurrent) + COleDateTimeSpan(step, 0, 0, 0));

            if(m_param.m_axis.apFitType == ftRight)
                m_param.m_axis.apMinCurrent = (DATE)(COleDateTime(m_param.m_axis.apMinCurrent) - COleDateTimeSpan(step, 0, 0, 0));
        }
        
        //шаг измеряется в часах
        if(m_param.NumberOfTimeStep >= 14 && m_param.NumberOfTimeStep < 20)
        {
            if(m_param.NumberOfTimeStep - 1 < 14)
                step = (TimeSteps[m_param.NumberOfTimeStep-1]*24-TimeSteps[m_param.NumberOfTimeStep])*QntOfMarks; //считаем, насколько часов увеличится интервал при выборе нового шага
            else
                step = (TimeSteps[m_param.NumberOfTimeStep-1]-TimeSteps[m_param.NumberOfTimeStep])*QntOfMarks; //считаем, насколько часов увеличится интервал при выборе нового шага
            
            if(m_param.m_axis.apFitType == ftCenter)
            {
                m_param.m_axis.apMinCurrent = (DATE)(COleDateTime(m_param.m_axis.apMinCurrent) - COleDateTimeSpan(0, step/2, 0, 0));
                m_param.m_axis.apMaxCurrent = (DATE)(COleDateTime(m_param.m_axis.apMaxCurrent) + COleDateTimeSpan(0, step/2, 0, 0));
            }

            if(m_param.m_axis.apFitType == ftLeft)
                m_param.m_axis.apMaxCurrent = (DATE)(COleDateTime(m_param.m_axis.apMaxCurrent) + COleDateTimeSpan(0, step, 0, 0));

            if(m_param.m_axis.apFitType == ftRight)
                m_param.m_axis.apMinCurrent = (DATE)(COleDateTime(m_param.m_axis.apMinCurrent) - COleDateTimeSpan(0, step, 0, 0));
        }

        //если шаг измеряется в минутах
        if(m_param.NumberOfTimeStep >= 20 && m_param.NumberOfTimeStep < 26)
        {
            if(m_param.NumberOfTimeStep -1 < 20)
                step = (TimeSteps[m_param.NumberOfTimeStep-1]*60-TimeSteps[m_param.NumberOfTimeStep])*QntOfMarks; //считаем, насколько часов увеличится интервал при выборе нового шага
            else
                step = (TimeSteps[m_param.NumberOfTimeStep-1]-TimeSteps[m_param.NumberOfTimeStep])*QntOfMarks; //считаем, насколько часов увеличится интервал при выборе нового шага

            if(m_param.m_axis.apFitType == ftCenter)
            {
                m_param.m_axis.apMinCurrent = (DATE)(COleDateTime(m_param.m_axis.apMinCurrent) - COleDateTimeSpan(0, 0, step/2, 0));
                m_param.m_axis.apMaxCurrent = (DATE)(COleDateTime(m_param.m_axis.apMaxCurrent) + COleDateTimeSpan(0, 0, step/2, 0));
            }

            if(m_param.m_axis.apFitType == ftLeft)
                m_param.m_axis.apMaxCurrent = (DATE)(COleDateTime(m_param.m_axis.apMaxCurrent) + COleDateTimeSpan(0, 0, step, 0));

            if(m_param.m_axis.apFitType == ftRight)
                m_param.m_axis.apMinCurrent = (DATE)(COleDateTime(m_param.m_axis.apMinCurrent) - COleDateTimeSpan(0, 0, step, 0));
        }

        //если шаг измеряется в секундах
        if(m_param.NumberOfTimeStep >= 26 && m_param.NumberOfTimeStep < 34)
        {
            if(m_param.NumberOfTimeStep - 1 < 27)
                step = (TimeSteps[m_param.NumberOfTimeStep-1]*60-TimeSteps[m_param.NumberOfTimeStep])*QntOfMarks; //считаем, на сколько секунд увеличится интервал при выборе нового шага
            else
                step = (TimeSteps[m_param.NumberOfTimeStep-1]-TimeSteps[m_param.NumberOfTimeStep])*QntOfMarks; //считаем, на сколько секунд увеличится интервал при выборе нового шага

            if(m_param.m_axis.apFitType == ftCenter)
            {
                m_param.m_axis.apMinCurrent = (DATE)(COleDateTime(m_param.m_axis.apMinCurrent) - COleDateTimeSpan(0, 0, 0, step/2));
                m_param.m_axis.apMaxCurrent = (DATE)(COleDateTime(m_param.m_axis.apMaxCurrent) + COleDateTimeSpan(0, 0, 0, step/2));
            }

            if(m_param.m_axis.apFitType == ftLeft)
                m_param.m_axis.apMaxCurrent = (DATE)(COleDateTime(m_param.m_axis.apMaxCurrent) + COleDateTimeSpan(0, 0, 0, step));

            if(m_param.m_axis.apFitType == ftRight)
                m_param.m_axis.apMinCurrent = (DATE)(COleDateTime(m_param.m_axis.apMinCurrent) - COleDateTimeSpan(0, 0, 0, step));
        }

        //если шаг измеряется в милисекундах
        if(m_param.NumberOfTimeStep >= 34 && m_param.NumberOfTimeStep < 37)
        {
            if(m_param.NumberOfTimeStep - 1 < 34)
                step = (TimeSteps[m_param.NumberOfTimeStep-1]*1000-TimeSteps[m_param.NumberOfTimeStep])*QntOfMarks; //считаем, на сколько милисекунд увеличится интервал при выборе нового шага
            else
                step = (TimeSteps[m_param.NumberOfTimeStep-1]-TimeSteps[m_param.NumberOfTimeStep])*QntOfMarks; //считаем, на сколько милисекунд увеличится интервал при выборе нового шага

            if(m_param.m_axis.apFitType == ftCenter)
            {
				millisecond_min -= step/2.0;
				millisecond_max += step/2.0;
            }

            if(m_param.m_axis.apFitType == ftLeft)
            {
                millisecond_max += step;

            }

            if(m_param.m_axis.apFitType == ftRight)
            {
                millisecond_min -= step;
            }
        }
    }

    if(rect == gRightScale || rect == gTopScale)
    {
        //если шаг измеряется в годах
        if(m_param.NumberOfTimeStep == 0)
        {
            step = 0.5*QntOfMarks;
            Flag = true;

            if(m_param.m_axis.apFitType == ftCenter)
            {
                year_max -=  step/2;
                year_min +=  step/2;
            }

            if(m_param.m_axis.apFitType == ftLeft)
                year_max -= step;

            if(m_param.m_axis.apFitType == ftRight)
                year_min += step;
        }

        //если шаг измеряется в месяцах
        if(m_param.NumberOfTimeStep > 0 && m_param.NumberOfTimeStep < 5)
        {
            step = (TimeSteps[m_param.NumberOfTimeStep]-TimeSteps[m_param.NumberOfTimeStep+1])*QntOfMarks; //считаем, насколько месяцев уменьшится интервал при выборе нового шага
            Flag = true;

            if(m_param.m_axis.apFitType == ftCenter)
            {
                month_min += step/2;
                month_max -= step/2;
            }

            if(m_param.m_axis.apFitType == ftLeft)
                month_max -= step;

            if(m_param.m_axis.apFitType == ftRight)
                month_min += step;
        }

        //если шаг измеряется в днях
        if(m_param.NumberOfTimeStep >= 5 && m_param.NumberOfTimeStep < 13)
        {
            if(m_param.NumberOfTimeStep < 6)
                step = (TimeSteps[m_param.NumberOfTimeStep]*30-TimeSteps[m_param.NumberOfTimeStep+1])*QntOfMarks; //считаем, насколько дней увеличится интервал при выборе нового шага
            else
                step = (TimeSteps[m_param.NumberOfTimeStep]-TimeSteps[m_param.NumberOfTimeStep+1])*QntOfMarks; //считаем, насколько дней увеличится интервал при выборе нового шага

            if(m_param.m_axis.apFitType == ftCenter)
            {
                m_param.m_axis.apMinCurrent = (DATE)(COleDateTime(m_param.m_axis.apMinCurrent) + COleDateTimeSpan(step/2, 0, 0, 0));
                m_param.m_axis.apMaxCurrent = (DATE)(COleDateTime(m_param.m_axis.apMaxCurrent) - COleDateTimeSpan(step/2, 0, 0, 0));
            }

            if(m_param.m_axis.apFitType == ftLeft)
                m_param.m_axis.apMaxCurrent = (DATE)(COleDateTime(m_param.m_axis.apMaxCurrent) - COleDateTimeSpan(step, 0, 0, 0));

            if(m_param.m_axis.apFitType == ftRight)
                m_param.m_axis.apMinCurrent = (DATE)(COleDateTime(m_param.m_axis.apMinCurrent) + COleDateTimeSpan(step, 0, 0, 0));
        }

        //если шаг измеряется в часах
        if(m_param.NumberOfTimeStep >= 13 && m_param.NumberOfTimeStep < 19)
        {
            if(m_param.NumberOfTimeStep < 14)
                step = (TimeSteps[m_param.NumberOfTimeStep]*24-TimeSteps[m_param.NumberOfTimeStep+1])*QntOfMarks; //считаем, насколько часов увеличится интервал при выборе нового шага
            else
                step = (TimeSteps[m_param.NumberOfTimeStep]-TimeSteps[m_param.NumberOfTimeStep+1])*QntOfMarks; //считаем, насколько часов увеличится интервал при выборе нового шага

            if(m_param.m_axis.apFitType == ftCenter)
            {
                m_param.m_axis.apMinCurrent = (DATE)(COleDateTime(m_param.m_axis.apMinCurrent) + COleDateTimeSpan(0, step/2, 0, 0));
                m_param.m_axis.apMaxCurrent = (DATE)(COleDateTime(m_param.m_axis.apMaxCurrent) - COleDateTimeSpan(0, step/2, 0, 0));
            }

            if(m_param.m_axis.apFitType == ftLeft)
                m_param.m_axis.apMaxCurrent = (DATE)(COleDateTime(m_param.m_axis.apMaxCurrent) - COleDateTimeSpan(0, step, 0, 0));

            if(m_param.m_axis.apFitType == ftRight)
                m_param.m_axis.apMinCurrent = (DATE)(COleDateTime(m_param.m_axis.apMinCurrent) + COleDateTimeSpan(0, step, 0, 0));
        }

        //если шаг измеряется в минутах
        if(m_param.NumberOfTimeStep >= 19 && m_param.NumberOfTimeStep < 26)
        {
            if(m_param.NumberOfTimeStep < 20)
                step = (TimeSteps[m_param.NumberOfTimeStep]*60-TimeSteps[m_param.NumberOfTimeStep+1])*QntOfMarks; //считаем, насколько часов увеличится интервал при выборе нового шага
            else
                step = (TimeSteps[m_param.NumberOfTimeStep]-TimeSteps[m_param.NumberOfTimeStep+1])*QntOfMarks; //считаем, насколько часов увеличится интервал при выборе нового шага

            if(m_param.m_axis.apFitType == ftCenter)
            {
                m_param.m_axis.apMinCurrent = (DATE)(COleDateTime(m_param.m_axis.apMinCurrent) + COleDateTimeSpan(0, 0, step/2, 0));
                m_param.m_axis.apMaxCurrent = (DATE)(COleDateTime(m_param.m_axis.apMaxCurrent) - COleDateTimeSpan(0, 0, step/2, 0));
            }

            if(m_param.m_axis.apFitType == ftLeft)
                m_param.m_axis.apMaxCurrent = (DATE)(COleDateTime(m_param.m_axis.apMaxCurrent) - COleDateTimeSpan(0, 0, step, 0));

            if(m_param.m_axis.apFitType == ftRight)
                m_param.m_axis.apMinCurrent = (DATE)(COleDateTime(m_param.m_axis.apMinCurrent) + COleDateTimeSpan(0, 0, step, 0));
        }

        //если шаг измеряется в секундах
        if(m_param.NumberOfTimeStep >= 26 && m_param.NumberOfTimeStep < 33)
        {
            if(m_param.NumberOfTimeStep < 27)
                step = (TimeSteps[m_param.NumberOfTimeStep]*60-TimeSteps[m_param.NumberOfTimeStep+1])*QntOfMarks; //считаем, на сколько секунд увеличится интервал при выборе нового шага
            else
                step = (TimeSteps[m_param.NumberOfTimeStep]-TimeSteps[m_param.NumberOfTimeStep+1])*QntOfMarks; //считаем, на сколько секунд увеличится интервал при выборе нового шага

            if(m_param.m_axis.apFitType == ftCenter)
            {
                m_param.m_axis.apMinCurrent = (DATE)(COleDateTime(m_param.m_axis.apMinCurrent) + COleDateTimeSpan(0, 0, 0, step/2));
                m_param.m_axis.apMaxCurrent = (DATE)(COleDateTime(m_param.m_axis.apMaxCurrent) - COleDateTimeSpan(0, 0, 0, step/2));
            }

            if(m_param.m_axis.apFitType == ftLeft)
                m_param.m_axis.apMaxCurrent = (DATE)(COleDateTime(m_param.m_axis.apMaxCurrent) - COleDateTimeSpan(0, 0, 0, step));

            if(m_param.m_axis.apFitType == ftRight)
                m_param.m_axis.apMinCurrent = (DATE)(COleDateTime(m_param.m_axis.apMinCurrent) + COleDateTimeSpan(0, 0, 0, step));
        }

        //если шаг измеряется в миллисекундах
        if(m_param.NumberOfTimeStep >= 33 && m_param.NumberOfTimeStep < 36)
        {
            if(m_param.NumberOfTimeStep < 34)
                step = (TimeSteps[m_param.NumberOfTimeStep]*1000-TimeSteps[m_param.NumberOfTimeStep+1])*QntOfMarks; //считаем, на сколько милисекунд увеличится интервал при выборе нового шага
            else
                step = (TimeSteps[m_param.NumberOfTimeStep]-TimeSteps[m_param.NumberOfTimeStep+1])*QntOfMarks; //считаем, на сколько милисекунд увеличится интервал при выборе нового шага

            if(m_param.m_axis.apFitType == ftCenter)
            {

            }

            if(m_param.m_axis.apFitType == ftLeft)
            {
                millisecond_max -= step;

            }

            if(m_param.m_axis.apFitType == ftRight)
            {
                millisecond_min += step;
            }
        }

    }

    if(rect == gCenterAutoScale)
    {

    }

    if(rect == gNone)
        return;


    //если одна из переменных отрицательна для меньшей временной границы
    if(millisecond_min < 0)
    {
        m_param.m_axis.apMinCurrent = COleDateTime(m_param.m_axis.apMinCurrent)
            - COleDateTimeSpan(0, 0, 0, 1 + abs(millisecond_min)/1000);
        m_param.m_axis.apMinCurrentMS = 1000 + millisecond_min%1000;
    }

    if(month_min < 1)
    {
        year_min -= (1 + abs(month_min)/12);
        month_min = 12 + month_min%12;
    }

    //если одна из переменных отрицательна для большей временной границы

    if(millisecond_max < 0)
    {
        m_param.m_axis.apMaxCurrent = COleDateTime(m_param.m_axis.apMaxCurrent)
            - COleDateTimeSpan(0, 0, 0, 1 + abs(millisecond_max)/1000);
        m_param.m_axis.apMaxCurrentMS = 1000 + millisecond_max%1000;
    }

    if(month_max < 1)
    {
        year_max -= (1 + abs(month_max)/12);
        month_max = 12 + month_max%12;
    }

    //если происходит переполнение одной из переменных для меньшей временнной границы

    if(millisecond_min > 999)
    {
        m_param.m_axis.apMinCurrent = COleDateTime(m_param.m_axis.apMinCurrent)
            + COleDateTimeSpan(0, 0, 0, millisecond_min/1000);
        m_param.m_axis.apMinCurrentMS = millisecond_min%1000;
    }

    if(month_min > 12)
    {
        year_min += month_min/12;
        month_min = month_min%12;
        if(month_min == 0)
            month_min = 12;
    }

    //если происходит переполнение одной из переменных для большей временнной границы

    if(millisecond_max > 999)
    {
        m_param.m_axis.apMaxCurrent = COleDateTime(m_param.m_axis.apMaxCurrent)
            + COleDateTimeSpan(0, 0, 0, millisecond_max/1000);
        m_param.m_axis.apMaxCurrentMS = millisecond_max%1000;
    }

    if(month_max > 12)
    {
        year_max += month_max/12;
        month_max = month_max%12;
        if(month_max == 0)
            month_max = 12;
    }

    if(Flag)
    {
        m_param.m_axis.apMaxCurrent = (DATE)COleDateTime(year_max, month_max,
            COleDateTime(m_param.m_axis.apMaxCurrent).GetDay(),
            COleDateTime(m_param.m_axis.apMaxCurrent).GetHour(),
            COleDateTime(m_param.m_axis.apMaxCurrent).GetMinute(),
            COleDateTime(m_param.m_axis.apMaxCurrent).GetSecond());

        m_param.m_axis.apMinCurrent = (DATE)COleDateTime(year_min, month_min,
            COleDateTime(m_param.m_axis.apMinCurrent).GetDay(),
            COleDateTime(m_param.m_axis.apMinCurrent).GetHour(),
            COleDateTime(m_param.m_axis.apMinCurrent).GetMinute(),
            COleDateTime(m_param.m_axis.apMinCurrent).GetSecond());
    }

    if(m_param.m_axis.apMinCurrent < m_param.m_axis.apBegin){
        COleDateTimeSpan tmp = COleDateTime(m_param.m_axis.apMaxCurrent) - COleDateTime(m_param.m_axis.apMinCurrent);
        m_param.m_axis.apMinCurrent = m_param.m_axis.apBegin;
        m_param.m_axis.apMaxCurrent = (DATE)(COleDateTime(m_param.m_axis.apMinCurrent) + tmp);
    }

    if(m_param.m_axis.apMaxCurrent > m_param.m_axis.apEnd){
        COleDateTimeSpan tmp = COleDateTime(m_param.m_axis.apMaxCurrent) - COleDateTime(m_param.m_axis.apMinCurrent);
        m_param.m_axis.apMaxCurrent = m_param.m_axis.apEnd;
        m_param.m_axis.apMinCurrent = (DATE)(COleDateTime(m_param.m_axis.apMaxCurrent) - tmp);
    }

    if((m_param.m_axis.apMaxCurrent >= m_param.m_axis.apEnd) && (m_param.m_axis.apMinCurrent <= m_param.m_axis.apBegin))
    {
        m_param.m_axis.apMinCurrent = m_param.m_axis.apBegin;
        m_param.m_axis.apMaxCurrent = m_param.m_axis.apEnd;
    }

    FillGridArray();
}
//------------------------------------------------------------------------------
void CTrendGrid::OnLButtonDown(UINT nFlags, CPoint point)
{
    if(m_param.m_axis.apGridType == gtVertical)
        m_pOwner->OnLButtonDown(nFlags, point);
}
//------------------------------------------------------------------------------
void CTrendGrid::SetGridColor(COLORREF GridColor)
{
	m_pDTrendGrid->SetGridColor(GridColor);
}
//------------------------------------------------------------------------------