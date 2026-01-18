#include "plotwidget.h"
#include <QPainter>
#include <algorithm>

PlotWidget::PlotWidget(QWidget *parent) : QWidget(parent) {}

void PlotWidget::setValues(const QVector<double> &xValues, const QVector<double> &yValues) {
    this->xValues = xValues;
    this->yValues = yValues;
    update();
}

void PlotWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(rect(), Qt::white);

    if (xValues.isEmpty() || yValues.isEmpty() || xValues.size() != yValues.size())
        return;

    double xMin = *std::min_element(xValues.begin(), xValues.end());
    double xMax = *std::max_element(xValues.begin(), xValues.end());
    double yMin = *std::min_element(yValues.begin(), yValues.end());
    double yMax = *std::max_element(yValues.begin(), yValues.end());

    double xScale = width() / (xMax - xMin);
    double yScale = height() / (yMax - yMin);

    painter.setPen(Qt::black);
    painter.drawLine(0, height() / 2, width(), height() / 2);
    painter.drawLine(width() / 2, 0, width() / 2, height());

    painter.setPen(Qt::blue);
    for (int i = 1; i < xValues.size(); ++i) {
        int x1 = (xValues[i - 1] - xMin) * xScale;
        int y1 = height() - (yValues[i - 1] - yMin) * yScale;
        int x2 = (xValues[i] - xMin) * xScale;
        int y2 = height() - (yValues[i] - yMin) * yScale;

        painter.drawLine(x1, y1, x2, y2);
    }
}
