#include "FrSliceView.h"
#include "QVTKWidget.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrInteractorStyle.h"
#include "FrDocumentReader.h"
#include "FrLayeredImage.h"
#include "FrLayerDocObj.h"
#include "FrViewDocObj.h"
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
#include "vtkConfigure.h"
#include "vtkToolkits.h"


// FrUpdateParams represents params 
class FrUpdateParams0 {
public:
    FrMainDocument*      Document;
    FrSliceViewSettings* ViewSettings;
    typedef std::vector<FrLayerDocObj*> LayerCollection;
    LayerCollection Layers;
};


// Default constructor
FrSliceView::FrSliceView(FrMainWindow* mainWindow)
: FrBaseView(mainWindow) {
    	
    // Create pipeline stuff
    m_docReader = FrDocumentReader::New();
    m_LayeredImage = FrLayeredImage::New();
}

FrSliceView::~FrSliceView(){
    this->RemoveRenderers();
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
            FrUpdateParams0::LayerCollection::iterator it,
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

            //m_LayeredImage->UpdateCamera();
        }
    default:
        // do nothing
        break;
    }

    // Write text for slice number
    int timePoint = this->GetMainWindow()->
        GetMainDocument()->
        GetCurrentViewObject()->
        GetTimePoint();
    if(timePoint < 0) 
        timePoint = -1;

	char sliceNumText[32] = "";
    sprintf(sliceNumText, "Slice %d, TP %d", 
        params.ViewSettings->SliceNumber, timePoint);

    m_LayeredImage->SetText(sliceNumText);

    // redraw scene
    vtkRenderWindow* renWin = GetRenderWindow();
    if (renWin) renWin->Render();
    
    this->ResetCurrentContext(renWin);
}

bool FrSliceView::InitUpdateParams(FrUpdateParams0& params){
    // Init params used when updating
    params.Document = this->GetMainWindow()->GetMainDocument();

    // Get view settings
    if(!params.Document->GetCurrentViewObject()) return false;
    params.ViewSettings = params.Document->
        GetCurrentViewObject()->GetSliceViewSettings();

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

void FrSliceView::ReadDocument(FrUpdateParams0& params){
    FrViewDocObj* vdo = params.Document->GetCurrentViewObject();

    m_docReader->SetDocument(params.Document);
    m_docReader->SetMosaic(false);
    m_docReader->SetOrientation(FrDocumentReader::XY);
    m_docReader->SetSlice(params.ViewSettings->SliceNumber);
    m_docReader->SetTimeSeries(vdo->GetTimeSeries());

    // update layers
    FrUpdateParams0::LayerCollection::iterator it, itEnd(params.Layers.end());

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
        // .. Add other layers
    }

    // get slice number from doc reader
    int Slice = m_docReader->GetSlice();
    
    // update slice number in View
    FrViewDocObj* viewDO = params.Document->GetCurrentViewObject(); 
    viewDO->GetSliceViewSettings()->SliceNumber = Slice;
    params.ViewSettings->SliceNumber = Slice;

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

void FrSliceView::UpdateColormap(FrUpdateParams0& params){
    FrUpdateParams0::LayerCollection::iterator it,
        itEnd(params.Layers.end());

    for(it = params.Layers.begin(); it != itEnd; ++it){
        if((*it)->IsColormap()){
            FrColormapLayerSettings* cmls = (FrColormapLayerSettings*)(*it)->GetSettings();
            m_LayeredImage->SetColormapSettings(cmls->ColormapSettings, (*it)->GetID());
        }
    }
    m_LayeredImage->UpdateColormap();
}

void FrSliceView::UpdateTbc(FrUpdateParams0& params){
     FrUpdateParams0::LayerCollection::iterator it,
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
