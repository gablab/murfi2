#include "FrOrthoView.h"
#include "FrMainWindow.h"
#include "QVTKWidget.h"
#include "FrDocumentReader.h"
#include "FrDocument.h"
#include "FrMainDocument.h"
#include "FrNotify.h"
#include "FrUtils.h"
#include "FrSliceExtractor.h"
#include "FrTabSettingsDocObj.h"
#include "FrLayeredImage.h"


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
    m_docReader = FrDocumentReader::New();

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
    if(m_docReader) m_docReader->Delete();

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
                        	
    // Update pipeline
    switch(point)
    {
    case FRP_FULL:
        {
            // NOTE: Do nothing here !!!
        }
    case FRP_READIMAGE:
        {
            this->ReadDocument(params);
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
    params.ActiveLayer[CORONAL_IMAGE] = GetLayerAndInitLayers(
        params.Layers[CORONAL_IMAGE], params.ViewSettings, CORONAL_IMAGE);

    // Sagital
    params.ActiveLayer[SAGITAL_IMAGE] = GetLayerAndInitLayers(
        params.Layers[SAGITAL_IMAGE], params.ViewSettings, SAGITAL_IMAGE);

    // Axial
    params.ActiveLayer[AXIAL_IMAGE] = GetLayerAndInitLayers(
        params.Layers[AXIAL_IMAGE], params.ViewSettings, AXIAL_IMAGE);
}

FrLayerSettings* FrOrthoView::GetLayerAndInitLayers(
                                           std::vector<FrLayerSettings*>* layers, 
                                           FrOrthoViewSettings* viewSets, 
                                           int rendererID){
    
    int layerID = viewSets->ActiveLayerID;
    GetLayerSettings(viewSets, *layers, rendererID);
    LayerCollection::iterator it, itEnd(layers->end());    

    // If not 'broadcast update' then get layer settings 
    FrLayerSettings* layer = 0L;
    if(layerID != ALL_LAYERS_ID){
        for(it = layers->begin(); it != itEnd; ++it){
            if((*it)->ID == layerID){
                layer = (*it);
                break;
            }
        }
    }
    return layer;
}

void FrOrthoView::ReadDocument(FrUpdateParams2& params){
    // Read document and connect filters
    m_docReader->SetDocument(params.Document);
    m_docReader->SetUnMosaicOn();
    m_docReader->Update();
	
    // Setup slice extractor filter
    for(int i=0; i < ORTHO_IMAGE_COUNT; ++i){
        m_SliceExtractor[i]->SetInput(m_docReader->GetOutput());
    }

    // Pass ROI data from output starting from 1
    // to each of slice extractors
    int count = m_docReader->GetOutputCount();
    for(int i=0; i < ORTHO_IMAGE_COUNT; ++i){
        for(int j=1; j < count; ++j){
            m_SliceExtractor[i]->SetInput(j, m_docReader->GetOutput(j));
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
            int count = m_docReader->GetOutputCount();
            for(int j=1; j < count; ++j){
                m_LayeredImage[i]->SetROIInput(j, m_SliceExtractor[i]->GetOutput(j));
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
        else {
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
        else {
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
        else {
            m_LayeredImage[i]->SetOpacity(
                params.ActiveLayer[i]->Opacity, params.ActiveLayer[i]->ID);
            m_LayeredImage[i]->SetVisibility(
                params.ActiveLayer[i]->Visibility, params.ActiveLayer[i]->ID);
        }
    }

    // TODO: add ROI opacity and visibility update ???
}