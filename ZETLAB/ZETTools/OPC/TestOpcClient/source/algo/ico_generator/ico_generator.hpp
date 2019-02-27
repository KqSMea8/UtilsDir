#ifndef ico_generator_hpp__
#define ico_generator_hpp__

#include <windows.h>

namespace utils {

HICON generate_ico(
    const VARTYPE& vt,
    const int&     width,
    const int&     height );

} // namespace utils

#endif // ico_generator_hpp__
