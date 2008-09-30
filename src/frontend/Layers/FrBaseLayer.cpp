#include "FrBaseLayer.h"

// VTK stuff
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkCamera.h"


FrBaseLayer::FrBaseLayer(vtkRenderWindow *renWindow){
    m_Renderer = vtkRenderer::New();
	m_RenderWindow = renWindow;		
	m_RenderWindow->AddRenderer(m_Renderer);
}

FrBaseLayer::~FrBaseLayer(){
    if(m_Renderer) m_Renderer->Delete();
}

void FrBaseLayer::Initialize(){
}

void FrBaseLayer::Update(){
}
