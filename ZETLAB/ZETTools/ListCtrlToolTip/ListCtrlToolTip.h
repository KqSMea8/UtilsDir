#pragma once
class CListCtrlToolTip : public CListCtrl
{
	int m_LastToolTipCol;
	int m_LastToolTipRow;
	
	CToolTipCtrl m_OwnToolTipCtrl;
public:
	CListCtrlToolTip();
	~CListCtrlToolTip();

	DECLARE_MESSAGE_MAP()

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);					// отслеживание движения мыши для отображения тултипов
	afx_msg BOOL OnToolNeedText(UINT id, NMHDR* pNMHDR, LRESULT* pResult);
	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void CellHitTest(const CPoint& pt, int& nRow, int& nCol) const;
	CString GetToolTipText(int nRow, int nCol);
};

