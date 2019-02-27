//------------------------------------------------------------------------------
// OscillGrid.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include <Interface2012\Custom\CustomGrid.h>
#include <Interface2012\Draw\DOscillGrid.h>

//#define QNT_MARKS_X 11 //количество меток
//#define QNT_MARKS_Y 9 //количество меток
#define Epsilon 0.00000001

const double LOG_GRID_SCALE_STEP = 0.8;

struct OscillLogGrid : public GridParam{
    OscillLogGrid()
	{
        m_axis.apScaleType = stLog;
        m_axis.apBegin = -1000;
        m_axis.apEnd = 1000;
        m_axis.apGridType = gtHorizontal;
        m_axis.apMaxCurrent = 100;
        m_axis.apMinCurrent = 0;
        m_axis.apStep = 0;
        m_axis.apShift = 10;
        m_axis.apScale = 2;
        m_axis.apTitle = L"Ось Х";
        m_axis.apFitType = ftLeft;
        m_axis.apQntMarks = 5;
        //m_axis.apFont = CDFonts();
        m_axis.apMinStep = 1;
        m_axis.apZStep.zfFractional = 1;
        m_axis.apZStep.zfInteger = 10;
        m_axis.apZMinCurrent.zfInteger = 0;
        m_axis.apZMinCurrent.zfFractional = 0;
        m_axis.apZMinCurrent.zfSign = false;
        
    }
	~OscillLogGrid()
	{

	}
};

class COscillLogGrid : public CCustomGrid
{
public:
    COscillLogGrid(CCustomContainer *owner, CRect rect, GridType gridType);
    virtual ~COscillLogGrid();
	virtual void Clear (void);
	double FromWorldToScreen(double val, const AxisParameters &axis, const CRect &rect) override;
private:
    CDOscillGrid *m_pDOscillGrid;
    OscillLogGrid m_param;

	CRect _prevMarkTextRect;

    void FillGridArray();
	void FillInnerGrid(double highVal, double lowVal, double step, double curDiff);
	void MakeMark(double val, double curDiff, bool isBold);
    void MakeText(double val, double curDiff, GridType type);
    bool MakeTextRect(double val, GridType type);
    //float GetRoundStep(int Pow, double val);

	inline double getLogValue(double val);
	inline double getMinRoundLogValue(double val);
	inline double getMaxRoundLogValue(double val);
	//double getMinRoundValueByLog(double val);
	//double getMaxRoundValueByLog(double val);

protected:

public:
    void SetAxisTitle(CString title, GridType type);

    void SetAxis(double min, double max);
	void SetMinStep(double MinStep) override;

    void Shift(zRect rect);
    void Scale(zRect rect);

    void AutoScale(double minValue, double maxValue);
    virtual void OnLButtonDown(UINT nFlags, CPoint point);
};
//------------------------------------------------------------------------------