#include "FrSliceView.h"
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
#include "FrUtils.h"
#include "FrSliceExtractor.h"
#include "FrTabSettingsDocObj.h"
#include "FrLayeredImage.h"
#include "FrColormapFilter.h"

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
FrSliceView::FrSliceView(FrMainWindow* mainWindow)
: FrBaseView(mainWindow) {
    	
    // Create pipeline stuff
    m_docReader = FrDocumentReader::New();
    m_tbcFilter = FrTBCFilter::New();
	m_SliceExtractor = FrSliceExtractor::New();
//	m_actor = Fr2DSliceActor::New();
//    m_tactor = vtkTextActor::New();
//    m_renderer = vtkRenderer::New();

	m_LayeredImage = new FrLayeredImage(GetRenderWindow());
}

FrSliceView::~FrSliceView(){
//    if(m_renderer) m_renderer->Delete();
    if(m_tbcFilter) m_tbcFilter->Delete();
	if(m_SliceExtractor) m_SliceExtractor->Delete();
//	if(m_actor) m_actor->Delete();
//    if(m_tactor) m_tactor->Delete();
    if(m_docReader) m_docReader->Delete();
}

void FrSliceView::Initialize(){
    // create renderer
//    m_renderer->GetActiveCamera()->ParallelProjectionOn();
//    m_renderer->SetBackground( 0.0, 0.0, 0.0 );

//    SetupRenderers();

	m_LayeredImage->Initialize();		// test

	GetRenderWindow()->Render();
}

void FrSliceView::SetupRenderers(){
    RemoveRenderers();
	
	m_LayeredImage->SetupRenderers();
//    QTVIEW3D->GetRenderWindow()->AddRenderer(m_renderer);
}

void FrSliceView::RemoveRenderers(){
    // Remove all renderers
	m_LayeredImage->RemoveRenderers();
//    QTVIEW3D->GetRenderWindow()->GetRenderers()->RemoveItem(m_renderer);
//    QTVIEW3D->GetRenderWindow()->GetRenderers()->InitTraversal();
}

void FrSliceView::UpdatePipeline(int point){
    // Setup some vars
    int sliceNumber = 0;
    bool isCleared  = false;

    vtkCamera* cam = 0L;    
    FrMainDocument* document = GetMainWindow()->GetMainDocument();
    ViewSettings& settings = document->GetCurrentTabSettings()->GetSliceViewSettings();
    TBCSettings& tbcSettings = settings.TbcSetting;
    CameraSettings& camSettings = settings.CamSettings;
	LayeredImageSettings& liSettings = document->GetCurrentTabSettings()->GetLayeredImageSettings();

	//FrColormapFilter* cmf = FrColormapFilter::New();	// test

	char text[20] = "";
	char tmp[5];

    // Update pipeline
    switch(point)
    {
    case FRP_FULL:
        {
            // Clear scene
//            m_renderer->RemoveAllViewProps();
//            m_renderer->ResetCamera();
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
			    //m_tactor->GetTextProperty()->SetColor(100, 100, 0);
			    //m_tactor->GetTextProperty()->SetBold(5);
			    //m_tactor->GetTextProperty()->SetFontSize(20);
			    //m_tactor->SetPosition(20, 20);
       //         m_renderer->AddActor(m_tactor);
            }
        }
	case FRP_LAYER:
		{
			m_LayeredImage->SetParams(liSettings);
		}
	case FRP_SLICE:
        {
            // set slice to be rendered
            settings.SliceNumber = ClampValue(settings.SliceNumber, 
                0, m_SliceExtractor->GetMaxSliceNumber());
            m_SliceExtractor->SetSlice(settings.SliceNumber);

            if(m_SliceExtractor->GetInput()){
			    m_SliceExtractor->Update();
                m_tbcFilter->SetInput(m_SliceExtractor->GetOutput());
            }
			
            // Set tactor input
			strcat(text, "Slice ");
            itoa(settings.SliceNumber, tmp, 10);
			strcat(text, tmp);
			m_LayeredImage->SetText(text);
			//m_tactor->SetInput(text);
        }
    case FRP_TBC:
        {
            // Threshold/brightness/contrast filter
            m_tbcFilter->SetThreshold(tbcSettings.Threshold);
            m_tbcFilter->SetBrightness(tbcSettings.Brightness);
            m_tbcFilter->SetContrast(tbcSettings.Contrast);
            
            // Keep data up-to-date
            tbcSettings.Threshold  = m_tbcFilter->GetThreshold();
            tbcSettings.Brightness = m_tbcFilter->GetBrightness();
            tbcSettings.Contrast   = m_tbcFilter->GetContrast();

            if(m_tbcFilter->GetInput()){
                m_tbcFilter->Update();
                //m_actor->SetInput(m_tbcFilter->GetOutput());
				
				//cmf->SetInput(m_tbcFilter->GetOutput());	// test
				//cmf->Update();								// test

				m_LayeredImage->SetInput(m_tbcFilter->GetOutput());
				// Add actor
                if(isCleared) {
                //    m_renderer->AddActor(m_actor);
                }
            }
        }
    case FRP_SETCAM:
        {
            // Setup camera here 
     //       cam = m_renderer->GetActiveCamera();
     //       cam->ParallelProjectionOn();
     //       cam->SetParallelScale(camSettings.Scale);
     //       cam->SetFocalPoint(camSettings.FocalPoint);
     //       cam->SetViewUp(camSettings.ViewUp);
     //       cam->SetPosition(camSettings.Position);
			m_LayeredImage->SetCamera(camSettings);	
        }
    default:
        // do nothing
        break;
    }

    // redraw scene
	//m_LayeredImage->Update();
	GetRenderWindow()->Render();
}

void FrSliceView::AddLayer(LayerSettings ls){
	m_LayeredImage->AddLayer(ls);
}