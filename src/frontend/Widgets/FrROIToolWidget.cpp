#include "FrROIToolWidget.h"

FrROIToolWidget::FrROIToolWidget(QWidget *parent)
: QToolBox(parent){


    connect(this, SIGNAL(currentChanged(int)), this, SLOT(toolChanged(int)));
}

void FrROIToolWidget::toolChanged(int index){
    m_CurrentId = index;
}