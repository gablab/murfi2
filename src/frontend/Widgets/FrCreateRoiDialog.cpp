#include "FrCreateRoiDialog.h"
#include "FrSpinSliderWidget.h"
#include "FrBrowseFileWidget.h"
#include "FrEmptyToolWidget.h"
#include "FrSpinToolWidget.h"

#include "Qt/qlayout.h"
#include "Qt/qlabel.h"
#include "Qt/qpushbutton.h"
#include "Qt/qboxlayout.h"
#include "Qt/qsizepolicy.h"
#include "Qt/qgroupbox.h"
#include "Qt/qcombobox.h"
#include "Qt/qvariant.h"
#include "Qt/qstackedlayout.h"


// Implementation
FrCreateRoiDialog::FrCreateRoiDialog(QWidget* parent, bool isModal)
: QDialog(parent){
    this->setModal(isModal);
    this->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

    QHBoxLayout* selectionLayout = this->CreateSelectionLayout();    
    QHBoxLayout* btnLayout = this->CreateButtonLayout();
    QGroupBox* gbParams = new QGroupBox(QString("Additional Parameters"), this);
    this->InitializeParams(gbParams);
    
    // Init main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(selectionLayout);
    mainLayout->addWidget(gbParams);
    mainLayout->addLayout(btnLayout);

    this->setFixedHeight(this->sizeHint().height());
    this->setFixedWidth(this->sizeHint().width());
}

QHBoxLayout* FrCreateRoiDialog::CreateSelectionLayout(){
    // create widgets
    QLabel* lbl = new QLabel(QString("ROI creation :"), this);
    m_cmbAction = new QComboBox(this);
        
    m_cmbAction->addItem(QString("Create empty"),      QVariant(FrCreateRoiDialog::CreateEmpty)); 
    m_cmbAction->addItem(QString("Threshold to mask"), QVariant(FrCreateRoiDialog::ThresholdToMask)); 
    m_cmbAction->addItem(QString("Load from file"),    QVariant(FrCreateRoiDialog::LoadFromFile)); 
    connect( m_cmbAction, SIGNAL(currentIndexChanged(int)), this, SLOT(OnActionIndexChanged(int)) );
    
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(lbl);
    layout->addWidget(m_cmbAction);

    QHBoxLayout* result = new QHBoxLayout();
    result->addLayout(layout);
    result->addStretch();
    return result;
}

void FrCreateRoiDialog::InitializeParams(QGroupBox* gb){
    m_emptyWidget = new FrEmptyToolWidget();

    m_threshWidget = new FrSpinToolWidget();
    m_threshWidget->SetName("Threshold : ");
    m_threshWidget->SetMinMaxValues(0, 100);
    m_threshWidget->SetValue(0);
    
    m_fileWidget = new FrBrowseFileWidget();
    m_fileWidget->SetFileName(QString(""));
        
    m_stacked = new QStackedLayout(gb);
    m_stacked->setSpacing(1);
    m_stacked->addWidget(m_emptyWidget);
    m_stacked->addWidget(m_threshWidget);
    m_stacked->addWidget(m_fileWidget);
}

QHBoxLayout* FrCreateRoiDialog::CreateButtonLayout(){
    // create buttons
    QPushButton* btnOk = new QPushButton(tr("OK"), this);
    QPushButton* btnCancel = new QPushButton(tr("Cancel"), this);
    connect( btnOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( btnCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(btnOk);
    btnLayout->addWidget(btnCancel);

    return btnLayout;
}

void FrCreateRoiDialog::SetCaption(QString& caption){
    this->setWindowTitle(caption);
}

FrCreateRoiDialog::Action FrCreateRoiDialog::GetAction(){
    int idx = m_cmbAction->currentIndex();
    int action = m_cmbAction->itemData(idx).toInt();
    return (FrCreateRoiDialog::Action)action;
}

double FrCreateRoiDialog::GetThreshold(){
    double result = double(m_threshWidget->GetValue()) / 100.0;
    return result;
}

QString FrCreateRoiDialog::GetFileName(){
    return m_fileWidget->GetFileName();
}

bool FrCreateRoiDialog::SimpleExec(){
    bool result = (this->exec() == QDialog::Accepted);
    return result;
}

void FrCreateRoiDialog::OnActionIndexChanged(int idx){
    m_stacked->setCurrentIndex(idx);
    
    //this->setFixedHeight(this->sizeHint().height());
    //this->resize(this->sizeHint().width(), 
    //             this->sizeHint().height());
}
