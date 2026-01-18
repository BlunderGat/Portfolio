#include "mainwindow.h"
#include "plotwidget.h"
#include "fsi.h"
#include "fsin.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <cmath>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    auto *mainLayout = new QVBoxLayout(this);

    // Создаем виджеты для ввода диапазона
    auto *x1Input = new QLineEdit(this);
    auto *x2Input = new QLineEdit(this);

    // Создаем лейбл
    auto *rangeLabel = new QLabel("Введите диапазон от x1 до x2:", this);

    // Горизонтальный лейаут для лейбла и двух полей ввода
    auto *inputLayout = new QHBoxLayout();
    inputLayout->addWidget(rangeLabel);
    inputLayout->addWidget(x1Input);
    inputLayout->addWidget(x2Input);

    // Создаем кнопки
    auto *sinusButton = new QPushButton("Синус", this);
    auto *integralSinusButton = new QPushButton("Интегральный синус", this);

    // Горизонтальный лейаут для кнопок
    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(sinusButton);
    buttonLayout->addWidget(integralSinusButton);

    // Добавляем элементы в основной лейаут
    mainLayout->addLayout(inputLayout);  // Добавляем горизонтальный лейаут с текстом и вводами
    mainLayout->addLayout(buttonLayout); // Добавляем горизонтальный лейаут с кнопками

    // Виджет графика
    plotWidget = new PlotWidget(this);
    mainLayout->addWidget(plotWidget);

    // Логика работы кнопок
    connect(sinusButton, &QPushButton::clicked, this, [=]() {
        currentFunction = [](double x) { return sin(x); };
        double x1 = x1Input->text().toDouble();
        double x2 = x2Input->text().toDouble();

        QVector<double> xValues;
        QVector<double> yValues;

        for (double x = x1; x <= x2; x += 0.1) {
            xValues.append(x);
            yValues.append(currentFunction(x));
        }

        plotWidget->setValues(xValues, yValues);
    });

    connect(integralSinusButton, &QPushButton::clicked, this, [=]() {
        currentFunction = [](double x) { return std::sin(x) / x; };
        double x1 = x1Input->text().toDouble();
        double x2 = x2Input->text().toDouble();

        QVector<double> xValues;
        QVector<double> yValues;

        for (double x = x1; x <= x2; x += 0.1) {
            xValues.append(x);
            yValues.append(currentFunction(x));
        }

        plotWidget->setValues(xValues, yValues);
    });

    currentFunction = [](double x) { return sin(x); };
}
