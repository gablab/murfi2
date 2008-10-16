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
#include "FrMyLayeredImage.h"

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


FrSliceExtractor::Orientation g_Orientation[ORTHO_IMAGE_COUNT];

// Default constructor
FrOrthoView::FrOrthoView(FrMainWindow* mainWindow)
: FrBaseView(mainWindow) {
       
    m_docReader = FrDocumentReader::New();

    // Init orientations
    g_Orientation[CORONAL_IMAGE] = FrSliceExtractor::XZ;
    g_Orientation[SAGITAL_IMAGE] = FrSliceExtractor::YZ;
    g_Orientation[AXIAL_IMAGE]   = FrSliceExtractor::XY;

    // create renderers
	for(int i = 0; i < ORTHO_IMAGE_COUNT; i++){
		m_SliceExtractor[i] = FrSliceExtractor::New();
        m_LayeredImage[i] = FrMyLayeredImage::New();
    }
    m_dummyRenderer = vtkRenderer::New();
    m_dummyRenderer->SetBackground(0.0, 0.0, 0.0);
    m_dummyRenderer->GetActiveCamera()->ParallelProjectionOn();
}

FrOrthoView::~FrOrthoView(){
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
    // Setup some vars
    int maxSliceNumber = 0;

    // Get common settings
    FrMainDocument* document = GetMainWindow()->GetMainDocument();
    FrTabSettingsDocObj* tabSets = document->GetCurrentTabSettings();
    FrOrthoViewSettings* viewSets = tabSets->GetOrthoViewSettings();

    // Get settings of layers
    LayerCollection  cLayers, sLayers, aLayers;
    LayerCollection* layers[3];
    layers[CORONAL_IMAGE] = &cLayers;
    layers[SAGITAL_IMAGE] = &sLayers;
    layers[AXIAL_IMAGE] = &aLayers;
    
    FrLayerSettings* layer[3];
    layer[CORONAL_IMAGE] = GetLayerAndInitLayers(cLayers, viewSets, CORONAL_IMAGE);
    layer[SAGITAL_IMAGE] = GetLayerAndInitLayers(sLayers, viewSets, SAGITAL_IMAGE);
    layer[AXIAL_IMAGE] = GetLayerAndInitLayers(aLayers, viewSets, AXIAL_IMAGE);

    LayerCollection::iterator it, itEnd;
                        	
    // Update pipeline
    switch(point)
    {
    case FRP_FULL:
        {
            // NOTE: Do nothing here !!!
        }
    case FRP_READIMAGE:
        {
            // read document and connect filters
            m_docReader->SetDocument(document);
            m_docReader->SetUnMosaicOn(true);
            m_docReader->Update();
			
            // Setup slice extractor filter
            for(int i=0; i < ORTHO_IMAGE_COUNT; ++i){
                m_SliceExtractor[i]->SetInput(m_docReader->GetOutput());
            }
        }
	case FRP_SLICE:
        {
            // set slice to be rendered
            m_SliceExtractor[CORONAL_IMAGE]->SetOrientation(g_Orientation[CORONAL_IMAGE]);
            maxSliceNumber = m_SliceExtractor[CORONAL_IMAGE]->GetMaxSliceNumber();
            viewSets->CoronalSlice = ClampValue(viewSets->CoronalSlice, 0, maxSliceNumber);
            m_SliceExtractor[CORONAL_IMAGE]->SetSlice(viewSets->CoronalSlice);
            
            m_SliceExtractor[SAGITAL_IMAGE]->SetOrientation(g_Orientation[SAGITAL_IMAGE]);
            maxSliceNumber = m_SliceExtractor[SAGITAL_IMAGE]->GetMaxSliceNumber();
            viewSets->SagitalSlice = ClampValue(viewSets->SagitalSlice, 0, maxSliceNumber);
            m_SliceExtractor[SAGITAL_IMAGE]->SetSlice(viewSets->SagitalSlice);

            m_SliceExtractor[AXIAL_IMAGE]->SetOrientation(g_Orientation[AXIAL_IMAGE]);
            maxSliceNumber = m_SliceExtractor[AXIAL_IMAGE]->GetMaxSliceNumber();
            viewSets->AxialSlice = ClampValue(viewSets->AxialSlice, 0, maxSliceNumber);
            m_SliceExtractor[AXIAL_IMAGE]->SetSlice(viewSets->AxialSlice);
                    
            // Connect output to layered image
            for(int i=0; i < ORTHO_IMAGE_COUNT; ++i){
                if(m_SliceExtractor[i]->GetInput()){
		            m_SliceExtractor[i]->Update();
                    m_LayeredImage[i]->SetInput(m_SliceExtractor[i]->GetOutput());
                }
            }
        }
    case FRP_COLORMAP:
        {
            // Update here colormap values
            for(int i=0; i < ORTHO_IMAGE_COUNT; ++i){
                if(viewSets->ActiveLayerID == ALL_LAYERS_ID){
                    it = layers[i]->begin();
                    itEnd = layers[i]->end();
                    for(; it != itEnd; ++it){
                        m_LayeredImage[i]->SetColormapSettings(
                            (*it)->ColormapSettings, (*it)->ID);
                    }
                }
                else {
                    m_LayeredImage[i]->SetColormapSettings(
                        layer[i]->ColormapSettings, layer[i]->ID);
                }
                m_LayeredImage[i]->UpdateColormap();
            }
        }
    case FRP_TBC:
        {
            // set TBC values
            for(int i=0; i < ORTHO_IMAGE_COUNT; ++i){
                if(viewSets->ActiveLayerID == ALL_LAYERS_ID){
                    it = layers[i]->begin();
                    itEnd = layers[i]->end();
                    for(; it != itEnd; ++it){
                        m_LayeredImage[i]->SetTBCSettings(
                            (*it)->TbcSettings, (*it)->ID);
                    }
                }
                else {
                    m_LayeredImage[i]->SetTBCSettings(
                        layer[i]->TbcSettings, layer[i]->ID);
                }
                m_LayeredImage[i]->UpdateTBC();
            }
        }
    case FRP_OPACITY_VISIBILITY:
        {
            for(int i=0; i < ORTHO_IMAGE_COUNT; ++i){
                if(viewSets->ActiveLayerID == ALL_LAYERS_ID){
                    it = layers[i]->begin();
                    itEnd = layers[i]->end();
                    for(; it != itEnd; ++it){
                        m_LayeredImage[i]->SetOpacity((*it)->Opacity, (*it)->ID);
                        m_LayeredImage[i]->SetVisibility((*it)->Visibility, (*it)->ID);
                    }
                }
                else {
                    m_LayeredImage[i]->SetOpacity(layer[i]->Opacity, layer[i]->ID);
                    m_LayeredImage[i]->SetVisibility(layer[i]->Visibility, layer[i]->ID);
                }
            }
        }
    case FRP_SETCAM:
        {
            for(int i=0; i < ORTHO_IMAGE_COUNT; ++i){
                m_LayeredImage[i]->SetCameraSettings(
                    viewSets->CamSettings[i], ALL_LAYERS_ID);	
                m_LayeredImage[i]->UpdateCamera();
            }
        }
    default:
        // do nothing
        break;
    }

    // redraw scene
	GetRenderWindow()->Render();
}


FrLayerSettings* FrOrthoView::GetLayerAndInitLayers(
                                           std::vector<FrLayerSettings*>& layers, 
                                           FrOrthoViewSettings* viewSets, 
                                           int rendererID){
    
    int layerID = viewSets->ActiveLayerID;
    GetLayerSettings(viewSets, layers, rendererID);
    LayerCollection::iterator it, itEnd(layers.end());    

    // If not 'broadcast update' then get layer settings 
    FrLayerSettings* layer = 0L;
    if(layerID != ALL_LAYERS_ID){
        for(it = layers.begin(); it != itEnd; ++it){
            if((*it)->ID == layerID){
                layer = (*it);
                break;
            }
        }
    }
    return layer;
}