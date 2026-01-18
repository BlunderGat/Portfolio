#include "polynom.h"
#include <iostream>
#include <stdexcept>

MYpolynom::MYpolynom() : a_n(0), stepen(0), roots_array(), coefficients_array() {}

MYpolynom::MYpolynom(number a_n, unsigned stepen, const Array& input_roots) {
    if (a_n == 0) {
        throw std::invalid_argument("a_n cannot be zero");
    }
    this->a_n = a_n;
    if (stepen != input_roots.getSize()) {
        throw std::invalid_argument("Array size does not match degree");
    }
    this->stepen = stepen;

    roots_array.Resize(stepen);
    coefficients_array.Resize(stepen + 1);
    for (int i = 0; i < stepen; i++) {
        roots_array[i] = input_roots[i];
    }
    Count_coeff();
}

void MYpolynom::Count_coeff() {
    coefficients_array[0] = 1;
    for (unsigned j = 1; j <= stepen; j++) {
        coefficients_array[j] = 0;
    }
    for (unsigned j = 0; j < stepen; j++) {
        for (int k = j; k >= 0; k--) {
            coefficients_array[k + 1] -= roots_array[j] * coefficients_array[k];
        }
    }
    for (unsigned i = 0; i <= stepen; i++) {
        coefficients_array[i] *= a_n;
    }
}

number MYpolynom::getCoefficient(unsigned pos) const {
    if (pos > stepen) {
        throw std::out_of_range("Index exceeds polynomial degree");
    }
    return coefficients_array[stepen - pos];
}

void MYpolynom::enterPolynom(unsigned new_stepen, number new_a_n, const Array& input_roots) {
    if (new_a_n == 0) {
        throw std::invalid_argument("a_n cannot be zero");
    }
    this->a_n = new_a_n;
    if (new_stepen != input_roots.getSize()) {
        throw std::invalid_argument("Array size does not match degree");
    }
    stepen = new_stepen;

    roots_array.Resize(stepen);
    coefficients_array.Resize(stepen + 1);
    for (int i = 0; i < stepen; i++) {
        roots_array[i] = input_roots[i];
    }
    Count_coeff();
}

void MYpolynom::changeA_n(number new_a_n) {
    a_n = new_a_n;
    Count_coeff();
}

void MYpolynom::changeX(unsigned index, number new_x) {
    if (index >= stepen) {
        throw std::out_of_range("Invalid index");
    }
    roots_array[index] = new_x;
    Count_coeff();
}

number MYpolynom::findValue(number x) const {
    number val = 1;
    for (unsigned i = 0; i < stepen; i++) {
        val *= (x - roots_array[i]);
    }
    return val * a_n;
}

std::ostream& operator<<(std::ostream& stream_out, const MYpolynom& pol) {
    stream_out << pol.a_n;
    for (int i = pol.stepen - 1; i >= 0; i--) {
        if (pol.roots_array[i] < 0) {
            stream_out << " * (x + " << -pol.roots_array[i] << ")";
        } else {
            stream_out << " * (x - " << pol.roots_array[i] << ")";
        }
    }
    return stream_out;
}

unsigned MYpolynom::getStepen() const {
    return stepen;
}
