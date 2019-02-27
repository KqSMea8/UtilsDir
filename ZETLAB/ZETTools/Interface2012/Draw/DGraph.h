//------------------------------------------------------------------------------
// DGraph.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include <Interface2012\Draw\CustomDraw.h>
#define ZDEPTH 4.5
#include <vector>

struct ImageSize
{
    int PicSize[2];
};

struct ParamGraph : public ParamCustomDraw {
    CRect m_rect, m_leftShiftRect, m_rightShiftRect, m_topShiftRect,
        m_bottomShiftRect, m_leftScaleRect, m_rightScaleRect,
        m_topScaleRect, m_bottomScaleRect, m_centerAutoScaleRect,
        m_gridRect, m_decorativeRect;

    std::vector<CRect>		m_bubblesField;					//область "бабла" для текста
    std::vector<CRect>		m_bubblesColor;					//окантовка "бабла" (цвет совпадает с цветом активного графика)
    std::vector<CRect>		m_bubblesBorder;				//черная граница "бабла"
    std::vector<CRect>		m_textFieldX, m_textFieldY;		//области под подписи
    std::vector<CString>	m_textLegendX, m_textLegendY;	//строка с текстом
    std::vector<COLORREF>	m_textColor;					//цвет текста
	COLORREF				m_backgroundColor;				//цвет экрана графика

    CDFonts font;

    int ActiveArrow;

    CString m_textureName;		//путь к файлу с текстурой
    CString m_normalMapName;	//путь к файлу с текстурой
    CString m_arrowTextureName;	//путь к файлу с текстурой для стрелок

    GLfloat mat_ambient[4]; 	//	{r, g, b, a}
    GLfloat mat_diffuse[4];		//	{r, g, b, a}
    GLfloat mat_specular[4];	//	{r, g, b, a}
    GLfloat mat_emission[4];	//	{r, g, b, a}
    GLfloat mat_shininess;		//	коэффициент блика

	GLfloat mat_ambient_a[4]; 	//	{r, g, b, a}
	GLfloat mat_diffuse_a[4];	//	{r, g, b, a}
	GLfloat mat_specular_a[4];	//	{r, g, b, a}
	GLfloat mat_emission_a[4];	//	{r, g, b, a}
	GLfloat mat_shininess_a;		//	коэффициент блика

    GLfloat mat_ambient_b[4]; 	//	{r, g, b, a}
    GLfloat mat_diffuse_b[4];	//	{r, g, b, a}
    GLfloat mat_specular_b[4];	//	{r, g, b, a}
    GLfloat mat_emission_b[4];	//	{r, g, b, a}
    GLfloat mat_shininess_b;	//	коэффициент блика

    GLuint VBO;

    ParamGraph(){
        Size = sizeof(ParamGraph);

        ActiveArrow = -1;

        mat_ambient[0] = 0.3;
		mat_ambient[1] = 0.3;
		mat_ambient[2] = 0.3;
        mat_ambient[3] = 1.0;

        mat_diffuse[0] = 1.0;
        mat_diffuse[1] = 1.0;
        mat_diffuse[2] = 1.0;
        mat_diffuse[3] = 1.0;

        mat_specular[0] = 0.3;
        mat_specular[1] = 0.3;
        mat_specular[2] = 0.3;
        mat_specular[3] = 1.0;

        mat_emission[0] = 0.1;
        mat_emission[1] = 0.1;
        mat_emission[2] = 0.1;
        mat_emission[3] = 1.0;

        mat_shininess = 100.f;

		mat_ambient_a[0] = 0.5;
		mat_ambient_a[1] = 0.5;
		mat_ambient_a[2] = 0.5;
		mat_ambient_a[3] = 1.0;

		mat_diffuse_a[0] = 1.0;
		mat_diffuse_a[1] = 1.0;
		mat_diffuse_a[2] = 1.0;
		mat_diffuse_a[3] = 1.0;

		mat_specular_a[0] = 0.3;
		mat_specular_a[1] = 0.3;
		mat_specular_a[2] = 0.3;
		mat_specular_a[3] = 1.0;

		mat_emission_a[0] = 0.1;
		mat_emission_a[1] = 0.1;
		mat_emission_a[2] = 0.1;
		mat_emission_a[3] = 1.0;

		mat_shininess_a = 100.f;

        mat_ambient_b[0] = 0.25;
        mat_ambient_b[1] = 0.25;
        mat_ambient_b[2] = 0.25;
        mat_ambient_b[3] = 0.0;

        mat_diffuse_b[0] = 1.0;
        mat_diffuse_b[1] = 1.0;
        mat_diffuse_b[2] = 1.0;
        mat_diffuse_b[3] = 1.0;

        mat_specular_b[0] = 0.4;
        mat_specular_b[1] = 0.4;
        mat_specular_b[2] = 0.4;
        mat_specular_b[3] = 1.0;

        mat_emission_b[0] = 58.f/255.f;
        mat_emission_b[1] = 117.f/255.f;
        mat_emission_b[2] = 196.f/255.f;
        mat_emission_b[3] = 0.0;

        mat_shininess_b = 100.24f;

		m_backgroundColor = RGB(0, 0, 0);

    }
};

class CDGraph : public CCustomDraw{
public:
    CDGraph(CRect rect, CRect gridRect, bool *pFlagIsChange, double *pTimeAnimation);
    virtual ~CDGraph();

protected:

private:
    ParamGraph m_param;
    CDetail *m_pButton[13];
    CDetail *m_pBorders[8];
	CDetail *m_pDecorative;
    GLuint Texture, ArrowTexture, NormalMap;
    int PicSize[2], PicSize1[2];
    ImageSize imageSize[9];
    void CreateButtons();
    void CreateBorders();
    void DrawButtons();
    void DrawBorders();
    void DrawArrows();
    void DrawLegend();
	void DrawBackGround();
public:
    void SetRectAll(CRect leftShiftRect, CRect rightShiftRect,
        CRect bottomShiftRect, CRect topShiftRect, CRect leftScaleRect, CRect rightScaleRect,
		CRect topScaleRect, CRect bottomScaleRect, CRect centerAutoScaleRect, CRect decorativeRect);
	void Resize(CRect rect, CRect gridRect, CRect leftShiftRect, CRect rightShiftRect,
		CRect bottomShiftRect, CRect topShiftRect, CRect leftScaleRect, CRect rightScaleRect,
		CRect topScaleRect, CRect bottomScaleRect, CRect centerAutoScaleRect, CRect decorativeRect);
    void SetRectBubble(CRect bubbleRect, int cursorId);
    CRect GetRectBubble(int cursorId);
    void SetTextBubble(CString textX, CString textY, int cursorId);
    void SetTexture(CString path);
    void SetArrowsTextures(CString path);
    void SetNormalMap(CString path);
    void BrighteningForArrows(bool lButtonState, int ActiveArrow);
    virtual void OnDraw();
	virtual void Clear();
    virtual void SaveParameters(SimpleXML *pSimpleXML);
    virtual void LoadParameters(SimpleXML *pSimpleXML);
    //virtual void OnLButtonDown(UINT nFlags, CPoint point);
    //virtual void OnMouseMove(UINT nFlags, CPoint point);

	void SetBackgroundColor(COLORREF backgroundColor);
};