#include "FrBrowseFileWidget.h"

// Qt stuff
#include "Qt/qlayout.h"
#include "Qt/qpushbutton.h"
#include "Qt/qlineedit.h"
#include "Qt/qapplication.h"
#include "Qt/qfiledialog.h"

FrBrowseFileWidget::FrBrowseFileWidget(QWidget* parent){
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    
    m_txtFile = new QLineEdit(QString(""), this);
    m_btnBrowse = new QPushButton(QString("Browse..."), this);
    connect(m_btnBrowse, SIGNAL(clicked()), this, SLOT(OnBrowseClick()));
    
    m_layout = new QHBoxLayout(this);
    m_layout->addWidget(m_txtFile);
    m_layout->addWidget(m_btnBrowse);
    this->setFixedHeight(this->sizeHint().height());
}

QString FrBrowseFileWidget::GetFileName(){
    return m_txtFile->text();
}

void FrBrowseFileWidget::SetFileName(const QString& fileName){
    m_txtFile->setText(fileName);
}
    

void FrBrowseFileWidget::OnBrowseClick(){
    QString fileName = QFileDialog::getOpenFileName(
        0L, tr("Open File"), tr(""), 
        tr("Maks Image (*.nii)"));

    m_txtFile->setText(fileName);
}