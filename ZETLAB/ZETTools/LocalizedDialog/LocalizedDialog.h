#pragma once

#include "../Localizator/localizator.h"

class CLocalizedDialog
{
public:
    virtual ~CLocalizedDialog() {};

protected:
    DECLARE_LOC_SUPPORT()
};
