#include "FrSliderWidget.h"
#include "FrUtils.h"

//Qt stuff
#include "Qt/qlabel.h"
#include "Qt/qslider.h"
#include "Qt/qboxlayout.h"

// Some defines 
#define DEF_MIN_VALUE 0
#define DEF_MAX_VALUE 100
#define DEF_DEF_VALUE 0

FrSliderWidget::FrSliderWidget(QWidget* parent)
: QWidget(parent){
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    m_slider = new QSlider(parent);
    m_slider->setMinimum(DEF_MIN_VALUE);
    m_slider->setMaximum(DEF_MAX_VALUE);
    m_slider->setValue(DEF_DEF_VALUE);
    m_slider->setOrientation(Qt::Horizontal);

    m_layout = new QHBoxLayout(this);
    m_layout->setSizeConstraint(QLayout::SetMinimumSize);
#if QT_VERSION >= 0x040300
    m_layout->setContentsMargins(0, 0, 0, 0);
#else
    m_layout->setMargin(0);
#endif
    m_layout->addWidget(m_slider);

    // Connect signals
    connect(m_slider, SIGNAL(valueChanged(int)), this, SLOT(OnSliderValueChanged(int)));
    this->setFixedHeight(this->sizeHint().height());
}

void FrSliderWidget::SetMinMax(int min, int max){
    if(min >= max) return;

    m_slider->setMinimum(min);
    m_slider->setMaximum(max);
}

void FrSliderWidget::SetValue(int value){
    int min = m_slider->minimum();
    int max = m_slider->maximum();
    value = ClampValue(value, min, max);

    m_slider->blockSignals(true);
    m_slider->setValue(value);
    m_slider->blockSignals(false);
}

int FrSliderWidget::GetValue(){
    return m_slider->value();
}

int FrSliderWidget::GetMinimum(){
    return m_slider->minimum();
}

int FrSliderWidget::GetMaximum(){
    return m_slider->maximum();
}

void FrSliderWidget::OnSliderValueChanged(int value){
    emit ValueChanged(value);
}
