#pragma once

/*
������ ����� �������� ����������� �� CStatic, ��������� ��� ��������� ��� ��������,
���� � ���, � ������ ����������� ��������� ��������� �������
 -  SetText(CString str), ������� ������ ���������� ������
 -  SetColorBack(COLORREF backColor), ��������� ����� ������� ����
 -  SetColorText(COLORREF color), ��������� ����� ������
 -  ReconstructFont(CFont* font), ����� ������
 -  ����������� ������ ������ �� ���������� ����
 ������������ ������ CStatic � ����������� ��������� ������� ����������� ��� � � ���, ���
 ���� ��� ��������� � ������������ � ��������� ��������, �������� �� �����-������ ActiveX,
 �� ������� ���� ������������ �������������.
*/
class CLabel : public CStatic
{
	DECLARE_DYNAMIC(CLabel)

public:
	CLabel();
	virtual ~CLabel();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnMove(int x, int y);

public:
	//��������� ����� ������
	void SetColorText(COLORREF color);
	COLORREF m_crText;

	//��������� ����� ������� ����
	void SetColorBack(COLORREF backColor);
	COLORREF m_backColor;

	//������� ������ ���������� ������
	void SetText(CString str);
	CString text;

	//����������� ������ ������ �� ���������� ����
	void DrawTransparentBitmap(HDC hdc, HDC hdc_, CRect rect, COLORREF cTransparentColor);
	void SetTransparent(bool isTransp);
	bool m_bTransparent;

	//����� ������
	void ReconstructFont(CFont* font);
	CFont* m_font;
	LOGFONT	m_lf;

	void UpdateSurface();
};


