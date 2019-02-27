#ifndef _ZET7XXXPASSPORT_AUTO_LINK_H_
#define _ZET7XXXPASSPORT_AUTO_LINK_H_

#define ZET7XXXPASSPORT_LIB_DIR "ZETStorage/lib/zet7xxxpassport/"

#if defined(_MSC_VER)

/* Architecture */
#ifdef _WIN64
#define ZET7XXXPASSPORT_LIB_ARCH "amd64/"
#else
#define ZET7XXXPASSPORT_LIB_ARCH "Win32/"
#endif /* _WIN64 */

/* Run-Time */
#ifdef _DLL
#define ZET7XXXPASSPORT_LIB_RT "-md"
#else
#define ZET7XXXPASSPORT_LIB_RT "-mt"
#endif /* _DLL */

/* Configuration */
#ifdef _DEBUG
#define ZET7XXXPASSPORT_LIB_CFG "d"
#else
#define ZET7XXXPASSPORT_LIB_CFG ""
#endif /* _DEBUG */

/* Extension */
#define ZET7XXXPASSPORT_LIB_EXT ".lib"

#endif /* _MSC_VER */


#if defined(ZET7XXXPASSPORT_LIB_ARCH) && defined(ZET7XXXPASSPORT_LIB_RT) && defined(ZET7XXXPASSPORT_LIB_EXT)
#pragma comment(lib, ZET7XXXPASSPORT_LIB_DIR    \
                     ZET7XXXPASSPORT_LIB_ARCH   \
                     "zet7xxxpassport"			\
                     ZET7XXXPASSPORT_LIB_RT     \
                     ZET7XXXPASSPORT_LIB_CFG    \
                     ZET7XXXPASSPORT_LIB_EXT)
#else
#error Not supported compiler
#endif

/* Cleanup */
#ifdef ZET7XXXPASSPORT_LIB_EXT
#undef ZET7XXXPASSPORT_LIB_EXT
#endif

#ifdef ZET7XXXPASSPORT_LIB_RT
#undef ZET7XXXPASSPORT_LIB_RT
#endif

#ifdef ZET7XXXPASSPORT_LIB_CFG
#undef ZET7XXXPASSPORT_LIB_CFG
#endif

#ifdef ZET7XXXPASSPORT_LIB_ARCH
#undef ZET7XXXPASSPORT_LIB_ARCH
#endif

#ifdef ZET7XXXPASSPORT_LIB_DIR
#undef ZET7XXXPASSPORT_LIB_DIR
#endif

#endif /* _ZET7XXXPASSPORT_AUTO_LINK_H_ */
