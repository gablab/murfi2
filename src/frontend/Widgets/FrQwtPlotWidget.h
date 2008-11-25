// class for qwt plot widget

#ifndef FR_QWTPLOT_WIDGET
#define FR_QWTPLOT_WIDGET

// Forward declarations
class FrSpinSliderWidget;
class QComboBox;
class QHBoxLayout;
class QGridLayout;
class QGroupBox;
class QToolBox;

// Includes 
#include "FrMacro.h"
#include "FrSettings.h"
#include "Qt/qwidget.h"

#include "qwt_plot.h"
#include "qwt_plot_curve.h"

#include <vector>

// forward declarations
class QwtPlotMarker;


class FrQwtPlotWidget : public QwtPlot {
	Q_OBJECT
public:
	FrQwtPlotWidget(QWidget *parent = 0);
    ~FrQwtPlotWidget();

    void AddGraph();                // FrGraph
    void RemoveGraph(int id);
    void SetData(int id);
    void SetVisibility(int id, bool visible);

    void RemoveAll();

    // Properties
    
    
    // Here all signals
signals:
    void pointClicked(QPointF p);
        
private:

private Q_SLOTS:
    void test(const QwtDoublePoint& point);
    void test2(double x, double y);

private:
    std::vector<QwtPlotCurve*> m_Curves;    
    QwtPlotMarker* m_PlotMarker;

};

#endif
