#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <QDataStream>
#include <QVector>
#include <QTcpServer>
#include <QTcpSocket>
#include "polynom.h"
#include "complex.h"
#include "rational.h"


class Communicator : public QTcpServer
{
    QString t = "c";
    Q_OBJECT

public:
    Communicator();
    QTcpSocket *socket;

public slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();

private:
    QVector <QTcpSocket*> Sockets;
    QByteArray Data;
    void SendToClient(QString str);
};

#endif // COMMUNICATOR_H
