//------------------------------------------------------------------------------
// TrendGrid.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include <Interface2012\Custom\CustomGrid.h>
#include <Interface2012\Draw\DTrendGrid.h>

//константы шагов
const int TimeSteps[37] = {1, /*год*/
    6, 4, 3, 2, 1, /*месяцы*/
    15, 10, 6, 5, 4, 3, 2, 1, /*дни*/
    12, 6, 4, 3, 2, 1, /*часы*/
    30, 20, 15, 10, 5, 2, 1, /*минуты*/
    30, 20, 15, 10, 5, 2, 1, /*секунды*/
    500, 200, 100 /*милисекунды*/};

const int DaysInMonthLeap[12] = {31, 29, 31, 30, 31, 30, 31,
    31, 30, 31, 30, 31};

struct TrendGrid : public GridParam
{
    //std::vector<CTime> TimeMarks; //массив временных меток 
    std::vector<COleDateTime> TimeMarks; //массив временных меток 
    int NumberOfTimeStep; //определяет текущий шаг в единицах времени по таблице констант шагов

    TrendGrid(){
        m_axis.apMinCurrent = (DATE)COleDateTime(2000, 3, 29, 7, 9, 11);
        m_axis.apMaxCurrent = (DATE)COleDateTime(2000, 3, 29, 7, 20, 17);
        m_axis.apScaleType = stLinear;
        m_axis.apBegin = (DATE)COleDateTime(1900, 1, 1, 0, 0, 0);
        m_axis.apEnd = (DATE)COleDateTime(2100, 1, 1, 1, 0, 0);
        m_axis.apGridType = gtHorizontal;
        m_axis.apStep = 10;
        m_axis.apShift = 10;
        m_axis.apScale = 2;
        m_axis.apTitle = L"Ось";
        m_axis.apFitType = ftLeft;
        m_axis.apQntMarks = MAX_QNT_MARKS;
        m_axis.apMinStep = 1;
        m_axis.apZStep.zfFractional = 1;
        m_axis.apZStep.zfInteger = 10;
        m_axis.apZMinCurrent.zfInteger = 0;
        m_axis.apZMinCurrent.zfFractional = 0;
        m_axis.apZMinCurrent.zfSign = false;
        m_axis.apCurrentTime = COleDateTime::GetCurrentTime();
        NumberOfTimeStep = 1;
    }
    ~TrendGrid()
    {
		Clear();
    }
	void Clear()
	{
		GridParam::Clear();
		if (!TimeMarks.empty())
			TimeMarks.clear();
	}
};

class CTrendGrid : public CCustomGrid
{
public:
    CTrendGrid(CCustomContainer *owner, CRect rect, GridType gridType);
    virtual ~CTrendGrid();
    virtual void Clear (void);
private:
    CDTrendGrid *m_pDTrendGrid;
    TrendGrid m_param;
    void FillGridArray();
    void MakeOverText(COleDateTime time);
    void MakeUnderText(COleDateTime time);
    void MakeUnderText(COleDateTime time, int milliseconds);
    void MakeOverTextRect(double val, GridType type);
    void MakeUnderTextRect(double val, GridType type);
    void SieveTextMarks();
    float GetRoundStep(int Pow, double val);
    TimeInterval GetInterval();
    void Shift(zRect rect);
    void Scale(zRect rect);
protected:

public:
    void SetAxis(double min, double max);
    void SetBounds(CTime Begin, CTime End);
    void SetAxisTitle(CString title, GridType type);
	virtual void OnLButtonDown(UINT nFlags, CPoint point) override;
	virtual void SetGridColor(COLORREF GridColor) override;
	virtual void Resize(CRect rect) override;
};