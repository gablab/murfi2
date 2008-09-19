#include "FrSaveTabSettingsCmd.h"
#include "FrTabSettingsDocObj.h"
#include "FrTabInfoDialog.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"

#include "FrView2D.h"
#include "FrMosaicView.h"
#include "FrOrthoView.h"
#include "FrTBCFilter.h"
#include "FrSliceExtractor.h"

#include "vtkCamera.h"
#include "vtkRenderer.h"

#include "Qt/QString.h"

FrSaveTabSettingsCmd::FrSaveTabSettingsCmd()
: m_IsDefault(false) {
}

bool FrSaveTabSettingsCmd::Execute(){
    // Check for safety
    if(!this->GetMainController()) return false;

    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrTabInfoDialog dlg(mv, true);
    dlg.SetCaption(QString("Save view to tab"));
    dlg.SetInfo(QString("Enter name and short description of tab"));

    bool result = false;
    if(dlg.SimpleExec()){
        // Create doc object
        FrTabSettingsDocObj* docObj = new FrTabSettingsDocObj(m_IsDefault);
        docObj->SetName(dlg.GetName());
        docObj->SetDescription(dlg.GetDescription());

        InitializeDocumentObj(docObj, mv);

        // add to main document
        FrMainDocument* doc = this->GetMainController()->GetMainDocument();
        result = doc->Add(docObj);
    }
    return result;
}

void FrSaveTabSettingsCmd::InitializeDocumentObj(FrTabSettingsDocObj* docObj, 
                                                 FrMainWindow* mv){
    
    FrView2D* sliceView = mv->GetSliceView();
    ViewSettings& svSettings = docObj->GetSliceViewSettings();
    svSettings.SliceNumber = sliceView->GetSliceExtractor()->GetSlice();
    InitCamSettings(&svSettings.CamSettings, 
                    sliceView->GetRenderer()->GetActiveCamera());
    InitTBCSettings(&svSettings.TbcSetting, 
                    sliceView->GetTBCFilter());

    FrMosaicView* mosaicView = mv->GetMosaicView();
    ViewSettings& mvSettings = docObj->GetMosaicViewSettings();
    InitCamSettings(&mvSettings.CamSettings, 
                    mosaicView->GetRenderer()->GetActiveCamera());
    InitTBCSettings(&mvSettings.TbcSetting, 
                    mosaicView->GetTBCFilter());

    // Do nothing here (with ORTHO VIEW)
    // ViewSettings& ovSettings = docObj->GetOrthoViewSettings();    
}

void FrSaveTabSettingsCmd::InitCamSettings(void* settings, vtkCamera* cam){
    //Assume that settings is of type CameraSettings
    CameraSettings* camSettings = (CameraSettings*)settings;

    cam->GetFocalPoint(camSettings->FocalPoint);
    cam->GetPosition(camSettings->Position);
    cam->GetViewUp(camSettings->ViewUp);

    camSettings->Scale = cam->GetParallelScale();
}

void FrSaveTabSettingsCmd::InitTBCSettings(void* settings, FrTBCFilter* tbcFilter){
    //Assume that settings is of type TBCSettings
    TBCSettings* tbcSettings = (TBCSettings*)settings;

    tbcSettings->Threshold  = tbcFilter->GetThreshold();
    tbcSettings->Brightness = tbcFilter->GetBrightness();
    tbcSettings->Contrast   = tbcFilter->GetContrast();
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrSaveTabSettingsCmd::CanUndo(){
    return false;
}

bool FrSaveTabSettingsCmd::Undo(){
    return false;
}

bool FrSaveTabSettingsCmd::Redo(){
    return false;
}