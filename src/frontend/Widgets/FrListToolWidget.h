#ifndef FR_LISTTOOL_WIDGET
#define FR_LISTTOOL_WIDGET

#include "FrBaseToolWidget.h"

#include "Qt/qwidget.h"
#include "Qt/qstring.h"

class QSpinBox;
class QLabel;

class FrListToolWidget: public FrBaseToolWidget{
    Q_OBJECT

public:
    FrListToolWidget(QWidget* parent);

    //int GetValue();
    //void SetName(QString name);
    //void SetValue(int value);

signals:

private Q_SLOTS:


private:
    QSpinBox* spinBox;
    QLabel* label;

};

#endif