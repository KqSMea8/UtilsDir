#ifndef _LOGGER_AUTO_LINK_H_
#define _LOGGER_AUTO_LINK_H_

#define LOGGER_LIB_DIR "ZETSimple/lib/logger/"

#if defined(_MSC_VER)

/* Architecture */
#ifdef _WIN64
#define LOGGER_LIB_ARCH "amd64/"
#else
#define LOGGER_LIB_ARCH "Win32/"
#endif /* _WIN64 */

/* Run-Time */
#ifdef _DLL
#define LOGGER_LIB_RT "-md"
#else
#define LOGGER_LIB_RT "-mt"
#endif /* _DLL */

/* Configuration */
#ifdef _DEBUG
#define LOGGER_LIB_CFG "d"
#else
#define LOGGER_LIB_CFG ""
#endif /* _DEBUG */

/* Extension */
#define LOGGER_LIB_EXT ".lib"

#endif /* _MSC_VER */


#if defined(LOGGER_LIB_ARCH) && defined(LOGGER_LIB_RT) && defined(LOGGER_LIB_EXT)
#pragma comment(lib, LOGGER_LIB_DIR    \
                     LOGGER_LIB_ARCH   \
                     "logger"			\
                     LOGGER_LIB_RT     \
                     LOGGER_LIB_CFG    \
                     LOGGER_LIB_EXT)
#else
#error Not supported compiler
#endif

/* Cleanup */
#ifdef LOGGER_LIB_EXT
#undef LOGGER_LIB_EXT
#endif

#ifdef LOGGER_LIB_RT
#undef LOGGER_LIB_RT
#endif

#ifdef LOGGER_LIB_CFG
#undef LOGGER_LIB_CFG
#endif

#ifdef LOGGER_LIB_ARCH
#undef LOGGER_LIB_ARCH
#endif

#ifdef LOGGER_LIB_DIR
#undef LOGGER_LIB_DIR
#endif

#endif /* _LOGGER_AUTO_LINK_H_ */
