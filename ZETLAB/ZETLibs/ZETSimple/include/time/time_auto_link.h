#ifndef _TIME_AUTO_LINK_H_
#define _TIME_AUTO_LINK_H_

#define TIME_LIB_DIR "ZETSimple/lib/time/"

#if defined(_MSC_VER)

/* Architecture */
#ifdef _WIN64
#define TIME_LIB_ARCH "amd64/"
#else
#define TIME_LIB_ARCH "Win32/"
#endif /* _WIN64 */

/* Run-Time */
#ifdef _DLL
#define TIME_LIB_RT "-md"
#else
#define TIME_LIB_RT "-mt"
#endif /* _DLL */

/* Configuration */
#ifdef _DEBUG
#define TIME_LIB_CFG "d"
#else
#define TIME_LIB_CFG ""
#endif /* _DEBUG */

/* Extension */
#define TIME_LIB_EXT ".lib"

#endif /* _MSC_VER */


#if defined(TIME_LIB_ARCH) && defined(TIME_LIB_RT) && defined(TIME_LIB_EXT)
#pragma comment(lib, TIME_LIB_DIR    \
                     TIME_LIB_ARCH   \
                     "time"			\
                     TIME_LIB_RT     \
                     TIME_LIB_CFG    \
                     TIME_LIB_EXT)
#else
#error Not supported compiler
#endif

/* Cleanup */
#ifdef TIME_LIB_EXT
#undef TIME_LIB_EXT
#endif

#ifdef TIME_LIB_RT
#undef TIME_LIB_RT
#endif

#ifdef TIME_LIB_CFG
#undef TIME_LIB_CFG
#endif

#ifdef TIME_LIB_ARCH
#undef TIME_LIB_ARCH
#endif

#ifdef TIME_LIB_DIR
#undef TIME_LIB_DIR
#endif

#endif /* _TIME_AUTO_LINK_H_ */
