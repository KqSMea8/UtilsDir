//------------------------------------------------------------------------------
// DOscillGrid.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include "math.h"
#include <Interface2012\Draw\CustomDraw.h>
#include <Interface2012\Custom\FunctionForAll.h>
#include <Interface2012\Draw\FunctionsForGraph.h>
#include <string>

struct ParamOscillGrid : public ParamCustomDraw {
    CRect m_rect;
    std::vector<double> pVertexs;
	std::vector<double> pBoldVertexs;
    std::vector<CString> pTextMarks;
    std::vector<CRect> pTextRect;
    CDFonts font;
    CString m_textureName; //путь к файлу с текстурой
    CString title;
    CRect titleRect;
	int Number_Texture;
    //цвет сетки, текста
    GLfloat m_gridColor[3];
    GLfloat m_textColor[3];

    //параметры материала
    GLfloat mat_ambient[4], mat_ambient1[4]; 	//	{r, g, b, a}
    GLfloat mat_diffuse[4], mat_diffuse1[4];	//	{r, g, b, a}
    GLfloat mat_specular[4], mat_specular1[4];	//	{r, g, b, a}
    GLfloat mat_emission[4], mat_emission1[4];	//	{r, g, b, a}
    GLfloat mat_shininess, mat_shininess1;		//	коэффициент блика

    ParamOscillGrid()
	{
		Number_Texture = 0;

        mat_ambient[0] = 0.2;
        mat_ambient[1] = 0.2;
        mat_ambient[2] = 0.2;
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

        m_gridColor[0] = 127;
        m_gridColor[1] = 127;
        m_gridColor[2] = 127;
#ifdef _ACTIVEX2012
        m_textColor[0] = 0;
        m_textColor[1] = 128;
        m_textColor[2] = 0;
#else
		m_textColor[0] = 255;
		m_textColor[1] = 255;
		m_textColor[2] = 255;
#endif

        font.SetColor(RGB(m_textColor[0],m_textColor[1],m_textColor[2]));
        font.SetAligmentHorizontal(StrAlignmentCenter);
    }
	~ParamOscillGrid()
	{
		font.Clear();
	}
};

class CDOscillGrid : public CCustomDraw{
public:
    CDOscillGrid(CRect rect, bool *pFlagIsChange, double *pTimeAnimation);
    virtual ~CDOscillGrid();
    ParamOscillGrid m_param;
    GLuint Texture;
    int PicSize[2];

protected:

private:
    //создать бортики вокруг графика
    void CreateBorders();
    //прорисовка сетки
    void DrawGrid();
    void DrawBoldGrid();
    //текста
    void DrawTextMarks();
    //отрисовка осей
    void DrawSignature();
public:
    //передать массив координат для отрисовки сетки
    void SetGridVertex(const std::vector<double> &vertex);
	void SetBoldGridVertex(const std::vector<double> &vertex);
    //передать массив строк для отрисовки разметки осей
    void SetTextArray(const std::vector<CString> &textArray);
    //передать массив областей для отрисовки разметки осей
    void SetTextRect(const std::vector<CRect> &rectArray);
    //установить подпись оси
    void SetAxisTitle(CString title);
    //установить область отрисовки подписи оси
    void SetTitleRect(CRect rect);
    //установить текстуру
    void SetTexture(CString path);

    void SetAligmentHorizontal(StringAlignmentOGL val);

	void Resize(CRect rect);
    //отрисовка всего элемента
    virtual void OnDraw();
	virtual void Clear (void);
    virtual void SaveParameters(SimpleXML *pSimpleXML);
    virtual void LoadParameters(SimpleXML *pSimpleXML);

	void SetGridColor(COLORREF color);
};