#include "FrLayerWidget.h"
#include "FrTabSettingsDocObj.h"
#include "FrRoiDocObj.h"

#include "Qt/qtablewidget.h"
#include "Qt/qboxlayout.h"
#include "Qt/qcheckbox.h"
#include "Qt/qtoolbutton.h"
#include "Qt/qvariant.h"
#include "Qt/qlabel.h"
#include "Qt/qpicture.h"
#include "Qt/qlayoutitem.h"
#include "Qt/qpainter.h"
#include "Qt/qpaintdevice.h"

#define CMTYPE_DEF_ICON "icons\\background.png"
#define CMTYPE_MULTI_ICON "icons\\multi.png"
#define CMTYPE_SINGLE_ICON "icons\\single.png"
#define CMTYPE_ROI_ICON "icons\\roi.png"


FrLayerWidget::FrLayerWidget(FrLayerSettings& params, QWidget *parent)
: QWidget(parent), m_isRoi(false) {    
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

FrLayerWidget::FrLayerWidget(FrRoiDocObj& roiDO, QWidget *parent)
: QWidget(parent), m_isRoi(true) {
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
    
    // Emulate layer params
    FrLayerSettings params;
    InitLayerDefault(&params);
    params.ID = roiDO.GetID();
    params.Name = roiDO.GetName();
    params.Visibility = roiDO.GetVisibility();
    params.Opacity = roiDO.GetOpacity();

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
	params.ColormapSettings.MidValue = m_params.ColormapSettings.MidValue;
	params.ColormapSettings.Threshold	  = m_params.ColormapSettings.Threshold;
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
	m_params.ColormapSettings.MidValue = params.ColormapSettings.MidValue;
    m_params.ColormapSettings.Threshold = params.ColormapSettings.Threshold;
    m_params.ColormapSettings.Color = params.ColormapSettings.Color;

    // Update controls
    layerNameLabel->setText(m_params.Name);
    visibilityCheckBox->setChecked(m_params.Visibility);

    if(m_isRoi){
        picLabel->setPixmap(QPixmap(CMTYPE_ROI_ICON));
    }
    else {
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
}

void FrLayerWidget::OnCheckBoxToggled(){
    m_params.Visibility = visibilityCheckBox->isChecked();
    emit VisibilityChanged(m_params.ID);
}
