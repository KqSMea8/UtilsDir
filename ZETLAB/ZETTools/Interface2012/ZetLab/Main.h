//------------------------------------------------------------------------------
// Main.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include <Interface2012\Custom\CustomMain.h>
#include <Interface2012\ZetLab\ButtonHMME.h>
#include <Interface2012\Draw\DMain.h>
//------------------------------------------------------------------------------
class CMain : public CCustomMain
{
public:
	CMain(CWnd* parent, CRect rect, CString sFolderPNG,
		bool bButtonHelp, bool bButtonMin, bool bButtonMax, bool bButtonEnd);
	virtual ~CMain();

private:
	CRect m_rect, m_data, m_param;
	CRgn *m_pRegion0, *m_pRegion1, *m_pRegion2, *m_pRegion3, *m_pRegion4;
	CRgn *m_pRegion5; //zettest
	bool m_bAddRect; //флаг дополнительной области для окна параметров
	CRect m_paramAdd;
	CPoint m_leftButtonDownPoint;
	CRect m_topSizeRect, m_leftTopSizeRect, m_leftSizeRect, m_leftBottomSizeRect,
		m_bottomSizeRect, m_rightBottomSizeRect, m_rightSizeRect, m_rightTopSizeRect; //зоны, которые отвечают за Resize окна
	bool m_topSize, m_leftTopSize, m_leftSize, m_leftBottomSize, m_bottomSize,
		m_rightBottomSize, m_rightSize, m_rightTopSize;
	int m_sizeBorder; //размер рамки-области Resize окна
protected:
public:
	CButtonHMME *m_pButtonEnd;
	CButtonHMME *m_pButtonMax;
	CButtonHMME *m_pButtonMin;
	CButtonHMME *m_pButtonHelp;
	
	void Init();

	virtual void Resize(CRect rect) override;

    virtual void SaveParameters(SimpleXML *pSimpleXML, CString FileName);
    virtual void LoadParameters(SimpleXML *pSimpleXML, CString FileName);
	virtual void Clear();
	virtual CWnd* GetParent() { return m_pParent; }
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnMouseMove(UINT nFlags, CPoint point);

	CRect GetDataRect() {return m_data;}
	virtual void SetDataRect(CRect rect);

	CRect GetParamRect() {return m_param;}
	virtual void SetParamRect(CRect rect);
	virtual void SetParamAddRect(CRect rect);

	void LoadIcon (CString str) { m_pDMain->LoadIcon(str);}

	void SetToolTipEnd (CString str) { m_pButtonEnd->AddToolTip(str); }
	void SetToolTipMax (CString str) { m_pButtonMax->AddToolTip(str); }
	void SetToolTipMin (CString str) { m_pButtonMin->AddToolTip(str); }
	void SetToolTipHelp (CString str) { m_pButtonHelp->AddToolTip(str); }

#ifdef _GDIPlUS
#else
	void SetRectMask(CRect rect);
#endif
};
//------------------------------------------------------------------------------