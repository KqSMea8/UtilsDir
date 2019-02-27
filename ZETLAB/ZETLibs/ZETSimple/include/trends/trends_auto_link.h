#ifndef _TRENDS_AUTO_LINK_H_
#define _TRENDS_AUTO_LINK_H_

#define TRENDS_LIB_DIR "ZETSimple/lib/trends/"

#if defined(_MSC_VER)

/* Architecture */
#ifdef _WIN64
#define TRENDS_LIB_ARCH "amd64/"
#else
#define TRENDS_LIB_ARCH "Win32/"
#endif /* _WIN64 */

/* Run-Time */
#ifdef _DLL
#define TRENDS_LIB_RT "-md"
#else
#define TRENDS_LIB_RT "-mt"
#endif /* _DLL */

/* Configuration */
#ifdef _DEBUG
#define TRENDS_LIB_CFG "d"
#else
#define TRENDS_LIB_CFG ""
#endif /* _DEBUG */

/* Extension */
#define TRENDS_LIB_EXT ".lib"

#endif /* _MSC_VER */


#if defined(TRENDS_LIB_ARCH) && defined(TRENDS_LIB_RT) && defined(TRENDS_LIB_EXT)
#pragma comment(lib, TRENDS_LIB_DIR    \
                     TRENDS_LIB_ARCH   \
                     "trends"			\
                     TRENDS_LIB_RT     \
                     TRENDS_LIB_CFG    \
                     TRENDS_LIB_EXT)
#else
#error Not supported compiler
#endif

/* Cleanup */
#ifdef TRENDS_LIB_EXT
#undef TRENDS_LIB_EXT
#endif

#ifdef TRENDS_LIB_RT
#undef TRENDS_LIB_RT
#endif

#ifdef TRENDS_LIB_CFG
#undef TRENDS_LIB_CFG
#endif

#ifdef TRENDS_LIB_ARCH
#undef TRENDS_LIB_ARCH
#endif

#ifdef TRENDS_LIB_DIR
#undef TRENDS_LIB_DIR
#endif

#endif /* _TRENDS_AUTO_LINK_H_ */
