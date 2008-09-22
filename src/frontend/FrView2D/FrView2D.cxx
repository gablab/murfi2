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
#include "FrTabSettingsDocObj.h"

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
	m_actor = Fr2DSliceActor::New();
    m_tactor = vtkTextActor::New();
    m_renderer = vtkRenderer::New();
}

FrView2D::~FrView2D(){

    if(m_renderer) m_renderer->Delete();
    if(m_tbcFilter) m_tbcFilter->Delete();
	if(m_SliceExtractor) m_SliceExtractor->Delete();
	if(m_actor) m_actor->Delete();
    if(m_tactor) m_tactor->Delete();
    if(m_docReader) m_docReader->Delete();
}

void FrView2D::Initialize(){
    // create renderer
    m_renderer->GetActiveCamera()->ParallelProjectionOn();
    m_renderer->SetBackground( 0.0, 0.0, 0.0 );

    SetupRenderers();
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
    int slice           = 0;
    int maxSliceNumber  = 0;
    bool isCleared      = false;

    vtkCamera* cam = 0L;    
    FrMainDocument* document = GetMainWindow()->GetMainDocument();
    ViewSettings& settings = document->GetCurrentTabSettings()->GetSliceViewSettings();
    CameraSettings& camSettings = settings.CamSettings;

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
            if(isCleared){
			    m_tactor->GetTextProperty()->SetColor(100, 100, 0);
			    m_tactor->GetTextProperty()->SetBold(5);
			    m_tactor->GetTextProperty()->SetFontSize(20);
			    m_tactor->SetPosition(20, 20);
                m_renderer->AddActor(m_tactor);
            }
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
			
            // Set tactor input
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
                m_actor->SetInput(m_tbcFilter->GetOutput());
                // Add actor
                if(isCleared) {
                    m_renderer->AddActor(m_actor);
                }
            }
        }
    case FRP_SETCAM:
        {
            // TODO: Setup camera here 
            cam = m_renderer->GetActiveCamera();
            cam->ParallelProjectionOn();
            cam->SetParallelScale(camSettings.Scale);
            cam->SetFocalPoint(camSettings.FocalPoint);
            cam->SetViewUp(camSettings.ViewUp);

            cam->SetPosition(camSettings.Position);
        }
    default:
        // do nothing
        break;
    }

    // redraw scene
    GetRenderWindow()->Render();
}