#include "FrViewDocObj.h"
#include "FrDocument.h"
#include "FrCommandController.h"
#include "FrTabSettingsDocObj.h"

// VTK includes
#include "vtkImageData.h"
#include "vtkPointData.h"

// Qt stuff
#include <Qt/qstring.h>
#include <Qt/qfile.h>


FrViewDocObj::FrViewDocObj()
: m_TimePoint(0), m_ActiveView(SliceView){          // TODO: add constant for default view
    // Create view settings
    m_SliceViewSettings = new FrSliceViewSettings();
    m_MosaicViewSettings = new FrMosaicViewSettings();
    m_OrthoViewSettings = new FrOrthoViewSettings();
}

FrViewDocObj::~FrViewDocObj(){
    delete m_SliceViewSettings;
    delete m_MosaicViewSettings;
    delete m_OrthoViewSettings;
}

void FrViewDocObj::OnAdd(FrDocument* doc){
    // Probably need the command updating current view
    FrChangeViewCmd* cmd = FrCommandController::CreateCmd<FrChangeViewCmd>();
    cmd->SetTargetView(FrChangeViewCmd::Synchronize);
    cmd->Execute();
    delete cmd;
}

void FrViewDocObj::OnRemove(FrDocument* doc){
    // NOTE: Do nothing here!
}

FrDocumentObj::ObjTypes FrViewDocObj::GetType(){
    return FrDocumentObj::ViewObject;
}

bool FrViewDocObj::IsSliceView(){
    return (m_ActiveView == SliceView);
}

bool FrViewDocObj::IsMosaicView(){
    return (m_ActiveView == MosaicView);
}

bool FrViewDocObj::IsOrthoView(){
    return (m_ActiveView == OrthoView);
}

// init methods
void FrViewDocObj::CopySettingsFrom(FrTabSettingsDocObj* src){
    m_TimePoint = src->GetTimePoint();
    m_ActiveView = src->GetActiveView();
    *(m_SliceViewSettings) = *(src->GetSliceViewSettings());
    *(m_MosaicViewSettings) = *(src->GetMosaicViewSettings());
    *(m_OrthoViewSettings) = *(src->GetOrthoViewSettings());
}

void FrViewDocObj::SaveSettingsTo(FrTabSettingsDocObj* dst){
    dst->SetTimePoint(m_TimePoint);
    dst->SetActiveView(m_ActiveView);
    *(dst->GetSliceViewSettings()) = *(m_SliceViewSettings);
    *(dst->GetMosaicViewSettings()) = *(m_MosaicViewSettings);
    *(dst->GetOrthoViewSettings()) = *(m_OrthoViewSettings);
}
