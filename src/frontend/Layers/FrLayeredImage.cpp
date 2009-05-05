#include "FrLayeredImage.h"
#include "FrROILayer.h"
#include "FrImageLayer.h"
//#include "FrColormapLayer.h"
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

#include <algorithm>

#define START_LAYER_ID 1


vtkStandardNewMacro(FrLayeredImage);


FrLayeredImage::FrLayeredImage() {
//    m_ImageLayer = FrImageLayer::New();
//    m_ImageLayer->SetID(DEF_LAYER_ID);

    // Add layer to internal collection
//    m_Layers.push_back(m_ImageLayer);

    m_SpecialLayer = FrSpecialLayer::New();
    m_SpecialLayer->SetID(SPC_LAYER_ID);
}

FrLayeredImage::~FrLayeredImage(){
    // delete all layers here
    this->RemoveLayers();
//    if(m_ImageLayer){
//        m_ImageLayer->Delete();
//    }
    if(m_SpecialLayer) {
        m_SpecialLayer->Delete();
    }
}

// Accessors/Modifiers
void FrLayeredImage::SetImageInput(vtkImageData* data, unsigned long id){
    // Setup all colormap images
    LayersCollection::iterator it, itEnd(m_Layers.end());
    for(it = m_Layers.begin(); it != itEnd; ++it){
        if((*it)->GetType() == FrBaseLayer::LtImage && (*it)->GetID() == id){
            (*it)->SetInput(data);
        }
    }
}

vtkImageData* FrLayeredImage::GetImageInput(unsigned long id){
    vtkImageData* result = 0;
    LayersCollection::iterator it, itEnd(m_Layers.end());
    for(it = m_Layers.begin(); it != itEnd; ++it){
      //cout << "FrLayeredImage::GetImageInput " << (*it)->GetID() << " " << id << endl;
        if((*it)->GetType() == FrBaseLayer::LtImage &&
	   (id == std::numeric_limits<unsigned long>::max() || (*it)->GetID() == id )){
            result = (*it)->GetInput();
            break;
        }
    }
    return result;
}

void FrLayeredImage::SetRoiInput(vtkImageData* data, unsigned long id){
    // find appropriate roi layer and set input to it
    LayersCollection::iterator it, itEnd(m_Layers.end());
    for(it = m_Layers.begin(); it != itEnd; ++it){
        if((*it)->GetType() == FrBaseLayer::LtRoi &&
            (*it)->GetID() == id){
            (*it)->SetInput(data);
            break;
        }
    }
}

vtkImageData* FrLayeredImage::GetRoiInput(unsigned long id){
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

void FrLayeredImage::SetColormapSettings(FrColormapSettings& settings, unsigned long id){
    if(id == ALL_LAYER_ID){
        LayersCollection::iterator it, itEnd(m_Layers.end());
        for(it = m_Layers.begin(); it != itEnd; ++it){
            if((*it)->GetType() == FrBaseLayer::LtImage){
                ((FrImageLayer*)(*it))->SetColormapSettings(settings);
            }
        }
    }
    else{
        FrBaseLayer* layer = this->GetLayerByID(id);
        if(layer->GetType() == FrBaseLayer::LtImage){
            ((FrImageLayer*)layer)->SetColormapSettings(settings);
        }
    }
}

void FrLayeredImage::SetTbcSettings(FrTbcSettings& settings, unsigned long id){
    if(id == ALL_LAYER_ID){
        LayersCollection::iterator it, itEnd(m_Layers.end());
        for(it = m_Layers.begin(); it != itEnd; ++it){
            switch((*it)->GetType()){
            case FrBaseLayer::LtImage:
                ((FrImageLayer*)(*it))->SetTbcSettings(settings);
                break;
            }
        }
    }
    else{
        FrBaseLayer* layer = this->GetLayerByID(id);
        if(layer){
            switch(layer->GetType()){
                case FrBaseLayer::LtImage:
                    ((FrImageLayer*)layer)->SetTbcSettings(settings);
                    break;
            }
        }
    }
}

void FrLayeredImage::SetCameraSettings(FrCameraSettings& settings, unsigned long id){
    // Setup camera for image layers
    if(id == ALL_LAYER_ID){
        LayersCollection::iterator it, itEnd(m_Layers.end());
        for(it = m_Layers.begin(); it != itEnd; ++it){
            (*it)->SetCameraSettings(settings);
        }
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

void FrLayeredImage::SetOpacity(double value, unsigned long id){
    if(id == ALL_LAYER_ID){
        // Image layers
        LayersCollection::iterator it, itEnd(m_Layers.end());
        for(it = m_Layers.begin(); it != itEnd; ++it){
            (*it)->SetOpacity(value);
        }
    }
    else{
        FrBaseLayer* layer = this->GetLayerByID(id);
        if(layer){
            layer->SetOpacity(value);
        }
    }
}

void FrLayeredImage::SetVisibility(bool value, unsigned long id){
    if(id == ALL_LAYER_ID){
        // Image layers
        LayersCollection::iterator it, itEnd(m_Layers.end());
        for(it = m_Layers.begin(); it != itEnd; ++it){
            (*it)->SetVisibility(value);
        }
    }
    else{
        FrBaseLayer* layer = this->GetLayerByID(id);
        if(layer){
            layer->SetVisibility(value);
        }
    }
}

// Update methods
void FrLayeredImage::UpdateColormap(){
    LayersCollection::iterator it, itEnd(m_Layers.end());
    for(it = m_Layers.begin(); it != itEnd; ++it){
        if((*it)->GetType() == FrBaseLayer::LtImage){
            ((FrImageLayer*)(*it))->UpdateColormap();
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
        }
    }
}

void FrLayeredImage::UpdateCamera(){
    // Images
    LayersCollection::iterator it, itEnd(m_Layers.end());
    for(it = m_Layers.begin(); it != itEnd; ++it){
        (*it)->UpdateCamera();
    }
}

// Layer management
bool FrLayeredImage::AddLayer(unsigned long id, LayerType type){    
  
    // first create layer
    FrBaseLayer* layer = 0;
    switch(type){
        case FrLayeredImage::Roi:
            layer = FrRoiLayer::New();
            break;
        case FrLayeredImage::Image:
	  // ohinds: 2009-02-23
	  // i dont understand this
//            // HACK: just assign ID and return
//            m_ImageLayer->SetID(id);
//            return true;
//            break;
//        case FrLayeredImage::Colormap:
	  layer = FrImageLayer::New();
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

	    // ohinds 2009-04-11 
	    //
	    // NOTE: here we get hangs and crashes due to thread
	    // synchronization issues when the data comes from the
	    // backend. 
            rw->AddRenderer(layer->GetRenderer());
            rw->SetNumberOfLayers(int(rens.size()));
            this->GetRenderers(rens);
        }
    }
    return (layer != 0);
}

bool FrLayeredImage::RemoveLayer(unsigned long id){
    // do not remove image layer!!!
    //if(m_ImageLayer->GetID() == id) return false;
    
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
        m_Layers.erase(m_Layers.begin());
        layer->Delete();
    }

    vtkRenderWindow* rw = m_SpecialLayer->
        GetRenderer()->GetRenderWindow();

    if(rw){
        // NOTE: we have 2 layers : default and special
        //m_ImageLayer->GetRenderer()->SetLayer(1);
        m_SpecialLayer->GetRenderer()->SetLayer(1);
        rw->SetNumberOfLayers(1); // i think ??
    }
}

bool FrLayeredImage::ChangeLayerPosition(unsigned long id, int inc){
    // Find layer
    LayersCollection::iterator it, itEnd(m_Layers.end());
    for(it = m_Layers.begin(); it != itEnd; ++it){
        if((*it)->GetID() == id) break;
    }
    
    bool result = false;
    if(it != itEnd){
        FrBaseLayer* layer = (*it);

        // check if we can perform this operation
        int oldPos = layer->GetRenderer()->GetLayer();
        int newPos = oldPos + inc;
        int nol = m_Layers.size();          // total number of layers

        if (newPos < 0 || newPos > nol)
            return false;

        // change layer order
        // find layer with newPos
        for(it = m_Layers.begin(); it != itEnd; ++it){
            if((*it)->GetRenderer()->GetLayer() == newPos) break;
        }

        if(it != itEnd){
            FrBaseLayer* layer2 = (*it);
        
            // change render order
            m_Layers[oldPos] = layer2;
            m_Layers[newPos] = layer;
            //layer->GetRenderer()->SetLayer(newPos);
            //layer2->GetRenderer()->SetLayer(oldPos);
            
            // HACK: GetRenderers() reorder all layers
            // by setting up proper 'LayerNumber' !!!
            std::vector<vtkRenderer*> rens;
            this->GetRenderers(rens);
        }

        result = true;
    }
    return result;
}


FrBaseLayer* FrLayeredImage::GetLayerByID(unsigned long id){
    LayersCollection::iterator it, itEnd(m_Layers.end());
//    if (m_ImageLayer->GetID() == id)        // should we include
//        m_ImageLayer to the layer collection?? ohinds: yes!
//        return m_ImageLayer;
//
    for(it = m_Layers.begin(); it != itEnd; ++it){
        if((*it)->GetID() == id) return (*it);
    }
    return 0L;
}

// the next layer to be focused after the one passed
unsigned long FrLayeredImage::GetNextLayerID(unsigned long id) {
  unsigned long nextID = BAD_LAYER_ID;

  LayersCollection::iterator it, itEnd(m_Layers.end());
  bool found = false;
  for(it = m_Layers.begin(); it != itEnd; ++it){
    if((*it)->GetID() == id) {
      found = true;
    }
    else {
      nextID = (*it)->GetID();
      if(found) {
	return nextID;
      }
    }
  }
  return nextID;
}


void FrLayeredImage::GetRenderers(std::vector<vtkRenderer*>& renderers){
    // Add renderer of each layer and udate layer number
    renderers.clear();
    int layerNumber = 0;

    // ohinds 2009-02-23
    // added layer to layer list
//    // First add default image layer
//    vtkRenderer* ren = m_ImageLayer->GetRenderer();
//    renderers.push_back(ren);
//    ren->SetLayer(layerNumber);
//    ++layerNumber;
 
    vtkRenderer* ren;

    // Add renderers for Colormap and Roi layers
    LayersCollection::iterator it, itEnd(m_Layers.end());
    for(it = m_Layers.begin(); it != itEnd; ++it){
      // ohinds 2009-02-23
      // unneccesary
//        if((*it)->GetType() == FrBaseLayer::LtColormap){
//            ren = (*it)->GetRenderer();
//            ren->SetLayer(layerNumber);
//            renderers.push_back(ren);
//
//            ++layerNumber;
//        }
//    }
//    
//    // Rois
//    for(it = m_Layers.begin(); it != itEnd; ++it){
//        if((*it)->GetType() == FrBaseLayer::LtRoi){

      ren = (*it)->GetRenderer();
      ren->SetLayer(layerNumber);
      renderers.push_back(ren);
      
      ++layerNumber;
    }
    //    }

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
  double *bounds = NULL;

  LayersCollection::iterator it, itEnd(m_Layers.end());
  for(it = m_Layers.begin(); it != itEnd; ++it){
    if((*it)->GetType() == FrBaseLayer::LtImage) {
      bounds = ((FrImageLayer*) *it)->GetActor()->GetBounds();
      break;
    }
  }
    
  return bounds;
}

double* FrLayeredImage::GetActorCenter(){
    double* center = NULL;
    
    LayersCollection::iterator it, itEnd(m_Layers.end());
    for(it = m_Layers.begin(); it != itEnd; ++it){
      if((*it)->GetType() == FrBaseLayer::LtImage) {
	center = ((FrImageLayer*) *it)->GetActor()->GetCenter();
	break;
      }
    }

    return center;
}

vtkImageActor* FrLayeredImage::GetActor(){
  vtkImageActor* actor = NULL;
   
    LayersCollection::iterator it, itEnd(m_Layers.end());
  for(it = m_Layers.begin(); it != itEnd; ++it){
    if((*it)->GetType() == FrBaseLayer::LtImage) {
      actor = ((FrImageLayer*) *it)->GetActor();
      break;
    }
  }

  return actor;
}
