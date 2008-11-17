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
: FrImageLayer(), m_nextLayerID(DEFAULT_LAYER_ID) {

    this->InitImageLayerDefault(this);

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
    this->RemoveRoiLayers();
    // and other stuff too
    if(m_SpecialLayer) m_SpecialLayer->Delete();
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

void FrLayeredImage::SetRoiInput(int id, vtkImageData *data){
    FrROILayer* layer = this->GetRoiLayerByID(id);
    if(layer){
        layer->SetInput(data);
    }
}

vtkImageData* FrLayeredImage::GetRoiInput(int id){
    vtkImageData* result = 0L;
    FrROILayer* layer = this->GetRoiLayerByID(id);
    if(layer){
        result = layer->GetInput();
    }
    return result;
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
        FrImageLayer* layer = this->GetImageLayerByID(layerId);
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
            (*it)->GetTBCSettings(settings);
        }
        FrImageLayer::SetTBCSettings(settings);
        // Values might be clamped so update them
        FrImageLayer::GetTBCSettings(settings);
    }
    else{
        FrImageLayer* layer = this->GetImageLayerByID(layerId);
        if(layer){
            layer->SetTBCSettings(settings);
            // Values might be clamped so update them
            layer->GetTBCSettings(settings);
        }
    }
}

void FrLayeredImage::SetCameraSettings(FrCameraSettings& settings, int layerId){
    // Setup camera for image layers
    if(layerId == ALL_LAYERS_ID){
        LayerCollection::iterator it, itEnd(m_ImageLayers.end());
        for(it = m_ImageLayers.begin(); it != itEnd; ++it){
            (*it)->SetCameraSettings(settings);
        }
        FrImageLayer::SetCameraSettings(settings);
    }
    else{
        FrImageLayer* layer = this->GetImageLayerByID(layerId);
        if(layer){
            layer->SetCameraSettings(settings);
        }
    }

    // Update camera for all ROI layers
    ROILayerCollection::iterator itr, itrEnd(m_ROILayers.end());
    for(itr = m_ROILayers.begin(); itr != itrEnd; ++itr){
        (*itr)->SetCameraSettings(settings);
    }

    // Update camera for special layer
    m_SpecialLayer->SetCameraSettings(settings);
}

void FrLayeredImage::SetOpacity(double value, int layerId){
    if(layerId == ALL_LAYERS_ID){
        // Image layers
        LayerCollection::iterator it, itEnd(m_ImageLayers.end());
        for(it = m_ImageLayers.begin(); it != itEnd; ++it){
            (*it)->SetOpacity(value);
        }
        FrImageLayer::SetOpacity(value);

        // Update opacity for all ROI layers
        ROILayerCollection::iterator itr, itrEnd(m_ROILayers.end());
        for(itr = m_ROILayers.begin(); itr != itrEnd; ++itr){
            (*itr)->SetOpacity(value);
        }
    }
    else{
        FrImageLayer* layer = this->GetImageLayerByID(layerId);
        if(layer){
            layer->SetOpacity(value);
        }
        else {
            FrROILayer* rLayer = this->GetRoiLayerByID(layerId);
            if(rLayer){
                 rLayer->SetOpacity(value);
            }
        }
    }
}

void FrLayeredImage::SetVisibility(bool value, int layerId){
    if(layerId == ALL_LAYERS_ID){
        // Image layers
        LayerCollection::iterator it, itEnd(m_ImageLayers.end());
        for(it = m_ImageLayers.begin(); it != itEnd; ++it){
            (*it)->SetVisibility(value);
        }
        FrImageLayer::SetVisibility(value);

        // Update visibility for all ROI layers
        ROILayerCollection::iterator itr, itrEnd(m_ROILayers.end());
        for(itr = m_ROILayers.begin(); itr != itrEnd; ++itr){
            (*itr)->SetVisibility(value);
        }
    }
    else{
        FrImageLayer* layer = this->GetImageLayerByID(layerId);
        if(layer){
            layer->SetVisibility(value);
        }
        else {
            FrROILayer* rLayer = this->GetRoiLayerByID(layerId);
            if(rLayer){
                 rLayer->SetVisibility(value);
            }
        }
    }
}

// Initialization
void FrLayeredImage::InitImageLayerDefault(FrImageLayer* layer){
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
    layer->SetID(m_nextLayerID);
    ++m_nextLayerID;
}

void FrLayeredImage::InitRoiLayerDefault(FrROILayer* layer){
    FrCameraSettings cs;
    InitCameraDefault(&cs);
    layer->SetCameraSettings(cs);

    // other
    layer->SetOpacity(DEF_LAYER_OPACITY);
    layer->SetVisibility(DEF_LAYER_VISIBILITY);

    // init ID
    layer->SetID(m_nextLayerID);
    ++m_nextLayerID;
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

    // Update ROI data
    ROILayerCollection::iterator itr, itrEnd(m_ROILayers.end());
    for(itr = m_ROILayers.begin(); itr != itrEnd; ++itr){
        (*itr)->UpdateData();
    }
}

void FrLayeredImage::UpdateCamera(){
    // Images
    LayerCollection::iterator it, itEnd(m_ImageLayers.end());
    for(it = m_ImageLayers.begin(); it != itEnd; ++it){
        (*it)->UpdateCamera();
    }
    FrImageLayer::UpdateCamera();

    // ROI
    ROILayerCollection::iterator itr, itrEnd(m_ROILayers.end());
    for(itr = m_ROILayers.begin(); itr != itrEnd; ++itr){
        (*itr)->UpdateCamera();
    }
}

// Layer management
int FrLayeredImage::AddImageLayer(){
    FrImageLayer* layer = FrImageLayer::New();

    // Now init all ...    
    this->InitImageLayerDefault(layer);
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

bool FrLayeredImage::RemoveImageLayer(int layerId){
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

int FrLayeredImage::AddRoiLayer(){
    FrROILayer* layer = FrROILayer::New();

    // Now init all ...    
    this->InitRoiLayerDefault(layer);

    // ... and synchronize camera ...
    FrCameraSettings cs;
    this->GetCameraSettings(cs);
    layer->SetCameraSettings(cs);

    // ... and render window with viewport
    // viewport's important for ortho view
    vtkRenderer* ren = layer->GetRenderer();
    ren->SetViewport(m_Renderer->GetViewport());
    m_ROILayers.push_back(layer);

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

bool FrLayeredImage::RemoveRoiLayer(int layerId){
    bool result = false;
    ROILayerCollection::iterator it, itEnd(m_ROILayers.end());
    for(it = m_ROILayers.begin(); it != itEnd; ++it){
        if((*it)->GetID() == layerId) break;
    }

    // delete layer if it were found
    if(it != itEnd){
        FrROILayer* layer = (*it);
        m_ROILayers.erase(it);

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
    m_ImageLayers.clear();
}

void FrLayeredImage::RemoveRoiLayers(){
    vtkRenderWindow* rw = m_Renderer->GetRenderWindow();

    // Delete all layers
    ROILayerCollection::iterator it, itEnd(m_ROILayers.end());
    for(it = m_ROILayers.begin(); it != itEnd; ++it){
        FrROILayer* layer = (*it);
        if(rw){
            vtkRenderer* ren = layer->GetRenderer();
            rw->RemoveRenderer(ren);
            ren->SetRenderWindow(0L);
        }
        layer->Delete();
    }
    m_ROILayers.clear();
}


void FrLayeredImage::RemoveAllLayers(){
    RemoveImageLayers();
    RemoveRoiLayers();

    vtkRenderWindow* rw = m_Renderer->GetRenderWindow();
    if(rw){
        // NOTE: we have 2 layers : default and special
        m_SpecialLayer->GetRenderer()->SetLayer(1);
        rw->SetNumberOfLayers(2);
    }
    m_nextLayerID = START_LAYER_ID;
}

FrImageLayer* FrLayeredImage::GetImageLayerByID(int id){
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

FrROILayer* FrLayeredImage::GetRoiLayerByID(int id){
    ROILayerCollection::iterator it, itEnd(m_ROILayers.end());
    for(it = m_ROILayers.begin(); it != itEnd; ++it){
        if((*it)->GetID() == id) return (*it);
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

    // Add renderer of each roi layer and udate layer number
    ROILayerCollection::iterator itr, itrEnd(m_ROILayers.end());
    for(itr = m_ROILayers.begin(); itr != itrEnd; ++itr){
        vtkRenderer* ren = (*itr)->GetRenderer();
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
