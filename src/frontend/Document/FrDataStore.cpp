#include "FrDataStore.h"
#include "FrMainDocument.h"
#include "FrImageDocObj.h"
#include "FrRoiDocObj.h"

// Backend includes
#include "RtDataID.h"
#include "RtDataStore.h"
#include "RtDataImage.h"
#include "RtMaskImage.h"
#include "RtMRIImage.h"

// Some defines
#define DEF_MRI_ID "mri"
#define DEF_ROI_ID "mask"


FrDataStore::FrDataStore(FrMainDocument* document) 
: m_Document(document), m_Store(0){
    m_Store = new RtDataStore();
    m_Store->addOutputForNotify(this);
}

FrDataStore::~FrDataStore(){
    if(m_Store) delete m_Store;
}

void FrDataStore::setData(RtData *data){
    // Process incoming data here
    std::string id = data->getDataID().getModuleID();
        
    // For now support MRI and ROI
    if(id == DEF_MRI_ID){
        this->AddImageToDocument(data);
    }
    else if(id == DEF_ROI_ID){
        this->AddRoiToDocument(data);
    }
    else {
        // TODO: Undefined data come
        // so we need to handle this somehow...
        delete data;
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
            if(ido->GetSeriesNumber() == 
               img->getDataID().getSeriesNum()){
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
            imgDO = new FrImageDocObj();
            m_Document->Add(imgDO);
            imgDO->AddTimePointData(img);
        }
        
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