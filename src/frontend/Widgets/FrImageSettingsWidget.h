// class for image settings control

#ifndef FR_IMAGESETTINGS_WIDGET
#define FR_IMAGESETTINGS_WIDGET

#include "Qt/qwidget.h"
#include "Qt/qmutex.h"
#include "FrLayerSettings.h"
#include "FrMacro.h"

class FrMainDocument;
class FrSpinSliderWidget;
class QLabel;
class QGroupBox;

class FrImageSettingsWidget: public QWidget {
	Q_OBJECT

public:
	FrImageSettingsWidget(QWidget *parent = 0, FrMainDocument* doc = 0);
    
    FrTbcSettings GetTbcSettings();
    int GetSliceNumber(int widget);
    void SetTbcSettings(FrTbcSettings params);
    void SetSliceNumber(int widget, int slice);

    void Update();

    FrSetPropMacro(FrMainDocument*, Document);

Q_SIGNALS:
    void ImageParamsChanged();
    void UpdateSignal();

private Q_SLOTS:
    void OnSpinSliderValueChanged(int value);
    void OnUpdate();

private:
    FrSpinSliderWidget* m_sliceWidget;
    FrSpinSliderWidget* m_coronalSliceWidget;
    FrSpinSliderWidget* m_sagitalSliceWidget;
    FrSpinSliderWidget* m_axialSliceWidget;

    QLabel* sliceLabel;
    QLabel* coronalLabel;
    QLabel* sagitalLabel;
    QLabel* axialLabel;

    QGroupBox* sliceGroupBox;

    FrSpinSliderWidget* m_threshWidget;
    FrSpinSliderWidget* m_brightWidget;
    FrSpinSliderWidget* m_contWidget;

    QMutex mutex;
    
};

#endif
