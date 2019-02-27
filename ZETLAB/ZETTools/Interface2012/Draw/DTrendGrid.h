//------------------------------------------------------------------------------
// DTrendGrid.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include "math.h"
#include <Interface2012\Draw\CustomDraw.h>
#include <Interface2012\Custom\FunctionForAll.h>
#include <Interface2012\Draw\FunctionsForGraph.h>
#include <string>

struct ParamTrendGrid : public ParamCustomDraw {
    CRect m_rect;
    std::vector<double> pVertexs;
    std::vector<double> pBoldVertexs;
    std::vector<CString> pUnderTextMarks, pOverTextMarks;
    std::vector<CRect> pUnderTextRect, pOverTextRect;
    CDFonts fontX;
    CDFonts fontY;
    CString m_textureName; //путь к файлу с текстурой
    CString title;
    CRect titleRect;

    //цвет сетки, текста
    GLfloat m_gridColor[3];
    GLfloat m_textColor[3];

    //параметры материала
    GLfloat mat_ambient[4], mat_ambient1[4]; 	//	{r, g, b, a}
    GLfloat mat_diffuse[4], mat_diffuse1[4];	//	{r, g, b, a}
    GLfloat mat_specular[4], mat_specular1[4];	//	{r, g, b, a}
    GLfloat mat_emission[4], mat_emission1[4];	//	{r, g, b, a}
    GLfloat mat_shininess, mat_shininess1;		//	коэффициент блика

    ParamTrendGrid()
    {
        mat_ambient[0] = 0.1;
        mat_ambient[1] = 0.1;
        mat_ambient[2] = 0.1;
        mat_ambient[3] = 1.0;

        mat_diffuse[0] = 0.5;
        mat_diffuse[1] = 0.5;
        mat_diffuse[2] = 0.5;
        mat_diffuse[3] = 1.0;

        mat_specular[0] = 0.296648;
        mat_specular[1] = 0.296648;
        mat_specular[2] = 0.296648;
        mat_specular[3] = 1.0;

        mat_emission[0] = 0.2;
        mat_emission[1] = 0.2;
        mat_emission[2] = 0.2;
        mat_emission[3] = 1.0;

        mat_shininess = 100.24f;

        mat_ambient1[0] = 0.2;
        mat_ambient1[1] = 0.2;
        mat_ambient1[2] = 0.2;
        mat_ambient1[3] = 0.0;

        mat_diffuse1[0] = 1.0;
        mat_diffuse1[1] = 1.0;
        mat_diffuse1[2] = 1.0;
        mat_diffuse1[3] = 1.0;

        mat_specular1[0] = 0.1;
        mat_specular1[1] = 0.1;
        mat_specular1[2] = 0.1;
        mat_specular1[3] = 0.1;

        mat_emission1[0] = 0.0;
        mat_emission1[1] = 0.0;
        mat_emission1[2] = 0.0;
        mat_emission1[3] = 0.0;

        mat_shininess1 = 1.0f;

        m_gridColor[0] = 255;
        m_gridColor[1] = 255;
        m_gridColor[2] = 255;

#ifdef _ACTIVEX2012
		m_textColor[0] = 0;
		m_textColor[1] = 128;
		m_textColor[2] = 0;
#else
        m_textColor[0] = 255;
        m_textColor[1] = 255;
        m_textColor[2] = 255;
#endif

        fontX.SetColor(RGB(m_textColor[0],m_textColor[1],m_textColor[2]));
        fontY.SetColor(RGB(m_textColor[0],m_textColor[1],m_textColor[2]));
        fontX.SetAligmentHorizontal(StrAlignmentCenter);
        fontY.SetAligmentHorizontal(StrAlignmentFar);
    }
    ~ParamTrendGrid()
    {
        fontX.Clear();
        fontY.Clear();
    }
	void Clear()
	{
		if (!pVertexs.empty())
			pVertexs.clear();
		if (!pBoldVertexs.empty())
			pBoldVertexs.clear();
		if (!pUnderTextMarks.empty())
			pUnderTextMarks.clear();
		if (!pUnderTextRect.empty())
			pUnderTextRect.clear();
	}
};

class CDTrendGrid : public CCustomDraw{
public:
    CDTrendGrid(CRect rect, bool *pFlagIsChange, double *pTimeAnimation);
    virtual ~CDTrendGrid();
    ParamTrendGrid m_param;
    GLuint Texture;
    int PicSize[2];

protected:

private:
	//прорисовка вспомогательных сетки
	void DrawGrid();
	//прорисовка основных линий сетки
	void DrawBoldGrid();
	//заднего фона
	void DrawBackGround();
	//текста
	void DrawTextMarks();
	//отрисовка осей
	void DrawSignature();

public:
    //передать массив координат для отрисовки сетки
    void SetGridVertex(const std::vector<double> &vertex);
    void SetBoldGridVertex(const std::vector<double> &vertex);
    //передать массив строк для отрисовки разметки осей (нижняя строка)
    void SetUnderTextArray(const std::vector<CString> &textArray);
    //передать массив областей для отрисовки разметки осей (нижняя строка)
    void SetUnderTextRect(const std::vector<CRect> &rectArray);
    //передать массив строк для отрисовки разметки осей (верхняя строка)
    void SetOverTextArray(const std::vector<CString> &textArray);
    //передать массив областей для отрисовки разметки осей (верхняя строка)
    void SetOverTextRect(const std::vector<CRect> &rectArray);
    //передать подпись оси
    void SetAxisTitle(CString title);
    //передать область для отрисовки подписи
    void SetTitleRect(CRect rect);
    //установить текстуру
    void SetTexture(CString path);
	//
	void Resize(CRect rect);

    //отрисовка всего элемента
    virtual void OnDraw();
    virtual void Clear (void);
    virtual void SaveParameters(SimpleXML *pSimpleXML);
    virtual void LoadParameters(SimpleXML *pSimpleXML);

	void SetTextColor(COLORREF color);
	void SetGridColor(COLORREF color);
};