#include "FrMosaicView.h"
#include "FrMainWindow.h"
#include "QVTKWidget.h"
#include "FrDocumentReader.h"
#include "FrDocument.h"
#include "FrMainDocument.h"
#include "FrTBCFilter.h"
#include "FrNotify.h"

#include "vtkPNGReader.h"
#include "vtkCamera.h"
#include "vtkImageActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRendererCollection.h"


// Default constructor
FrMosaicView::FrMosaicView(FrMainWindow* mainWindow)
: FrBaseView(mainWindow) {
    m_docReader = FrDocumentReader::New();
    m_renderer = vtkRenderer::New();
    m_actor = vtkImageActor::New();
    m_tbcFilter = FrTBCFilter::New();
}

FrMosaicView::~FrMosaicView(){
    if(m_docReader) m_docReader->Delete();
    if(m_renderer) m_renderer->Delete();
    if(m_actor) m_actor->Delete();
    if(m_tbcFilter) m_tbcFilter->Delete();
}

void FrMosaicView::Initialize()
{
    // create renderer
    m_renderer->GetActiveCamera()->ParallelProjectionOn();
    m_renderer->SetBackground( 0.0, 0.0, 0.0 );

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

void FrMosaicView::UpdatePipeline(int point){

    bool isCleared = false;
    FrMainDocument* document = GetMainWindow()->GetMainDocument();

    switch(point){
    case FRP_FULL:
        {
            // Clear scene
            m_renderer->RemoveAllViewProps();
            m_renderer->ResetCamera();
            GetRenderWindow()->Render();

            isCleared = true;
        }
    case FRP_READIMAGE:
        {
            // read data from document
            m_docReader->SetDocument(document);
            m_docReader->SetMosaicOn(true);
            m_docReader->Update();

            m_tbcFilter->SetInput(m_docReader->GetOutput());
        }
    case FRP_SLICE:
        {
            // NOTE: Do nothing here for a while
        }
    case FRP_TBC:
        {
            // Setup brightness contrast
            m_tbcFilter->SetThreshold(document->GetThreshold());
            m_tbcFilter->SetBrightness(document->GetBrightness());
            m_tbcFilter->SetContrast(document->GetContrast());

            if(m_tbcFilter->GetInput()){                
                m_tbcFilter->Update();

                m_actor->SetInput(m_tbcFilter->GetOutput());
            }
        }
    default:
        // do nothing
        break;
    }    
    
    if(isCleared){
        m_renderer->AddActor(m_actor);
        double* center = m_actor->GetCenter();
        m_actor->AddPosition(-center[0], -center[1], 0);
		
		m_renderer->ResetCamera();
		m_renderer->GetActiveCamera()->ParallelProjectionOn();
		m_renderer->GetActiveCamera()->SetParallelScale(200);
		m_renderer->Render();
	}

    // render scene
    GetRenderWindow()->Render();
}