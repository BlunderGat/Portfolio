#ifndef POLYNOM_H
#define POLYNOM_H

#include <string>
#include <iostream>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "array.h"
#include "rational.h"

template <typename number>

class MYpolynom {
public:
    number a_n;
    Array<number> roots_array;
    unsigned stepen;
    Array<number> coefficients_array;


public:
    MYpolynom();
    MYpolynom(number a_n, unsigned stepen,  Array<number>& input_roots);
    void Count_coeff();
    void clean_complex();
    void clean_rational();
    number getCoefficient(unsigned pos) const;
    void enterPolynom(unsigned stepen, number a_n, const Array<number>& input_roots);
    void changeA_n_complex(number new_a_n);
    void changeA_n_rational(number new_a_n);
    void changeX(unsigned index, number new_x);
    number findValue(number x) ;
    unsigned getStepen() const;
    void addMonomial(number coeff);
    number getA_n();
    void addEl(number x);
    void removeMonomial(unsigned index);
    std::string toStringComplex( MYpolynom& pol);
    std::string toStringRational( MYpolynom& pol);

    friend std::ostream& operator<<(std::ostream& stream_out, const MYpolynom& pol);
};

using namespace std;


template <typename number>
MYpolynom<number>::MYpolynom() : a_n(0), stepen(0), roots_array(), coefficients_array() {}


template <typename number>
MYpolynom<number>::MYpolynom(number a_n, unsigned stepen,  Array<number>& input_roots) {
    this->a_n = a_n;
    if (stepen != input_roots.getSize()) {
        throw std::invalid_argument("Array size does not match degree");
    }
    this->stepen = stepen;

    roots_array.resize(stepen);
    coefficients_array.resize(stepen + 1);
    for (int i = 0; i < stepen; i++) {
        roots_array[i] = input_roots[i];
    }
    Count_coeff();
}

template <typename number>
void MYpolynom<number>::clean_complex()
{
    roots_array.clear();
    coefficients_array.clear();
    a_n.re = 0;
    a_n.im = 0;
    stepen = 0;
}

template <typename number>
void MYpolynom<number>::clean_rational()
{
    roots_array.clear();
    coefficients_array.clear();
    a_n.num = 0;
    stepen = 0;
}

template <typename number>
void MYpolynom<number>::addMonomial(number coeff) {
    if (coeff == 0) return;
        if (stepen == 0 && coefficients_array.getSize() == 0)
        {
            coefficients_array.pushBack(coeff);
            roots_array.pushBack(coeff);
            stepen++;
        }
        else if (stepen < coefficients_array.getSize() - 1) {
            coefficients_array[stepen + 1] = coeff;
            stepen++;
        } else {
            coefficients_array.pushBack(coeff);
            roots_array.pushBack(coeff);
            stepen++;
        }
}


template <typename number>
void MYpolynom<number>::removeMonomial(unsigned index) {
    if (index >= stepen) {
        throw std::out_of_range("Index exceeds polynomial degree");
    }

    if (index == stepen - 1) {
        coefficients_array.resize(stepen - 1);
        roots_array.popIn(index);
        stepen--;
        return;
    }

    for (unsigned i = index; i < stepen - 1; i++) {
        coefficients_array[i] = coefficients_array[i + 1];
        roots_array[i] = roots_array[i + 1];
    }
    coefficients_array.popIn(index);
    roots_array.popIn(index);
    stepen--;
}

template <typename number>
std::string MYpolynom<number>::toStringComplex( MYpolynom& pol)  {
    string str = "";


     if (a_n.re * a_n.im != 0.0) {
         std::stringstream ss1;
         std::stringstream ss2;

         double xre = pol.a_n.re;
         double xim = pol.a_n.im;
         ss1 << std::fixed << std::setprecision(1) << xre;
         ss2 << std::fixed << std::setprecision(1) << xim;
         str += ("(");
         str +=  ss1.str() + " + " + ss2.str() + "i";
         str+= ")";
         if (pol.roots_array.getSize() != 0) {str += " * ";}
     }
     if (stepen > 0){
         for (int i = 0; i <=  pol.stepen - 1; i++) {
             std::stringstream ss1;
             std::stringstream ss2;
             number x = pol.roots_array[i];
             double xre = x.re;
             double xim = x.im;
             ss1 << std::fixed << std::setprecision(1) << xre;
             ss2 << std::fixed << std::setprecision(1) << xim;
             if (i != 0) {str+= " * ";}
             str += "(x - (";
             str += ss1.str() + " + " + ss2.str() + "i";
             str+="))";

         }
     }
        return str;
}

template <typename number>
std::string MYpolynom<number>::toStringRational(MYpolynom& pol)  {
    string str = "";
     if (a_n.num != 0.0) {
         std::stringstream s;
         s << std::fixed << std::setprecision(1) << a_n.num;
         str +=  s.str();
         if (pol.roots_array.getSize() != 0) {str += " * ";}
     }
     if (stepen > 0){
         for (int i = 0; i <=  pol.stepen - 1; i++) {
             std::stringstream s;
             TRational x = pol.roots_array[i];
             double n = x.num;

             if (i != 0) {str+= " * ";}
             if (n < 0)
             {
             n = -n;
             s << std::fixed << std::setprecision(1) << n;
             str += "(x + ";
             str += s.str();
             str+=")";
             }
             else
             {
                 s << std::fixed << std::setprecision(1) << n;
                 str += "(x - ";
                 str += s.str();
                 str+=")";
             }
         }
     }
        return str;
}



template <typename number>
void MYpolynom<number>::Count_coeff() {
    coefficients_array[0] = 1;
    for (unsigned j = 1; j <= stepen; j++) {
        coefficients_array[j] = 0;
    }
    for (unsigned j = 0; j < stepen; j++) {
        for (int k = j; k >= 0; k--) {
            coefficients_array[k + 1] = coefficients_array[k + 1] - roots_array[j] * coefficients_array[k];
        }
    }
    for (unsigned i = 0; i <= stepen; i++) {
        coefficients_array[i] = coefficients_array[i] * a_n;
    }
}

template <typename number>
number MYpolynom<number>::getCoefficient(unsigned pos) const {
    if (pos > stepen) {
        throw std::out_of_range("Index exceeds polynomial degree");
    }
    return coefficients_array[stepen - pos];
}

template <typename number>
void MYpolynom<number>::enterPolynom(unsigned new_stepen, number new_a_n, const Array<number>& input_roots) {
    if (new_a_n == 0) {
        throw std::invalid_argument("a_n cannot be zero");
    }
    this->a_n = new_a_n;
    if (new_stepen != input_roots.getSize()) {
        throw std::invalid_argument("Array size does not match degree");
    }
    stepen = new_stepen;

    roots_array.resize(stepen);
    coefficients_array.resize(stepen + 1);
    for (int i = 0; i < stepen; i++) {
        roots_array[i] = input_roots[i];
    }
    Count_coeff();
}

template <typename number>
void MYpolynom<number>::changeA_n_complex(number new_a_n) {
    a_n.re = new_a_n.re;
    a_n.im = new_a_n.im;
    //Count_coeff();
}

template <typename number>
void MYpolynom<number>::changeA_n_rational(number new_a_n) {
    a_n.num = new_a_n.num;
    //Count_coeff();
}

template <typename number>
number MYpolynom<number>::getA_n()
{
    return a_n;
}

template <typename number>
void MYpolynom<number>::changeX(unsigned index, number new_x) {
    if (index >= stepen) {
        throw std::out_of_range("Invalid index");
    }
    roots_array[index] = new_x;
    Count_coeff();
}

template <typename number>
number MYpolynom<number>::findValue(number x)  {
    number val = 1;
    for (unsigned i = 0; i < stepen; i++) {
        val = val * (x - roots_array[i]);
    }
    return val * a_n;
}

template <typename number>
std::ostream& operator<<(std::ostream& stream_out, const MYpolynom<number>& pol) {
    stream_out << "(" << pol.a_n << ")";
    for (int i = pol.stepen - 1; i >= 0; i--) {
        number x = pol.roots_array[i];
        stream_out << x;
    }
    return stream_out;
}

template <typename number>
unsigned MYpolynom<number>::getStepen() const {
    return stepen;
}

#endif // POLYNOM_H
