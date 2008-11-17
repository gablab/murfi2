#include "FrMaskDilateErodeCmd.h"
#include "FrRoiDocObj.h"

// VTK stuff
#include "vtkImageData.h"
#include "vtkImageDilateErode3D.h"

#define DEF_KERNEL_SIZE     1
#define DEF_DDILATE_VALUE   255.0
#define DEF_DERODE_VALUE    0.0
#define DEF_EDILATE_VALUE   0.0
#define DEF_EERODE_VALUE    255.0

FrMaskDilateErodeCmd::FrMaskDilateErodeCmd()
: m_Filter(0), m_Action(FrMaskDilateErodeCmd::None),
  m_KernelSize(DEF_KERNEL_SIZE) {
    m_Filter = vtkImageDilateErode3D::New();
}

FrMaskDilateErodeCmd::~FrMaskDilateErodeCmd(){
    if(m_Filter) m_Filter->Delete();
}

bool FrMaskDilateErodeCmd::Execute(){
    if(!this->GetMainController()) return false;

    bool result = false;
    switch(m_Action){
    case FrMaskDilateErodeCmd::None:
        result = false;
        break;
    case FrMaskDilateErodeCmd::Dilate:
        result = this->DoDilate();
        break;
    case FrMaskDilateErodeCmd::Erode:
        result = this->DoErode();
        break;
    }
    return result;
}

// Helpers
bool FrMaskDilateErodeCmd::DoDilate(){
    bool result = false;
    FrRoiDocObj* roiDO = this->GetCurrentRoi();
    if(roiDO){
        vtkImageData* imageData = this->GetRoiImageData(roiDO->GetID()); 
    
        m_Filter->SetKernelSize(m_KernelSize, m_KernelSize, 1);
        m_Filter->SetDilateValue(DEF_DDILATE_VALUE);
        m_Filter->SetErodeValue(DEF_DERODE_VALUE);
        m_Filter->SetInput(imageData);
        m_Filter->Update();
        
        this->ApplyDataToRoi(m_Filter->GetOutput(), roiDO);
        result = true;
    }
    return result;
}

bool FrMaskDilateErodeCmd::DoErode(){
    bool result = false;
    FrRoiDocObj* roiDO = this->GetCurrentRoi();
    if(roiDO){
        vtkImageData* imageData = this->GetRoiImageData(roiDO->GetID()); 
    
        m_Filter->SetKernelSize(m_KernelSize, m_KernelSize, 1);
        m_Filter->SetDilateValue(DEF_EDILATE_VALUE);
        m_Filter->SetErodeValue(DEF_EERODE_VALUE);
        m_Filter->SetInput(imageData);
        m_Filter->Update();
        
        this->ApplyDataToRoi(m_Filter->GetOutput(), roiDO);
        result = true;
    }
    return result;
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrMaskDilateErodeCmd::CanUndo(){
    return false;
}

bool FrMaskDilateErodeCmd::Undo(){
    return false;
}

bool FrMaskDilateErodeCmd::Redo(){
    return false;
}
