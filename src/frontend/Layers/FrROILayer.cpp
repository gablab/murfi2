#include "FrRoiLayer.h"
#include "FrMaskToRgbaFilter.h"
#include "FrSettings.h"
#include "FrMacro.h"


// VTK stuff
#include "vtkObjectFactory.h"
#include "vtkImageData.h"
#include "vtkImageActor.h"
#include "vtkRenderer.h"
#include "vtkCamera.h"


vtkStandardNewMacro(FrRoiLayer);


FrRoiLayer::FrRoiLayer() 
    : m_actor(0), m_filter(0) {
    // Pipline stuff
    m_filter = FrMaskToRgbaFilter::New();
    m_actor = vtkImageActor::New();
    m_actor->InterpolateOff();

    // add actor
    m_Renderer->AddActor(m_actor);
}

FrRoiLayer::~FrRoiLayer(){    
    if(m_Renderer) m_Renderer->RemoveActor(m_actor);
    if(m_filter) m_filter->Delete();
    if(m_actor) m_actor->Delete();
}

// Accessors / Modifiers
void FrRoiLayer::SetInput(vtkImageData* data){
    if(m_filter){
        m_filter->SetInput(data);
    }
}

vtkImageData* FrRoiLayer::GetInput(){
    vtkImageData* result = 0L;
    if(m_filter){
        result = m_filter->GetInput();
    }
    return result;
}

void FrRoiLayer::SetOpacity(double value){
    if(m_actor){
        m_actor->SetOpacity(value);
    }
}

double FrRoiLayer::GetOpacity(){
    double result = 0.0;
    if(m_actor){
        result = m_actor->GetOpacity();
    }
    return result;
}

void FrRoiLayer::SetVisibility(bool value){
    if(m_actor){
        m_actor->SetVisibility(value ? 1 : 0);
    }
}
bool FrRoiLayer::GetVisibility(){
    bool result = false;
    if(m_actor){
        result = (m_actor->GetVisibility() == 1);
    }
    return result;
}

void FrRoiLayer::UpdateData(){
    if(m_filter && m_filter->GetInput()){
        m_filter->Update();

        // Pass data futher
        vtkImageData* data = m_filter->GetOutput();
        if(data != m_actor->GetInput()){
            m_actor->SetInput(data);
        }

        // Have to update display extent
        if(data){
            int extent[6];
            extent[0] = -1;
            extent[1] = extent[2] = extent[3] = extent[4] = extent[5] = 0;
            m_actor->SetDisplayExtent(extent);
        }
    }
}

