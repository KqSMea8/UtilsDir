#ifndef _INVOICE_AUTO_LINK_H_
#define _INVOICE_AUTO_LINK_H_

#define INVOICE_LIB_DIR "ZETStorage/lib/invoice/"

#if defined(_MSC_VER)

/* Architecture */
#ifdef _WIN64
#define INVOICE_LIB_ARCH "amd64/"
#else
#define INVOICE_LIB_ARCH "Win32/"
#endif /* _WIN64 */

/* Run-Time */
#ifdef _DLL
#define INVOICE_LIB_RT "-md"
#else
#define INVOICE_LIB_RT "-mt"
#endif /* _DLL */

/* Configuration */
#ifdef _DEBUG
#define INVOICE_LIB_CFG "d"
#else
#define INVOICE_LIB_CFG ""
#endif /* _DEBUG */

/* Extension */
#define INVOICE_LIB_EXT ".lib"

#endif /* _MSC_VER */


#if defined(INVOICE_LIB_ARCH) && defined(INVOICE_LIB_RT) && defined(INVOICE_LIB_EXT)
#pragma comment(lib, INVOICE_LIB_DIR    \
                     INVOICE_LIB_ARCH   \
                     "invoice"			\
                     INVOICE_LIB_RT     \
                     INVOICE_LIB_CFG    \
                     INVOICE_LIB_EXT)
#else
#error Not supported compiler
#endif

/* Cleanup */
#ifdef INVOICE_LIB_EXT
#undef INVOICE_LIB_EXT
#endif

#ifdef INVOICE_LIB_RT
#undef INVOICE_LIB_RT
#endif

#ifdef INVOICE_LIB_CFG
#undef INVOICE_LIB_CFG
#endif

#ifdef INVOICE_LIB_ARCH
#undef INVOICE_LIB_ARCH
#endif

#ifdef INVOICE_LIB_DIR
#undef INVOICE_LIB_DIR
#endif

#endif /* _INVOICE_AUTO_LINK_H_ */
