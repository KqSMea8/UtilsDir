//------------------------------------------------------------------------------
// AutoScaleXY.h : ���� ���������
// ������ �� 12.08.2014
// ����������, ��� "����", �������� �.�.
//------------------------------------------------------------------------------
#pragma once
#include <list>
//------------------------------------------------------------------------------
enum TipContrl			// ��� ��������
{	// ��������, ��������� �������� �� �������� ������������� ���������� ����.
	// �������� ������������ �� ����� �� ����
	tipLeftTop,			// 0 - ������ ��������
	tipLeftBottom,		// 1 - ������ �������
	tipRightTop,		// 2 - ������� ��������
	tipRightBottom,		// 3 - ������� �������
	// �������������� ��������
	tipReSize,			// 4 - ������� � ����������� ���������
	tipReSizeNo,		// 5 - ������� �� �������� �� � ����� ������, �� ���
						// ��������� ����������� ��� �� �� tipReSize
	// ��������, ��������� ������� ��������� � ��������-�������.
	// ����� �������� �������� ����������� � ����.
	tipWithMaster,
};
//------------------------------------------------------------------------------
enum TipFlags
{
	tfLeft   = 1,		// ����� ������� ���������
	tfTop    = 2,		// ������� ������� ���������
	tfRight  = 4,		// ������ ������� ���������
	tfBottom = 8,		// ������ ������� ���������
	tfConstHeight = 16	// ������ �������� �� ����������
};
//------------------------------------------------------------------------------
struct UseControlReSize
{
	HWND hwnd;
	WORD flags;
	float _top;
	float _height;
	float _left;
	float _width;
	RECT rect;
	long _default_height;	// ����������� ������ ��������
};
//------------------------------------------------------------------------------
struct ControlNoSize
{
	WORD flags;
	HWND hwnd;
};
//------------------------------------------------------------------------------
struct ControlWithMaster
{
	WORD flags;
	WORD flagsMaster;
	TipContrl type; 
	HWND hwnd;
	HWND hwndMaster;
	long shift;
	UseControlReSize resize;
};
//------------------------------------------------------------------------------
class CAutoScaleXY
{
private:
	bool m_bNoFinish;
	HWND m_hParent;
	int m_HeightAll;
	int m_WidthAll;

	std::list<UseControlReSize> m_ListReSize;
	std::list<ControlNoSize> m_ListLeftTop;
	std::list<ControlNoSize> m_ListLeftBottom;
	std::list<ControlNoSize> m_ListRightTop;
	std::list<ControlNoSize> m_ListRightBottom;
	std::list<ControlWithMaster> m_ListWithMaster;

	RECT m_rectReSize;
	bool CheckControls();
	bool ScaleControl(UseControlReSize *puc);
	bool ShiftControl(HWND hwnd, int dx, int dy,
						bool bLeft = true, bool bTop = true);
	bool ShiftWithMaster(ControlWithMaster *par, int dx, int dy,
		HWND hCont = NULL, HWND hContMaster = NULL);
	bool ScaleWithMaster(ControlWithMaster *par, int dx, int dy);
	void SortControls();
	void TestAndMove(UseControlReSize *puc);
protected:

public:
	CAutoScaleXY();
	virtual ~CAutoScaleXY();
	bool Create(HWND hParent);

	int GetClientWidthAll() { return m_WidthAll; }
	int GetClientHeightAll() { return m_HeightAll; }

	int GetNumberControlReSize() { return m_ListReSize.size(); }

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
};
//------------------------------------------------------------------------------