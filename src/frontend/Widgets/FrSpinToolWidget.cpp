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

    label = new QLabel(this);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(label);
    layout->addWidget(spinBox);
}

void FrSpinToolWidget::SetValue(int value){
    spinBox->setValue(value);
}

void FrSpinToolWidget::SetName(QString name){
    label->setText(name);
}

int FrSpinToolWidget::GetValue(){
    return spinBox->value();
}