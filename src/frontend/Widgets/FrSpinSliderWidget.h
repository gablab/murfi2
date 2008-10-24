// class for storing bookmark info

#ifndef FR_SPINSLIDER_WIDGET
#define FR_SPINSLIDER_WIDGET

// Forward declarations
class QLabel;
class QSpinBox;
class QSlider;
class QHBoxLayout;

// Includes 
#include "FrMacro.h"

#include "Qt/QString.h"
#include "QtGui/QWidget"

class FrSpinSliderWidget : public QWidget {
	Q_OBJECT
public:
	FrSpinSliderWidget(QWidget *parent = 0);
    
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
    void OnSpinValueChanged(int newValue);
    void OnSliderValueChanged(int newValue);

private:
    QSpinBox*   m_spin;
    QSlider*    m_slider;
    QHBoxLayout* m_layout;
};

#endif