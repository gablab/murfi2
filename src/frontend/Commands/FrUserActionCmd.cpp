#include "FrUserActionCmd.h"
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
#include "FrGraphDocObj.h"
#include "FrColormapWidget.h"
#include "FrImageSettingsWidget.h"
#include "FrPointsDocObj.h"

#include "FrBaseView.h"
#include "FrSliceView.h"
#include "FrMosaicView.h"
#include "FrOrthoView.h"
#include "FrTBCFilter.h"
#include "FrSliceExtractor.h"

#include "Qt/qmessagebox.h"


#define ALL_ITEMS_COUNT 5

FrUserActionCmd::FrUserActionCmd() 
: m_Action(FrUserActionCmd::Undefined), m_isID(false){
}

bool FrUserActionCmd::Execute(){
    // Check for safety
    if(!this->GetMainController()) return false;

    bool result = false;
    switch(m_Action){
        case FrUserActionCmd::AddLayer: result = addLayer();
            break;
        case FrUserActionCmd::DeleteLayer: result = deleteLayer();
            break;
        case FrUserActionCmd::AddGraph: result = addGraph();
            break;
        case FrUserActionCmd::DeleteGraph: result = deleteGraph();
            break;
        case FrUserActionCmd::ChangeSettings: result = ChangeImageSettings();
            break;
        case FrUserActionCmd::Undefined:
        default:
            // Do nothing here
            break;
    }
    return result;
}

bool FrUserActionCmd::addLayer(){
    bool result = false;
    
    // create new layer doc object and get settings from dialog
    FrLayerDocObj* layerDO = new FrLayerDocObj(FrLayerSettings::LColormap);

    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrLayerDialog dlg(mv, true);
    if(!dlg.SimpleExec()) return false;
    FrColormapLayerSettings* cmlSets = new FrColormapLayerSettings();
    dlg.GetLayerParams(*cmlSets);

    layerDO->SetSettings(cmlSets);

    // finally add new doc to MainDocument
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    doc->Add(layerDO);
    result = true;

    return result;
}

bool FrUserActionCmd::deleteLayer(){
    // Get proper ID
    if(!m_isID){
        m_ID = GetActiveLayerID();
    }

    // if wrong ID is specified return
    // Default layer cannot be deleted
    if(m_ID < DEF_LAYER_ID) return false;
    if(m_ID == DEF_LAYER_ID){
        QMessageBox::critical(this->GetMainController()->GetMainView(), 
            "Delete Layer Command", "Can't delete default layer...");
        return false;
    }

    // find appropriate layer doc obj and remove it from main doc
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrDocument::DocObjCollection layers;
    doc->GetObjectsByType(layers, FrDocumentObj::LayerObject);    

    if(layers.size() > 0){
        for (int i = 0; i < layers.size(); i++){
            FrLayerDocObj* layerDO = dynamic_cast<FrLayerDocObj*>(layers[i]);
            if (layerDO->GetID() == m_ID){
                doc->Remove(layerDO);
                break;
            }
        }
    }  
    
    return true;
}

bool FrUserActionCmd::ChangeImageSettings(){
    // check what should be changed TBC values or slice number
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrMainWindow* mw = this->GetMainController()->GetMainView();

    FrViewDocObj* viewDO = doc->GetCurrentViewObject();
    // get current slice number
    int old_slice[3];
    int new_slice[3];
    FrLayerSettings* ls;
    FrTbcSettings old_tbc;
    FrTbcSettings new_tbc;
    bool roi = true;

    // get tbc settings from current layer
    int id = GetActiveLayerID();
    FrLayerDocObj* layerDO = doc->GetLayerDocObjByID(id);

    if (!layerDO)
        return false;

    if (!layerDO->IsRoi()){
        roi = false;
        ls = layerDO->GetSettings();

        if (layerDO->IsColormap()){
            old_tbc = ((FrColormapLayerSettings*)ls)->TbcSettings;
        }
        else if (layerDO->IsImage()){
            old_tbc = ((FrImageLayerSettings*)ls)->TbcSettings;
        }

        // get tbc settings from widget
        new_tbc = mw->GetImageSettingsWidget()->GetTbcSettings();

        // check if tbc settings changed
        if (old_tbc.Brightness != new_tbc.Brightness || old_tbc.Contrast != new_tbc.Contrast ||
            old_tbc.Threshold != new_tbc.Threshold){
            // set new tbc settings
            old_tbc.Brightness = new_tbc.Brightness;
            old_tbc.Contrast = new_tbc.Contrast;
            old_tbc.Threshold = new_tbc.Threshold;
            
            if (layerDO->IsColormap()){
                ((FrColormapLayerSettings*)ls)->TbcSettings.Brightness = new_tbc.Brightness;
                ((FrColormapLayerSettings*)ls)->TbcSettings.Contrast = new_tbc.Contrast;
                ((FrColormapLayerSettings*)ls)->TbcSettings.Threshold = new_tbc.Threshold;
            }
            else if (layerDO->IsImage()){
                ((FrImageLayerSettings*)ls)->TbcSettings.Brightness = new_tbc.Brightness;
                ((FrImageLayerSettings*)ls)->TbcSettings.Contrast = new_tbc.Contrast;
                ((FrImageLayerSettings*)ls)->TbcSettings.Threshold = new_tbc.Threshold;
            }

            layerDO->SetSettings(ls);

            FrBaseCmd::UpdatePipelineForID(ALL_LAYER_ID, FRP_TBC);
        }
    }

    Views view = viewDO->GetActiveView();
    switch(view){
        case SliceView:
            old_slice[0] = viewDO->GetSliceViewSettings()->SliceNumber;
            new_slice[0] = mw->GetImageSettingsWidget()->GetSliceNumber(3);

            // check if slice number changed
            if (old_slice[0] != new_slice[0]){
                // set new slice number
                viewDO->GetSliceViewSettings()->SliceNumber = new_slice[0];

                FrBaseCmd::UpdatePipelineForID(ALL_LAYER_ID, FRP_READ);
            }
            break;
        case MosaicView:
            // do nothing here
            break;
        case OrthoView:
            for(int i=0; i < ORTHO_VIEWS_CNT; ++i){
                old_slice[i] = viewDO->GetOrthoViewSettings()->SliceNumber[i];
                new_slice[i] = mw->GetImageSettingsWidget()->GetSliceNumber(i);
            }
            if (old_slice[0] != new_slice[0] || old_slice[1] != new_slice[1] || old_slice[2] != new_slice[2]){
                // set new slice number
                viewDO->GetOrthoViewSettings()->SliceNumber[0] = new_slice[0];
                viewDO->GetOrthoViewSettings()->SliceNumber[1] = new_slice[1];
                viewDO->GetOrthoViewSettings()->SliceNumber[2] = new_slice[2];
    
                FrBaseCmd::UpdatePipelineForID(ALL_LAYER_ID, FRP_READ);
            }
            break;
    }
    
    return true;
}

bool FrUserActionCmd::addGraph(){
    bool result = false;
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();

    // TODO: get graph type by id


    // do not add graph if don't have a point
    // get point from points doc obj
    FrPointsDocObj* pointsDO = 0L;
    FrDocument::DocObjCollection pointObjects;
    doc->GetObjectsByType(pointObjects, FrDocumentObj::PointsObject);    

    if(pointObjects.size() == 0)
        return false;

    pointsDO = (FrPointsDocObj*)pointObjects[0];
    int point[3];
    
    pointsDO->GetPoint(point);
    if (point[0] == -1)
        return false;

    // create new graph doc object and get settings from anywhere (widget, dialog)
    FrGraphDocObj* graphDO = new FrGraphDocObj(FrGraphSettings::GT_Intencity);  // set correct graph type
    graphDO->SetTimeSeria(m_GraphID);
    
    // prepare settings (NOTE: different settings for different types of graphs)
    FrIntencityGraphSettings* gs = new FrIntencityGraphSettings();

    gs->Color = QColor(0, 0, 0);
    gs->I = point[0];
    gs->J = point[1];
    gs->K = point[2];
    
    graphDO->SetSettings(gs);

    // finally add new doc to MainDocument
    doc->Add(graphDO);
    result = true;

    return result;
}

bool FrUserActionCmd::deleteGraph(){
    // find appropriate graph doc obj and remove it from main doc
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrDocument::DocObjCollection graphs;
    doc->GetObjectsByType(graphs, FrDocumentObj::GraphObject);    

    if(graphs.size() > 0){
        for (int i = 0; i < graphs.size(); i++){
            FrGraphDocObj* graphDO = dynamic_cast<FrGraphDocObj*>(graphs[i]);
            if (graphDO->GetTimeSeria() == m_GraphID){
                doc->Remove(graphDO);
                break;
            }
        }
    }  
    
    return true;
}

// delete active layer
int FrUserActionCmd::GetActiveLayerID(){
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrViewDocObj* viewDO = doc->GetCurrentViewObject();
    return viewDO->GetActiveLayerID();
}

//FrLayerDocObj* FrUserActionCmd::GetLayerDocObjByID(int id){
//    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
//
//    FrLayerDocObj* layerDO = 0L;
//    FrDocument::DocObjCollection layers;
//    doc->GetObjectsByType(layers, FrDocumentObj::LayerObject);    
//
//    if(layers.size() > 0){
//        for (int i = 0; i < layers.size(); i++){
//            layerDO = dynamic_cast<FrLayerDocObj*>(layers[i]);
//            if (layerDO->GetID() == id)
//                return layerDO;
//        }
//    }
//
//    return 0L;
//}

//bool FrUserActionCmd::IsRoiLayer(int id){
//    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
//    
//    std::vector<FrDocumentObj*> objects;
//    doc->GetObjectsByType(objects, FrDocumentObj::RoiObject);
//
//    // Iterate through all found rois
//    std::vector<FrDocumentObj*>::iterator itr, itrEnd(objects.end());
//    for(itr = objects.begin(); itr != itrEnd; ++itr){
//        FrRoiDocObj* roiDO = (FrRoiDocObj*)(*itr);
//        if(id == roiDO->GetID()) return true;
//    }
//    return false;
//}
///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrUserActionCmd::CanUndo(){
    return false;
}

bool FrUserActionCmd::Undo(){
    return false;
}

bool FrUserActionCmd::Redo(){
    return false;
}
