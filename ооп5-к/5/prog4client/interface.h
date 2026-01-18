#ifndef INTERFACE_H
#define INTERFACE_H
#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVector>
#include "polynom.h"

class Interface : public QMainWindow {
    Q_OBJECT
public:
    Interface(QWidget* parent = nullptr);
    void updatePolynomialDisplay();
    void answer(const QString& msg);
signals:
    void request(QString);
private slots:
    void addMonomial();
    void removeMonomial();
private:
    MYpolynom myPolynom;
    QLabel* polynomialLabel;
    QVector<QLineEdit*> coefficientEdits;
};

#endif // INTERFACE_H