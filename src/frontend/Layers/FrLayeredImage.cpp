#include "FrLayeredImage.h"
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


FrLayeredImage* FrLayeredImage::New(){  
  vtkObject* result = vtkObjectFactory::CreateInstance("FrLayeredImage");
  if(result){
      return (FrLayeredImage*)result;
  }
  return new FrLayeredImage();
}


FrLayeredImage::FrLayeredImage() {
    m_nextLayerId = DEFAULT_LAYER_ID;
    this->InitDefault(this);
       
    // NOTE: remove colormap filter from pipline
    if(m_cmFilter) {
        m_cmFilter->Delete();
        m_cmFilter = 0L;
    }

    // Init text stuff
    m_TextMapper = vtkTextMapper::New();
    m_TextMapper->SetInput("");

    vtkTextProperty* pProperty = m_TextMapper->GetTextProperty();
	pProperty->SetBold(1);
    pProperty->SetItalic(0);
    pProperty->SetFontSize(20);
    pProperty->SetFontFamily(VTK_ARIAL);
    pProperty->SetColor(100.0, 100.0, 0.0);
    pProperty->SetJustification(VTK_TEXT_LEFT);
    pProperty->SetVerticalJustification(VTK_TEXT_CENTERED);
    
    m_TextActor = vtkActor2D::New();
    m_TextActor->SetMapper(m_TextMapper);
    m_TextActor->SetPosition(20, 20);
    m_TextActor->PickableOff();
    m_renderer->AddActor2D(m_TextActor);
}

FrLayeredImage::~FrLayeredImage(){
    // delete all layers here
    for(int id = 0; id < m_nextLayerId; ++id){
        this->RemoveLayer(id);
    }

    // Delete text stuff
    if(m_TextMapper) m_TextMapper->Delete();
    if(m_TextActor) m_TextActor->Delete();
}

// Modifiers
void FrLayeredImage::SetInput(vtkImageData* data){
    LayerCollection::iterator it, itEnd(m_layers.end());
    for(it = m_layers.begin(); it != itEnd; ++it){
        (*it)->SetInput(data);
    }    
    // NOTE: Do not use colormap for default layer and 
    // set input to TBC filter immediatly
    m_tbcFilter->SetInput(data);
}

void FrLayeredImage::SetColormapSettings(FrColormapSettings& settings, int layerId){
    if(layerId == ALL_LAYERS_ID){
        LayerCollection::iterator it, itEnd(m_layers.end());
        for(it = m_layers.begin(); it != itEnd; ++it){
            (*it)->SetColormapSettings(settings);
        }
        FrLayer::SetColormapSettings(settings);
    }
    else{
        FrLayer* layer = this->GetLayerByID(layerId);
        if(layer){
            layer->SetColormapSettings(settings);
        }
    }
}

void FrLayeredImage::SetTBCSettings(FrTBCSettings& settings, int layerId){
    if(layerId == ALL_LAYERS_ID){
        LayerCollection::iterator it, itEnd(m_layers.end());
        for(it = m_layers.begin(); it != itEnd; ++it){
            (*it)->SetTBCSettings(settings);
        }
        FrLayer::SetTBCSettings(settings);
    }
    else{
        FrLayer* layer = this->GetLayerByID(layerId);
        if(layer){
            layer->SetTBCSettings(settings);
        }
    }
}

void FrLayeredImage::SetCameraSettings(FrCameraSettings& settings, int layerId){
    if(layerId == ALL_LAYERS_ID){
        LayerCollection::iterator it, itEnd(m_layers.end());
        for(it = m_layers.begin(); it != itEnd; ++it){
            (*it)->SetCameraSettings(settings);
        }
        FrLayer::SetCameraSettings(settings);
    }
    else{
        FrLayer* layer = this->GetLayerByID(layerId);
        if(layer){
            layer->SetCameraSettings(settings);
        }
    }
}

void FrLayeredImage::SetOpacity(double value, int layerId){
    if(layerId == ALL_LAYERS_ID){
        LayerCollection::iterator it, itEnd(m_layers.end());
        for(it = m_layers.begin(); it != itEnd; ++it){
            (*it)->SetOpacity(value);
        }
        FrLayer::SetOpacity(value);
    }
    else{
        FrLayer* layer = this->GetLayerByID(layerId);
        if(layer){
            layer->SetOpacity(value);
        }
    }
}

void FrLayeredImage::SetVisibility(bool value, int layerId){
    if(layerId == ALL_LAYERS_ID){
        LayerCollection::iterator it, itEnd(m_layers.end());
        for(it = m_layers.begin(); it != itEnd; ++it){
            (*it)->SetVisibility(value);
        }
        FrLayer::SetVisibility(value);
    }
    else{
        FrLayer* layer = this->GetLayerByID(layerId);
        if(layer){
            layer->SetVisibility(value);
        }
    }
}

// Initialization
void FrLayeredImage::InitDefault(FrLayer* layer){
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
void FrLayeredImage::UpdateColormap(){
    LayerCollection::iterator it, itEnd(m_layers.end());
    for(it = m_layers.begin(); it != itEnd; ++it){
        (*it)->UpdateColormap();
    }
    // NOTE: skip colormap update for default layer
    //FrLayer::UpdateColormap();
}

void FrLayeredImage::UpdateTBC(){    
    LayerCollection::iterator it, itEnd(m_layers.end());
    for(it = m_layers.begin(); it != itEnd; ++it){
        (*it)->UpdateTBC();
    }
    FrLayer::UpdateTBC();
}

void FrLayeredImage::UpdateCamera(){
    LayerCollection::iterator it, itEnd(m_layers.end());
    for(it = m_layers.begin(); it != itEnd; ++it){
        (*it)->UpdateCamera();
    }
    FrLayer::UpdateCamera();
}

// Layer management
int FrLayeredImage::AddLayer(){
    FrLayer* layer = FrLayer::New();

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

bool FrLayeredImage::RemoveLayer(int layerId){
    if(layerId == DEFAULT_LAYER_ID) return false;

    bool result = false;
    LayerCollection::iterator it, itEnd(m_layers.end());
    for(it = m_layers.begin(); it != itEnd; ++it){
        if((*it)->GetID() == layerId) break;
    }
    
    // delete layer if it were found
    if(it != itEnd){
        FrLayer* layer = (*it);
        m_layers.erase(it);

        // Remove renderer from render window if any
        vtkRenderWindow* rw = layer->GetRenderer()->GetRenderWindow();
        if(rw != 0){
            rw->RemoveRenderer(layer->GetRenderer());
            rw->GetRenderers()->InitTraversal();
            
            rw->SetNumberOfLayers(m_layers.size()+1);
        }
        
        // Update layer IDs and number of layers
        m_nextLayerId = START_LAYER_ID;
        LayerCollection::iterator it, itEnd(m_layers.end());
        for(it = m_layers.begin(); it != itEnd; ++it){
            (*it)->SetID(m_nextLayerId);
            (*it)->GetRenderer()->SetLayer(m_nextLayerId);
            m_nextLayerId++;
        }

        // Delete layer object
        layer->Delete();
        result = true;
    }
    return result;
}

void FrLayeredImage::RemoveLayers(){
    vtkRenderWindow* rw = m_renderer->GetRenderWindow();

    // Delete all layers
    LayerCollection::iterator it, itEnd(m_layers.end());
    for(it = m_layers.begin(); it != itEnd; ++it){
        FrLayer* layer = (*it);
        if(rw) rw->RemoveRenderer(layer->GetRenderer());
        layer->Delete();
    }
    if(rw) rw->SetNumberOfLayers(1);

    m_layers.clear();
    m_nextLayerId = START_LAYER_ID;
}

FrLayer* FrLayeredImage::GetLayerByID(int id){
    if(id == DEFAULT_LAYER_ID){
        return (FrLayer*)this;
    }
    else{
        LayerCollection::iterator it, itEnd(m_layers.end());
        for(it = m_layers.begin(); it != itEnd; ++it){
            if((*it)->GetID() == id) return (*it);
        }
    }
    return 0L;
}

void FrLayeredImage::GetRenderers(std::vector<vtkRenderer*>& renderers){
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

// Text management
void FrLayeredImage::SetText(const char* text){
    m_TextMapper->SetInput(text);
}