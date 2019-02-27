#ifndef _SINGLETON_AUTO_LINK_H_
#define _SINGLETON_AUTO_LINK_H_

#define SINGLETON_LIB_DIR "ZETSimple/lib/singleton/"

#if defined(_MSC_VER)

/* Architecture */
#ifdef _WIN64
#define SINGLETON_LIB_ARCH "amd64/"
#else
#define SINGLETON_LIB_ARCH "Win32/"
#endif /* _WIN64 */

/* Run-Time */
#ifdef _DLL
#define SINGLETON_LIB_RT "-md"
#else
#define SINGLETON_LIB_RT "-mt"
#endif /* _DLL */

/* Configuration */
#ifdef _DEBUG
#define SINGLETON_LIB_CFG "d"
#else
#define SINGLETON_LIB_CFG ""
#endif /* _DEBUG */

/* Extension */
#define SINGLETON_LIB_EXT ".lib"

#endif /* _MSC_VER */


#if defined(SINGLETON_LIB_ARCH) && defined(SINGLETON_LIB_RT) && defined(SINGLETON_LIB_EXT)
#pragma comment(lib, SINGLETON_LIB_DIR    \
                     SINGLETON_LIB_ARCH   \
                     "singleton"			\
                     SINGLETON_LIB_RT     \
                     SINGLETON_LIB_CFG    \
                     SINGLETON_LIB_EXT)
#else
#error Not supported compiler
#endif

/* Cleanup */
#ifdef SINGLETON_LIB_EXT
#undef SINGLETON_LIB_EXT
#endif

#ifdef SINGLETON_LIB_RT
#undef SINGLETON_LIB_RT
#endif

#ifdef SINGLETON_LIB_CFG
#undef SINGLETON_LIB_CFG
#endif

#ifdef SINGLETON_LIB_ARCH
#undef SINGLETON_LIB_ARCH
#endif

#ifdef SINGLETON_LIB_DIR
#undef SINGLETON_LIB_DIR
#endif

#endif /* _SINGLETON_AUTO_LINK_H_ */
