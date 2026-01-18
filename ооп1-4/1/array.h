#include "number.h"
#include <iostream>
#ifndef ARRAY_H
#define ARRAY_H


class Array {
private:
    number* data;
    int size;
public:
    Array();
    Array(int len, number value = 0);
    ~Array();

    void Resize(int newSize);
    void Sort(bool isUp);
    number findSr();
    number findSKO();
    void in(int N);
    void setElement(int index, number value);
    int getSize();
    number getElement(int index);
    friend std::istream& operator>>(std::istream& is, Array& array);
    friend std::ostream& operator<<(std::ostream& os, const Array& array);
};

#endif // ARRAY_H
