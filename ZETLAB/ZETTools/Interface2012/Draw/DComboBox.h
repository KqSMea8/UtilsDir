//------------------------------------------------------------------------------
// DComboBox.h : ���� ���������
//------------------------------------------------------------------------------
// ����� ����������� ComboBox. ������� �� ������ ��������� ����� 256 �����
// ��� Combobox ����������� �� ����, ��� ���������� ������� SetTextString.
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Draw\CustomDraw.h"
#include "C:\ZETTools\Interface2012\ZetLab\Panel.h"
#include "C:\ZETTools\Interface2012\ZetLab\Button.h"
#include "C:\ZETTools\Interface2012\ZetLab\ScrollBar.h" //way!

typedef void(*FunctionChan)();
//static bool flagScroll; //���� ���������� �� ��������� ���������
#define DETAILS 9
enum ComboBoxType
{
	cbtSimple   = 0,
	cbtDropDown = 1,
	cbtDropList = 2
};
//------------------------------------------------------------------------------
struct ParamComboBox : public ParamCustomDraw
{// � ������������ ��������� - ����� ���������, ����� - ������� ���������
	int BorderWidth;			// ������ �������
	COLORREF BorderColor;		// ���� ������� ��� ������
	COLORREF BorderColorFocus;	// ���� ������� � �������
	COLORREF BackColor;			// ���� ���� ��� ����
	COLORREF BackColorMouse;	// ���� ���� � �����
	COLORREF FieldSolidColor;
	long IndexCur;				// ������� Item ����

	Point pt[3];				// ����������� ����� �������������
	CString Caption;			// ����, ������� ��������� � ����� ����
	UINT_PTR StartItemID;		// ID �������� Item ����, ������� ����� ������
	CString StartItemName;
	long NumberMax;
	ComboBoxType Type;
    CString m_textureName;
	CRect rect2;
	CString Texture_1_FileName;
	CString Texture_2_FileName;
	CString Texture_3_FileName;
	CString Texture_4_FileName;
	int Number_Texture[4];
#ifdef _GDIPLUS
#else
	GLfloat mat_ambient[4]; 	//	{r, g, b, a}
	GLfloat mat_diffuse[4];		//	{r, g, b, a}
	GLfloat mat_specular[4];	//	{r, g, b, a}
	GLfloat mat_emission[4];	//	{r, g, b, a}
	GLfloat mat_shininess;		//	����������� �����
	int ItemHeight;
#endif
	ParamComboBox()
	{
		Size = sizeof(ParamComboBox);
		StartItemID = 0;
		BorderWidth = 0;		// � ������������ ������������ 1
		BorderColor = RGB(66,66,66);
		BorderColorFocus = RGB(150, 210, 255);	// 164, 210, 244;  150 210 255
		BackColor = RGB(250, 250, 250);
		BackColorMouse = RGB(220, 240, 255);
		FieldSolidColor = RGB(203, 203, 203);
	//	BackColorMouse = RGB(230, 243, 255);	// ���������
		IndexCur = -1;
		Caption = L"";
		NumberMax = 256;
		Texture_1_FileName = L"C:\\ZETTools\\Interface2012\\Textures\\Patterns\\metal_mart14.bmp";
		Texture_2_FileName = L"C:\\ZETTools\\Interface2012\\Textures\\Seamless\\Other\\combo_down.bmp";
		Texture_3_FileName = L"C:\\ZETTools\\Interface2012\\Textures\\Seamless\\Other\\downbutton_01.bmp";
		Texture_4_FileName = L"C:\\ZETTools\\Interface2012\\Textures\\Seamless\\Other\\upbutton_01.bmp";
		Number_Texture[0] = 0;
		Number_Texture[1] = 18;
		Number_Texture[2] = 21;
		Number_Texture[3] = 22;
#ifdef _GDIPLUS
#else
		mat_ambient[0] = 0.2;
		mat_ambient[1] = 0.2;
		mat_ambient[2] = 0.2;
		mat_ambient[3] = 0.0;

		mat_diffuse[0] = 0.72;
		mat_diffuse[1] = 0.72;
		mat_diffuse[2] = 0.72;
		mat_diffuse[3] = 0.0;

		mat_specular[0] = 0.296648;
		mat_specular[1] = 0.296648;
		mat_specular[2] = 0.296648;
		mat_specular[3] = 0.0;

		mat_emission[0] = 0.5;
		mat_emission[1] = 0.5;
		mat_emission[2] = 0.5;
		mat_emission[3] = 0.0;

		mat_shininess = 10.24f;
		StartItemID = 0;
		Type = cbtDropDown; // true - stl-model
		ItemHeight = 23;
#endif
	}
};
//------------------------------------------------------------------------------
class CDComboBox : public CCustomDraw
{
public:
#ifdef _GDIPLUS
	CDComboBox(CRect rect, PVOID ppGraphics,
				bool *pFlagIsChange, double *pTimeAnimation);
#else
	CDComboBox(CRect rect,
		bool *pFlagIsChange, double *pTimeAnimation);
#endif
	virtual ~CDComboBox();

private:
	ParamComboBox m_param;	// ��������� ���� ���������� �����������
	COLORREF m_BackColorCur;
	COLORREF m_BorderColorCur;
	CRect m_rect2, m_rect3;			// ������������� ���������� ����
//	CRect m_rectT;			// ������������� ������������
	CMenu m_menu;			// ����
	int m_CounterItem;		// ���-�� ����� � ����
	bool m_bViewMenu;		// ���������� ����
	bool m_bEvent;			// ���� �������
	int size;

	CString arrayOfNames[256];
	CCustomContainer *pParent;
	CZButton *m_Button[30]; //zettest
	CZButton *mButton;
	CZButton *m_ButtonUp;
	CZButton *m_ButtonDown;
	std::vector<CZButton*> m_Vector; //������ �� ��������� ����
	CZScrollBar *m_ScrollBar; //zettest
	bool m_bVisUD;
	
	int lowestItem; //����� ������� ��������, ���� Combobox � �������� UP DOWN
	bool Invert; // ���� true - Combobox �������� ����� �����, false - ������ ����
	CRect spaceRect; // ������� ����������, � ������� ��������� Combobox

	FunctionChan pointFun; //��������� �� �������

	int _pVisNum; //���������� ������� ���������
	int _pMaxVisNum; // ����� ���������� ����������� ������� ��������� �� ������

#ifdef _GDIPLUS
	CZFont *m_pFont;
#else
	
	CDFonts *m_pFont;
	GLuint Texture1;
	GLuint Texture2;
	CDetail *m_ComboBox;
	CDetail *m_pComboBox[DETAILS];
	CDetail *m_pButton;
	CDetail *m_Border;
	CDetail *m_pPanel;
	int NumberOfStrings;
	int PicSize1[2];
	int PicSize2[2];

#endif
protected:

public:
	int DCount; 
	int _pNum; //����� ���������� ���� ���������
	bool Fit; //���������� �������� � �������� ���� ��������� ��� ���

	void SetParent(CCustomContainer *val) {pParent = val;}
	// ������� ���������� �����������
	int GetBorderWidth() { return m_param.BorderWidth; }
	void SetBorderWidth(int val);
	
	COLORREF GetBorderColor() { return m_param.BorderColor; }
	void SetBorderColor(COLORREF val);

	COLORREF GetBorderColorFocus() { return m_param.BorderColorFocus; }
	void SetBorderColorFocus(COLORREF val);

	COLORREF GetBackColor() { return m_param.BackColor; }
	void SetBackColor(COLORREF val);

	COLORREF GetBackColorMouse() { return m_param.BackColorMouse; }
	void SetBackColorMouse(COLORREF val);

	long GetIndexCur() { return m_param.IndexCur; }
	void SetIndexCur(long val);

	CString GetCaption() { return m_param.Caption; }
	void SetCaption(CString str);

	ComboBoxType GetType() { return m_param.Type; }
	void SetType(ComboBoxType val);

    void SetTexture(CString path, bool texture);

	void CreateMenuButtons();

	void CreateScrollingItems();

	void SetMenuButtonsState(bool val);

	void SetTextColor(int r, int g, int b);

	void ComboboxUpButton();

	void ComboboxDownButton();

	void SetSpaceSize(CRect rect) { spaceRect = rect; }

	void DeleteMenuButtons();

#ifdef _GDIPLUS
	CZFont* GetFont() { return m_pFont; }
	void SetFont(CZFont *pFont) { m_pFont = pFont; }
#endif
	// ����������� �������, ���������������� � ������
	virtual void OnDraw();
	virtual void Clear (void);
	virtual void GetFocus();
	virtual void KillFocus();
	virtual void OnMouseEnter();
	virtual void OnMouseLeave();
	virtual void OnMouseWheel(UINT nflags, CPoint point, short delta);
    virtual void SaveParameters(SimpleXML *pSimpleXML);
    virtual void LoadParameters(SimpleXML *pSimpleXML);
	// ������� ������
	CMenu* GetMenu() { return &m_menu; }
	void SetTextString(CString val);
	void EnableMenuItem(long index, bool bEnable);
	int GetCounterItem() { return m_CounterItem; }
	void SetStartItemID(UINT_PTR val) { m_param.StartItemID = val; }

	int GetAmountOfItems() {return NumberOfStrings;} // ��������� ���� ���������� ��������� � ����������
	CString GetCaptionOfItem(int val) { return arrayOfNames[val]; } //��������� ������� ������������� ��������

	void CompareStrings(CString val);

	CRect GetButRect() { return m_rect; } //���������� ������������� � ������� �������� ������� �������
	int GetHeightCB() { return m_param.ItemHeight*_pVisNum; } //���������� ������ ����� �������� ���� ����������
	int GetWidhtCB() { return m_rect.right; } //���������� ������ ����� �������� ���� ����������

	void ReDrawMenuButtons();

	static void ScrollItems();

};
//------------------------------------------------------------------------------