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

private slots:
    void addMonomial();
    void removeMonomial();

private:
    MYpolynom myPolynom;
    QLabel* polynomialLabel;
    QLineEdit* monomialInput;
    QPushButton* addButton;
    QPushButton* removeButton;
    QPushButton* updateButton;
    QVector<QLineEdit*> coefficientEdits;

    void setupUi();
};

#endif // INTERFACE_H
