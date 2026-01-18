#include "array.h"
#include <cmath>
#include <iostream>

Array::Array() : data(nullptr), size(0) {};

Array::Array(int len, number value) : size(len) {
    data = new number[len];
    for (int i = 0; i < len; i++ ){
        data[i] = value;
    }
}

Array::~Array(){
    delete[] data;
};

void Array::Resize(int newSize) {
    number* newData = new number[newSize];
    int elementsToCopy = (newSize < size) ? newSize : size; // нужно меньшее
    for (int i = 0; i < elementsToCopy; i++){
        newData[i] = data[i];
    }

    for (int i = elementsToCopy; i < newSize; i++) {
        newData[i] = 0;
    }
    delete[] data;
    data = newData;
    size = newSize;
};

void Array::Sort(bool isUp) {
    for (int i = 0; i < size - 1; i++){
        for (int j = i + 1; j < size; j++){
            if ((isUp && data[i] > data[j]) || (!isUp && data[i] < data[j])){
                std::swap(data[i],data[j]);
            }
        }
    }
};

number Array::findSr() {
    if (size == 0) {
        return 0;
    }
    number sum = 0;
    for (int i = 0; i < size; i++) {
        sum += data[i];
    }
    return sum / size;
}

number Array::findSKO(){
    if (size <= 1) {
        return 0;
    }

    number sred = findSr();
    number sumSKO = 0;
    for (int i = 0; i < size; i++){
        sumSKO += (data[i] - sred) * (data[i] - sred);
    }
    return std::sqrt(sumSKO / (size - 1));
};

void Array::in(int N) {
    if (data != nullptr) {
        delete[] data;
    }
    size = N;
    data = new number[N];
    std::cout << "Enter " << N << " elements: ";
    for (int i = 0; i < N; i++) {
        std::cin >> data[i];
    }
};

std::istream& operator>>(std::istream& is, Array& array) {
    if (array.data != nullptr) {
        delete[] array.data;
    }

    int N = array.size;
    array.data = new number[N];

    std::cout << "Enter " << N << " elements: ";
    for (int i = 0; i < N; i++) {
        is >> array.data[i];
    }

    return is;
}


std::ostream& operator<<(std::ostream& os, const Array& array) {
    os << "Array size: " << array.size << "\nElements: ";
    for (int i = 0; i < array.size; i++) {
        os << array.data[i] << " ";
    }
    os << std::endl;
    return os;
}

void Array::setElement(int index, number value){
    if (index >= 0 && index < size){
        data[index] = value;
    }
};

int Array::getSize() {
    return size;
};

number Array::getElement(int index){
    if (index >= 0 && index < size){
        return data[index];
    }
    return 0;
};
