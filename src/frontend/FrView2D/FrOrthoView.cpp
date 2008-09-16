#include "FrOrthoView.h"
#include "FrMainWindow.h"
#include "QVTKWidget.h"
#include "FrDocumentReader.h"
#include "FrDocument.h"
#include "FrMainDocument.h"
#include "FrTBCFilter.h"
#include "FrNotify.h"
#include "Fr2DSliceActor.h"
#include "FrSliceExtractor.h"

// VTK stuff
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
    //m_tbcFilter = FrTBCFilter::New();
	//m_SliceExtractor = FrSliceExtractor::New();

    // create renderers
	for(int i = 0; i < RENDERER_COUNT; i++){
        m_renderer[i] = vtkRenderer::New();
		m_actor2[i] = Fr2DSliceActor::New();
		m_SliceExtractor[i] = FrSliceExtractor::New();
		m_tbcFilter[i] = FrTBCFilter::New();	// testing
    }
}

FrOrthoView::~FrOrthoView(){

    if(m_actor) m_actor->Delete();
    //if(m_tbcFilter) m_tbcFilter->Delete();
    if(m_docReader) m_docReader->Delete();

    for(int i = 0; i < RENDERER_COUNT; i++){
        if(m_renderer[i]) m_renderer[i]->Delete();
		if(m_actor2[i]) m_actor2[i]->Delete(); 
		if(m_SliceExtractor[i]) m_SliceExtractor[i]->Delete();
		if(m_tbcFilter[i]) m_tbcFilter[i]->Delete();	// testing
    }    
}

void FrOrthoView::Initialize()
{
    // create renderers
	for(int i = 0; i < RENDERER_COUNT; i++){        
        m_renderer[i]->GetActiveCamera()->ParallelProjectionOn();
        m_renderer[i]->SetBackground( 0.0, 0.0, 0.0 );
    }

    SetupRenderers();
}

void FrOrthoView::SetupRenderers(){
    
    RemoveRenderers();
	GetRenderWindow()->Render();
    
    QTVIEW3D->GetRenderWindow()->AddRenderer(m_renderer[CORONAL_RENDERER]);
    m_renderer[CORONAL_RENDERER]->SetViewport(0.0, 0.5, 0.5, 1.0);
//    m_renderer[CORONAL_RENDERER]->SetBackground(0.8, 0.1, 0.1);
    //m_renderer[CORONAL_RENDERER]->Render();
    
    QTVIEW3D->GetRenderWindow()->AddRenderer(m_renderer[SAGITAL_RENDERER]);
	m_renderer[SAGITAL_RENDERER]->SetViewport(0.5, 0.5, 1.0, 1.0);
//    m_renderer[SAGITAL_RENDERER]->SetBackground(0.1, 0.8, 0.1);
    //m_renderer[SAGITAL_RENDERER]->Render();
    
    QTVIEW3D->GetRenderWindow()->AddRenderer(m_renderer[AXIAL_RENDERER]);
	m_renderer[AXIAL_RENDERER]->SetViewport(0.0, 0.0, 0.5, 0.5);
//    m_renderer[AXIAL_RENDERER]->SetBackground(0.1, 0.1, 0.8);    
    //m_renderer[AXIAL_RENDERER]->Render();

    QTVIEW3D->GetRenderWindow()->AddRenderer(m_renderer[DUMMY_RENDERER]);
	m_renderer[DUMMY_RENDERER]->SetViewport(0.5, 0.0, 1.0, 0.5);
 //   m_renderer[DUMMY_RENDERER]->SetBackground(0.1, 0.1, 0.1);    
 //   m_renderer[DUMMY_RENDERER]->Render();
    
}

void FrOrthoView::RemoveRenderers(){

    // Remove all renderers
    for (int i = 0; i < RENDERER_COUNT; i++) {      
        QTVIEW3D->GetRenderWindow()->GetRenderers()->RemoveItem(m_renderer[i]);
    }
    QTVIEW3D->GetRenderWindow()->GetRenderers()->InitTraversal();
}

void FrOrthoView::UpdatePipeline(int point){

    // TODO: implement!!!
    bool isCleared = false;
    FrMainDocument* document = GetMainWindow()->GetMainDocument();

    switch(point){
    case FRP_FULL:
        {
            // Clear scene
            for(int i = 0; i < RENDERER_COUNT; i++){
                m_renderer[i]->RemoveAllViewProps();
                m_renderer[i]->ResetCamera();
            }
			GetRenderWindow()->Render();
			
            isCleared = true;
        }
    case FRP_READIMAGE:
        {
            m_docReader->SetDocument(document);
            m_docReader->SetUnMosaicOn(true);
            m_docReader->Update();

            //m_tbcFilter->SetInput(m_docReader->GetOutput());
			
			for(int i = 0; i < RENDERER_COUNT-1; i++){
				m_SliceExtractor[i]->SetInput(m_docReader->GetOutput());
				m_SliceExtractor[i]->SetOutputOrientation(i);
				m_SliceExtractor[i]->Update();

				m_tbcFilter[i]->SetInput(m_SliceExtractor[i]->GetOutput());
				m_tbcFilter[i]->Update();
				
				m_actor2[i]->SetInput(m_SliceExtractor[i]->GetOutput());
				//m_actor2[i]->SetCurrentPlane(i);
				m_renderer[i]->AddActor(m_actor2[i]);

				double* center = m_actor2[i]->GetCenter();
		        m_actor2[i]->AddPosition(-center[0], -center[1], 0);

				m_renderer[i]->ResetCamera();
				m_renderer[i]->GetActiveCamera()->ParallelProjectionOn();
				m_renderer[i]->GetActiveCamera()->SetParallelScale(120);
				m_renderer[i]->Render();
			}
        }
    case FRP_TBC:
        {
            for(int i = 0; i < RENDERER_COUNT-1; i++){
				// Setup brightness contrast
				m_tbcFilter[i]->SetThreshold(document->GetThreshold());
				m_tbcFilter[i]->SetBrightness(document->GetBrightness());
				m_tbcFilter[i]->SetContrast(document->GetContrast());

				if(m_tbcFilter[i]->GetInput()){                
					m_tbcFilter[i]->Update();
					
					m_actor2[i]->SetInput(m_tbcFilter[i]->GetOutput());
				}
            }
        }
    case FRP_SLICE:
        {
            // get absolute mouse position
			x = document->GetXCoord();
			y = document->GetYCoord();

			// Implement...
        }

    default:
        // do nothing
        break;
    }    
    
    if(isCleared){
        //for(int i=0; i < RENDERER_COUNT; i++){
        //    m_renderer[i]->AddActor(m_actor2[i]);
        //}
    }

    GetRenderWindow()->Render();
}