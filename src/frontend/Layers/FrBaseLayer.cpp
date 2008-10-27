#include "FrBaseLayer.h"
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

vtkStandardNewMacro(FrBaseLayer);

FrBaseLayer::FrBaseLayer() 
: m_Renderer(0) {
    // renderer
    m_Renderer = vtkRenderer::New();
    m_Renderer->SetBackground(DEF_REN_BACKGROUND);
    m_Renderer->GetActiveCamera()->ParallelProjectionOn();
}

FrBaseLayer::~FrBaseLayer(){        
    if(m_Renderer){
        //m_Renderer->RemoveAllProps();
        m_Renderer->Delete();
    }
}

void FrBaseLayer::SetCameraSettings(FrCameraSettings& settings){
    if(!m_Renderer) return;

    vtkCamera* cam = m_Renderer->GetActiveCamera();
    cam->SetPosition(settings.Position);
    cam->SetFocalPoint(settings.FocalPoint);
    cam->SetViewUp(settings.ViewUp);
    cam->SetParallelScale(settings.Scale);
}

void FrBaseLayer::GetCameraSettings(FrCameraSettings& settings){
    if(!m_Renderer) return;
    
    vtkCamera* cam = m_Renderer->GetActiveCamera();
    COPY_ARR3(settings.Position, cam->GetPosition());
    COPY_ARR3(settings.FocalPoint, cam->GetFocalPoint());
    COPY_ARR3(settings.ViewUp, cam->GetViewUp());
    settings.Scale = cam->GetParallelScale();
}