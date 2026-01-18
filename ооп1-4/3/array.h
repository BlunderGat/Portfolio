#ifndef ARRAY_H
#define ARRAY_H

#include "number.h"
#include <iostream>
#include <stdexcept>

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
    int getSize() const;
    number getElement(int index);

    number& operator[](int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }

    const number& operator[](int index) const {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }

    friend std::istream& operator>>(std::istream& is, Array& array);
    friend std::ostream& operator<<(std::ostream& os, const Array& array);
};

#endif // ARRAY_H
