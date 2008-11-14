#include "FrListToolWidget.h"

#include "Qt/qlistwidget.h"
#include "Qt/qlabel.h"
#include "Qt/qlayout.h"
#include "Qt/qapplication.h"


FrListToolWidget::FrListToolWidget(QWidget* parent)
: FrBaseToolWidget(parent){
    roiList = new QListWidget(this);
    connect(roiList, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), 
        this, SLOT(ROIListItemChanged(QListWidgetItem*, QListWidgetItem*)));

    label = new QLabel("test", this);  

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(label);
    mainLayout->addWidget(roiList);

    SetList();      // test
}

void FrListToolWidget::SetName(QString name){
    label->setText(name);
}

void FrListToolWidget::SetList(){
    // add items to list
    roiList->clear();

    QListWidgetItem *item = new QListWidgetItem("test", roiList);
}

void FrListToolWidget::ROIListItemChanged(QListWidgetItem* current, QListWidgetItem* previous){
    emit ParamsChanged();    
}