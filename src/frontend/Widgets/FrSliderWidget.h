// class for storing bookmark info

#ifndef FR_SLIDER_WIDGET
#define FR_SLIDER_WIDGET

// Forward declarations
class QLabel;
class QSpinBox;
class QSlider;
class QHBoxLayout;

// Includes 
#include "FrMacro.h"

#include "Qt/qstring.h"
#include "Qt/qwidget.h"

class FrSliderWidget : public QWidget {
    Q_OBJECT
public:
    FrSliderWidget(QWidget *parent = 0);

    // Modifiers
    void SetMinMax(int min, int max);
    void SetValue(int value);

    // Accessors
    int GetValue();
    int GetMinimum();
    int GetMaximum();

    // add signals here
signals:
    void ValueChanged(int newValue);

private Q_SLOTS:
    void OnSliderValueChanged(int newValue);

private:
    QSlider*    m_slider;
    QHBoxLayout* m_layout;
};

#endif
