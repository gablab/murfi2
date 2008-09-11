#include "FrLowPanel.h"

FrLowPanel::FrLowPanel(QWidget *parent): QWidget(parent){
	setupUi(this);
 
    connect(brightnessSlider, SIGNAL(valueChanged(int)), 
            this, SLOT(brightnessSliderPositionChanged()));
	connect(contrastSlider,   SIGNAL(valueChanged(int)), 
            this, SLOT(contrastSliderPositionChanged()));
	connect(thresholdSlider,  SIGNAL(valueChanged(int)), 
            this, SLOT(thresholdSliderPositionChanged()));

    thresholdSlider->setValue(0);
}

void FrLowPanel::brightnessSliderPositionChanged(){
    int value = brightnessSlider->value();
	lcdBrightness->display(value);
	
	// here is the signal changing the current brightness of the scene
//	emit brightnessValueChanged(brightnessSlider->value());
}

void FrLowPanel::contrastSliderPositionChanged(){
    int value = contrastSlider->value();
	lcdContrast->display(value);
    	
	// here is the signal changing the current contras of the scene
//	emit contrastValueChanged(contrastSlider->value());
}

void FrLowPanel::thresholdSliderPositionChanged(){
    int value = thresholdSlider->value();
	lcdThreshold->display(value);
	
	// here is the signal changing the current threshold of the scene
//	emit thresholdValueChanged(thresholdSlider->value());
}

int FrLowPanel::GetThresholdValue(){
    return thresholdSlider->value();
}

int FrLowPanel::GetBrightnessValue(){
    return brightnessSlider->value();
}

int FrLowPanel::GetContrastValue(){
    return contrastSlider->value();
}

void FrLowPanel::SetBrightnessValue(double value){
	brightnessSlider->setValue(value*100);
}

void FrLowPanel::SetContrastValue(double value){
	contrastSlider->setValue(value*100);
}