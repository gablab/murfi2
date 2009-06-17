#include "FrLayerListWidget.h"
#include "FrTabSettingsDocObj.h"
#include "FrLayerWidget.h"
#include "FrColormapWidget.h"
#include "FrSliderWidget.h"
#include "FrSpinSliderWidget.h"
#include "FrROIToolWidget.h"
#include "FrRoiDocObj.h"
#include "FrLayerDocObj.h"
#include "FrMainDocument.h"
#include "FrViewDocObj.h"
#include "FrMarshalling.h"

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

#define MULTICOLOR_ITEM_IDX     0
#define SINGLECOLOR_ITEM_IDX    1

#define INSERT_ROW_NUM 0

FrLayerListWidget::FrLayerListWidget(QWidget *parent, FrMainDocument* doc)
: QWidget(parent), m_Document(doc), m_signalsBlocked(false) {    

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
    m_btnChange->setText("c");
    m_btnChange->setToolTip("Configure selected layer");

    // opacity widget
    //QLabel*	lblOpacity = new QLabel("Opacity: ", this);
    m_opacityWidget = new FrSliderWidget(this);
    m_opacityWidget->SetMinMax(0, 100);
    m_opacityWidget->SetValue(100);

    QHBoxLayout* btnLayout = new QHBoxLayout(btnBlock);
    btnLayout->addWidget(m_btnAdd);
    btnLayout->addWidget(m_btnDelete);
    btnLayout->addWidget(m_btnChange);
    btnLayout->addWidget(m_opacityWidget);
    //btnLayout->addStretch();
    btnBlock->setLayout(btnLayout);

    // setup layer layout (leftmost)
    QVBoxLayout* layerLayout = new QVBoxLayout();
    layerLayout->addWidget(m_layerTable);
    layerLayout->addWidget(btnBlock);

    // add up and down arrow buttons
    m_btnUpArrow = new QToolButton();
    m_btnUpArrow->setToolTip("Move layer up");
    m_btnUpArrow->setArrowType(Qt::UpArrow);

    m_btnDownArrow = new QToolButton();
    m_btnDownArrow->setToolTip("Move layer down");
    m_btnDownArrow->setArrowType(Qt::DownArrow);
    
    // setup layout for arrow buttons
	QSpacerItem *spacerItem1;
	spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
	QSpacerItem *spacerItem2;
	spacerItem2 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

    QVBoxLayout* arrowsLayout = new QVBoxLayout();
    arrowsLayout->addItem(spacerItem1);
    arrowsLayout->addWidget(m_btnUpArrow);
    arrowsLayout->addWidget(m_btnDownArrow);
    arrowsLayout->addItem(spacerItem2);

    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addLayout(arrowsLayout);
    hLayout->addLayout(layerLayout);

    // ohinds: 2009-02-23
    // moved the opacity slider to the btn block
//    QHBoxLayout* opacityLayout = new QHBoxLayout();
//    opacityLayout->addWidget(lblOpacity);
//    opacityLayout->addWidget(m_opacityWidget);
//
    //// spacer
    //QSpacerItem *spacerItem;
    //spacerItem = new QSpacerItem(40, 40, QSizePolicy::Fixed, QSizePolicy::Expanding);

    // Setup property layout (rightmost)
    m_roiToolWidget = new FrROIToolWidget(this);
    m_roiToolWidget->setVisible(false);
    m_roiToolWidget->setEnabled(false);

    // colormap widget
    m_colormapWidget = new FrColormapWidget(this);

    QVBoxLayout* propLayout = new QVBoxLayout();
    propLayout->addWidget(m_colormapWidget);
    propLayout->addWidget(m_roiToolWidget);
    //propLayout->addLayout(opacityLayout);
        
    // Setup main layout
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->addLayout(hLayout);//layerLayout);
    mainLayout->addLayout(propLayout);

    // Connect signals
    // NOTE use cellClicked since problems for Qt 4.2.3
    connect(m_layerTable, SIGNAL(cellClicked(int,int)),
            this, SLOT(OnCellClicked(int,int)));

    connect( m_btnAdd, SIGNAL(clicked()), this, SLOT(OnAddClicked()) );
    connect( m_btnDelete, SIGNAL(clicked()), this, SLOT(OnDeleteClicked()) );
    connect( m_btnChange, SIGNAL(clicked()), this, SLOT(OnChangeClicked()) );

    connect( m_btnUpArrow, SIGNAL(clicked()), this, SLOT(OnUpArrowClicked()) );
    connect( m_btnDownArrow, SIGNAL(clicked()), this, SLOT(OnDownArrowClicked()) );

    connect( m_opacityWidget, SIGNAL(ValueChanged(int)), 
             this, SLOT(OnOpacityChanged(int)) );

    connect( m_colormapWidget, SIGNAL(ParamsChanged()), 
             this, SLOT(OnColormapParamsChanged()) );
       
    connect( m_roiToolWidget, SIGNAL(CurrentToolChanged()), this, SLOT(OnRoiToolChanged()) );
    connect( m_roiToolWidget, SIGNAL(ClearCurrentRoiAction()), this, SLOT(OnClearCurrentRoiAction()) );

    this->setMinimumHeight(this->sizeHint().height());
    this->setFixedWidth(this->sizeHint().width());
}

// Common actions
void FrLayerListWidget::AddLayer(FrLayerDocObj* layerDO, int pos){
    // NOTE: Layers are added in the top
    //m_layerTable->insertRow(INSERT_ROW_NUM);
    //m_layerTable->setRowHeight(INSERT_ROW_NUM, 40);

    // Set ID
    QString strID = QString().setNum(layerDO->GetID());
    m_layerTable->setItem(pos, TAB_ID_IDX, new QTableWidgetItem(strID));

    // Create layer widget and add it to table
    FrLayerWidget* lw = new FrLayerWidget(layerDO, this);
    connect(lw, SIGNAL(VisibilityChanged(unsigned long)), 
            this, SLOT(OnVisibilityChanged(unsigned long)));
    m_layerTable->setCellWidget(pos, TAB_LAYER_IDX, lw);
    
    // Init opacity
    int opacity = int(layerDO->GetOpacity() * m_opacityWidget->GetMaximum());
    m_opacityWidget->SetValue(opacity);

    // if colormap layer then init Colormap settings
    if (layerDO->IsImage()){
        FrImageLayerSettings* cmlSet = (FrImageLayerSettings*) layerDO->GetSettings();    
        m_colormapWidget->SetColormapParams(*cmlSet);
    }
}

void FrLayerListWidget::RemoveLayers(){
    int g = 5;
    while(m_layerTable->rowCount() > 0){
        m_layerTable->removeRow(0);
        //m_layerTable->clear();
    }
}

void FrLayerListWidget::UpdateRoiList(){    
    // Get selected layer widget
    int curRow = m_layerTable->currentRow();
    FrLayerWidget* curWgt = dynamic_cast<FrLayerWidget*>(
            m_layerTable->cellWidget(curRow, TAB_LAYER_IDX));
    
    unsigned long id = 0;
    if (curWgt)
        id = curWgt->GetID();
    
    FrLayerDocObj* layerDO = m_Document->GetLayerDocObjByID(id);
    if (!layerDO)
        return;

    // Update only when ROI layer selected
    if(layerDO->IsRoi()){
        // get all roi infos except current
        std::vector<FrROIToolWidget::RoiInfo> roiInfos;
        for(int row = 0; row < m_layerTable->rowCount(); ++row){
            // skip current row
            if(row == curRow) continue;

            FrLayerWidget* wgt = dynamic_cast<FrLayerWidget*>(
                m_layerTable->cellWidget(row, TAB_LAYER_IDX));
            
            unsigned long wid = 0;
            if (wgt)
                wid = wgt->GetID();
            
            FrLayerDocObj* layerDoc = m_Document->GetLayerDocObjByID(wid);

            if(layerDoc->IsRoi()){
                FrROIToolWidget::RoiInfo info;
                info.ID = wid;
                info.Name = layerDoc->GetSettings()->Name;
                roiInfos.push_back(info);
            }
        }
        m_roiToolWidget->UpdateRoiInfo(roiInfos);
    }
}

void FrLayerListWidget::SetSelectedLayer(unsigned long layerID){
    for(int row = 0; row < m_layerTable->rowCount(); ++row){
        FrLayerWidget* wgt = dynamic_cast<FrLayerWidget*>(
            m_layerTable->cellWidget(row, TAB_LAYER_IDX));

        unsigned long id = 0;
        if (wgt)
            id = wgt->GetID();

        if(id == layerID){
            // NOTE setup current cell and emulate click
            m_layerTable->setCurrentCell(row, TAB_LAYER_IDX);
            this->OnCellClicked(row, TAB_LAYER_IDX);
            break;
        }
    }
}

// Private slots
void FrLayerListWidget::OnCellClicked(int row, int col){
    FrLayerWidget* wgt = dynamic_cast<FrLayerWidget*>(m_layerTable->cellWidget(row, TAB_LAYER_IDX));

    if(wgt){
        // Setup widgets
        unsigned long id = wgt->GetID();
        
        // TODO: get layer DO with specified ID
        FrLayerDocObj* layerDO = m_Document->GetLayerDocObjByID(id);

	if(layerDO == NULL) {
	  return;
	}

        if(layerDO->IsRoi()){
            m_colormapWidget->setVisible(false);
            m_roiToolWidget->setVisible(true);
            this->UpdateRoiList();
        }
        else {
            // Not a ROI layer selected
            m_roiToolWidget->setVisible(false);
            m_colormapWidget->setVisible(true);
            
            //if (id == DEF_LAYER_ID){
            if (layerDO->IsImage()){        
                FrImageLayerSettings* cmlParams = (FrImageLayerSettings*)layerDO->GetSettings();

                m_colormapWidget->setVisible(true);
                m_colormapWidget->BlockSignals(true);
                m_colormapWidget->SetColormapParams(*cmlParams);
                m_colormapWidget->BlockSignals(false);
            }
        }

        // Update opacity
        int opacity = int(layerDO->GetSettings()->Opacity * m_opacityWidget->GetMaximum());

        // Update visibility
        bool visibility = layerDO->GetSettings()->Visibility;
        
        bool old = m_signalsBlocked;
        m_signalsBlocked = true;
        m_opacityWidget->SetValue(opacity);
        //wgt->SetVisibility(visibility);
        m_signalsBlocked = old;
        
        if(m_signalsBlocked) return;
        emit LayerSelected(id);
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

void FrLayerListWidget::OnUpArrowClicked(){
    if(m_signalsBlocked) return;
    emit ChangeLayerPosition(1);    // move layer up
}

void FrLayerListWidget::OnDownArrowClicked(){
    if(m_signalsBlocked) return;
    emit ChangeLayerPosition(-1);   // move layer down
}

// Manage some params
void FrLayerListWidget::OnVisibilityChanged(unsigned long id){    
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

void FrLayerListWidget::OnRoiToolChanged(){
    this->UpdateRoiList();
    emit RoiToolChanged();
}

void FrLayerListWidget::OnClearCurrentRoiAction(){
    emit ClearCurrentRoi();
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

        unsigned long id = wgt->GetID();
        FrLayerDocObj* layerDO = m_Document->GetLayerDocObjByID(id);
                
        layerDO->GetSettings()->Opacity = double(m_opacityWidget->GetValue()) / 
                         double(m_opacityWidget->GetMaximum());

        FrImageLayerSettings* cmlParams = (FrImageLayerSettings*)layerDO->GetSettings();
        m_colormapWidget->GetColormapParams(*cmlParams);
        wgt->SetLayerParams(layerDO);

        m_signalsBlocked = oldSB;
    }
}


//FrLayerDocObj* FrLayerListWidget::GetLayerDocObjByID(int id){
//    FrLayerDocObj* layerDO = 0L;
//    FrDocument::DocObjCollection layers;
//    m_Document->GetObjectsByType(layers, FrDocumentObj::LayerObject);    
//
//    if(layers.size() > 0){
//        for (int i = 0; i < layers.size(); i++){
//            layerDO = dynamic_cast<FrLayerDocObj*>(layers[i]);
//            if (layerDO->GetID() == id)
//                return layerDO;
//        }
//    }
//
//    return 0L;
//}

bool FrLayerListWidget::GetLayerVisibility(unsigned long id){
    // find widget with given id
    for(int row = 0; row < m_layerTable->rowCount(); ++row){
        FrLayerWidget* wgt = dynamic_cast<FrLayerWidget*>(
            m_layerTable->cellWidget(row, TAB_LAYER_IDX));

        if(wgt && wgt->GetID() == id){
            return wgt->GetVisibility();
        }
    }

    return false;
}

int FrLayerListWidget::GetOpacity(){
    return m_opacityWidget->GetValue();    
}

// HACK: signal will be emited to main thread
void FrLayerListWidget::Update(){
    // We have to create update event and marshal it to main thread 
    // to prevent cross thread interaction in Qt (this may freeze app)
    FrLayerListWidget* llw = const_cast<FrLayerListWidget*>(this);
    FrLLWMarshalingEvent* event = new FrLLWMarshalingEvent(llw);
    MarshalToMainThread(event);
}

// Update info displayed by widget
void FrLayerListWidget::OnUpdate(){
    if(!m_Document) return;
    // Get all layers
    std::vector<FrDocumentObj*> layers;
    m_Document->GetObjectsByType(layers, FrDocumentObj::LayerObject);
    if(layers.size() <= 0) return;

    // Clear
    this->RemoveLayers();
    
    // TRICK: first create rows in table 
    for (int i = 0; i<layers.size(); i++){
        m_layerTable->insertRow(INSERT_ROW_NUM);
        m_layerTable->setRowHeight(INSERT_ROW_NUM, 40);
    }

    // Get selected layer ID 
    FrViewDocObj* viewDO = m_Document->GetCurrentViewObject();
    if(viewDO == 0) return;

    unsigned long layerID = viewDO->GetActiveLayerID();
    
    this->BlockSignals(true);

    // add layers in order
    std::vector<FrDocumentObj*>::iterator it, itEnd(layers.end());
    for(it = layers.begin(); it != itEnd; ++it){
        FrLayerDocObj* layerDO = (FrLayerDocObj*) (*it);
        // get layer position
        int pos = layers.size() - layerDO->GetPosition() - 1;   // first layer has max position but should be at top in widget (0)
	    this->AddLayer(layerDO, pos);
    }

    this->SetSelectedLayer(layerID);
    this->UpdateRoiList();
    this->BlockSignals(false);
}

