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


class FrQwtPlotWidget : public QwtPlot {
	Q_OBJECT
public:
	FrQwtPlotWidget(QWidget *parent = 0);

    // Properties

    
    // Here all signals
signals:

private:

private Q_SLOTS:

private:

};

#endif
