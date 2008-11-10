#include "FrMaskPenCmd.h"

// VTK stuff
#include "vtkImageData.h"
#include "vtkImageLogic.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"


FrMaskPenCmd::FrMaskPenCmd(){
}

FrMaskPenCmd::~FrMaskPenCmd(){
}

bool FrMaskPenCmd::Execute(){
    if(!this->GetMainController()) return false;

    bool result = false;
    return result;
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrMaskPenCmd::CanUndo(){
    return false;
}

bool FrMaskPenCmd::Undo(){
    return false;
}

bool FrMaskPenCmd::Redo(){
    return false;
}
