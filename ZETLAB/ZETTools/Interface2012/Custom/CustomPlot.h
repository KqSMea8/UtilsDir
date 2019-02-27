//------------------------------------------------------------------------------
// CustomPlot.h : ���� ���������
//------------------------------------------------------------------------------
#pragma once
#include <Interface2012\Custom\CustomElement.h>
#include <Interface2012\Custom\Custom_ZETi12.h>
#include <Interface2012\Draw\FunctionsForGraph.h>
#include <vector>
#include <limits.h>
#define Epsilon 0.00000001

struct PlotAxis
{
    double paMaxCurrent, paMinCurrent; //������� ������� ������� �� ���
    double paBegin, paEnd; //������� �����������
    double paMinCurrentValue, paMaxCurrentValue; //����������� � ������������ �������� ������ � �������
    std::vector<double> paDataArray; //������ ������
    std::vector<double> paDrawArray; //������ �� ���������
    ScaleType paScaleType; //��� �����
    GridType paGridType; //��� ���
    FitType paFitType; //�������� ��������������� (����, �����)

	void Clear()
	{
		if (!paDataArray.empty())
			paDataArray.clear();
		if (!paDrawArray.empty())
			paDrawArray.clear();
	};
};

struct PlotParam
{
    CRect m_rect; //�������, ���������� ��������
    PlotAxis m_axisX, m_axisY; //��������� ��������� ������ ��� ������ ���
    double m_startValue, m_endValue; //������ � ����� �������
    double m_firstPoint, m_lastPoint; //� ����� � �� ����� ����� ��������
    double m_pxStep, m_step; //��� ������� (�������� ������� � �������)
    std::vector<double> pVertexs; //������ ������
    int m_qntPoints; //���������� ����� (������ �������)
	bool m_invertation;
	void Clear()
	{
		m_axisX.Clear();
		m_axisY.Clear();
		if (!pVertexs.empty())
			pVertexs.clear();
	};
};

class CCustomPlot : public CCustomElement
{
public:
    CCustomPlot(CCustomContainer *owner);
    virtual ~CCustomPlot();

private:

protected:

public:
    PlotParam *m_pPlotParam;

	virtual double GetDataStep() { return m_pPlotParam->m_step; };
    //������� �� ����������� ��������� � ��������
    virtual double FromWorldToScreen(double val, double MinCurrent, double MaxCurrent, GridType gridType, ScaleType scaleType, const CRect &rect);
    //�������� ������ (������ ����� �� ��� � � Y)
    virtual void SetData(std::vector<double> &pDataX, std::vector<double> &pDataY,
        int quality);
    //�������� ������ (������ Y)
	virtual void SetData(std::vector<double> &pDataY, int quality, bool Invertation = false);
    //�������� ������ (������ Y) ��� float
	virtual void SetDataFloat(std::vector<float> &pDataY, int quality, bool Invertation = false);
    //�������������� ��������� ����� �� �, ���� Invertation = false, � �� Y � ���� ������ � ��������� �����
	virtual int SetSpace(double pointOfStart, double pointOfEnd, double step, bool Invertation = false);
    //�������������� ��������� ����� �� ��� �, ���� Invertation = false, � �� Y � ���� ������ � ��������� ���������� �����
	virtual double SetSpace(double pointOfStart, double pointOfEnd, long Qnt, bool Invertation = false);
	//�������� ������ ������� �� ��� X
    virtual double GetStartValue();
	//�������� ����� ������� �� ��� Y
    virtual double GetEndValue();
    //�������� ������ �����, ������� 
    virtual int GetFirstPoint();
    //���������� ������� ������� �� ���� � � Y
    virtual void SetAxis(double minX, double maxX, double minY, double maxY);
	//�������� ����������� � ������������ �������� ����� ������ �� ��� X
	virtual TwoDouble CCustomPlot::GetMinMaxAxisX();
    //�������� ����������� � ������������ �������� ����� ������ �� ��� Y
    virtual TwoDouble GetMinMaxAxisY();
    //��������� ��������� ������ �� ��������� ������� �� ��� � � ��� ������,
	//���� ��� � �������� ���� ���������
    virtual CursorValues GetCursorValueX(int px, PlotType plotType);
	//��������� ��������� ������ �� ��������� ������� �� ��� � � ��� ������,
	//���� ��� � �� �������� ���� ���������
	virtual CursorValues GetCursorValueXInv(int px, PlotType plotType);
	//��������� ��������� ������ �� ��������� ������� �� ��� Y � ��� ������,
	//���� ��� � �������� ���� ���������
	virtual CursorValues GetCursorValueY(int px, PlotType plotType);
	//��������� ��������� ������ �� ��������� ������� �� ��� � � ��� ������,
	//���� ��� � �� �������� ���� ���������
	virtual CursorValues GetCursorValueYInv(int px, PlotType plotType);
    //�������� �������� ����� ������ �� ��� X ��� Y
    virtual double GetPointOfStartVal(bool invertation = false);
	//
	virtual double GetCursorPositionInPoint(unsigned int pointNumber, bool invertation = false);
	//
	virtual void SetCursorPositin(int pointNumber, bool invertation = false){};
	//���������� ���� �������
	virtual void SetPlotColor(COLORREF color){};
	//���������� ������� �������
	virtual void SetPlotWidth(float width){};
	//���������� ������������ �������
	virtual void SetPlotTransparency(float transparency){};
	//����� �� ������ � ������������� �������
	virtual bool PlotInRect(CRect rect){return false;};
	//
	virtual void PlotBacklight(){};
};