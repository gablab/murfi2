#include "FrLayerDialog.h"
#include "FrSpinSliderWidget.h"
#include "FrColormapWidget.h"
#include "FrSettings.h"

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

#define MULTICOLOR_ITEM_IDX 0 
#define SINGLECOLOR_ITEM_IDX 1 

FrLayerDialog::FrLayerDialog(QWidget* parent, bool isModal)
: QDialog(parent){    
    this->setModal(isModal);
    this->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
        
    QHBoxLayout* titleLayout = this->CreateTitleLayout();

    // Init params layout
    QHBoxLayout* generalLayout = this->CreateGeneralLayout();
    m_chkVisibility = new QCheckBox(QString("Visible"), this);
    m_chkVisibility->setChecked(true);
    
    QVBoxLayout* paramsLayout = new QVBoxLayout();
    paramsLayout->addLayout(generalLayout);
    paramsLayout->addWidget(m_chkVisibility);

    m_colormapWidget = new FrColormapWidget(this);

    QHBoxLayout* btnLayout = this->CreateButtonLayout();

	// Init main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(titleLayout);
    mainLayout->addLayout(paramsLayout);
    mainLayout->addWidget(m_colormapWidget);
    mainLayout->addLayout(btnLayout);
}

QHBoxLayout* FrLayerDialog::CreateTitleLayout(){
    // Create title
    QLabel *lblGeneral = new QLabel("General", this);
	lblGeneral->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	QFrame *frmGeneral = new QFrame(this);
    frmGeneral->setFrameShape(QFrame::HLine);
    frmGeneral->setFrameShadow(QFrame::Sunken);

    QHBoxLayout* titleLayout = new QHBoxLayout();
	titleLayout->addWidget(lblGeneral);
	titleLayout->addWidget(frmGeneral);

    return titleLayout;
}

QHBoxLayout* FrLayerDialog::CreateGeneralLayout(){
    
    // Create labels
	QLabel* lblName = new QLabel("Name: ", this);
	QLabel* lblOpacity = new QLabel("Opacity: ", this);

	QVBoxLayout* lblLayout = new QVBoxLayout();
	lblLayout->addWidget(lblName);
	lblLayout->addWidget(lblOpacity);

    // Create widgets
    m_txtName = new QLineEdit(QString("New Layer"), this);
    m_opacityWidget = new FrSpinSliderWidget(this);
    m_opacityWidget->SetMinMax(0, 100);
    m_opacityWidget->SetValue(100);

    QVBoxLayout* wgtLayout = new QVBoxLayout();
    wgtLayout->addWidget(m_txtName);
    wgtLayout->addWidget(m_opacityWidget);

    //Result
    QHBoxLayout* result = new QHBoxLayout();
    result->addLayout(lblLayout);
    result->addLayout(wgtLayout);
    return result;
}

QHBoxLayout* FrLayerDialog::CreateButtonLayout(){
    // create buttons
    m_btnOk = new QPushButton(tr("OK"), this);
    m_btnCancel = new QPushButton(tr("Cancel"), this);
    connect( m_btnOk, SIGNAL( clicked() ), this, SLOT( OnBtnOKClicked() ) );
    connect( m_btnCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(m_btnOk);
    btnLayout->addWidget(m_btnCancel);

    return btnLayout;
}

void FrLayerDialog::SetCaption(QString& caption){
    this->setWindowTitle(caption);
}

void FrLayerDialog::GetLayerParams(FrLayerSettings& layerSets){
    // Common props
    layerSets.Name = m_txtName->text();
	layerSets.ID = BAD_LAYER_ID;

    // Get opacity amd normalize it
    layerSets.Opacity = double(m_opacityWidget->GetValue());
    layerSets.Opacity /= double(m_opacityWidget->GetMaximum());

    layerSets.Visibility = m_chkVisibility->isChecked();

    // TBC props
    InitTbcDefault(&layerSets.TbcSettings);

    // Colormap props
    m_colormapWidget->GetColormapParams(layerSets.ColormapSettings);
}

void FrLayerDialog::SetLayerParams(FrLayerSettings& layerSets){
    // Common props
	m_txtName->setText(layerSets.Name);
    m_chkVisibility->setChecked(layerSets.Visibility);
    int opacity = int(layerSets.Opacity * m_opacityWidget->GetMaximum());
    m_opacityWidget->SetValue(opacity);
        
    // Colormap props
    m_colormapWidget->SetColormapParams(layerSets.ColormapSettings);
}

bool FrLayerDialog::SimpleExec(){
    bool result = (this->exec() == QDialog::Accepted);
    return result;
}

#define VALIDATING_PIPELINE for(int i=0; i < 1; ++i)
void FrLayerDialog::OnBtnOKClicked(){
    bool isValid = false;
    QString message = "Validation OK";
    VALIDATING_PIPELINE {
        FrColormapSettings params;
        m_colormapWidget->GetColormapParams(params);

        // Check min and max
        if(params.MinValue >= params.MaxValue){
            message = "MIN value has to be less then MAX value...";
            break;
        }
        isValid = true;
    }
    
    // Process validation results 
    if(isValid){
        this->accept();
    }
    else {        
        QMessageBox::critical(this, "Validating Error", message);
    }
}