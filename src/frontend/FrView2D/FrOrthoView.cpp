#include "FrOrthoView.h"
#include "FrMainWindow.h"
#include "QVTKWidget.h"

#include "vtkCamera.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRendererCollection.h"


// Default constructor
FrOrthoView::FrOrthoView(FrMainWindow* mainWindow)
: FrBaseView(mainWindow) {
       
    // create renderers
	for(int i=0; i < RENDERER_COUNT; ++i){
        m_renderer[i] = 0L;
    }
}

FrOrthoView::~FrOrthoView(){

    for(int i=0; i < RENDERER_COUNT; ++i){
        if(m_renderer[i]) m_renderer[i]->Delete();
    }    
}

void FrOrthoView::Initialize()
{
    // create renderers
	for(int i=0; i < RENDERER_COUNT; ++i){
        vtkRenderer* ren = vtkRenderer::New();
        ren->GetActiveCamera()->ParallelProjectionOn();
        ren->SetBackground( 0.1, 0.2, 0.4 );
        m_renderer[i] = ren;
    }

    SetupRenderers();
}

void FrOrthoView::SetupRenderers(){
    
    RemoveRenderers();

    m_renderer[CORONAL_RENDERER]->SetViewport(0.0, 0.5, 0.5, 0.5); 
	m_renderer[SAGITAL_RENDERER]->SetViewport(0.5, 1.0, 1.0, 0.5);
	m_renderer[AXIAL_RENDERER]->SetViewport(0.0, 0.5,  0.5, 1.0);

    for (int i=0; i < RENDERER_COUNT; ++i){
        QTVIEW3D->GetRenderWindow()->AddRenderer(m_renderer[i]);
    }
}

void FrOrthoView::RemoveRenderers(){

    // Remove all renderers
    for (int i=0; i < RENDERER_COUNT; ++i) {      
        QTVIEW3D->GetRenderWindow()->GetRenderers()->RemoveItem(m_renderer[i]);
    }
    QTVIEW3D->GetRenderWindow()->GetRenderers()->InitTraversal();
}

void FrOrthoView::SetupPipeline(){
    // TODO: implement!!!
}

void FrOrthoView::UpdatePipeline(){

    // TODO: implement!!!
}