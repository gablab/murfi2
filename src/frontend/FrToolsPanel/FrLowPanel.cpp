#include "FrLowPanel.h"

FrLowPanel::FrLowPanel(QWidget *parent): QWidget(parent){
	setupUi(this);
}

void FrLowPanel::brightnessSliderPositionChanged(){
	lcdBrightness->display(brightnessSlider->value());
	
	// here is the signal changing the current brightness of the scene
	emit brightnessValueChanged(brightnessSlider->value());
}

void FrLowPanel::contrastSliderPositionChanged(){
	lcdContrast->display(contrastSlider->value());
	
	// here is the signal changing the current contras of the scene
	emit contrastValueChanged(contrastSlider->value());
}

void FrLowPanel::thresholdSliderPositionChanged(){
	lcdThreshold->display(thresholdSlider->value());
	
	// here is the signal changing the current threshold of the scene
	emit thresholdValueChanged(thresholdSlider->value());
}

