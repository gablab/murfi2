#ifndef FR_EMTYTOOL_WIDGET
#define FR_EMTYTOOL_WIDGET

#include "FrBaseToolWidget.h"

#include "Qt/qwidget.h"

class FrEmptyToolWidget: public FrBaseToolWidget{
    Q_OBJECT

public:
    FrEmptyToolWidget(QWidget* parent=0L);

private:

};

#endif
