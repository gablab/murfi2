// class for graphs panel

#ifndef FR_GRAPHPANE_WIDGET
#define FR_GRAPHPANE_WIDGET

// Forward declarations
class FrSpinSliderWidget;
class QComboBox;
class QHBoxLayout;
class QGridLayout;
class QGroupBox;
class QToolBox;
class FrQwtPlotWidget;

// Includes 
#include "FrMacro.h"
#include "FrSettings.h"
#include "Qt/qwidget.h"

class FrGraphPaneWidget : public QWidget {
	Q_OBJECT
public:
	FrGraphPaneWidget(QWidget *parent = 0);

    // Properties
    FrGetPropMacro(FrQwtPlotWidget*, QwtPlotWidget);
    
    // Here all signals
Q_SIGNALS:

private Q_SLOTS:

private:

};

#endif
