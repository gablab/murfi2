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


class FrQwtPlotWidget : public QwtPlot {
	Q_OBJECT
public:
	FrQwtPlotWidget(QWidget *parent = 0);
    ~FrQwtPlotWidget();
    
    void AddGraph(int id, QString& name, QColor& color);
    void RemoveGraph(int id);
    void RemoveAll();

    void SetData(int id, double data[], int dataSize);
    void SetVisibility(int id, bool visible);
    bool SetMarkerPosition(int timePoint, bool blockSignals = false);
    
    int GetMaxTimePoint();
    void SetNumberOfTimePoints(int num);
        
    // Here all signals
signals:
    void pointClicked(QPointF p);
    void markerPositionChange(int position);
        
private:

private Q_SLOTS:
    void onPointClicked(const QwtDoublePoint& point);

private:
    typedef std::map<int, QwtPlotCurve*> CurvesMap;
    CurvesMap m_Curves;

    QwtPlotGrid* m_Grid;
    QwtPlotMarker* m_PlotMarker;
    
};

#endif
