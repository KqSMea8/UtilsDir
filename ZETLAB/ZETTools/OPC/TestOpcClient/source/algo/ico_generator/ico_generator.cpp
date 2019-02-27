#include "stdafx.h"
#include "ico_generator.hpp"

#include <GdiPlus.h>

namespace utils {

namespace labels {
    
    const TCHAR empty   = _T(' ');
    const TCHAR integer = _T('i');
    const TCHAR real    = _T('r');
    const TCHAR boolean = _T('b');
    const TCHAR date    = _T('d');
    const TCHAR string  = _T('s');
    const TCHAR undef   = _T('?');
}

namespace colors {

    const COLORREF lgrey  = RGB(240, 240, 240);
    const COLORREF blue   = RGB(  0,  75, 255);
    const COLORREF green  = RGB(  0, 155,   0);
    const COLORREF red    = RGB(155,   0,   0);
    const COLORREF purple = RGB(155,  55, 255);
    const COLORREF azure  = RGB( 55, 155, 255);
    const COLORREF grey   = RGB(155, 155, 155);
}

namespace {

HICON generate_ico_impl(
    const TCHAR&    label,
    const COLORREF& labelColor,
    const int&      width,
    const int&      height )
{
    using namespace Gdiplus;

    Bitmap* bitmap = new Bitmap(width, height, PixelFormat32bppARGB);

    // Font for label
    Gdiplus::Font font(_T("Consolas"), 9, FontStyleBold);
    SolidBrush    fontBrush(Color(255, 255, 255, 255));

    SolidBrush background(Color(255, GetRValue(labelColor), GetGValue(labelColor), GetBValue(labelColor)));
    Pen        inStroke  (Color(200, 200, 200, 200));
    Pen        outStroke (Color(255, 120, 120, 120));

    StringFormat format;

    // Align center vertically & horizontally
    format.SetLineAlignment( StringAlignmentCenter );
    format.SetAlignment    ( StringAlignmentCenter );

    // Drawing
    Graphics graphics(bitmap);

    // Enable anti-aliasing
    graphics.SetSmoothingMode( SmoothingModeAntiAlias );

    // Draw background
    graphics.FillEllipse( &background, 1, 1, width - 2, height - 2 );
    graphics.DrawEllipse( &inStroke,   2, 2, width - 4, height - 4 );
    graphics.DrawEllipse( &outStroke,  1, 1, width - 2, height - 2 );
    // Draw label
    graphics.DrawString ( &label, 1, &font, RectF(0, 1, REAL(width), REAL(height)), &format, &fontBrush );

    // Return HICON
    HICON icon = NULL;
    bitmap->GetHICON( &icon );

    return icon;
}

} // unnamed namespace

HICON generate_ico(
    const VARTYPE& vt,
    const int&     width,
    const int&     height )
{
    switch (vt)
    {
    case VT_EMPTY:
        return generate_ico_impl( labels::empty, colors::lgrey, width, height );

    case VT_I1:
    case VT_UI1:
    case VT_I2:
    case VT_UI2:
    case VT_I4:
    case VT_UI4:
    case VT_I8:
    case VT_UI8:
    case VT_INT:
    case VT_UINT:
        return generate_ico_impl( labels::integer, colors::blue, width, height );

    case VT_R4:
    case VT_R8:
        return generate_ico_impl( labels::real, colors::green, width, height );

    case VT_BOOL:
        return generate_ico_impl( labels::boolean, colors::red, width, height );

    case VT_DATE:
        return generate_ico_impl( labels::date, colors::azure, width, height );

    case VT_BSTR:
        return generate_ico_impl( labels::string, colors::purple, width, height );

    default:
        return generate_ico_impl( labels::undef, colors::grey, width, height );
    }
}

} // namespace utils
