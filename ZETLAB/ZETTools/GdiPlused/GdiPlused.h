#pragma once

#include <GdiPlus.h>
#pragma comment(lib, "GdiPlus.lib")

class CGdiPlused
{
public:
    CGdiPlused()
    {
        using namespace Gdiplus;

        GdiplusStartupInput input;

        GdiplusStartup( &_token, &input, NULL );
    };

    virtual ~CGdiPlused()
    {
        using namespace Gdiplus;

        GdiplusShutdown( _token );
    };

private:
    ULONG_PTR _token;
};
