//------------------------------------------------------------------------------
// DButton.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Draw\CustomDraw.h"
#ifdef _GDIPLUS
//#include "C:\ZETTools\Interface2012\Custom\Font.h"
#else
//#include "C:\TestDialog\TestDialog\STLRead.h"
//#include "C:\ZETTools\Interface2012\Draw\SampleObject.h"
#endif

enum eButtonAnimation {AnimationOn, AnimationOff};
enum eButtonColoring {ColoringOn, ColoringOff};

//------------------------------------------------------------------------------
struct ParamButton : public ParamCustomDraw
{// В родительской структуре - общие параметры, здесь - частные параметры
	int BorderWidth;			// ширина границы
	COLORREF BorderColor;		// цвет границы без фокуса
	COLORREF BorderColorFocus;	// цвет границы с фокусом
	COLORREF BackColor;			// цвет фона без мыши
	COLORREF BackColorMouse;	// цвет фона при наведении мыши
	COLORREF BackColorEnter;	// цвет фона при нажатой кнопке
	COLORREF CheckColorMouse;
	COLORREF CheckColor;
	CString Caption;			// текст, который выводится на кнопке
	CString CaptionChange;			// текст, который выводится на кнопке при нажатии для фиксированной кнопки
	CString CaptionCut; //короткий текст, который будет отображаться на кнопке, в случае если он не помещается с определенным рамером
	CString fileName;		// базовое имя файлов с картинками
	bool isClicked;			//флаг нажатия кнопки
	int Number_Texture;
#ifdef _GDIPLUS
	CZFont *pFont;		// экземпляр класса шрифта
	StringAlignment AlignmentHorizontal;	// выравнивание по горизонтали
	StringAlignment AlignmentVertical;		// выравнивание по вертикали

	LinearGradientMode GradientMode;	// тип градиента
	bool CheckBox;			// тип кнопки
	bool ButtonType;        // тип кнопки
#else
	//Zettest i12
	CDFonts *pFont;
	GLfloat mat_ambient[4]; 	//	{r, g, b, a}
	GLfloat mat_diffuse[4];		//	{r, g, b, a}
	GLfloat mat_specular[4];	//	{r, g, b, a}
	GLfloat mat_emission[4];	//	{r, g, b, a}
	GLfloat mat_shininess;		//	коэффициент блика

	GLfloat mat_ambient_b[4]; 	//	набор цветов ambient для объекта m_Button. 
	GLfloat mat_diffuse_b[4];		//  Изменяя этот параметр только у кнопки, цвет бортика остается неизменным

	GLfloat mat_unit[4]; // rgba при ативном элементе галочки

	GLfloat mat_diffuse_rb[4];    //набор цветов для Radio Button 
	GLfloat mat_specular_rb[4];
	GLfloat mat_shininess_rb;
	
#endif

	CRect rect; //прямоугольник в который будет вписываться элемент
	double dTimeAnimation;	// длительность задержки кнопки
	double dTimeCur;		// текущая продолжительность анимации
	double dTimeStart;		// время начала анимации
	float SizeText;			// размер текстового окна (для CheckBox)

	eButtonType ButtonType; //тип кнопки Fixed - фиксируется, NotFoxed - не фиксируется
	eButtonColoring ButtonColoring; //тип кнопки ColoringOn - изменяет цвет, ColorongOff - не меняет
	eButtonAnimation ButtonAnimation; //тип кнопки AnimationOn - с анимацией, AnimationOff - без анимации
	float fValueCur; // текущее положение кнопки в относительных координатах
	GLfloat ScaleStep; // шаг изменения глубины положения кнопки
	GLfloat maxDepth; // максимальная глубина, на которую опускается кнопка
	GLfloat generalDepth; // обычное "выключенное" положение кнопки
	float posZ; // параметр, отвечающий за текущее положение кнопки при анимации в координатах OpenGL
	CRect rectForCaption; // прямоугольник, в который впишется текст кнопки
	bool BelongsMenu; //принадлежность выпадающим спискам
	CString Texture_FileName;
	bool ActiveBut;
	bool Active; //является ли кнопка нажимаемой или нет
	ParamButton()
		: ActiveBut(false)
		, BorderWidth(0)		// в конструкторе выставляется 1
		, BorderColor(RGB(82, 82, 82))
		, BorderColorFocus(RGB(150, 210, 255))	// 164, 210, 244;  150 210 255
		, BackColor(RGB(255, 255, 255))
		, BackColorMouse(RGB(186, 255, 255))
		, BackColorEnter(RGB(195, 195, 195))
		, CheckColorMouse(RGB(26, 71, 128))
		, CheckColor(RGB(80, 80, 80))
		, Number_Texture(0)
		, Texture_FileName(L"C:\\ZETTools\\Interface2012\\Textures\\Patterns\\metal_mart14.bmp")
		, SizeText(14.0)
		, Caption(L"")
		, CaptionChange(L"")
		, CaptionCut(L"")
		, dTimeAnimation(0.7)
		, dTimeCur(0.)
		, dTimeStart(0.)
		, fileName(L"")
		, isClicked(false)
		, fValueCur(0.0)
		, posZ(0.f)
		, ButtonType(NotFixed)
		, ButtonColoring(ColoringOff)
		, ButtonAnimation(AnimationOff)
		, ScaleStep(0.0)
		, maxDepth(-0.5)
		, pFont(NULL)
		, BelongsMenu(false)
		, Active(true)
	{
		Size = sizeof(ParamButton);

#ifdef _GDIPLUS
		pFont = NULL;
		AlignmentHorizontal = StringAlignmentCenter;
		AlignmentVertical = StringAlignmentCenter;
		GradientMode = LinearGradientModeForwardDiagonal;
#else		
		// начальные цвета кнопки
		mat_ambient[0] = 0.0;
		mat_ambient[1] = 0.0;
		mat_ambient[2] = 0.0;
		mat_ambient[3] = 1.0;

		mat_diffuse[0] = 1.0;
		mat_diffuse[1] = 1.0;
		mat_diffuse[2] = 1.0;
		mat_diffuse[3] = 1.0;

		mat_specular[0] = 1.0;
		mat_specular[1] = 1.0;
		mat_specular[2] = 1.0;
		mat_specular[3] = 1.0;

		mat_emission[0] = 0.3;
		mat_emission[1] = 0.3;
		mat_emission[2] = 0.3;
		mat_emission[3] = 1.0;

		mat_shininess = 5.24f;

		mat_ambient_b[0] = 0.0;
		mat_ambient_b[1] = 0.0;
		mat_ambient_b[2] = 0.0;
		mat_ambient_b[3] = 0.0;

		mat_diffuse_b[0] = 1.0;
		mat_diffuse_b[1] = 1.0;
		mat_diffuse_b[2] = 1.0;
		mat_diffuse_b[3] = 1.0;

		mat_unit[0] = 0.0;
		mat_unit[1] = 0.0;
		mat_unit[2] = 0.0;
		mat_unit[3] = 1.0;

		mat_diffuse_rb[0] = 1.0;
		mat_diffuse_rb[1] = 1.0;
		mat_diffuse_rb[2] = 1.0;
		mat_diffuse_rb[3] = 1.0;

		mat_specular_rb[0] = 0.5;
		mat_specular_rb[1] = 0.5;
		mat_specular_rb[2] = 0.5;
		mat_specular_rb[3] = 1.0;
		
		mat_shininess_rb = 4.24f;

		
#endif
		
	}
};
//------------------------------------------------------------------------------
class CDButton : public CCustomDraw
{
public:
#ifdef _GDIPLUS
	CDButton(CRect rect, PVOID ppGraphics,
		bool *pFlagIsChange, double *pTimeAnimation);
#else
	CDButton(CRect rect, bool *pFlagIsChange, double *pTimeAnimation);
	CDButton(CRect rect, bool *pFlagIsChange, double *pTimeAnimation, CString ImageFileint, int NumberTexture);
#endif
	virtual ~CDButton();
private:	
	
	ParamButton m_param;	// структура всех параметров отображения
	COLORREF m_BackColorCur;
	COLORREF m_BorderColorCur;
	COLORREF m_CheckColorCur;
	CRect m_rect1, m_rect4;			// прямоугольник CheckBox
	CRect m_rect2, m_rect3;	// прямоугольник текстового поля
	bool m_bEvent;			// флаг события
	bool m_bFixation;		// флаг типа кнопки //в новом интерфейсе флаг того нажата кнопка или нет

	CString m_Text;
	GLuint Texture;
	GLuint Texture1;
	int PicSize[2];
#ifdef _GDIPLUS
	Image *m_pImage_Cur;	// ук. на текущую картинку
	Image *m_pImage_s;		// стандартное отображение
	Image *m_pImage_m;		// отображение с мышью
	Image *m_pImage_p;		// отображение в нажатом состоянии
	Image *m_pImage_pm;		// отображение с мышью для checkbox в нажатом состоянии
	CZFont *m_pFont;
#else
	CDetail *m_Button, *m_Border, *m_CheckBox; //детали для обычных прямоугольных кнопок
#endif
	
protected:

public:
	// функции параметров отображения
	int GetBorderWidth() { return m_param.BorderWidth; }
	void SetBorderWidth(int val);

	COLORREF GetBorderColor() { return m_param.BorderColor; }
	void SetBorderColor(COLORREF val);

	COLORREF GetBorderColorFocus() { return m_param.BorderColorFocus; }
	void SetBorderColorFocus(COLORREF val);

	CString GetCaption() { return m_param.Caption; }
	void SetCaption(CString str);

	int GetSizeText() { return m_param.SizeText; }
	void SetSizeText(int val);

	CString GetCaptionChange() { return m_param.CaptionChange; }
	void SetCaptionChange(CString str);

	double GetTimeAnimation() { return m_param.dTimeAnimation;}
	void SetTimeAnimation(double val);

	bool GetAnimation()	 { return m_param.bAnimation; }
	void SetAnimation(bool val) { m_param.bAnimation = val; }

	eButtonType GetButtonType() { return m_param.ButtonType; }
	void SetButtonType(eButtonType val);

	eButtonColoring GetButtonColoring() { return m_param.ButtonColoring; }
	void SetButtonColoring(eButtonColoring val) { m_param.ButtonColoring = val; }

	eButtonAnimation GetButtonAnimation() { return m_param.ButtonAnimation; }
	void SetButtonAnimation(eButtonAnimation val) { m_param.ButtonAnimation = val; }

	CString GetImageFileName() { return m_param.fileName;}
	void SetImageFileName(CString str);

	void SetImageFileNameHMME(CString str);

	void ReturnButtonToState (bool val); // программное нажатие/отжатие кнопки

	bool GetButtonState (void) {return m_bFixation;}
	void SetButtonState(bool val) {m_bFixation = val;}

	void SetTextColor(int r, int g, int b);

/*	void SetTextSize (float val);
	float GetTextVal (void) { return m_param.SizeText; }//*/

	bool GetIsClicked() {return m_param.isClicked;}
	void SetIsClicked(bool val) {m_param.isClicked = val;}

	bool GetButtonActive() {return m_param.Active;}
	void SetButtonActive(bool val) {m_param.Active = val;}

	bool DGetBelongsMenu() { return m_param.BelongsMenu; }
	void DSetBelongsMenu(bool val) { m_param.BelongsMenu = val; }

	void LiftButton(float val) {m_Button->SetOffsetZ(val); m_Border->SetOffsetZ(val);}
	void SetActivePos (bool val) {m_param.ActiveBut = val; }

#ifdef _GDIPLUS
	CZFont* GetFont() { return m_pFont; }
	void SetFont(CZFont *pFont) { m_pFont = pFont; }

	StringAlignment GetAlignmentHorizontal() {return m_param.AlignmentHorizontal; }
	void SetAlignmentHorizontal(StringAlignment val);

	StringAlignment GetAlignmentVertical() {return m_param.AlignmentVertical; }
	void SetAlignmentVertical(StringAlignment val);
	
	bool GetButtonType() { return m_param.ButtonType; }
	void SetButtonType(bool val) { m_param.ButtonType = val; }

	bool GetCheckBox() { return m_param.CheckBox; }
	void SetCheckBox(bool val) { m_param.CheckBox = val; }

	bool GetFixsation() { return m_bFixation; }
#else	
	void SetColor(GLfloat *ma, GLfloat *md,	
		GLfloat *me, 
		GLfloat *msp, GLfloat msh); // устанавливаем цвет материала объекта

	void OnDrawText();

	void SetAligmentHorizontal(StringAlignmentOGL val) { m_param.pFont->SetAligmentHorizontal(val);} // меняет выравнивание в строке по горизонтали
	
	//void ChangeSTLModel(char *FileName) ;
#endif
	// виртуальные функции, переопределяемые в классе
	virtual void OnDraw();
	virtual void Clear(void);
	virtual void GetFocus();
	virtual void KillFocus();
	virtual void OnMouseEnter();
	virtual void OnMouseLeave();
	virtual void DoProcess();
	virtual void OnLButtonClk(UINT nFlags, CPoint point);
    virtual void SaveParameters(SimpleXML *pSimpleXML);
    virtual void LoadParameters(SimpleXML *pSimpleXML);
	virtual void OnLButtonDblClk(UINT nFlags, CPoint point){
		TRACE("Double click!\n");
	};

	void SetRectForDraw(CRect val);
	static int m_number_buttons;
};
//------------------------------------------------------------------------------