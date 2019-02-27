/*M*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 1995 Intel Corporation. All Rights Reserved.
//
//      $Workfile: nspcnv2d.h $
//      $Revision: 3 $
//      $Modtime: Dec 16 1996 4:22p $
//
//  Purpose: NSP 2D Filtering Functions
*M*/
#ifdef __cplusplus
extern "C" {
#endif
#if !defined (_NSPCONV2D_H) || defined (_OWN_BLDPCS)
# define _NSPCONV2D_H
/*
 * Conv2D
 *
 * Perform finite, linear convolution of two two-dimensional signals.
*/

NSPAPI(void,nspsConv2D,(const float  *x, int xCols, int  xRows,
                        const float  *h, int hCols, int  hRows, float  *y))
NSPAPI(void,nspdConv2D,(const double *x, int xCols, int  xRows,
                        const double *h, int hCols, int  hRows, double *y))

NSPAPI(void,nspwConv2D,(const short  *x, int xCols, int  xRows,
                        const short  *h, int hCols, int  hRows, short  *y,
                              int     scaleMode,    int *scaleFactorPtr))
#endif
#ifdef __cplusplus
}
#endif
