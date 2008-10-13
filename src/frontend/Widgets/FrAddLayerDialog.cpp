#include "FrAddLayerDialog.h"

#include "Qt/QLayout.h"
#include "Qt/QLabel.h"
#include "Qt/QLineEdit.h"
#include "Qt/QPushButton.h"
#include "Qt/QBoxLayout.h"
#include "Qt/QSizePolicy.h"
#include "Qt/QGroupBox.h"
#include "Qt/QCheckBox.h"
#include "Qt/QSpinBox.h"
#include "Qt/QSlider.h"
#include "Qt/QFrame.h"
#include "Qt/QComboBox.h"
#include "Qt/QApplication.h"
#include "Qt/QColorDialog.h"
#include "Qt/QMessageBox.h"


FrAddLayerDialog::FrAddLayerDialog(QWidget* parent, bool isModal)
: QDialog(parent){    
    this->setModal(isModal);
    this->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
        
    // create buttons
    m_btnOk = new QPushButton(tr("OK"), this);
    m_btnCancel = new QPushButton(tr("Cancel"), this);
    connect( m_btnOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( m_btnCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );

	m_vLayout = new QVBoxLayout(this);
	
	// general group
	QLabel *general;
	general = new QLabel("General", this);
	general->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	QFrame *line;
	line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

	QHBoxLayout* hl = new QHBoxLayout(this);
	hl->addWidget(general);
	hl->addWidget(line);

	QLabel *label;
	label = new QLabel("Name: ", this);
	QLabel *label_2;
	label_2 = new QLabel("Opacity: ", this);

	QVBoxLayout* vl = new QVBoxLayout(this);
	vl->addWidget(label);
	vl->addWidget(label_2);

	QLineEdit *lineEdit;
	lineEdit = new QLineEdit("New layer", this);

	OpacitySpinBox = new QSpinBox(this);
    OpacitySpinBox->setMaximum(255);
    OpacitySpinBox->setValue(255);
	connect(OpacitySpinBox, SIGNAL(valueChanged(int)), this, SLOT(SetOpacitySliderPosition(int)));

	OpacitySlider = new QSlider(this);
    OpacitySlider->setMaximum(255);
    OpacitySlider->setValue(255);
    OpacitySlider->setOrientation(Qt::Horizontal);
	connect(OpacitySlider, SIGNAL(valueChanged(int)), this, SLOT(SetOpacitySpinBoxPosition(int)));

	QHBoxLayout* hl1 = new QHBoxLayout(this);
	hl1->addWidget(OpacitySpinBox);
	hl1->addWidget(OpacitySlider);

	QVBoxLayout* vl2 = new QVBoxLayout(this);
	vl2->addWidget(lineEdit);
	vl2->addLayout(hl1);

	QHBoxLayout* hl2 = new QHBoxLayout(this);
	hl2->addLayout(vl);
	hl2->addLayout(vl2);

	checkBox = new QCheckBox("Visible", this);
    checkBox->setChecked(true);
	
	// colormap group
	QLabel *colormap;
	colormap = new QLabel("Colormap", this);
	colormap->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	QFrame *line2;
	line2 = new QFrame(this);
    line2->setFrameShape(QFrame::HLine);
    line2->setFrameShadow(QFrame::Sunken);

	QHBoxLayout* hl3 = new QHBoxLayout(this);
	hl3->addWidget(colormap);
	hl3->addWidget(line2);

	QLabel *label_3;
	label_3 = new QLabel("Type: ", this);
	QLabel *label_4;
	label_4 = new QLabel("PxMin: ", this);
	QLabel *label_5;
	label_5 = new QLabel("PxMax: ", this);

	QVBoxLayout* vl3 = new QVBoxLayout(this);
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

	QHBoxLayout* hl4 = new QHBoxLayout(this);
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

	QHBoxLayout* hl5 = new QHBoxLayout(this);
	hl5->addWidget(PxMaxSpinBox);
	hl5->addWidget(PxMaxSlider);

	comboBox = new QComboBox(this);
    comboBox->insertItems(0, QStringList()
     << QApplication::translate("Dialog", "Multicolor", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Dialog", "Singlecolor", 0, QApplication::UnicodeUTF8)
    );
	comboBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxChange(int)));

	QVBoxLayout* vl4 = new QVBoxLayout(this);
	vl4->addWidget(comboBox);
	vl4->addLayout(hl4);
	vl4->addLayout(hl5);

	QHBoxLayout* hmainc = new QHBoxLayout(this);
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
//	colorWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	
	QPalette palette;
    QBrush brush(QColor(0, 0, 255, 255));
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

	// button layout
    m_btnLayout = new QHBoxLayout(this);
    m_btnLayout->addStretch();    
    m_btnLayout->addWidget(m_btnOk);
    m_btnLayout->addWidget(m_btnCancel);

	// main vertical layout
	m_vLayout->addLayout(hl);
	m_vLayout->addLayout(hl2);
	m_vLayout->addWidget(checkBox);
	m_vLayout->addLayout(hl3);
	m_vLayout->addLayout(hmainc);
	m_vLayout->addWidget(groupBox);
	m_vLayout->addWidget(groupBox2);
    m_vLayout->addLayout(m_btnLayout);

	groupBox->setVisible(true);
	groupBox2->setVisible(false);
	cmType = 1;		// multi colormap
	color = QColor(0, 0, 255, 255);
}

//void FrAddLayerDialog::SetCaption(QString& caption){
//    this->setWindowTitle(caption);
//}
//
//QString FrAddLayerDialog::GetName(){
//    return m_txtName->text();
//}
//
//void FrAddLayerDialog::SetName(QString& value){
//    m_txtName->setText(value);
//}
//
//QString FrAddLayerDialog::GetDescription(){
//    return m_txtDescription->text();
//}
//
//void FrAddLayerDialog::SetDescription(QString& value){
//    m_txtDescription->setText(value);
//}

LayerSettings FrAddLayerDialog::GetLayerParams(){
	// TODO: add pxmin pxmax 
	LayerSettings ls;
	ls.color = color;
	ls.ColormapType = cmType;
	ls.Opacity = OpacitySpinBox->value();
	ls.Threshold = ThresholdSpinBox->value();
	ls.Visible = checkBox->isChecked();
	
	return ls;
}

bool FrAddLayerDialog::SimpleExec(){
    bool result = (this->exec() == QDialog::Accepted);
    return result;
}

void FrAddLayerDialog::onColorBtnClicked(){
	//QMessageBox(QMessageBox::Icon::Information, "Test", "test");
	color = QColorDialog::getColor(Qt::white, this);

	QPalette palette;
    QBrush brush(color);
    brush.setStyle(Qt::SolidPattern);	
	palette.setBrush(QPalette::Active, QPalette::Window, brush);
	palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
    palette.setBrush(QPalette::Disabled, QPalette::Window, brush);

	colorWidget->setPalette(palette);
}

void FrAddLayerDialog::onComboBoxChange(int index){
	switch(index){
		case 0:		// multi colormap
			cmType = 1;
			groupBox->setVisible(true);
			groupBox2->setVisible(false);
			break;
		case 1:		// single colormap
			cmType = 2;
			groupBox->setVisible(false);
			groupBox2->setVisible(true);
			break;
	}
}

void FrAddLayerDialog::SetOpacitySliderPosition(int value){
	OpacitySlider->setValue(value);
}

void FrAddLayerDialog::SetPxMinSliderPosition(int value){
	PxMinSlider->setValue(value);
}

void FrAddLayerDialog::SetPxMaxSliderPosition(int value){
	PxMaxSlider->setValue(value);
}

void FrAddLayerDialog::SetThresholdSliderPosition(int value){
	ThresholdSlider->setValue(value);
}

void FrAddLayerDialog::SetOpacitySpinBoxPosition(int value){
	OpacitySpinBox->setValue(value);
}

void FrAddLayerDialog::SetPxMinSpinBoxPosition(int value){
	PxMinSpinBox->setValue(value);
}

void FrAddLayerDialog::SetPxMaxSpinBoxPosition(int value){
	PxMaxSpinBox->setValue(value);
}

void FrAddLayerDialog::SetThresholdSpinBoxPosition(int value){
	ThresholdSpinBox->setValue(value);
}
