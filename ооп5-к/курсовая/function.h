#ifndef TFUNCTION_H
#define TFUNCTION_H
#include "polynom.h"
#include "array.h"

template <class number>
class Function : protected Polynom<number> {
public:
    Function(unsigned int degree, Array<number> derivatives) {
        Array<number> coefficients(degree+1,0);
        long long factorial = 1;
        for (unsigned int i = 0; i <= degree; ++i) {
            factorial = factorial * (i+1);
            coefficients.setElement(i, number(derivatives.getElement(i) / factorial));
        }

        this->coefficients = coefficients;
    }

    number operator()(number x) {
        return this->evaluateAt(x);
    }
};

#endif // TFUNCTION_H
