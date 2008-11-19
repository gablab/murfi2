#include "FrOrthoView.h"
#include "FrMainWindow.h"
#include "QVTKWidget.h"
#include "FrDocumentReader.h"
#include "FrMainDocument.h"
#include "FrNotify.h"
#include "FrUtils.h"
#include "FrSliceExtractor.h"
#include "FrTabSettingsDocObj.h"
#include "FrLayeredImage.h"
#include "FrRoiDocObj.h"

// VTK stuff
#include "vtkCamera.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRendererCollection.h"
#include "vtkTextActor.h"
#include "vtkImageActor.h"
#include "vtkRenderWindow.h"
#include "vtkCoordinate.h"
#include "vtkTextProperty.h"

#include <vector>

// FrUpdateParams represents params 
class FrUpdateParams2 {
public:
    FrMainDocument*      Document;
    FrTabSettingsDocObj* TabSettings;
    FrOrthoViewSettings* ViewSettings;
    int                  ActiveLayerID;
    FrLayerSettings*     ActiveLayer[3];
    LayerCollection*     Layers[3];
    std::vector<int>     RoiIDs;

public:    
    FrUpdateParams2(){
        ActiveLayer[CORONAL_IMAGE] = 0L;
        ActiveLayer[SAGITAL_IMAGE] = 0L;
        ActiveLayer[AXIAL_IMAGE] = 0L;

        Layers[CORONAL_IMAGE] = new LayerCollection();
        Layers[SAGITAL_IMAGE] = new LayerCollection();
        Layers[AXIAL_IMAGE] = new LayerCollection();
    }

    ~FrUpdateParams2(){
        delete Layers[CORONAL_IMAGE];
        delete Layers[SAGITAL_IMAGE];
        delete Layers[AXIAL_IMAGE];
    }
};

FrSliceExtractor::Orientation g_Orientation[ORTHO_IMAGE_COUNT];

// Default constructor
FrOrthoView::FrOrthoView(FrMainWindow* mainWindow)
: FrBaseView(mainWindow) {
    // creating reader
    m_imgReader = FrDocumentReader::New();
    m_imgReader->SetTarget(FrDocumentReader::Image);

    m_roiReader = FrDocumentReader::New();
    m_roiReader->SetTarget(FrDocumentReader::ROI);

    // Init orientations
    g_Orientation[CORONAL_IMAGE] = FrSliceExtractor::XZ;
    g_Orientation[SAGITAL_IMAGE] = FrSliceExtractor::YZ;
    g_Orientation[AXIAL_IMAGE]   = FrSliceExtractor::XY;

    // create renderers
	for(int i = 0; i < ORTHO_IMAGE_COUNT; i++){
		m_SliceExtractor[i] = FrSliceExtractor::New();
        m_LayeredImage[i] = FrLayeredImage::New();
    }
    m_dummyRenderer = vtkRenderer::New();
    m_dummyRenderer->SetBackground(0.0, 0.0, 0.0);
    m_dummyRenderer->GetActiveCamera()->ParallelProjectionOn();
}

FrOrthoView::~FrOrthoView(){
    this->RemoveRenderers();
    if(m_imgReader) m_imgReader->Delete();
    if(m_roiReader) m_roiReader->Delete();

    for(int i = 0; i < ORTHO_IMAGE_COUNT; i++){
        if(m_SliceExtractor[i]) m_SliceExtractor[i]->Delete();
        if(m_LayeredImage[i]) m_LayeredImage[i]->Delete();
    } 

    if(m_dummyRenderer) m_dummyRenderer->Delete();
}

void FrOrthoView::Initialize(){
    // Some initialization
    SetupRenderers();
}

void FrOrthoView::SetupRenderers(){
    
    RemoveRenderers();

    int layersNum = 0;
    std::vector<vtkRenderer*> renderers;
	vtkRenderWindow* renWin = QTVIEW3D->GetRenderWindow();
    for(int i=0; i < ORTHO_IMAGE_COUNT; ++i){
        renderers.clear();
        m_LayeredImage[i]->GetRenderers(renderers);
        layersNum = std::max(layersNum, int(renderers.size()));

        // Add renderers and adjust viewport
        std::vector<vtkRenderer*>::iterator it, itEnd(renderers.end());
        for(it = renderers.begin(); it != itEnd; ++it){
            renWin->AddRenderer( (*it) );
            if(i == CORONAL_IMAGE){
                (*it)->SetViewport(0.0, 0.5, 0.5, 1.0);
            }
            else if(i == SAGITAL_IMAGE){
                (*it)->SetViewport(0.5, 0.5, 1.0, 1.0);
            }
            else if(i == AXIAL_IMAGE){
                (*it)->SetViewport(0.0, 0.0, 0.5, 0.5);
            }
        }
    }
    // add dummy renderer
    renWin->AddRenderer(m_dummyRenderer);
    m_dummyRenderer->SetViewport(0.5, 0.0, 1.0, 0.5);

    renWin->GetRenderers()->InitTraversal();
    renWin->SetNumberOfLayers(layersNum);
}

void FrOrthoView::RemoveRenderers(){
    // Remove all renderers
    std::vector<vtkRenderer*> renderers;
	vtkRenderWindow* renWin = QTVIEW3D->GetRenderWindow();
    for(int i=0; i < ORTHO_IMAGE_COUNT; ++i){
        renderers.clear();
        m_LayeredImage[i]->GetRenderers(renderers);

        std::vector<vtkRenderer*>::iterator it, itEnd(renderers.end());
        for(it = renderers.begin(); it != itEnd; ++it){
            renWin->RemoveRenderer( (*it) );
            (*it)->SetRenderWindow(0);
        }
        
    }
    // remove dummy renderer
    renWin->RemoveRenderer(m_dummyRenderer);

    renWin->GetRenderers()->InitTraversal();
    renWin->SetNumberOfLayers(0);
}

void FrOrthoView::UpdatePipeline(int point){

    // Init params
    FrUpdateParams2 params;
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
            if(params.ActiveLayerID == ALL_LAYERS_ID){
                // have to update rois
                for(int i=0; i < ORTHO_IMAGE_COUNT; ++i){
                    for(it = rois.begin(); it != itEnd; ++it){
                        roi = (FrRoiDocObj*)(*it);
                        m_LayeredImage[i]->SetOpacity(roi->GetOpacity(), roi->GetID());
                        m_LayeredImage[i]->SetVisibility(roi->GetVisibility(), roi->GetID());
                    }
                }
            }
        }
    case FRP_SETCAM:
        {
            for(int i=0; i < ORTHO_IMAGE_COUNT; ++i){
                m_LayeredImage[i]->SetCameraSettings(
                    params.ViewSettings->CamSettings[i], ALL_LAYERS_ID);	
                m_LayeredImage[i]->UpdateCamera();
            }
        }
    default:
        // do nothing
        break;
    }

    // Draw border
    for(int i=0; i < ORTHO_IMAGE_COUNT; ++i){
        m_LayeredImage[i]->UpdateBorder();
    }

    // redraw scene
	GetRenderWindow()->Render();
}

void FrOrthoView::InitUpdateParams(FrUpdateParams2& params){
    // Get common settings
    params.Document = this->GetMainWindow()->GetMainDocument();
    params.TabSettings = params.Document->GetCurrentTabSettings();
    params.ViewSettings = params.TabSettings->GetOrthoViewSettings();

    // Get settings of layers
    params.ActiveLayerID = params.ViewSettings->ActiveLayerID;

    // Coronal
    GetLayerSettings(params.ViewSettings, *(params.Layers[CORONAL_IMAGE]), CORONAL_IMAGE);
    params.ActiveLayer[CORONAL_IMAGE] = this->GetActiveLayer(
        *(params.Layers[CORONAL_IMAGE]), params.ActiveLayerID);

    // Sagital
    GetLayerSettings(params.ViewSettings, *(params.Layers[SAGITAL_IMAGE]), SAGITAL_IMAGE);
    params.ActiveLayer[SAGITAL_IMAGE] = this->GetActiveLayer(
        *(params.Layers[SAGITAL_IMAGE]), params.ActiveLayerID);

    // Axial
    GetLayerSettings(params.ViewSettings, *(params.Layers[AXIAL_IMAGE]), AXIAL_IMAGE);
    params.ActiveLayer[AXIAL_IMAGE] = this->GetActiveLayer(
        *(params.Layers[AXIAL_IMAGE]), params.ActiveLayerID);

    // ROIs
    this->GetRoiIDs(params.Document, params.RoiIDs);
}

void FrOrthoView::ReadImage(FrUpdateParams2& params){
    // Read document and connect filters
    m_imgReader->SetDocument(params.Document);
    m_imgReader->SetUnMosaicOn();
    m_imgReader->Update();
	
    // Setup slice extractor filter
    for(int i=0; i < ORTHO_IMAGE_COUNT; ++i){
        m_SliceExtractor[i]->SetInput(m_imgReader->GetOutput());
    }
}

void FrOrthoView::ReadRoi(FrUpdateParams2& params){
    // Read document and connect filters
    m_roiReader->SetDocument(params.Document);
    m_roiReader->SetUnMosaicOn();
    m_roiReader->Update();
	
    // Pass ROI data from output starting from 1
    // to each of slice extractors
    int count = m_imgReader->GetOutputCount();
    for(int i=0; i < ORTHO_IMAGE_COUNT; ++i){
        // first clear all
        m_SliceExtractor[i]->RemoveAdditionalPorts();

        for(int j=0, port=1; j < count; ++j, ++port){
            m_SliceExtractor[i]->SetInput(
                port, m_roiReader->GetOutput(j));
        }
    }
}

void FrOrthoView::ExtractSlice(FrUpdateParams2& params){
    // set slice to be rendered
    int maxSliceNumber = 0;
    // Coronal
    m_SliceExtractor[CORONAL_IMAGE]->SetOrientation(g_Orientation[CORONAL_IMAGE]);
    maxSliceNumber = m_SliceExtractor[CORONAL_IMAGE]->GetMaxSliceNumber();
    params.ViewSettings->CoronalSlice = ClampValue(
        params.ViewSettings->CoronalSlice, 0, maxSliceNumber);
    m_SliceExtractor[CORONAL_IMAGE]->SetSlice(params.ViewSettings->CoronalSlice);
    //Sagital
    m_SliceExtractor[SAGITAL_IMAGE]->SetOrientation(g_Orientation[SAGITAL_IMAGE]);
    maxSliceNumber = m_SliceExtractor[SAGITAL_IMAGE]->GetMaxSliceNumber();
    params.ViewSettings->SagitalSlice = ClampValue(
        params.ViewSettings->SagitalSlice, 0, maxSliceNumber);
    m_SliceExtractor[SAGITAL_IMAGE]->SetSlice(params.ViewSettings->SagitalSlice);
    // Axial
    m_SliceExtractor[AXIAL_IMAGE]->SetOrientation(g_Orientation[AXIAL_IMAGE]);
    maxSliceNumber = m_SliceExtractor[AXIAL_IMAGE]->GetMaxSliceNumber();
    params.ViewSettings->AxialSlice = ClampValue(
        params.ViewSettings->AxialSlice, 0, maxSliceNumber);
    m_SliceExtractor[AXIAL_IMAGE]->SetSlice(params.ViewSettings->AxialSlice);
            
    // Connect output to layered image
    char* text[3];
    text[CORONAL_IMAGE] = "Ortho View: Coronal";
    text[SAGITAL_IMAGE] = "Ortho View: Sagital";
    text[AXIAL_IMAGE]   = "Ortho View: Axial";
    for(int i=0; i < ORTHO_IMAGE_COUNT; ++i){
        if(m_SliceExtractor[i]->GetInput()){
            m_SliceExtractor[i]->Update();

            // Set image data
            m_LayeredImage[i]->SetInput(m_SliceExtractor[i]->GetOutput());
            
            // Set ROI data
            int count = m_roiReader->GetOutputCount();
            for(int j=0, port=1; j < count; ++j, ++port){
                int roiID = params.RoiIDs[j];
                m_LayeredImage[i]->SetRoiInput(roiID, m_SliceExtractor[i]->GetOutput(port));
            }

            // Set text
            m_LayeredImage[i]->SetText(text[i]);
        }
        else {
            m_LayeredImage[i]->SetText("");
        }
    }
}

void FrOrthoView::UpdateColormap(FrUpdateParams2& params){
    // Update here colormap values
    for(int i=0; i < ORTHO_IMAGE_COUNT; ++i){
        if(params.ActiveLayerID == ALL_LAYERS_ID){
            // Update all layers
            LayerCollection::iterator it, itEnd(params.Layers[i]->end());
            for(it = params.Layers[i]->begin(); it != itEnd; ++it){
                m_LayeredImage[i]->SetColormapSettings(
                    (*it)->ColormapSettings, (*it)->ID);
            }
        }
        else if(params.ActiveLayer[i] != 0) {
            m_LayeredImage[i]->SetColormapSettings(
                params.ActiveLayer[i]->ColormapSettings, params.ActiveLayer[i]->ID);
        }
        m_LayeredImage[i]->UpdateColormap();
    }
}

void FrOrthoView::UpdateTBC(FrUpdateParams2& params){
    // set TBC values
    for(int i=0; i < ORTHO_IMAGE_COUNT; ++i){
        if(params.ActiveLayerID == ALL_LAYERS_ID){
            // Update all layers
            LayerCollection::iterator it, itEnd(params.Layers[i]->end());
            for(it = params.Layers[i]->begin(); it != itEnd; ++it){
                m_LayeredImage[i]->SetTBCSettings(
                    (*it)->TbcSettings, (*it)->ID);
            }
        }
        else if(params.ActiveLayer[i] != 0) {
            m_LayeredImage[i]->SetTBCSettings(
                params.ActiveLayer[i]->TbcSettings, params.ActiveLayer[i]->ID);
        }
        m_LayeredImage[i]->UpdateTBC();
    }
}

void FrOrthoView::UpdateOpacityVisibility(FrUpdateParams2& params){
    for(int i=0; i < ORTHO_IMAGE_COUNT; ++i){
        if(params.ActiveLayerID == ALL_LAYERS_ID){
            // Update all layers
            LayerCollection::iterator it, itEnd(params.Layers[i]->end());
            for(it = params.Layers[i]->begin(); it != itEnd; ++it){
                m_LayeredImage[i]->SetOpacity((*it)->Opacity, (*it)->ID);
                m_LayeredImage[i]->SetVisibility((*it)->Visibility, (*it)->ID);
            }
        }
        else if(params.ActiveLayer[i] != 0) {
            m_LayeredImage[i]->SetOpacity(
                params.ActiveLayer[i]->Opacity, params.ActiveLayer[i]->ID);
            m_LayeredImage[i]->SetVisibility(
                params.ActiveLayer[i]->Visibility, params.ActiveLayer[i]->ID);
        }
        else {
            // Seems this is ROI layer
            bool roiVisibility = true;
            double roiOpacity = 1.0;
            if(this->GetRoiParams(params.Document, 
                params.ActiveLayerID, roiVisibility, roiOpacity)){
                // apply roi params
                m_LayeredImage[i]->SetOpacity(roiOpacity, params.ActiveLayerID);
                m_LayeredImage[i]->SetVisibility(roiVisibility, params.ActiveLayerID);
            }
        }
    }
}
