#include "FrLayerListWidget.h"
#include "FrTabSettingsDocObj.h"
#include "FrLayerWidget.h"
#include "FrColormapWidget.h"
#include "FrSpinSliderWidget.h"
#include "FrLayerToolWidget.h"
#include "FrROIToolWidget.h"
#include "FrRoiDocObj.h"

#include "Qt/qtablewidget.h"
#include "Qt/qboxlayout.h"
#include "Qt/qcheckbox.h"
#include "Qt/qtoolbutton.h"
#include "Qt/qvariant.h"
#include "Qt/qlabel.h"
#include "Qt/qlayoutitem.h"

#define TAB_ID_IDX          0
#define TAB_LAYER_IDX       1
#define COLUMN_COUNT        2
#define DEFAULT_LAYER_ID    0

#define MULTICOLOR_ITEM_IDX     0
#define SINGLECOLOR_ITEM_IDX    1

#define INSERT_ROW_NUM 0

FrLayerListWidget::FrLayerListWidget(QWidget *parent)
: QWidget(parent), m_signalsBlocked(false) {    

    // Create table widget
    m_layerTable = new QTableWidget(this);
    m_layerTable->setShowGrid(false);
    m_layerTable->setRowCount(0);
    m_layerTable->setColumnCount(2);
    m_layerTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_layerTable->setSelectionMode(QAbstractItemView::SingleSelection);

    QTableWidgetItem* twi = new QTableWidgetItem("Id");
    m_layerTable->setHorizontalHeaderItem(TAB_ID_IDX, twi);
    m_layerTable->setColumnWidth(TAB_ID_IDX, 50);
    m_layerTable->hideColumn(TAB_ID_IDX);

    twi = new QTableWidgetItem("Layers");
    m_layerTable->setHorizontalHeaderItem(TAB_LAYER_IDX, twi);
    m_layerTable->setColumnWidth(TAB_LAYER_IDX, 150);

    // create block of buttons
    QWidget* btnBlock = new QWidget(this);
    m_btnAdd = new QToolButton(btnBlock);
    m_btnAdd->setText("+");
    m_btnAdd->setToolTip("Add new layer");
    m_btnDelete = new QToolButton(btnBlock);
    m_btnDelete->setText("x");
    m_btnDelete->setToolTip("Delete selected layer");
    m_btnChange = new QToolButton(btnBlock);
    m_btnChange->setText("C");
    m_btnChange->setToolTip("Change selected layer");

    QHBoxLayout* btnLayout = new QHBoxLayout(btnBlock);
    btnLayout->addWidget(m_btnAdd);
    btnLayout->addWidget(m_btnDelete);
    btnLayout->addWidget(m_btnChange);
    btnLayout->addStretch();
    btnBlock->setLayout(btnLayout);

    // setup layer layout (leftmost)
    QVBoxLayout* layerLayout = new QVBoxLayout();
    layerLayout->addWidget(m_layerTable);
    layerLayout->addWidget(btnBlock);

    //// colormap widget
    //m_colormapWidget = new FrColormapWidget(this);

    //// opacity widget
    //QLabel*	lblOpacity = new QLabel("Opacity: ", this);
    //m_opacityWidget = new FrSpinSliderWidget(this);
    //m_opacityWidget->SetMinMax(0, 100);
    //m_opacityWidget->SetValue(100);

    //QHBoxLayout* opacityLayout = new QHBoxLayout();
    //opacityLayout->addWidget(lblOpacity);
    //opacityLayout->addWidget(m_opacityWidget);

    //// spacer
    ////QSpacerItem *spacerItem;
    ////spacerItem = new QSpacerItem(40, 40, QSizePolicy::Fixed, QSizePolicy::Expanding);

    //// Setup property layout (rightmost)
    //QVBoxLayout* propLayout = new QVBoxLayout();
    //propLayout->addWidget(m_colormapWidget);
    //propLayout->addLayout(opacityLayout);
    ////propLayout->addItem(spacerItem);

    m_layerToolWidget = new FrLayerToolWidget(this);
    //m_layerToolWidget->setVisible(false);   // test
    m_roiToolWidget = new FrROIToolWidget(this);
    m_roiToolWidget->setVisible(false);    

    // Setup main layout
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->addLayout(layerLayout);
    //mainLayout->addLayout(propLayout);
    mainLayout->addWidget(m_layerToolWidget);
    mainLayout->addWidget(m_roiToolWidget);

    // Connect signals
    // NOTE use cellClicked since problems for Qt 4.2.3
    connect(m_layerTable, SIGNAL(cellClicked(int,int)),
            this, SLOT(OnCellClicked(int,int)));

    connect( m_btnAdd, SIGNAL(clicked()), this, SLOT(OnAddClicked()) );
    connect( m_btnDelete, SIGNAL(clicked()), this, SLOT(OnDeleteClicked()) );
    connect( m_btnChange, SIGNAL(clicked()), this, SLOT(OnChangeClicked()) );

    //connect( m_opacityWidget, SIGNAL(ValueChanged(int)), 
    //         this, SLOT(OnOpacityChanged(int)) );

    //connect( m_colormapWidget, SIGNAL(ParamsChanged()), 
    //         this, SLOT(OnColormapParamsChanged()) );

    connect( m_layerToolWidget->GetOpacityWidget(), SIGNAL(ValueChanged(int)), 
             this, SLOT(OnOpacityChanged(int)) );

    connect( m_layerToolWidget->GetColormapWidget(), SIGNAL(ParamsChanged()), 
             this, SLOT(OnColormapParamsChanged()) );
   
    this->setFixedHeight(this->sizeHint().height());
    this->setFixedWidth(this->sizeHint().width());
}

// Common actions
void FrLayerListWidget::AddLayer(FrLayerSettings* layerSets){
    // NOTE: Layers are added in the top
    m_layerTable->insertRow(INSERT_ROW_NUM);
    m_layerTable->setRowHeight(INSERT_ROW_NUM, 40);

    // Set ID
    QString strID = QString().setNum(layerSets->ID);
    m_layerTable->setItem(INSERT_ROW_NUM, TAB_ID_IDX, new QTableWidgetItem(strID));

    // Create layer widget and add it to table
    FrLayerWidget* lw = new FrLayerWidget(*layerSets, this);
    connect(lw, SIGNAL(VisibilityChanged(int)), 
            this, SLOT(OnVisibilityChanged(int)));
    m_layerTable->setCellWidget(INSERT_ROW_NUM, TAB_LAYER_IDX, lw);

    FrColormapWidget* cmpw = m_layerToolWidget->GetColormapWidget();
    FrSpinSliderWidget* opw = m_layerToolWidget->GetOpacityWidget();

    // Init opacity
    int opacity = int(layerSets->Opacity * opw->GetMaximum());
    opw->SetValue(opacity);

    // Init Colormap settings
    cmpw->SetColormapParams(layerSets->ColormapSettings);
}

void FrLayerListWidget::AddRoiLayer(FrRoiDocObj* roiDO){
    // NOTE: Layers are added in the top
    m_layerTable->insertRow(INSERT_ROW_NUM);
    m_layerTable->setRowHeight(INSERT_ROW_NUM, 40);

    // Set ID
    QString strID = QString().setNum(roiDO->GetID());
    m_layerTable->setItem(INSERT_ROW_NUM, TAB_ID_IDX, new QTableWidgetItem(strID));

    // Create layer widget and add it to table
    FrLayerWidget* lw = new FrLayerWidget(*roiDO, this);
    connect(lw, SIGNAL(VisibilityChanged(int)), 
            this, SLOT(OnVisibilityChanged(int)));
    m_layerTable->setCellWidget(INSERT_ROW_NUM, TAB_LAYER_IDX, lw);

    FrColormapWidget* cmpw = m_layerToolWidget->GetColormapWidget();
    FrSpinSliderWidget* opw = m_layerToolWidget->GetOpacityWidget();

    // Init opacity
    int opacity = int(roiDO->GetOpacity() * opw->GetMaximum());
    opw->SetValue(opacity);
}

void FrLayerListWidget::RemoveLayers(){
    while(m_layerTable->rowCount() > 0){
        m_layerTable->removeRow(0);
    }
}

void FrLayerListWidget::SetSelectedLayer(int layerID){
    for(int row = 0; row < m_layerTable->rowCount(); ++row){
        FrLayerWidget* wgt = dynamic_cast<FrLayerWidget*>(
            m_layerTable->cellWidget(row, TAB_LAYER_IDX));

        if(wgt && wgt->GetLayerID() == layerID){
            // NOTE setup current cell and emulate click
            m_layerTable->setCurrentCell(row, TAB_LAYER_IDX);
            this->OnCellClicked(row, TAB_LAYER_IDX);
            break;
        }
    }
}

bool FrLayerListWidget::GetLayerParams(int id, FrLayerSettings& params){
    for(int row = 0; row < m_layerTable->rowCount(); ++row){
        FrLayerWidget* wgt = dynamic_cast<FrLayerWidget*>(
            m_layerTable->cellWidget(row, TAB_LAYER_IDX));

        if(wgt && wgt->GetLayerID() == id){
            wgt->GetLayerParams(params);
            return true;
        }
    }
    return false;
}

// Private slots
void FrLayerListWidget::OnCellClicked(int row, int col){
    FrLayerWidget* wgt = dynamic_cast<FrLayerWidget*>(m_layerTable->cellWidget(row, TAB_LAYER_IDX));

    if(wgt){
        // Setup widgets
        FrLayerSettings params;
        wgt->GetLayerParams(params);

        if(wgt->IsRoiLayer()){
            m_layerToolWidget->setVisible(false);
            m_roiToolWidget->setVisible(true);

            //m_roiToolWidget->GetOp
        }
        else {
            // Not a ROI layer selected
            m_roiToolWidget->setVisible(false);
            m_layerToolWidget->setVisible(true);
        
            FrColormapWidget* cmpw = m_layerToolWidget->GetColormapWidget();
            FrSpinSliderWidget* opw = m_layerToolWidget->GetOpacityWidget();

            if (params.ID == DEFAULT_LAYER_ID){
                cmpw->setVisible(false);
            }
            else {
                cmpw->setVisible(true);
                cmpw->BlockSignals(true);
                cmpw->SetColormapParams(params.ColormapSettings);
                cmpw->BlockSignals(false);
            }

            int opacity = int(params.Opacity * opw->GetMaximum());
            opw->SetValue(opacity);
        }

        if(m_signalsBlocked) return;
        emit LayerSelected(params.ID);
    }
}

// Manage whole layer
void FrLayerListWidget::OnAddClicked(){
    if(m_signalsBlocked) return;
    emit NewLayer();
}

void FrLayerListWidget::OnDeleteClicked(){
    if(m_signalsBlocked) return;
    emit DeleteLayer();
}

void FrLayerListWidget::OnChangeClicked(){
    if(m_signalsBlocked) return;
    emit ChangeLayer();
}

// Manage some params
void FrLayerListWidget::OnVisibilityChanged(int id){    
    this->SetSelectedLayer(id);

    if(m_signalsBlocked) return;
    emit ChangeLayerParams();
}

void FrLayerListWidget::OnOpacityChanged(int value){
    this->UpdateCurrentLayerParams();

    if(m_signalsBlocked) return;
    emit ChangeLayerParams();
}


void FrLayerListWidget::OnColormapParamsChanged(){
    this->UpdateCurrentLayerParams();

    if(m_signalsBlocked) return;
    emit ChangeLayerColormap();
}

// Utility methods
void FrLayerListWidget::UpdateCurrentLayerParams(){
    // Sync params stored in current layer widget
    int row = m_layerTable->currentRow();
    FrLayerWidget* wgt = dynamic_cast<FrLayerWidget*>(
            m_layerTable->cellWidget(row, TAB_LAYER_IDX));

    if(wgt){
        // Save block signal flag
        bool oldSB = m_signalsBlocked;
        m_signalsBlocked = true;

        FrLayerSettings params;
        wgt->GetLayerParams(params);
        FrColormapWidget* cmpw = m_layerToolWidget->GetColormapWidget();
        FrSpinSliderWidget* opw = m_layerToolWidget->GetOpacityWidget();

        params.Opacity = double(opw->GetValue()) / 
                         double(opw->GetMaximum());

        cmpw->GetColormapParams(params.ColormapSettings);
        wgt->SetLayerParams(params);

        m_signalsBlocked = oldSB;
    }
}

