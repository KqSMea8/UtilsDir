//------------------------------------------------------------------------------
// CursorField.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include <Interface2012\Custom\CustomElement.h>
#include <Interface2012\Draw\DCursorField.h>
#include <vector>

//структура данных для позиционирования курсора по точкам графика
struct CursorData
{
    double cdStep;
    TimeInterval cdTimeStep;
    double cdCurrentMin;
    double cdCurrentMax;
    CTime cdCurrentTimeMin;
    CTime cdCurrentTimeMax;
    int cdMSecMin, cdMSecMax;
    double cdPointOfStartVal;

    CursorData()
    {
        cdStep = 1;
        cdCurrentMin = 0;
        cdCurrentMax = 10;
        cdPointOfStartVal = 0;
        cdMSecMin = 0;
        cdMSecMax = 0;
		memset (&cdTimeStep, 0 , sizeof(cdTimeStep));
    }
};

class CCursorField : public CCustomElement
{
public:
    CCursorField(CCustomContainer *owner, CRect rect, int lineLength, int qntOfCursors, bool invertation);
    virtual ~CCursorField();

    //получить координату положения курсора по оси Х
    int GetCursorPosition(int cursorId);
    //установить шаг перемещения курсора
    void SetStep(double step);
    //установить шаг перемещения курсора в формате дат
    void SetStep(TimeInterval step);
    //установить минимальное и максимальное значение видимой области
    void SetMinMax(double min, double max);
    //задать точку, с которой начинается видимая часть графика
    void SetPointOfStartVal(double val);
    //Получить номер активного курсора
    int GetActiveCursor();
    //???
	void CalcCursorPosition(CPoint point);

	virtual void Resize(CRect rect) override;

	void ResizeLine(int lineLength);

    virtual void OnDraw();
    virtual void OnLButtonDown(UINT nFlags, CPoint point);
    virtual void OnLButtonUp(UINT nFlags, CPoint point);
    virtual void OnMouseMove(UINT nFlags, CPoint point);
    virtual void OnMouseLeave();
	virtual void Clear();
protected:
    CDCursorField *m_pDCursorField;
private:
    std::vector<CRect> cursorRect;
    CursorData m_Data;
    int m_qntOfCursors;
    int m_activeCursor;
    bool m_bFlag;
    CRect m_rect;
    bool DataType; //тип используемых данных (формат дат или числа) true - числа, false - даты
	bool m_invertation;
};