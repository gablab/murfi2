#include "FrMaskBaseCmd.h"
#include "FrMaskEditor.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrRoiDocObj.h"
#include "FrTabSettingsDocObj.h"
#include "FrLayeredImage.h"
#include "FrSettings.h"

#include "FrSliceView.h"
#include "FrMosaicView.h"
#include "FrOrthoView.h"

// VTK includes
#include "vtkImageData.h"


FrMaskBaseCmd::FrMaskBaseCmd()
: m_ImageNumber(-1) {
    // NOTE: do nothing in this class
}

FrMaskBaseCmd::~FrMaskBaseCmd(){
    // NOTE: do nothing in this class
}

FrRoiDocObj* FrMaskBaseCmd::GetCurrentRoi(){
    FrRoiDocObj* result = 0L;
    if(this->GetMainController()){
        std::vector<FrDocumentObj*> rois;

        FrMainDocument* doc = this->GetMainController()->GetMainDocument();
        doc->GetObjectsByType(rois, FrDocumentObj::RoiObject);
        
        if(rois.size() > 0){
            int layerID = -1;
            FrTabSettingsDocObj* tsDO = doc->GetCurrentTabSettings();
            switch(tsDO->GetActiveView()){
                case FrTabSettingsDocObj::SliceView:
                    layerID = tsDO->GetSliceViewSettings()->ActiveLayerID;
                    break;
                case FrTabSettingsDocObj::MosaicView:
                    layerID = tsDO->GetMosaicViewSettings()->ActiveLayerID;
                    break;
                case FrTabSettingsDocObj::OrthoView:
                    layerID = tsDO->GetOrthoViewSettings()->ActiveLayerID;
                    break;
            }

            std::vector<FrDocumentObj*>::iterator it, itEnd(rois.end());
            for(it = rois.begin(); it != itEnd; ++it){
                FrRoiDocObj* roiDO = (FrRoiDocObj*)(*it);
                if(roiDO->GetID() == layerID){
                    result = roiDO;
                    break;
                }
            }
        }
    }
    return result;
}

vtkImageData* FrMaskBaseCmd::GetRoiImageData(int id){
    vtkImageData* result = 0L;
    if(this->GetMainController()){
        FrMainWindow* mv = this->GetMainController()->GetMainView();
        
        if(mv->GetCurrentView() == mv->GetSliceView()){
            result = mv->GetSliceView()->GetImage()->GetRoiInput(id);
        }
        else if(mv->GetCurrentView() == mv->GetMosaicView()){
            result = mv->GetMosaicView()->GetImage()->GetRoiInput(id);
        }
        else if(mv->GetCurrentView() == mv->GetOrthoView()){
            if(0 <= m_ImageNumber && m_ImageNumber < ORTHO_IMAGE_COUNT){
                result = mv->GetOrthoView()->GetImage(m_ImageNumber)->GetRoiInput(id);
            }
        }
    }
    return result;
}
    
void FrMaskBaseCmd::ApplyDataToRoi(vtkImageData* data, FrRoiDocObj* roiDO, int sliceNumber){
    
    FrMaskEditor* me = FrMaskEditor::New();
    me->SetInput(data);
    me->SetDocumentObj(roiDO);
    me->SetView(FrMaskEditor::Slice);
    me->SetOrientation(FrMaskEditor::XY);

    // Setup slice number
    if (sliceNumber == -1)
        sliceNumber = this->GetCurrentRoiSliceNumber();
    me->SetSliceNumber(sliceNumber);
        
    me->Update();
    me->Delete();
}

int FrMaskBaseCmd::GetCurrentRoiSliceNumber(){
    int sliceNumber = -1;
    if(this->GetMainController()){
        FrMainDocument* doc = this->GetMainController()->GetMainDocument();
        
        FrTabSettingsDocObj* tsDO = doc->GetCurrentTabSettings();
        switch(tsDO->GetActiveView()){
            case FrTabSettingsDocObj::SliceView:
                sliceNumber = tsDO->GetSliceViewSettings()->SliceNumber;
                break;
            case FrTabSettingsDocObj::MosaicView:
                // NOTE: always 0 fo mosaic view
                sliceNumber = 0;
                break;
            case FrTabSettingsDocObj::OrthoView:
                // Setup proper slice for ortho view
                switch(m_ImageNumber){
                    case CORONAL_IMAGE: 
                        sliceNumber = tsDO->GetOrthoViewSettings()->CoronalSlice;
                        break;
                    case SAGITAL_IMAGE:
                        sliceNumber = tsDO->GetOrthoViewSettings()->SagitalSlice;
                        break;
                    case AXIAL_IMAGE:
                        sliceNumber = tsDO->GetOrthoViewSettings()->AxialSlice;
                        break;
                }
                break;
            default: ;
                // NOTE: do nothing
        }
    }
    return sliceNumber;
}

FrSpecialLayer* FrMaskBaseCmd::GetSpecialLayer(){
    FrSpecialLayer* sl;
    
    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrMainDocument* md = this->GetMainController()->GetMainDocument();
    FrTabSettingsDocObj* ts = md->GetCurrentTabSettings();

    enum FrTabSettingsDocObj::View view = ts->GetActiveView();
    switch(view){
        case FrTabSettingsDocObj::SliceView:
            sl = mv->GetSliceView()->GetImage()->GetSpecialLayer();
            break;
        case FrTabSettingsDocObj::MosaicView:
            sl = mv->GetMosaicView()->GetImage()->GetSpecialLayer();
            break;
        case FrTabSettingsDocObj::OrthoView:
            {
                FrOrthoView* ov =  mv->GetOrthoView();

                if (m_ImageNumber != -1){
                    sl = ov->GetImage(m_ImageNumber)->GetSpecialLayer();
                }
                else{
                    return false;
                }
                break;
            }
    } // end switch(view)

    return sl;
}