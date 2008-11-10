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

// VTK stuff
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRendererCollection.h"

// FrUpdateParams represents params 
class FrUpdateParams1 {
public:
    FrMainDocument*      Document;
    FrTabSettingsDocObj* TabSettings;
    FrMosaicViewSettings* ViewSettings;
    int                  ActiveLayerID;
    FrLayerSettings*     ActiveLayer;
    LayerCollection      Layers;
};



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
    FrUpdateParams1 params;
    InitUpdateParams(params);
                    	
    // Update pipeline
    switch(point)
    {
    case FRP_FULL:
        {
            // NOTE: Do nothing here !!!
        }
    case FRP_READIMAGE:
        {
            this->ReadDocument(params);
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
            this->UpdateColormap(params);
        }
    case FRP_TBC:
        {
            this->UpdateTBC(params);
        }
    case FRP_OPACITY_VISIBILITY:
        {
            this->UpdateOpacityVisibility(params);
        }
    case FRP_SETCAM:
        {
            m_LayeredImage->SetCameraSettings(
                params.ViewSettings->CamSettings, ALL_LAYERS_ID);	
            m_LayeredImage->UpdateCamera();
        }
    default:
        // do nothing
        break;
    }

    // Draw border
    // m_LayeredImage->UpdateBorder();

    // redraw scene
	GetRenderWindow()->Render();
}

void FrMosaicView::InitUpdateParams(FrUpdateParams1& params){
    // Init params used when updating
    params.Document     = this->GetMainWindow()->GetMainDocument();
    params.TabSettings  = params.Document->GetCurrentTabSettings();
    params.ViewSettings = params.TabSettings->GetMosaicViewSettings();

    params.ActiveLayerID = params.ViewSettings->ActiveLayerID;    
    GetLayerSettings(params.ViewSettings, params.Layers);

    // If not 'broadcast update' then get layer settings 
    if(params.ActiveLayerID != ALL_LAYERS_ID){
        // Set default value
        params.ActiveLayer = 0L;

        // Look for layer settings
        LayerCollection::iterator it, itEnd(params.Layers.end());
        for(it = params.Layers.begin(); it != itEnd; ++it){
            if((*it)->ID == params.ActiveLayerID){
                params.ActiveLayer = (*it);
                break;
            }
        }
    }
}

void FrMosaicView::ReadDocument(FrUpdateParams1& params){
    // read document and connect filters
    m_docReader->SetDocument(params.Document);
    m_docReader->SetMosaicOn();
    m_docReader->Update();
	
    // Pass image input 
    m_LayeredImage->SetInput(m_docReader->GetOutput());
    
    // Pass ROI data from output starting from 1
    int count = m_docReader->GetOutputCount();
    for(int i=1; i < count; ++i){
        m_LayeredImage->SetROIInput(i, m_docReader->GetOutput(i));
    }
}

void FrMosaicView::UpdateColormap(FrUpdateParams1& params){
    if(params.ActiveLayerID == ALL_LAYERS_ID){
        // Update all layers
        LayerCollection::iterator it, itEnd(params.Layers.end());
        for(it = params.Layers.begin(); it != itEnd; ++it){
            m_LayeredImage->SetColormapSettings(
                (*it)->ColormapSettings, (*it)->ID);
        }
    }
    else {
        m_LayeredImage->SetColormapSettings(
            params.ActiveLayer->ColormapSettings, params.ActiveLayer->ID);
    }
    m_LayeredImage->UpdateColormap();
}

void FrMosaicView::UpdateTBC(FrUpdateParams1& params){
    // Update TBC
    if(params.ActiveLayerID == ALL_LAYERS_ID){
        // Update all layers
        LayerCollection::iterator it, itEnd(params.Layers.end());
        for(it = params.Layers.begin(); it != itEnd; ++it){
            m_LayeredImage->SetTBCSettings((*it)->TbcSettings, (*it)->ID);
        }
    }
    else {
        m_LayeredImage->SetTBCSettings(
            params.ActiveLayer->TbcSettings, params.ActiveLayer->ID);
    }
    m_LayeredImage->UpdateTBC();
}

void FrMosaicView::UpdateOpacityVisibility(FrUpdateParams1& params){
    // Update Opacity and visibility
    if(params.ActiveLayerID == ALL_LAYERS_ID){
        // Update all params
        LayerCollection::iterator it, itEnd(params.Layers.end());
        for(it = params.Layers.begin(); it != itEnd; ++it){
            m_LayeredImage->SetOpacity((*it)->Opacity, (*it)->ID);
            m_LayeredImage->SetVisibility((*it)->Visibility, (*it)->ID);
        }
    }
    else {
        m_LayeredImage->SetOpacity(
            params.ActiveLayer->Opacity, params.ActiveLayer->ID);
        m_LayeredImage->SetVisibility(
            params.ActiveLayer->Visibility, params.ActiveLayer->ID);
    }

    // TODO: add ROI opacity and visibility update ???
}