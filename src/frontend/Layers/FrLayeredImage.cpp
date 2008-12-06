#include "FrLayeredImage.h"
#include "FrRoiLayer.h"
#include "FrImageLayer.h"
#include "FrColormapLayer.h"
#include "FrSpecialLayer.h"
#include "FrLayerSettings.h"

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
#include "vtkImageActor.h"

#define START_LAYER_ID 1


vtkStandardNewMacro(FrLayeredImage);


FrLayeredImage::FrLayeredImage() {
    m_ImageLayer = FrImageLayer::New();
    m_ImageLayer->SetID(DEF_LAYER_ID);

    m_SpecialLayer = FrSpecialLayer::New();
    m_SpecialLayer->SetID(SPC_LAYER_ID);
}

FrLayeredImage::~FrLayeredImage(){
    // delete all layers here
    this->RemoveLayers();
    if(m_ImageLayer){
        m_ImageLayer->Delete();
    }
    if(m_SpecialLayer) {
        m_SpecialLayer->Delete();
    }
}

// Accessors/Modifiers
void FrLayeredImage::SetImageInput(vtkImageData* data){
    if(m_ImageLayer){
        m_ImageLayer->SetInput(data);
    }

    // Setup all colormap images
    LayersCollection::iterator it, itEnd(m_Layers.end());
    for(it = m_Layers.begin(); it != itEnd; ++it){
        if((*it)->GetType() == FrBaseLayer::LtColormap){
            (*it)->SetInput(data);
        }
    }
}

vtkImageData* FrLayeredImage::GetImageInput(){
    vtkImageData* result = 0;
    if(m_ImageLayer){
        result = m_ImageLayer->GetInput();
    }
    return result;
}

void FrLayeredImage::SetRoiInput(vtkImageData* data, unsigned int id){
}

vtkImageData* FrLayeredImage::GetRoiInput(unsigned int id){
    vtkImageData* result = 0;

    LayersCollection::iterator it, itEnd(m_Layers.end());
    for(it = m_Layers.begin(); it != itEnd; ++it){
        if((*it)->GetType() == FrBaseLayer::LtRoi &&
            (*it)->GetID() == id){
            result = (*it)->GetInput();
            break;
        }
    }
    return result;
}

void FrLayeredImage::SetColormapSettings(FrColormapSettings& settings, unsigned int id){
    if(id == ALL_LAYER_ID){
        LayersCollection::iterator it, itEnd(m_Layers.end());
        for(it = m_Layers.begin(); it != itEnd; ++it){
            if((*it)->GetType() == FrBaseLayer::LtColormap){
                ((FrColormapLayer*)(*it))->SetColormapSettings(settings);
            }
        }
    }
    else{
        FrBaseLayer* layer = this->GetLayerByID(id);
        if(layer->GetType() == FrBaseLayer::LtColormap){
            ((FrColormapLayer*)layer)->SetColormapSettings(settings);
        }
    }
}

void FrLayeredImage::SetTbcSettings(FrTbcSettings& settings, unsigned int id){
    if(id == ALL_LAYER_ID){
        LayersCollection::iterator it, itEnd(m_Layers.end());
        for(it = m_Layers.begin(); it != itEnd; ++it){
            switch((*it)->GetType()){
            case FrBaseLayer::LtColormap:
                ((FrColormapLayer*)(*it))->SetTbcSettings(settings);
                break;
            case FrBaseLayer::LtImage:
                ((FrImageLayer*)(*it))->SetTbcSettings(settings);
                break;
            }
        }
        m_ImageLayer->SetTbcSettings(settings);
    }
    else{
        FrBaseLayer* layer = this->GetLayerByID(id);
        switch(layer->GetType()){
            case FrBaseLayer::LtColormap:
                ((FrColormapLayer*)layer)->SetTbcSettings(settings);
                break;
            case FrBaseLayer::LtImage:
                ((FrImageLayer*)layer)->SetTbcSettings(settings);
                break;
        }
    }
}

void FrLayeredImage::SetCameraSettings(FrCameraSettings& settings, unsigned int id){
    // Setup camera for image layers
    if(id == ALL_LAYER_ID){
        LayersCollection::iterator it, itEnd(m_Layers.end());
        for(it = m_Layers.begin(); it != itEnd; ++it){
            (*it)->SetCameraSettings(settings);
        }
        m_ImageLayer->SetCameraSettings(settings);
    }
    else{
        FrBaseLayer* layer = this->GetLayerByID(id);
        if(layer){
            layer->SetCameraSettings(settings);
        }
    }
    // Update camera for special layer
    m_SpecialLayer->SetCameraSettings(settings);
}

void FrLayeredImage::SetOpacity(double value, unsigned int id){
    if(id == ALL_LAYER_ID){
        // Image layers
        LayersCollection::iterator it, itEnd(m_Layers.end());
        for(it = m_Layers.begin(); it != itEnd; ++it){
            (*it)->SetOpacity(value);
        }
        m_ImageLayer->SetOpacity(value);
    }
    else{
        FrBaseLayer* layer = this->GetLayerByID(id);
        if(layer){
            layer->SetOpacity(value);
        }
    }
}

void FrLayeredImage::SetVisibility(bool value, unsigned int id){
    if(id == ALL_LAYER_ID){
        // Image layers
        LayersCollection::iterator it, itEnd(m_Layers.end());
        for(it = m_Layers.begin(); it != itEnd; ++it){
            (*it)->SetVisibility(value);
        }
        m_ImageLayer->SetVisibility(value);
    }
    else{
        FrBaseLayer* layer = this->GetLayerByID(id);
        if(layer){
            layer->SetOpacity(value);
        }
    }
}

// Update methods
void FrLayeredImage::UpdateColormap(){
    LayersCollection::iterator it, itEnd(m_Layers.end());
    for(it = m_Layers.begin(); it != itEnd; ++it){
        if((*it)->GetType() == FrBaseLayer::LtColormap){
            ((FrColormapLayer*)(*it))->UpdateColormap();
        }
    }
}

void FrLayeredImage::UpdateTbc(){    
    LayersCollection::iterator it, itEnd(m_Layers.end());
    for(it = m_Layers.begin(); it != itEnd; ++it){
        switch((*it)->GetType()){
            case FrBaseLayer::LtRoi:
                ((FrRoiLayer*)(*it))->UpdateData();
                break;
            case FrBaseLayer::LtImage:
                ((FrImageLayer*)(*it))->UpdateTbc();
                break;
            case FrBaseLayer::LtColormap:
                ((FrColormapLayer*)(*it))->UpdateTbc();
                break;
        }
    }
    m_ImageLayer->UpdateTbc();
}

void FrLayeredImage::UpdateCamera(){
    // Images
    LayersCollection::iterator it, itEnd(m_Layers.end());
    for(it = m_Layers.begin(); it != itEnd; ++it){
        (*it)->UpdateCamera();
    }
    m_ImageLayer->UpdateCamera();
}

// Layer management
bool FrLayeredImage::AddLayer(unsigned int id, LayerType type){    
    // first create layer
    FrBaseLayer* layer = 0;
    switch(type){
        case FrBaseLayer::LtRoi:
            layer = FrRoiLayer::New();
            break;
        case FrBaseLayer::LtImage:
            // HACK: just assign ID and return
            m_ImageLayer->SetID(id);
            return true;
            break;
        case FrBaseLayer::LtColormap:
            layer = FrColormapLayer::New();
            break;
    }

    // If created then init params.
    if(layer){
        // Since special layer is always on so use it
        layer->SetID(id);
        
        FrCameraSettings cs;
        m_SpecialLayer->GetCameraSettings(cs);
        layer->SetCameraSettings(cs);
        
        vtkRenderer* ren = m_SpecialLayer->GetRenderer();
        layer->GetRenderer()->SetViewport(ren->GetViewport());

        // Add layer to internal collection
        m_Layers.push_back(layer);

        // Update render window if any
        vtkRenderWindow* rw = ren->GetRenderWindow();
        if(rw){
            // HACK: GetRenderers() reorder all layers
            // by setting up proper 'LayerNumber' !!!
            std::vector<vtkRenderer*> rens;
            this->GetRenderers(rens);

            // Add renderer to render window
            rw->AddRenderer(layer->GetRenderer());
            rw->SetNumberOfLayers(int(rens.size()));
        }
    }
    return (layer != 0);
}

bool FrLayeredImage::RemoveLayer(unsigned int id){
    // do not remove image layer!!!
    if(m_ImageLayer->GetID() == id) return false;
    
    // Find layer to remove
    LayersCollection::iterator it, itEnd(m_Layers.end());
    for(it = m_Layers.begin(); it != itEnd; ++it){
        if((*it)->GetID() == id) break;
    }

    bool result = false;
    if(it != itEnd){
        FrBaseLayer* layer = (*it);
        m_Layers.erase(it);

        // Remove renderer from render window if any
        vtkRenderWindow* rw = layer->GetRenderer()->GetRenderWindow();
        if(rw != 0){
            rw->RemoveRenderer(layer->GetRenderer());
            rw->GetRenderers()->InitTraversal();

            // HACK: GetRenderers() reorder all layers
            // by setting up proper 'LayerNumber' !!!
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

void FrLayeredImage::RemoveLayers(){
    
    while(m_Layers.size() > 0){
        FrBaseLayer* layer = (FrBaseLayer*)(*(m_Layers.begin()));
        layer->Delete();
    }

    vtkRenderWindow* rw = m_SpecialLayer->
        GetRenderer()->GetRenderWindow();

    if(rw){
        // NOTE: we have 2 layers : default and special
        m_ImageLayer->GetRenderer()->SetLayer(1);
        m_SpecialLayer->GetRenderer()->SetLayer(1);
        rw->SetNumberOfLayers(2);
    }
}

FrBaseLayer* FrLayeredImage::GetLayerByID(unsigned int id){
    LayersCollection::iterator it, itEnd(m_Layers.end());
    for(it = m_Layers.begin(); it != itEnd; ++it){
        if((*it)->GetID() == id) return (*it);
    }
    return 0L;
}

void FrLayeredImage::GetRenderers(std::vector<vtkRenderer*>& renderers){
    // First add default
    renderers.clear();
    int layerNumber = 0;

    vtkRenderer* ren = m_ImageLayer->GetRenderer();
    renderers.push_back(ren);
    ren->SetLayer(layerNumber);
    ++layerNumber;

    // Add renderer of each layer and udate layer number
    LayersCollection::iterator it, itEnd(m_Layers.end());
    for(it = m_Layers.begin(); it != itEnd; ++it){
        ren = (*it)->GetRenderer();
        ren->SetLayer(layerNumber);
        renderers.push_back(ren);

        ++layerNumber;
    }

    // Last add renderer from special layer
    ren = m_SpecialLayer->GetRenderer();
    ren->SetLayer(layerNumber);
    renderers.push_back(ren);
}

// Text and border management
void FrLayeredImage::SetText(const char* text){
    m_SpecialLayer->SetText(text);
}

void FrLayeredImage::UpdateBorder(){
    vtkRenderer* ren = m_SpecialLayer->GetRenderer();
    vtkRenderWindow* rw = ren->GetRenderWindow();
    if(rw){
        int* size = ren->GetSize();
        m_SpecialLayer->UpdateBorder(size[0]-1, size[1]);
    }
}

bool FrLayeredImage::IsInViewport(int mouseX, int mouseY){
    // Use SpecialLayer since it is alway active
    bool result = false;
    if(m_SpecialLayer){
        result = m_SpecialLayer->
            GetRenderer()->IsInViewport(mouseX, mouseY);
    }
    return result;
}

double* FrLayeredImage::GetActorBounds(){
    double *bounds;

    bounds = m_ImageLayer->GetActor()->GetBounds();
    
    return bounds;
}