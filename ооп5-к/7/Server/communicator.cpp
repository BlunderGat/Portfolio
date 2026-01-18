#include "communicator.h"
#include "array.h"
#include <string.h>
#include "polynom.h"
#include "functions.h"


complex a_n_c(0, 0);
Array<complex> arr_c;
int s_c = 0;
MYpolynom<complex> p_c(a_n_c, s_c, arr_c);

TRational a_n_r(0);
Array<TRational>arr_r;
int s_r = 0;
MYpolynom<TRational>p_r(a_n_r, s_r, arr_r);

int accur(100);

Communicator::Communicator()
{

    if(this->listen(QHostAddress::Any, 2323))
    {
        qDebug() << "start";
    }
    else
    {
        qDebug() << "error";
    }
}

void Communicator::incomingConnection(qintptr socketDescriptor)
{
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::readyRead, this, &Communicator::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &Communicator::deleteLater);
    Sockets.push_back(socket);
    qDebug() << "client connected";
}

void Communicator::slotReadyRead()
{

    socket = (QTcpSocket*)sender();
    QDataStream in(socket);
    QString tpast = "";
    if(in.status() == QDataStream::Ok){
        qDebug() << "read";
        QString str;
        in >> str;
        if ((str == "ch") || (str == "rh"))
        {
            p_c.clean_complex();
            p_r.clean_rational();
        }

//        if (str == "r" || str == "c")
//        {
//            qDebug() << "IF str" + str;
//            if (str == "r") {t = "r";}
//            else if (str == "c") {t = "c";}
//        }
        qDebug() << str;
        if (str == "c")
        {
            QString str;
            in >> str;
            qDebug() << "read";
            qDebug() << "Current complex";
            if (str == "add_el")
            {
                qDebug() << "op0";
                QString str1;
                in >> str1;
                QString str2;
                in >> str2;
                complex x;
                x.re = str1.toDouble();
                x.im = str2.toDouble();
                p_c.addMonomial(x);
                Communicator::SendToClient("");

            }
            else if (str == "chng_coef")
            {
                qDebug() << "op1";
                QString str1;
                in >> str1;
                QString str2;
                in >> str2;
                complex x;
                x.re = str1.toDouble();
                x.im = str2.toDouble();
                p_c.changeA_n_complex(x);
                Communicator::SendToClient("");

            }
            else if (str == "del_el")
            {
                qDebug() << "op2";
                QString str1;
                in >> str1;
                int i = str.toInt();
                p_c.removeMonomial(i);
                qDebug() << "RM" + QString::fromStdString(p_r.toStringRational(p_r));
                Communicator::SendToClient("");

            }
            else if (str == "find_val")
            {
                qDebug() << "op3";
                QString str1;
                in >> str1;
                QString str2;
                in >> str2;
                complex x;
                x.re = str1.toDouble();
                x.im = str2.toDouble();
                complex xf = p_c.findValue(x);
                QString st = "("+QString::number(xf.re) + " + "+  QString::number(xf.im) + "i)";
                Communicator::SendToClient(st);

            }

            QString pol = QString::fromStdString(p_c.toStringComplex(p_c));
            qDebug() << "Data send";
            qDebug() << pol;
            Communicator::SendToClient(pol);
    }
        else if (str == "r")
        {
            QString str;
            in >> str;
            qDebug() << "read";
            qDebug() << "Current rational";
            if (str == "c") {t ="r";}
            else if (str == "r") {t = "c";}
            if (str == "add_el")
            {
                qDebug() << "op0";
                QString str;
                in >> str;
                TRational x;
                x.num = str.toDouble();
                p_r.addMonomial(x);
                Communicator::SendToClient("");

            }
            else if (str == "chng_coef")
            {
                qDebug() << "op1";
                QString str;
                in >> str;
                TRational x;
                x.num = str.toDouble();
                p_r.changeA_n_rational(x);
                Communicator::SendToClient("");

            }
            else if (str == "del_el")
            {
                qDebug() << "op2";
                QString str1;
                in >> str1;
                int i = str.toInt();
                p_r.removeMonomial(i);
                Communicator::SendToClient("");

            }
            else if (str == "find_val")
            {
                qDebug() << "op3";
                QString str1;
                in >> str1;
                TRational x;
                x.num = str1.toDouble();
                TRational xf = p_r.findValue(x);
                QString st = QString::number(xf.num);
                Communicator::SendToClient(st);

            }
            QString pol = QString::fromStdString(p_r.toStringRational(p_r));
            qDebug() << pol;
            qDebug() << "Data send";
            Communicator::SendToClient(pol);
        }
        else if (str == "f")
        {
            in >> str;
            if (str == "del_el")
            {
                QString str_d;
                in >> str_d;
                accur = str_d.toInt();
                Communicator::SendToClient("");
            }
            if (str == "sin")
            {
                TFsin<complex> *SinComplex;
                in >> str;
                complex x;
                QString str1;
                in >> str1;
                QString str2;
                in >> str2;
                x.re = str1.toDouble();
                x.im = str2.toDouble();
                SinComplex = new TFsin<complex>(accur);
                complex v = SinComplex->value(x);
                QString st = "("+QString::number(v.re) + " + "+  QString::number(v.im) + "i)";
                Communicator::SendToClient(st);
            }
            else if (str == "SI")
            {
                TFSi<complex> *SIComplex;
                in >> str;
                complex x;
                QString str1;
                in >> str1;
                QString str2;
                in >> str2;
                x.re = str1.toDouble();
                x.im = str2.toDouble();
                SIComplex = new TFSi<complex>(accur);
                complex v = SIComplex->value(x);
                QString st = "("+QString::number(v.re) + " + "+  QString::number(v.im) + "i)";
                Communicator::SendToClient(st);
            }

        }

    }

    else
    {
        qDebug() << "Data Stream error";
    }

}

void Communicator::SendToClient(QString str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out << str;
    socket->write(Data);
}
