/*M*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 1995 Intel Corporation. All Rights Reserved.
//
//      $Workfile: nspwin.h $
//      $Revision: 3 $
//      $Modtime: 27/12/99 4:37p $
//
//  Purpose: NSP Windowing Functions. Declaration
//                                                                 
//     
*M*/
#ifdef __cplusplus
extern "C" {
#endif
#if !defined (_NSPWIN_H)  || defined (_OWN_BLDPCS)
   #define _NSPWIN_H
 /* ------------------------------------------------------------------------*/
/*                                                                         */
/*   Win                                                                   */
/*                                                                         */
/* Multiply a vector by a windowing function.                              */
/*                                                                         */

   NSPAPI(void,nspsWinBartlett, (float  *vec, int N))
   NSPAPI(void,nspcWinBartlett, (SCplx  *vec, int N))
   NSPAPI(void,nspdWinBartlett, (double *vec, int N))
   NSPAPI(void,nspzWinBartlett, (DCplx  *vec, int N))
   NSPAPI(void,nspwWinBartlett, (short  *vec, int N))
   NSPAPI(void,nspvWinBartlett, (WCplx  *vec, int N))

   NSPAPI(void,nspsWinBartlett2, (const float  *src,float  *dst, int N))
   NSPAPI(void,nspcWinBartlett2, (const SCplx  *src,SCplx  *dst, int N))
   NSPAPI(void,nspdWinBartlett2, (const double *src,double *dst, int N))
   NSPAPI(void,nspzWinBartlett2, (const DCplx  *src,DCplx  *dst, int N))
   NSPAPI(void,nspwWinBartlett2, (const short  *src,short  *dst, int N))
   NSPAPI(void,nspvWinBartlett2, (const WCplx  *src,WCplx  *dst, int N))

   NSPAPI(void,nspsWinHann, (float  *vec, int N))
   NSPAPI(void,nspcWinHann, (SCplx  *vec, int N))
   NSPAPI(void,nspdWinHann, (double *vec, int N))
   NSPAPI(void,nspzWinHann, (DCplx  *vec, int N))
   NSPAPI(void,nspwWinHann, (short  *vec, int N))
   NSPAPI(void,nspvWinHann, (WCplx  *vec, int N))

   NSPAPI(void,nspsWinHann2, (const float  *src,float  *dst, int N))
   NSPAPI(void,nspcWinHann2, (const SCplx  *src,SCplx  *dst, int N))
   NSPAPI(void,nspdWinHann2, (const double *src,double *dst, int N))
   NSPAPI(void,nspzWinHann2, (const DCplx  *src,DCplx  *dst, int N))
   NSPAPI(void,nspwWinHann2, (const short  *src,short  *dst, int N))
   NSPAPI(void,nspvWinHann2, (const WCplx  *src,WCplx  *dst, int N))

   NSPAPI(void,nspsWinHamming, (float  *vec, int N))
   NSPAPI(void,nspcWinHamming, (SCplx  *vec, int N))
   NSPAPI(void,nspdWinHamming, (double *vec, int N))
   NSPAPI(void,nspzWinHamming, (DCplx  *vec, int N))
   NSPAPI(void,nspwWinHamming, (short  *vec, int N))
   NSPAPI(void,nspvWinHamming, (WCplx  *vec, int N))

   NSPAPI(void,nspsWinHamming2, (const float  *src,float  *dst, int N))
   NSPAPI(void,nspcWinHamming2, (const SCplx  *src,SCplx  *dst, int N))
   NSPAPI(void,nspdWinHamming2, (const double *src,double *dst, int N))
   NSPAPI(void,nspzWinHamming2, (const DCplx  *src,DCplx  *dst, int N))
   NSPAPI(void,nspwWinHamming2, (const short  *src,short  *dst, int N))
   NSPAPI(void,nspvWinHamming2, (const WCplx  *src,WCplx  *dst, int N))

   NSPAPI(void,nspsWinBlackman, (float  *vec, int N, float  alpha))
   NSPAPI(void,nspcWinBlackman, (SCplx  *vec, int N, float  alpha))
   NSPAPI(void,nspdWinBlackman, (double *vec, int N, double alpha))
   NSPAPI(void,nspzWinBlackman, (DCplx  *vec, int N, double alpha))
   NSPAPI(void,nspwWinBlackman, (short  *vec, int N, float  alpha))
   NSPAPI(void,nspvWinBlackman, (WCplx  *vec, int N, float  alpha))

   NSPAPI(void,nspsWinBlackman2, (const float  *src, float  *dst, int N, float  alpha))
   NSPAPI(void,nspcWinBlackman2, (const SCplx  *src, SCplx  *dst, int N, float  alpha))
   NSPAPI(void,nspdWinBlackman2, (const double *src, double *dst, int N, double alpha))
   NSPAPI(void,nspzWinBlackman2, (const DCplx  *src, DCplx  *dst, int N, double alpha))
   NSPAPI(void,nspwWinBlackman2, (const short  *src, short  *dst, int N, float  alpha))
   NSPAPI(void,nspvWinBlackman2, (const WCplx  *src, WCplx  *dst, int N, float  alpha))

   NSPAPI(void,nspsWinBlackmanStd, (float  *vec, int N))
   NSPAPI(void,nspcWinBlackmanStd, (SCplx  *vec, int N))
   NSPAPI(void,nspdWinBlackmanStd, (double *vec, int N))
   NSPAPI(void,nspzWinBlackmanStd, (DCplx  *vec, int N))
   NSPAPI(void,nspwWinBlackmanStd, (short  *vec, int N))
   NSPAPI(void,nspvWinBlackmanStd, (WCplx  *vec, int N))

   NSPAPI(void,nspsWinBlackmanStd2, (const float  *src, float  *dst, int N))
   NSPAPI(void,nspcWinBlackmanStd2, (const SCplx  *src, SCplx  *dst, int N))
   NSPAPI(void,nspdWinBlackmanStd2, (const double *src, double *dst, int N))
   NSPAPI(void,nspzWinBlackmanStd2, (const DCplx  *src, DCplx  *dst, int N))
   NSPAPI(void,nspwWinBlackmanStd2, (const short  *src, short  *dst, int N))
   NSPAPI(void,nspvWinBlackmanStd2, (const WCplx  *src, WCplx  *dst, int N))

   NSPAPI(void,nspsWinBlackmanOpt, (float  *vec, int N))
   NSPAPI(void,nspcWinBlackmanOpt, (SCplx  *vec, int N))
   NSPAPI(void,nspdWinBlackmanOpt, (double *vec, int N))
   NSPAPI(void,nspzWinBlackmanOpt, (DCplx  *vec, int N))
   NSPAPI(void,nspwWinBlackmanOpt, (short  *vec, int N))
   NSPAPI(void,nspvWinBlackmanOpt, (WCplx  *vec, int N))

   NSPAPI(void,nspsWinBlackmanOpt2, (const float  *src, float  *dst, int N))
   NSPAPI(void,nspcWinBlackmanOpt2, (const SCplx  *src, SCplx  *dst, int N))
   NSPAPI(void,nspdWinBlackmanOpt2, (const double *src, double *dst, int N))
   NSPAPI(void,nspzWinBlackmanOpt2, (const DCplx  *src, DCplx  *dst, int N))
   NSPAPI(void,nspwWinBlackmanOpt2, (const short  *src, short  *dst, int N))
   NSPAPI(void,nspvWinBlackmanOpt2, (const WCplx  *src, WCplx  *dst, int N))

   NSPAPI(void,nspsWinKaiser, (float  *vec, int N, float  beta))
   NSPAPI(void,nspcWinKaiser, (SCplx  *vec, int N, float  beta))
   NSPAPI(void,nspdWinKaiser, (double *vec, int N, double beta))
   NSPAPI(void,nspzWinKaiser, (DCplx  *vec, int N, double beta))
   NSPAPI(void,nspwWinKaiser, (short  *vec, int N, float  beta))
   NSPAPI(void,nspvWinKaiser, (WCplx  *vec, int N, float  beta))

   NSPAPI(void,nspsWinKaiser2, (const float  *src, float  *dst, int N, float  beta))
   NSPAPI(void,nspcWinKaiser2, (const SCplx  *src, SCplx  *dst, int N, float  beta))
   NSPAPI(void,nspdWinKaiser2, (const double *src, double *dst, int N, double beta))
   NSPAPI(void,nspzWinKaiser2, (const DCplx  *src, DCplx  *dst, int N, double beta))
   NSPAPI(void,nspwWinKaiser2, (const short  *src, short  *dst, int N, float  beta))
   NSPAPI(void,nspvWinKaiser2, (const WCplx  *src, WCplx  *dst, int N, float  beta))

#endif                                  /* _NSPWIN_H                       */
#ifdef __cplusplus
}
#endif
