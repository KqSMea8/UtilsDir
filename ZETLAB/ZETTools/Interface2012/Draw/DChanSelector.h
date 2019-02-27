//------------------------------------------------------------------------------
// DchanSelector.h : ���� ���������
//------------------------------------------------------------------------------
// ����� ����������� ChanSelector.
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Draw\CustomDraw.h"
#include "C:\ZETTools\Interface2012\ZetLab\Panel.h"
#include "C:\ZETTools\Interface2012\ZetLab\Button.h"
#include "C:\ZETTools\Interface2012\ZetLab\ScrollBar.h" //zettest
#define DETAILS 9
enum LevelType {ComputerName, DeviceName, ChannelName};
typedef void (*FunctionChan)();
//static bool flagScroll; //���� ���������� �� ��������� ���������
//------------------------------------------------------------------------------
struct ParamChanSelector : public ParamCustomDraw
{// � ������������ ��������� - ����� ���������, ����� - ������� ���������
	int BorderWidth;			// ������ �������
	COLORREF BorderColor;		// ���� ������� ��� ������
	COLORREF BorderColorFocus;	// ���� ������� � �������
	COLORREF BackColor;			// ���� ���� ��� ����
	COLORREF BackColorMouse;	// ���� ���� ��� ��������� ����
	COLORREF BackColorEnter;	// ���� ���� ��� ������� ������
	COLORREF CheckColorMouse;
	COLORREF CheckColor;
	COLORREF FieldSolidColor;
	CString Caption;			// �����, ������� ��������� �� ������
	CString CaptionChange;			// �����, ������� ��������� �� ������ ��� ������� ��� ������������� ������
	CString fileName;		// ������� ��� ������ � ����������
	UINT_PTR StartItemID;		// ID �������� Item ����, ������� ����� ������
	long IndexCur;				// ������� Item ����
#ifdef _GDIPLUS
	CZFont *pFont;		// ��������� ������ ������
#endif
	//������ ���������
	StringAlignment AlignmentHorizontal;	// ������������ �� �����������
	StringAlignment AlignmentVertical;		// ������������ �� ���������
	double dTimeAnimation;	// ������������ �������� ������
	double dTimeCur;		// ������� ����������������� ��������
	double dTimeStart;		// ����� ������ ��������
	int SizeText;			// ������ ���������� ���� (��� CheckBox) ??
	LinearGradientMode GradientMode;	// ��� ���������
	Point pt[3];	
	//����� ���������
	int ItemHeight; //������ ������ ���� chanselector
	CString Texture_1_FileName;
	CString Texture_2_FileName;
	CString Texture_3_FileName;
	CString Texture_4_FileName;
	int Number_Texture[4];
    CString m_textureName;
	CRect rect2;
	ParamChanSelector()
	{
		Size = sizeof(ParamChanSelector);
		Texture_1_FileName = L"C:\\ZETTools\\Interface2012\\Textures\\Patterns\\metal_mart14.bmp";
		Texture_2_FileName = L"C:\\ZETTools\\Interface2012\\Textures\\Seamless\\Other\\combo_down.bmp";
		Texture_3_FileName = L"C:\\ZETTools\\Interface2012\\Textures\\Seamless\\Other\\downbutton_01.bmp";
		Texture_4_FileName = L"C:\\ZETTools\\Interface2012\\Textures\\Seamless\\Other\\upbutton_01.bmp";
		BorderWidth = 0;		// � ������������ ������������ 1
		BorderColor = RGB(66, 66, 66);
		BorderColorFocus = RGB(66, 66, 66);	// 164, 210, 244;  150 210 255
		BackColor = RGB(240, 240, 240);
		BackColorMouse = RGB(223, 243, 255);
		BackColorEnter = RGB(240, 240, 240);
		FieldSolidColor = RGB(203, 203, 203);
		Number_Texture[0] = 0;
		Number_Texture[1] = 18;
		Number_Texture[2] = 21;
		Number_Texture[3] = 22;
		//ButtonType = false;
		//CheckBox = false;
		SizeText = 60;
		Caption = L"";
		CaptionChange = L"";
		IndexCur = 0;
		StartItemID = 0;
		CheckColorMouse = RGB (0,0,0);
		CheckColor = RGB (0,0,0);
#ifdef _GDIPLUS
		pFont = NULL;
#endif
		dTimeAnimation = 0.5;
		dTimeCur = 0.;
		dTimeStart = 0.;
		AlignmentHorizontal = StringAlignmentCenter;
		AlignmentVertical = StringAlignmentCenter;
		GradientMode = LinearGradientModeForwardDiagonal;
		fileName = L"";
		ItemHeight = 23;
	}
};

struct ParamItem
{
	LevelType levelType;
	UINT_PTR itemID; //id ��� ������ ��������������� ����
	bool expanded; //��� ���, � ���� ���� �������� ��������
	bool visible; //� ������ ������������/�������������� ����� �������� ��������
	CString Caption; //������� �� ��������
	CString ParentCaption; //������� �� �������� ��������
	bool hasChildren; //����� �� ������� �������� �������� 
	HTREEITEM parentID; //��������� �� ������������ ������� ������
	HTREEITEM personalID;
	CZButton *m_pButItem; //������ � �������� �������� ������, ���������� ��� �������
	CZButton *m_pButSlew; //������, ������������ ���� ��������� ��� ����� ������. �� ������������, ����� levelType = ChannelName
	int index; //���������� ��������� ����
	CString conversion; //������� ��������� ������
	long idChan; //������������� ������
	ParamItem()
	{
		levelType = ComputerName;
		itemID = NULL;
		expanded = false;
		visible = false;
		Caption = L"";
		ParentCaption = L"";
		hasChildren = false;
		index = 0;
		m_pButItem = nullptr;
		m_pButSlew = nullptr;
		conversion = L"";
		idChan = NULL;
	}
};

struct ExpStruct
{
	bool expand;
	CString Caption;
	CString ParentCaption;
	ExpStruct()
	{
		expand = false;
		Caption = L"";
		ParentCaption = L"";
	}
};

struct ColorsOfMenu{
	// ��������� ������ ��� ��������� ����
	GLfloat mat_ambient[4]; 	//	{r, g, b, a}
	GLfloat mat_diffuse[4];		//	{r, g, b, a}
	GLfloat mat_specular[4];	//	{r, g, b, a}
	GLfloat mat_emission[4];	//	{r, g, b, a}
	GLfloat mat_shininess;		//	����������� �����
	
	ColorsOfMenu(){
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

		mat_emission[0] = 0.;
		mat_emission[1] = 0.;
		mat_emission[2] = 0.;
		mat_emission[3] = 1.;

		mat_shininess = 4.24f;
	}
};
//------------------------------------------------------------------------------
class CDChanSelector : public CCustomDraw
{
public:
#ifdef _GDIPLUS
	CDChanSelector(CRect rect, PVOID ppGraphics,
		bool *pFlagIsChange, double *pTimeAnimation);
#else
	CDChanSelector(CRect rect, bool *pFlagIsChange, double *pTimeAnimation);
#endif

	virtual ~CDChanSelector();

private:
	ParamChanSelector m_param;	// ��������� ���� ���������� �����������
	ColorsOfMenu m_dinamic_color, m_color_com, m_color_device, m_color_chan, m_color_com1;
	COLORREF m_BackColorCur;
	COLORREF m_BorderColorCur;
	CRect m_rect1;			// ������������� ���������� ����
	bool m_bEvent;			// ���� �������
	bool m_bFixation;		// ���� ���� ������
	CCustomContainer *pParent;
	CTreeCtrl *Tree; //������, �������� � ���� �������� ��������� � �������
	int _pNum; //����� ���������� ��������� � ������
	int _pChanNum; //����� ���������� �������� � ������
	int _pVisNum; //����� ���������� ������� ���������
	int _pMaxVisNum; //����� ���������� ����������� ������� ��-�� �� ������
	CRect spaceRect; //������� � ������ ����������� ���������� ChanSelector
	CRect m_ButRect; // �������, � �������  ���������� �������� �������� ���� 
	//������ ��� ��������� ��������� ����... test
	CZButton *m_ButtonUp;
	CZButton *m_ButtonDown;
	CZScrollBar *m_pScrollBar; //��������� ���������
	int DCount; //������� �������� ������ ��� ������� "�����" � "����"
	bool m_bVisUD; //���� ����, ������ ���������� ������ ����� � ���� �� ������ ��� ���
	CString s_filterMeasure; //������ ��� ������� �� �������� ���������
	CString s_filterName; //������ ��� ������� �� �����
	CString s_filterIdChan; 
	bool b_filterMeasure; //����, ��������� � ���, ��� ������ ���������� �� �������� ���������
	bool b_FilterName; //����, ��������� � ���, ��� ������ ���������� �� �����
	bool b_FilterIdChan;

	std::vector<ParamItem> m_Vector; //������ �� ��������� ����
	std::vector<ParamItem*> m_VisVector; //������ �� ���������� �� ������� �������� ����
	ParamItem m_paramItem; //��������� ���������� �������� ����
	std::vector<ExpStruct> m_ExpVector; //������, ������������ ������������� � ����
	ExpStruct m_exp;
	FunctionChan pointFun; //��������� �� �������


#ifdef _GDIPLUS
	CZFont *m_pFont;
#else
	CDFonts *m_pFont;
	GLuint Texture;
	CDetail *m_ChanSelector;
	CDetail *m_pButton;
	CDetail *m_Border;
	CDetail *m_pPanel;
	CDetail *m_pComboBox[DETAILS];
	int PicSize1[2];
	int PicSize2[2];
	GLuint Texture1;
	GLuint Texture2;
//	int NumberOfStrings;


#endif

	CString m_Text;
	Image *m_pImage_Cur;	// ��. �� ������� ��������
	Image *m_pImage_s;		// ����������� �����������
	Image *m_pImage_m;		// ����������� � �����
	Image *m_pImage_p;		// ����������� � ������� ���������
	Image *m_pImage_pm;		// ����������� � ����� ��� checkbox � ������� ���������
protected:

public:
	void SetParent(CCustomContainer *val) {pParent = val;}
	// ������� ���������� �����������
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

	StringAlignment GetAlignmentHorizontal() {return m_param.AlignmentHorizontal; }
	void SetAlignmentHorizontal(StringAlignment val);

	StringAlignment GetAlignmentVertical() {return m_param.AlignmentVertical; }
	void SetAlignmentVertical(StringAlignment val);

	bool GetFixsation() { return m_bFixation; }
    void SetTexture(CString path, bool texture);
	CString GetImageFileName() { return m_param.fileName;}
	void SetImageFileName(CString str);

	int GetItemsCount() {return _pNum;}
	CString GetUseButtonCaption (void);
	void SetActiveButton (long val);

	// ����������� �������, ���������������� � ������
	virtual void OnDraw();
	virtual void GetFocus();
	virtual void KillFocus();
	virtual void OnMouseEnter();
	virtual void OnMouseLeave();
	virtual void Clear (void);
	//virtual void DoProcess();
	virtual void OnLButtonClk(UINT nFlags, CPoint point);
public:
	void SetTextString(CTreeCtrl *var); 
	void SetStartItemID(UINT_PTR val) { m_param.StartItemID = val; }

	long GetIndexCur();
	void SetIndexCur(long val); 
	void SetSpaceSize(CRect rect) { spaceRect = rect; } //������������� ���������� ������� ���������� ChanSelector
	void DrawChannelTree(); //drawing only Visible items 
	void ReDrawChannelTree(); //drawing only Visible items 
	void ExpandBranch();
	void DeleteElementDetails (void);

	//������ �� ���������� 
	void SetItemLevel(LevelType val) {m_paramItem.levelType = val;}
	void SetItemCaption(CString val) {m_paramItem.Caption = val;}
	void SetItemParentCaption(CString val) { m_paramItem.ParentCaption = val;}
	void SetItemVisible(bool val) {m_paramItem.visible = val;}
	void SetItemHasChildren(bool val) {m_paramItem.hasChildren = val;}
	void SetItemID(UINT_PTR val) {m_paramItem.itemID = val;}
	void SetItemConversion(CString val) {m_paramItem.conversion = val;}
	void SetExpanded(bool val) {m_paramItem.expanded = val;}
	void SetHTREEITEMItem(HTREEITEM val) {m_paramItem.personalID = val;}
	void SetHTREEITEMParent(HTREEITEM val) {m_paramItem.parentID = val;}
	void SetIdChan(long val) { m_paramItem.idChan = val; }
	void InsertVectorItem() { m_Vector.push_back(m_paramItem); }

	LevelType GetItemLevel() {return m_paramItem.levelType;}
	CString GetItemCaption() {return m_paramItem.Caption;}
	bool GetItemVisible() {return m_paramItem.visible;}
	bool GetItemHasChildren() {return m_paramItem.hasChildren;}
	UINT_PTR GetItemID() {return m_paramItem.itemID;}
	bool GetExpanded() {return m_paramItem.expanded;}
	HTREEITEM GetHTREEITEMItem() {return m_paramItem.personalID;}
	HTREEITEM GetHTREEITEMParent() {return m_paramItem.parentID;}

	void DeleteMenuButtons();

	void SetMenuColors();

	void SetMenuButtonsState(bool val);

	void DrawScrollingItems(); //�������� ��������� ��� ��������� �������� ������
	//�������� ������ ����� � ����
	void ButtonUp();
	void ButtonDown();

	CRect GetButRect() {return m_ButRect;} //���������� ������������� � ������� �������� ������� �������
	int GetHeightChs() {return m_param.ItemHeight*_pVisNum; } //���������� ������ ����� �������� ���� ������������
	int GetWidhtChs() {return  m_bVisUD*m_param.ItemHeight; } //���������� ������ ����� �������� ���� ������������
	void SetUnusedButtons (int val);

	static void ScrollItems(); 
	
	bool mPress; //������� ������ ��������� � ���, ��� ����������� ������
	void SetPress(bool val) { mPress = val;}
	
	void SaveExpand(); //���������� ��������� ������������� ������������
	void ClearVector();
	void FilterByMeasure(CString val);
	void FilterByName(CString val);
	void SetFilterMeasure(bool val) { b_filterMeasure = val; } //���������� ������� �� �������� ���������
	void SetFilterName(bool val) { b_FilterName = val; } //��������� ������� �� �����

	void SetMeasureString(CString val) { s_filterMeasure = val; } //��������� ������ ������ ��������� ��� ��������� �� ������� 
	void SetNameString(CString val) { s_filterName = val; } //��������� ������ �������� ��� ��������� �� �������
	
	bool IsWildCardMatching(const CString &sStr, const CString &sWild); //��������� ���� �����

	void FilterByIdChan(CString val);
	void SetFilterIdChan(bool val) { b_FilterIdChan = val; }
	void SetIdChanString(CString val) { s_filterIdChan = val; }
};
//------------------------------------------------------------------------------