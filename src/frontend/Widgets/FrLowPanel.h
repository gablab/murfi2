// class for panel at the bottom of the 2D image tab

#ifndef FR_LOWPANEL
#define FR_LOWPANEL

#include "ui_FrLowPanel.h"

class FrLowPanel: public QWidget, public Ui::LowPanel
{
	Q_OBJECT
public:
	FrLowPanel(QWidget *parent = 0);

    int GetThresholdValue();
    int GetBrightnessValue();
    int GetContrastValue();
	void SetBrightnessValue(double value);
	void SetContrastValue(double value);

public slots:
	void brightnessSliderPositionChanged();
	void contrastSliderPositionChanged();
	void thresholdSliderPositionChanged();

signals:
//	void brightnessValueChanged(int value);
//	void contrastValueChanged(int value);
//	void thresholdValueChanged(int value);


protected:


private:


};

#endif