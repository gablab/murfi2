#include "FrSliceView.h"
#include "QVTKWidget.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrImageDocObj.h"
#include "FrInteractorStyle.h"
#include "FrDocumentReader.h"
#include "FrSliceExtractor.h"
#include "FrTabSettingsDocObj.h"
#include "FrLayeredImage.h"
#include "FrNotify.h"
#include "FrUtils.h"
#include "FrRoiDocObj.h"

// VTK
#include "vtkRenderWindowInteractor.h"
#include "vtkInteractorStyleImage.h"
#include "vtkRenderer.h"
#include "vtkCamera.h"
#include "vtkImageData.h"
#include "vtkImageActor.h"
#include "vtkPNGReader.h"
#include "vtkRenderWindow.h"
#include "vtkActorCollection.h"
#include "vtkRendererCollection.h"

// FrUpdateParams represents params 
class FrUpdateParams0 {
public:
    FrMainDocument*      Document;
    FrTabSettingsDocObj* TabSettings;
    FrSliceViewSettings* ViewSettings;
    int                  ActiveLayerID;
    FrLayerSettings*     ActiveLayer;
    LayerCollection      Layers;
    std::vector<int>     RoiIDs;
};


// Default constructor
FrSliceView::FrSliceView(FrMainWindow* mainWindow)
: FrBaseView(mainWindow) {
    	
    // Create pipeline stuff
    m_imgReader = FrDocumentReader::New();
    m_imgReader->SetTarget(FrDocumentReader::Image);

    m_roiReader = FrDocumentReader::New();
    m_roiReader->SetTarget(FrDocumentReader::ROI);

	m_SliceExtractor = FrSliceExtractor::New();
    m_LayeredImage = FrLayeredImage::New();
}

FrSliceView::~FrSliceView(){
    this->RemoveRenderers();
	if(m_SliceExtractor) m_SliceExtractor->Delete();
    if(m_imgReader) m_imgReader->Delete();
    if(m_roiReader) m_roiReader->Delete();
    if(m_LayeredImage) m_LayeredImage->Delete();
}

void FrSliceView::Initialize(){
    // Some initialization
    this->SetupRenderers();
}

void FrSliceView::SetupRenderers(){
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
    renWin->SetNumberOfLayers( int(renderers.size()) );
}

void FrSliceView::RemoveRenderers(){
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

void FrSliceView::UpdatePipeline(int point){    
    // Get common settings
    FrUpdateParams0 params;
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
            this->ExtractSlice(params);
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

    // redraw scene
	GetRenderWindow()->Render();
}

void FrSliceView::InitUpdateParams(FrUpdateParams0& params){
    // Init params used when updating
    params.Document     = this->GetMainWindow()->GetMainDocument();
    params.TabSettings  = params.Document->GetCurrentTabSettings();
    params.ViewSettings = params.TabSettings->GetSliceViewSettings();

    params.ActiveLayerID = params.ViewSettings->ActiveLayerID;    
    GetLayerSettings(params.ViewSettings, params.Layers);

    params.ActiveLayer = this->GetActiveLayer(params.Layers, params.ActiveLayerID);
    
    // Add all roi IDs
    this->GetRoiIDs(params.Document, params.RoiIDs);
}

void FrSliceView::ReadImage(FrUpdateParams0& params){
    // Read document and connect filters
    m_imgReader->SetDocument(params.Document);
    m_imgReader->SetUnMosaicOn();
    m_imgReader->Update();
	
    // Pass image to slice extractor
    m_SliceExtractor->SetInput(m_imgReader->GetOutput());
}

void FrSliceView::ReadRoi(FrUpdateParams0& params){
    m_roiReader->SetDocument(params.Document);
    m_roiReader->SetUnMosaicOn();
    m_roiReader->Update();

    // Remove all ports except default
    m_SliceExtractor->ClearAdditionalPorts();

    // Pass ROI data from output starting from port#1
    // since port#0 used for image data
    int count = m_roiReader->GetOutputCount();
    for(int i=0, port=1; i < count; ++i, ++port){
        m_SliceExtractor->SetInput(port, m_roiReader->GetOutput(i));
    }
}

void FrSliceView::ExtractSlice(FrUpdateParams0& params){
    // set slice to be rendered
    int maxSliceNumber = m_SliceExtractor->GetMaxSliceNumber();
    params.ViewSettings->SliceNumber = ClampValue(
        params.ViewSettings->SliceNumber, 0, maxSliceNumber);

    m_SliceExtractor->SetSlice(params.ViewSettings->SliceNumber);

    if(m_SliceExtractor->GetInput()){
	    m_SliceExtractor->Update();

        // Set image data
        m_LayeredImage->SetInput(m_SliceExtractor->GetOutput());

        // Set ROI data
        int count = m_roiReader->GetOutputCount();
        for(int i=0, port=1; i < count; ++i, ++port){
            int roiID = params.RoiIDs[i];
            m_LayeredImage->SetROIInput(roiID, m_SliceExtractor->GetOutput(port));
        }

        // Set text
        char text[255];
        sprintf(text, "Slice View: %d of %d", 
            params.ViewSettings->SliceNumber, maxSliceNumber);

        m_LayeredImage->SetText(text);
    }
    else {
        m_LayeredImage->SetText("");
    }
}

void FrSliceView::UpdateColormap(FrUpdateParams0& params){
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

void FrSliceView::UpdateTBC(FrUpdateParams0& params){
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

void FrSliceView::UpdateOpacityVisibility(FrUpdateParams0& params){
    // Update Opacity and visibility
    if(params.ActiveLayerID == ALL_LAYERS_ID){
        // Update all params
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

