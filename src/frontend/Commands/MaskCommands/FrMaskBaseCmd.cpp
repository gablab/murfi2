#include "FrMaskBaseCmd.h"
#include "FrMaskEditor.h"

// VTK includes
#include "vtkImageData.h"


FrMaskBaseCmd::FrMaskBaseCmd(){
    // NOTE: do nothing in this class
}

FrMaskBaseCmd::~FrMaskBaseCmd(){
    // NOTE: do nothing in this class
}

vtkImageData* FrMaskBaseCmd::GetCurrentROIImageData(){
    

    // TODO: implement
    return 0L;
}
    
void FrMaskBaseCmd::ApplyToCurrentROI(vtkImageData* data){
    // TODO: implement

    // Get Current ROI
    FrMaskEditor* me;    
    me->SetInput(data);
    me->SetView(FrMaskEditor::Slice);
    me->SetOrientation(FrMaskEditor::XY);
    me->SetSliceNumber(0);
    me->Update();
}
