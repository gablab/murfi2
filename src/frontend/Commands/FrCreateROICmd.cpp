#include "FrCreateROICmd.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrCreateRoiDialog.h"

#include "FrRoiDocObj.h"
#include "FrImageDocObj.h"

#include "Qt/qmessagebox.h"

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
    FrRoiDocObj* roiDO = 0L;
    FrImageDocObj* imgDO = (FrImageDocObj*)images[0];
    switch(action){
        case FrCreateRoiDialog::CreateEmpty:
            roiDO = new FrRoiDocObj();
            roiDO->CreateEmptyMaskImage(imgDO);
            result = doc->Add(roiDO);
            break;
        case FrCreateRoiDialog::ThresholdToMask:
            roiDO = new FrRoiDocObj();
            roiDO->CreateByThresholdMaskImage(imgDO, threshold);
            result = doc->Add(roiDO);
            break;
        case FrCreateRoiDialog::LoadFromFile:
            roiDO = new FrRoiDocObj();
            if(roiDO->LoadFromFile(imgDO, fileName)){
                result = doc->Add(roiDO);
            }
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
