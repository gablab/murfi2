#include "FrQwtPlotWidget.h"
#include "FrMainDocument.h"
#include "FrUtils.h"

// Qt stuff
#include "Qt/qtoolbox.h"
#include "Qt/qlayout.h"
#include "Qt/qtooltip.h"

// Qwt
#include "qwt_plot_curve.h"
#include "qwt_plot_grid.h"
#include "qwt_plot_picker.h"
#include "qwt_plot_marker.h"

// For GCC. Helps him find abs(double)
#include <cmath>

#define DUMMY_GRAPH_ID -1
#define PLOT_GRID_COLOR Qt::gray


FrQwtPlotWidget::FrQwtPlotWidget(QWidget* parent) 
: QwtPlot(parent){  
    // Setup this properties
    //this->setCanvasBackground(Qt::black);
    //this->setAxisTitle(QwtPlot::xBottom, QString("Time Points"));
    QFont font= this->axisFont(QwtPlot::xBottom);
    font.setPointSize(8);
    this->setAxisFont(QwtPlot::xBottom, font);
    this->setAxisScale(QwtPlot::xBottom, 0.0, 0.0, 10.0);

    // create grid lines for plot
    m_Grid = new QwtPlotGrid();
    m_Grid->enableY(false);
    m_Grid->setPen(QPen(PLOT_GRID_COLOR));
    m_Grid->attach(this);

    // canvas
    setCanvasBackground(Qt::black);

    // marker
    m_PlotMarker = new QwtPlotMarker();
    m_PlotMarker->setValue(0.0, 0.0);
    m_PlotMarker->setLineStyle(QwtPlotMarker::VLine);
    m_PlotMarker->setLabelAlignment(Qt::AlignRight | Qt::AlignBottom);
    m_PlotMarker->setLinePen(QPen(Qt::green, 1, Qt::SolidLine));
    m_PlotMarker->attach(this);

    m_PlotPicker = new QwtPlotPicker(this->canvas());
    m_PlotPicker->setTrackerMode(QwtPicker::AlwaysOn);
    m_PlotPicker->setSelectionFlags(
        QwtPicker::PointSelection | 
	//QwtPicker::ClickSelection |
        //QwtPicker::RectSelection | 
        QwtPicker::DragSelection);
    
    // connect signals and slots
    connect(m_PlotPicker, 
        SIGNAL(selected(const QwtDoublePoint& )), 
        this, SLOT(onPointClicked(const QwtDoublePoint& )));

    connect(m_PlotPicker, 
        SIGNAL(moved(const QwtDoublePoint& )), 
        this, SLOT(onPointMoved(const QwtDoublePoint& )));

    this->setMinimumHeight(this->sizeHint().height());
    //this->setFixedWidth(this->sizeHint().width());
    //this->setFixedHeight(this->sizeHint().height());

    yMin = 999.0f;      // test
    yMax = 0.0f;

}

FrQwtPlotWidget::~FrQwtPlotWidget(){
    this->RemoveAll();

    if (m_Grid){
        m_Grid->detach();
        delete m_Grid;
    }
    if (m_PlotMarker){
        m_PlotMarker->detach();
        delete m_PlotMarker;
    }
    if (m_PlotPicker) delete m_PlotPicker;
}

void FrQwtPlotWidget::AddGraph(unsigned long id, QString& name, QColor& color){
    // Check if already added
    if(m_Curves.find(id) != m_Curves.end()) return;
    
    QwtPlotCurve *curve = new QwtPlotCurve(name);
    m_Curves[id] = curve;
    m_Curves[id]->setVisible(true);     // NOTE: always true at this moment
    
    //// TODO: delete after testing
    //// test -----------------------------
    //int dataSize = 9;
    //double* data = new double[dataSize];
    //srand(31415 + id);
    //for (int i = 0; i < dataSize; i++){

    //    data[i] = 1.0 * (rand() % 255);
    //}
    //this->SetData(id, data, dataSize);
    //delete[] data;
    //// ---- test ------------------

    curve->setPen(QPen(color,2,Qt::SolidLine));
    curve->attach(this);    
    
    // refresh the plot
    //this->replot();
}

void FrQwtPlotWidget::RemoveGraph(unsigned long id){
    // Check if exists
    CurvesMap::iterator it = m_Curves.find(id);
    if(it == m_Curves.end()) return;

    it->second->detach();
    delete it->second;
    m_Curves.erase(it);

    this->setAxisScale(
        QwtPlot::xBottom, 
        0.0, 0.0, 1.0);

    // refresh the plot
    //this->replot();
}

void FrQwtPlotWidget::RemoveAll(){
    if (m_Curves.size() == 0)
        return;

    // clear all curves
    CurvesMap::iterator it, itEnd(m_Curves.end());
    for(it = m_Curves.begin(); it != itEnd; ++it){

        it->second->detach();
        delete it->second;
    }
    
    // Clear all curves and update scale of x axis
    m_Curves.clear();
    this->setAxisScale(
        QwtPlot::xBottom, 
        0.0, 0.0, 1.0);

    // refresh the plot
    //this->replot();
}


void FrQwtPlotWidget::SetData(unsigned long id, double data[], int dataSize){
    // check
    CurvesMap::iterator it = m_Curves.find(id);
    if(it == m_Curves.end()) return;

    yMin = data[0];
    yMax = data[0];

    // Init x values and find min and max
    double* xValues = new double[dataSize];
    for (int i = 0; i < dataSize; i++){
        xValues[i] = double(i);
	    if(yMin > data[i]) {
	      yMin = data[i];
	    }
	    if(yMax < data[i]) {
	      yMax = data[i];
	    }
    }
    it->second->setData(xValues, data, dataSize);
    delete[] xValues;

//    cout << "drawing with pen color: " 
//	 << it->second->pen().color().red() << " "
//	 << it->second->pen().color().green() << " "
//	 << it->second->pen().color().blue() << " "
//	 << endl;

    // Update axis
    double maxTimePoint = double(dataSize - 1);
    if(!this->axisScaleDiv(QwtPlot::xBottom)->contains(maxTimePoint)){
        //int step = maxTimePoint/10;
        int step = int(maxTimePoint/10);
        if (step == 0)
            step = 1;
        this->setAxisScale(QwtPlot::xBottom, 0.0, maxTimePoint, step);
    }
 
    this->setAxisScale(QwtPlot::yLeft, yMin, yMax);

   
    // refresh the plot
//    this->replot();
}

void FrQwtPlotWidget::SetVisibility(unsigned long id, bool visible){
    // get specified curve
    CurvesMap::iterator it = m_Curves.find(id);
    if(it == m_Curves.end()) return;

    it->second->setVisible(visible);

    // not sure we need this
    this->replot();
}

void FrQwtPlotWidget::onPointClicked(const QwtDoublePoint& point){
    // Check
    if (!this->axisScaleDiv(
        QwtPlot::xBottom)->
        contains(point.x())) return;

    // have to move current time point marker
    double x = point.x();
    double floorValue = floor(x);
    double ceilValue = floorValue + 1.0;
    x = (std::abs(x - floorValue) < 0.5) ? floorValue : ceilValue;

    if(m_PlotMarker->value().x() == x){

        x = ((std::abs(point.x()) - std::abs(x)) > 0) ? ceilValue : floorValue;
    }
    this->SetMarkerPosition(int(x));

    // refresh the plot
    this->replot();
    emit pointClicked(point);
}

// added by ohinds 2009-02-06
void FrQwtPlotWidget::onPointMoved(const QwtDoublePoint& point){
    // Check
    if (!this->axisScaleDiv(
        QwtPlot::xBottom)->
        contains(point.x())) return;

    // have to move current time point marker
    int ix = (int) rint(point.x());

    this->SetMarkerPosition(ix);

    // refresh the plot
    this->replot();
    emit pointMoved(point);
}

bool FrQwtPlotWidget::SetMarkerPosition(int timePoint, bool blockSignals){

    double x = double(timePoint);
    double y = 0.0;

    // Check
    if (!this->axisScaleDiv(
        QwtPlot::xBottom)->
        contains(x)) return false;

    m_PlotMarker->setValue(x, y);
    
    this->replot();   // scopic Alan: test 01.05.09, this string causes crushes while getting data from backend
                        // probably qt threading issue 
    if(!blockSignals){

        emit markerPositionChange(int(x));
    }

    return true;
}

void FrQwtPlotWidget::SetNumberOfTimePoints(int num){

    // Add dummy graph if needed
//    if (m_Curves.size() == 0)
//        return;

    if(m_Curves.find(DUMMY_GRAPH_ID) == m_Curves.end()){
        QString dummyGraphName("Dummy");
        QColor dummyGraphColor(PLOT_GRID_COLOR);
        this->AddGraph(
            DUMMY_GRAPH_ID, 
            dummyGraphName, 
            dummyGraphColor); 
    }

    double maxTimePoint = double(num - 1);
    if(!this->axisScaleDiv(QwtPlot::xBottom)->contains(maxTimePoint)){
        // we have to setup dummy data
        //double* xData = new double[num];
        double* yData = new double[num];
        for(int i=0; i < num; ++i){

            //xData[i] = double(i);
            yData[i] = 0.0;
        }
        this->SetData(DUMMY_GRAPH_ID, yData, num);
    }

    //this->SetMarkerPosition(num-1);
}

int FrQwtPlotWidget::GetMaxTimePoint(){

    double result = this->axisScaleDiv(
        QwtPlot::xBottom)->hBound();

    return int(result);
}

