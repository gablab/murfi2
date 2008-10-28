#include "FrSliceView.h"
#include "QVTKWidget.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrImageDocObj.h"
#include "FrInteractorStyle.h"
#include "FrDocumentReader.h"
#include "FrSliceExtractor.h"
#include "FrTabSettingsDocObj.h"
#include "FrLayeredImage.h"
#include "FrNotify.h"
#include "FrUtils.h"


// VTK
#include "vtkRenderWindowInteractor.h"
#include "vtkInteractorStyleImage.h"
#include "vtkRenderer.h"
#include "vtkCamera.h"
#include "vtkImageData.h"
#include "vtkImageActor.h"
#include "vtkPNGReader.h"
#include "vtkRenderWindow.h"
#include "vtkActorCollection.h"
#include "vtkRendererCollection.h"


// Default constructor
FrSliceView::FrSliceView(FrMainWindow* mainWindow)
: FrBaseView(mainWindow) {
    	
    // Create pipeline stuff
    m_docReader = FrDocumentReader::New();
	m_SliceExtractor = FrSliceExtractor::New();
    m_LayeredImage = FrLayeredImage::New();
}

FrSliceView::~FrSliceView(){
	if(m_SliceExtractor) m_SliceExtractor->Delete();
    if(m_docReader) m_docReader->Delete();
    if(m_LayeredImage) m_LayeredImage->Delete();
}

void FrSliceView::Initialize(){
    // Some initialization
    this->SetupRenderers();
}

void FrSliceView::SetupRenderers(){
    this->RemoveRenderers();

    // Get all renderers
    std::vector<vtkRenderer*> renderers;
	m_LayeredImage->GetRenderers(renderers);

    // Add them 
    vtkRenderWindow* renWin = QTVIEW3D->GetRenderWindow();
    std::vector<vtkRenderer*>::iterator it, itEnd(renderers.end());
    for(it = renderers.begin(); it != itEnd; ++it){
        renWin->AddRenderer( (*it) );
    }

    renWin->GetRenderers()->InitTraversal();
    renWin->SetNumberOfLayers( int(renderers.size()) );
}

void FrSliceView::RemoveRenderers(){
    // Get all renderers
    std::vector<vtkRenderer*> renderers;
	m_LayeredImage->GetRenderers(renderers);

    // Remove them
    vtkRenderWindow* renWin = QTVIEW3D->GetRenderWindow();
    std::vector<vtkRenderer*>::iterator it, itEnd(renderers.end());
    for(it = renderers.begin(); it != itEnd; ++it){
        renWin->RemoveRenderer( (*it) );
        (*it)->SetRenderWindow(0);
    }

    renWin->GetRenderers()->InitTraversal();
    renWin->SetNumberOfLayers(0);
}

void FrSliceView::UpdatePipeline(int point){
    // Setup some vars
    int maxSliceNumber = 0;

    // Get common settings
    FrMainDocument* document = GetMainWindow()->GetMainDocument();
    FrTabSettingsDocObj* tabSets = document->GetCurrentTabSettings();
    FrSliceViewSettings* viewSets = tabSets->GetSliceViewSettings();

    // Get settings of layers
    int layerID = viewSets->ActiveLayerID;
    FrLayerSettings* layer = 0L;
    LayerCollection  layers;
    GetLayerSettings(viewSets, layers);
    LayerCollection::iterator it, itEnd(layers.end());    

    // If not 'broadcast update' then get layer settings 
    if(layerID != ALL_LAYERS_ID){
        for(it = layers.begin(); it != itEnd; ++it){
            if((*it)->ID == viewSets->ActiveLayerID){
                layer = (*it);
                break;
            }
        }
    }
                    	
    // Update pipeline
    switch(point)
    {
    case FRP_FULL:
        {
            // NOTE: Do nothing here !!!
        }
    case FRP_READIMAGE:
        {
            // read document and connect filters
            m_docReader->SetDocument(document);
            m_docReader->SetUnMosaicOn();
            m_docReader->Update();
			
            m_SliceExtractor->SetInput(m_docReader->GetOutput());
        }
	case FRP_SLICE:
        {
            // set slice to be rendered
            maxSliceNumber = m_SliceExtractor->GetMaxSliceNumber();
            viewSets->SliceNumber = ClampValue(viewSets->SliceNumber, 0, maxSliceNumber);
            m_SliceExtractor->SetSlice(viewSets->SliceNumber);

            if(m_SliceExtractor->GetInput()){
			    m_SliceExtractor->Update();
                m_LayeredImage->SetInput(m_SliceExtractor->GetOutput());

                // Set text
                char text[255];
                sprintf(text, "Slice View: %d of %d", 
                    viewSets->SliceNumber, maxSliceNumber);
                m_LayeredImage->SetText(text);
            }
            else {
                m_LayeredImage->SetText("");
            }
        }
    case FRP_COLORMAP:
        {
            // Update here colormap values
            if(viewSets->ActiveLayerID == ALL_LAYERS_ID){
                for(it=layers.begin(); it != itEnd; ++it){
                    m_LayeredImage->SetColormapSettings(
                        (*it)->ColormapSettings, (*it)->ID);
                }
            }
            else {
                m_LayeredImage->SetColormapSettings(
                    layer->ColormapSettings, layer->ID);
            }
            m_LayeredImage->UpdateColormap();
        }
    case FRP_TBC:
        {
            // set TBC values
            if(viewSets->ActiveLayerID == ALL_LAYERS_ID){
                for(it=layers.begin(); it != itEnd; ++it){
                    m_LayeredImage->SetTBCSettings(
                        (*it)->TbcSettings, (*it)->ID);
                }
            }
            else {
                m_LayeredImage->SetTBCSettings(
                    layer->TbcSettings, layer->ID);
            }
            m_LayeredImage->UpdateTBC();
        }
    case FRP_OPACITY_VISIBILITY:
        {
            // Opacity and visibility
            if(viewSets->ActiveLayerID == ALL_LAYERS_ID){
                for(it=layers.begin(); it != itEnd; ++it){
                    m_LayeredImage->SetOpacity((*it)->Opacity, (*it)->ID);
                    m_LayeredImage->SetVisibility((*it)->Visibility, (*it)->ID);
                }
            }
            else {
                m_LayeredImage->SetOpacity(layer->Opacity, layer->ID);
                m_LayeredImage->SetVisibility(layer->Visibility, layer->ID);
            }
        }
    case FRP_SETCAM:
        {
            m_LayeredImage->SetCameraSettings(viewSets->CamSettings, ALL_LAYERS_ID);	
            m_LayeredImage->UpdateCamera();
        }
    default:
        // do nothing
        break;
    }

    // Draw border
    m_LayeredImage->UpdateBorder();

    // redraw scene
	GetRenderWindow()->Render();
}
