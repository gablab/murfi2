// class for storing bookmark info

#ifndef FR_COLORMAP_WIDGET
#define FR_COLORMAP_WIDGET

// Forward declarations
class FrImageLayerSettings;
class FrSpinSliderWidget;
class QComboBox;
class QHBoxLayout;
class QGridLayout;
class QGroupBox;

// Includes 
#include "FrMacro.h"
#include "FrSettings.h"
#include "FrLayerSettings.h"
#include "Qt/qwidget.h"

class FrColormapWidget : public QWidget {
	Q_OBJECT
public:
	FrColormapWidget(QWidget *parent = 0);

    void GetColormapParams(FrImageLayerSettings& params);
    void SetColormapParams(FrImageLayerSettings& params);
    void BlockSignals(bool value){
        m_isBlocked = value;
    }

    // Here all signals
signals:
    void ParamsChanged();

private:
    QHBoxLayout* CreateTitleLayout();
    QGridLayout* CreateMainParamsLayout();
    QGroupBox* CreateSingleParams();
    QGroupBox* CreateMultiParams();

    void UpdateColorWidget(QColor& color);

private Q_SLOTS:
    void OnComboBoxChange(int);
    void OnSpinSliderValueChanged(int);
    void OnBtnColorClicked();

private:
    // Manage vars
    bool m_isBlocked;

    // Main params widgets 
    QComboBox*  m_cmbType;
    FrSpinSliderWidget* m_minWidget;
    FrSpinSliderWidget* m_maxWidget;

    // additional params
    QGroupBox* m_gbSingle;
    QGroupBox* m_gbMulti;
    QWidget* m_wgtColor;
    FrSpinSliderWidget* m_midWidget;
    FrSpinSliderWidget* m_threshWidget;

    // Params
    QColor m_color;
    FrColormapSettings::Types m_cmType;

};

#endif
