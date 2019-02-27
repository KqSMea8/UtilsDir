#pragma once

#include "../Localizator/localizator.h"

class CLocalized
{
public:
    virtual ~CLocalized()
    {
    };

protected:
    DECLARE_LOC_SUPPORT()
};
