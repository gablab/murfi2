#include "FrQwtPlotWidget.h"

// Qt stuff
#include "Qt/qtoolbox.h"
#include "Qt/qlayout.h"
#include "Qt/qtooltip.h"

// Qwt
#include "qwt_plot_curve.h"
#include "qwt_plot_grid.h"
#include "qwt_plot_picker.h"
#include "qwt_plot_marker.h"
//#include "scalepicker.h"


FrQwtPlotWidget::FrQwtPlotWidget(QWidget* parent) 
: QwtPlot(parent){
    //this->setCanvasBackground(Qt::black);

    m_Curves.clear();

    // add curves
    QwtPlotCurve *curve1 = new QwtPlotCurve("Curve 1");
    QwtPlotCurve *curve2 = new QwtPlotCurve("Curve 2");
    
    // create grid lines for plot
    QwtPlotGrid* grid = new QwtPlotGrid();
    //QwtScaleDiv xdiv;
    //xdiv.setInterval(0, 1000);
    //QwtScaleDiv ydiv;
    //ydiv.setInterval(0, 1000);
    
    //grid->setXDiv(xdiv);
    //grid->setYDiv(ydiv);
    grid->setPen(QPen(Qt::gray));
    grid->attach(this);

    // marker
    m_PlotMarker = new QwtPlotMarker();
    m_PlotMarker->setValue(0.0, 0.0);
    m_PlotMarker->setLineStyle(QwtPlotMarker::VLine);
    m_PlotMarker->setLabelAlignment(Qt::AlignRight | Qt::AlignBottom);
    m_PlotMarker->setLinePen(QPen(Qt::black, 0, Qt::SolidLine));
    m_PlotMarker->attach(this);

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

    // test
    QwtPlotPicker *plotPicker = new QwtPlotPicker(this->canvas());
    plotPicker->setTrackerMode(QwtPicker::AlwaysOn);
    //plotPicker->setRubberBand(QwtPicker::VLineRubberBand);
    //plotPicker->setRubberBandPen(QPen(Qt::red));

    // connect signals and slots
    //connect(picker, SIGNAL(selected(const QwtPolygon &)), this, SLOT(test(const QwtPolygon &)));
    connect(plotPicker, SIGNAL(selected(const QwtDoublePoint& )), this, SLOT(test(const QwtDoublePoint& )));

    // emit the position of clicks on widget
//    plotPicker->setSelectionFlags(QwtPicker::VLineRubberBand & QwtPicker::PointSelection | QwtPicker::ClickSelection);
    plotPicker->setSelectionFlags(QwtPicker::PointSelection | QwtPicker::ClickSelection);

//    ScalePicker* scalePicker = new ScalePicker(this);
  //  connect(scalePicker, SIGNAL(clicked(double, double)), this, SLOT(test2(double, double)));
}

FrQwtPlotWidget::~FrQwtPlotWidget(){
    
}

void FrQwtPlotWidget::AddGraph(){
    // QString name = graph.getName();
    // graph.getData(double* x, double* y);
    // QColor color = graph.getColor();
    QString name = "Curve 1";
    QColor color = QColor(100, 100, 0);

    QwtPlotCurve *curve = new QwtPlotCurve(name);
    m_Curves.push_back(curve);
    
    // test -----------------------------
    double x[100], y[100];        // x and y values
    for (int i = 0; i<100; i++){
        x[i] = i;
        y[i] = x[i]*2;
    }
    // ---- test ------------------

    curve->setData(x, y, 100);
    curve->setPen(color);
    curve->attach(this);    
    
    // refresh the plot
    this->replot();
}

void FrQwtPlotWidget::RemoveGraph(int id){
    QwtPlotCurve* curve = m_Curves[id];
    curve->detach();

    // find appropriate object and remove it
//    std::vector<QwtPlotCurve*>::iterator it;
//    it = std::find(m_Curves.begin(), m_Curves.end(), curve);

//    m_Curves.erase(it);

    // refresh the plot
    this->replot();
}

void FrQwtPlotWidget::SetData(int id){
    // graph.getData(double* x, double* y);
    // get specified curve
    QwtPlotCurve *curve = m_Curves[id];

    // test -----------------------------
    double x[100], y[100];        // x and y values
    for (int i = 0; i<100; i++){
        x[i] = i;
        y[i] = x[i]*2;
    }
    // ---- test ------------------

    curve->setData(x, y, 100);

    // refresh the plot
    this->replot();
}

void FrQwtPlotWidget::SetVisibility(int id, bool visible){
    // get specified curve
    QwtPlotCurve *curve = m_Curves[id];
    curve->setVisible(visible);

    // refresh the plot
    this->replot();             // not sure we need this
}

void FrQwtPlotWidget::test(const QwtDoublePoint& point){
//    QString s;
//    s = QString::number(point.x()) + ", " + QString::number(point.y());

//    QToolTip::showText(QPoint(0, 0), "test", this);
    m_PlotMarker->setValue(point);
    
    // refresh the plot
    this->replot();

    emit pointClicked(point);
}

void FrQwtPlotWidget::test2(double x, double y){
    int g = 5;
}

void FrQwtPlotWidget::RemoveAll(){
    // TODO: clear all curves
}