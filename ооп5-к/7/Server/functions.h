#ifndef OOP_QT_PROJECT_FUNCTION_H
#define OOP_QT_PROJECT_FUNCTION_H

#include "polynom.h"
#include "cmath"
#include "iostream"
#include "number.h"
#include "rational.h"
#include "complex.h"

int factorial( int n) {
    int result = 1;
    for ( int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

template <class number>
class TFunction : public MYpolynom<number> {
public:
    TFunction( int n, number *coefficients_array);
    number value(number point);
};

template<class number>
TFunction<number>::TFunction( int n, number *coefficients_array) {
    this->stepen = n;
    Array<number> a = this->coefficients_array;
    a.arr = coefficients_array;
}

template<class number>
number TFunction<number>::value(number point) {
    number result = 0;
    MYpolynom<number> MYpolynom (this->stepen);
    point = point * 3.14 / 180.0;
    cout << MYpolynom.stepen << " " << MYpolynom.coefficients_array.getCapacity() << endl;
    for(int i = 0; i <= MYpolynom.stepen; i++) {
        for (int pow = 1; pow <= i; i++) {point = point * point;}
        result = result + (point * this->coefficients_array[i]);
    }
    return result;
}

template<class number>
class TFsin: public TFunction<number> {
    number *derivatives;
    number* derivativesCalc(int n) {
        derivatives = new number[n];
        for (int i = 0; i <= n; ++i) {
            if (i % 4 == 0) {
                derivatives[i] = 0;
            } else if (i % 4 == 1) {
                derivatives[i] = 1.0 / factorial(i);
            } else if (i % 4 == 2) {
                derivatives[i] = 0;
            } else if (i % 4 == 3) {
                derivatives[i] = -1.0 / factorial(i);
            }
            cout << derivatives[i] << " ";
        }
        cout << "\n";
        return derivatives;
    }
    public:
    TFsin( int n): TFunction<number>(n, derivativesCalc(n)) {}
    ~TFsin() { delete[] derivatives; }
};

template<class number>
class TFSi: public TFunction<number> {
    number *derivatives;
    number* derivativesCalc( int n) {
        derivatives = new number[n];
        for (int k = 0; k <= n; ++k) {
            if (k % 2 == 0) {
                derivatives[k] = 0; // Чётные производные равны 0
            } else {
                // Нечётные производные
                int m = k / 2; // m соответствует номеру нечётного члена
                double factorial = 1;
                for (int i = 1; i <= k; ++i) {
                    factorial *= i; // Вычисление факториала
                }
                derivatives[k] = (m % 2 == 0 ? 1 : -1) / ((k + 1) * factorial);
            }
        }
        return derivatives;
    }
    public:
    TFSi( int n): TFunction<number>(n, derivativesCalc(n)) {}
    ~TFSi() { delete[] derivatives; }
};

#endif //OOP_QT_PROJECT_FUNCTION_H
