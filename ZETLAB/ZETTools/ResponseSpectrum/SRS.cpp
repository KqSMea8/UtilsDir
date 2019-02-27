//----------------------------------------------------------------------
// SRS, version 0.1 22.07.2002
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
#include "stdafx.h"
#include "Srs.h"
#include <math.h>
#define _USE_MATH_DEFINES
#include <math.h>


/*! \brief default constructor   */
//---------------------------------------------------------------------------

Srs::Srs() : Length_th(1), Length_srs(1)
{
    fs = 1.;
    a1s = 0.; a2s = 0.; b0 = 0.; b1 = 0.; b2 = 0.;
    InputTimeHistory.resize(Length_th);
    InputTimeHistory[0] = 0.;
    RespTimeHistory.resize(Length_th);
    RespTimeHistory[0] = 0.;
    ShockResponseSpectrum.resize(Length_srs);
    ShockResponseSpectrum[0] = 0.;
    FreqVec.resize(Length_srs);
    FreqVec[0] = 0.;
    DampVec.resize(Length_srs);
    DampVec[0] = 0.;
}

/*! \brief constructor   */
//---------------------------------------------------------------------------

Srs::Srs( const double fsa, const int l_th, const double* BaseAcc, const int l_srs, const double* f, const double* xis)
{
    fs = fsa;
    Length_th = l_th;
    InputTimeHistory.resize(Length_th);
    for (int i = 0; i < Length_th; i++)
		InputTimeHistory[i] = BaseAcc[i];
    RespTimeHistory.resize(Length_th);

    Length_srs = l_srs;
    ShockResponseSpectrum.resize(Length_srs);
    FreqVec.resize(Length_srs);
    for (int i = 0; i < Length_srs; i++)
		FreqVec[i] = f[i];
    DampVec.resize(Length_srs);
    for (int i = 0; i < Length_srs; i++) 
		DampVec[i] = xis[i];
}

/*! \brief constructor   */
//---------------------------------------------------------------------------

Srs::Srs( const double fsa, const int l_th, const double* BaseAcc, const int nf, const double f1, const double f2, const double xi)
{
    fs = fsa;
    Length_th = l_th;
    InputTimeHistory.resize(Length_th);
    for (int i = 0; i < Length_th; i++) 
		InputTimeHistory[i] = BaseAcc[i];
    RespTimeHistory.resize(Length_th);

    CompFreqConstLog(nf, f1, f2, xi);
}

//---------------------------------------------------------------------------

/*! \brief Copy-Konstruktor */
Srs::Srs(const Srs& rhs)
{
    using namespace std;

    fs = rhs.fs;
    
    a1s = rhs.a1s;
    a2s = rhs.a2s;
    b0 = rhs.b0;
    b1 = rhs.b1;
    b2 = rhs.b2;
    Length_th = rhs.Length_th;
    InputTimeHistory.resize(Length_th);
    InputTimeHistory = rhs.InputTimeHistory;
    RespTimeHistory.resize(Length_th);
    RespTimeHistory = rhs.RespTimeHistory;
    Length_srs = rhs.Length_srs;
    ShockResponseSpectrum.resize(Length_srs);
    ShockResponseSpectrum = rhs.ShockResponseSpectrum;
    FreqVec.resize(Length_srs);
    FreqVec = rhs.FreqVec;
    DampVec.resize(Length_srs);
    DampVec = rhs.DampVec;
}

//---------------------------------------------------------------------------

/*! \brief assignment */
const Srs& Srs::operator=(const Srs& rhs)
{
    if (this != &rhs)
    {
        fs = rhs.fs;
        
        a1s = rhs.a1s;
        a2s = rhs.a2s;
        b0 = rhs.b0;
        b1 = rhs.b1;
        b2 = rhs.b2;

        Length_th = rhs.Length_th;
        InputTimeHistory.resize(Length_th);
        InputTimeHistory = rhs.InputTimeHistory;;
        RespTimeHistory.resize(Length_th);
        RespTimeHistory = rhs.RespTimeHistory;
        
        Length_srs = rhs.Length_srs;
        ShockResponseSpectrum.resize(Length_srs);
        ShockResponseSpectrum = rhs.ShockResponseSpectrum;
        FreqVec.resize(Length_srs);
        FreqVec = rhs.FreqVec;
        DampVec.resize(Length_srs);
        DampVec = rhs.DampVec;
    };
    return *this;
}

//---------------------------------------------------------------------------

/*! \brief Smallwood filter coefficient of relative displacement model */
void Srs::SmallwoodFilterCoeffAppRDM(const double& fn, const double& xi, const double& frqs)
// fn: frequency of 1-mass-spring--damper system
// xi: ratio of critical damping of 1-mass-spring--damper system
// fs: sampling frequency
{
	const double pi = 3.141592653590; 
	double omegan = 2.*M_PI*fn;
	double Ts = 1. /frqs;

	a1s = 2.*xi*omegan*Ts + pow((omegan*Ts),2.)*(1.-2.*xi*xi);
	a2s = -2.*xi*omegan*Ts + 2.*xi*xi*pow((omegan*Ts),2.);
	b0 = pow((omegan*Ts),2.) / 6.;
	b1 = 2.*pow((omegan*Ts),2.) / 3.;
	b2 = b0;
}

//---------------------------------------------------------------------------

/*! \brief Smallwood filter coefficient of absolute acceleration model */
void Srs::SmallwoodFilterCoeffAppAAM(const double& fn, const double& xi, const double& frqs)
// fn: frequency of 1-mass-spring--damper system
// xi: ratio of critical damping of 1-mass-spring--damper system
// fs: sampling frequency
{
   const double pi = 3.141592653590; 
   double omegan = 2.*M_PI*fn;
   double Ts = 1. / frqs;

   a1s = 2.*xi*omegan*Ts + pow((omegan*Ts),2.)*(1.-2.*xi*xi);
   a2s = -2.*xi*omegan*Ts + 2.*xi*xi*pow((omegan*Ts),2.);
   b0 = xi*omegan*Ts + pow((omegan*Ts),2.) * (1./6. - 2./3.*xi*xi);
   b1 = 2./3.*pow((omegan*Ts),2.) * (1.-xi*xi);
   b2 = -xi*omegan*Ts + pow((omegan*Ts),2.) * (1./6. + 4./3.*xi*xi);
}

//---------------------------------------------------------------------------

/*! \brief Compute time history of response by Smallwood filtering */
void Srs::CompSmallwoodResp(const double& fn, const double& xi, const double& frqs)
// fn: frequency of 1-mass-spring--damper system
// xi: ratio of critical damping of 1-mass-spring--damper system
// fs: sampling frequency
{
	if (fn > frqs / 2.)
	{
		std::cout << "Warning: mass-spring-damper system eigenfrequency is not small" << std::endl;
		std::cout << "  compared to sampling frequency! Response may get wrong!" << std::endl;
		std::cout << "    mass-spring-damper system eigenfrequency: " << fn << std::endl;
		std::cout << "    sampling frequency:                       " << fs << std::endl;
	};
	if (Length_th == 1) 
		RespTimeHistory[0] = b0 * InputTimeHistory[0];
	else if (Length_th == 2) 
	{
		RespTimeHistory[0] = b0*InputTimeHistory[0];
		RespTimeHistory[1] = b0*InputTimeHistory[1] + b1*InputTimeHistory[0] + 2.*RespTimeHistory[0] - a1s*RespTimeHistory[0];
	}
	else if (Length_th > 2) 
	{
		RespTimeHistory[0] = b0*InputTimeHistory[0];
		RespTimeHistory[1] = b0*InputTimeHistory[1] + b1*InputTimeHistory[0] + 2.*RespTimeHistory[0] - a1s*RespTimeHistory[0];
		for (int i = 2; i < Length_th; i++)
			RespTimeHistory[i] = b0*InputTimeHistory[i] + b1*InputTimeHistory[i-1] + b2*InputTimeHistory[i-2] + RespTimeHistory[i-1]
			                   + (RespTimeHistory[i-1] - RespTimeHistory[i-2]) - a1s*RespTimeHistory[i-1] - a2s*RespTimeHistory[i-2];
	}
	else 
	{
		std::cout << "Error in Srs::filter(const double& fn, const double& xi, const double& fs): wrong Length_th!" << std::endl;
		std::cout << "  Length_th = " << Length_th << std::endl;
	};
}

//---------------------------------------------------------------------------

/*! \brief Compute shock response spectrum with Smallwood method (absolute acceleration model). */
void Srs::CompSmallwoodSRSAAM(const int& SrsType)
// SrsType: type of SRS
//   SrsType < 0: Max. of negative values of output acceleration time history
//   SrsType = 0: Max. of all values of output acceleration time history
//   SrsType > 0: Max. of positive values of output acceleration time history
// The SRS is the max. acceleration of the time history of the mass degree
// of freedom a 1-mass-spring-damper system, which is excited by a base
// acceleration.
{
	// compute absolute acceleration response of 1-mass spring damper system at
	// mass degree of freedom (DOF) for given base acceleration
	long k;
	double minval,maxval;

	for (int i = 0; i < Length_srs; i++) 
	{
		double fs = this->fs;
		double fn = this->FreqVec[i];
		double xi = this->DampVec[i];
		this->SmallwoodFilterCoeffAppAAM(fn, xi, fs);
		this->CompSmallwoodResp(fn, xi, fs);

		if(SrsType < 0) 
		{
//			this->ShockResponseSpectrum[i] = fabs(this->RespTimeHistory.min());
			minval = this->RespTimeHistory[0];
			for (k = 0; k < Length_th; k++)
				if (minval > this->RespTimeHistory[k]) 
					minval = this->RespTimeHistory[k];
			if (minval < 0)
				minval = -minval;
			this->ShockResponseSpectrum[i] = minval;
		}
		else 
			if (SrsType == 0)
			{

//				double maxneg = fabs(this->RespTimeHistory.min());
//				double maxpos = fabs(this->RespTimeHistory.max());
//				this->ShockResponseSpectrum[i] = std::max(maxneg, maxpos);
				minval = this->RespTimeHistory[0];
				maxval = this->RespTimeHistory[0];
				for(k = 0; k < Length_th; k++)
				{
					if (minval > this->RespTimeHistory[k])
						minval = this->RespTimeHistory[k];
					if (maxval < this->RespTimeHistory[k]) 
						maxval = this->RespTimeHistory[k];
				}
				if (minval < 0)
					minval = -minval;
				if (maxval < 0)
					maxval = -maxval;
				if (minval < maxval)
					minval = maxval;
				this->ShockResponseSpectrum[i] = minval;
			}
			else 
			{
//				this->ShockResponseSpectrum[i] = fabs(this->RespTimeHistory.max());
				maxval = this->RespTimeHistory[0];
				for(k = 0; k < Length_th; k++)
					if (maxval < this->RespTimeHistory[k])
						maxval = this->RespTimeHistory[k];
				if (maxval < 0)
					maxval = -maxval;
				this->ShockResponseSpectrum[i] = maxval;
			};
	};
}

//---------------------------------------------------------------------------

/*! \brief Create frequency vector with logarithmic weighting. */
void Srs::CompFreqConstLog(const int nf, const double f1, const double f2, const double xi)
// CompFreqConstLog10 return a vector of frequencies between f1 and f2 with
// const distance in log scale.
// nf: no. of frequencies per octave
// xi: ratio of critical damping of 1-mass-spring--damper system
{
//	const double e = M_E;


	Length_srs = int(ceil( log(f2/f1) / log(2.) ) * nf - 1);

	// calculate length of vector
	double flow = f1;
	int i = 0;
	double c = log(2.)/double(nf-1);
	while (flow < f2) 
	{
		i++;
		for (int j = 1; j < nf-1; j++) 
		{
			i++;
		}
		flow = flow * 2.;
	}
	Length_srs = i;

	// compute frequency vector
	ShockResponseSpectrum.resize(Length_srs);
	FreqVec.resize(Length_srs);
	DampVec.resize(Length_srs);

	flow = f1;
	i = 0;
	while (flow < f2) 
	{
		FreqVec[i] = flow;
		DampVec[i] = xi;
		i++;
		for (int j = 1; j < nf-1; j++) 
		{
			FreqVec[i] = FreqVec[i-1]*pow(M_E,c);
			DampVec[i] = xi;
			i++;
		}
		flow = flow * 2.;
	}

}

//-----------------------------------------------------------------------

/*! \brief Clear all memory and vlues*/
void Srs::Clear()
{
	fs = 0;
	a1s = 0;
	a2s = 0;
	b0 = 0;
	b1 = 0;
	b2 = 0;

	Length_th = 0;
	InputTimeHistory.free();
	RespTimeHistory.free();

	Length_srs = 0;
	FreqVec.free();
	DampVec.free();
	ShockResponseSpectrum.free();

}

