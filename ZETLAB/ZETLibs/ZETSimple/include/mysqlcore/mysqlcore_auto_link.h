#ifndef _MYSQLCORE_AUTO_LINK_H_
#define _MYSQLCORE_AUTO_LINK_H_

#define MYSQLCORE_LIB_DIR "ZETSimple/lib/mysqlcore/"

#if defined(_MSC_VER)

/* Architecture */
#ifdef _WIN64
#define MYSQLCORE_LIB_ARCH "amd64/"
#else
#define MYSQLCORE_LIB_ARCH "Win32/"
#endif /* _WIN64 */

/* Run-Time */
#ifdef _DLL
#define MYSQLCORE_LIB_RT "-md"
#else
#define MYSQLCORE_LIB_RT "-mt"
#endif /* _DLL */

/* Configuration */
#ifdef _DEBUG
#define MYSQLCORE_LIB_CFG "d"
#else
#define MYSQLCORE_LIB_CFG ""
#endif /* _DEBUG */

/* Extension */
#define MYSQLCORE_LIB_EXT ".lib"

#endif /* _MSC_VER */


#if defined(MYSQLCORE_LIB_ARCH) && defined(MYSQLCORE_LIB_RT) && defined(MYSQLCORE_LIB_EXT)
#pragma comment(lib, MYSQLCORE_LIB_DIR    \
                     MYSQLCORE_LIB_ARCH   \
                     "mysqlcore"			\
                     MYSQLCORE_LIB_RT     \
                     MYSQLCORE_LIB_CFG    \
                     MYSQLCORE_LIB_EXT)
#else
#error Not supported compiler
#endif

/* Cleanup */
#ifdef MYSQLCORE_LIB_EXT
#undef MYSQLCORE_LIB_EXT
#endif

#ifdef MYSQLCORE_LIB_RT
#undef MYSQLCORE_LIB_RT
#endif

#ifdef MYSQLCORE_LIB_CFG
#undef MYSQLCORE_LIB_CFG
#endif

#ifdef MYSQLCORE_LIB_ARCH
#undef MYSQLCORE_LIB_ARCH
#endif

#ifdef MYSQLCORE_LIB_DIR
#undef MYSQLCORE_LIB_DIR
#endif

#endif /* _MYSQLCORE_AUTO_LINK_H_ */
