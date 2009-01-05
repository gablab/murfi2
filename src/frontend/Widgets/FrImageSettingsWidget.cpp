#include "FrImageSettingsWidget.h"
#include "FrSpinSliderWidget.h"
#include "FrMainDocument.h"
#include "FrViewDocObj.h"
#include "FrImageDocObj.h"
#include "FrLayerDocObj.h"

// backend 
#include "RtMRIImage.h"

//Qt stuff
#include "Qt/qstring.h"
#include "Qt/qlabel.h"
#include "Qt/qpushbutton.h"
#include "Qt/qboxlayout.h"
#include "Qt/qgroupbox.h"
#include "Qt/qapplication.h"


FrImageSettingsWidget::FrImageSettingsWidget(QWidget* parent, FrMainDocument* doc) 
: QWidget(parent), m_Document(doc) {
    // slice number
    sliceGroupBox = new QGroupBox("Slice", this);

    sliceLabel = new QLabel(QString("Slice: "), sliceGroupBox); 

    m_sliceWidget = new FrSpinSliderWidget(sliceGroupBox);
    m_sliceWidget->SetMinMax(0, 100);
    m_sliceWidget->SetValue(0);
    connect(m_sliceWidget, SIGNAL(ValueChanged(int)),
            this, SLOT(OnSpinSliderValueChanged(int)));

    // ortho slice numbers
    coronalLabel = new QLabel(QString("Coronal: "), sliceGroupBox); 

    m_coronalSliceWidget = new FrSpinSliderWidget(sliceGroupBox);
    m_coronalSliceWidget->SetMinMax(0, 100);
    m_coronalSliceWidget->SetValue(0);
    connect(m_coronalSliceWidget, SIGNAL(ValueChanged(int)),
            this, SLOT(OnSpinSliderValueChanged(int)));

    sagitalLabel = new QLabel(QString("Sagital: "), sliceGroupBox); 

    m_sagitalSliceWidget = new FrSpinSliderWidget(sliceGroupBox);
    m_sagitalSliceWidget->SetMinMax(0, 100);
    m_sagitalSliceWidget->SetValue(0);
    connect(m_sagitalSliceWidget, SIGNAL(ValueChanged(int)),
            this, SLOT(OnSpinSliderValueChanged(int)));

    axialLabel = new QLabel(QString("Axial: "), sliceGroupBox); 

    m_axialSliceWidget = new FrSpinSliderWidget(sliceGroupBox);
    m_axialSliceWidget->SetMinMax(0, 100);
    m_axialSliceWidget->SetValue(0);
    connect(m_axialSliceWidget, SIGNAL(ValueChanged(int)),
            this, SLOT(OnSpinSliderValueChanged(int)));

    QVBoxLayout* slayout = new QVBoxLayout();
    slayout->addWidget(sliceLabel);
    slayout->addWidget(coronalLabel);
    slayout->addWidget(sagitalLabel);
    slayout->addWidget(axialLabel);
    
    QVBoxLayout* slayout2 = new QVBoxLayout();
    slayout2->addWidget(m_sliceWidget);
    slayout2->addWidget(m_coronalSliceWidget);
    slayout2->addWidget(m_sagitalSliceWidget);
    slayout2->addWidget(m_axialSliceWidget);

    QHBoxLayout* slayout3 = new QHBoxLayout(sliceGroupBox);
    slayout3->addLayout(slayout);
    slayout3->addLayout(slayout2);

    // tbc settings
    QGroupBox* groupBox = new QGroupBox("Tbc Settings", this);

    
    QLabel* label1 = new QLabel(QString("Threshold: "), groupBox); 

    m_threshWidget = new FrSpinSliderWidget(groupBox);
    m_threshWidget->SetMinMax(0, 100);
    m_threshWidget->SetValue(0);
    connect(m_threshWidget, SIGNAL(ValueChanged(int)),
            this, SLOT(OnSpinSliderValueChanged(int)));
    
    QLabel* label2 = new QLabel(QString("Brightness: "), groupBox); 

    m_brightWidget = new FrSpinSliderWidget(groupBox);
    m_brightWidget->SetMinMax(0, 100);
    m_brightWidget->SetValue(0);
    connect(m_brightWidget, SIGNAL(ValueChanged(int)),
            this, SLOT(OnSpinSliderValueChanged(int)));

    QLabel* label3 = new QLabel(QString("Contrast: "), groupBox); 

    m_contWidget = new FrSpinSliderWidget(groupBox);
    m_contWidget->SetMinMax(0, 100);
    m_contWidget->SetValue(0);
    connect(m_contWidget, SIGNAL(ValueChanged(int)),
            this, SLOT(OnSpinSliderValueChanged(int)));
    
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(label1);
    layout->addWidget(label2);
    layout->addWidget(label3);
    
    QVBoxLayout* layout2 = new QVBoxLayout();
    layout2->addWidget(m_threshWidget);
    layout2->addWidget(m_brightWidget);
    layout2->addWidget(m_contWidget);

    QHBoxLayout* layout3 = new QHBoxLayout(groupBox);
    layout3->addLayout(layout);
    layout3->addLayout(layout2);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(sliceGroupBox);
    mainLayout->addWidget(groupBox);
}

void FrImageSettingsWidget::OnSpinSliderValueChanged(int value){
    emit ImageParamsChanged();
}

FrTbcSettings FrImageSettingsWidget::GetTbcSettings(){
    FrTbcSettings params;
    params.Threshold = (double)m_threshWidget->GetValue()/100.0;
    params.Brightness = (double)m_brightWidget->GetValue()/100.0;
    params.Contrast = (double)m_contWidget->GetValue()/100.0;

    return params;
}
 
int FrImageSettingsWidget::GetSliceNumber(int widget){
    int slice = 0;

    switch (widget){
        case 0:
            slice = m_coronalSliceWidget->GetValue();
            break;
        case 1:
            slice = m_sagitalSliceWidget->GetValue();
            break;
        case 2:
            slice = m_axialSliceWidget->GetValue();
            break;
        case 3:
            slice = m_sliceWidget->GetValue();
            break;
    }

    return slice;
}

void FrImageSettingsWidget::SetTbcSettings(FrTbcSettings params){
    m_threshWidget->SetValue(params.Threshold*100);
    m_brightWidget->SetValue(params.Brightness*100);
    m_contWidget->SetValue(params.Contrast*100);
}

void FrImageSettingsWidget::SetSliceNumber(int widget, int slice){
    switch (widget){
        case 0:
            m_coronalSliceWidget->SetValue(slice);
            break;
        case 1:
            m_sagitalSliceWidget->SetValue(slice);
            break;
        case 2:
            m_axialSliceWidget->SetValue(slice);
            break;
        case 3:
            m_sliceWidget->SetValue(slice);
            break;
    }
}

// Update info displayed by widget
void FrImageSettingsWidget::Update(){
    if(!m_Document) return;

    // TODO: set TBC settings from current layer

    // Get selected layer ID 
    FrViewDocObj* viewDO = m_Document->GetCurrentViewObject();
    if(viewDO == 0) return;

    // get image dimensions
    // Find appropriate image volume
    RtMRIImage* mri = 0;
    FrDocument::DocObjCollection images;
    m_Document->GetObjectsByType(images, FrDocumentObj::ImageObject);
    
    if (images.size() == 0) return;

    FrImageDocObj* imgDO = (FrImageDocObj*)images[0];
    mri = imgDO->GetTimePointData(0);   // any timepoint is good
    
    int xDim = mri->getDim(0)-1;
    int yDim = mri->getDim(1)-1;
    int zDim = mri->getDim(2)-1;

    m_sliceWidget->SetMinMax(0, zDim);
    m_coronalSliceWidget->SetMinMax(0, yDim);
    m_sagitalSliceWidget->SetMinMax(0, xDim);
    m_axialSliceWidget->SetMinMax(0, zDim);

    Views view = viewDO->GetActiveView();
    switch(view){
        case SliceView:
            sliceGroupBox->setVisible(true);

            m_coronalSliceWidget->setVisible(false);
            m_sagitalSliceWidget->setVisible(false);
            m_axialSliceWidget->setVisible(false);
            
            coronalLabel->setVisible(false);
            sagitalLabel->setVisible(false);
            axialLabel->setVisible(false);
            
            sliceLabel->setVisible(true);
            m_sliceWidget->setVisible(true);
            
            SetSliceNumber(3, viewDO->GetSliceViewSettings()->SliceNumber);
            break;
        case MosaicView:
            // hide all slice controls
            sliceGroupBox->setVisible(false);
            break;
        case OrthoView:
            sliceGroupBox->setVisible(true);
            
            m_sliceWidget->setVisible(false);
            sliceLabel->setVisible(false);

            m_coronalSliceWidget->setVisible(true);
            m_sagitalSliceWidget->setVisible(true);
            m_axialSliceWidget->setVisible(true);

            coronalLabel->setVisible(true);
            sagitalLabel->setVisible(true);
            axialLabel->setVisible(true);

            for(int i=0; i < ORTHO_VIEWS_CNT; ++i){
                SetSliceNumber(i, viewDO->GetOrthoViewSettings()->SliceNumber[i]);
            }
            break;
    }

    int layerID = viewDO->GetActiveLayerID();
    
    FrLayerDocObj* layerDO = m_Document->GetLayerDocObjByID(layerID);
    if (!layerDO) return;

    FrImageLayerSettings* ls = (FrImageLayerSettings*)layerDO->GetSettings();
    this->SetTbcSettings(ls->TbcSettings);
}