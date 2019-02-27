//------------------------------------------------------------------------------
// OscillGrid.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include <Interface2012\Custom\CustomGrid.h>
#include <Interface2012\Draw\DOscillGrid.h>

#define QNT_MARKS_X 11 //количество меток
#define QNT_MARKS_Y 11 //количество меток
#define Epsilon 0.00000001

struct OscillGrid : public GridParam{
    OscillGrid()
	{
        m_axis.apScaleType = stLinear;
        m_axis.apBegin = -1000;
        m_axis.apEnd = 1000;
        m_axis.apGridType = gtHorizontal;
        m_axis.apMaxCurrent = 160;
        m_axis.apMinCurrent = 0;
        m_axis.apStep = 20;
        m_axis.apShift = 10;
        m_axis.apScale = 2;
        m_axis.apTitle = L"Ось Х";
        m_axis.apFitType = ftLeft;
        m_axis.apQntMarks = QNT_MARKS_X;
        //m_axis.apFont = CDFonts();
        m_axis.apMinStep = 1;
        m_axis.apZStep.zfFractional = 1;
        m_axis.apZStep.zfInteger = 2;
        m_axis.apZMinCurrent.zfInteger = 0;
        m_axis.apZMinCurrent.zfFractional = 0;
        m_axis.apZMinCurrent.zfSign = false;
        
    }
	~OscillGrid()
	{

	}
};

class COscillGrid : public CCustomGrid
{
public:
    COscillGrid(CCustomContainer *owner, CRect rect, GridType gridType);
    virtual ~COscillGrid();
	virtual void Clear (void);
private:
    CDOscillGrid *m_pDOscillGrid;
    OscillGrid m_param;
    void FillGridArray();
    void MakeText(int Counter, GridType type);
    void MakeTextRect(double val, GridType type);
	void SieveTextMarks();
    float GetRoundStep(int Pow, double val);
protected:

public:
    void SetAxisTitle(CString title, GridType type);

    void SetAxis(double min, double max);

    void Shift(zRect rect);
    void Scale(zRect rect);

	virtual void Resize(CRect rect) override;

    void AutoScale(double minValue, double maxValue);
    virtual void OnLButtonDown(UINT nFlags, CPoint point);

	virtual void SetGridColor(COLORREF GridColor);
};
//------------------------------------------------------------------------------