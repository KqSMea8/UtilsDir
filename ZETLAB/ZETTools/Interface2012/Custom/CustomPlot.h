//------------------------------------------------------------------------------
// CustomPlot.h : файл заголовка
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
    double paMaxCurrent, paMinCurrent; //границы видимой области по оси
    double paBegin, paEnd; //границы отображения
    double paMinCurrentValue, paMaxCurrentValue; //минимальное и максимальное значение данных в массиве
    std::vector<double> paDataArray; //массив данных
    std::vector<double> paDrawArray; //массив на отрисовку
    ScaleType paScaleType; //тип шкалы
    GridType paGridType; //тип оси
    FitType paFitType; //привязка масштабирования (края, центр)

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
    CRect m_rect; //область, занимаемая графиком
    PlotAxis m_axisX, m_axisY; //структуры параметов данных для каждой оси
    double m_startValue, m_endValue; //начало и конец отрезка
    double m_firstPoint, m_lastPoint; //с какой и по какую точку рисовать
    double m_pxStep, m_step; //шаг графика (условные единицы и пиксели)
    std::vector<double> pVertexs; //массив вершин
    int m_qntPoints; //количество точек (размер массива)
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
    //перевод из абстрактных координат в экранные
    virtual double FromWorldToScreen(double val, double MinCurrent, double MaxCurrent, GridType gridType, ScaleType scaleType, const CRect &rect);
    //записать данные (массив точек по оси Х и Y)
    virtual void SetData(std::vector<double> &pDataX, std::vector<double> &pDataY,
        int quality);
    //записать данные (массив Y)
	virtual void SetData(std::vector<double> &pDataY, int quality, bool Invertation = false);
    //записать данные (массив Y) тип float
	virtual void SetDataFloat(std::vector<float> &pDataY, int quality, bool Invertation = false);
    //автоматическая генерация точек по Х, если Invertation = false, и по Y в ином случае с указанным шагом
	virtual int SetSpace(double pointOfStart, double pointOfEnd, double step, bool Invertation = false);
    //автоматическая генерация точек по оси Х, если Invertation = false, и по Y в ином случае с указанием количества точек
	virtual double SetSpace(double pointOfStart, double pointOfEnd, long Qnt, bool Invertation = false);
	//получить начало отрезка по оси X
    virtual double GetStartValue();
	//получить конец отрезка по сои Y
    virtual double GetEndValue();
    //получить первую точку, которая 
    virtual int GetFirstPoint();
    //установить видимую область по осям Х и Y
    virtual void SetAxis(double minX, double maxX, double minY, double maxY);
	//получить минимальное и максимальное значение среди данных по оси X
	virtual TwoDouble CCustomPlot::GetMinMaxAxisX();
    //получить минимальное и максимальное значение среди данных по оси Y
    virtual TwoDouble GetMinMaxAxisY();
    //заполнить структуру данных по положению курсора на оси Х в том случае,
	//если ось Х является осью разбиения
    virtual CursorValues GetCursorValueX(int px, PlotType plotType);
	//заполнить структуру данных по положению курсора на оси Х в том случае,
	//если ось Х не является осью разбиения
	virtual CursorValues GetCursorValueXInv(int px, PlotType plotType);
	//заполнить структуру данных по положению курсора на оси Y в том случае,
	//если ось Х является осью разбиения
	virtual CursorValues GetCursorValueY(int px, PlotType plotType);
	//заполнить структуру данных по положению курсора на оси Х в том случае,
	//если ось Х не является осью разбиения
	virtual CursorValues GetCursorValueYInv(int px, PlotType plotType);
    //получить значение точки старта по оси X или Y
    virtual double GetPointOfStartVal(bool invertation = false);
	//
	virtual double GetCursorPositionInPoint(unsigned int pointNumber, bool invertation = false);
	//
	virtual void SetCursorPositin(int pointNumber, bool invertation = false){};
	//установить цвет графика
	virtual void SetPlotColor(COLORREF color){};
	//установить толщину графика
	virtual void SetPlotWidth(float width){};
	//утсановить прозрачность графика
	virtual void SetPlotTransparency(float transparency){};
	//лежит ли график в прямоугольной области
	virtual bool PlotInRect(CRect rect){return false;};
	//
	virtual void PlotBacklight(){};
};