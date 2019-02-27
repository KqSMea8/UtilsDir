//------------------------------------------------------------------------------
// CustomContainer.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include <vector>
#include <Interface2012\Custom\CustomAll.h>

//------------------------------------------------------------------------------
class CCustomContainer : public CCustomAll
{
public:
	CCustomContainer(CCustomContainer *owner);
	virtual ~CCustomContainer();
	
private:
protected:
	CCustomContainer *m_pOwner;
	std::vector<CCustomAll*> m_Vector;
	std::vector<CCustomAll*> m_Vector_2;

public:
	void AddClass(CCustomAll* pClass);
	CCustomAll* WhoOwnerMouse();
	void ReStartTabFocus();

	virtual	CString GetFolderPNG();
	virtual void MessageToParent(UINT msg, WPARAM wParam, LPARAM lParam)
	{ if (m_pOwner) m_pOwner->MessageToParent(msg, wParam, lParam); }

	virtual void DoProcess();
    virtual void DoAction(){}; //единоразовое действие
	virtual void OnDraw();
	virtual void OnSize();
    virtual void SaveParameters(SimpleXML *pSimpleXML);
    virtual void LoadParameters(SimpleXML *pSimpleXML);
	virtual void OnMouseMove(UINT nFlags, CPoint point);
//	virtual void OnLButtonDown(UINT nFlags, CPoint point);

	virtual void SetDataRect(CRect rect){}
	virtual void SetParamRect(CRect rect){}
	virtual void SetParamAddRect(CRect rect){}

	void ResetRadioButtons(CString nameActiveRB);
	virtual void CloseMenu();
	void CleanVector();
    int GetQntPlants();
};
//------------------------------------------------------------------------------