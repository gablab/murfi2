#ifndef FR_BASETOOL_WIDGET
#define FR_BASETOOL_WIDGET

#include "Qt/qwidget.h"

class FrBaseToolWidget: public QWidget{
    Q_OBJECT

public:
    FrBaseToolWidget(QWidget* parent=0L);

signals:
     virtual void ParamsChanged();

private Q_SLOTS:


private:

};

#endif
