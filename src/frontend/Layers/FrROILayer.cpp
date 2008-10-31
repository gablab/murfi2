#include "FrROILayer.h"
#include "FrSelection.h"
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


vtkStandardNewMacro(FrROILayer);


FrROILayer::FrROILayer() 
: m_actor(0) {
    // Pipline stuff
    m_actor = vtkImageActor::New();
	m_selection = FrSelection::New();

    // add actor
    m_Renderer->AddActor(m_actor);
	m_Renderer->AddActor(m_selection);

	m_selection->SetVisibility(false);	// selection is invisible by default
}

FrROILayer::~FrROILayer(){    
    if(m_Renderer) m_Renderer->RemoveActor(m_actor);
    if(m_actor) m_actor->Delete();
}

// Accessors / Modifiers
void FrROILayer::SetInput(vtkImageData* data){
    if(m_actor){
        m_actor->SetInput(data);

        int extent[6];
        extent[0] = -1;
        extent[1] = extent[2] = extent[3] = extent[4] = extent[5] = 0;
        m_actor->SetDisplayExtent(extent);
    }
}

vtkImageData* FrROILayer::GetInput(){
    vtkImageData* result = 0L;
    if(m_actor){
        result = m_actor->GetInput();
    }
    return result;
}

void FrROILayer::SetOpacity(double value){
    if(m_actor){
        m_actor->SetOpacity(value);
    }
}

double FrROILayer::GetOpacity(){
    double result = 0.0;
    if(m_actor){
        result = m_actor->GetOpacity();
    }
    return result;
}

void FrROILayer::SetVisibility(bool value){
    if(m_actor){
        m_actor->SetVisibility(value ? 1 : 0);
    }
}
bool FrROILayer::GetVisibility(){
    bool result = false;
    if(m_actor){
        result = (m_actor->GetVisibility() == 1);
    }
    return result;
}

void FrROILayer::UpdateCamera(){
    if(m_Renderer){
        m_Renderer->Render(); 
    }
}

void FrROILayer::SetSelection(){
	// TODO: implement
}