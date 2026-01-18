#include <QCoreApplication>
#include "number.h"
#include "communicator.h"
#include "polynom.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    Communicator s;
//    MYpolynom<TRational> p;
//    TRational an(1.2);
//    for (int i = 0; i < 5; i++) {TRational x; x.num = i; p.addMonomial(x);}
//    p.a_n = an;
//    string str = p.toStringRational(p);
//    cout << str << endl;
//    p.removeMonomial(1);
//    str = p.toStringRational(p);
//    cout << str << endl;
    return a.exec();
}
