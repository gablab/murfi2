#ifndef FR_LAYERTOOL_WIDGET
#define FR_LAYERTOOL_WIDGET

#include "FrMacro.h"

#include "Qt/qwidget.h"

class FrColormapWidget;
class FrSpinSliderWidget;

class FrLayerToolWidget: public QWidget{
    Q_OBJECT
public:
    FrLayerToolWidget(QWidget *parent);

    FrGetPropMacro(FrColormapWidget*, ColormapWidget);
    FrGetPropMacro(FrSpinSliderWidget*, OpacityWidget);

signals:

private Q_SLOTS:

private:

};


#endif