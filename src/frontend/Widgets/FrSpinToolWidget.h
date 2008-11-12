#ifndef FR_SPINTOOL_WIDGET
#define FR_SPINTOOL_WIDGET

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
    virtual void ParamsChanged();

private Q_SLOTS:
    void spinBoxValueChanged(int value);

private:
    QSpinBox* spinBox;
    QLabel* label;

};

#endif