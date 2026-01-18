#pragma once
#ifndef RUNCODE_H
#define RUNCODE_H

#include "polynom.h"

class Runcode {
public:
    Runcode();
    void run();

private:
    MYpolynom myPolynom;
    void showMenu();
    void input();
    void enterOrModifyPolynomial();
    void modifyCoefficient();
    void modifyRootValue();
    void calculatePolynomialValue();
    void printPolynomial();
    void clearScreen();
};

#endif
