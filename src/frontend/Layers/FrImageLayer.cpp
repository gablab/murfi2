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
: m_cmFilter(0), m_tbcFilter(0), m_actor(0) {
    // Pipline stuff
    m_cmFilter = FrColormapFilter::New();
    m_tbcFilter = FrTBCFilter::New();
    m_actor = vtkImageActor::New();    
    // add actor
    m_Renderer->AddActor(m_actor);
}

FrImageLayer::~FrImageLayer(){    
    if(m_cmFilter) m_cmFilter->Delete();
    if(m_tbcFilter) m_tbcFilter->Delete();
    if(m_actor) m_actor->Delete();
}

// Accessors / Modifiers
void FrImageLayer::SetInput(vtkImageData* data){
    if(m_cmFilter){
        m_cmFilter->SetInput(data);
    }
}

vtkImageData* FrImageLayer::GetInput(){
    vtkImageData* result = 0L;
    if(m_cmFilter){
        result = m_cmFilter->GetInput();
    }
    else if(m_tbcFilter){
        result = m_tbcFilter->GetInput();
    }
    return result;
}

void FrImageLayer::SetColormapSettings(FrColormapSettings& settings){
    if(!m_cmFilter) return;

    // Set type first
    if(settings.Type == FrColormapSettings::SingleColor){
        m_cmFilter->SetType(FrColormapFilter::SingleColor);
    }
    else if(settings.Type == FrColormapSettings::MultiColor) {
        m_cmFilter->SetType(FrColormapFilter::MultiColor);
    }

    // Set other params
    m_cmFilter->SetPxMin(settings.MinValue);
    m_cmFilter->SetPxMax(settings.MaxValue);    
    m_cmFilter->SetColor(settings.Color);
    m_cmFilter->SetThreshold(settings.Threshold);

    if(m_cmFilter->GetIsModified()){
        m_tbcFilter->Modified();
    }
}

void FrImageLayer::GetColormapSettings(FrColormapSettings& settings){
    if(!m_cmFilter) return;

    // Get type first
    if(m_cmFilter->GetType() == FrColormapFilter::SingleColor){
        settings.Type = FrColormapSettings::SingleColor;
    }
    else if(m_cmFilter->GetType() == FrColormapFilter::MultiColor) {
        settings.Type = FrColormapSettings::MultiColor;
    }

    // Get other params
    settings.MinValue = m_cmFilter->GetPxMin();
    settings.MaxValue = m_cmFilter->GetPxMax();    
    settings.Color    = m_cmFilter->GetColor();
    settings.Threshold = m_cmFilter->GetThreshold();
}

void FrImageLayer::SetTBCSettings(FrTBCSettings& settings){
    if(!m_tbcFilter) return;

    m_tbcFilter->SetThreshold(settings.Threshold);
    m_tbcFilter->SetBrightness(settings.Brightness);
    m_tbcFilter->SetContrast(settings.Contrast);
}

void FrImageLayer::GetTBCSettings(FrTBCSettings& settings){
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

// Update methods
void FrImageLayer::UpdateColormap(){
    if(m_cmFilter && m_cmFilter->GetInput()){
        m_cmFilter->Update();

        // Pass data futher
        vtkImageData* data = m_cmFilter->GetOutput();
        if(data != m_tbcFilter->GetInput()){
            m_tbcFilter->SetInput(data);
        }
    }
}

void FrImageLayer::UpdateTBC(){
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