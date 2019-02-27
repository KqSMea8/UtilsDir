//------------------------------------------------------------------------------
// CustomContainer.cpp : файл реализации
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include <Interface2012\Custom\CustomContainer.h>
#include <Interface2012\Draw\CustomDraw.h>
#ifdef _ACTIVEX2012
#include <Interface2012\Custom\ActiveXBase.h>
#else
#include <Interface2012\Custom\Dialog_ZETi12.h>
#endif


//------------------------------------------------------------------------------
CCustomContainer::CCustomContainer(CCustomContainer *owner) : CCustomAll()
{
	m_pCOwner = owner;
	m_pOwner = owner;
	m_Vector.clear();

	if (m_pOwner)
	{
		m_pOwner->AddClass(this);
		m_hCursor = m_pOwner->m_hCursor;
	}
	else
		m_hCursor = 0;
}
//------------------------------------------------------------------------------
CCustomContainer::~CCustomContainer()
{
	/*if (m_Vector.size() > 0)
		for(auto it=m_Vector.begin(); it!=m_Vector.end(); )
			{
				(*it)->Clear();
				it = m_Vector.erase(it);
			}*/
}
//------------------------------------------------------------------------------
CString CCustomContainer::GetFolderPNG()
{
	CString ret(L"");
	if (m_pOwner)
		ret = m_pOwner->GetFolderPNG();
	else
		ret.Empty();
	return ret;
}
//------------------------------------------------------------------------------
void CCustomContainer::AddClass(CCustomAll* pClass)
{
	m_Vector.push_back(pClass);
}
//------------------------------------------------------------------------------
void CCustomContainer::ReStartTabFocus()
{
//	if (m_Vector.size() > 0)
	if (!m_Vector.empty())
		for(auto it=m_Vector.begin(); it!=m_Vector.end(); ++ it)
			(*it)->m_IndexTabFocus = -1;
}
//------------------------------------------------------------------------------
void CCustomContainer::OnDraw()
{// Вначале надо перерисовать себя, а затем своих вассалов
//	if (m_Vector.size() > 0)	// Перерисовка вассалов
	if (!m_Vector.empty())	// Перерисовка вассалов
	{
		for(auto it=m_Vector.begin(); it!=m_Vector.end(); ++ it)
			(*it)->OnDraw();
	}	
}
//------------------------------------------------------------------------------
void CCustomContainer::OnSize()
{
}
//------------------------------------------------------------------------------
void CCustomContainer::DoProcess()
{
//	if (m_Vector.size() > 0)
	if (!m_Vector.empty())
	{
		for(auto it=m_Vector.begin(); it!=m_Vector.end(); ++ it)
			(*it)->DoProcess();	

	}
}
//------------------------------------------------------------------------------
CCustomAll* CCustomContainer::WhoOwnerMouse()
{// Кто владелец мыши? Вариантов только 3:
//	1. я
//	2. кто-то из моих вассалов
//	3. мой сюзерен или кто-то из его вассалов
//	Вассал моего вассала не может быть владельцем мыши!
	CCustomAll* pret = NULL;
	if ( GetMouseStatus() )
		pret = this;	// я владелец
	else
	{
//		if (m_Vector.size() > 0)
		if (!m_Vector.empty())
		{// владелец кто-то из моих вассалов
			for(auto it=m_Vector.begin(); it!=m_Vector.end(); ++ it)
			{
				if ( (*it)->GetMouseStatus() )
				{
					pret = *it;
					break;
				}
			}
		}
		// владелец мой сюзерен или его вассалы
		if (pret == NULL && m_pOwner)	// если я не Main
			pret = m_pOwner->WhoOwnerMouse();
	}
	//if (pret)
	//	TRACE("MOUSE OWNER %s\n", (CStringA)pret->GetName());
	return pret;
}
//------------------------------------------------------------------------------
void CCustomContainer::OnMouseMove(UINT nFlags, CPoint point)
{
	CCustomAll *pTemp = NULL;
	bool m_bMyMessage = true;
//	if (m_Vector.size() > 0)
	if (!m_Vector.empty())
	{
		for(auto it=m_Vector.begin(); it!=m_Vector.end(); ++ it)
		{
			if ( (*it)->GetVisible() && PointInRect( point, &(*it)->GetRect() ))
			{
					pTemp = *it;
			}
		}
		if (pTemp)
		{
			pTemp->OnMouseMove(nFlags, point);
			m_bMyMessage = false;
		}
	}
	CWnd *pParent = GetParent();
	if (m_bMyMessage)
	{// я получил мышь
		CCustomAll::OnMouseMove(nFlags, point);		// ToolTip
		if ( !GetMouseStatus() )
		{// раньше мышью не владел
			pTemp = WhoOwnerMouse();	// потерявший мышь
			if (pTemp)
				pTemp->OnMouseLeave();
			OnMouseEnter();
			if (pParent)
			{
#ifdef _ACTIVEX2012
				((CActiveXBaseCtrl*)pParent)->RegistrationOwnerMouse(this);
#else
				((CDialog_ZETi12*)pParent)->RegistrationOwnerMouse(this);
#endif
				if ( GetVisible() && GetEnable() )
					SetClassLongW(pParent->m_hWnd, GCL_HCURSOR, (LONG)m_hCursor);
			}
		}
	}
//	else	// и раньше мышь была у меня, ничего делать не надо
}
//------------------------------------------------------------------------------
void CCustomContainer::ResetRadioButtons(CString nameActiveRB)
{
//	if (m_Vector.size() > 0)
	if (!m_Vector.empty())
	{
		for(auto it=m_Vector.begin(); it!=m_Vector.end(); ++ it)			
			if ((*it)->GetName()!=nameActiveRB)
			{
				(*it)->SetElementState(false);
			}	
			else if ((*it)->GetName()==nameActiveRB)
			{
				(*it)->SetElementState(true);
			}		
	}
}
//------------------------------------------------------------------------------
void CCustomContainer::SaveParameters(SimpleXML *pSimpleXML)
{
//  if (m_Vector.size() > 0)	// Перерисовка вассалов
	if (!m_Vector.empty())	// Перерисовка вассалов
    {
        for(auto it=m_Vector.begin(); it!=m_Vector.end(); ++ it)
            (*it)->SaveParameters(pSimpleXML);
    }
}
//------------------------------------------------------------------------------
void CCustomContainer::LoadParameters(SimpleXML *pSimpleXML)
{
//  if (m_Vector.size() > 0)	// Перерисовка вассалов
	if (!m_Vector.empty())	// Перерисовка вассалов
    {
        for(auto it=m_Vector.begin(); it!=m_Vector.end(); ++ it)
            (*it)->LoadParameters(pSimpleXML);
    }
}	
//------------------------------------------------------------------------------
void CCustomContainer::CleanVector()
{//используется для удаления кнопок, принадлежащих Combobox и ChanSelector, из общего вектора
//	if (m_Vector.size() > 0)
	if (!m_Vector.empty())
	{
		for(auto it=m_Vector.begin(); it!=m_Vector.end(); ++ it)			
			if ((*it)->Removal != true)
				m_Vector_2.push_back(*it);
			else 
			{
				(*it)->Clear();
				delete *it;
			}
	}
	m_Vector.swap(m_Vector_2);
	m_Vector_2.clear();
}
//------------------------------------------------------------------------------
int CCustomContainer::GetQntPlants()
{
    return 0;
}
//------------------------------------------------------------------------------
void CCustomContainer::CloseMenu()
{
	SetParamAddRect(CRect(0, 0, 0, 0));

//	if (m_Vector.size() > 0)
	if (!m_Vector.empty())
	{
		for (auto it = m_Vector.begin(); it != m_Vector.end(); ++ it)
		{
			(*it)->m_Press = false;
			(*it)->SetPress(false);
			if ((*it)->BelongsMenu)
			{
				(*it)->SetVisible(false);
				(*it)->AddToolTip(L"");
			}
			if ((*it)->OpenMenu)
			{
				(*it)->SetParamAddRect(CRect(0, 0, 0, 0));
				(*it)->CloseMenu();
			}
		}
	}
	OpenMenu = false;
}
//------------------------------------------------------------------------------
