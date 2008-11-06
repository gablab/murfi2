#ifndef FR_ROITOOL_WIDGET
#define FR_ROITOOL_WIDGET

#include "FrMacro.h"

#include "Qt/qtoolbox.h"

class FrROIToolWidget: public QToolBox{
    Q_OBJECT
public:
    FrROIToolWidget(QWidget *parent);

    FrPropMacro(int, CurrentId);     

signals:

private Q_SLOTS:
    void toolChanged(int index);

private:
    

};


#endif