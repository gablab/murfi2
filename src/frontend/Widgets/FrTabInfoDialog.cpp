#include "FrTabInfoDialog.h"

#include "Qt/QLayout.h"
#include "Qt/QLabel.h"
#include "Qt/QLineEdit.h"
#include "Qt/QPushButton.h"
#include "Qt/QBoxLayout.h"
#include "Qt/QSizePolicy.h"


FrTabInfoDialog::FrTabInfoDialog(QWidget* parent, bool isModal)
: QDialog(parent){
    
    this->setModal(isModal);
    // this->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    
    m_btnOk = new QPushButton(tr("OK"), this);
    m_btnCancel = new QPushButton(tr("Cancel"), this);
    connect( m_btnOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( m_btnCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );

    QHBoxLayout* m_hLayout = new QHBoxLayout(this);
    m_hLayout->addStretch();    
    m_hLayout->addWidget(m_btnOk);
    m_hLayout->addWidget(m_btnCancel);

    m_lblInfo = new QLabel(this);
    m_lblInfo->setText("Some information");

    m_txtName = new QLineEdit(this);
    m_txtDescription = new QLineEdit(this);

    QVBoxLayout* m_vLayout = new QVBoxLayout(this);
    m_vLayout->addSpacing(6);
    m_vLayout->addWidget(m_lblInfo);
    m_vLayout->addSpacing(6);
    m_vLayout->addWidget(m_txtName);
    m_vLayout->addSpacing(6);
    m_vLayout->addWidget(m_txtDescription);
    m_vLayout->addSpacing(6);
    m_vLayout->addLayout(m_hLayout, Qt::AlignRight);
    
    //QSize sh = sizeHint().expandedTo( QSize(300, 10) );
    //resize( sh.width(), m_vLayout->heightForWidth(sh.width()) );

}

void FrTabInfoDialog::SetCaption(QString& caption){
    this->setWindowTitle(caption);
}

void FrTabInfoDialog::SetInfo(QString& value){
    m_lblInfo->setText(value);
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