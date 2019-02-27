//------------------------------------------------------------------------------
// CustomElement.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include <Interface2012\Custom\CustomContainer.h>
//------------------------------------------------------------------------------
class CCustomElement : public CCustomAll
{
public:
	CCustomElement(CCustomContainer *owner);
	virtual ~CCustomElement();

private:
	void RegistrationFocus();
	
protected:
	CCustomContainer *m_pOwner;
	

public:
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnLButtonClk(UINT nFlags, CPoint point);
	virtual void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);

	virtual void OnPickItem(long index) {}
	
};
//------------------------------------------------------------------------------