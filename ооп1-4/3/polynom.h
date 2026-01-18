#ifndef POLYNOM_H
#define POLYNOM_H

#include "array.h"
#include "number.h"

class MYpolynom {
private:
    number a_n;
    Array roots_array;
    unsigned stepen;
    Array coefficients_array;

public:
    MYpolynom();
    MYpolynom(number a_n, unsigned stepen, const Array& input_roots);
    void Count_coeff();
    number getCoefficient(unsigned pos) const;
    void enterPolynom(unsigned stepen, number a_n, const Array& input_roots);
    void changeA_n(number new_a_n);
    void changeX(unsigned index, number new_x);
    number findValue(number x) const;
    unsigned getStepen() const;

    friend std::ostream& operator<<(std::ostream& stream_out, const MYpolynom& pol);
};

#endif // POLYNOM_H
