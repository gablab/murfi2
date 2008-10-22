#include "FrLayerListWidget.h"
#include "FrTabSettingsDocObj.h"
#include "FrLayerWidget.h"

#include "Qt/QTableWidget.h"
#include "Qt/QBoxLayout.h"
#include "Qt/QCheckBox.h"
#include "Qt/QToolButton.h"
#include "Qt/QVariant.h"
#include "Qt/QLabel.h"
#include "Qt/QSpinBox.h"
#include "Qt/QComboBox.h"
#include "Qt/QSlider.h"
#include "Qt/QGroupBox.h"
#include "Qt/QApplication.h"
#include "Qt/QPushButton.h"
#include "Qt/QColorDialog.h"


#define TAB_ID_IDX      0
#define TAB_NAME_IDX    1
#define TAB_VISIBLE_IDX 2
#define TAB_OPACITY_IDX 3
#define TAB_CMAP_IDX    4
#define TAB_RANGE_IDX   5
#define TAB_THRESH_IDX  6
#define COLUMN_COUNT    7

#define MULTICOLOR_ITEM_IDX 0 
#define SINGLECOLOR_ITEM_IDX 1 

#define INSERT_ROW_NUM 0

FrLayerListWidget::FrLayerListWidget(QWidget *parent)
: QWidget(parent), m_signalsBlocked(false) {
	m_layerTable = new QTableWidget(this);
	m_layerTable->setShowGrid(false);
    m_layerTable->setRowCount(0);
	m_layerTable->setColumnCount(2);
	m_layerTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    QTableWidgetItem* twi = new QTableWidgetItem("Id");
	m_layerTable->setHorizontalHeaderItem(TAB_ID_IDX, twi);
	m_layerTable->setColumnWidth(TAB_ID_IDX, 50);
    m_layerTable->hideColumn(TAB_ID_IDX);

	twi = new QTableWidgetItem("Layers");
	m_layerTable->setHorizontalHeaderItem(TAB_NAME_IDX, twi);
	m_layerTable->setColumnWidth(TAB_NAME_IDX, 150);

	//m_layerTable->setFixedWidth(200);

	// colormap group
	QLabel *colormap;
	colormap = new QLabel("Colormap", this);
	colormap->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	QFrame *line2;
	line2 = new QFrame(this);
    line2->setFrameShape(QFrame::HLine);
    line2->setFrameShadow(QFrame::Sunken);

	QHBoxLayout* hl3 = new QHBoxLayout();
	hl3->addWidget(colormap);
	hl3->addWidget(line2);

	QLabel *label_3;
	label_3 = new QLabel("Type: ", this);
	QLabel *label_4;
	label_4 = new QLabel("PxMin: ", this);
	QLabel *label_5;
	label_5 = new QLabel("PxMax: ", this);

	QVBoxLayout* vl3 = new QVBoxLayout();
	vl3->addWidget(label_3);
	vl3->addWidget(label_4);
	vl3->addWidget(label_5);

	PxMinSpinBox = new QSpinBox(this);
    PxMinSpinBox->setMaximum(255);
    PxMinSpinBox->setValue(0);
	connect(PxMinSpinBox, SIGNAL(valueChanged(int)), this, SLOT(SetPxMinSliderPosition(int)));

	PxMinSlider = new QSlider(this);
    PxMinSlider->setMaximum(255);
    PxMinSlider->setValue(0);
    PxMinSlider->setOrientation(Qt::Horizontal);
	connect(PxMinSlider, SIGNAL(valueChanged(int)), this, SLOT(SetPxMinSpinBoxPosition(int)));

	QHBoxLayout* hl4 = new QHBoxLayout();
	hl4->addWidget(PxMinSpinBox);
	hl4->addWidget(PxMinSlider);

	PxMaxSpinBox = new QSpinBox(this);
    PxMaxSpinBox->setMaximum(255);
    PxMaxSpinBox->setValue(255);
	connect(PxMaxSpinBox, SIGNAL(valueChanged(int)), this, SLOT(SetPxMaxSliderPosition(int)));

	PxMaxSlider = new QSlider(this);
    PxMaxSlider->setMaximum(255);
    PxMaxSlider->setValue(255);
    PxMaxSlider->setOrientation(Qt::Horizontal);
	connect(PxMaxSlider, SIGNAL(valueChanged(int)), this, SLOT(SetPxMaxSpinBoxPosition(int)));

	QHBoxLayout* hl5 = new QHBoxLayout();
	hl5->addWidget(PxMaxSpinBox);
	hl5->addWidget(PxMaxSlider);

	comboBox = new QComboBox(this);
    comboBox->insertItems(0, QStringList()
     << QApplication::translate("Dialog", "Multicolor", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Dialog", "Singlecolor", 0, QApplication::UnicodeUTF8)
    );
	comboBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxChange(int)));

	QVBoxLayout* vl4 = new QVBoxLayout();
	vl4->addWidget(comboBox);
	vl4->addLayout(hl4);
	vl4->addLayout(hl5);

	QHBoxLayout* hmainc = new QHBoxLayout();
	hmainc->addLayout(vl3);
	hmainc->addLayout(vl4);

	// additional params group for multi colormap
	groupBox = new QGroupBox("Additional Params", this);

	QLabel *label_6;
	label_6 = new QLabel("Threshold: ", groupBox);

	ThresholdSpinBox = new QSpinBox(groupBox);
    ThresholdSpinBox->setMaximum(255);
    ThresholdSpinBox->setValue(128);
	connect(ThresholdSpinBox, SIGNAL(valueChanged(int)), this, SLOT(SetThresholdSliderPosition(int)));

	ThresholdSlider = new QSlider(groupBox);
    ThresholdSlider->setMaximum(255);
    ThresholdSlider->setValue(128);
    ThresholdSlider->setOrientation(Qt::Horizontal);
	connect(ThresholdSlider, SIGNAL(valueChanged(int)), this, SLOT(SetThresholdSpinBoxPosition(int)));
	
	QHBoxLayout* groupBoxLayout = new QHBoxLayout(groupBox);
	groupBoxLayout->addWidget(label_6);
	groupBoxLayout->addWidget(ThresholdSpinBox);
	groupBoxLayout->addWidget(ThresholdSlider);

	// additional params group for single colormap
	groupBox2 = new QGroupBox("Additional Params", this);

	QLabel *label_7;
	label_7 = new QLabel("Selected Color: ", groupBox2);

	colorWidget = new QWidget(groupBox2);
	colorWidget->setSizeIncrement(100, 20);
    colorWidget->setMinimumSize(24, 24);
		
	QPalette palette;
    QBrush brush(QColor(DEF_CM_COLOR));
    brush.setStyle(Qt::SolidPattern);	
	palette.setBrush(QPalette::Active, QPalette::Window, brush);
	palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
    palette.setBrush(QPalette::Disabled, QPalette::Window, brush);

	colorWidget->setPalette(palette);
	colorWidget->setAutoFillBackground(true);

	QPushButton* colorBtn;
	colorBtn = new QPushButton("Color", groupBox2);
	connect(colorBtn, SIGNAL(clicked()), this, SLOT(onColorBtnClicked()));

	QHBoxLayout* groupBoxLayout2 = new QHBoxLayout(groupBox2);
	groupBoxLayout2->addWidget(label_7);
	groupBoxLayout2->addWidget(colorWidget);
	groupBoxLayout2->addWidget(colorBtn);

	// opacity
	QLabel*	label_2 = new QLabel("Opacity: ", this);

	OpacitySpinBox = new QSpinBox(this);
    OpacitySpinBox->setMaximum(255);
    OpacitySpinBox->setValue(255);
	connect(OpacitySpinBox, SIGNAL(valueChanged(int)), this, SLOT(SetOpacitySliderPosition(int)));

	OpacitySlider = new QSlider(this);
    OpacitySlider->setMaximum(255);
    OpacitySlider->setValue(255);
    OpacitySlider->setOrientation(Qt::Horizontal);
	connect(OpacitySlider, SIGNAL(valueChanged(int)), this, SLOT(SetOpacitySpinBoxPosition(int)));

	QHBoxLayout* hl1 = new QHBoxLayout();
	hl1->addWidget(label_2);
	hl1->addWidget(OpacitySpinBox);
	hl1->addWidget(OpacitySlider);

	// main vertical layout
	QVBoxLayout* m_vLayout = new QVBoxLayout();
	m_vLayout->addLayout(hl3);
	m_vLayout->addLayout(hmainc);
	m_vLayout->addWidget(groupBox);
	m_vLayout->addWidget(groupBox2);
	m_vLayout->addLayout(hl1);

	groupBox->setVisible(true);
	groupBox2->setVisible(false);

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

	QVBoxLayout* verl = new QVBoxLayout();
	verl->addWidget(m_layerTable);
	verl->addWidget(btnBlock);

	m_layout = new QHBoxLayout(this);
    m_layout->addLayout(verl);
	m_layout->addLayout(m_vLayout);
    this->setLayout(m_layout);

    connect(m_layerTable, SIGNAL(itemSelectionChanged()), this, SLOT(OnSelectionChanged()));
    connect(m_layerTable, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(OnNameChanged(int, int)));

    connect( m_btnAdd, SIGNAL(clicked()), this, SLOT(OnAddClicked()) );
    connect( m_btnDelete, SIGNAL(clicked()), this, SLOT(OnDeleteClicked()) );
    connect( m_btnChange, SIGNAL(clicked()), this, SLOT(OnChangeClicked()) );
}

void FrLayerListWidget::AddLayer(FrLayerSettings* layerSets){
  //  // NOTE: Layers are added in the top
    m_layerTable->insertRow(INSERT_ROW_NUM);
	m_layerTable->setRowHeight(INSERT_ROW_NUM, 40);

    QString value = "";
	int cmtype;

	value.setNum(layerSets->ID);
    m_layerTable->setItem(INSERT_ROW_NUM, TAB_ID_IDX, new QTableWidgetItem(value));

    if(layerSets->ID == DEFAULT_LAYER_ID){
        value = "Default";
		cmtype = 0;
    }
    else{
		value = layerSets->Name;
		cmtype = 1;
		if (layerSets->ColormapSettings.Type == FrColormapSettings::SingleColor)
			cmtype = 2;
    }
	
	FrLayerWidget* lw = new FrLayerWidget(cmtype, value, layerSets->Visibility, this);
	m_layerTable->setCellWidget(INSERT_ROW_NUM, 1, lw);

	SetLayerParams(layerSets);	
}

void FrLayerListWidget::SetLayerParams(FrLayerSettings* layerSets){
    QString value = "";

	SetOpacitySliderPosition(int(layerSets->Opacity * 255));
	SetOpacitySpinBoxPosition(int(layerSets->Opacity * 255));

	QPalette palette;
	QBrush brush(layerSets->ColormapSettings.Color);

	if(layerSets->ID != DEFAULT_LAYER_ID){
        value = "none";
        switch(layerSets->ColormapSettings.Type){
            case FrColormapSettings::SingleColor: 
				comboBox->setCurrentIndex(1);

				brush.setStyle(Qt::SolidPattern);	
				palette.setBrush(QPalette::Active, QPalette::Window, brush);
				palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
				palette.setBrush(QPalette::Disabled, QPalette::Window, brush);
				colorWidget->setPalette(palette);

				break;
            case FrColormapSettings::MultiColor: 
				comboBox->setCurrentIndex(0);

				SetThresholdSliderPosition(layerSets->ColormapSettings.Threshold);
				SetThresholdSpinBoxPosition(layerSets->ColormapSettings.Threshold);
				break;
        }

		SetPxMinSliderPosition(layerSets->ColormapSettings.MinValue);
		SetPxMinSpinBoxPosition(layerSets->ColormapSettings.MinValue);
		SetPxMaxSliderPosition(layerSets->ColormapSettings.MaxValue);
		SetPxMaxSpinBoxPosition(layerSets->ColormapSettings.MaxValue);
    }
	else{
		// TODO: disable input elements?
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

void FrLayerListWidget::onComboBoxChange(int index){
	switch(index){
		case MULTICOLOR_ITEM_IDX:
            //cmType = FrColormapSettings::MultiColor;
			groupBox2->setVisible(false);
            groupBox->setVisible(true);
			break;
		case SINGLECOLOR_ITEM_IDX:
            //cmType = FrColormapSettings::SingleColor;
			groupBox->setVisible(false);
			groupBox2->setVisible(true);
			break;
	}
}

void FrLayerListWidget::onColorBtnClicked(){
	QColor newColor = QColorDialog::getColor(color, this);
    if(newColor.isValid()){
        color = newColor;
    }

	QPalette palette;
    QBrush brush(color);
    brush.setStyle(Qt::SolidPattern);	
	palette.setBrush(QPalette::Active, QPalette::Window, brush);
	palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
    palette.setBrush(QPalette::Disabled, QPalette::Window, brush);

	colorWidget->setPalette(palette);
}

void FrLayerListWidget::SetOpacitySliderPosition(int value){
	OpacitySlider->setValue(value);
}

void FrLayerListWidget::SetPxMinSliderPosition(int value){
	PxMinSlider->setValue(value);    
}

void FrLayerListWidget::SetPxMaxSliderPosition(int value){
	PxMaxSlider->setValue(value);
}

void FrLayerListWidget::SetThresholdSliderPosition(int value){
	ThresholdSlider->setValue(value);
}

void FrLayerListWidget::SetOpacitySpinBoxPosition(int value){
	OpacitySpinBox->setValue(value);
}

void FrLayerListWidget::SetPxMinSpinBoxPosition(int value){
	PxMinSpinBox->setValue(value);
}

void FrLayerListWidget::SetPxMaxSpinBoxPosition(int value){
	PxMaxSpinBox->setValue(value);
}

void FrLayerListWidget::SetThresholdSpinBoxPosition(int value){
	ThresholdSpinBox->setValue(value);
}

void FrLayerListWidget::OnVisibilityChanged(int state){
	// TODO: implement
}

void FrLayerListWidget::OnNameChanged(int row, int column){
	// TODO: implement
}

