//------------------------------------------------------------------------------
//	DeleteObject.h
//------------------------------------------------------------------------------
#pragma once
#include "afxdisp.h"
#include "afxwin.h"
#include "math.h"
#include <GdiPlus.h>
using namespace Gdiplus;
//------------------------------------------------------------------------------
template <class T>
void DeleteGdiPlusObject(T *pObject)
{
	if (pObject)
	{
		DeleteObject(pObject);
		delete pObject;
		pObject = NULL;
	}
}
//------------------------------------------------------------------------------