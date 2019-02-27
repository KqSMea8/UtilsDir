#ifndef _FRL_AUTO_LINK_H_
#define _FRL_AUTO_LINK_H_

#define FRL_LIB_DIR "ZETOPC/lib/frl/"

#if defined(_MSC_VER)

/* Architecture */
#ifdef _WIN64
#define FRL_LIB_ARCH "amd64/"
#else
#define FRL_LIB_ARCH "Win32/"
#endif /* _WIN64 */

/* Run-Time */
#ifdef _DLL
#define FRL_LIB_RT "-md"
#else
#define FRL_LIB_RT "-mt"
#endif /* _DLL */

/* Configuration */
#ifdef _DEBUG
#define FRL_LIB_CFG "d"
#else
#define FRL_LIB_CFG ""
#endif /* _DEBUG */

/* Extension */
#define FRL_LIB_EXT ".lib"

#endif /* _MSC_VER */


#if defined(FRL_LIB_ARCH) && defined(FRL_LIB_RT) && defined(FRL_LIB_EXT)
#pragma comment(lib, FRL_LIB_DIR    \
                     FRL_LIB_ARCH   \
                     "frl"			\
                     FRL_LIB_RT     \
                     FRL_LIB_CFG    \
                     FRL_LIB_EXT)
#else
#error Not supported compiler
#endif

/* Cleanup */
#ifdef FRL_LIB_EXT
#undef FRL_LIB_EXT
#endif

#ifdef FRL_LIB_RT
#undef FRL_LIB_RT
#endif

#ifdef FRL_LIB_CFG
#undef FRL_LIB_CFG
#endif

#ifdef FRL_LIB_ARCH
#undef FRL_LIB_ARCH
#endif

#ifdef FRL_LIB_DIR
#undef FRL_LIB_DIR
#endif

#endif /* _FRL_AUTO_LINK_H_ */
