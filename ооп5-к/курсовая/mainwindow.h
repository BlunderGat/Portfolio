#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QVector>
#include <functional>

class PlotWidget;

class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    PlotWidget *plotWidget;
    std::function<double(double)> currentFunction;
};

#endif // MAINWINDOW_H
