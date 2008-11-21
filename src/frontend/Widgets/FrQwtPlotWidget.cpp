#include "FrQwtPlotWidget.h"

// Qt stuff
#include "Qt/qtoolbox.h"
#include "Qt/qlayout.h"

// Qwt
#include "qwt_plot_curve.h"


FrQwtPlotWidget::FrQwtPlotWidget(QWidget* parent) 
: QwtPlot(parent){
    this->setCanvasBackground(Qt::black);

    // add curves
    QwtPlotCurve *curve1 = new QwtPlotCurve("Curve 1");
    QwtPlotCurve *curve2 = new QwtPlotCurve("Curve 2");

    double x[100], y1[100], y2[100];        // x and y values
    for (int i = 0; i<100; i++){
        x[i] = i;
        y1[i] = x[i]*2;
        y2[i] = -x[i]*3;
    }

    // copy the data into the curves
    curve1->setData(x, y1, 100);
    curve2->setData(x, y2, 100);

    curve1->setPen(QColor(Qt::red));
    curve2->setPen(QColor(Qt::green));

    curve1->attach(this);
    curve2->attach(this);

    // finally, refresh the plot
    this->replot();

    // connect signals and slots
}
