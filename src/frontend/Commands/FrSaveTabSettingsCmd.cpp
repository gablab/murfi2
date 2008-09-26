#include "FrSaveTabSettingsCmd.h"
#include "FrTabSettingsDocObj.h"
#include "FrTabInfoDialog.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"

#include "FrBaseView.h"
#include "FrSliceView.h"
#include "FrMosaicView.h"
#include "FrOrthoView.h"
#include "FrTBCFilter.h"
#include "FrSliceExtractor.h"

#include "vtkCamera.h"
#include "vtkRenderer.h"

#include "Qt/QString.h"

FrSaveTabSettingsCmd::FrSaveTabSettingsCmd()
: m_IsDefault(false), m_IsCurrent(false) {
}

bool FrSaveTabSettingsCmd::Execute(){
    // Check for safety
    if(!this->GetMainController()) return false;

    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrMainWindow* mv = this->GetMainController()->GetMainView();

    FrTabInfoDialog dlg(mv, true);
    dlg.SetCaption(QString("Save view to tab"));

    bool result = false;
    if(!m_IsDefault && dlg.SimpleExec()){
        // Create doc object & set params
        FrTabSettingsDocObj* docObj = new FrTabSettingsDocObj();
        docObj->SetIsCurrent(false);
        docObj->SetName(dlg.GetName());
        docObj->SetDescription(dlg.GetDescription());
        // Use new instead
        // InitDocObjFromView(docObj, mv);
        InitDocObjFromActive(docObj);

        // add to main document
        result = doc->Add(docObj);
    }
    else if(m_IsDefault){
        // Create default doc object and set params
        FrTabSettingsDocObj* docObj = new FrTabSettingsDocObj(true);
        docObj->SetIsCurrent(false);
        docObj->SetName(QString("Deafult"));
        docObj->SetDescription(QString("Default tab"));
        
        // add to main document
        result = doc->Add(docObj);
    }
    return result;
}

void FrSaveTabSettingsCmd::InitDocObjFromActive(FrTabSettingsDocObj* docObj){
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrTabSettingsDocObj* source = doc->GetCurrentTabSettings();
    docObj->InitFrom(source);
}

void FrSaveTabSettingsCmd::InitDocObjFromView(FrTabSettingsDocObj* docObj, 
                                              FrMainWindow* mv){
    // Setup common settings
    SetActiveViewSettings(docObj, mv);

    // Init slice view settings
    FrSliceView* sliceView = mv->GetSliceView();
    ViewSettings& svSettings = docObj->GetSliceViewSettings();
    svSettings.SliceNumber = sliceView->GetSliceExtractor()->GetSlice();
    InitCamSettings(&svSettings.CamSettings, 
                    sliceView->GetRenderer()->GetActiveCamera());
    InitTBCSettings(&svSettings.TbcSetting, 
                    sliceView->GetTBCFilter());

    // Init mosaic view settings
    FrMosaicView* mosaicView = mv->GetMosaicView();
    ViewSettings& mvSettings = docObj->GetMosaicViewSettings();
    InitCamSettings(&mvSettings.CamSettings, 
                    mosaicView->GetRenderer()->GetActiveCamera());
    InitTBCSettings(&mvSettings.TbcSetting, 
                    mosaicView->GetTBCFilter());

    // Init ortho view settings
    FrOrthoView* orthoView = mv->GetOrthoView();
    OViewSettings& ovSettings = docObj->GetOrthoViewSettings();

    ovSettings.CoronalSlice = orthoView->GetSliceExtractor(CORONAL_RENDERER)->GetSlice();
    ovSettings.SagitalSlice = orthoView->GetSliceExtractor(SAGITAL_RENDERER)->GetSlice();
    ovSettings.AxialSlice   = orthoView->GetSliceExtractor(AXIAL_RENDERER)->GetSlice();

    for(int i=0; i < RENDERER_COUNT-1; ++i){
        InitCamSettings(&ovSettings.CamSettings[i], 
                        orthoView->GetRenderer(i)->GetActiveCamera());
        InitTBCSettings(&ovSettings.TbcSettings[i], 
                        orthoView->GetTBCFilter(i));
    }
}

void FrSaveTabSettingsCmd::SetActiveViewSettings(FrTabSettingsDocObj* docObj, 
                                                 FrMainWindow* mv){
    ActiveView::View activeView = ActiveView::Unknown;
    
    FrBaseView* currentView = mv->GetCurrentView();
    if(currentView == mv->GetSliceView()){
        activeView = ActiveView::Slice; 
    }
    else if(currentView == mv->GetMosaicView()){
        activeView = ActiveView::Mosaic;
    }
    else if(currentView == mv->GetOrthoView()){
        activeView = ActiveView::Ortho;
    }

    docObj->SetActiveView(activeView);
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