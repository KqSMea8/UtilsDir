#ifndef HEADER_PUGIAUTOLINK_HPP
#define HEADER_PUGIAUTOLINK_HPP

#include "pugixml.hpp"

#define PUGIAUTOLINK_LIB_DIR "pugixml/lib/"

#if defined(_MSC_VER)

/* Architecture */
#ifdef _WIN64
#define PUGIAUTOLINK_LIB_ARCH "amd64/"
#else
#define PUGIAUTOLINK_LIB_ARCH "Win32/"
#endif /* _WIN64 */

/* Run-Time */
#ifdef _DLL
#define PUGIAUTOLINK_LIB_RT "-md"
#else
#define PUGIAUTOLINK_LIB_RT "-mt"
#endif /* _DLL */

/* Configuration */
#ifdef _DEBUG
#define PUGIAUTOLINK_LIB_CFG "d"
#else
#define PUGIAUTOLINK_LIB_CFG ""
#endif /* _DEBUG */

/* Extension */
#define PUGIAUTOLINK_LIB_EXT ".lib"

#endif /* _MSC_VER */


#if defined(PUGIAUTOLINK_LIB_ARCH) && defined(PUGIAUTOLINK_LIB_RT) && defined(PUGIAUTOLINK_LIB_EXT)
#pragma comment(lib, PUGIAUTOLINK_LIB_DIR    \
                     PUGIAUTOLINK_LIB_ARCH   \
                     "pugixml"			\
                     PUGIAUTOLINK_LIB_RT     \
                     PUGIAUTOLINK_LIB_CFG    \
                     PUGIAUTOLINK_LIB_EXT)
#else
#error Not supported compiler
#endif

/* Cleanup */
#ifdef PUGIAUTOLINK_LIB_EXT
#undef PUGIAUTOLINK_LIB_EXT
#endif

#ifdef PUGIAUTOLINK_LIB_RT
#undef PUGIAUTOLINK_LIB_RT
#endif

#ifdef PUGIAUTOLINK_LIB_CFG
#undef PUGIAUTOLINK_LIB_CFG
#endif

#ifdef PUGIAUTOLINK_LIB_ARCH
#undef PUGIAUTOLINK_LIB_ARCH
#endif

#ifdef PUGIAUTOLINK_LIB_DIR
#undef PUGIAUTOLINK_LIB_DIR
#endif

#endif
