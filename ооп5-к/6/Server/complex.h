#ifndef COMPLEX_H
#define COMPLEX_H

#include<iostream>
#include <cmath>

using namespace std;

class complex {
public:
    complex();
    complex(double r, double i);
    complex(int i);

    double re;
    double im;

    complex operator+ (const complex& c);
    complex operator- (const complex& c);
    complex operator* (const complex& c);
    complex operator/ (const complex& c);
    complex& operator= (const complex& c);

    bool operator== (const complex& c);
    bool operator!= (const complex& c);
    bool operator> (const complex& c);
    bool operator< (const complex& c);

    friend complex sqrt(const complex& c);
    friend istream& operator >> (istream& in, complex& c);
    friend ostream& operator << (ostream& os, const complex& c);

protected:

private:
};

#endif // COMPLEX_H
