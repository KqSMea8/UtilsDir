#ifndef _OPCF_AUTO_LINK_H_
#define _OPCF_AUTO_LINK_H_

#define OPCF_LIB_DIR "ZETOPC/lib/opcf/"

#if defined(_MSC_VER)

/* Architecture */
#ifdef _WIN64
#define OPCF_LIB_ARCH "amd64/"
#else
#define OPCF_LIB_ARCH "Win32/"
#endif /* _WIN64 */

/* Run-Time */
#ifdef _DLL
#define OPCF_LIB_RT "-md"
#else
#define OPCF_LIB_RT "-mt"
#endif /* _DLL */

/* Configuration */
#ifdef _DEBUG
#define OPCF_LIB_CFG "d"
#else
#define OPCF_LIB_CFG ""
#endif /* _DEBUG */

/* Extension */
#define OPCF_LIB_EXT ".lib"

#endif /* _MSC_VER */


#if defined(OPCF_LIB_ARCH) && defined(OPCF_LIB_RT) && defined(OPCF_LIB_EXT)
#pragma comment(lib, OPCF_LIB_DIR    \
                     OPCF_LIB_ARCH   \
                     "opcf"			\
                     OPCF_LIB_RT     \
                     OPCF_LIB_CFG    \
                     OPCF_LIB_EXT)
#else
#error Not supported compiler
#endif

/* Cleanup */
#ifdef OPCF_LIB_EXT
#undef OPCF_LIB_EXT
#endif

#ifdef OPCF_LIB_RT
#undef OPCF_LIB_RT
#endif

#ifdef OPCF_LIB_CFG
#undef OPCF_LIB_CFG
#endif

#ifdef OPCF_LIB_ARCH
#undef OPCF_LIB_ARCH
#endif

#ifdef OPCF_LIB_DIR
#undef OPCF_LIB_DIR
#endif

#endif /* _OPCF_AUTO_LINK_H_ */
