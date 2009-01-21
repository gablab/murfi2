#include "FrUpdateGraphTabsCmd.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrMainController.h"
#include "FrGraphBookmarkWidget.h"
#include "FrGraphPaneWidget.h"
#include "FrGraphTabDocObj.h"
#include "FrBaseView.h"
#include "FrSliceView.h"
#include "FrMosaicView.h"
#include "FrOrthoView.h"
#include "FrImageDocObj.h"
#include "RtMRIImage.h"

#define ITEMS_NUM  5


FrUpdateGraphTabsCmd::FrUpdateGraphTabsCmd()
: m_Action(FrUpdateGraphTabsCmd::None),
  m_GraphTabDocObj(0L), 
  m_TabID(BAD_TAB_ID) {

}

bool FrUpdateGraphTabsCmd::Execute(){
    // Check params
    if(!this->GetMainController() || m_Action == FrUpdateGraphTabsCmd::None) return false;

    bool result = false;
    if(m_Action == FrUpdateGraphTabsCmd::SetCurrentTab){
        result = this->SetupTab();
    }
    else {
        result = this->UpdateGraphs();
    }
    return result;
}

bool FrUpdateGraphTabsCmd::SetupTab(){
    // check params
    if(m_TabID == BAD_TAB_ID) return false;

    // Update current tab in document!
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    if(m_TabID != CURRENT_TAB_ID){
        // First save all settings to prev tab if any
        std::vector<FrDocumentObj*> docTabs;
        doc->GetObjectsByType(docTabs, FrDocumentObj::GraphTab);

        std::vector<FrDocumentObj*>::iterator it, itEnd(docTabs.end());
        for(it = docTabs.begin(); it != itEnd; ++it){
            FrGraphTabDocObj* tsDO = (FrGraphTabDocObj*)(*it);
            tsDO->SetIsCurrent( (tsDO->GetID() == m_TabID) );
        }
    }

    // Setup graph tab objects 
    //this->SetupLayeredImage();
    this->SetupTabObjects();
    FrBaseCmd::UpdatePipelineForID(ALL_LAYER_ID, FRP_FULL);

    return true;
}

bool FrUpdateGraphTabsCmd::UpdateGraphs(){
    // check params
    if(m_GraphTabDocObj == 0L) return false;
    // Performe action in view
    FrGraphBookmarkWidget* bmWidget = 
        this->GetMainController()->GetMainView()->GetGraphPaneWidget()->GetGraphBookmarkWidget();

    bool result = false;
    switch(m_Action){
        case FrUpdateGraphTabsCmd::Add:
            result = bmWidget->AddBookmark(m_GraphTabDocObj);
            break;
        case FrUpdateGraphTabsCmd::Remove: 
            result = bmWidget->RemoveBookmark(m_GraphTabDocObj);
            break;
        case FrUpdateGraphTabsCmd::Update:
            // TODO: Implement !!!
            //result = bmWidget->UpdateTab(m_GraphTabDocObj);
            break;
    }
    return result;
}


void FrUpdateGraphTabsCmd::SetupTabObjects(){
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrGraphTabDocObj* tabSets = doc->GetCurrentGraphTab();

    // Remove graphs
    FrDocument::DocObjCollection oldGraphs;
    doc->GetObjectsByType(oldGraphs, FrDocumentObj::GraphObject);
    
    FrDocument::DocObjCollection::iterator it, itEnd(oldGraphs.end());
    for(it = oldGraphs.begin(); it != itEnd; ++it){
        FrGraphDocObj* graphDO = (FrGraphDocObj*)(*it);
        doc->Remove(graphDO);
    }

    // Remove points
    FrDocument::DocObjCollection points;
    doc->GetObjectsByType(points, FrDocumentObj::PointsObject);
    for(it = points.begin(); it != points.end(); ++it){
        doc->Remove(*it);
    }

    // Add new
    FrPointsDocObj* tmp = tabSets->GetPoints();
    if (tmp){
        FrPointsDocObj* pointsDO = new FrPointsDocObj();
        pointsDO->CopySettings(tmp);
        doc->Add(pointsDO);
    }
    else{
        //// get image doc obj
        //std::vector<FrDocumentObj*> images;
        //doc->GetObjectsByType(images, FrDocumentObj::ImageObject);
        //if (images.size()>0){
        //    FrImageDocObj* img = (FrImageDocObj*)images[0];
        //    RtMRIImage* rtImg = img->GetTimePointData(img->GetLastTimePoint());

        //    // add new points doc obj
        //    FrPointsDocObj* pointsDO = new FrPointsDocObj();
        //    pointsDO->GetDimsFromImg(rtImg);
        //    doc->Add(pointsDO);
        //}
    }


    FrGraphTabDocObj::GraphsCollection& newGraphs = tabSets->GetGraphs();
    std::vector<FrGraphDocObj*>::iterator itr, itrEnd(newGraphs.end());
    for(itr = newGraphs.begin(); itr != itrEnd; ++itr){
        FrGraphDocObj* graphDO = (FrGraphDocObj*)(*itr);
        FrGraphDocObj* tmp = new FrGraphDocObj(graphDO);
        doc->Add(tmp);
    }
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrUpdateGraphTabsCmd::CanUndo(){
    return false;
}

bool FrUpdateGraphTabsCmd::Undo(){
    return false;
}

bool FrUpdateGraphTabsCmd::Redo(){
    return false;
}
