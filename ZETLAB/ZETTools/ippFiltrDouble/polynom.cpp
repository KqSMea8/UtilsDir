#include "stdafx.h"
#include "math.h"
#include "polynom.h"

#define ZERO Complex(0., 0.)

Polynom::Polynom(Complex a)
{
	m_Coeffs.push_back(a);
}

Polynom::Polynom(Complex a, Complex b)
{
	m_Coeffs.push_back(b);
	m_Coeffs.push_back(a);
}

Polynom::Polynom(Complex a, Complex b, Complex c)
{
	m_Coeffs.push_back(c);
	m_Coeffs.push_back(b);
	m_Coeffs.push_back(a);
}

Polynom::Polynom(Complex a, Complex b, Complex c, Complex d)
{
	m_Coeffs.push_back(d);
	m_Coeffs.push_back(c);
	m_Coeffs.push_back(b);
	m_Coeffs.push_back(a);
}

Polynom::~Polynom()
{
}

int Polynom::getOrder() const
{
	int order = int(m_Coeffs.size()) - 1;
	Coefficients::const_reverse_iterator it = m_Coeffs.rbegin();
	for (; (it != m_Coeffs.rend()) && (*it == ZERO); ++it)
		order--;
	return order;
}

complex<double> Polynom::scalarProd(const Polynom& poly) const
{
	Complex tmp(ZERO);
	int maxorder = getOrder() + 1;
	if (maxorder < poly.getOrder() + 1)
		maxorder = poly.getOrder() + 1;
	for (int i = 0; i < maxorder; ++i)
		tmp += poly.getCoeff(i) * getCoeff(i);
	return tmp;
}

CString Polynom::toString() const
{
	CString tmp(L"Polynom { ");
	CString temp;

	int order = getOrder();
	for (int i = m_Coeffs.size() - 1; i >= 0; --i)
	{
		if (m_Coeffs[i] != ZERO || (order == 0))
		{
			if (m_Coeffs[i].real() == 0.)
			{
				if (m_Coeffs[i].imag() > 0)
					temp.Format(L"+%fj", m_Coeffs[i].imag());
				else
					temp.Format(L"%fj", m_Coeffs[i].imag());
			}
			else
			if (m_Coeffs[i].imag() == 0.)
			{
				if (m_Coeffs[i].real() > 0)
					temp.Format(L"+%f", m_Coeffs[i].real());
				else
					temp.Format(L"%f", m_Coeffs[i].real());
			}
			else
			{
				if (m_Coeffs[i].imag() > 0)
					temp.Format(L"+(%f + %fj)", m_Coeffs[i].real(), m_Coeffs[i].imag());
				else
					temp.Format(L"+(%f - %fj)", m_Coeffs[i].real(), abs(m_Coeffs[i].imag()));
			}
			tmp.Append(temp);
			tmp.Append(L"x");
			tmp.Append(L"²");
			temp.Format(L"%d", order);
			tmp.Append(temp);
		}
		order --;
	}
	tmp += " }";
	return tmp;
}

void Polynom::setLengthFill(int length)
{
	if (length >= int(m_Coeffs.size()))
		for (int num = length - int(m_Coeffs.size()) + 1; num > 0; --num)
			m_Coeffs.push_back(ZERO);
}

void Polynom::setCoeff(int index, Complex value) 
{
	setLengthFill(index);
	if (index >= int(m_Coeffs.size()))
		return;
	m_Coeffs[index] = value;
}

Complex Polynom::getCoeff(int index) const
{
	if ((index >= int(m_Coeffs.size())) || (index < 0))
		return ZERO;
	else
		return m_Coeffs[index];
}

Complex& Polynom::operator [] (int index)
{
	setLengthFill(index);
	if (index >= int(m_Coeffs.size()))
		return m_Coeffs[0];

	return m_Coeffs[index];
}

Polynom Polynom::operator + (const Polynom& poly) const
{
	Polynom p(poly);
	int maxorder = getOrder();
	if (maxorder < p.getOrder())
		maxorder = p.getOrder();
	for (int i = 0; i <= maxorder; ++i)
		p[i] += getCoeff(i);
	return p;
}

Polynom Polynom::operator - (const Polynom& poly) const
{
	Polynom p(*this);
	int maxorder = getOrder();
	if (maxorder < p.getOrder())
		maxorder = p.getOrder();
	for (int i = 0; i <= maxorder; ++i)
		p[i] -= poly.getCoeff(i);
	return p;
}

Polynom Polynom::operator * (Complex lambda) const
{
	Polynom p(*this);
	for (int i = p.getOrder(); i >= 0; --i)
		p[i] *= lambda;
	return p;
}

Polynom Polynom::operator / (Complex lambda) const
{
	if (lambda == ZERO)
		return ZERO;
	
	Polynom p(*this);
	for (int i = p.getOrder(); i >= 0; --i)
		p[i] /= lambda;
	return p;
}

Polynom Polynom::operator * (const Polynom& poly) const
{
	Polynom p;
	int order_sum = poly.getOrder() + getOrder() + 1;
	for (int i = 0; i < order_sum; ++i)
		for (int j = 0; j <= i; ++j)
			p[i] += poly.getCoeff(i - j) * getCoeff(j);
	return p;
}

Polynom Polynom::pow(int degree) const
{
	if (degree < 0)
		return ZERO;

	Polynom p (Complex(1., 0.));
	for (int i = 0; i < degree; ++i)
		p = p * (*this);
	return p;
}

void Polynom::operator *= (const Polynom& poly)
{
	*this = *this * poly;
}

void Polynom::operator *= (Complex lambda)
{
	*this = *this * lambda;
}

void Polynom::operator /= (Complex lambda)
{
	*this = *this / lambda;
}

void Polynom::operator += (const Polynom& poly)
{
	*this = *this + poly;
}

void Polynom::operator -= (const Polynom& poly)
{
	*this = *this - poly;
}

Polynom Polynom::insertPoly(const Polynom& poly) const
{
	Polynom p;
	for (int i = getOrder(); i >= 0; --i)
		p += (poly.pow(i) * getCoeff(i));
	return p;
}

Polynom Polynom::derivate() const
{
	Polynom p;
	for (int i = getOrder() - 1; i >= 0; --i)
		p[i] = getCoeff(i + 1) * Complex(double(i + 1), 0.);
	return p;
}

Polynom Polynom::integrate() const
{
	Polynom p;
	for (int i = getOrder() + 1; i >= 1; --i)
		p[i] = getCoeff(i - 1) / Complex(double(i), 0.);
	return p;
}

Complex Polynom::evaluate(Complex t) const
{
	Complex total(ZERO);
	for (int i = getOrder(); i >= 0; --i)
	{
		Complex temp(1., 0.);
		for (int j = 0; j < i; j++)
			temp *= t;
		total += temp * getCoeff(i);
	}
	return total;
}

void Polynom::inverse()
{
	Coefficients coeffs_;
	for (int i = getOrder(); i >= 0; --i)
		coeffs_.push_back(getCoeff(i));
	for (int i = 0; i <= getOrder(); ++i)
		m_Coeffs[i] = coeffs_[i];
}
