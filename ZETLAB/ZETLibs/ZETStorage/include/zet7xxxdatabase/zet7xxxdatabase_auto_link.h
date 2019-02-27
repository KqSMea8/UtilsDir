#ifndef _ZET7XXXDATABASE_AUTO_LINK_H_
#define _ZET7XXXDATABASE_AUTO_LINK_H_

#define ZET7XXXDATABASE_LIB_DIR "ZETStorage/lib/zet7xxxdatabase/"

#if defined(_MSC_VER)

/* Architecture */
#ifdef _WIN64
#define ZET7XXXDATABASE_LIB_ARCH "amd64/"
#else
#define ZET7XXXDATABASE_LIB_ARCH "Win32/"
#endif /* _WIN64 */

/* Run-Time */
#ifdef _DLL
#define ZET7XXXDATABASE_LIB_RT "-md"
#else
#define ZET7XXXDATABASE_LIB_RT "-mt"
#endif /* _DLL */

/* Configuration */
#ifdef _DEBUG
#define ZET7XXXDATABASE_LIB_CFG "d"
#else
#define ZET7XXXDATABASE_LIB_CFG ""
#endif /* _DEBUG */

/* Extension */
#define ZET7XXXDATABASE_LIB_EXT ".lib"

#endif /* _MSC_VER */


#if defined(ZET7XXXDATABASE_LIB_ARCH) && defined(ZET7XXXDATABASE_LIB_RT) && defined(ZET7XXXDATABASE_LIB_EXT)
#pragma comment(lib, ZET7XXXDATABASE_LIB_DIR    \
                     ZET7XXXDATABASE_LIB_ARCH   \
                     "zet7xxxdatabase"			\
                     ZET7XXXDATABASE_LIB_RT     \
                     ZET7XXXDATABASE_LIB_CFG    \
                     ZET7XXXDATABASE_LIB_EXT)
#else
#error Not supported compiler
#endif

/* Cleanup */
#ifdef ZET7XXXDATABASE_LIB_EXT
#undef ZET7XXXDATABASE_LIB_EXT
#endif

#ifdef ZET7XXXDATABASE_LIB_RT
#undef ZET7XXXDATABASE_LIB_RT
#endif

#ifdef ZET7XXXDATABASE_LIB_CFG
#undef ZET7XXXDATABASE_LIB_CFG
#endif

#ifdef ZET7XXXDATABASE_LIB_ARCH
#undef ZET7XXXDATABASE_LIB_ARCH
#endif

#ifdef ZET7XXXDATABASE_LIB_DIR
#undef ZET7XXXDATABASE_LIB_DIR
#endif

#endif /* _ZET7XXXDATABASE_AUTO_LINK_H_ */
