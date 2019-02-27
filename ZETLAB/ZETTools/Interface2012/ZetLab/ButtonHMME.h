//------------------------------------------------------------------------------
// ButtonHMME.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include <Interface2012\Custom\CustomElement.h>
#include <Interface2012\Draw\DButtonHMME.h>

//------------------------------------------------------------------------------
class CButtonHMME : public CCustomElement
{
public:
	CButtonHMME(CCustomContainer *owner, CRect rect, TypeButtonHMME codeButton);
	virtual ~CButtonHMME();

private:
	WPARAM m_command;
	CDButtonHMME *m_pDButton;

protected:

public:
	virtual void OnLButtonClk(UINT nFlags, CPoint point);
	virtual void Clear (void);
	virtual void Resize(CRect rect) override;
};
//------------------------------------------------------------------------------