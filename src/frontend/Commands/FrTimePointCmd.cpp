#include "FrTimePointCmd.h"
#include "FrMainDocument.h"
#include "FrMainWindow.h"
#include "FrViewDocObj.h"
#include "FrImageDocObj.h"
#include "FrGraphPaneWidget.h"
#include "FrAppSettingsDocObj.h"

#define BAD_TIME_POINT -1
#define DEFAULT_CHECK_LIFE false

FrTimePointCmd::FrTimePointCmd() 
: m_Action(FrTimePointCmd::None), 
  m_TimePoint(BAD_TIME_POINT),
  m_CheckLifeMode(DEFAULT_CHECK_LIFE) {

}

bool FrTimePointCmd::Execute(){
    // Check for safety
    if(!this->GetMainController()) return false;

    bool result = false;
    switch(m_Action){
        case FrTimePointCmd::SetLast: 
            result = this->SetLastTimePoint();
            break;
        case FrTimePointCmd::SetNext:
            result = this->SetNextTimePoint();
            break;
        case FrTimePointCmd::SetPrevious:
            result = this->SetPreviousTimePoint();
            break;
        case FrTimePointCmd::SetUserDefined:
            result = this->SetUserDefinedTimePoint();
            break;
        default:
            // Do nothing here
            break;
    }

    FrBaseCmd::UpdatePipelineForID(ALL_LAYER_ID, FRP_READ);

    return result;
}

// Helper methods
bool FrTimePointCmd::SetLastTimePoint(){

    FrMainDocument* doc = this->GetMainController()->GetMainDocument();

    // Have to check that life mode is on
    if(m_CheckLifeMode){

        FrAppSettingsDocObj* appSettings = doc->GetAppSettings();
        if(appSettings == 0 && !appSettings->GetLiveMode()){
            
            return false;
        }
    }

    FrImageDocObj* imgDO = this->GetImageObject();
    if(imgDO != 0){

        m_TimePoint = imgDO->GetLastTimePoint();
    }
    return this->SetUserDefinedTimePoint();
}

bool FrTimePointCmd::SetNextTimePoint(){
    // Assume there is no any missed TP
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrViewDocObj* vdo = doc->GetCurrentViewObject();
    m_TimePoint = vdo->GetTimePoint() + 1;

    return this->SetUserDefinedTimePoint();
}

bool FrTimePointCmd::SetPreviousTimePoint(){
    // Assume there is no any missed TP
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrViewDocObj* vdo = doc->GetCurrentViewObject();
    m_TimePoint = vdo->GetTimePoint() - 1;

    return this->SetUserDefinedTimePoint();
}

bool FrTimePointCmd::SetUserDefinedTimePoint(){
    if(m_TimePoint == BAD_TIME_POINT) return false;
    
    // Check if specified timepoint exists
    FrImageDocObj* imgDO = this->GetImageObject();
    if(imgDO == 0 || imgDO->GetTimePointData(m_TimePoint) == 0){

        return false;
    }

    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrViewDocObj* viewDO = doc->GetCurrentViewObject();
    viewDO->SetTimePoint(m_TimePoint);

    FrMainWindow* mv = this->GetMainController()->GetMainView();
    mv->GetGraphPaneWidget()->UpdateTimePoint();

//    FrBaseCmd::UpdatePipelineForID(ALL_LAYER_ID, FRP_READ);

    return true;
}

FrImageDocObj* FrTimePointCmd::GetImageObject(){
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();

    // NOTE: use the only one image for now
    FrImageDocObj* imgDO = 0;
    std::vector<FrDocumentObj*> images;
    doc->GetObjectsByType(images, FrDocumentObj::ImageObject);
    
    int timeSeries = doc->GetCurrentViewObject()->GetTimeSeries();

    std::vector<FrDocumentObj*>::iterator it, itEnd(images.end());
    for(it = images.begin(); it != itEnd; ++it){

        FrImageDocObj* img = (FrImageDocObj*)(*it);
        if(img->GetSeriesNumber() == timeSeries){

            imgDO = img;
            break;
        }
    }
    
    return imgDO;
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrTimePointCmd::CanUndo(){
    return false;
}

bool FrTimePointCmd::Undo(){
    return false;
}

bool FrTimePointCmd::Redo(){
    return false;
}
