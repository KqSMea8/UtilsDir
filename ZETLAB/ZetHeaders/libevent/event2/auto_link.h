#ifndef _EVENT2_AUTO_LINK_H_
#define _EVENT2_AUTO_LINK_H_

#define EV2_LIB_DIR "C:/ZETLibs/libevent/"

#if defined(_MSC_VER)

/* Architecture */
#ifdef _WIN64
#define EV2_LIB_ARCH "amd64/"
#else
#define EV2_LIB_ARCH ""
#endif /* _WIN64 */

/* Run-Time */
#ifdef _DLL
#define EV2_LIB_RT "-md"
#else
#define EV2_LIB_RT "-mt"
#endif /* _DLL */

/* Configuration */
#ifdef _DEBUG
#define EV2_LIB_CFG "d"
#else
#define EV2_LIB_CFG ""
#endif /* _DEBUG */

/* Extension */
#define EV2_LIB_EXT ".lib"

#endif /* _MSC_VER */


#if defined(EV2_LIB_ARCH) && defined(EV2_LIB_RT) && defined(EV2_LIB_EXT)
#pragma comment(lib, EV2_LIB_DIR    \
                     EV2_LIB_ARCH   \
                     "libevent"     \
                     EV2_LIB_RT     \
                     EV2_LIB_CFG    \
                     EV2_LIB_EXT)
#else
#error Not supported compiler
#endif

/* Cleanup */
#ifdef EV2_LIB_EXT
#undef EV2_LIB_EXT
#endif

#ifdef EV2_LIB_RT
#undef EV2_LIB_RT
#endif

#ifdef EV2_LIB_CFG
#undef EV2_LIB_CFG
#endif

#ifdef EV2_LIB_ARCH
#undef EV2_LIB_ARCH
#endif

#ifdef EV2_LIB_DIR
#undef EV2_LIB_DIR
#endif

#endif /* _EVENT2_AUTO_LINK_H_ */
