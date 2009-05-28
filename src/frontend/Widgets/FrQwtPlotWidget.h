// class for qwt plot widget

#ifndef FR_QWTPLOT_WIDGET
#define FR_QWTPLOT_WIDGET

// Forward declarations
class QString;
class QColor;

// Includes 
#include "FrMacro.h"
#include "FrSettings.h"
#include "Qt/qwidget.h"

#include "qwt_plot.h"
#include "qwt_plot_curve.h"

#include <map>

// forward declarations
class QwtPlotMarker;
class QwtPlotGrid;
class QwtPlotPicker;

class FrQwtPlotWidget : public QwtPlot {
    Q_OBJECT
public:
    FrQwtPlotWidget(QWidget *parent = 0);
    virtual ~FrQwtPlotWidget();
    
    void AddGraph(unsigned long id, QString& name, QColor& color);
    void RemoveGraph(unsigned long id);
    void RemoveAll();

    void SetData(unsigned long id, double data[], int dataSize);
    void SetVisibility(unsigned long id, bool visible);
    bool SetMarkerPosition(int timePoint, bool blockSignals = false);
    
    int GetMaxTimePoint();
    void SetNumberOfTimePoints(int num);
        
    // Here all signals
signals:
    void pointClicked(QPointF p);
    void pointMoved(QPointF p);
    void markerPositionChange(int position);
        
private:

private Q_SLOTS:
    void onPointClicked(const QwtDoublePoint& point);
    void onPointMoved(const QwtDoublePoint& point);

private:
    typedef std::map<int, QwtPlotCurve*> CurvesMap;
    CurvesMap m_Curves;

    QwtPlotGrid* m_Grid;
    QwtPlotMarker* m_PlotMarker;
    QwtPlotPicker *m_PlotPicker;

    double yMin;
    double yMax;
    
};

#endif
