#pragma once
#include <complex>
#include <vector>
//#include "stdafx.h"
//#include "EllipticFunctions.h"
//#include "math.h"

double EllipticIntegral(double k)
{
	double dPrecision(1e-9);
//	double dPrecision(pow(10., -9.));
	double ki(k);
	double K(M_PI_2);
	while (ki > dPrecision)
	{
		ki = pow(ki / (1. + sqrt(1 - pow(ki, 2))), 2);
		K *= (1. + ki);
	}
	return K;
}

double EllipticSinus( double u, double k )
{
	double dPrecision(1e-9);
//	double dPrecision(pow(10., -9.));
	double ki(k);
	std::vector <double> vki;
	while (ki > dPrecision)
	{
		ki = pow(ki / (1. + sqrt(1 - pow(ki, 2))), 2);
		vki.push_back(ki);
	}
	double wm = sin (u * M_PI_2);
	for (int i = vki.size() - 1; i >= 0; --i)
	{
		wm = (vki[i] + 1.) / (1. / wm + vki[i] * wm);
	}
	return wm;
}

std::complex<double> EllipticSinus( std::complex<double> u, double k )
{
	double dPrecision(1e-9);
//	double dPrecision(pow(10., -9.));
	double ki(k);
	std::vector <double> vki;
	while (ki > dPrecision)
	{
		ki = pow(ki / (1. + sqrt(1 - pow(ki, 2))), 2);
		vki.push_back(ki);
	}
	std::complex<double> wm = sin (u * M_PI_2);
	for (int i = vki.size() - 1; i >= 0; --i)
	{
		wm = (vki[i] + 1.) / (1. / wm + vki[i] * wm);
	}
	return wm;
}

double EllipticCosinus(double u, double k)
{
	double dPrecision(1e-9);
//	double dPrecision(pow(10., -9.));
	double ki(k);
	std::vector <double> vki;
	while (ki > dPrecision)
	{
		ki = pow(ki / (1. + sqrt(1 - pow(ki, 2))), 2);
		vki.push_back(ki);
	}
	double wm = cos (u * M_PI_2);
	for (int i = vki.size() - 1; i >= 0; --i)
	{
		wm = (vki[i] + 1.) / (1. / wm + vki[i] * wm);
	}
	return wm;
}

std::complex<double> EllipticCosinus( std::complex<double> u, double k )
{
	double dPrecision(1e-9);
//	double dPrecision(pow(10., -9.));
	double ki(k);
	std::vector <double> vki;
	while (ki > dPrecision)
	{
		ki = pow(ki / (1. + sqrt(1 - pow(ki, 2))), 2);
		vki.push_back(ki);
	}
	std::complex<double> wm = cos (u * M_PI_2);
	for (int i = vki.size() - 1; i >= 0; --i)
	{
		wm = (vki[i] + 1.) / (1. / wm + vki[i] * wm);
	}
	return wm;
}


std::complex<double> EllipticArcsinus(std::complex<double> u, double k)
{
	double dPrecision(1e-9);
//	double dPrecision(pow(10., -9.));
	double ki(k), ki_1(k);
	std::complex <double> ui(u), j(0., 1.);
	while (ki > dPrecision)
	{
		ki = pow(ki_1 / (1. + sqrt(1 - pow(ki_1, 2))), 2);
		ui = (2. * ui) / ((1. + ki) * (1. + sqrt(1. - pow (ui * ki_1, 2))));
		ki_1 = ki;
	}

	ui = - j * log(j * ui + sqrt(1. - pow(ui, 2))) / M_PI_2;

	return ui;
}