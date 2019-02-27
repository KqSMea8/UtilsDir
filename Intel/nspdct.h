/*M*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 1996 Intel Corporation. All Rights Reserved.
//
//      $Workfile: nspdct.h $
//      $Revision: 3 $
//      $Modtime: 08/12/96 4:22p $
//
//    Purpose:
*M*/
#ifdef __cplusplus
extern "C" {
#endif
#if !defined (_NSPDCT_H_) || defined (_OWN_BLDPCS)

   #define _NSPDCT_H_

   #if !defined _OWN_BLDPCS

      #define  NSP_DCT_Forward      1
      #define  NSP_DCT_Inverse      2
      #define  NSP_DCT_Free         8

   #endif

   NSPAPI(void,nspsDct,(const float* src, float* dst, int len, int flags))
   NSPAPI(void,nspdDct,(const double* src, double* dst, int len, int flags))
   NSPAPI(void,nspwDct,(const short* src, short* dst, int len, int flags))

#endif
#ifdef __cplusplus
}
#endif
