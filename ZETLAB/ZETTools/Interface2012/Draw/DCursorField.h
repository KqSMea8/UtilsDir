//------------------------------------------------------------------------------
//	DCursorField.h - файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include <Interface2012\Draw\CustomDraw.h>
#include <Interface2012\Draw\FunctionsForGraph.h>
#define ZDEPTH 5.0
#define QNTOFCOLORS 5

struct ParamCursorField : public ParamCustomDraw {
    CRect m_rect;
    CString m_textureName; //путь к файлу с текстурой
    CPoint m_start, m_end;

    GLfloat mat_ambient[4]; 	//	{r, g, b, a}
    GLfloat mat_diffuse[4];	//	{r, g, b, a}
    GLfloat mat_specular[4];	//	{r, g, b, a}
    GLfloat mat_emission[4];	//	{r, g, b, a}
    GLfloat mat_shininess;		//	коэффициент блика
    COLORREF palette[QNTOFCOLORS];

    ParamCursorField(){
        Size = sizeof(ParamCursorField);
        mat_ambient[0] = 0.1;
        mat_ambient[1] = 0.1;
        mat_ambient[2] = 0.1;
        mat_ambient[3] = 1.0;

        mat_diffuse[0] = 1.0;
        mat_diffuse[1] = 1.0;
        mat_diffuse[2] = 1.0;
        mat_diffuse[3] = 1.0;

        mat_specular[0] = 1.0;
        mat_specular[1] = 1.0;
        mat_specular[2] = 1.0;
        mat_specular[3] = 1.0;

        mat_emission[0] = 0.2;
        mat_emission[1] = 0.2;
        mat_emission[2] = 0.2;
        mat_emission[3] = 0.0;

        palette[0] = RGB(0, 128, 0);
        palette[1] = RGB(0, 255, 0);
        palette[2] = RGB(0, 0, 255);
        palette[3] = RGB(255, 255, 0);
        palette[4] = RGB(255, 0, 255);

        mat_shininess = 100.24f;

    }
};

class CDCursorField : public CCustomDraw
{
public:
    CDCursorField(CRect rect, int qnt, bool *pFlagIsChange, double *pTimeAnimation);
    ~CDCursorField();

    void SetActiveCursor(int id);
    void SetCursorPosition(CPoint point);
    void SetLineLength(double val);
	void Resize(CRect rect);
    virtual void OnDraw();
    virtual void Clear();
protected:

private:
    ParamCursorField m_param;
    std::vector<CDetail*> m_pCursor;
    int m_qntOfCursors;
    int m_currentCursor;
    CPoint currentCursorPosition;
    double m_lineLength;
    void DrawField();
    void DrawCursor();
    void DrawLine();
};