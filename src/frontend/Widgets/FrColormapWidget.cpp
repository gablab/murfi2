#include "FrColormapWidget.h"
#include "FrSpinSliderWidget.h"
#include "FrUtils.h"

//Qt stuff
#include "Qt/QString.h"
#include "Qt/QLabel.h"
#include "Qt/QPushButton.h"
#include "Qt/QBoxLayout.h"
#include "Qt/QGroupBox.h"
#include "Qt/QFrame.h"
#include "Qt/QComboBox.h"
#include "Qt/QApplication.h"
#include "Qt/QColorDialog.h"

// Some defines 
#define MULTICOLOR_ITEM_IDX 0 
#define SINGLECOLOR_ITEM_IDX 1


FrColormapWidget::FrColormapWidget(QWidget* parent) 
: QWidget(parent), m_isBlocked(false), m_color(DEF_CM_COLOR),
  m_cmType(FrColormapSettings::MultiColor){
    // init widgets and layouts
	QHBoxLayout* titleLayout = this->CreateTitleLayout();
    
    QGridLayout* paramsLayout = this->CreateMainParamsLayout();
    
    m_gbSingle = this->CreateSingleParams();

    m_gbMulti = this->CreateMultiParams();
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(titleLayout);
    mainLayout->addLayout(paramsLayout);
    mainLayout->addWidget(m_gbSingle);
    mainLayout->addWidget(m_gbMulti);

    m_gbSingle->setVisible(false);
    m_gbMulti->setVisible(true);
}

QHBoxLayout* FrColormapWidget::CreateTitleLayout(){
    QLabel *lblColormap = new QLabel("Colormap", this);
	lblColormap->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);    

	QFrame *frmColormap = new QFrame(this);
    frmColormap->setFrameShape(QFrame::HLine);
    frmColormap->setFrameShadow(QFrame::Sunken);

    QHBoxLayout* titleLayout = new QHBoxLayout();
    titleLayout->addWidget(lblColormap);
    titleLayout->addWidget(frmColormap);

    return titleLayout;
}

QGridLayout* FrColormapWidget::CreateMainParamsLayout(){
    // Labels    
	QLabel* lblType = new QLabel("Type: ", this);
    QLabel* lblMin  = new QLabel("PxMin: ", this);
    QLabel* lblMax  = new QLabel("PxMax: ", this);

    // Widgets
    m_cmbType = new QComboBox(this);	
    m_cmbType->insertItem(MULTICOLOR_ITEM_IDX, QString("Multicolor"));
    m_cmbType->insertItem(SINGLECOLOR_ITEM_IDX, QString("Singlecolor"));     
    m_cmbType->setCurrentIndex(MULTICOLOR_ITEM_IDX);
    connect(m_cmbType, SIGNAL(currentIndexChanged(int)), 
            this, SLOT(OnComboBoxChange(int)));

    m_minWidget = new FrSpinSliderWidget(this);
    m_minWidget->SetMinMax(0, 255);
    m_minWidget->SetValue(0);
    connect(m_minWidget, SIGNAL(ValueChanged(int)),
            this, SLOT(OnSpinSliderValueChanged(int)));

    m_maxWidget = new FrSpinSliderWidget(this);
    m_maxWidget->SetMinMax(0, 255);
    m_maxWidget->SetValue(255);
    connect(m_maxWidget, SIGNAL(ValueChanged(int)),
            this, SLOT(OnSpinSliderValueChanged(int)));
    
    // layout widgets
    int row = 0;
    QGridLayout* result = new QGridLayout();
    result->addWidget(lblType, row, 0);
    result->addWidget(m_cmbType, row, 1);
    row++;

    result->addWidget(lblMin, row, 0);
    result->addWidget(m_minWidget, row, 1);
    row++;

    result->addWidget(lblMax, row, 0);
    result->addWidget(m_maxWidget, row, 1);
    row++;

    return result;
}

QGroupBox* FrColormapWidget::CreateSingleParams(){
    QGroupBox* result = new QGroupBox("Additional Params", this);
	QLabel* lblColor = new QLabel("Selected Color: ", result);

	m_wgtColor = new QWidget(result);
	m_wgtColor->setSizeIncrement(100, 20);
    m_wgtColor->setMinimumSize(24, 24);
	m_wgtColor->setAutoFillBackground(true);
    this->UpdateColorWidget(QColor(DEF_CM_COLOR));

	QPushButton* btnColor = new QPushButton("Color", result);
	connect(btnColor, SIGNAL(clicked()), 
            this, SLOT(OnBtnColorClicked()));

	QHBoxLayout* layout = new QHBoxLayout(result);
	layout->addWidget(lblColor);
	layout->addWidget(m_wgtColor);
	layout->addWidget(btnColor);

    return result;
}

QGroupBox* FrColormapWidget::CreateMultiParams(){
    QGroupBox* result = new QGroupBox("Additional Params", this);

    QLabel* label = new QLabel(QString("PxMid: "), result); 

    m_midWidget = new FrSpinSliderWidget(result);
    m_midWidget->SetMinMax(0, 100);
    m_midWidget->SetValue(50);
    connect(m_midWidget, SIGNAL(ValueChanged(int)),
            this, SLOT(OnSpinSliderValueChanged(int)));
    
    QLabel* label2 = new QLabel(QString("Threshold: "), result); 

    m_threshWidget = new FrSpinSliderWidget(result);
    m_threshWidget->SetMinMax(0, 10);
    m_threshWidget->SetValue(4);
    connect(m_threshWidget, SIGNAL(ValueChanged(int)),
            this, SLOT(OnSpinSliderValueChanged(int)));

	QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(label);
    layout->addWidget(label2);
    
    QVBoxLayout* layout2 = new QVBoxLayout();
    layout2->addWidget(m_midWidget);
    layout2->addWidget(m_threshWidget);

    QHBoxLayout* layout3 = new QHBoxLayout(result);
    layout3->addLayout(layout);
    layout3->addLayout(layout2);

    return result;
}

void FrColormapWidget::OnComboBoxChange(int index){
    switch(index){
		case MULTICOLOR_ITEM_IDX:
            m_cmType = FrColormapSettings::MultiColor;
			m_gbSingle->setVisible(false);
            m_gbMulti->setVisible(true);
			break;
		case SINGLECOLOR_ITEM_IDX:
            m_cmType = FrColormapSettings::SingleColor;
            m_gbMulti->setVisible(false);
			m_gbSingle->setVisible(true);            
			break;
	}

    if(!m_isBlocked){
        emit ParamsChanged();
    }
}

void FrColormapWidget::OnSpinSliderValueChanged(int value){
    if(!m_isBlocked){
        emit ParamsChanged();
    }
}

void FrColormapWidget::OnBtnColorClicked(){
    QColor newColor = QColorDialog::getColor(m_color, this);
    if(newColor.isValid()){
        this->UpdateColorWidget(newColor);
        emit ParamsChanged();
    }
}

void FrColormapWidget::GetColormapParams(FrColormapSettings& params){
    // Colormap props
    params.Type = m_cmType;
    params.MinValue = m_minWidget->GetValue();
    params.MaxValue = m_maxWidget->GetValue();
	params.MidValue = m_midWidget->GetValue();
	params.Threshold = m_threshWidget->GetValue();
    params.Color = m_color;
}

void FrColormapWidget::SetColormapParams(FrColormapSettings& params){
    // Set colormap settings
    m_minWidget->SetValue(params.MinValue);
    m_maxWidget->SetValue(params.MaxValue);
	m_midWidget->SetValue(params.MidValue);
	m_threshWidget->SetValue(params.Threshold);

    switch(params.Type){
        case FrColormapSettings::MultiColor:
            m_cmbType->setCurrentIndex(MULTICOLOR_ITEM_IDX);
            break;
        case FrColormapSettings::SingleColor:
            m_cmbType->setCurrentIndex(SINGLECOLOR_ITEM_IDX);
            break;
        default:
            // Do nothing here
            break;
    }

    // Set color    
    UpdateColorWidget(params.Color);
}

void FrColormapWidget::UpdateColorWidget(QColor& color){
    // Set color
    m_color = color;

    // Update widget
    QPalette palette;
    QBrush brush(m_color);
    brush.setStyle(Qt::SolidPattern);	
    palette.setBrush(QPalette::Active, QPalette::Window, brush);
    palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
    palette.setBrush(QPalette::Disabled, QPalette::Window, brush);
    m_wgtColor->setPalette(palette);
}