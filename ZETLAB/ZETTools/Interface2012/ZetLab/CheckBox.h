//------------------------------------------------------------------------------
// MainCheckBox.h : файл заголовка
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Custom\CustomButton.h"
#include "C:\ZETTools\Interface2012\Draw\DButton.h"
//------------------------------------------------------------------------------
class CZCheckBox : public CCustomButton
{
public:
	CZCheckBox(CCustomContainer *owner, CRect rect);
	virtual ~CZCheckBox();

private:
	CDButton *m_pDCheckBox;
protected:

public:
	CString GetCaption() { return m_pDCheckBox->GetCaption();}
	void SetCaption(CString str){ m_pDCheckBox->SetCaption(str);}

	int GetSizeText() { return m_pDCheckBox->GetSizeText();}
	void SetSizeText(int val){m_pDCheckBox->SetSizeText(val);}
};
//------------------------------------------------------------------------------