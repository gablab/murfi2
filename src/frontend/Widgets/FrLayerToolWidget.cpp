#include "FrLayerToolWidget.h"
#include "FrColormapWidget.h"
#include "FrSpinSliderWidget.h"

#include "Qt/qlabel.h"
#include "Qt/qlayout.h"

FrLayerToolWidget::FrLayerToolWidget(QWidget* parent)
: QWidget(parent){
    // colormap widget
    m_ColormapWidget = new FrColormapWidget(this);

    // opacity widget
    QLabel*	lblOpacity = new QLabel("Opacity: ", this);
    m_OpacityWidget = new FrSpinSliderWidget(this);
    m_OpacityWidget->SetMinMax(0, 100);
    m_OpacityWidget->SetValue(100);

    QHBoxLayout* opacityLayout = new QHBoxLayout();
    opacityLayout->addWidget(lblOpacity);
    opacityLayout->addWidget(m_OpacityWidget);

    // Setup main layout 
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_ColormapWidget);
    mainLayout->addLayout(opacityLayout);
}

