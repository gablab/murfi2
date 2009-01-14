#include "FrOrthoView.h"
#include "FrMainWindow.h"
#include "QVTKWidget.h"
#include "FrDocumentReader.h"
#include "FrMainDocument.h"
#include "FrNotify.h"
#include "FrUtils.h"
#include "FrLayeredImage.h"
#include "FrViewDocObj.h"
#include "FrLayerDocObj.h"

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
    FrOrthoViewSettings* ViewSettings;
    typedef std::vector<FrLayerDocObj*> LayerCollection;

    LayerCollection Layers;
};

FrDocumentReader::Orientations g_Orientation[ORTHO_VIEWS_CNT];

// Default constructor
FrOrthoView::FrOrthoView(FrMainWindow* mainWindow)
: FrBaseView(mainWindow) {
    // creating reader
    m_docReader = FrDocumentReader::New();

    // Init orientations
    g_Orientation[DEF_CORONAL] = FrDocumentReader::XZ;
    g_Orientation[DEF_SAGITAL] = FrDocumentReader::YZ;
    g_Orientation[DEF_AXIAL]   = FrDocumentReader::XY;

    // create renderers
	for(int i = 0; i < ORTHO_VIEWS_CNT; i++){
        m_LayeredImage[i] = FrLayeredImage::New();
    }
    m_dummyRenderer = vtkRenderer::New();
    m_dummyRenderer->SetBackground(0.0, 0.0, 0.0);
    m_dummyRenderer->GetActiveCamera()->ParallelProjectionOn();
}

FrOrthoView::~FrOrthoView(){
    this->RemoveRenderers();
    if(m_docReader) m_docReader->Delete();
    
    for(int i = 0; i < ORTHO_VIEWS_CNT; ++i){
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
    for(int i=0; i < ORTHO_VIEWS_CNT; ++i){
        renderers.clear();
        m_LayeredImage[i]->GetRenderers(renderers);
        layersNum = std::max(layersNum, int(renderers.size()));

        // Add renderers and adjust viewport
        std::vector<vtkRenderer*>::iterator it, itEnd(renderers.end());
        for(it = renderers.begin(); it != itEnd; ++it){
            renWin->AddRenderer( (*it) );
            if(i == DEF_CORONAL){
                (*it)->SetViewport(0.0, 0.5, 0.5, 1.0);
            }
            else if(i == DEF_SAGITAL){
                (*it)->SetViewport(0.5, 0.5, 1.0, 1.0);
            }
            else if(i == DEF_AXIAL){
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
    for(int i=0; i < ORTHO_VIEWS_CNT; ++i){
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
    // Get common settings
    FrUpdateParams2 params;
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
            for(int i=0; i < ORTHO_VIEWS_CNT; ++i){
                FrUpdateParams2::LayerCollection::iterator it,
                    itEnd(params.Layers.end());

                for(it = params.Layers.begin(); it != itEnd; ++it){
                    unsigned int id = (*it)->GetID();
                    m_LayeredImage[i]->SetOpacity((*it)->GetOpacity(), id);
                    m_LayeredImage[i]->SetVisibility((*it)->GetVisibility(), id); 
                }
            }
        }
    case FRP_SETCAM:
        {
            for(int i=0; i < ORTHO_VIEWS_CNT; ++i){
                m_LayeredImage[i]->SetCameraSettings(
                    params.ViewSettings->CamSettings[i], 
                    ALL_LAYER_ID);

                //m_LayeredImage[i]->UpdateCamera();
            }
        }
    default:
        // do nothing
        break;
    }

    // Draw border
    const char* text[] = { "Coronal XZ", "Sagital YZ", "Axial XY" };
    for(int i=0; i < ORTHO_VIEWS_CNT; ++i){
        m_LayeredImage[i]->UpdateBorder();
        m_LayeredImage[i]->SetText(text[i]);
    }

    // redraw scene
	GetRenderWindow()->Render();
    wglMakeCurrent(NULL, NULL);
}

bool FrOrthoView::InitUpdateParams(FrUpdateParams2& params){
    // Init params used when updating
    params.Document = this->GetMainWindow()->GetMainDocument();

    // Get view settings
    if(!params.Document->GetCurrentViewObject()) return false;
    params.ViewSettings = params.Document->
        GetCurrentViewObject()->GetOrthoViewSettings();

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

void FrOrthoView::ReadDocument(FrUpdateParams2& params){
    FrViewDocObj* vdo = params.Document->GetCurrentViewObject();

    m_docReader->SetDocument(params.Document);
    m_docReader->SetMosaic(false);
    m_docReader->SetTimeSeries(vdo->GetTimeSeries());
    
    FrUpdateParams2::LayerCollection::iterator it,
        itEnd(params.Layers.end());

    for(int i=0; i < ORTHO_VIEWS_CNT; ++i){
        m_docReader->SetOrientation(g_Orientation[i]);
        m_docReader->SetSlice(params.ViewSettings->SliceNumber[i]);

        for(it = params.Layers.begin(); it != itEnd; ++it){
            if((*it)->IsRoi()){
                m_docReader->SetTarget(FrDocumentReader::Roi);
                m_docReader->SetDataID((*it)->GetID());
                m_docReader->Update();
                m_LayeredImage[i]->SetRoiInput(m_docReader->GetOutput(),
                                              (*it)->GetID());
            }
            else if((*it)->IsImage()){  
                // ID is current Timepoint since we have 
                // just one time series 
                m_docReader->SetTarget(FrDocumentReader::Mri);
                m_docReader->SetDataID(vdo->GetTimePoint());
                m_docReader->Update();

                m_LayeredImage[i]->SetImageInput(m_docReader->GetOutput());
            }
            // .. Add other layers
        }

        // set input to special layer here
        m_docReader->SetTarget(FrDocumentReader::Points);
        m_docReader->Update();

        // set params
        SelectionParams Params;
        Params.type = 4;

        FrSpecialLayer* sl = m_LayeredImage[i]->GetSpecialLayer();
        sl->SetSelectionData(m_docReader->GetOutput());
        sl->SetSelection(Params);
        sl->SetSelectionVisibility(true);
    }
}

void FrOrthoView::UpdateColormap(FrUpdateParams2& params){
    FrUpdateParams2::LayerCollection::iterator it,
        itEnd(params.Layers.end());

    for(int i=0; i < ORTHO_VIEWS_CNT; ++i){
        // Update layers for each image
        for(it = params.Layers.begin(); it != itEnd; ++it){
            // Colormap layers only
            if((*it)->IsColormap()){
                FrColormapLayerSettings* cmls = 
                    (FrColormapLayerSettings*)(*it)->GetSettings();

                m_LayeredImage[i]->SetColormapSettings(
                    cmls->ColormapSettings, (*it)->GetID());
            }
        }
        m_LayeredImage[i]->UpdateColormap();
    }
}

void FrOrthoView::UpdateTbc(FrUpdateParams2& params){
    FrUpdateParams2::LayerCollection::iterator it,
        itEnd(params.Layers.end());
    
    for(int i=0; i < ORTHO_VIEWS_CNT; ++i){
        // Update layers for each image
        for(it = params.Layers.begin(); it != itEnd; ++it){
            if((*it)->IsColormap()){
                FrColormapLayerSettings* cmls = 
                    (FrColormapLayerSettings*)(*it)->GetSettings();

                m_LayeredImage[i]->SetTbcSettings(
                    cmls->TbcSettings, (*it)->GetID());
            }
            else if((*it)->IsImage()){
                FrImageLayerSettings* ils = 
                    (FrImageLayerSettings*)(*it)->GetSettings();

                m_LayeredImage[i]->SetTbcSettings(
                    ils->TbcSettings, (*it)->GetID());
            }
        }
        m_LayeredImage[i]->UpdateTbc();
    }
}
