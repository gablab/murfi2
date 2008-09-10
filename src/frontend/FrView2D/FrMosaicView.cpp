#include "FrMosaicView.h"
#include "FrMainWindow.h"
#include "QVTKWidget.h"

#include "vtkCamera.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRendererCollection.h"


// Default constructor
FrMosaicView::FrMosaicView(FrMainWindow* mainWindow)
: FrBaseView(mainWindow) {
    m_renderer = 0L;
}

FrMosaicView::~FrMosaicView(){
    if(m_renderer) m_renderer->Delete();
}

void FrMosaicView::Initialize()
{
    // create renderer
    m_renderer = vtkRenderer::New();
    m_renderer->GetActiveCamera()->ParallelProjectionOn();
    m_renderer->SetBackground( 0.1, 0.2, 0.4 );

    SetupRenderers();
}

void FrMosaicView::SetupRenderers(){
    
    RemoveRenderers();
    
    QTVIEW3D->GetRenderWindow()->AddRenderer(m_renderer);
}

void FrMosaicView::RemoveRenderers(){

    // Remove all renderers
    QTVIEW3D->GetRenderWindow()->GetRenderers()->RemoveItem(m_renderer);
    QTVIEW3D->GetRenderWindow()->GetRenderers()->InitTraversal();
}

void FrMosaicView::SetupPipeline(){
    // TODO: implement!!!
}

void FrMosaicView::UpdatePipeline(){

    // TODO: implement!!!
}