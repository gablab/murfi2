#include "FrMaskCopyAdjacentCmd.h"
#include "FrRoiDocObj.h"

// VTK stuff
#include "vtkImageData.h"

FrMaskCopyAdjacentCmd::FrMaskCopyAdjacentCmd(){
}

FrMaskCopyAdjacentCmd::~FrMaskCopyAdjacentCmd(){
}

bool FrMaskCopyAdjacentCmd::Execute(){
    if(!this->GetMainController()) return false;

    bool result = false;
    FrRoiDocObj* roiDO = this->GetCurrentRoi();
    if(roiDO){
        vtkImageData* imageData = this->GetRoiImageData(roiDO->GetID()); 
        // TODO: implement 
        
        result = true;
    }
    return result;
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrMaskCopyAdjacentCmd::CanUndo(){
    return false;
}

bool FrMaskCopyAdjacentCmd::Undo(){
    return false;
}

bool FrMaskCopyAdjacentCmd::Redo(){
    return false;
}
