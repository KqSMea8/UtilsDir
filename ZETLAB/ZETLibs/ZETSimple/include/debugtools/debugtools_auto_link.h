#ifndef _DEBUGTOOLS_AUTO_LINK_H_
#define _DEBUGTOOLS_AUTO_LINK_H_

#define DEBUGTOOLS_LIB_DIR "ZETSimple/lib/debugtools/"

#if defined(_MSC_VER)

/* Architecture */
#ifdef _WIN64
#define DEBUGTOOLS_LIB_ARCH "amd64/"
#else
#define DEBUGTOOLS_LIB_ARCH "Win32/"
#endif /* _WIN64 */

/* Run-Time */
#ifdef _DLL
#define DEBUGTOOLS_LIB_RT "-md"
#else
#define DEBUGTOOLS_LIB_RT "-mt"
#endif /* _DLL */

/* Configuration */
#ifdef _DEBUG
#define DEBUGTOOLS_LIB_CFG "d"
#else
#define DEBUGTOOLS_LIB_CFG ""
#endif /* _DEBUG */

/* Extension */
#define DEBUGTOOLS_LIB_EXT ".lib"

#endif /* _MSC_VER */


#if defined(DEBUGTOOLS_LIB_ARCH) && defined(DEBUGTOOLS_LIB_RT) && defined(DEBUGTOOLS_LIB_EXT)
#pragma comment(lib, DEBUGTOOLS_LIB_DIR    \
                     DEBUGTOOLS_LIB_ARCH   \
                     "debugtools"		   \
                     DEBUGTOOLS_LIB_RT     \
                     DEBUGTOOLS_LIB_CFG    \
                     DEBUGTOOLS_LIB_EXT)
#else
#error Not supported compiler
#endif

/* Cleanup */
#ifdef DEBUGTOOLS_LIB_EXT
#undef DEBUGTOOLS_LIB_EXT
#endif

#ifdef DEBUGTOOLS_LIB_RT
#undef DEBUGTOOLS_LIB_RT
#endif

#ifdef DEBUGTOOLS_LIB_CFG
#undef DEBUGTOOLS_LIB_CFG
#endif

#ifdef DEBUGTOOLS_LIB_ARCH
#undef DEBUGTOOLS_LIB_ARCH
#endif

#ifdef DEBUGTOOLS_LIB_DIR
#undef DEBUGTOOLS_LIB_DIR
#endif

#endif /* _DEBUGTOOLS_AUTO_LINK_H_ */
