#ifndef TFSI_H
#define TFSI_H
#include "function.h"

using namespace std;

template <class number>
class TFSi : public Function<number> {
    static Array<number> calculateDerivatives(unsigned int degree) {
        Array<number> derivatives(degree+2, 0);
        for (unsigned int i = 0; i <= degree; ++i) {
            if (i % 4 == 3 || i % 4 == 1)
                derivatives.setElement(i, 0);
            else if (i % 4 == 2){
                number val = -1.0 / static_cast<double>(i + 1);
                derivatives.setElement(i, val);
            }
            else if (i % 4 == 0)
                derivatives.setElement(i, 1.0/(i+1));
        }
        return derivatives;
    }

public:
    TFSi(unsigned int degree)
        : Function<number>(degree, calculateDerivatives(degree)) {}
};

#endif // TFSI_H
