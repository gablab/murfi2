#include "FrDataStoreDialog.h"
#include "FrMainDocument.h"
#include "FrDataStore.h"
#include "FrBrowseFileWidget.h"
#include "FrLayerDocObj.h"
#include "FrTreeWidgetItem.h"

#include "Qt/qtreewidget.h"
#include "Qt/qlayout.h"
#include "Qt/qlabel.h"
#include "Qt/qpushbutton.h"
#include "Qt/qboxlayout.h"
#include "Qt/qsizepolicy.h"
#include "Qt/qgroupbox.h"
#include "Qt/qvariant.h"

#include <algorithm>
#include <string>

struct RootItem {
    std::string ModuleID;
    int TimeSeries;

    std::vector<RtDataID> Items;
};

struct MatchByID{
    std::string moduleID;
    int seriesNum;

    MatchByID(RtDataID dataID){
        moduleID = dataID.getModuleID();
        seriesNum = dataID.getSeriesNum();
    }
    bool operator()(RootItem& item){
        return (seriesNum == item.TimeSeries &&
                moduleID == item.ModuleID);
    }
};


// Implementation
FrDataStoreDialog::FrDataStoreDialog(QWidget* parent, bool isModal)
: QDialog(parent){
    this->setModal(isModal);
    this->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
   
    m_tvDataStore = this->CreateTreeViewWidget();
    //m_tvApplication = this->CreateTreeViewWidget();

    QHBoxLayout* wgtLayout = new QHBoxLayout();
    wgtLayout->addWidget(m_tvDataStore);
    //wgtLayout->addWidget(m_tvApplication);

    QHBoxLayout* btnLayout = this->CreateButtonLayout();
   
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(wgtLayout);
    mainLayout->addLayout(btnLayout);

    this->setFixedHeight(this->sizeHint().height());
    this->setFixedWidth(this->sizeHint().width());

    connect(m_tvDataStore, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(SelectID(QTreeWidgetItem*, int)));
}

QTreeWidget* FrDataStoreDialog::CreateTreeViewWidget(){
    QTreeWidget* treeView = new QTreeWidget(this);
    treeView->setColumnCount(2);

    QStringList headerLabels;
    headerLabels.push_back(QString("Module"));
    headerLabels.push_back(QString("Data ID"));
    treeView->setHeaderLabels(headerLabels);

    return treeView;
}


QHBoxLayout* FrDataStoreDialog::CreateButtonLayout(){
    // create buttons
    QPushButton* btnClose = new QPushButton(tr("Cancel"), this);
    connect( btnClose, SIGNAL( clicked() ), this, SLOT( reject() ) );

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(btnClose);

    return btnLayout;
}

void FrDataStoreDialog::SetCaption(QString& caption){
    this->setWindowTitle(caption);
}

bool FrDataStoreDialog::SimpleExec(){
    bool result = (this->exec() == QDialog::Accepted);
    return result;
}

void FrDataStoreDialog::Initialize(FrMainDocument* doc){
    // Get descriptors of data from data store
    std::vector<RtDataID> data;
    FrDataStore* dataStore = doc->GetDataStore();
    dataStore->GetStuff(data);
    
    // Prepare list of items to add 
    std::vector<RootItem> roots;
    std::vector<RtDataID>::iterator i, iEnd(data.end());
    for(i = data.begin(); i != iEnd; ++i){
        // find root item here
        RtDataID& dataID = (*i);
        std::vector<RootItem>::iterator it = 
            std::find_if( roots.begin(), roots.end(), 
                          MatchByID(dataID) );

        // if not found create new and add 
        if(it == roots.end()){

            RootItem newRootItem;
            newRootItem.ModuleID = dataID.getModuleID();
            newRootItem.TimeSeries = dataID.getSeriesNum();
            roots.push_back(newRootItem);
            
            it = roots.end();
            --it;
        }
        
        // add item to that root
        it->Items.push_back(dataID);
    }

    // Add data 
    QStringList itemValues;
    QList<QTreeWidgetItem*> rootItems;

    std::vector<RootItem>::iterator it, itEnd(roots.end());
    for(it = roots.begin(); it != itEnd; ++it){
        // Create root item
        itemValues.clear();
        itemValues.push_back(QString("%1").arg(it->ModuleID.c_str())); // Module
        // Data ID (Time Series for root)
        itemValues.push_back(QString("TS:%1").arg(it->TimeSeries)); 

        QTreeWidgetItem* rootItem = new QTreeWidgetItem(itemValues);
        rootItems.push_back(rootItem);

        std::vector<RtDataID>::iterator itr, itrEnd(it->Items.end());
        for(itr = it->Items.begin(); itr != itrEnd; ++itr){

            itemValues.clear();
            itemValues.push_back(QString("")); // Module is empty
            // Data ID (Time Point for child for root)
            itemValues.push_back(QString("%1").arg(itr->getTimePoint())); 

            FrTreeWidgetItem* childItem = new FrTreeWidgetItem(itemValues);
            childItem->SetID((*itr));

            // check if current image has been already added as layer
            if (IsImageAddedAsLayer(doc, (*itr)))
                childItem->setBackgroundColor(1, QColor(125,125,125));

            rootItem->addChild(childItem);//new QTreeWidgetItem(itemValues));
        }

    }
    m_tvDataStore->insertTopLevelItems(0, rootItems);
}

//bool FrDataStoreDialog::IsImageAddedAsLayer(FrMainDocument* doc, RtDataID id){
//    FrDocument::DocObjCollection objects;
//    doc->GetObjectsByType(objects, FrDocumentObj::LayerObject);
//    FrDocument::DocObjCollection::iterator it, itEnd(objects.end());
//
//    for(it = objects.begin(); it != itEnd; ++it){
//        FrLayerDocObj* layerDO = (FrLayerDocObj*)(*it);
//        RtDataID layerID = layerDO->GetSettings()->DataID;
//        if (layerID == id)
//            return true;
//    }
//
//    return false;
//}

void FrDataStoreDialog::SelectID(QTreeWidgetItem* item, int column){
    if (item->childCount() > 0)    // root item
        return;

    FrTreeWidgetItem* it = dynamic_cast<FrTreeWidgetItem*>(item);
  
    m_ID = it->GetID();
    
    this->accept();
}