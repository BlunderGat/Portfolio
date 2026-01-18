#include <QCoreApplication>
#include "number.h"
#include "communicator.h"
#include "polynom.h"
#include "functions.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    //Communicator s;

    complex point(1, 2);
    TFsin<complex> *SinComplex;
    SinComplex = new TFsin<complex>(2);
    complex value = SinComplex->value(point);
    cout << value;

    return a.exec();
}
