//------------------------------------------------------------------------------
// StaticImage.h : ���� ���������
//------------------------------------------------------------------------------
// ����� �������� "����������� ��������"
//------------------------------------------------------------------------------
#pragma once
#include "C:\ZETTools\Interface2012\Custom\CustomElement.h"
#include "C:\ZETTools\Interface2012\Draw\DStaticImage.h"
//------------------------------------------------------------------------------
class CStaticImage : public CCustomElement
{
public:
	CStaticImage(CCustomContainer *owner, CRect rect);
	virtual ~CStaticImage();

private:
	CDStaticImage *m_pDImage;

protected:

public:
	// ������� ���������� �����������
	int GetNumberElement() { return m_pDImage->GetNumberElement(); }

	CString GetImageFileName() { return m_pDImage->GetImageFileName();}
	void SetImageFileName(CString str) { m_pDImage->SetImageFileName(str); }

	float GetCoordZ() { return m_pDImage->GetCoordZ();}
	void SetCoord(float val) { m_pDImage->SetCoord(val);}

	// ����������� �������, ���������������� � ������
//	virtual void OnDraw();
};
//------------------------------------------------------------------------------