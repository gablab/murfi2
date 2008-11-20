#include "FrGraphDialog.h"

#include "Qt/qlayout.h"
#include "Qt/qlabel.h"
#include "Qt/qlineedit.h"
#include "Qt/qpushbutton.h"
#include "Qt/qboxlayout.h"
#include "Qt/qsizepolicy.h"


FrGraphDialog::FrGraphDialog(QWidget* parent, bool isModal)
: QDialog(parent){    
    this->setModal(isModal);
    this->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

    // create buttons
    m_btnOk = new QPushButton(tr("OK"), this);
    m_btnCancel = new QPushButton(tr("Cancel"), this);
    connect( m_btnOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( m_btnCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );

    m_btnLayout = new QHBoxLayout();
    m_btnLayout->addStretch();    
    m_btnLayout->addWidget(m_btnOk);
    m_btnLayout->addWidget(m_btnCancel);
    
    
    m_vLayout = new QVBoxLayout(this);
    m_vLayout->addLayout(m_btnLayout);

    this->setFixedHeight(this->sizeHint().height());
    this->setFixedWidth(this->sizeHint().width());
}

void FrGraphDialog::SetCaption(QString& caption){
    this->setWindowTitle(caption);
}

bool FrGraphDialog::SimpleExec(){
    bool result = (this->exec() == QDialog::Accepted);
    return result;
}
