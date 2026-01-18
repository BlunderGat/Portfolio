#pragma once

#include <iostream>
#include <stdexcept>

template <typename number>
class Array
{
public:
    number* arr;
    int size = 0;
    int capacity = 0;

    friend std::ostream& operator<<(std::ostream& os, const Array<number> arr);

public:
    Array();
    Array(int input_size);
    ~Array();

    void resize(int new_size);
    void increaseCapacity();
    void pushBack(number data);
    void pushFront(number data);
    void pushIn(number data, int index);
    void popBack();
    void popFront();
    void popIn(int index);
    void clear();
    int getSize();
    int getCapacity();

    number operator[](const int index);


};


template <typename number>
Array<number>::Array()
{
    size = 0;
    capacity = 10;
    arr = new number[capacity];
}

template <typename number>
Array<number>::Array(int input_size)
{
    if (input_size <= 0) { input_size = 5; }

    capacity = input_size * 2;
    arr = new number[capacity];
}

template <typename number>
Array<number>::~Array()
{
    delete[] arr;
}

template <typename number>
void Array<number>::resize(int new_size)
{
    if (new_size == 0)
    {
        if (arr != nullptr) { delete[]arr; }

        arr = nullptr;
        size = 0;
        capacity = 0;
    }
    else
    {
        number* new_arr;
        capacity = new_size;

        try {
            new_arr = new number[capacity];
        }
        catch (std::bad_alloc&) { throw "Memory error"; }

        for (unsigned i = 0; i < size; i++) {
            new_arr[i] = arr[i];
        }
        delete[] arr;
        arr = new_arr;
        size = new_size;
    }
}

template <typename number>
void Array<number>::increaseCapacity()
{
    number* new_arr;

    try { new_arr = new number[capacity * 2]; }
    catch (std::bad_alloc&)
    {
        throw "Memory error";
        return;
    }

    for (unsigned i = 0; i < size; i++) {
        new_arr[i] = arr[i];
    }

    delete[]arr;
    arr = new_arr;
    capacity *= 2;
}

template <typename number>
void Array<number>::pushBack(number data)
{
    pushIn(data, size);
}

template <typename number>
void Array<number>::pushFront(number data)
{
    pushIn(data, 0);
}

template <typename number>
void Array<number>::pushIn(number data, int index)
{
    if ((arr == nullptr) && (index == 0))
    {
        arr = new number[1];
        capacity = 2;
    }

    if (index > size) { return; }

    if ((size + 1) > capacity) { increaseCapacity(); }

    for (unsigned i = size; i > index; i--)
    {
        arr[i] = arr[i - 1];
    }

    arr[index] = data;
    size++;
}

template <typename number>
void Array<number>::popBack()
{
    popIn(size - 1);
}

template <typename number>
void Array<number>::popFront()
{
    popIn(0);
}

template <typename number>
void Array<number>::clear()
{
    while (size != 0)
    {
        popBack();
    }
}

template <typename number>
void Array<number>::popIn(int index)
{
    if (index > size)
    {
        throw "Index out of range";
        return;
    }
    for (unsigned i = index; i < size - 1; i++) {
        arr[i] = arr[i + 1];
    }
    size--;
}

template <typename number>
int Array<number>::getSize()
{
    return size;
}

template <typename number>
int Array<number>::getCapacity()
{
    return capacity;
}

template <typename number>
number Array<number>::operator[](const int index)
{
    if (index < 0 || index >= size) { throw std::out_of_range("Index out of range"); }
    return arr[index];
}

template <typename number>
std::ostream& operator<<(std::ostream& os, const Array<number> arr)
{
    os << "[";
    for (int i = 0; i < arr.size; ++i) {
        if (i > 0) os << ", ";
        os << arr.arr[i];
    }
    os << "]";
    return os;
}
