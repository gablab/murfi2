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
};


// Default constructor
FrSliceView::FrSliceView(FrMainWindow* mainWindow)
: FrBaseView(mainWindow) {
    	
    // Create pipeline stuff
    m_docReader = FrDocumentReader::New();
	m_SliceExtractor = FrSliceExtractor::New();
    m_LayeredImage = FrLayeredImage::New();
}

FrSliceView::~FrSliceView(){
    this->RemoveRenderers();
	if(m_SliceExtractor) m_SliceExtractor->Delete();
    if(m_docReader) m_docReader->Delete();
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
    InitUpdateParams(params);
                    	
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

    // If not 'broadcast update' then get layer settings 
    if(params.ActiveLayerID != ALL_LAYERS_ID){
        // Set default value
        params.ActiveLayer = 0L;

        // Look for layer settings
        LayerCollection::iterator it, itEnd(params.Layers.end());
        for(it = params.Layers.begin(); it != itEnd; ++it){
            if((*it)->ID == params.ActiveLayerID){
                params.ActiveLayer = (*it);
                break;
            }
        }
    }
}

void FrSliceView::ReadDocument(FrUpdateParams0& params){
    // Read document and connect filters
    m_docReader->SetDocument(params.Document);
    m_docReader->SetUnMosaicOn();
    m_docReader->Update();
	
    // Pass image Set image input
    m_SliceExtractor->SetInput(m_docReader->GetOutput());

    // Pass ROI data from output starting from 1
    int count = m_docReader->GetOutputCount();
    for(int i=1; i < count; ++i){
        m_SliceExtractor->SetInput(i, m_docReader->GetOutput(i));
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
        int count = m_docReader->GetOutputCount();
        for(int i=1; i < count; ++i){
            m_LayeredImage->SetROIInput(i, m_SliceExtractor->GetOutput(i));
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
    else {
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
    else {
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
    else {
        m_LayeredImage->SetOpacity(
            params.ActiveLayer->Opacity, params.ActiveLayer->ID);
        m_LayeredImage->SetVisibility(
            params.ActiveLayer->Visibility, params.ActiveLayer->ID);
    }

    // TODO: add ROI opacity and visibility update ???
}
