// class for graphs panel

#ifndef FR_GRAPHPANE_WIDGET
#define FR_GRAPHPANE_WIDGET

// Forward declarations
class QHBoxLayout;
class FrQwtPlotWidget;
class FrGraphListWidget;
class FrPlayControlWidget;

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
    FrGetPropMacro(FrGraphListWidget*, GraphListWidget);
    FrGetPropMacro(FrPlayControlWidget*, PlayControlWidget);
    
    // Methods
    void UpdateTimePoint();

    // Here all signals
Q_SIGNALS:

private Q_SLOTS:

};

#endif
