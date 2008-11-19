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

    this->setFixedHeight(this->sizeHint().height());
}

void FrListToolWidget::SetName(QString name){
    label->setText(name);
}

void FrListToolWidget::ClearAll(){
    roiList->clear();
}

void FrListToolWidget::AddListItem(int ID, QString& name){
    QListWidgetItem *item = new QListWidgetItem(name, roiList);
    item->setToolTip(QString("%1 : %2").arg(ID).arg(name));
    item->setData(Qt::UserRole, QVariant(ID));
    item->setCheckState(Qt::Unchecked);
}

int FrListToolWidget::GetCurrentItemID(){
    int ID = -1;
    QListWidgetItem *item = roiList->currentItem();
    if(item){
        ID = item->data(Qt::UserRole).toInt();
    }
    return ID;
}

void FrListToolWidget::ROIListItemChanged(QListWidgetItem* current, QListWidgetItem* previous){
    emit ParamsChanged();
}
