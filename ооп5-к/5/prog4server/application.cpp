#include "application.h"
#include "polynom.h"
#include "common.h"

TApplication::TApplication(int argc, char *argv[])
    : QCoreApplication(argc, argv)
{
    TCommParams pars = { QHostAddress("127.0.0.1"), 10000,
                         QHostAddress("127.0.0.1"), 10001};
    comm = new TCommunicator(pars, this);
    connect(comm, SIGNAL(recieved(QByteArray)), this, SLOT(recieve(QByteArray)));
}

void TApplication::recieve(QByteArray msg) {
    QString answer, s;
    int pos = msg.indexOf(separator);
    int t = msg.left(pos).toInt();
    switch (t) {
        case PRINT_CLASSIC_REQUEST:
            msg = msg.right(msg.length() - pos - 1);
            pos = msg.indexOf(separator);
            double coeff = msg.left(pos).toDouble();
            unsigned degree = msg.right(msg.length() - pos - 1).toUInt();
            MYpolynom p;
            p.addMonomial(coeff, degree);
            s = QString::fromStdString(p.toString());
            answer << QString().setNum(PRINT_CLASSIC_ANSWER) << separator << s;
            break;
        default: return;
    }
    comm->send(QByteArray().append(answer));
}