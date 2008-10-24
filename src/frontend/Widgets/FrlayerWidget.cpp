#include "FrLayerWidget.h"
#include "FrTabSettingsDocObj.h"

#include "Qt/QTableWidget.h"
#include "Qt/QBoxLayout.h"
#include "Qt/QCheckBox.h"
#include "Qt/QToolButton.h"
#include "Qt/QVariant.h"
#include "Qt/QLabel.h"
#include "Qt/QPicture.h"
#include <QtGui/QSpacerItem>
#include "Qt/QPainter.h"
#include "Qt/QPaintDevice.h"

#define CMTYPE_DEF_ICON "icons\\background.png"
#define CMTYPE_MULTI_ICON "icons\\multi.png"
#define CMTYPE_SINGLE_ICON "icons\\single.png"


FrLayerWidget::FrLayerWidget(FrLayerSettings& params, QWidget *parent)
: QWidget(parent) {
    // Colormap picture
	picLabel = new QLabel("", this);
    // Name of layer
	layerNameLabel = new QLabel("", this);
    // Spacer
	QSpacerItem *spacerItem;
	spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    // Checkbox
	visibilityCheckBox = new QCheckBox(this);
	
	m_layout = new QHBoxLayout(this);
	m_layout->addWidget(picLabel);
	m_layout->addWidget(layerNameLabel);
	m_layout->addItem(spacerItem);
	m_layout->addWidget(visibilityCheckBox);
    
    this->SetLayerParams(params);
    
    // Connect signals
    connect(visibilityCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(OnCheckBoxToggled()));
}

void FrLayerWidget::GetLayerParams(FrLayerSettings& params){
    // copy data
    params.ID         = m_params.ID;
    params.Name       = m_params.Name;
    params.Opacity    = m_params.Opacity;
    params.Visibility = m_params.Visibility;

    params.ColormapSettings.Type      = m_params.ColormapSettings.Type;
    params.ColormapSettings.MinValue  = m_params.ColormapSettings.MinValue;
    params.ColormapSettings.MaxValue  = m_params.ColormapSettings.MaxValue;
    params.ColormapSettings.Threshold = m_params.ColormapSettings.Threshold;
    params.ColormapSettings.Color     = m_params.ColormapSettings.Color;
}

void FrLayerWidget::SetLayerParams(FrLayerSettings& params){
    // copy data
    m_params.ID = params.ID;
    m_params.Name = params.Name;
    m_params.Opacity = params.Opacity;
    m_params.Visibility = params.Visibility;
    m_params.ColormapSettings.Type = params.ColormapSettings.Type;
    m_params.ColormapSettings.MinValue = params.ColormapSettings.MinValue;
    m_params.ColormapSettings.MaxValue = params.ColormapSettings.MaxValue;
    m_params.ColormapSettings.Threshold = params.ColormapSettings.Threshold;
    m_params.ColormapSettings.Color = params.ColormapSettings.Color;

    // Update controls
    layerNameLabel->setText(m_params.Name);
    visibilityCheckBox->setChecked(m_params.Visibility);

    enum FrColormapSettings::Type cmType = m_params.ColormapSettings.Type;
    if(m_params.ID == DEFAULT_LAYER_ID){
            picLabel->setPixmap(QPixmap(CMTYPE_DEF_ICON));
    }
    else if(cmType == FrColormapSettings::MultiColor) {
        picLabel->setPixmap(QPixmap(CMTYPE_MULTI_ICON));
    }
    else if(cmType == FrColormapSettings::SingleColor){
        picLabel->setPixmap(QPixmap(CMTYPE_SINGLE_ICON));
    }
}

void FrLayerWidget::OnCheckBoxToggled(){
    m_params.Visibility = visibilityCheckBox->isChecked();
    emit VisibilityChanged(m_params.ID);
}