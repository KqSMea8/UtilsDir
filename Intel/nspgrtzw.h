/*M*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 1995 Intel Corporation. All Rights Reserved.
//
//      $Workfile: nspgrtzw.h $
//      $Revision: 3 $
//      $Modtime: Dec 16 1996 18:31:26  $
//
//  Purpose: NSP Single Frequency DFT (Goertzel)
*M*/
#ifdef __cplusplus
extern "C" {
#endif
#if !defined (_NSPGRTZW_H) || defined (_OWN_BLDPCS)
    #define _NSPGRTZW_H
/* ------------------------------------------------------------------------*/

    #if !defined (_OWN_BLDPCS)

    typedef NSPSGoertzState NSPWGoertzState, NSPVGoertzState;

    #endif

/* ------------------------------------------------------------------------*/
/*        GoertzInit                                                       */
/*                                                                         */
/* Initializes the coefficients and zeros the delay line.                  */

    NSPAPI(void,nspwGoertzInit,(float  freq, NSPWGoertzState *stPtr))
    NSPAPI(void,nspvGoertzInit,(float  freq, NSPVGoertzState *stPtr))

/* ------------------------------------------------------------------------*/
/*        GoertzReset                                                      */
/*                                                                         */
/* Zeros the delay line.                                                   */

    NSPAPI(void,nspwGoertzReset,(NSPWGoertzState *stPtr))
    NSPAPI(void,nspvGoertzReset,(NSPVGoertzState *stPtr))

/* ------------------------------------------------------------------------*/
/*        Goertz                                                           */
/*                                                                         */
/* Single Frequency DFT (Goertzel algorithm)                               */

    NSPAPI(WCplx,nspwGoertz,(NSPWGoertzState *stPtr, short  sample, 
                             int  scaleMode,  int *scaleFactor))
    NSPAPI(WCplx,nspvGoertz,(NSPVGoertzState *stPtr, WCplx  sample, 
                             int  scaleMode,  int *scaleFactor))

    NSPAPI(WCplx,nspwbGoertz,(NSPWGoertzState *stPtr,const short  *inSamps,
                              int len, int  scaleMode,  int *scaleFactor))
    NSPAPI(WCplx,nspvbGoertz,(NSPVGoertzState *stPtr,const WCplx  *inSamps,
                              int len, int  scaleMode,  int *scaleFactor))

#endif                                 /* _NSPGRTZW_H                      */
#ifdef __cplusplus
}
#endif
