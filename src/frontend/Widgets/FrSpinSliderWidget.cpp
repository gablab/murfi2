#include "FrSpinSliderWidget.h"
#include "FrUtils.h"

//Qt stuff
#include "Qt/qlabel.h"
#include "Qt/qspinbox.h"
#include "Qt/qslider.h"
#include "Qt/qboxlayout.h"

// Some defines 
#define DEF_MIN_VALUE 0
#define DEF_MAX_VALUE 100
#define DEF_DEF_VALUE 0

FrSpinSliderWidget::FrSpinSliderWidget(QWidget* parent)
: QWidget(parent){
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    m_spin = new QSpinBox(parent);
    m_spin->setMinimum(DEF_MIN_VALUE);
    m_spin->setMinimum(DEF_MIN_VALUE);

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
    m_layout->addWidget(m_spin);
    m_layout->addWidget(m_slider);

    // Connect signals
    connect(m_spin, SIGNAL(valueChanged(int)), this, SLOT(OnSpinValueChanged(int)));
    connect(m_slider, SIGNAL(valueChanged(int)), this, SLOT(OnSliderValueChanged(int)));
}

void FrSpinSliderWidget::SetMinMax(int min, int max){
    if(min >= max) return;

    m_spin->setMinimum(min);
    m_slider->setMinimum(min);
    m_spin->setMaximum(max);
    m_slider->setMaximum(max);
}

void FrSpinSliderWidget::SetValue(int value){
    int min = m_spin->minimum();
    int max = m_spin->maximum();
    value = ClampValue(value, min, max);
    
    m_spin->blockSignals(true);
    m_spin->setValue(value);
    m_spin->blockSignals(false);

    m_slider->blockSignals(true);
    m_slider->setValue(value);
    m_slider->blockSignals(false);
}

int FrSpinSliderWidget::GetValue(){
    return m_spin->value();
}

int FrSpinSliderWidget::GetMinimum(){
    return m_spin->minimum();
}

int FrSpinSliderWidget::GetMaximum(){
    return m_spin->maximum();
}

void FrSpinSliderWidget::OnSpinValueChanged(int value){
    m_slider->blockSignals(true);
    m_slider->setValue(value);
    m_slider->blockSignals(false);

    emit ValueChanged(value);
}

void FrSpinSliderWidget::OnSliderValueChanged(int value){
    m_spin->blockSignals(true);
    m_spin->setValue(value);
    m_spin->blockSignals(false);

    emit ValueChanged(value);
}
