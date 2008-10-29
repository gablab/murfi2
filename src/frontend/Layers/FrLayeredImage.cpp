#include "FrLayeredImage.h"
#include "FrSpecialLayer.h"
#include "FrColormapFilter.h"
#include "FrTBCFilter.h"
#include "FrSettings.h"


// VTK stuff
#include "vtkRendererCollection.h"
#include "vtkObjectFactory.h"
#include "vtkRenderWindow.h"
#include "vtkImageData.h"
#include "vtkRenderer.h"
#include "vtkCamera.h"
#include "vtkTextMapper.h"
#include "vtkActor2D.h"
#include "vtkTextProperty.h"

#define START_LAYER_ID 1


vtkStandardNewMacro(FrLayeredImage);


FrLayeredImage::FrLayeredImage() 
: FrImageLayer(), m_nextImageLayerID(DEFAULT_LAYER_ID) {
    
    this->InitDefault(this);
       
    // NOTE: remove colormap filter from pipline
    if(m_cmFilter) {
        m_cmFilter->Delete();
        m_cmFilter = 0L;
    }
    m_SpecialLayer = FrSpecialLayer::New();
}

FrLayeredImage::~FrLayeredImage(){
    // delete all layers here
    this->RemoveImageLayers();
    // and other stuff too
    if(m_SpecialLayer){
        m_SpecialLayer->Delete();
    }
}

// Modifiers
void FrLayeredImage::SetInput(vtkImageData* data){
    LayerCollection::iterator it, itEnd(m_ImageLayers.end());
    for(it = m_ImageLayers.begin(); it != itEnd; ++it){
        (*it)->SetInput(data);
    }    
    // NOTE: Do not use colormap for default layer and 
    // set input to TBC filter immediatly
    m_tbcFilter->SetInput(data);
}

void FrLayeredImage::SetColormapSettings(FrColormapSettings& settings, int layerId){
    if(layerId == ALL_LAYERS_ID){
        LayerCollection::iterator it, itEnd(m_ImageLayers.end());
        for(it = m_ImageLayers.begin(); it != itEnd; ++it){
            (*it)->SetColormapSettings(settings);
        }
        FrImageLayer::SetColormapSettings(settings);
    }
    else{
        FrImageLayer* layer = this->GetLayerByID(layerId);
        if(layer){
            layer->SetColormapSettings(settings);
        }
    }
}

void FrLayeredImage::SetTBCSettings(FrTBCSettings& settings, int layerId){
    if(layerId == ALL_LAYERS_ID){
        LayerCollection::iterator it, itEnd(m_ImageLayers.end());
        for(it = m_ImageLayers.begin(); it != itEnd; ++it){
            (*it)->SetTBCSettings(settings);
        }
        FrImageLayer::SetTBCSettings(settings);
    }
    else{
        FrImageLayer* layer = this->GetLayerByID(layerId);
        if(layer){
            layer->SetTBCSettings(settings);
        }
    }
}

void FrLayeredImage::SetCameraSettings(FrCameraSettings& settings, int layerId){
    if(layerId == ALL_LAYERS_ID){
        LayerCollection::iterator it, itEnd(m_ImageLayers.end());
        for(it = m_ImageLayers.begin(); it != itEnd; ++it){
            (*it)->SetCameraSettings(settings);
        }
        FrImageLayer::SetCameraSettings(settings);
    }
    else{
        FrImageLayer* layer = this->GetLayerByID(layerId);
        if(layer){
            layer->SetCameraSettings(settings);
        }
    }
    // Update camera for special layer
    m_SpecialLayer->SetCameraSettings(settings);
}

void FrLayeredImage::SetOpacity(double value, int layerId){
    if(layerId == ALL_LAYERS_ID){
        LayerCollection::iterator it, itEnd(m_ImageLayers.end());
        for(it = m_ImageLayers.begin(); it != itEnd; ++it){
            (*it)->SetOpacity(value);
        }
        FrImageLayer::SetOpacity(value);
    }
    else{
        FrImageLayer* layer = this->GetLayerByID(layerId);
        if(layer){
            layer->SetOpacity(value);
        }
    }
}

void FrLayeredImage::SetVisibility(bool value, int layerId){
    if(layerId == ALL_LAYERS_ID){
        LayerCollection::iterator it, itEnd(m_ImageLayers.end());
        for(it = m_ImageLayers.begin(); it != itEnd; ++it){
            (*it)->SetVisibility(value);
        }
        FrImageLayer::SetVisibility(value);
    }
    else{
        FrImageLayer* layer = this->GetLayerByID(layerId);
        if(layer){
            layer->SetVisibility(value);
        }
    }
}

// Initialization
void FrLayeredImage::InitDefault(FrImageLayer* layer){
    FrColormapSettings cms;
    InitColormapDefault(&cms);
    layer->SetColormapSettings(cms);

    FrTBCSettings tbcs;
    InitTbcDefault(&tbcs);
    layer->SetTBCSettings(tbcs);

    FrCameraSettings cs;
    InitCameraDefault(&cs);
    layer->SetCameraSettings(cs);

    // other
    layer->SetOpacity(DEF_LAYER_OPACITY);
    layer->SetVisibility(DEF_LAYER_VISIBILITY);

    // init ID
    layer->SetID(m_nextImageLayerID);
    ++m_nextImageLayerID;
}

// Update methods
void FrLayeredImage::UpdateColormap(){
    LayerCollection::iterator it, itEnd(m_ImageLayers.end());
    for(it = m_ImageLayers.begin(); it != itEnd; ++it){
        (*it)->UpdateColormap();
    }
    // NOTE: skip colormap update for default layer
    //FrImageLayer::UpdateColormap();
}

void FrLayeredImage::UpdateTBC(){    
    LayerCollection::iterator it, itEnd(m_ImageLayers.end());
    for(it = m_ImageLayers.begin(); it != itEnd; ++it){
        (*it)->UpdateTBC();
    }
    FrImageLayer::UpdateTBC();
}

void FrLayeredImage::UpdateCamera(){
    LayerCollection::iterator it, itEnd(m_ImageLayers.end());
    for(it = m_ImageLayers.begin(); it != itEnd; ++it){
        (*it)->UpdateCamera();
    }
    FrImageLayer::UpdateCamera();
}

// Layer management
int FrLayeredImage::AddLayer(){
    FrImageLayer* layer = FrImageLayer::New();

    // Now init all ...    
    this->InitDefault(layer);
    layer->SetInput(m_tbcFilter->GetInput());

    // ... and synchronize camera ...
    FrCameraSettings cs;
    this->GetCameraSettings(cs);
    layer->SetCameraSettings(cs);

    // ... and render window with viewport
    // viewport's important for ortho view
    vtkRenderer* ren = layer->GetRenderer();
    ren->SetViewport(m_Renderer->GetViewport());
    m_ImageLayers.push_back(layer);

    // Update render window if any
    vtkRenderWindow* rw = m_Renderer->GetRenderWindow();
    if(rw){
        // HACK: GetRenderers() reorder all layers
        // setting proper 'LayerNumber' !!!
        std::vector<vtkRenderer*> rens;
        this->GetRenderers(rens);
        
        // Add renderer to render window
        rw->AddRenderer(ren);
        rw->SetNumberOfLayers(int(rens.size()));
    }

    // Return layer's ID
    return layer->GetID();
}

bool FrLayeredImage::RemoveLayer(int layerId){
    if(layerId == DEFAULT_LAYER_ID) return false;

    bool result = false;
    LayerCollection::iterator it, itEnd(m_ImageLayers.end());
    for(it = m_ImageLayers.begin(); it != itEnd; ++it){
        if((*it)->GetID() == layerId) break;
    }
    
    // delete layer if it were found
    if(it != itEnd){
        FrImageLayer* layer = (*it);
        m_ImageLayers.erase(it);

        // Remove renderer from render window if any
        vtkRenderWindow* rw = layer->GetRenderer()->GetRenderWindow();
        if(rw != 0){
            rw->RemoveRenderer(layer->GetRenderer());
            rw->GetRenderers()->InitTraversal();

            // HACK: GetRenderers() reorder all layers
            // setting proper 'LayerNumber' !!!
            std::vector<vtkRenderer*> rens;
            this->GetRenderers(rens);
            
            rw->SetNumberOfLayers(rens.size());
        }

        // Delete layer object
        layer->Delete();
        result = true;
    }
    return result;
}

void FrLayeredImage::RemoveImageLayers(){
    vtkRenderWindow* rw = m_Renderer->GetRenderWindow();

    // Delete all layers
    LayerCollection::iterator it, itEnd(m_ImageLayers.end());
    for(it = m_ImageLayers.begin(); it != itEnd; ++it){
        FrImageLayer* layer = (*it);
        if(rw){
            vtkRenderer* ren = layer->GetRenderer();
            rw->RemoveRenderer(ren);
            ren->SetRenderWindow(0L);
        }
        layer->Delete();
    }
    // Update number of layers
    if(rw){
        // NOTE: we have 2 layers : default and special
        m_SpecialLayer->GetRenderer()->SetLayer(1);
        rw->SetNumberOfLayers(2);
    }

    m_ImageLayers.clear();
}

FrImageLayer* FrLayeredImage::GetLayerByID(int id){
    if(id == DEFAULT_LAYER_ID){
        return (FrImageLayer*)this;
    }
    else{
        LayerCollection::iterator it, itEnd(m_ImageLayers.end());
        for(it = m_ImageLayers.begin(); it != itEnd; ++it){
            if((*it)->GetID() == id) return (*it);
        }
    }
    return 0L;
}

void FrLayeredImage::GetRenderers(std::vector<vtkRenderer*>& renderers){
    // First add default
    renderers.clear();
    int layerNumber = 0;

    renderers.push_back(m_Renderer);
    m_Renderer->SetLayer(layerNumber);
    ++layerNumber;

    // Add renderer of each layer and udate layer number
    LayerCollection::iterator it, itEnd(m_ImageLayers.end());
    for(it = m_ImageLayers.begin(); it != itEnd; ++it){
        vtkRenderer* ren = (*it)->GetRenderer();
        ren->SetLayer(layerNumber);
        renderers.push_back(ren);
        
        ++layerNumber;
    }

    // Last add renderer from special layer
    vtkRenderer* ren = m_SpecialLayer->GetRenderer();
    ren->SetLayer(layerNumber);
    renderers.push_back(ren);
}

// Text and border management
void FrLayeredImage::SetText(const char* text){
    m_SpecialLayer->SetText(text);
}

void FrLayeredImage::UpdateBorder(){
    vtkRenderWindow* rw = m_Renderer->GetRenderWindow();
    if(rw){
        int* size = m_Renderer->GetSize();
        m_SpecialLayer->UpdateBorder(size[0]-1, size[1]);
    }
}