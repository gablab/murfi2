#ifndef FR_EMTYTOOL_WIDGET
#define FR_EMTYTOOL_WIDGET

#include "FrBaseToolWidget.h"

#include "Qt/qwidget.h"
#include "Qt/qstring.h"

class QSpinBox;
class QLabel;

class FrSpinToolWidget: public FrBaseToolWidget{
    Q_OBJECT

public:
    FrSpinToolWidget(QWidget* parent);

    int GetValue();
    void SetName(QString name);
    void SetValue(int value);

signals:

private Q_SLOTS:


private:
    QSpinBox* spinBox;
    QLabel* label;

};

#endif