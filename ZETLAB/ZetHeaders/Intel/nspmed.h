/*M*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 1995 Intel Corporation. All Rights Reserved.
//
//      $Workfile: nspmed.h $
//      $Revision: 3 $
//      $Modtime: 7/10/99 4:35p $
//
//  Purpose: NSP Median Filter
*M*/
#ifdef __cplusplus
extern "C" {
#endif

#if !defined (_NSPMED_H) || defined (_OWN_BLDPCS)
   #define _NSPMED_H

/* ------------------------------------------------------------------------*/

   NSPAPI(void,nspsbMedianFilter1,( float  *inout, int len, int masksize))
   NSPAPI(void,nspdbMedianFilter1,( double *inout, int len, int masksize))
   NSPAPI(void,nspwbMedianFilter1,( short  *inout, int len, int masksize))

   NSPAPI(void,nspsbMedianFilter2,( const float  *in, float  *out, int len,
                                    int masksize))
   NSPAPI(void,nspdbMedianFilter2,( const double *in, double *out, int len,
                                    int masksize))
   NSPAPI(void,nspwbMedianFilter2,( const short  *in, short  *out, int len,
                                    int masksize))

#endif                                          /* _NSPMED_H               */
#ifdef __cplusplus
}
#endif
