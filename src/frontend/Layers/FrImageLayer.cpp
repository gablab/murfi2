#include "FrImageLayer.h"
#include "FrColormapFilter.h"
#include "FrTBCFilter.h"
#include "FrSettings.h"
#include "FrMacro.h"


// VTK stuff
#include "vtkObjectFactory.h"
#include "vtkImageData.h"
#include "vtkImageActor.h"
#include "vtkRenderer.h"
#include "vtkCamera.h"

// Some defines
#define DEF_REN_BACKGROUND 0.0, 0.0, 0.0


vtkStandardNewMacro(FrImageLayer);


FrImageLayer::FrImageLayer() 
: m_tbcFilter(0), m_actor(0) {
    // Pipline stuff
    m_tbcFilter = FrTbcFilter::New();
    m_actor = vtkImageActor::New();
    m_actor->InterpolateOff();
    // add actor
    m_Renderer->AddActor(m_actor);
}

FrImageLayer::~FrImageLayer(){
    if(m_tbcFilter) m_tbcFilter->Delete();
    if(m_Renderer) m_Renderer->RemoveActor(m_actor);
    if(m_actor) m_actor->Delete();
}

// Accessors / Modifiers
void FrImageLayer::SetInput(vtkImageData* data){
    if(m_tbcFilter){
        m_tbcFilter->SetInput(data);
    }
}

vtkImageData* FrImageLayer::GetInput(){
    vtkImageData* result = 0L;
    if(m_tbcFilter){
        result = m_tbcFilter->GetInput();
    }
    return result;
}

void FrImageLayer::SetTbcSettings(FrTbcSettings& settings){
    if(!m_tbcFilter) return;

    m_tbcFilter->SetThreshold(settings.Threshold);
    m_tbcFilter->SetBrightness(settings.Brightness);
    m_tbcFilter->SetContrast(settings.Contrast);
}

void FrImageLayer::GetTbcSettings(FrTbcSettings& settings){
    if(!m_tbcFilter) return;

    settings.Threshold = m_tbcFilter->GetThreshold();
    settings.Brightness = m_tbcFilter->GetBrightness();
    settings.Contrast = m_tbcFilter->GetContrast();
}

void FrImageLayer::SetOpacity(double value){
    if(m_actor){
        m_actor->SetOpacity(value);
    }
}

double FrImageLayer::GetOpacity(){
    double result = 0.0;
    if(m_actor){
        result = m_actor->GetOpacity();
    }
    return result;
}

void FrImageLayer::SetVisibility(bool value){
    if(m_actor){
        m_actor->SetVisibility(value ? 1 : 0);
    }
}
bool FrImageLayer::GetVisibility(){
    bool result = false;
    if(m_actor){
        result = (m_actor->GetVisibility() == 1);
    }
    return result;
}

void FrImageLayer::UpdateTbc(){
    if(m_tbcFilter && m_tbcFilter->GetInput()){
        m_tbcFilter->Update();

        // Pass data futher
        vtkImageData* data = m_tbcFilter->GetOutput();
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

void FrImageLayer::UpdateCamera(){
    if(m_Renderer){
        m_Renderer->Render();
    }
}

void FrImageLayer::UpdateColormap(){
}