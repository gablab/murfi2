#include "FrMyLayeredImage.h"
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

#define START_LAYER_ID 1


FrMyLayeredImage* FrMyLayeredImage::New(){  
  vtkObject* result = vtkObjectFactory::CreateInstance("FrMyLayeredImage");
  if(result){
      return (FrMyLayeredImage*)result;
  }
  return new FrMyLayeredImage();
}


FrMyLayeredImage::FrMyLayeredImage() {
    m_nextLayerId = DEFAULT_LAYER_ID;
    this->InitDefault(this);

    // NOTE: remove colormap filter from pipline
    if(m_cmFilter) {
        m_cmFilter->Delete();
        m_cmFilter = 0L;
    }
}

FrMyLayeredImage::~FrMyLayeredImage(){
    // delete all layers here
    for(int id = 0; id < m_nextLayerId; ++id){
        this->RemoveLayer(id);
    }
}

// Modifiers
void FrMyLayeredImage::SetInput(vtkImageData* data){
    LayerCollection::iterator it, itEnd(m_layers.end());
    for(it = m_layers.begin(); it != itEnd; ++it){
        (*it)->SetInput(data);
    }    
    // NOTE: Do not use colormap for default layer and 
    // set input to TBC filter immediatly
    m_tbcFilter->SetInput(data);
}

void FrMyLayeredImage::SetColormapSettings(FrColormapSettings& settings, int layerId){
    if(layerId == ALL_LAYERS_ID){
        LayerCollection::iterator it, itEnd(m_layers.end());
        for(it = m_layers.begin(); it != itEnd; ++it){
            (*it)->SetColormapSettings(settings);
        }
        FrMyLayer::SetColormapSettings(settings);
    }
    else{
        FrMyLayer* layer = this->GetLayerByID(layerId);
        if(layer){
            layer->SetColormapSettings(settings);
        }
    }
}

void FrMyLayeredImage::SetTBCSettings(FrTBCSettings& settings, int layerId){
    if(layerId == ALL_LAYERS_ID){
        LayerCollection::iterator it, itEnd(m_layers.end());
        for(it = m_layers.begin(); it != itEnd; ++it){
            (*it)->SetTBCSettings(settings);
        }
        FrMyLayer::SetTBCSettings(settings);
    }
    else{
        FrMyLayer* layer = this->GetLayerByID(layerId);
        if(layer){
            layer->SetTBCSettings(settings);
        }
    }
}

void FrMyLayeredImage::SetCameraSettings(FrCameraSettings& settings, int layerId){
    if(layerId == ALL_LAYERS_ID){
        LayerCollection::iterator it, itEnd(m_layers.end());
        for(it = m_layers.begin(); it != itEnd; ++it){
            (*it)->SetCameraSettings(settings);
        }
        FrMyLayer::SetCameraSettings(settings);
    }
    else{
        FrMyLayer* layer = this->GetLayerByID(layerId);
        if(layer){
            layer->SetCameraSettings(settings);
        }
    }
}

void FrMyLayeredImage::SetOpacity(double value, int layerId){
    if(layerId == ALL_LAYERS_ID){
        LayerCollection::iterator it, itEnd(m_layers.end());
        for(it = m_layers.begin(); it != itEnd; ++it){
            (*it)->SetOpacity(value);
        }
        FrMyLayer::SetOpacity(value);
    }
    else{
        FrMyLayer* layer = this->GetLayerByID(layerId);
        if(layer){
            layer->SetOpacity(value);
        }
    }
}

void FrMyLayeredImage::SetVisibility(bool value, int layerId){
    if(layerId == ALL_LAYERS_ID){
        LayerCollection::iterator it, itEnd(m_layers.end());
        for(it = m_layers.begin(); it != itEnd; ++it){
            (*it)->SetVisibility(value);
        }
        FrMyLayer::SetVisibility(value);
    }
    else{
        FrMyLayer* layer = this->GetLayerByID(layerId);
        if(layer){
            layer->SetVisibility(value);
        }
    }
}

// Initialization
void FrMyLayeredImage::InitDefault(FrMyLayer* layer){
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
    layer->SetID(m_nextLayerId);
    m_nextLayerId++;
}

// Update methods
void FrMyLayeredImage::UpdateColormap(){
    LayerCollection::iterator it, itEnd(m_layers.end());
    for(it = m_layers.begin(); it != itEnd; ++it){
        (*it)->UpdateColormap();
    }
    // NOTE: skip colormap update for default layer
    //FrMyLayer::UpdateColormap();
}

void FrMyLayeredImage::UpdateTBC(){    
    LayerCollection::iterator it, itEnd(m_layers.end());
    for(it = m_layers.begin(); it != itEnd; ++it){
        (*it)->UpdateTBC();
    }
    FrMyLayer::UpdateTBC();
}

void FrMyLayeredImage::UpdateCamera(){
    LayerCollection::iterator it, itEnd(m_layers.end());
    for(it = m_layers.begin(); it != itEnd; ++it){
        (*it)->UpdateCamera();
    }
    FrMyLayer::UpdateCamera();
}

// Layer management
int FrMyLayeredImage::AddLayer(){
    FrMyLayer* layer = FrMyLayer::New();

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
    ren->SetViewport(m_renderer->GetViewport());

    // Update render window if any
    vtkRenderWindow* rw = m_renderer->GetRenderWindow();
    if(rw){
        int layerNum = rw->GetNumberOfLayers();
        ren->SetLayer(layerNum);

        rw->AddRenderer(ren);
        rw->SetNumberOfLayers(rw->GetNumberOfLayers()+1);
    }

    // Add layer and return its ID
    m_layers.push_back(layer);
    return layer->GetID();
}

bool FrMyLayeredImage::RemoveLayer(int layerId){
    if(layerId == DEFAULT_LAYER_ID) return false;

    bool result = false;
    LayerCollection::iterator it, itEnd(m_layers.end());
    for(it = m_layers.begin(); it != itEnd; ++it){
        if((*it)->GetID() == layerId)break;
    }
    
    // delete layer if it were found
    if(it != itEnd){
        FrMyLayer* layer = (*it);
        m_layers.erase(it);

        // Remove renderer from render window if any
        vtkRenderWindow* rw = layer->GetRenderer()->GetRenderWindow();
        if(rw != 0){
            rw->RemoveRenderer(layer->GetRenderer());
            rw->GetRenderers()->InitTraversal();

            // Update layer numbers
            int layerID = START_LAYER_ID;
            LayerCollection::iterator it, itEnd(m_layers.end());
            for(it = m_layers.begin(); it != itEnd; ++it){
                (*it)->GetRenderer()->SetLayer(layerID);
                ++layerID;
            }
        }

        layer->Delete();
        result = true;
    }
    return result;
}

FrMyLayer* FrMyLayeredImage::GetLayerByID(int id){
    if(id == DEFAULT_LAYER_ID){
        return (FrMyLayer*)this;
    }
    else{
        LayerCollection::iterator it, itEnd(m_layers.end());
        for(it = m_layers.begin(); it != itEnd; ++it){
            if((*it)->GetID() == id) return (*it);
        }
    }
    return 0L;
}

void FrMyLayeredImage::GetRenderers(std::vector<vtkRenderer*>& renderers){
    // First add default
    renderers.clear();
    renderers.push_back(m_renderer);
    m_renderer->SetLayer(0);

    // Add renderer of each layer and udate layer number
    int layer = 1;
    LayerCollection::iterator it, itEnd(m_layers.end());
    for(it = m_layers.begin(); it != itEnd; ++it){
        renderers.push_back((*it)->GetRenderer());
        (*it)->GetRenderer()->SetLayer(layer);
        layer++;
    }
}