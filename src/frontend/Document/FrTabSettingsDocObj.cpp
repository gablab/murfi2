#include "FrTabSettingsDocObj.h"
#include "FrDocument.h"
#include "FrCommandController.h"

// some defines
#define DEF_THRESHOLD   0.0
#define DEF_BRIGHTNESS  0.0
#define DEF_CONTRAST    0.0

#define DEF_CAM_SCALE    200.0
#define DEF_CAM_FOCUS    0.0, 0.0, 0.0
#define DEF_CAM_VIEWUP   0.0, 1.0, 0.0
#define DEF_CAM_POSITION 0.0, 0.0, 1.0

#define DEF_VIEW (ActiveView::Slice)
#define DEF_SLICE_NUMBER 0

// init helper
inline void INIT_ARR3(double arr[], double a0, double a1, double a2){
    arr[0] = a0; arr[1] = a1; arr[2] = a2;
}
inline void INIT_ARR_ARR(double arr0[], double arr1[]){
    arr0[0] = arr1[0]; arr0[1] = arr1[1]; arr0[2] = arr1[2];
}

// Implementation
FrTabSettingsDocObj::FrTabSettingsDocObj(bool isDefault){
    m_IsCurrent = false;
    m_IsDefault = isDefault;
    m_ActiveView = DEF_VIEW;

    //  Setup simple views
    ViewSettings* vs[] = { &m_SliceViewSettings, &m_MosaicViewSettings };
    for(int i=0; i < 2; ++i){
        vs[i]->SliceNumber  = DEF_SLICE_NUMBER;
                
        vs[i]->TbcSetting.Contrast = DEF_CONTRAST;
        vs[i]->TbcSetting.Threshold = DEF_THRESHOLD;
        vs[i]->TbcSetting.Brightness = DEF_BRIGHTNESS;

        vs[i]->CamSettings.Scale = DEF_CAM_SCALE;
        INIT_ARR3(vs[i]->CamSettings.ViewUp, DEF_CAM_VIEWUP);
        INIT_ARR3(vs[i]->CamSettings.FocalPoint, DEF_CAM_FOCUS);
        INIT_ARR3(vs[i]->CamSettings.Position, DEF_CAM_POSITION);
    }

    // Setup ortho view
    OViewSettings* ovs = &m_OrthoViewSettings;
    ovs->CoronalSlice = DEF_SLICE_NUMBER;
    ovs->SagitalSlice = DEF_SLICE_NUMBER;
    ovs->AxialSlice   = DEF_SLICE_NUMBER;

    for(int i=0; i < ORTHO_VIEW_NUM; ++i){
        ovs->TbcSettings[i].Contrast = DEF_CONTRAST;
        ovs->TbcSettings[i].Threshold = DEF_THRESHOLD;
        ovs->TbcSettings[i].Brightness = DEF_BRIGHTNESS;

        ovs->CamSettings[i].Scale = DEF_CAM_SCALE;
        INIT_ARR3(ovs->CamSettings[i].ViewUp, DEF_CAM_VIEWUP);
        INIT_ARR3(ovs->CamSettings[i].FocalPoint, DEF_CAM_FOCUS);
        INIT_ARR3(ovs->CamSettings[i].Position, DEF_CAM_POSITION);
    }

	// Setup layer settings, add default layer
	LayerSettings ls;
	ls.color = QColor(0, 0, 0);
	ls.ColormapType = 0;			// no colormap
	ls.Opacity = 255;
	ls.Threshold = 128;
	ls.Visible = true;

	LayeredImageSettings* lis = &m_LayeredImageSettings;
	lis->push_back(ls);
}

FrTabSettingsDocObj::~FrTabSettingsDocObj(){
}

void FrTabSettingsDocObj::OnAdd(FrDocument* doc){    
    // We need to add tab here
    FrUpdateTabsCmd* cmd = FrCommandController::CreateCmd<FrUpdateTabsCmd>();
    cmd->SetAction(FrUpdateTabsCmd::Add);
    cmd->SetTabSettingsDocObj(this);
    cmd->Execute();
    delete cmd;
}

void FrTabSettingsDocObj::OnRemove(FrDocument* doc){    
    // We need to remove tab here
    FrUpdateTabsCmd* cmd = FrCommandController::CreateCmd<FrUpdateTabsCmd>();
    cmd->SetAction(FrUpdateTabsCmd::Remove);
    cmd->SetTabSettingsDocObj(this);
    cmd->Execute();
    delete cmd;
}

FrDocumentObj::ObjType FrTabSettingsDocObj::GetType(){
    return FrDocumentObj::TabSettings;
}

void FrTabSettingsDocObj::InitFrom(FrTabSettingsDocObj* docObj){
    // Do not need copy this
    // Have to be set by user manually
    //m_IsCurrent = docObj->m_IsCurrent;
    //m_IsDefault = docObj->m_IsDefault;
    m_ActiveView = docObj->m_ActiveView;

    //  Setup simple views
    ViewSettings* vs[] = { &m_SliceViewSettings, &m_MosaicViewSettings };
    ViewSettings* vsSrc[] = { &docObj->m_SliceViewSettings, &docObj->m_MosaicViewSettings };
    for(int i=0; i < 2; ++i){
        vs[i]->SliceNumber  = vsSrc[i]->SliceNumber;
                
        vs[i]->TbcSetting.Contrast = vsSrc[i]->TbcSetting.Contrast;
        vs[i]->TbcSetting.Threshold = vsSrc[i]->TbcSetting.Threshold;
        vs[i]->TbcSetting.Brightness = vsSrc[i]->TbcSetting.Brightness;

        vs[i]->CamSettings.Scale = vsSrc[i]->CamSettings.Scale;
        INIT_ARR_ARR(vs[i]->CamSettings.ViewUp, vsSrc[i]->CamSettings.ViewUp);
        INIT_ARR_ARR(vs[i]->CamSettings.Position, vsSrc[i]->CamSettings.Position);
        INIT_ARR_ARR(vs[i]->CamSettings.FocalPoint, vsSrc[i]->CamSettings.FocalPoint);
    }

    // Setup ortho view
    OViewSettings* ovs = &m_OrthoViewSettings;
    OViewSettings* ovsSrc = &docObj->m_OrthoViewSettings;
    ovs->CoronalSlice = ovsSrc->CoronalSlice;
    ovs->SagitalSlice = ovsSrc->SagitalSlice;
    ovs->AxialSlice   = ovsSrc->AxialSlice;

    for(int i=0; i < ORTHO_VIEW_NUM; ++i){
        ovs->TbcSettings[i].Contrast = ovsSrc->TbcSettings[i].Contrast;
        ovs->TbcSettings[i].Threshold = ovsSrc->TbcSettings[i].Threshold;
        ovs->TbcSettings[i].Brightness = ovsSrc->TbcSettings[i].Brightness;

        ovs->CamSettings[i].Scale = ovsSrc->CamSettings[i].Scale;
        INIT_ARR_ARR(ovs->CamSettings[i].ViewUp, ovsSrc->CamSettings[i].ViewUp);
        INIT_ARR_ARR(ovs->CamSettings[i].Position, ovsSrc->CamSettings[i].Position);
        INIT_ARR_ARR(ovs->CamSettings[i].FocalPoint, ovsSrc->CamSettings[i].FocalPoint);
    }

	// setup layered image
	//LayeredImageSettings lis = &m_LayeredImageSettings;
	LayeredImageSettings lisSrc = docObj->m_LayeredImageSettings;

	for (int i = 0; i<lisSrc.size(); i++){
		if (i == 0){
			m_LayeredImageSettings[i].color = QColor(0, 0, 0);
			m_LayeredImageSettings[i].ColormapType = 0;			// no colormap
			m_LayeredImageSettings[i].Opacity = 255;
			m_LayeredImageSettings[i].Threshold = 128;
			m_LayeredImageSettings[i].Visible = true;
		}
		else
			m_LayeredImageSettings.push_back(lisSrc[i]);
	}
}

void FrTabSettingsDocObj::AddLayer(LayerSettings ls){
	LayeredImageSettings* lis = &m_LayeredImageSettings;
	lis->push_back(ls);
}

void FrTabSettingsDocObj::RemoveLayer(LayerSettings ls){
	// TODO: not finished
	LayeredImageSettings* lis = &m_LayeredImageSettings;

	std::vector<LayerSettings>::iterator Iter;
   
	for (Iter = m_LayeredImageSettings.begin(); Iter != m_LayeredImageSettings.end(); Iter++){
		LayerSettings layer = (LayerSettings)(*Iter);
		if (layer.id == ls.id){		// should be implemented support for multiple deletion? not only active layer deletion?
			lis->erase(Iter);
			//delete layer;
			return;					// because we can't store two layers with the same ids 	
		}
	}
	
	// update layers ids
}