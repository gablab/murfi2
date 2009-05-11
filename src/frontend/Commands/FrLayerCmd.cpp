#include "FrLayerCmd.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrLayerDialog.h"
#include "FrLayerListWidget.h"
#include "FrTabSettingsDocObj.h"
#include "FrLayeredImage.h"
#include "FrUtils.h"
#include "FrRoiDocObj.h"
#include "FrLayerSettings.h"
#include "FrLayerDocObj.h"
#include "FrViewDocObj.h"
#include "FrColormapWidget.h"

#include "FrBaseView.h"
#include "FrSliceView.h"
#include "FrMosaicView.h"
#include "FrOrthoView.h"
#include "FrTBCFilter.h"
#include "FrSliceExtractor.h"

#include "Qt/qmessagebox.h"


#define ALL_ITEMS_COUNT 5

FrLayerCmd::FrLayerCmd() 
: m_Action(FrLayerCmd::Undefined), 
  m_DocObj(0), m_isID(false){
}

void FrLayerCmd::SetID(unsigned long id){
    m_ID = id;
    m_isID = true;
}

bool FrLayerCmd::Execute(){
    // Check for safety
    if(!this->GetMainController()) return false;

    bool result = false;
    switch(m_Action){
        case FrLayerCmd::Add: result = AddLayer();
            break;
        case FrLayerCmd::Delete: result = DeleteLayer();
            break;
        case FrLayerCmd::ChangeOld: result = ChangeLayerOld();
            break;
        case FrLayerCmd::ChangeParams: result = ChangeLayerParams();
            break;
        case FrLayerCmd::ChangeColormap: result = ChangeLayerColormap();
            break;
        case FrLayerCmd::UpdateSelectedID: result = UpdateSelectedLayerID();
            break;
        case FrLayerCmd::ChangePosition: result = ChangeLayerPosition();
            break;
        case FrLayerCmd::Undefined:
        default:
            // Do nothing here
            break;
    }
    return result;
}

bool FrLayerCmd::AddLayer(){
    if(m_DocObj == 0) return false;

    // Init data
    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrLayeredImage* layeredImage[ALL_ITEMS_COUNT];
    layeredImage[0] = mv->GetSliceView()->GetImage();
    layeredImage[1] = mv->GetMosaicView()->GetImage();
    layeredImage[2] = mv->GetOrthoView()->GetImage(DEF_CORONAL);
    layeredImage[3] = mv->GetOrthoView()->GetImage(DEF_SAGITAL);
    layeredImage[4] = mv->GetOrthoView()->GetImage(DEF_AXIAL);

    bool result = false;
    if(m_DocObj->IsImage()){
        for(int i=0; i < ALL_ITEMS_COUNT; ++i){
            // Do we need copy params here???
            layeredImage[i]->AddLayer(m_DocObj->GetID(), FrLayeredImage::Image);
        }
        
	// ohinds 2009-02-27
	// there is no image data yet when we are adding the layer
	//FrBaseCmd::UpdatePipelineForID(ALL_LAYER_ID, FRP_READ);
        result = true;
    }
    else if(m_DocObj->IsRoi()){
        for(int i=0; i < ALL_ITEMS_COUNT; ++i){
            // Do we need to copy values ???
            layeredImage[i]->AddLayer(m_DocObj->GetID(), FrLayeredImage::Roi);
        }
	// ohinds 2009-02-27
	// there is no image data yet when we are adding the layer
        //FrBaseCmd::UpdatePipelineForID(ALL_LAYER_ID, FRP_READ);
        result = true;
    }
    
    // set new layer as active
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrViewDocObj* viewDO = doc->GetCurrentViewObject();
    
    unsigned long id = m_DocObj->GetID();
    //cout << "layer doc id " << id << " addr " << viewDO << endl;
    viewDO->GetSliceViewSettings()->SetActiveLayerID(id);
    viewDO->GetMosaicViewSettings()->SetActiveLayerID(id);
    viewDO->GetOrthoViewSettings()->SetActiveLayerID(id);

    // set default view

    // set layers order
    SetLayersOrder();
    FrBaseCmd::UpdatePipelineForID(ALL_LAYER_ID, FRP_READ);     // scopic Alan 07.05.09: without this command display will not be updated to show new layer

    return result;
}

bool FrLayerCmd::DeleteLayer(){
    if(m_DocObj == 0) return false;

    // Init data
    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrLayeredImage* layeredImage[ALL_ITEMS_COUNT];
    layeredImage[0] = mv->GetSliceView()->GetImage();
    layeredImage[1] = mv->GetMosaicView()->GetImage();
    layeredImage[2] = mv->GetOrthoView()->GetImage(DEF_CORONAL);
    layeredImage[3] = mv->GetOrthoView()->GetImage(DEF_SAGITAL);
    layeredImage[4] = mv->GetOrthoView()->GetImage(DEF_AXIAL);

    // next id to focus
    unsigned long nextID = layeredImage[0]->GetNextLayerID(m_DocObj->GetID());

    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrViewDocObj* viewDO = doc->GetCurrentViewObject();

    // delete layer from LayeredImage
    for(int i=0; i < ALL_ITEMS_COUNT; ++i){
        layeredImage[i]->RemoveLayer(m_DocObj->GetID());
    }

    // Assume that there is good sync between layers 
    // ohinds 2009-02-25
    // find next layer id
    
    viewDO->GetSliceViewSettings()->SetActiveLayerID(nextID);
    viewDO->GetMosaicViewSettings()->SetActiveLayerID(nextID);
    viewDO->GetOrthoViewSettings()->SetActiveLayerID(nextID);


    // ohinds 2009-02-25
    // need to make sure this is ok, might cause crash on update when deleting?
    if (nextID != BAD_LAYER_ID)
      FrBaseCmd::UpdatePipelineForID(nextID, FRP_COLORMAP);

    // set layers order
    SetLayersOrder();

    return true;
}

bool FrLayerCmd::UpdateSelectedLayerID(){
    if(!m_isID || m_ID <= BAD_LAYER_ID) return false;
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrViewDocObj* viewDO = doc->GetCurrentViewObject();

    // update selected layer settings on LayerListWidget's right panel
    viewDO->GetSliceViewSettings()->SetActiveLayerID(m_ID);
    viewDO->GetMosaicViewSettings()->SetActiveLayerID(m_ID);
    viewDO->GetOrthoViewSettings()->SetActiveLayerID(m_ID);
    return true;
}

// change Colormap params with dialog (only for colormap layers)
bool FrLayerCmd::ChangeLayerOld(){
    // Get proper ID
    if(!m_isID){
        m_ID = this->GetActiveLayerID();
    }
    // if wrong ID is specified return
    if(m_ID <= BAD_LAYER_ID) return false;

    // Init data
    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();    

    // find layer settings
    // NOTE: assume that layers in different views 
    // but with the same ID are the same
    FrDocument::DocObjCollection layers;
    doc->GetObjectsByType(layers, FrDocumentObj::LayerObject);    

    FrLayerDocObj* layerDO = 0L;
    if(layers.size() > 0){
        // get appropriate LayerDocObj
        for (int i = 0; i < layers.size(); i++){
            layerDO = dynamic_cast<FrLayerDocObj*>(layers[i]);
            if (layerDO->GetID() == m_ID) 
                break;
            else 
                layerDO = 0L;
        }
    }    

    // Update layer params
    bool result = false;
    if(layerDO != 0L && !layerDO->IsImage()){
        FrLayerDialog dlg(mv, true);
        dlg.SetLayerParams(*((FrImageLayerSettings*)layerDO->GetSettings()));

        if(dlg.SimpleExec()){
            // NOTE: Need to save ID and TBC since they may be changed
            FrImageLayerSettings* cmlParams = (FrImageLayerSettings*)layerDO->GetSettings();
            FrTbcSettings saveTbc = cmlParams->TbcSettings;
            dlg.GetLayerParams(*cmlParams);
            cmlParams->TbcSettings = saveTbc;
            layerDO->SetSettings(cmlParams);

            result = true;
        }
        //this->UpdateSelectedLayerID();
    
        FrBaseCmd::UpdatePipelineForID(m_ID, FRP_COLORMAP);
    }
    return result;
}

bool FrLayerCmd::ChangeLayerParams(){
    // Get proper ID
    if(!m_isID){
        m_ID = this->GetActiveLayerID();
    }

    FrMainWindow* mw = this->GetMainController()->GetMainView();
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();

    // find appropriate layer doc obj
    FrDocument::DocObjCollection layers;
    doc->GetObjectsByType(layers, FrDocumentObj::LayerObject);    

    FrLayerDocObj* layerDO = 0L;
    if(layers.size() > 0){
        // get appropriate LayerDocObj
        for (int i = 0; i < layers.size(); i++){
            layerDO = (FrLayerDocObj*)layers[i];
            //layerDO = dynamic_cast<FrLayerDocObj*>(layers[i]);
            if (layerDO->GetID() == m_ID) 
                break;
            else 
                layerDO = 0L;
        }
    }    

    int opacity = mw->GetLayerListWidget()->GetOpacity();
    FrLayerSettings* params = layerDO->GetSettings();           
    params->Opacity = opacity/100.0;
    
    bool visibility = mw->GetLayerListWidget()->GetLayerVisibility(m_ID);
    params->Visibility = visibility;

    layerDO->SetSettings(params);

    FrBaseCmd::UpdatePipelineForID(m_ID, FRP_OPACITY_VISIBILITY);    
    return true;
}

bool FrLayerCmd::ChangeLayerColormap(){
    // Get proper ID
    if(!m_isID){
        m_ID = this->GetActiveLayerID();
    }

    // Init data
    FrMainWindow* mw = this->GetMainController()->GetMainView();
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();

    // find appropriate layer doc obj
    FrDocument::DocObjCollection layers;
    doc->GetObjectsByType(layers, FrDocumentObj::LayerObject);    

    FrLayerDocObj* layerDO = 0L;
    if(layers.size() > 0){
        // delete appropriate LayerDocObj
        for (int i = 0; i < layers.size(); i++){
            layerDO = dynamic_cast<FrLayerDocObj*>(layers[i]);
            if (layerDO->GetID() == m_ID) break;
        }
    }    

    if (layerDO->IsImage()){
        // get colormap params and opacity
        int opacity = mw->GetLayerListWidget()->GetOpacity();
        FrImageLayerSettings* imageParams = (FrImageLayerSettings*)layerDO->GetSettings();
        imageParams->Opacity = opacity;

        bool visibility = mw->GetLayerListWidget()->GetLayerVisibility(m_ID);
        imageParams->Visibility = visibility;

        mw->GetLayerListWidget()->GetColormapWidget()->GetColormapParams(*imageParams);
        
        layerDO->SetSettings(imageParams);
    }
    else
        return false;   // trying to update colormap params of non colormap layer

    FrBaseCmd::UpdatePipelineForID(m_ID, FRP_COLORMAP);
    return true;
}

bool FrLayerCmd::ChangeLayerPosition(){
    if (m_Increment == 0) return false;

    // Get proper ID
    if(!m_isID){
        m_ID = this->GetActiveLayerID();
    }

    // Init data
    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();

    FrLayeredImage* layeredImage[ALL_ITEMS_COUNT];
    layeredImage[0] = mv->GetSliceView()->GetImage();
    layeredImage[1] = mv->GetMosaicView()->GetImage();
    layeredImage[2] = mv->GetOrthoView()->GetImage(DEF_CORONAL);
    layeredImage[3] = mv->GetOrthoView()->GetImage(DEF_SAGITAL);
    layeredImage[4] = mv->GetOrthoView()->GetImage(DEF_AXIAL);
    
    bool result = false;
    // change layer position for all layered images
    for(int i=0; i < ALL_ITEMS_COUNT; ++i){
        layeredImage[i]->ChangeLayerPosition(m_ID, m_Increment);
    }
    
    FrBaseCmd::UpdatePipelineForID(ALL_LAYER_ID, FRP_READ);   
    result = true;
    
    // set layers order
    SetLayersOrder();

    return result;
}


// delete active layer
unsigned long FrLayerCmd::GetActiveLayerID(){
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrViewDocObj* viewDO = doc->GetCurrentViewObject();
    return viewDO->GetActiveLayerID();
}

bool FrLayerCmd::IsRoiLayer(unsigned long id){
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    
    std::vector<FrDocumentObj*> objects;
    doc->GetObjectsByType(objects, FrDocumentObj::RoiObject);

    // Iterate through all found rois
    std::vector<FrDocumentObj*>::iterator itr, itrEnd(objects.end());
    for(itr = objects.begin(); itr != itrEnd; ++itr){
        FrRoiDocObj* roiDO = (FrRoiDocObj*)(*itr);
        if(id == roiDO->GetID()) return true;
    }
    return false;
}

void FrLayerCmd::SetLayersOrder(){
    // init data
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrMainWindow* mv = this->GetMainController()->GetMainView();

    FrLayeredImage* layeredImage;
    
    switch (doc->GetCurrentViewObject()->GetActiveView()){
            case SliceView:
                layeredImage = mv->GetSliceView()->GetImage();
                break;
            case MosaicView:
                layeredImage = mv->GetMosaicView()->GetImage();
                break;
            case OrthoView:
                layeredImage = mv->GetOrthoView()->GetImage(DEF_CORONAL);
                break;
    }

    // find appropriate layer doc obj
    FrDocument::DocObjCollection layers;
    doc->GetObjectsByType(layers, FrDocumentObj::LayerObject); 

    FrLayerDocObj* layerDO = 0L;
    if(layers.size() > 0){
        // find appropriate Layer in layered image
        for (int i = 0; i < layers.size(); i++){
            layerDO = dynamic_cast<FrLayerDocObj*>(layers[i]);
            int id = layerDO->GetID();
            int pos = layeredImage->GetLayerByID(id)->GetPosition();
            layerDO->SetPosition(pos);
        }
    }   
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrLayerCmd::CanUndo(){
    return false;
}

bool FrLayerCmd::Undo(){
    return false;
}

bool FrLayerCmd::Redo(){
    return false;
}
