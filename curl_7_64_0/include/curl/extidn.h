#ifndef __EXTIDN_H
#define __EXTIDN_H

#include "curl.h"  // For CURL_EXTERN

/*
 * Header file with declarations of external IDN implementation
 */

#ifdef  __cplusplus
extern "C" {
#endif

CURL_EXTERN bool ext_idna_to_unicode(const char* domain, char** encoded);
CURL_EXTERN bool ext_idna_to_ascii(const char* domain, char** encoded);

#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif  /* __EXTIDN_H */
