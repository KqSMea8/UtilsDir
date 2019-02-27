//----------------------------------------------------------------------
// SRS, version 1.0 25.12.2009
// filter weights of absolute acceleration and relative displacement model
//----------------------------------------------------------------------
//
// Copyright (C) 2002 by FreeDynamics
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.  This program is distributed in
// the hope that it will be useful, but WITHOUT ANY WARRANTY; without
// even the implied warranty of MERCHANTABILITY or FITNESS FOR A
// PARTICULAR PURPOSE.  See the GNU General Public License for more
// details; it is available at <http://www.fsf.org/copyleft/gpl.html>,
// or by writing to the Free Software Foundation, Inc., 59 Temple Place
// - Suite 330, Boston, MA 02111-1307, USA.

/*
Theory taken from
  AN IMPROVED RECURSIVE FORMULA FOR CALCULATING SHOCK RESPONSE SPECTRA
  by David O. Smallwood, Sandia National Labs.
  Originally published in the Shock and Vibration Bulletin, No. 51, May 1981.
*/
#ifndef SrsH
#define SrsH

#include<valarray>
#include<iostream>
#define _USE_MATH_DEFINES
#include <math.h>



//---------------------------------------------------------------------------

/*! \brief Class Shock Response Spectrum */

class Srs {
private:
protected:
public:
    double fs;   // sampling frequency
    // Smallwood filter coefficients
    double a1s;  // Smallwood filter coefficicient
    double a2s;  // Smallwood filter coefficicient
    double b0;   // Smallwood filter coefficicient
    double b1;   // Smallwood filter coefficicient
    double b2;   // Smallwood filter coefficicient

    Srs();	  /*!< default constructor. */
    Srs(const Srs& rhs); /*!< Copy-construktor. */
	Srs( const double fsa, const int l_th, const double* BaseAcc, const int l_srs, const double* f, const double* xis); /*!< constructor. */
    Srs( const double fsa, const int l_th, const double* BaseAcc, const int nf, const double f1, const double f2, const double xi); /*!< constructor. */

    const Srs& operator=(const Srs& rhs);	/*!< assignment. */

    int Length_th; /*!<  length of time history vector. */
    std::valarray<double> InputTimeHistory; /*!< input acceleration time history. */
    std::valarray<double> RespTimeHistory; /*!< output acceleration time history. */

    int Length_srs; /*!<  length of shock response spectrum vector. */
    std::valarray<double> FreqVec; /*!vector of shock response frequencies. */
    std::valarray<double> DampVec; /*!vector of damping ratios. */
    std::valarray<double> ShockResponseSpectrum; /*!vector of shock response spectrum. */

	void SmallwoodFilterCoeffAppRDM(const double& fn, const double& xi, const double& frqs);
	void SmallwoodFilterCoeffAppAAM(const double& fn, const double& xi, const double& frqs);
	void CompSmallwoodResp(const double& fn, const double& xi, const double& frqs);
    void CompSmallwoodSRSAAM(const int& SrsType);
    void CompFreqConstLog(const int nf, const double f1, const double f2, const double xi);

	void Clear();
};

//---------------------------------------------------------------------------
#endif
