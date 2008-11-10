#include "FrMaskSphereCmd.h"

// VTK stuff
#include "vtkImageData.h"
#include "vtkImageLogic.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"


FrMaskSphereCmd::FrMaskSphereCmd(){
}

FrMaskSphereCmd::~FrMaskSphereCmd(){
}

bool FrMaskSphereCmd::Execute(){
    if(!this->GetMainController()) return false;

    bool result = false;
    return result;
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrMaskSphereCmd::CanUndo(){
    return false;
}

bool FrMaskSphereCmd::Undo(){
    return false;
}

bool FrMaskSphereCmd::Redo(){
    return false;
}
