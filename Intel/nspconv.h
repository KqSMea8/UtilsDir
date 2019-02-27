/*M*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 1995 Intel Corporation. All Rights Reserved.
//
//      $Workfile: nspconv.h $
//      $Revision: 3 $
//      $Modtime: Dec 16 1996 4:22p $
//
//  Purpose: NSP Convolution
*M*/
#ifdef __cplusplus
extern "C" {
#endif
#if !defined (_NSPCONV_H) || defined (_OWN_BLDPCS)
    #define _NSPCONV_H
/*-------------------------------------------------------------------------*/
/*         Conv                                                            */
/*                                                                         */
/* Performs finite, linear convolution of two sequences.                   */

    NSPAPI(void,nspsConv, (const float  *x, int xLen,
                           const float  *h, int hLen, float  *y))

    NSPAPI(void,nspcConv, (const SCplx  *x, int xLen,
                           const SCplx  *h, int hLen, SCplx  *y))

    NSPAPI(void,nspdConv, (const double *x, int xLen,
                           const double *h, int hLen, double *y))

    NSPAPI(void,nspzConv, (const DCplx  *x, int xLen, 
                           const DCplx  *h, int hLen, DCplx  *y))

    NSPAPI(void,nspscConv,(const float  *x, int xLen,
                           const SCplx  *h, int hLen, SCplx  *y))

    NSPAPI(void,nspcsConv,(const SCplx  *x, int xLen, 
                           const float  *h, int hLen, SCplx  *y))

    NSPAPI(void,nspdzConv,(const double *x, int xLen, 
                           const DCplx  *h, int hLen, DCplx  *y))

    NSPAPI(void,nspzdConv,(const DCplx  *x, int xLen, 
                           const double *h, int hLen, DCplx  *y))

    NSPAPI(void,nspwConv, (const short  *x, int xLen, 
                           const short  *h, int hLen, short  *y,
                           int     scaleMode,   int    *scaleFactor))

#endif                                  /* _NSPCONV_H                      */
#ifdef __cplusplus
}
#endif
