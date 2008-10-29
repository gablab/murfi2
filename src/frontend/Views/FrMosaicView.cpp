#include "FrMosaicView.h"
#include "FrMainWindow.h"
#include "QVTKWidget.h"
#include "FrDocumentReader.h"
#include "FrDocument.h"
#include "FrMainDocument.h"
#include "FrNotify.h"
#include "FrTabSettingsDocObj.h"
#include "FrLayeredImage.h"
#include "FrUtils.h"


#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRendererCollection.h"


// Default constructor
FrMosaicView::FrMosaicView(FrMainWindow* mainWindow)
: FrBaseView(mainWindow) {
    m_docReader = FrDocumentReader::New();	
    m_LayeredImage = FrLayeredImage::New();
}

FrMosaicView::~FrMosaicView(){
    this->RemoveRenderers();
    if(m_docReader) m_docReader->Delete();
    if(m_LayeredImage) m_LayeredImage->Delete();
}

void FrMosaicView::Initialize(){
    SetupRenderers();    
}

void FrMosaicView::SetupRenderers(){
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
    renWin->SetNumberOfLayers(renderers.size());
}

void FrMosaicView::RemoveRenderers(){
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

void FrMosaicView::UpdatePipeline(int point){
    // Get common settings
    FrMainDocument* document = GetMainWindow()->GetMainDocument();
    FrTabSettingsDocObj* tabSets = document->GetCurrentTabSettings();
    FrMosaicViewSettings* viewSets = tabSets->GetMosaicViewSettings();

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
            m_docReader->SetMosaicOn();
            m_docReader->Update();
			
            m_LayeredImage->SetInput(m_docReader->GetOutput());
        }
	case FRP_SLICE:
        {
            // NOTE: Do nothing here but setup text
            // Set text
            if(m_LayeredImage->GetInput()){
                m_LayeredImage->SetText("Mosaic View:");
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