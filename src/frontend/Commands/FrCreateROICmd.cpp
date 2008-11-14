#include "FrCreateROICmd.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrCreateRoiDialog.h"

#include "FrRoiDocObj.h"
#include "FrImageDocObj.h"

#include "Qt/qmessagebox.h"

#define ALL_ITEMS_COUNT 5

FrCreateROICmd::FrCreateROICmd(){
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
    
    FrCreateRoiDialog dlg(mv, true);
    if(!dlg.SimpleExec()) return false;

    bool result = false;
    FrRoiDocObj* roiDO = 0L;
    FrImageDocObj* imgDO = (FrImageDocObj*)images[0];
    switch(dlg.GetAction()){
        case FrCreateRoiDialog::CreateEmpty:
            roiDO = new FrRoiDocObj();
            roiDO->CreateEmptyMaskImage(imgDO);
            result = doc->Add(roiDO);
            break;
        case FrCreateRoiDialog::ThresholdToMask:
            roiDO = new FrRoiDocObj();
            roiDO->CreateByThresholdMaskImage(imgDO, dlg.GetThreshold());
            result = doc->Add(roiDO);
            break;
        case FrCreateRoiDialog::LoadFromFile:
            roiDO = new FrRoiDocObj();
            roiDO->LoadFromFile(imgDO, dlg.GetFileName());
            result = doc->Add(roiDO);
            break;
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
