#ifndef RATIONAL_H
#define RATIONAL_H
#include <iostream>

using namespace std;

class TRational
{
public:
    TRational();
    TRational(double i);

    double num;

    TRational operator+ (const TRational& x);
    TRational operator- (const TRational& x);
    TRational operator/ (const TRational& x);
    TRational operator* (const TRational& x);

    TRational& operator= (const TRational& x);
    bool operator== (const TRational& x);
    bool operator!= (const TRational& x);

    friend ostream& operator<<(ostream& os, const TRational& c);
};

#endif // RATIONAL_H
