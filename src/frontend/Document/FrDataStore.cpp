#include "FrDataStore.h"
#include "FrMainDocument.h"
#include "FrImageDocObj.h"
#include "FrLayerDocObj.h"
#include "FrLayerSettings.h"
#include "FrRoiDocObj.h"
#include "FrPointsDocObj.h"
#include "FrViewDocObj.h"
#include "FrCommandController.h"
#include "FrMarshalling.h"

// Backend includes
#include "RtDataID.h"
#include "RtDataStore.h"
#include "RtDataImage.h"
#include "RtDataID.h"
#include "RtMaskImage.h"
#include "RtMRIImage.h"

// Some defines
#define DEF_MRI_ID "mri"
#define DEF_ROI_ID "mask"


FrDataStore::FrDataStore(FrMainDocument* document) 
: m_Document(document), m_Store(0){
    //m_Store = new RtDataStore();
    //m_Store->addOutputForNotify(this);
}

FrDataStore::~FrDataStore(){
    //if(m_Store) delete m_Store;
}

void FrDataStore::SetStore(RtDataStore *store){
    m_Store = store;
    m_Store->addListener(this);
}

void FrDataStore::notify(const RtDataID &dataID){
    if(m_Store == 0) return;

    // Data to data store may come from different thread context (i.e. from server)
    // This cause problems(crashes) with rendering subsystem. 
    // So we must marshall execution to main thread.
    FrDataStoreNotifyMarshallingEvent* event = 
        new FrDataStoreNotifyMarshallingEvent(
            const_cast<FrDataStore*>(this), dataID);

    if(!MarshalToMainThread(event)){
        // Cannot marshall... Something went wrong maybe try to update directly????
        // this->OnNotify(dataID);
        cout << "Marshalling failed for " << dataID << endl;
    }
}

void FrDataStore::OnNotify(const RtDataID &dataID){
    // For now support MRI and ROI
    if(dataID.getModuleID() == DEF_MRI_ID){
      RtMRIImage *img = dynamic_cast<RtMRIImage*>(m_Store->getData(dataID));
      if(img) {
	this->AddImageToDocument(img);
      }
    }
    else if(dataID.getModuleID() == DEF_ROI_ID){
      RtMaskImage *roi = dynamic_cast<RtMaskImage*>(m_Store->getData(dataID));
      if(roi) {
	// ohinds: comment for debug
	this->AddRoiToDocument(roi);
      }
    }
    else {
      cout << "FrDataStore::notify(): ignoring for " << dataID << endl;
    }
}

void FrDataStore::AddImageToDocument(RtData* data){
    RtMRIImage* img = dynamic_cast<RtMRIImage*>(data);
    if(img){
        if(img->seemsMosaic()){
            img->unmosaic();
        }

        // We have to add every RtMRIImage as a new timepoint
        // into existing Image Document Object
        std::vector<FrDocumentObj*> objects;
        m_Document->GetObjectsByType(objects, FrDocumentObj::ImageObject);

        FrImageDocObj* imgDO = 0;
        std::vector<FrDocumentObj*>::iterator it, itEnd(objects.end());
        for(it = objects.begin(); it != itEnd; ++it) {
            FrImageDocObj* ido = (FrImageDocObj*)(*it);

            RtDataID imageDocDataID(ido->GetDataID());

            // look for any timepoint (and maybe dataname)
            imageDocDataID.setTimePoint(DATAID_UNSET_VALUE); 
            imageDocDataID.setDataName("");

            if(imageDocDataID == img->getDataID()){
                // Found!
                imgDO = ido;
                break;
            }
        }

        if(imgDO != 0){
            // TODO: invalid image data may come
            // so we need to add checking... ???
            imgDO->AddTimePointData(img);
        }
        else {
            FrTabSettingsDocObj* tabSets = m_Document->GetCurrentTabSettings();
            FrLayerDocObj* imgLayer = new FrLayerDocObj(FrLayerSettings::LImage, img->getDataID(), 
                QString(img->getDataID().getDataName().c_str()));

            m_Document->Add(imgLayer);

            imgDO = new FrImageDocObj();

            // get view doc obj and set current timeseria
            //FrViewDocObj* viewDO = m_Document->GetCurrentViewObject();

            m_Document->Add(imgDO);
            imgDO->AddTimePointData(img);


              // initialize points doc object with new image
            FrPointsDocObj* pointsDO = new FrPointsDocObj();
            pointsDO->GetDimsFromImg(img);
            m_Document->Add(pointsDO);
        }

        //// test
        //// Remove points objects
        //m_Document->GetObjectsByType(objects, FrDocumentObj::PointsObject);
        //itEnd = objects.end();
        //for(it = objects.begin(); it != itEnd; ++it){
        //    m_Document->Remove( (*it) );
        //}        
        //
        //// initialize points doc object with new image
        //FrPointsDocObj* pointsDO = new FrPointsDocObj();
        //pointsDO->GetDimsFromImg(img);
        //m_Document->Add(pointsDO);
    }
}

void FrDataStore::AddRoiToDocument(RtData* data){
    RtMaskImage* roi = dynamic_cast<RtMaskImage*>(data);
    if(roi){
        if(roi->seemsMosaic()){
            roi->unmosaic();
        }
        
        // TODO: invalid roi data may come
        // so we need to add checking... ???
        FrRoiDocObj* roiDO = new FrRoiDocObj(roi);
        m_Document->Add(roiDO);
    }
}

void FrDataStore::AddData(RtData *data){
    m_Store->setData(data);
}

void FrDataStore::GetStuff(std::vector<RtDataID>& data){
    data.clear();

    if(m_Store != 0){

        std::map<RtDataID,RtData*,RtDataIDCompare>::iterator it, itEnd;
        it = m_Store->store.begin();
        itEnd = m_Store->store.end();

        while(it != itEnd){
            data.push_back(it->first);
            ++it;
        }
    }
}

void FrDataStore::GetAvailableData(std::vector<DataItem>& data){
    // Get descriptors of data from data store
    std::vector<RtDataID> dataIDs;
    this->GetStuff(dataIDs);

    // Prepare list of items to add 
    std::vector<RtDataID>::iterator i, iEnd(dataIDs.end());
    for(i = dataIDs.begin(); i != iEnd; ++i){
        // find root item here
        RtDataID& dataID = (*i);
        std::vector<DataItem>::iterator it = 
            std::find_if( data.begin(), data.end(), 
                          MatchByID(dataID) );

        // if not found create new and add 
        if(it == data.end()){

            DataItem newDataItem;
            newDataItem.ModuleID = dataID.getModuleID();
            newDataItem.TimeSeries = dataID.getSeriesNum();
            data.push_back(newDataItem);
            
            it = data.end();
            --it;
        }
        
        // add item to that root
        it->Items.push_back(dataID);
    }

}