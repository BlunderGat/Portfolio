#include "interface.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QRegularExpression>

Interface::Interface(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Interface");
    polynomialLabel = new QLabel("Polynomial: ");
    QPushButton* addButton = new QPushButton("+");
    QPushButton* removeButton = new QPushButton("-");
    QPushButton* updateButton = new QPushButton("Update");
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(polynomialLabel);
    layout->addWidget(addButton);
    layout->addWidget(removeButton);
    layout->addWidget(updateButton);
    coefficientEdits.push_back(new QLineEdit(this));
    layout->addWidget(coefficientEdits.back());
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
    connect(addButton, &QPushButton::clicked, this, &Interface::addMonomial);
    connect(removeButton, &QPushButton::clicked, this, &Interface::removeMonomial);
    connect(updateButton, &QPushButton::clicked, this, &Interface::updatePolynomialDisplay);
}

void Interface::addMonomial() {
    if (coefficientEdits.empty()) {
        QMessageBox::warning(this, "Input Error", "No coefficient field available.");
        return;
    }
    QString input = coefficientEdits.back()->text().trimmed();
    if (input.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a monomial.");
        return;
    }
    input.remove(QRegularExpression("^\\s*\\(\\s*"));
    input.remove(QRegularExpression("\\s*\\)\\s*$"));
    bool ok;
    double coeff = 1.0;
    unsigned degree = 0;
    if (input.contains('x')) {
        int index = input.indexOf('x');
        QString coeffStr = input.left(index).trimmed();
        if (!coeffStr.isEmpty()) {
            coeff = coeffStr.toDouble(&ok);
            if (!ok) {
                QMessageBox::warning(this, "Input Error", "Invalid coefficient.");
                return;
            }
        }
        if (input.length() > index + 1 && input[index + 1] == '^') {
            QString degreeStr = input.mid(index + 2).trimmed();
            degree = degreeStr.toUInt(&ok);
            if (!ok) {
                QMessageBox::warning(this, "Input Error", "Invalid degree.");
                return;
            }
        }
    } else {
        coeff = input.toDouble(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Input Error", "Invalid coefficient.");
            return;
        }
    }
    try {
        myPolynom.addMonomial(coeff, degree);
        updatePolynomialDisplay();
        QString request = QString().setNum(PRINT_CLASSIC_REQUEST) + separator + QString::number(coeff) + separator + QString::number(degree);
        emit request(request);
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Input Error", e.what());
    }
    coefficientEdits.push_back(new QLineEdit(this));
    QWidget* parentWidget = qobject_cast<QWidget*>(polynomialLabel->parent());
    if (parentWidget) {
        parentWidget->layout()->addWidget(coefficientEdits.back());
    } else {
        QMessageBox::warning(this, "Layout Error", "Could not access layout.");
    }
}

void Interface::removeMonomial() {
    if (!coefficientEdits.empty()) {
        delete coefficientEdits.back();
        coefficientEdits.pop_back();
    }
}

void Interface::updatePolynomialDisplay() {
    QString polynomialString;
    for (auto& edit : coefficientEdits) {
        QString input = edit->text().trimmed();
        if (!input.isEmpty()) {
            polynomialString += input + " * ";
        }
    }
    if (!polynomialString.isEmpty()) {
        polynomialString.chop(3);
    }
    polynomialLabel->setText("Polynomial: " + polynomialString);
}

void Interface::answer(const QString& msg) {
    polynomialLabel->setText("Answer: " + msg);
}