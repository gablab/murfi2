// class for drawing panel for ROI

#ifndef FR_DRAWINGPANEL_WIDGET
#define FR_DRAWINGPANEL_WIDGET

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
#include "QtGui/QWidget"

class FrDrawingPanelWidget : public QWidget {
	Q_OBJECT
public:
	FrDrawingPanelWidget(QWidget *parent = 0);

    // Here all signals
signals:

private:

private Q_SLOTS:

private:

};

#endif