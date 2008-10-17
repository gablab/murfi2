#include "FrLayerListWidget.h"
#include "FrTabSettingsDocObj.h"

#include "Qt/QTableWidget.h"
#include "Qt/QBoxLayout.h"
#include "Qt/QCheckBox.h"
#include "Qt/QToolButton.h"
#include "Qt/QVariant.h"

#define TAB_ID_IDX      0
#define TAB_NAME_IDX    1
#define TAB_VISIBLE_IDX 2
#define TAB_OPACITY_IDX 3
#define TAB_CMAP_IDX    4
#define TAB_RANGE_IDX   5
#define TAB_THRESH_IDX  6
#define COLUMN_COUNT    7

#define INSERT_ROW_NUM 0

FrLayerListWidget::FrLayerListWidget(QWidget *parent)
: QWidget(parent), m_signalsBlocked(false) {
	m_layerTable = new QTableWidget(this);
    
	// add column headers
    m_layerTable->setRowCount(0);
	m_layerTable->setColumnCount(COLUMN_COUNT);
	m_layerTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    QTableWidgetItem* twi = new QTableWidgetItem("Id");
	m_layerTable->setHorizontalHeaderItem(TAB_ID_IDX, twi);
	m_layerTable->setColumnWidth(TAB_ID_IDX, 50);
    m_layerTable->hideColumn(TAB_ID_IDX);

    twi = new QTableWidgetItem("Name");
	m_layerTable->setHorizontalHeaderItem(TAB_NAME_IDX, twi);
	m_layerTable->setColumnWidth(TAB_NAME_IDX, 100);
    
    twi = new QTableWidgetItem("Visible");
    m_layerTable->setHorizontalHeaderItem(TAB_VISIBLE_IDX, twi);
	m_layerTable->setColumnWidth(TAB_VISIBLE_IDX, 50);

    twi = new QTableWidgetItem("Opacity");
	m_layerTable->setHorizontalHeaderItem(TAB_OPACITY_IDX, twi);
	m_layerTable->setColumnWidth(TAB_OPACITY_IDX, 70);

    twi = new QTableWidgetItem("Colormap");
	m_layerTable->setHorizontalHeaderItem(TAB_CMAP_IDX, twi);
	m_layerTable->setColumnWidth(TAB_CMAP_IDX, 70);

    twi = new QTableWidgetItem("Range");
	m_layerTable->setHorizontalHeaderItem(TAB_RANGE_IDX, twi);
	m_layerTable->setColumnWidth(TAB_RANGE_IDX, 70);
    
    twi = new QTableWidgetItem("Threshold");
	m_layerTable->setHorizontalHeaderItem(TAB_THRESH_IDX, twi);
	m_layerTable->setColumnWidth(TAB_THRESH_IDX, 70);
	
    // setup block of buttons
    QWidget* btnBlock = new QWidget(this);
    QHBoxLayout* btnLayout = new QHBoxLayout(btnBlock);
    m_btnAdd = new QToolButton(btnBlock);
    m_btnAdd->setText("+");
    m_btnAdd->setToolTip("Add new layer");
    m_btnDelete = new QToolButton(btnBlock);
    m_btnDelete->setText("x");
    m_btnDelete->setToolTip("Delete selected layer");
    m_btnChange = new QToolButton(btnBlock);
    m_btnChange->setText("C");
    m_btnChange->setToolTip("Change selected layer");
    
    btnLayout->addWidget(m_btnAdd);
    btnLayout->addWidget(m_btnDelete);
    btnLayout->addWidget(m_btnChange);
    btnLayout->addStretch();
    btnBlock->setLayout(btnLayout);
    
	m_layout = new QVBoxLayout(this);
    //m_layout->setSpacing(1);
    m_layout->addWidget(btnBlock);
    m_layout->addWidget(m_layerTable);
    this->setLayout(m_layout);

    connect(m_layerTable, SIGNAL(itemSelectionChanged()), this, SLOT(OnSelectionChanged()));

    connect( m_btnAdd, SIGNAL(clicked()), this, SLOT(OnAddClicked()) );
    connect( m_btnDelete, SIGNAL(clicked()), this, SLOT(OnDeleteClicked()) );
    connect( m_btnChange, SIGNAL(clicked()), this, SLOT(OnChangeClicked()) );
}

void FrLayerListWidget::AddLayer(FrLayerSettings* layerSets){
    // NOTE: Layers are added in the top
    m_layerTable->insertRow(INSERT_ROW_NUM);

    QString value = "";
    value.setNum(layerSets->ID);
    m_layerTable->setItem(INSERT_ROW_NUM, TAB_ID_IDX, new QTableWidgetItem(value));
    
    if(layerSets->ID == DEFAULT_LAYER_ID){
        value = "Default";
    }
    else{
        value = "Layer";
        value += QString().setNum(layerSets->ID);
    }
    m_layerTable->setItem(INSERT_ROW_NUM, TAB_NAME_IDX, new QTableWidgetItem(value));

    value = (layerSets->Visibility) ? "+" : "-";
    m_layerTable->setItem(INSERT_ROW_NUM, TAB_VISIBLE_IDX, new QTableWidgetItem(value));

    value.setNum(int(layerSets->Opacity * 255));
    m_layerTable->setItem(INSERT_ROW_NUM, TAB_OPACITY_IDX, new QTableWidgetItem(value));
    
    if(layerSets->ID != DEFAULT_LAYER_ID){
        value = "none";
        switch(layerSets->ColormapSettings.Type){
            case FrColormapSettings::SingleColor: value = "single"; break;
            case FrColormapSettings::MultiColor: value = "multi"; break;
        }
        m_layerTable->setItem(INSERT_ROW_NUM, TAB_CMAP_IDX, new QTableWidgetItem(value));

        value = QString().setNum(layerSets->ColormapSettings.MinValue);
        value += " : ";
        value += QString().setNum(layerSets->ColormapSettings.MaxValue);
	    m_layerTable->setItem(INSERT_ROW_NUM, TAB_RANGE_IDX, new QTableWidgetItem(value));

        value.setNum(layerSets->ColormapSettings.Threshold);
        m_layerTable->setItem(INSERT_ROW_NUM, TAB_THRESH_IDX, new QTableWidgetItem(value));
    }
}

void FrLayerListWidget::RemoveLayers(){
    while(m_layerTable->rowCount() > 0){
        m_layerTable->removeRow(0);
    }
}

void FrLayerListWidget::SetSelectedLayer(int layerID){
    for(int row = 0; row < m_layerTable->rowCount(); ++row){
        QVariant var = m_layerTable->item(row, TAB_ID_IDX)->data(0);
        int id = var.toInt();

        if(layerID == id){
            m_layerTable->selectRow(row);
            break;
        }
    }
}

void FrLayerListWidget::OnSelectionChanged(){
    if(m_signalsBlocked) return;

    int row = m_layerTable->currentRow();
    QVariant var = m_layerTable->item(row, TAB_ID_IDX)->data(0);
	int id = var.toInt();
    emit LayerSelected(id);
}

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