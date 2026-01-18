#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H
#pragma once
#include "polynom.h"

class Application {
public:
    Application();
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

#endif // APPLICATION_H
