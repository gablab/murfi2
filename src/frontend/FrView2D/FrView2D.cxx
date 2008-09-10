#include "FrView2D.h"
#include "QVTKWidget.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrImageDocObj.h"
#include "FrTBCFilter.h"
#include "FrMosaicFilter.h"
#include "FrInteractorStyle.h"
#include "Fr2DSliceActor.h"
#include "FrDocumentReader.h"

// Qt
#include "Qt/QWidget.h"
#include "QtGUI/QSizePolicy.h"

// VTK
#include "vtkRenderWindowInteractor.h"
#include "vtkInteractorStyleImage.h"
#include "vtkRenderer.h"
#include "vtkCamera.h"
#include "vtkPNGReader.h"
#include "vtkImageActor.h"
#include "vtkTextActor.h"
#include "vtkTextProperty.h"
#include "vtkImageData.h"
#include "vtkImageReslice.h"
#include "vtkRenderWindow.h"
#include "vtkActorCollection.h"
#include "vtkBMPWriter.h"
#include "vtkImageWriter.h"
#include "vtkDataWriter.h"
#include "vtkImagePadFilter.h"
#include "vtkPointData.h"
#include "vtkRendererCollection.h"


// Default constructor
FrView2D::FrView2D(FrMainWindow* mainWindow)
: FrBaseView(mainWindow) {
    // create renderer
	m_renderer = 0;
	
    // Create filter
    m_tbcFilter = FrTBCFilter::New();
	m_tactor = vtkTextActor::New();
	m_actor2 = Fr2DSliceActor::New();

	m_slice = 0;
}

FrView2D::~FrView2D(){

    if(m_renderer) m_renderer->Delete();
    if(m_tbcFilter) m_tbcFilter->Delete();
	if(m_tactor) m_tactor->Delete();
	if(m_actor2) m_actor2->Delete();
}

//void FrView2D::UpdateSlice(){
//    FrMainDocument* document = m_mainWindow->GetDocument();
//    if(document){
//		// check input data
//		int slice = document->GetSlice();	// actually we get not slice number but slice increment, it can be positive or negative
//		m_slice += slice;
//		if (m_slice > numSlices-1)
//			m_slice = 0;//numSlices-1;
//		if (m_slice < 0)
//			m_slice = numSlices-1;//0;
//			
//		m_actor2->SetCurrentFrame(m_slice);
//		//m_actor->SetDisplayExtent(0, m_dims[0]-1, 0, m_dims[1]-1, m_slice, m_slice);
//		//m_renderer->ResetCameraClippingRange();
//		double scale = m_renderer->GetActiveCamera()->GetParallelScale();
//		m_renderer->ResetCamera();
//		m_renderer->GetActiveCamera()->SetParallelScale(scale);
//		m_imageViewer->GetRenderWindow()->Render();
//	}	
//}
//-----------------------------------------------------------------------
void FrView2D::Initialize(){
    // create renderer
    m_renderer = vtkRenderer::New();
    m_renderer->GetActiveCamera()->ParallelProjectionOn();
    m_renderer->SetBackground( 0.1, 0.2, 0.4 );

    SetupRenderers();
}

void FrView2D::SetupRenderers(){
    
    RemoveRenderers();
    
    QTVIEW3D->GetRenderWindow()->AddRenderer(m_renderer);
}

void FrView2D::RemoveRenderers(){

    // Remove all renderers
    QTVIEW3D->GetRenderWindow()->GetRenderers()->RemoveItem(m_renderer);
    QTVIEW3D->GetRenderWindow()->GetRenderers()->InitTraversal();
}

void FrView2D::SetupPipeline(){
    // Clear scene
    m_renderer->RemoveAllViewProps();
    m_renderer->ResetCamera();
    GetRenderWindow()->Render();
    
    FrMainDocument* document = GetMainWindow()->GetMainDocument();
    FrDocumentReader* docReader = FrDocumentReader::New();
    docReader->SetDocument(document);
    docReader->Update();

    // Threshold/brightness/contrast filter            
    m_tbcFilter->SetInput(docReader->GetOutput());
    m_tbcFilter->SetThreshold(document->GetThreshold());
    m_tbcFilter->SetBrightness(document->GetBrightness());
    m_tbcFilter->SetContrast(document->GetContrast());            
    m_tbcFilter->Update();

    vtkImageData* data = m_tbcFilter->GetOutput();
    m_actor = vtkImageActor::New();
    m_actor->SetInput(data);
     
    /*m_tactor->SetInput("Test text");
    m_tactor->ScaledTextOn();
    m_tactor->GetTextProperty()->SetFontSize(20);
    m_tactor->GetTextProperty()->SetBold(4);*/

    //m_actor2->SetInterpolation(0);
    //m_actor2->Update();

    m_renderer->AddActor(m_actor);
    //m_renderer->AddActor(m_tactor);
    //actor->Delete();

    //m_renderer->ResetCamera();
    //vtkCamera* cam = m_renderer->GetActiveCamera();
    //cam->ParallelProjectionOn();
    //cam->SetParallelScale(60); // 120
    //cam->SetFocalPoint(0,0,0);
    //cam->SetPosition(0,0,1);
    //cam->SetViewUp(0,1,0);
    
    // redraw scene
    GetRenderWindow()->Render();
}

void FrView2D::UpdatePipeline(){

    if(m_tbcFilter->GetInput() != 0){
        
        FrMainDocument* document = GetMainWindow()->GetMainDocument();

        m_tbcFilter->SetThreshold(document->GetThreshold());
        m_tbcFilter->SetBrightness(document->GetBrightness());
        m_tbcFilter->SetContrast(document->GetContrast());
        m_tbcFilter->Update();

        m_actor->SetInput(m_tbcFilter->GetOutput());
    }
    GetRenderWindow()->Render();
}