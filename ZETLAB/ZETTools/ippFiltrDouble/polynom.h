#include <vector>
#include <complex>

using namespace std;
typedef complex<double> Complex;

class Polynom
{
private:
	typedef vector <Complex> Coefficients;
	Coefficients m_Coeffs;

	void setLengthFill(int length);
	Complex scalarProd(const Polynom& poly) const;

public: 
	Polynom(Complex a = (0., 0.));
	Polynom(Complex a, Complex b);
	Polynom(Complex a, Complex b, Complex c);
	Polynom(Complex a, Complex b, Complex c, Complex d);

	virtual ~Polynom();
	
	// Accessors
	void setCoeff(int index, Complex value);
	Complex getCoeff(int index) const;
	Complex& operator [] (int index);

	int getOrder() const;

	// polynom operations
	Polynom operator + (const Polynom& poly) const;
	Polynom operator - (const Polynom& poly) const;
	Polynom operator * (const Polynom& poly) const;
	Polynom operator * (Complex lambda) const;
	Polynom operator / (Complex lambda) const;
	void operator += (const Polynom& poly);
	void operator -= (const Polynom& poly);
	void operator *= (const Polynom& poly);
	void operator *= (Complex lambda);
	void operator /= (Complex lambda);
	Polynom insertPoly(const Polynom& poly) const;
	Polynom derivate() const;
	Polynom integrate() const;
	Polynom pow(int degree) const;
	void inverse();
	Complex	evaluate(Complex t) const;

	virtual CString toString() const;
};