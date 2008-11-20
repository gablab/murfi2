#include "FrSaveImageCmd.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrImageDocObj.h"
#include "FrRoiDocObj.h"
#include "FrCommandController.h"

#include "Qt/qmessagebox.h"
#include "Qt/qfiledialog.h"

FrSaveImageCmd::FrSaveImageCmd()
: m_Action(FrSaveImageCmd::Undefined), m_SaveAsMode(false){

}

bool FrSaveImageCmd::Execute(){    
    if(!this->GetMainController()) return false;
    if(m_Action == FrSaveImageCmd::Undefined) return false;

    bool result = false;
    switch(m_Action){
        case FrSaveImageCmd::SaveImage:
            result = this->PerformeSaveImage();
            break;
        case FrSaveImageCmd::SaveRoi:
            result = this->PerformeSaveRoi();
            break;
        case FrSaveImageCmd::SaveImageAndRemove:
            result = this->PerformeSaveImage() && 
                     this->PerformeSaveRoi();
            break;
    }
	
	return result;
}

bool FrSaveImageCmd::PerformeSaveImage(bool removeAfterSave){
    // Get all images
    std::vector<FrDocumentObj*> images;
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    doc->GetObjectsByType(images, FrDocumentObj::ImageObject);

    bool result = false;
    std::vector<FrDocumentObj*>::iterator it, itEnd(images.end());
    for(it = images.begin(); it != itEnd; ++it){
        // TODO: implement!
        if(m_SaveAsMode){
        }
    }
    return result;
}

bool FrSaveImageCmd::PerformeSaveRoi(bool removeAfterSave){
    // Get all ROIs
    std::vector<FrDocumentObj*> rois;
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    
    doc->GetObjectsByType(rois, FrDocumentObj::RoiObject);

    bool result = false;
    std::vector<FrDocumentObj*>::iterator it, itEnd(rois.end());
    // TODO: implement!

    return result;
}

QString FrSaveImageCmd::GetFileName(bool forROI){
    FrMainWindow* mv = this->GetMainController()->GetMainView();

    QString message = (forROI) ? QString("Save ROI data") 
        : QString("Save MRI Image Data");

    QString fileName = QFileDialog::getSaveFileName(
        mv, message, QString(""), QString("NIfTI Image (*.nii)"));

    return fileName;
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrSaveImageCmd::CanUndo(){
    return false;
}

bool FrSaveImageCmd::Undo(){
    return false;
}

bool FrSaveImageCmd::Redo(){
    return false;
}
