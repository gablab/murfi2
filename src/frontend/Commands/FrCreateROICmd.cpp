#include "FrCreateROICmd.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrCreateRoiDialog.h"
#include "FrCommandController.h"
#include "FrRoiDocObj.h"
#include "FrImageDocObj.h"
#include "FrLayerSettings.h"
#include "FrLayerDocObj.h"

#include "RtMaskImage.h"

#include "Qt/qmessagebox.h"
#include "Qt/qfile.h"

#define ALL_ITEMS_COUNT 5

FrCreateROICmd::FrCreateROICmd()
: m_CreateTest(false){
}

bool FrCreateROICmd::Execute(){
    // Check for safety
    if(!this->GetMainController()) return false;
    
    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    
    // Get images from document
    std::vector<FrDocumentObj*> images;
    doc->GetObjectsByType(images, FrDocumentObj::ImageObject);
    
    if(images.size() == 0){
        QMessageBox::critical(mv, QString("Creation of ROI"), 
            QString("Can't create ROI. None image opened.") );
        return false;
    }

    // Init used params
    enum FrCreateRoiDialog::Action action = FrCreateRoiDialog::None;
    double threshold = 0.0;
    QString fileName = QString("");

    if(m_CreateTest){
        // NOTE  have to change to contol by external
        action = FrCreateRoiDialog::CreateEmpty;
        threshold = 0.0;
        fileName = QString("");
    }
    else {
        FrCreateRoiDialog dlg(mv, true);
        if(!dlg.SimpleExec()) return false;

        action = dlg.GetAction();
        threshold = dlg.GetThreshold();
        fileName = dlg.GetFileName();
    }

    // process params
    bool result = false;
    FrImageDocObj* imgDO = (FrImageDocObj*)images[0];
    
    switch(action){
        case FrCreateRoiDialog::CreateEmpty:
            result = CreateEmptyMask(imgDO);
            break;
        case FrCreateRoiDialog::ThresholdToMask:
            result = ThresholdToMask(imgDO, threshold);
            break;
        case FrCreateRoiDialog::LoadFromFile:
            result = LoadFromFile(imgDO, fileName);
            break;
        default:break;
    }

    if(result) {
        FrBaseCmd::UpdatePipelineForID(ALL_LAYER_ID, FRP_FULL);
    }

    return result;
}

bool FrCreateROICmd::CreateEmptyMask(FrImageDocObj* imgDO){
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();

    RtMaskImage* m_MaskImage = new RtMaskImage();
    // Setup id
    m_MaskImage->getDataID().setModuleID("mask");
    m_MaskImage->getDataID().setSeriesNum(0);

    // NOTE: use timepoint 0 for now
    // May be better to use current timepoint...
    m_MaskImage->setInfo( *(imgDO->GetTimePointData(0)) );
        
    // init all values to default (not a ROI)    
    // DEF_MASK_VALUE == 0
    m_MaskImage->initToZeros();

    doc->AddDataToStore(m_MaskImage); 
    
    return true;
}

bool FrCreateROICmd::ThresholdToMask(FrImageDocObj* imgDO, double threshold){
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();

    RtMaskImage* m_MaskImage = new RtMaskImage();
    // NOTE: use timepoint 0 for now
    // May be better to use current timepoint...
    m_MaskImage = new RtMaskImage(*(imgDO->GetTimePointData(0)), threshold);
    // Setup id
    m_MaskImage->getDataID().setModuleID("mask");
    m_MaskImage->getDataID().setSeriesNum(0);

    doc->AddDataToStore(m_MaskImage);    

    return true;
}

bool FrCreateROICmd::LoadFromFile(FrImageDocObj* imgDO, QString filename){
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    
    bool result = false;

    if(QFile::exists(filename)){
        std::string stdFileName(filename.toAscii());
        RtMaskImage* m_MaskImage = new RtMaskImage();

        if(m_MaskImage->readNifti(stdFileName)){
            // have to unmosaic
            if(m_MaskImage->seemsMosaic()){
                m_MaskImage->unmosaic();
            }

            // check params
            bool hasSameDim = (m_MaskImage->getDims().size() 
                == imgDO->GetTimePointData(0)->getDims().size());
            bool hasSamePDim = (m_MaskImage->getPixDims().size() 
                == imgDO->GetTimePointData(0)->getPixDims().size());
            if(hasSameDim && hasSamePDim){

                for(int i=0; i < m_MaskImage->getDims().size(); ++i){

                    hasSameDim = (m_MaskImage->getDim(i) == 
                                  imgDO->GetTimePointData(0)->getDim(i));
                    hasSamePDim = (m_MaskImage->getPixDim(i) ==
                                   imgDO->GetTimePointData(0)->getPixDim(i));
                    if(!hasSameDim || !hasSamePDim) break;
                }

                // if OK set loaded image as current
                if(hasSameDim && hasSamePDim){
                    // Setup id
                    m_MaskImage->getDataID().setModuleID("mask");
                    m_MaskImage->getDataID().setSeriesNum(0);

                    doc->AddDataToStore(m_MaskImage);
                    result = true;
                }
                else{

                    delete m_MaskImage;
                    result = false;
                }
            }
        }
    }

    return result;
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrCreateROICmd::CanUndo(){
    return false;
}

bool FrCreateROICmd::Undo(){
    return false;
}

bool FrCreateROICmd::Redo(){
    return false;
}
