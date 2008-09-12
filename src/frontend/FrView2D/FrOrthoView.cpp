#include "FrOrthoView.h"
#include "FrMainWindow.h"
#include "QVTKWidget.h"
#include "FrDocumentReader.h"
#include "FrDocument.h"
#include "FrMainDocument.h"
#include "FrTBCFilter.h"
#include "FrNotify.h"

#include "vtkCamera.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRendererCollection.h"
#include "vtkPNGReader.h"
#include "vtkImageActor.h"
#include "vtkRenderWindow.h"


// Default constructor
FrOrthoView::FrOrthoView(FrMainWindow* mainWindow)
: FrBaseView(mainWindow) {
       
    m_actor = vtkImageActor::New();
    m_docReader = FrDocumentReader::New();
    m_tbcFilter = FrTBCFilter::New();

    // create renderers
	for(int i=0; i < RENDERER_COUNT; ++i){
        m_renderer[i] = vtkRenderer::New();;
    }
}

FrOrthoView::~FrOrthoView(){

    if(m_actor) m_actor->Delete();
    if(m_tbcFilter) m_tbcFilter->Delete();
    if(m_docReader) m_docReader->Delete();

    for(int i=0; i < RENDERER_COUNT; ++i){
        if(m_renderer[i]) m_renderer[i]->Delete();
    }    
}

void FrOrthoView::Initialize()
{
    // create renderers
	for(int i=0; i < RENDERER_COUNT; ++i){        
        m_renderer[i]->GetActiveCamera()->ParallelProjectionOn();
        m_renderer[i]->SetBackground( 0.1*i, 0.2*i, 0.4*i );
    }

    SetupRenderers();
}

void FrOrthoView::SetupRenderers(){
    
    RemoveRenderers();
    
    //QTVIEW3D->GetRenderWindow()->AddRenderer(m_renderer[CORONAL_RENDERER]);
    //m_renderer[CORONAL_RENDERER]->SetViewport(0.0, 0.5, 0.5, 1.0);
    //m_renderer[CORONAL_RENDERER]->SetBackground(1.0, 1.0, 1.0);
    //m_renderer[CORONAL_RENDERER]->Render();
    
    //QTVIEW3D->GetRenderWindow()->AddRenderer(m_renderer[SAGITAL_RENDERER]);
	//m_renderer[SAGITAL_RENDERER]->SetViewport(0.5, 1.0, 0.5, 1.0);
    //m_renderer[SAGITAL_RENDERER]->SetBackground(0.0, 0.0, 0.0);
    //m_renderer[SAGITAL_RENDERER]->Render();
    
    
	//m_renderer[AXIAL_RENDERER]->SetViewport(0.0, 0.5, 0.0, 0.5);
    //m_renderer[AXIAL_RENDERER]->SetBackground(0.5, 0.5, 0.5);
    //QTVIEW3D->GetRenderWindow()->AddRenderer(m_renderer[AXIAL_RENDERER]);
    //m_renderer[AXIAL_RENDERER]->Render();
    
}

void FrOrthoView::RemoveRenderers(){

    // Remove all renderers
    for (int i=0; i < RENDERER_COUNT; ++i) {      
        QTVIEW3D->GetRenderWindow()->GetRenderers()->RemoveItem(m_renderer[i]);
    }
    QTVIEW3D->GetRenderWindow()->GetRenderers()->InitTraversal();
}

void FrOrthoView::UpdatePipeline(int point){

    //// TODO: implement!!!
    //bool isCleared = false;
    //FrMainDocument* document = GetMainWindow()->GetMainDocument();

    //switch(point){
    //case FRP_FULL:
    //    {
    //        // Clear scene
    //        for(int i=0; i < RENDERER_COUNT; ++i){
    //            m_renderer[i]->RemoveAllViewProps();
    //            m_renderer[i]->ResetCamera();
    //        }
    //        GetRenderWindow()->Render();

    //        isCleared = true;
    //    }
    //case FRP_READIMAGE:
    //    {
    //        m_docReader->SetDocument(document);
    //        m_docReader->SetMosaicOn(true);
    //        m_docReader->Update();

    //        m_tbcFilter->SetInput(m_docReader->GetOutput());
    //    }
    //case FRP_TBC:
    //    {
    //        // Setup brightness contrast
    //        m_tbcFilter->SetThreshold(document->GetThreshold());
    //        m_tbcFilter->SetBrightness(document->GetBrightness());
    //        m_tbcFilter->SetContrast(document->GetContrast());

    //        if(m_tbcFilter->GetInput()){                
    //            m_tbcFilter->Update();

    //            m_actor->SetInput(m_tbcFilter->GetOutput());
    //        }
    //    }
    //case FRP_SLICE:
    //    {
    //        // DO nothing here for a while
    //    }

    //default:
    //    // do nothing
    //    break;
    //}    
    //
    //if(isCleared){
    //    for(int i=0; i < RENDERER_COUNT; ++i){
    //        m_renderer[i]->AddActor(m_actor);
    //    }
    //}

    GetRenderWindow()->Render();
}