#ifndef _LIBUV_AUTO_LINK_H_
#define _LIBUV_AUTO_LINK_H_

/**/
#if !defined(_MSC_VER)
#error Not supported compiler
#endif

/* Architecture */
#ifdef _WIN64
#define LIBUV_LIB_ARCH "x64/"
#else
#define LIBUV_LIB_ARCH "Win32/"
#endif /* _WIN64 */

/* Run-Time */
#ifdef _DLL
#define LIBUV_LIB_RT "-md"
#else
#define LIBUV_LIB_RT "-mt"
#endif /* _DLL */

/* Configuration */
#ifdef _DEBUG
#define LIBUV_LIB_CFG "d"
#else
#define LIBUV_LIB_CFG ""
#endif /* _DEBUG */

/* Auto link */
#pragma comment(lib, "libuv/lib/" LIBUV_LIB_ARCH "libuv" LIBUV_LIB_RT LIBUV_LIB_CFG ".lib")

/* Cleanup */
#undef LIBUV_LIB_ARCH
#undef LIBUV_LIB_RT
#undef LIBUV_LIB_CFG

#include "uv.h"

#endif /* _LIBUV_AUTO_LINK_H_ */
