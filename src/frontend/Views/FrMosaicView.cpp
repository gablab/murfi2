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
#include "FrRoiDocObj.h"

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
    std::vector<int>     RoiIDs;
};



// Default constructor
FrMosaicView::FrMosaicView(FrMainWindow* mainWindow)
: FrBaseView(mainWindow) {
    m_imgReader = FrDocumentReader::New();	
    m_imgReader->SetTarget(FrDocumentReader::Image);

    m_roiReader = FrDocumentReader::New();	
    m_roiReader->SetTarget(FrDocumentReader::ROI);

    m_LayeredImage = FrLayeredImage::New();
}

FrMosaicView::~FrMosaicView(){
    this->RemoveRenderers();
    if(m_imgReader) m_imgReader->Delete();
    if(m_roiReader) m_roiReader->Delete();
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
    this->InitUpdateParams(params);

    FrRoiDocObj* roi = 0L;
    std::vector<FrDocumentObj*> rois;
    params.Document->GetObjectsByType(rois, FrDocumentObj::RoiObject);
    std::vector<FrDocumentObj*>::iterator it, itEnd(rois.end());

    // Update pipeline
    switch(point)
    {
    case FRP_FULL:
        {
            // NOTE: Do nothing here !!!
        }
    case FRP_READIMAGE:
        {
            this->ReadImage(params);
        }
    case FRP_READROI:
        {
            this->ReadRoi(params);
        }
	case FRP_SLICE:
        {
            // NOTE: Do nothing here but setup text
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
            
            // have to update rois
            if(params.ActiveLayerID == ALL_LAYERS_ID){
                for(it = rois.begin(); it != itEnd; ++it){
                    roi = (FrRoiDocObj*)(*it);
                    m_LayeredImage->SetOpacity(roi->GetOpacity(), roi->GetID());
                    m_LayeredImage->SetVisibility(roi->GetVisibility(), roi->GetID());
                }
            }
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

    params.ActiveLayer = this->GetActiveLayer(params.Layers, params.ActiveLayerID);
    
    // add all ROI IDs
    this->GetRoiIDs(params.Document, params.RoiIDs);
}

void FrMosaicView::ReadImage(FrUpdateParams1& params){
    // read document and connect filters
    m_imgReader->SetDocument(params.Document);
    m_imgReader->SetMosaicOn();
    m_imgReader->Update();
	
    // Pass image input 
    m_LayeredImage->SetInput(m_imgReader->GetOutput());
}

void FrMosaicView::ReadRoi(FrUpdateParams1& params){
    // read document and connect filters
    m_roiReader->SetDocument(params.Document);
    m_roiReader->SetMosaicOn();
    m_roiReader->Update();
	    
    // Pass ROI data from output starting from 1
    int count = m_roiReader->GetOutputCount();
    for(int i=0; i < count; ++i){
        int roiID = params.RoiIDs[i];
        m_LayeredImage->SetRoiInput(roiID, m_roiReader->GetOutput(i));
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
    else if(params.ActiveLayer != 0) {
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
    else if(params.ActiveLayer != 0) {
        m_LayeredImage->SetTBCSettings(
            params.ActiveLayer->TbcSettings, params.ActiveLayer->ID);
    }
    m_LayeredImage->UpdateTBC();
}

void FrMosaicView::UpdateOpacityVisibility(FrUpdateParams1& params){
    // Update Opacity and visibility
    if(params.ActiveLayerID == ALL_LAYERS_ID){
        // Update all layers
        LayerCollection::iterator it, itEnd(params.Layers.end());
        for(it = params.Layers.begin(); it != itEnd; ++it){
            m_LayeredImage->SetOpacity((*it)->Opacity, (*it)->ID);
            m_LayeredImage->SetVisibility((*it)->Visibility, (*it)->ID);
        }
    }
    else if(params.ActiveLayer != 0) {
        m_LayeredImage->SetOpacity(
            params.ActiveLayer->Opacity, params.ActiveLayer->ID);
        m_LayeredImage->SetVisibility(
            params.ActiveLayer->Visibility, params.ActiveLayer->ID);
    }
    else {
        // Seems this is ROI layer
        bool roiVisibility = true;
        double roiOpacity = 1.0;
        if(this->GetRoiParams(params.Document, 
            params.ActiveLayerID, roiVisibility, roiOpacity)){
            // apply roi params
            m_LayeredImage->SetOpacity(roiOpacity, params.ActiveLayerID);
            m_LayeredImage->SetVisibility(roiVisibility, params.ActiveLayerID);
        }
    }
}
