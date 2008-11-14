#include "FrListToolWidget.h"

#include "Qt/qlistwidget.h"
#include "Qt/qlabel.h"
#include "Qt/qlayout.h"
#include "Qt/qapplication.h"

#define DEF_LIST_HEIGHT 100
#define DEF_LIST_WIDTH  180


FrListToolWidget::FrListToolWidget(QWidget* parent)
: FrBaseToolWidget(parent){
    roiList = new QListWidget(this);
    roiList->setFixedHeight(DEF_LIST_HEIGHT);
    roiList->setFixedWidth(DEF_LIST_WIDTH);

    connect(roiList, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), 
        this, SLOT(ROIListItemChanged(QListWidgetItem*, QListWidgetItem*)));

    label = new QLabel("test", this);  

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(label);
    mainLayout->addWidget(roiList);

    this->SetList();      // test
    this->setFixedHeight(this->sizeHint().height());
}

void FrListToolWidget::SetName(QString name){
    label->setText(name);
}

void FrListToolWidget::SetList(){
    // add items to list
    roiList->clear();

    QListWidgetItem *item1 = new QListWidgetItem("ROI 1", roiList);
    item1->setCheckState(Qt::Checked);
    QListWidgetItem *item2 = new QListWidgetItem("ROI 2", roiList);
    item2->setCheckState(Qt::Checked);
    QListWidgetItem *item3 = new QListWidgetItem("ROI 3", roiList);
    item3->setCheckState(Qt::Checked);
}

void FrListToolWidget::ROIListItemChanged(QListWidgetItem* current, QListWidgetItem* previous){
    emit ParamsChanged();    
}