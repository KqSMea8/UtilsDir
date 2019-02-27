#pragma once

#include "../../utils/toolbar/toolbar.h"

class CMainToolBar : public CToolBarEx
{
public:
    virtual void LoadItemImages();

    virtual BOOL Create(
        CWnd* pParentWnd,
        UINT  nIDContainer,
        UINT  nIDResource );
};
