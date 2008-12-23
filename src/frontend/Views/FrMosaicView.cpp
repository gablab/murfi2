#include "FrMosaicView.h"
#include "FrMainWindow.h"
#include "QVTKWidget.h"
#include "FrDocumentReader.h"
#include "FrDocument.h"
#include "FrMainDocument.h"
#include "FrNotify.h"
#include "FrLayeredImage.h"
#include "FrUtils.h"
#include "FrLayerDocObj.h"
#include "FrViewDocObj.h"

// VTK stuff
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRendererCollection.h"

// FrUpdateParams represents params 
class FrUpdateParams1 {
public:
    FrMainDocument*      Document;
    FrMosaicViewSettings* ViewSettings;
    typedef std::vector<FrLayerDocObj*> LayerCollection;
    LayerCollection Layers;
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
    if(!this->InitUpdateParams(params)) return;
                        	
    // Update pipeline
    switch(point)
    {
    case FRP_FULL: // NOTE: Do nothing here !!!
    case FRP_READ:
        {
            this->ReadDocument(params);
        }
    case FRP_COLORMAP:
        {
            this->UpdateColormap(params);
        }
    case FRP_TBC:
        {
            this->UpdateTbc(params);
        }
    case FRP_OPACITY_VISIBILITY:
        {   
            FrUpdateParams1::LayerCollection::iterator it,
                itEnd(params.Layers.end());

            for(it = params.Layers.begin(); it != itEnd; ++it){
                unsigned int id = (*it)->GetID();
                m_LayeredImage->SetOpacity((*it)->GetOpacity(), id);
                m_LayeredImage->SetVisibility((*it)->GetVisibility(), id); 
            }
        }
    case FRP_SETCAM:
        {
            m_LayeredImage->SetCameraSettings(
                params.ViewSettings->CamSettings, 
                ALL_LAYER_ID);

            m_LayeredImage->UpdateCamera();
        }
    default:
        // do nothing
        break;
    }

    // redraw scene
	GetRenderWindow()->Render();
}

bool FrMosaicView::InitUpdateParams(FrUpdateParams1& params){
    // Init params used when updating
    params.Document = this->GetMainWindow()->GetMainDocument();

    // Get view settings
    if(!params.Document->GetCurrentViewObject()) return false;
    params.ViewSettings = params.Document->
        GetCurrentViewObject()->GetMosaicViewSettings();

    // Get layers for update
    unsigned int activeLayerID = params.ViewSettings->ActiveLayerID;
    FrDocument::DocObjCollection objects;
    params.Document->GetObjectsByType(objects, FrDocumentObj::LayerObject);

    params.Layers.clear();
    FrDocument::DocObjCollection::iterator it, itEnd(objects.end());
    for(it = objects.begin(); it != itEnd; ++it){
        FrLayerDocObj* layerDO = (FrLayerDocObj*)(*it);
        if(activeLayerID == ALL_LAYER_ID){
            params.Layers.push_back(layerDO);
        }
        else if(activeLayerID == layerDO->GetID()){
            params.Layers.push_back(layerDO);
            break;
        }
    }
    return true;
}

void FrMosaicView::ReadDocument(FrUpdateParams1& params){
    FrViewDocObj* vdo = params.Document->GetCurrentViewObject();
    m_docReader->SetDocument(params.Document);
    m_docReader->SetMosaic(true);
    m_docReader->SetOrientation(FrDocumentReader::XY);    
    m_docReader->SetTimeSeries(vdo->GetTimeSeries());

    FrUpdateParams1::LayerCollection::iterator it,
        itEnd(params.Layers.end());

    for(it = params.Layers.begin(); it != itEnd; ++it){
        if((*it)->IsRoi()){
            m_docReader->SetTarget(FrDocumentReader::Roi);
            m_docReader->SetDataID((*it)->GetID());
            m_docReader->Update();
            m_LayeredImage->SetRoiInput(m_docReader->GetOutput(),
                                        (*it)->GetID());
            
        }
        else if((*it)->IsImage()){  
            // ID is current Timepoint since we have 
            // just one time series 
            m_docReader->SetTarget(FrDocumentReader::Mri);
            m_docReader->SetDataID(vdo->GetTimePoint());
            m_docReader->Update();

            m_LayeredImage->SetImageInput(m_docReader->GetOutput());
        }
    }

    // set input to special layer here
    m_docReader->SetTarget(FrDocumentReader::Points);
    m_docReader->Update();

    // set params
    SelectionParams Params;
    Params.type = 4;

    FrSpecialLayer* sl = m_LayeredImage->GetSpecialLayer();
    sl->SetSelectionData(m_docReader->GetOutput());
    sl->SetSelection(Params);
    sl->SetSelectionVisibility(true);
}

void FrMosaicView::UpdateColormap(FrUpdateParams1& params){
    FrUpdateParams1::LayerCollection::iterator it,
        itEnd(params.Layers.end());

    for(it = params.Layers.begin(); it != itEnd; ++it){
        if((*it)->IsColormap()){
            FrColormapLayerSettings* cmls = (FrColormapLayerSettings*)(*it)->GetSettings();
            m_LayeredImage->SetColormapSettings(cmls->ColormapSettings, (*it)->GetID());
        }
    }
    m_LayeredImage->UpdateColormap();
}

void FrMosaicView::UpdateTbc(FrUpdateParams1& params){
     FrUpdateParams1::LayerCollection::iterator it,
        itEnd(params.Layers.end());

    for(it = params.Layers.begin(); it != itEnd; ++it){
        if((*it)->IsColormap()){
            FrColormapLayerSettings* cmls = (FrColormapLayerSettings*)(*it)->GetSettings();
            m_LayeredImage->SetTbcSettings(cmls->TbcSettings, (*it)->GetID());
        }
        else if((*it)->IsImage()){
            FrImageLayerSettings* ils = (FrImageLayerSettings*)(*it)->GetSettings();
            m_LayeredImage->SetTbcSettings(ils->TbcSettings, (*it)->GetID());
        }
    }
    m_LayeredImage->UpdateTbc();
}
