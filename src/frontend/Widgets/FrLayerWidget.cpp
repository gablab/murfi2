#include "FrLayerWidget.h"
#include "FrLayerDocObj.h"

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


FrLayerWidget::FrLayerWidget(FrLayerDocObj* layerDO, QWidget *parent)
: QWidget(parent){    
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
    
    this->SetID(layerDO->GetID());
    this->SetLayerParams(layerDO);

    // Connect signals
    connect(visibilityCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(OnCheckBoxToggled()));
}

void FrLayerWidget::SetLayerParams(FrLayerDocObj* layerDO){
    // Update controls
    layerNameLabel->setText(layerDO->GetSettings()->Name);
    visibilityCheckBox->setChecked(layerDO->GetSettings()->Visibility);
    m_Visibility = layerDO->GetSettings()->Visibility;

    if(layerDO->IsRoi()){
        picLabel->setPixmap(QPixmap(CMTYPE_ROI_ICON));
    }
    else{
        FrColormapLayerSettings* cmlParams = (FrColormapLayerSettings*)layerDO->GetSettings();

        FrColormapSettings::Types cmType = cmlParams->ColormapSettings.Type;
        if(layerDO->GetID() == DEF_LAYER_ID){
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
    this->SetVisibility(visibilityCheckBox->isChecked());
    emit VisibilityChanged(this->GetID());
}
