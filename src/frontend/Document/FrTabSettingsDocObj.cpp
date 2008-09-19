#include "FrTabSettingsDocObj.h"
#include "FrDocument.h"

// some defines
#define DEF_THRESHOLD   0.0
#define DEF_BRIGHTNESS  0.0
#define DEF_CONTRAST    0.0

#define DEF_CAM_SCALE    1.0
#define DEF_CAM_FOCUS    0.0, 0.0, 0.0
#define DEF_CAM_VIEWUP   0.0, 0.0, 0.0
#define DEF_CAM_POSITION 0.0, 0.0, 0.0

#define DEF_VIEW (ActiveView::Slice)
#define DEF_SLICE_NUMBER 0

// init helper
inline void INIT_ARR3(double arr[], double a0, double a1, double a2){
    arr[0] = a0; arr[1] = a1; arr[2] = a2;
} 

// Implementation

FrTabSettingsDocObj::FrTabSettingsDocObj(bool isDefault){
    m_IsCurrent = false;
    m_IsDefault = isDefault;
    m_ActiveView = DEF_VIEW;

    ViewSettings vs[] = { m_SliceViewSettings, m_MosaicViewSettings, m_OrthoViewSettings };
    for(int i=0; i < 3; ++i){
        vs[i].SliceNumber  = DEF_SLICE_NUMBER;
        vs[i].CoronalSlice = DEF_SLICE_NUMBER;
        vs[i].SagitalSlice = DEF_SLICE_NUMBER;
        vs[i].AxialSlice   = DEF_SLICE_NUMBER;
        
        vs[i].TbcSetting.Contrast = DEF_CONTRAST;
        vs[i].TbcSetting.Threshold = DEF_THRESHOLD;
        vs[i].TbcSetting.Brightness = DEF_BRIGHTNESS;

        vs[i].CamSettings.Scale = DEF_CAM_SCALE;
        INIT_ARR3(vs[i].CamSettings.ViewUp, DEF_CAM_VIEWUP);
        INIT_ARR3(vs[i].CamSettings.FocalPoint, DEF_CAM_FOCUS);
        INIT_ARR3(vs[i].CamSettings.Position, DEF_CAM_POSITION);
    }
}

FrTabSettingsDocObj::~FrTabSettingsDocObj(){
}

void FrTabSettingsDocObj::OnAdd(FrDocument* doc){
    // TODO: generate command to update view
    // We need to add tab here
}

void FrTabSettingsDocObj::OnRemove(FrDocument* doc){
    // TODO: Generate command to update view
    // We need to remove tab here
}

FrDocumentObj::ObjType FrTabSettingsDocObj::GetType(){
    return FrDocumentObj::TabSettings;
}