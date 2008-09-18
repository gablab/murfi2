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
#include "FrNotify.h"
#include "FrSliceExtractor.h"

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
#include "vtkOutlineFilter.h"
#include "vtkPolyDataMapper.h"
#include "vtkFollower.h"


// Default constructor
FrView2D::FrView2D(FrMainWindow* mainWindow)
: FrBaseView(mainWindow) {
    	
    // Create pipline stuff
    m_docReader = FrDocumentReader::New();
    m_tbcFilter = FrTBCFilter::New();
	m_SliceExtractor = FrSliceExtractor::New();
	m_MosaicFilter = FrMosaicFilter::New();
	m_tactor = vtkTextActor::New();
	m_actor2 = Fr2DSliceActor::New();
    m_actor = vtkImageActor::New();
    m_renderer = vtkRenderer::New();

	m_slice = 0;
}

FrView2D::~FrView2D(){

    if(m_renderer) m_renderer->Delete();
    if(m_tbcFilter) m_tbcFilter->Delete();
	if(m_SliceExtractor) m_SliceExtractor->Delete();
	if(m_tactor) m_tactor->Delete();
	if(m_actor2) m_actor2->Delete();
    if(m_actor) m_actor->Delete();
    if(m_docReader) m_docReader->Delete();
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
//		GetRenderWindow()->Render();
//	}	
//}

void FrView2D::Initialize(){
    // create renderer
    m_renderer->GetActiveCamera()->ParallelProjectionOn();
    m_renderer->SetBackground( 0.0, 0.0, 0.0 );

    SetupRenderers();
//	GetRenderWindow()->SetCurrentCursor(5);
//	GetRenderWindow()->ShowCursor();
	GetRenderWindow()->Render();
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

void FrView2D::UpdatePipeline(int point){
    // Setup some vars
    int slice = 0;
    int maxSliceNumber = 0;
    bool isCleared = false;
    FrMainDocument* document = GetMainWindow()->GetMainDocument();
	char text[20] = "";
	char tmp[5];

    // Update pipeline
    switch(point)
    {
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
            // read document and connect filters
            m_docReader->SetDocument(document);
            m_docReader->SetUnMosaicOn(true);
            m_docReader->Update();
			
			m_SliceExtractor->SetInput(m_docReader->GetOutput());

			// set text actor
			m_tactor->GetTextProperty()->SetColor(100, 100, 0);
			m_tactor->GetTextProperty()->SetBold(5);
			m_tactor->GetTextProperty()->SetFontSize(20);
			m_tactor->SetPosition(20, 20);
			m_renderer->AddActor(m_tactor);	
        }
    case FRP_SLICE:
        {
            // Extract slice to be rendered
            slice = m_SliceExtractor->GetSlice();
            slice += document->GetSlice(); // this is delta

            // clamp value and set it
            maxSliceNumber = m_SliceExtractor->GetMaxSliceNumber();
            if(slice > maxSliceNumber) slice = maxSliceNumber;
            if(slice < 0) slice = 0;
			m_SliceExtractor->SetSlice(slice);

            if(m_SliceExtractor->GetInput()){
			    m_SliceExtractor->Update();
                m_tbcFilter->SetInput(m_SliceExtractor->GetOutput());
            }
			
			strcat(text, "Slice ");
			itoa(slice, tmp, 10);
			strcat(text, tmp);

			m_tactor->SetInput(text);
        }
    case FRP_TBC:
        {
            // Threshold/brightness/contrast filter
            m_tbcFilter->SetThreshold(document->GetThreshold());
            m_tbcFilter->SetBrightness(document->GetBrightness());
            m_tbcFilter->SetContrast(document->GetContrast());            
                        
            if(m_tbcFilter->GetInput()){
                m_tbcFilter->Update();

                m_actor2->SetInput(m_tbcFilter->GetOutput());
                // Add actor
                if(isCleared) {
                    m_renderer->AddActor(m_actor2);
                }
            }
        }
    case FRP_SETCAM:
        {
            // TODO: Setup camera here 
            m_renderer->ResetCamera();
			m_renderer->GetActiveCamera()->ParallelProjectionOn();
			m_renderer->GetActiveCamera()->SetParallelScale(200);
		
			m_renderer->Render();
        }
    default:
        // do nothing
        break;
    }

    // redraw scene
    GetRenderWindow()->Render();
}