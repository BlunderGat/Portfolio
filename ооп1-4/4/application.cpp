#include "application.h"
#include <iostream>
#include <conio.h>

using namespace std;

Application::Application() : myPolynom() {}

void Application::run() {
    bool isRunning = true;
    while (isRunning) {
        showMenu();
        input();
    }
}

void Application::showMenu() {
    cout << "Commands: \n";
    cout << "1. Exit \n";
    cout << "2. Enter/Modify polynomial \n";
    cout << "3. Modify coefficient a_n \n";
    cout << "4. Modify root value by index \n";
    cout << "5. Calculate polynomial value at a given point \n";
    cout << "6. Print polynomial \n";
}

void Application::input() {
    int command;
    cin >> command;
    switch (command) {
    case 1:
        exit(0);
    case 2:
        enterOrModifyPolynomial();
        break;
    case 3:
        modifyCoefficient();
        break;
    case 4:
        modifyRootValue();
        break;
    case 5:
        calculatePolynomialValue();
        break;
    case 6:
        printPolynomial();
        break;
    default:
        cout << "Invalid command!\n";
        break;
    }
    clearScreen();
}

void Application::enterOrModifyPolynomial() {
    unsigned degree;
    number a_n;
    int N;

    cout << "Enter degree of polynomial: ";
    cin >> degree;

    cout << "Enter a_n: ";
    cin >> a_n;

    N = degree;

    Array inputRoots(N);
    cout << "Enter roots    -    ";
    cin >> inputRoots;

    myPolynom.enterPolynom(degree, a_n, inputRoots);
}

void Application::modifyCoefficient() {
    number new_a_n;
    cout << "Enter new a_n: ";
    cin >> new_a_n;
    myPolynom.changeA_n(new_a_n);
}

void Application::modifyRootValue() {
    unsigned index;
    number new_x;
    cout << "Enter index of root to modify: ";
    cin >> index;
    cout << "Enter new value: ";
    cin >> new_x;
    myPolynom.changeX(index, new_x);
}

void Application::calculatePolynomialValue() {
    number x;
    cout << "Enter value of x: ";
    cin >> x;
    cout << "Polynomial value at x = " << x << " is " << myPolynom.findValue(x) << endl;
}

void Application::printPolynomial() {
    cout << myPolynom << endl;
}


void Application::clearScreen() {
    cout << "Press any key to continue...";
    getch();
    system("cls");
}
