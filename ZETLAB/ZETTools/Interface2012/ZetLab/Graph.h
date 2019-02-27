//------------------------------------------------------------------------------
// Graph.h : ���� ���������
//------------------------------------------------------------------------------
#pragma once
#include <Interface2012\Custom\CustomContainer.h>
#include <Interface2012\ZetLab\OscillGrid.h>
#include <Interface2012\ZetLab\SimpleGrid.h>
#include <Interface2012\ZetLab\OscillLogGrid.h>
#include <Interface2012\ZetLab\TrendGrid.h>
#include <Interface2012\ZetLab\OscillPlot.h>
#include <Interface2012\ZetLab\SpectrBar.h>
#include <Interface2012\Draw\DGraph.h>
#include <Interface2012\ZetLab\Button.h>
#include <Interface2012\ZetLab\CursorField.h>
#include <Interface2012\Custom\FunctionForAll.h>
//#include "Resource.h"
#include <vector>

//------------------------------------------------------------------------------
//���������, ���������� ��� ����������� ���������� � �������
struct CursorInfo
{
    COLORREF ciColor; //���� ����� ������ "�����-���������"
    CString ciTextX; //����� ��� ����������� �������� �� ��� �
    CString ciTextY; //����� ��� ����������� �������� �� ��� Y
    double ciX; //�������� ������� � ����� �� ��� X
    double ciY; //�������� ������� � ����� �� ��� Y
    CRect ciRect; //�������� ������� ����� ����� ������� (�������������)
};

struct GraphParam
{
	CRect rect;
	PlotType plotType;
	double leftBorder;
	double rightBorder;
	double topBorder;
	double bottomBorder;
	int qntHorCursors;
	int qntVertCursors;
	bool createButtons;
};

class CGraph : public CCustomContainer
{
public:
    CGraph(CCustomContainer *owner, CRect rect, PlotType plotType, double leftBorder, 
        double rightBorder, double topBorder, double bottomBorder,
        int qntHorCursors, int qntVertCursors);
	CGraph(CCustomContainer *owner, GraphParam param);
    virtual ~CGraph();
private:
	//����� ��������� �������
    CDGraph *m_pDGraph;
	//������ ��������� �������, ������� �� ��������
    CZButton *m_pTopShiftButton;
	CSimpleGrid *m_pSimpleGridX, *m_pSimpleGridY;
    COscillGrid *m_pOscillGridX, *m_pOscillGridY;
	COscillLogGrid *m_pOscillLogGridX, *m_pOscillLogGridY;
    CTrendGrid *m_pTrendGridX, *m_pTrendGridY;
    CCustomGrid *m_pCustomGridX, *m_pCustomGridY;
    COscillPlot *m_pOscillPlot;
	CSpectrBar *m_pSpectrBar;
    CCustomPlot *m_pCustomPlot;
	std::vector<CCustomPlot*> m_pCustomPlotVector; //��� ���������� ����������� ���������� ��������
    CCursorField *m_pHorCursor, *m_pVertCursor;
	PlotType m_plotType; //��� �������
    bool m_setSettingsType;
	bool m_createButtons; //���� ������� ��� ���������� ������ �������� (true - ���������, false - ���)
    CRect m_rect, m_leftShiftRect, m_rightShiftRect, m_topShiftRect,
        m_bottomShiftRect, m_leftScaleRect, m_rightScaleRect,
        m_topScaleRect, m_bottomScaleRect, m_centerAutoScaleRect,
        m_gridRect, m_borderRect, m_HorCursorRect, m_VertCursorRect, m_decorativeRect;
    double m_maxX, m_minX, m_maxY, m_minY;
    double m_leftBorder, m_rightBorder, m_topBorder, m_bottomBorder;
    ScaleType m_scaleTypeX, m_scaleTypeY;
    std::vector<CursorInfo> m_cursorInfo;
    int m_qntOfHorCursors, m_qntOfVertCursors;
    void SetScaleShiftRect();
	bool m_invertation=false;
	int m_activePlot;
protected:
    
public:
    // ������� ���������� �����������

    void SetData(std::vector<double> &pDataX, std::vector<double> &pDataY, int plotNumber = 0, int quality = 0);
	void SetData(std::vector<double> &pDataY, int plotNumber = 0, int quality = 0);
    
	void SetDataFloat(std::vector<float> &pDataY, int plotNumber = 0, int quality = 0);

    long SetSpace(double pointOfStart, double pointOfEnd, double dx, double dy = 1.0); //����������� � ������������ �������� �� ��� �, ��������� � �����
    double SetSpace(double pointOfStart, double pointOfEnd, long Qnt, double dy = 1.0); //����������� � ������������ �������� �� ��� �, ��������� � ����������� �����

    void SetAxisX(double minX, double maxX);
    void SetAxisY(double minY, double maxY);
    void SetCurrentTime(double NewTime);

	TwoDouble GetAxisAbs(GridType gridType);
    TwoDouble GetAxis(GridType gridType);
    double GetStep(GridType gridType);

    CursorInfo GetCursorInfo(int cursorId);

	void SetQntOfPlots(int QntOfPlots);

    void SetBoundsX(double xBegin, double xEnd); //������� �����������
    void SetBoundsY(double yBegin, double yEnd);

    void SetMinStepX(double xMinStep); //����������� ��� �����
    void SetMinStepY(double yMinStep);

    void SetAxisTitleX(CString titleX); //������� ����
    void SetAxisTitleY(CString titleY);

    void SetFitX(FitType fitTypeX); //�������� ��������������� (������ �������, ������� �������, �����)
    void SetFitY(FitType fitTypeY);

    void Shift(zRect rectType); //����� �������
	//�����������
	void AutoScaleX();
	void AutoScaleY();
    void Scale(zRect rectType); //��������������� �������

	void SetCursorPosition(int pointNumber); //���������� ������ �� ������� � ����� � ��������� �������

	// ������� ��������� �������� ���� ����������
	void SetPlotColor(COLORREF PlotColor, int plotNumber = 0);
	void SetGridColor(COLORREF GridColor);
	void SetBackgroundColor(COLORREF BackgroundColor);

    // ����������� �������, ���������������� � ������
	virtual void OnDraw() override;
	virtual void Clear(void) override;
	virtual void SaveParameters(SimpleXML *pSimpleXML) override;
	virtual void LoadParameters(SimpleXML *pSimpleXML) override;
	virtual void OnLButtonDown(UINT nFlags, CPoint point) override;
	virtual void OnLButtonUp(UINT nFlags, CPoint point) override;
	virtual void OnRButtonDown(UINT nFlags, CPoint point) override;
	virtual void OnRButtonUp(UINT nFlags, CPoint point) override;
	virtual void OnMouseMove(UINT nFlags, CPoint point) override;
	virtual void DoAction() override;
	virtual void Resize(CRect rect) override;
};
//------------------------------------------------------------------------------