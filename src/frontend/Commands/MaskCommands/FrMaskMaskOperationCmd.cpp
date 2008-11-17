#include "FrMaskMaskOperationCmd.h"
#include "FrMainWindow.h"
#include "FrLayerListWidget.h"
#include "FrROIToolWidget.h"
#include "FrListToolWidget.h"
#include "FrRoiDocObj.h"

// VTK stuff
#include "vtkImageData.h"
#include "vtkImageLogic.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"

#define FR_INVALID_OPERATION -1
#define DEF_OUTPUT_TRUE 255
#define DEF_OUTPUT_FALSE 0 


FrMaskMaskOperationCmd::FrMaskMaskOperationCmd()
: m_Action(FrMaskMaskOperationCmd::None) {
}

FrMaskMaskOperationCmd::~FrMaskMaskOperationCmd(){
}

bool FrMaskMaskOperationCmd::Execute(){
    if(!this->GetMainController()) return false;

    bool result = false;

    FrRoiDocObj* roiDO = this->GetCurrentRoi();
    if(roiDO){
        vtkImageData* imageData1 = this->GetRoiImageData(roiDO->GetID()); 
        vtkImageData* imageData2 = this->GetTargetRoiImageData();
        
        switch(m_Action){
        case FrMaskMaskOperationCmd::None:
            break;
        case FrMaskMaskOperationCmd::Intersect:
            result = this->PerformeIntersectOperation(imageData1, imageData2);
            break;
        case FrMaskMaskOperationCmd::Union:
            result = this->PerformeUnionOperation(imageData1, imageData2);
            break;
        case FrMaskMaskOperationCmd::Invert:
            result = this->PerformeInvertOperation(imageData1, imageData2);
            break;
        case FrMaskMaskOperationCmd::Subtract:
            result = this->PerformeSubtractOperation(imageData1, imageData2);
            break;
        }

        if(result){
            this->ApplyDataToRoi(imageData1, roiDO);
            this->UpdatePipelineForID(roiDO->GetID(), FRP_READROI);
        }
    }
    return result;
}

bool FrMaskMaskOperationCmd::PerformeUnionOperation(vtkImageData* in1, vtkImageData* in2){
    bool result = false;
        
    if(in1 && in2){
        int size = in1->GetPointData()->GetScalars()->GetSize();
        unsigned char* dstPtr = (unsigned char*)in1->GetScalarPointer();
        unsigned char* srcPtr = (unsigned char*)in2->GetScalarPointer();
        for(int i=0; i < size; ++i){
            (*dstPtr) = ((*dstPtr) | (*srcPtr)) ? DEF_OUTPUT_TRUE : DEF_OUTPUT_FALSE;
            ++dstPtr; ++srcPtr;
        }
        result = true;
    }
    return result;
}

bool FrMaskMaskOperationCmd::PerformeInvertOperation(vtkImageData* in1, vtkImageData* in2){
    bool result = false;
        
    if(in1){
        int size = in1->GetPointData()->GetScalars()->GetSize();
        unsigned char* dstPtr = (unsigned char*)in1->GetScalarPointer();
        for(int i=0; i < size; ++i){
            (*dstPtr) = ((*dstPtr) ?  DEF_OUTPUT_FALSE : DEF_OUTPUT_TRUE);
            ++dstPtr;
        }
        result = true;
    }
    return result;
}

bool FrMaskMaskOperationCmd::PerformeIntersectOperation(vtkImageData* in1, vtkImageData* in2){
    bool result = false;
        
    if(in1 && in2){
        int size = in1->GetPointData()->GetScalars()->GetSize();
        unsigned char* dstPtr = (unsigned char*)in1->GetScalarPointer();
        unsigned char* srcPtr = (unsigned char*)in2->GetScalarPointer();
        for(int i=0; i < size; ++i){
            (*dstPtr) = ((*dstPtr) & (*srcPtr)) ? DEF_OUTPUT_TRUE : DEF_OUTPUT_FALSE;
            ++dstPtr; ++srcPtr;
        }
        result = true;
    }
    return result;
}

bool FrMaskMaskOperationCmd::PerformeSubtractOperation(vtkImageData* in1, vtkImageData* in2){
    bool result = false;
        
    if(in1 && in2){
        int size = in1->GetPointData()->GetScalars()->GetSize();
        unsigned char* dstPtr = (unsigned char*)in1->GetScalarPointer();
        unsigned char* srcPtr = (unsigned char*)in2->GetScalarPointer();
        for(int i=0; i < size; ++i){
            (*dstPtr) = ((*dstPtr) && !(*srcPtr)) ? DEF_OUTPUT_TRUE : DEF_OUTPUT_FALSE;
            ++dstPtr; ++srcPtr;
        }
        result = true;
    }
    return result;
}

vtkImageData* FrMaskMaskOperationCmd::GetTargetRoiImageData(){
    // TODO: implement
    vtkImageData* result = 0L;
    
    if(this->GetMainController()){
        
        FrMainWindow* mv = this->GetMainController()->GetMainView();
        FrROIToolWidget* rtWidget = mv->GetLayerListWidget()->GetRoiToolWidget();
        
        int roiID = -1;
        if(rtWidget->GetCurrentToolType() == FrROIToolWidget::Union){
            roiID = rtWidget->GetUnionWidget()->GetCurrentItemID();
        }
        else if(rtWidget->GetCurrentToolType() == FrROIToolWidget::Intersect){
            roiID = rtWidget->GetIntersectWidget()->GetCurrentItemID();
        }
        else if(rtWidget->GetCurrentToolType() == FrROIToolWidget::Subtract){
            roiID = rtWidget->GetSubtractWidget()->GetCurrentItemID();
        }        
        
        if(roiID > 0){
            result = this->GetRoiImageData(roiID);
        }
    }
    return result;
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
