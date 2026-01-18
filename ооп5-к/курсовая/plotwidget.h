#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include <QWidget>
#include <QVector>

class PlotWidget : public QWidget {
    Q_OBJECT

public:
    explicit PlotWidget(QWidget *parent = nullptr);
    void setValues(const QVector<double> &xValues, const QVector<double> &yValues);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<double> xValues;
    QVector<double> yValues;
};

#endif // PLOTWIDGET_H
