#include "FrSpinToolWidget.h"

#include "Qt/qspinbox.h"
#include "Qt/qlabel.h"
#include "Qt/qlayout.h"

#define DEF_MIN_VALUE 0
#define DEF_MAX_VALUE 100


FrSpinToolWidget::FrSpinToolWidget(QWidget* parent)
: FrBaseToolWidget(parent){
    spinBox = new QSpinBox(this);
    spinBox->setMinimum(DEF_MIN_VALUE);
    spinBox->setMaximum(DEF_MAX_VALUE);
    connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));

    label = new QLabel("test", this);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(label);
    layout->addWidget(spinBox);
}

void FrSpinToolWidget::SetValue(int value){
    spinBox->setValue(value);
}

void FrSpinToolWidget::SetMinMaxValues(int min, int max){
    if(min < max){
        spinBox->setMinimum(min);
        spinBox->setMaximum(max);
    }
}

void FrSpinToolWidget::SetName(QString name){
    label->setText(name);
}

int FrSpinToolWidget::GetValue(){
    return spinBox->value();
}

void FrSpinToolWidget::spinBoxValueChanged(int value){
    emit ParamsChanged();
}
