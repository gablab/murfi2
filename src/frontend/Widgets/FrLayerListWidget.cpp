#include "FrLayerListWidget.h"
#include "FrTabSettingsDocObj.h"

#include "Qt/QTableWidget.h"
#include "Qt/QBoxLayout.h"
#include "Qt/QCheckBox.h"
#include "Qt/QVariant.h"

#define TAB_ID_IDX      0
#define TAB_NAME_IDX    1
#define TAB_VISIBLE_IDX 2
#define TAB_OPACITY_IDX 3
#define TAB_CMAP_IDX    4
#define TAB_THRESH_IDX  5

#define INSERT_ROW_NUM 0

FrLayerListWidget::FrLayerListWidget(QWidget *parent)
: QWidget(parent){
	m_layerTable = new QTableWidget(this);
    
	// add column headers
	m_layerTable->setColumnCount(6);
	m_layerTable->setRowCount(0);
	m_layerTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    QTableWidgetItem* twi = new QTableWidgetItem("Id");
	m_layerTable->setHorizontalHeaderItem(TAB_ID_IDX, twi);
	m_layerTable->setColumnWidth(0, 50);
    m_layerTable->hideColumn(TAB_ID_IDX);

    twi = new QTableWidgetItem("Name");
	m_layerTable->setHorizontalHeaderItem(TAB_NAME_IDX, twi);
	m_layerTable->setColumnWidth(2, 100);
    
    twi = new QTableWidgetItem("Visible");
    m_layerTable->setHorizontalHeaderItem(TAB_VISIBLE_IDX, twi);
	m_layerTable->setColumnWidth(1, 50);

    twi = new QTableWidgetItem("Opacity");
	m_layerTable->setHorizontalHeaderItem(TAB_OPACITY_IDX, twi);
	m_layerTable->setColumnWidth(4, 70);

    twi = new QTableWidgetItem("Colormap");
	m_layerTable->setHorizontalHeaderItem(TAB_CMAP_IDX, twi);
	m_layerTable->setColumnWidth(3, 70);
    
    twi = new QTableWidgetItem("Threshold");
	m_layerTable->setHorizontalHeaderItem(TAB_THRESH_IDX, twi);
	m_layerTable->setColumnWidth(5, 70);
	
	m_layout = new QVBoxLayout(this);
    m_layout->setSpacing(1);
    m_layout->addWidget(m_layerTable);
    this->setLayout(m_layout);

	connect(m_layerTable, SIGNAL(cellDoubleClicked(int, int)), 
            this, SLOT(OnLayerEdit(int, int)));

    connect(m_layerTable, SIGNAL(cellClicked(int, int)), 
            this, SLOT(OnLayerSelect(int, int)));
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

void FrLayerListWidget::OnLayerEdit(int row, int column){
    int id = GetLayerID(row, column);
    emit LayerEdit(id);
}

void FrLayerListWidget::OnLayerSelect(int row, int column){
    int id = GetLayerID(row, column);
    emit LayerSelected(id);
}

int FrLayerListWidget::GetLayerID(int row, int column){
    QVariant var = m_layerTable->item(row, TAB_ID_IDX)->data(0);
	int result = var.toInt();
    return result;
}