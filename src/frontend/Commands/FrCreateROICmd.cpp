#include "FrCreateROICmd.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"

#include "FrRoiDocObj.h"
#include "FrImageDocObj.h"

#include "Qt/qmessagebox.h"

#define ALL_ITEMS_COUNT 5

FrCreateROICmd::FrCreateROICmd(){
}

bool FrCreateROICmd::Execute(){
    // Check for safety
    if(!this->GetMainController()) return false;
    
    // TODO: add ROI dialog with ability 
    // to select how to create ROI

    //FrMainWindow* mv = this->GetMainController()->GetMainView();
    //FrLayerDialog dlg(mv, true);
    //if(!dlg.SimpleExec()) return false;

    FrMainDocument* doc = this->GetMainController()->GetMainDocument();

    // Get images from document
    std::vector<FrDocumentObj*> images;
    doc->GetObjectsByType(images, FrDocumentObj::ImageObject);

    FrMainWindow* mv = this->GetMainController()->GetMainView();
    if(images.size() == 0){
        QMessageBox::critical(mv, QString("Creation of ROI"), 
            QString("Can't create ROI. None image opened.") );
        return false;
    }

    // Create from image 
    FrImageDocObj* imgDO = (FrImageDocObj*)images[0];
    FrRoiDocObj* roiDO = new FrRoiDocObj();
    roiDO->CreateMaskImage(imgDO);

    bool result = doc->Add(roiDO);
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
