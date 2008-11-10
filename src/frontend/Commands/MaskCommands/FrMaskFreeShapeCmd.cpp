#include "FrMaskFreeShapeCmd.h"

// VTK stuff
#include "vtkImageData.h"
#include "vtkImageLogic.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"


FrMaskFreeShapeCmd::FrMaskFreeShapeCmd(){
}

FrMaskFreeShapeCmd::~FrMaskFreeShapeCmd(){
}

bool FrMaskFreeShapeCmd::Execute(){
    if(!this->GetMainController()) return false;

    bool result = false;
    vtkImageData* imageData = this->GetCurrentROIImageData();
    if(imageData){
        // TODO: implement
        result = true;
    }
    return result;
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrMaskFreeShapeCmd::CanUndo(){
    return false;
}

bool FrMaskFreeShapeCmd::Undo(){
    return false;
}

bool FrMaskFreeShapeCmd::Redo(){
    return false;
}
