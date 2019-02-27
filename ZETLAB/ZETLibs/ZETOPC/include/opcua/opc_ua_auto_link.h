#ifndef _OPCUA_AUTO_LINK_H_
#define _OPCUA_AUTO_LINK_H_

#define OPCUA_LIB_DIR "ZETOPC/lib/opcua/"

#if defined(_MSC_VER)

/* Architecture */
#ifdef _WIN64
#define OPCUA_LIB_ARCH "amd64/"
#else
#define OPCUA_LIB_ARCH "Win32/"
#endif /* _WIN64 */

/* Run-Time */
#ifdef _DLL
#define OPCUA_LIB_RT "-md"
#else
#define OPCUA_LIB_RT "-mt"
#endif /* _DLL */

/* Configuration */
#ifdef _DEBUG
#define OPCUA_LIB_CFG "d"
#else
#define OPCUA_LIB_CFG ""
#endif /* _DEBUG */

/* Extension */
#define OPCUA_LIB_EXT ".lib"

#endif /* _MSC_VER */


#if defined(OPCUA_LIB_ARCH) && defined(OPCUA_LIB_RT) && defined(OPCUA_LIB_EXT)
#pragma comment(lib, OPCUA_LIB_DIR    \
                     OPCUA_LIB_ARCH   \
                     "opcua"			\
                     OPCUA_LIB_RT     \
                     OPCUA_LIB_CFG    \
                     OPCUA_LIB_EXT)
#else
#error Not supported compiler
#endif

/* Cleanup */
#ifdef OPCUA_LIB_EXT
#undef OPCUA_LIB_EXT
#endif

#ifdef OPCUA_LIB_RT
#undef OPCUA_LIB_RT
#endif

#ifdef OPCUA_LIB_CFG
#undef OPCUA_LIB_CFG
#endif

#ifdef OPCUA_LIB_ARCH
#undef OPCUA_LIB_ARCH
#endif

#ifdef OPCUA_LIB_DIR
#undef OPCUA_LIB_DIR
#endif

#endif /* _OPCF_AUTO_LINK_H_ */
