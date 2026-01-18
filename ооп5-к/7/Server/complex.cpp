#include "complex.h"
#include <cmath>
#include <iostream>

complex::complex() : re(0.0), im(0.0) {}

complex::complex(double r, double i) : re(r), im(i) {}

complex::complex(int i) : re(0.0), im(0.0 + i) {}

complex complex::operator + (const complex& c)
{
    return complex(re + c.re, im + c.im);
}

complex complex::operator - (const complex& c)
{
    return complex(re - c.re, im - c.im);
}

complex complex::operator * (const complex& c)
{
    return complex(re * c.re - im * c.im, re * c.im + im * c.re);
}

complex complex::operator / (const complex& c)
{
    double denominator = c.re * c.re + c.im * c.im;
    return complex((re * c.re + im * c.im) / denominator,
        (im * c.re - re * c.im) / denominator);
}

complex& complex::operator = (const complex& c)
{
    re = c.re;
    im = c.im;
    return *this;
}

double complex::getRe() const{
    return this->re;
}


double complex::getIm() const{
    return this->im;
}

double complex::module(complex *complex) {
    double a = std::pow(complex->getRe(),2);
    double b = std::pow(complex->getIm(), 2);
    return std::pow(a + b, 0.5);
}

complex pow(complex complex, double n){
    double argZ;
    if(complex.re > 0) argZ = atan(complex.im / complex.re);
    else if(complex.re < 0 && complex.im >= 0) argZ = 3.14 - atan(complex.im / complex.re);
    else if(complex.re < 0 && complex.im < 0) argZ = -3.14 + atan(complex.im / complex.re);
    else if(complex.re == 0 && complex.im > 0) argZ = 3.14/2;
    else argZ = -3.14/2;
    double re = (std::pow(complex.module(&complex), n) * cos(argZ * n));
    double im = (std::pow(complex.module(&complex), n) * sin(argZ * n));
    complex.re = re;
    complex.im = im;
    return complex;
}

bool complex::operator == (const complex& c)
{
    return (re == c.re && im == c.im);
}
bool complex::operator != (const complex& c)
{
    return (re != c.re || im != c.im);
}

bool complex::operator > (const complex& c) {
    double r1 = sqrt(re * re + im * im);
    double r2 = sqrt(c.re * c.re + c.im * c.im);

    if (r1 > r2) {
        return true;
    }
    else if (r1 == r2) {
        double phi1 = atan2(im, re);
        double phi2 = atan2(c.im, c.re);
        return phi1 > phi2;
    }
    return false;
}

bool complex::operator < (const complex& c) {
    return !(*this > c) && *this != c;
}

istream& operator >> (istream& in, complex& c)
{
    in >> c.re >> c.im;
    return in;
}

ostream& operator << (ostream& os, const complex& c)
{
    if (c.im < 0) {
        os << c.re << c.im << "i";
    }
    else {
        os << c.re << "+" << c.im << "i";
    }

    return os;
}

complex sqrt(const complex& c) {
    double modulus = std::sqrt(c.re * c.re + c.im * c.im);
    double angle = std::atan2(c.im, c.re) / 2;

    double sqrt_re = modulus * std::cos(angle);
    double sqrt_im = modulus * std::sin(angle);

    return complex(sqrt_re, sqrt_im);
}
