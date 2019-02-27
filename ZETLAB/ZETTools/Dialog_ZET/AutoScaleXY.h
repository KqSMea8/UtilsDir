//------------------------------------------------------------------------------
// AutoScaleXY.h : файл заголовка
// Зеленоград, ЗАО "ЭТМС", Савостин Ю.М.
//------------------------------------------------------------------------------
#pragma once
#include <list>
//------------------------------------------------------------------------------
enum TipContrl			// тип элемента
{	// Элемент, положение которого не меняется отностительно заданного угла, с
	// возможным масштабированием по одному из направлений.
	tipLeftTop,			// 0 - элемент будет привязян к левому верхнему углу
	tipLeftBottom,		// 1 - элемент будет привязян к левому нижнему углу
	tipRightTop,		// 2 - элемент будет привязян к правому верхнему углу
	tipRightBottom,		// 3 - элемент будет привязян к правому нижнему углу
	// Масштабируемый элемент
	tipReSize,			// 4 - элемент с изменяемыми размерами
	tipReSizeNo,		// 5 - элемент не вносится ни в какой список, но его
						// положение учитывается как бы он tipReSize
	// Элемент, положение которого привязано в элементу-мастеру.
	// Также возможна привязка антагориста к углу.
	tipWithMaster,		// 6
};
//------------------------------------------------------------------------------
// Флаги привязываемых границ
enum TipFlags
{
	tfLeft   = 1,		// левая граница привязана
	tfTop    = 2,		// верхняя граница привязана
	tfRight  = 4,		// правая граница привязана
	tfBottom = 8,		// нижняя граница привязана
	tfConstHeight = 16	// высота элемента не изменяется
};
//------------------------------------------------------------------------------
// Структура параметров масштабируемых элементов
struct UseControlReSize
{
	WORD flags;
	HWND hwnd;
	long _default_height;	// изначальная высота элемента
	float _top;
	float _height;
	float _left;
	float _width;
	RECT rect;

	UseControlReSize()
	{
		ZeroMemory(this, sizeof(UseControlReSize));
	}
};
//------------------------------------------------------------------------------
struct ControlNoSize
{
	WORD flags;
	HWND hwnd;
	int WidthError = 0;		// если ширина контрола стала отрицательной запоминаем на сколько
	int HeightError = 0;	// если высота контрола стала отрицательной запоминаем на сколько

	ControlNoSize()
	{
		ZeroMemory(this, sizeof(ControlNoSize));
	}
};
//------------------------------------------------------------------------------
// Структура параметров простых не масштабируемых элементов
struct ControlNoSizeWithShow : public ControlNoSize
{
	bool bShow;		// отображение элемента
	long index;		// индекс
	long dy;
	RECT rect;

	ControlNoSizeWithShow()
	{
		ZeroMemory(this, sizeof(ControlNoSizeWithShow));
		bShow = true;
	}
	bool operator < (const ControlNoSizeWithShow & val)
	{
		return rect.top < val.rect.top;
	}
};
//------------------------------------------------------------------------------
// Структура параметров элементов, привязанных к элементу-мастеру
struct ControlWithMaster
{
	WORD flags;
	WORD flagsMaster;
	TipContrl type; 
	HWND hwnd;
	HWND hwndMaster;
	long shift;
	UseControlReSize resize;

	ControlWithMaster()
	{
		ZeroMemory(this, sizeof(ControlWithMaster));
	}
};
//------------------------------------------------------------------------------
class CAutoScaleXY
{
private:
	bool m_bInit = false;
	CWnd * m_pParent = nullptr;
	long m_cy_all = 0;
	long m_cx_all = 0;
	long m_top0_RightTop = 0;

 	std::list<ControlNoSize> m_list_LeftTop;
	std::list<ControlNoSize> m_list_LeftBottom;
	std::list<ControlNoSizeWithShow> m_list_RightTop_Src;
	std::list<ControlNoSizeWithShow> m_list_RightTop;
	std::list<ControlNoSize> m_list_RightBottom;

	std::list<UseControlReSize> m_list_ReSize;
	std::list<ControlWithMaster> m_list_WithMaster;

	CRect m_rectReSize;
	bool CheckControlsReSize();
	bool ScaleControl(UseControlReSize & puc);
	bool ShiftControl(ControlNoSize & control, int dx, int dy,
						bool bLeft = true, bool bTop = true);
	bool ShiftWithMaster(ControlWithMaster & par, int dx, int dy,
		CWnd *pCont = NULL, CWnd *pContMaster = NULL);
	bool ScaleWithMaster(ControlWithMaster & par, int dx, int dy);
	void SortControls();
	void TestAndMove(UseControlReSize & puc);
	void AddControlFinish_RightTop();

protected:

public:
	CAutoScaleXY(CWnd * parent = nullptr);
	virtual ~CAutoScaleXY();
	bool Create(CWnd * parent);

	int GetClientWidthAll() { return m_cx_all; }
	int GetClientHeightAll() { return m_cy_all; }

	int GetNumberControlReSize() { return m_list_ReSize.size(); }

	bool AddControl(HWND hControl, TipContrl type, WORD flags = 0);
	bool AddControlWithMaster(HWND hControl, TipContrl type, WORD flags,
		HWND hControlMaster, WORD flagsMaster, long shift = 0);
	void AddControlFinish();

	bool AutoScale(int cx, int cy);
	bool DeleteControl(HWND hControlDel, HWND hControlNew = 0);
	bool DivideControl(HWND hControlDiv, bool flagTop,
						HWND hControl1, HWND hControl2 = 0);
	bool EqualHight();
	bool IsEmpty();
	bool NewControl(HWND hControl, int HeightAsPercentOfMiddle = 100);

	bool Show(HWND hControl, bool bShow);
};
//------------------------------------------------------------------------------