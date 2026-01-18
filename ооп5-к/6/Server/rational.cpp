#include "rational.h"

TRational::TRational(): num(0) {}

TRational::TRational(double n)
{
    num = n;
}


TRational TRational::operator+(const TRational& x)
{
    return this->num + x.num;
}

TRational TRational::operator-(const TRational& x)
{
    return this->num - x.num;
}

TRational TRational::operator/(const TRational& x)
{
    return this->num / x.num;
}

TRational TRational::operator*(const TRational& x)
{
    return this->num * x.num;
}

TRational& TRational::operator= (const TRational& x)
{
    num = x.num;
    return *this;
};

bool TRational::operator== (const TRational& x)
{
    return (this->num == x.num);
};

bool TRational::operator!= (const TRational& x)
{
    return (this->num != x.num);
};

std::ostream& operator<<(std::ostream& os, const TRational& c)
{
    os << c.num;
    return os;
}
