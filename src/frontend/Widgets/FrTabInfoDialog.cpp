#include "FrTabInfoDialog.h"

#include "Qt/qlayout.h"
#include "Qt/qlabel.h"
#include "Qt/qlineedit.h"
#include "Qt/qpushbutton.h"
#include "Qt/qboxlayout.h"
#include "Qt/qsizepolicy.h"


FrTabInfoDialog::FrTabInfoDialog(QWidget* parent, bool isModal)
: QDialog(parent){    
    this->setModal(isModal);
    this->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
        
    // create lables and text edits
    m_lblName = new QLabel(tr("Tab Name:"), this);
    m_txtName = new QLineEdit(tr(""), this);
    m_lblDescription = new QLabel(tr("Tab Description:"), this); 
    m_txtDescription = new QLineEdit(tr(""), this);

    m_vLayout = new QVBoxLayout(this);
    m_vLayout->addWidget(m_lblName);
    m_vLayout->addWidget(m_txtName);
    m_vLayout->addWidget(m_lblDescription);
    m_vLayout->addWidget(m_txtDescription);

    // create buttons
    m_btnOk = new QPushButton(tr("OK"), this);
    m_btnCancel = new QPushButton(tr("Cancel"), this);
    connect( m_btnOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( m_btnCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );

    m_btnLayout = new QHBoxLayout();
    m_btnLayout->addStretch();    
    m_btnLayout->addWidget(m_btnOk);
    m_btnLayout->addWidget(m_btnCancel);
    m_vLayout->addLayout(m_btnLayout);
}

void FrTabInfoDialog::SetCaption(QString& caption){
    this->setWindowTitle(caption);
}

QString FrTabInfoDialog::GetName(){
    return m_txtName->text();
}

void FrTabInfoDialog::SetName(QString& value){
    m_txtName->setText(value);
}

QString FrTabInfoDialog::GetDescription(){
    return m_txtDescription->text();
}

void FrTabInfoDialog::SetDescription(QString& value){
    m_txtDescription->setText(value);
}

bool FrTabInfoDialog::SimpleExec(){
    bool result = (this->exec() == QDialog::Accepted);
    return result;
}
