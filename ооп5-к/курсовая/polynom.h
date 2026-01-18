#ifndef POLYNOM_H
#define POLYNOM_H
#include "array.h"
#include <QString>
#include <iostream>
#include <cmath>

template <class number>
class Polynom {
protected:
    int degree;
    Array<number> roots;        // Массив корней
    number an;         // Ведущий коэффициен
    Array<number> coefficients; // Массив коэффициентов
public:
    Polynom() : degree(0), roots(Array<number>(0)), coefficients(Array<number>(1)), an(0) {
        coefficients.setElement(0, 0);
    }

    Polynom(Array<number> coeffs, Array<number> rts, int deg, number a): degree(deg), roots(rts), coefficients(coeffs), an(a) {
        if (roots.getSize() == 0) {
            roots = Array<number>(0); // Создаем пустой массив для корней по умолчанию
        }
    }

    number evaluateAt(number x) {
        number result(0);
        int length = coefficients.getSize();
        for(int i = 0; i < length; ++i){
            result = result + coefficients.getElement(i) * pow(x, i+1);
        }
        return result;
    }

    void createNullPolinom() {
        degree = 0;
        an = 0;
        roots = Array<number>(0); // Пустой массив для корней
        coefficients = Array<number>(1, 0); // Только a0
    }

    bool checkN() const {
        return degree >= 0;
    }

    number getRoot(int index) const {
        if (index < 0 || index >= roots.getSize()) {
            std::cerr << "Ошибка: индекс корня выходит за пределы массива." << std::endl;
            return 0;
        }
        return roots.getElement(index);
    }

    void setRoot(int index, number value) {
        if (index < 0 || index >= roots.getSize()) {
            std::cerr << "Ошибка: индекс корня выходит за пределы массива." << std::endl;
            return;
        }
        roots.setElement(index, value);
        coefficients.countCoefficients(degree, roots, an); // Пересчитываем коэффициенты
    }

    void setElement(int index, const number& value) {
        coefficients.setElement(index, value);
    }

    int getRootsSize() const {
        return roots.getSize();
    }

    void changeAnRoot(number newAn) {
        an = newAn;
        std::cout << "Коэффициент an изменен на: " << an << std::endl;
    }

    number countPolinom(number xPoint) const {
        number result = an;
        for (int i = 0; i < degree; i++) {
            result = result * (xPoint-roots.getElement(i));
        }
        return result;
    }

    void changeSizeRootCoefficient(int newSizeRootCoefficient) {
        if (newSizeRootCoefficient < 0) {
            std::cerr << "Ошибка! Размер массива корней должен быть положительным." << std::endl;
            return;
        }

        roots.resize(newSizeRootCoefficient);
        coefficients.resize(newSizeRootCoefficient + 1);

        for (int i = 0; i < newSizeRootCoefficient; ++i) {
            number temp;
            std::cout << "Корень[" << i << "] = ";
            std::cin >> temp;
            roots.setElement(i, temp);
        }

        coefficients.setElement(0, an); // Устанавливаем коэффициент a_0
        coefficients.countCoefficients(newSizeRootCoefficient, roots, an); // Пересчитываем коэффициенты

        degree = newSizeRootCoefficient;
        std::cout << "Размеры массивов успешно изменены." << std::endl;
    }

    number getAn() const {
        return an;
    }

    void output(std::ostream& out, int form) const {
        out << "Степень: " << degree << ", Коэффициент an: " << an << std::endl;
        out << "Коэффициенты: ";
        for (int i = 0; i <= degree; ++i) {
            out << coefficients.getElement(i) << " ";
        }
        out << std::endl;

        if (form == 1) {
            out << "p(x) = " << an << "*x^" << degree;
            for (int i = degree - 1; i >= 0; --i) {
                out << " + " << coefficients.getElement(degree - i) << "*x^" << i;
            }
        } else if (form == 2) {
            out << "p(x) = (" << an << ")";
            for (int i = 0; i < degree; ++i) {
                out << "(x - " << roots.getElement(i) << ")";
            }
        }
        out << std::endl;
    }

    void calcCoefs(){
        Array<number> coefs(degree+1, 0);
        coefs.setElement(degree, an);
        for (int k = 0; k < degree; ++k) {
            number root = roots.getElement(k);
            for (int i = degree; i > 0; --i) {
                coefs.setElement(i, coefs.getElement(i) - coefs.getElement(i - 1) * root);
            }
            coefs.setElement(0, coefs.getElement(0) * root * -1);
        }
        for (int i = 0; i <= degree; ++i) {
            coefs.setElement(i, coefs.getElement(i) * an);
        }
        coefficients = coefs;
    }

    QString outputS(){
        QString ans = "p(x) = ";
        for (int i = degree; i >= 0; --i) {
            number el = coefficients.getElement(i);
            if (i != degree)
                ans += " + ";
            ans += QString::number(el);
            ans += "*x^";
            ans += QString::number(i);
        }
        return ans;
    }

    friend istream& operator>>(istream& is, Polynom<number>& p) {
        while (true) {
            cout << "Введите степень полинома:" << endl;
            is >> p.degree;
            if (p.degree >= 0) {
                break;
            }
            cout << "Ошибка! Повторите ввод.\n" << endl;
        }

        cout << "Введите коэффициент a(n):" << endl;
        is >> p.an; // Предполагается, что an тоже может быть комплексным

        cout << "Введите корни полинома:" << endl;
        p.roots.resize(p.degree);  // Изменяем размер массива корней

        for (int i = 0; i < p.degree; ++i) {
            cout << "Корень " << i + 1 << ": ";
            number root; // Предполагается, что корни являются комплексными
            is >> root;  // Используем перегруженный оператор для комплексного числа
            p.roots.setElement(i, root); // Устанавливаем новый корень в полиноме
        }

        p.coefficients.resize(p.degree + 1);
        p.coefficients.countCoefficients(p.degree, p.roots, p.an);  // Пересчитываем коэффициенты на основе корней

        return is;
    }
    friend std::ostream& operator<<(std::ostream& os, const Polynom<number>& p) {
        os << "p(x) = " << p.an; // начинаем с коэффициента an
        for (int i = p.degree; i >= 0; --i) {
            if (i == 0) {
                os << " + " << p.coefficients.getElement(i);
            } else {
                os << " + " << p.coefficients.getElement(i) << "*x^" << i;
            }
        }
        os << std::endl;
        return os;
    }
};

#endif // POLYNOM_H
