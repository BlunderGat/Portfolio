#include "complex.h"
#include <cmath>
#include <iostream>

complex::complex() : re(0.0), im(0.0) {}

complex::complex(double r, double i) : re(r), im(i) {}

complex::complex(int i) : re(0.0), im(0.0 + i) {}

complex complex::operator + (const complex & c)
{
    return complex(re + c.re, im + c.im);
}

complex complex::operator - (const complex & c)
{
    return complex(re - c.re, im - c.im);
}

complex complex::operator * (const complex & c)
{
    return complex(re * c.re - im * c.im, re * c.im + im * c.re);
}

complex complex::operator / (const complex & c)
{
    double denominator = c.re * c.re + c.im * c.im;
    return complex((re * c.re + im * c.im) / denominator,
                    (im * c.re - re * c.im) / denominator);
}

complex & complex::operator = (const complex& c)
{
    re = c.re;
    im = c.im;
    return *this;
}

bool complex::operator == (const complex & c)
{
    return (re == c.re && im == c.im);
}
bool complex::operator != (const complex & c)
{
    return (re != c.re || im != c.im);
}

bool complex::operator > (const complex & c){
    double r1 = sqrt(re * re + im * im);
    double r2 = sqrt(c.re * c.re + c.im * c.im);

    if (r1 > r2) {
        return true;
    } else if (r1 == r2) {
        double phi1 = atan2(im, re);
        double phi2 = atan2(c.im, c.re);
        return phi1 > phi2;
    }
    return false;
}

bool complex::operator < (const complex & c) {
    return !(*this > c) && *this != c;
}

istream& operator >> (istream & in, complex & c)
{
    in >> c.re >> c.im;
    return in;
}

ostream& operator << (ostream & os, const complex & c)
{
    if (c.im < 0) {
        os << c.re << c.im << "i";
    } else {
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
