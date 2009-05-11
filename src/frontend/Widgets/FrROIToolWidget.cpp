#include "FrROIToolWidget.h"
#include "FrBaseToolWidget.h"
#include "FrEmptyToolWidget.h"
#include "FrSpinToolWidget.h"
#include "FrListToolWidget.h"
#include "FrSpinSliderWidget.h"

#include "Qt/qlayout.h"
#include "Qt/qboxlayout.h"
#include "Qt/qlabel.h"
#include "Qt/qcombobox.h"
#include "Qt/qgroupbox.h"
#include "Qt/qstackedlayout.h"
#include "Qt/qpushbutton.h"


FrROIToolWidget::FrROIToolWidget(QWidget *parent)
: QWidget(parent){
    
    QHBoxLayout* selectionLayout = this->CreateSelectionLayout();
    QGroupBox* gbParams = new QGroupBox(
        QString("Parameters/Description"), this);
    this->InitAdditionalParams(gbParams);
    
    // Init main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(selectionLayout);
    mainLayout->addWidget(gbParams);
   
    this->setFixedHeight(this->sizeHint().height());
}

QHBoxLayout* FrROIToolWidget::CreateSelectionLayout(){
    // create widgets
    m_label = new QLabel(QString("Current tool :"), this);
    m_cmbTool = new QComboBox(this);

    // populate combo
    m_cmbTool->addItem(QString("Information tool"),  QVariant(FrROIToolWidget::Info)); 
    m_cmbTool->addItem(QString("Rectangle tool"),    QVariant(FrROIToolWidget::Rectangle));
    m_cmbTool->addItem(QString("Free shape tool"),   QVariant(FrROIToolWidget::FreeShape));
    m_cmbTool->addItem(QString("Pen tool"),          QVariant(FrROIToolWidget::Pen));
    m_cmbTool->addItem(QString("Sphere tool"),       QVariant(FrROIToolWidget::Sphere));
    m_cmbTool->addItem(QString("Dilate/erode tool"), QVariant(FrROIToolWidget::DilateErode));
    m_cmbTool->addItem(QString("Invert tool"),       QVariant(FrROIToolWidget::Invert));
    m_cmbTool->addItem(QString("Intersect tool"),    QVariant(FrROIToolWidget::Intersect));
    m_cmbTool->addItem(QString("Subtract tool"),     QVariant(FrROIToolWidget::Subtract));
    m_cmbTool->addItem(QString("Union tool"),        QVariant(FrROIToolWidget::Union));
    m_cmbTool->addItem(QString("Copy tool"),         QVariant(FrROIToolWidget::Copy));

    // connect signal
    connect( m_cmbTool, SIGNAL(currentIndexChanged(int)), this, SLOT(OnToolChanged(int)) );
    
    m_clearBtn = new QPushButton(tr("Clear"), this);
    connect( m_clearBtn, SIGNAL(clicked()), this, SLOT(OnClearBtnPressed()) );

    QHBoxLayout* hlayout = new QHBoxLayout();
    hlayout->addWidget(m_cmbTool);
    hlayout->addWidget(m_clearBtn);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(m_label);
    layout->addLayout(hlayout);

    QHBoxLayout* result = new QHBoxLayout();
    result->addLayout(layout);
    result->addStretch();
    return result;
}

void FrROIToolWidget::InitAdditionalParams(QGroupBox* gb){

    // create tools
    m_InfoWidget = new FrEmptyToolWidget(gb);
    m_RectangleWidget = new FrEmptyToolWidget(gb);
    m_FreeShapeWidget = new FrEmptyToolWidget(gb);        
    
    m_PenWidget = new FrSpinToolWidget(gb);
    m_PenWidget->SetName(QString("Size : "));

    m_SphereWidget = new FrSpinToolWidget(gb);
    m_SphereWidget->SetName(QString("Radius : "));

    m_DilateErodeWidget = new FrSpinToolWidget(gb);
    m_DilateErodeWidget->SetName(QString("Voxel amount:"));

    m_InvertWidget = new FrEmptyToolWidget(gb);

    m_IntersectWidget = new FrListToolWidget(gb);
    m_IntersectWidget->SetName(QString("List of ROIs :"));

    m_SubtractWidget = new FrListToolWidget(gb);
    m_SubtractWidget->SetName(QString("List of ROIs :"));

    m_UnionWidget = new FrListToolWidget(gb);
    m_UnionWidget->SetName(QString("List of ROIs :"));

    m_CopyWidget = new FrEmptyToolWidget(gb);
    
    // add tools
    m_layout = new QStackedLayout(gb);
    m_layout->addWidget(m_InfoWidget);
    m_layout->addWidget(m_RectangleWidget);
    m_layout->addWidget(m_FreeShapeWidget);
    m_layout->addWidget(m_PenWidget);
    m_layout->addWidget(m_SphereWidget);
    m_layout->addWidget(m_DilateErodeWidget);
    m_layout->addWidget(m_InvertWidget);
    m_layout->addWidget(m_IntersectWidget);
    m_layout->addWidget(m_SubtractWidget);
    m_layout->addWidget(m_UnionWidget);
    m_layout->addWidget(m_CopyWidget);
}

FrROIToolWidget::ToolType FrROIToolWidget::GetCurrentToolType(){
    int index = m_cmbTool->currentIndex();
    int type = m_cmbTool->itemData(index).toInt();

    return ((FrROIToolWidget::ToolType) type);
}

void FrROIToolWidget::OnToolChanged(int index){
    m_layout->setCurrentIndex(index);
    emit CurrentToolChanged();
}

void FrROIToolWidget::OnToolParamChanged(){
    // TODO: get value of current tool
    int value = 0;
    emit CurrentToolParamChanged(value);
}

void FrROIToolWidget::OnClearBtnPressed(){
    emit ClearCurrentRoiAction();
}

void FrROIToolWidget::UpdateRoiInfo(std::vector<RoiInfo>& roiInfos){
    // get widget
    FrListToolWidget* wgt = 0L;
    switch(this->GetCurrentToolType()){
        case FrROIToolWidget::Union: 
            wgt = m_UnionWidget;
            break;
        case FrROIToolWidget::Subtract: 
            wgt = m_SubtractWidget;
            break;
        case FrROIToolWidget::Intersect: 
            wgt = m_IntersectWidget;
            break;
        default:
           // NOTE: do nothing
            break;
    }

    // update list
    if(wgt){
        wgt->ClearAll();

        std::vector<RoiInfo>::iterator it, itEnd(roiInfos.end());
        for(it = roiInfos.begin(); it != itEnd; ++it){
            wgt->AddListItem(it->ID, it->Name);
        }
    }
}
