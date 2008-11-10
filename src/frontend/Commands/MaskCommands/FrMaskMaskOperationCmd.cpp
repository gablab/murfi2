#include "FrMaskMaskOperationCmd.h"

// VTK stuff
#include "vtkImageData.h"
#include "vtkImageLogic.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"

#define FR_INVALID_OPERATION -1
#define DEF_OUTPUT_TRUE 255
#define DEF_OUTPUT_FALSE 0 


FrMaskMaskOperationCmd::FrMaskMaskOperationCmd()
: m_Filter(0), m_Action(FrMaskMaskOperationCmd::None) {
    m_Filter = vtkImageLogic::New();
}

FrMaskMaskOperationCmd::~FrMaskMaskOperationCmd(){
    if(m_Filter) m_Filter->Delete();
}

bool FrMaskMaskOperationCmd::Execute(){
    if(!this->GetMainController()) return false;

    bool result = false;
    switch(m_Action){
    case FrMaskMaskOperationCmd::None:
        break;
        // NOTE: Fall through
    case FrMaskMaskOperationCmd::Intersect:
    case FrMaskMaskOperationCmd::Union:
    case FrMaskMaskOperationCmd::Invert:
        result = this->PerformeStandardOperation();
        break;
    case FrMaskMaskOperationCmd::Subtract:
        result = this->PerformeSubtractOperation();
        break;
    }
    return result;
}

bool FrMaskMaskOperationCmd::PerformeStandardOperation(){
    bool result = false;

    int operation = FR_INVALID_OPERATION;
    switch(m_Action){
        case FrMaskMaskOperationCmd::Intersect:
            operation = VTK_AND;
            break;
        case FrMaskMaskOperationCmd::Union:
            operation = VTK_OR;
            break;
        case FrMaskMaskOperationCmd::Invert:
            operation = VTK_NOT;
            break;
        default:
            // do nothing here
            break;
    }

    vtkImageData* imageData1 = this->GetCurrentROIImageData(); 
    vtkImageData* imageData2 = this->GetTargetROIImageData();

    if(imageData1 && imageData2 && (operation != FR_INVALID_OPERATION)){
        m_Filter->SetInput1(imageData1);
        m_Filter->SetInput2(imageData2);  
        m_Filter->SetOperation(operation);
        m_Filter->SetOutputTrueValue(double(DEF_OUTPUT_TRUE));
        m_Filter->Update();
        
        this->ApplyToCurrentROI(m_Filter->GetOutput());
        result = true;
    }
    return result;
}

bool FrMaskMaskOperationCmd::PerformeSubtractOperation(){
    bool result = false;
    vtkImageData* imageData1 = this->GetCurrentROIImageData(); 
    vtkImageData* imageData2 = this->GetTargetROIImageData();
    
    if(imageData1 && imageData2){
        vtkImageData* output = vtkImageData::New();
        output->CopyStructure(imageData1);
        output->AllocateScalars();

        unsigned char* srcPtr1 = (unsigned char*)imageData1->GetScalarPointer();
        unsigned char* srcPtr2 = (unsigned char*)imageData2->GetScalarPointer();
        unsigned char* dstPtr  = (unsigned char*)output->GetScalarPointer();
        int size = output->GetPointData()->GetScalars()->GetSize();
        for(int i=0; i < size; ++i){
            (*dstPtr) = ((*srcPtr1) & (*srcPtr2)) ? DEF_OUTPUT_TRUE : DEF_OUTPUT_FALSE;
            ++dstPtr; ++srcPtr1; ++srcPtr2;
        }

        this->ApplyToCurrentROI(output);
        output->Delete();
        result = true;
    }
    return result;
}

vtkImageData* FrMaskMaskOperationCmd::GetTargetROIImageData(){
    // TODO: implement
    return 0L;
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrMaskMaskOperationCmd::CanUndo(){
    return false;
}

bool FrMaskMaskOperationCmd::Undo(){
    return false;
}

bool FrMaskMaskOperationCmd::Redo(){
    return false;
}
