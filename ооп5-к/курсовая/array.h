#ifndef ARRAY_H
#define ARRAY_H
#include <iostream>

using namespace std;

template <class number>
class Array {
private:
    number* data = nullptr;
    int size;
public:
    ~Array() {
        data = nullptr;
    }
    Array(int newSize, number value) : size(newSize) {
        if (newSize < 0) {
            cerr << "Ошибка: размер массива не может быть отрицательным.\n";
            size = 0;
            data = nullptr;
            return;
        }

        data = new (std::nothrow) number[size];
        if (!data) {
            cerr << "Ошибка: не удалось выделить память.\n";
            size = 0;
            return;
        }

        for (int i = 0; i < size; ++i) {
            data[i] = value;
        }
    }

    Array(const int a) : size(a) {
        data = new (std::nothrow) number[size];
    }

    void creat(istream& in) {
        cout << "Введите размер массива: ";
        int newSize;
        in >> newSize;

        if (newSize < 0) {
            cerr << "Ошибка: размер массива не может быть отрицательным.\n";
            return;
        }

        delete[] data; // Освобождение старых данных
        size = newSize;
        data = new (std::nothrow) number[size];
        if (!data) {
            cerr << "Ошибка: не удалось выделить память.\n";
            size = 0;
            return;
        }

        cout << "Массив создан.\n";
    }

    void resize(int newSize) {
        if (newSize < 0) {
            cerr << "Ошибка: размер массива не может быть отрицательным.\n";
            return;
        }
        number* newData = new (std::nothrow) number[newSize];
        if (!newData) {
            cerr << "Ошибка: не удалось выделить память.\n";
            return;
        }
        int limit = (newSize < size) ? newSize : size;
        for (int i = 0; i < limit; ++i) {
            newData[i] = data[i];  // Копируем данные в новый массив
        }
        delete[] data;  // Освобождаем старую память
        data = newData;  // Устанавливаем указатель на новый массив
        size = newSize;  // Обновляем размер
    }

    void output(ostream& out) const {
        out << "Элементы массива:\n";
        for (int i = 0; i < size; ++i) {
            out << data[i] << " ";
        }
        out << endl;
    }

    number calculateMean() const {
        if (size == 0) {
            cerr << "Ошибка: невозможно вычислить среднее значение для массива нулевого размера.\n";
            return 0; // или другое значение по умолчанию, если требуется
        }

        number sum = 0;
        for (int i = 0; i < size; ++i) {
            sum += data[i];
        }
        return sum / static_cast<number>(size);
    }

    number calculateStdDev() const {
        if (size == 0) {
            cerr << "Ошибка: невозможно вычислить стандартное отклонение для массива нулевого размера.\n";
            return 0; // или другое значение по умолчанию, если требуется
        }

        number mean = calculateMean();
        number sumSquares = 0;

        for (int i = 0; i < size; ++i) {
            sumSquares += pow(data[i] - mean, 2);
        }

        return sqrt(sumSquares / static_cast<number>(size));
    }

    void sort(bool ascending) {
        for (int i = 0; i < size - 1; ++i) {
            for (int j = 0; j < size - i - 1; ++j) {
                if ((ascending && data[j] > data[j + 1]) || (!ascending && data[j] < data[j + 1])) {
                    swap(data[j], data[j + 1]);
                }
            }
        }
        cout << "Массив отсортирован\n";
    }

    void setElement(int index, number value) {
        if (index < size)
            this->data[index] = value;
    }

    int getSize() const {
        return size;
    }

    number getElement(int index) const {
        if (index >= 0 && index < size) {
            return data[index];
        }
        cerr << "Ошибка: индекс выходит за границы массива.\n";
        return 0;
    }

    void countCoefficients(int n, Array& arrayRoots, number an) {
        if (arrayRoots.getSize() == 0 || arrayRoots.getSize() < n) {
            cerr << "Ошибка: недостаточно корней для вычислений.\n";
            return;
        }

        resize(n + 1); // Увеличиваем массив для хранения коэффициентов

        // Устанавливаем коэффициент a_n
        setElement(n, an);

        for (int i = n - 1; i >= 0; --i) {
            number valueRoot = arrayRoots.getElement(i);
            // Пересчитываем коэффициенты, используя формулу Вьета
            for (int j = n; j >= 1; --j) {
                setElement(j - 1, getElement(j - 1) - valueRoot * getElement(j));
            }
        }
    }
};

#endif // ARRAY_H
