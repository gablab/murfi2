#include "FrMaskBaseCmd.h"
#include "FrMaskEditor.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrRoiDocObj.h"
#include "FrTabSettingsDocObj.h"

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
                    layerID = tsDO->GetSliceViewSettings()->ActiveLayerID();
                    break;
                case FrTabSettingsDocObj::MosaicView:
                    layerID = tsDO->GetMosaicViewSettings()->ActiveLayerID();
                    break;
                case FrTabSettingsDocObj::OrthoView:
                    layerID = tsDO->GetOrthoViewSettings()->ActiveLayerID();
                    break;
            }

            std::vector<FrDocumentObj*>::iterator it, itEnd(rois.end());
            for(it = rois.begin(); it != itEnd; ++it){
                FrRoiDocObj* roiDO = (FrRoiDocObj*)(*it);
                if(roiDO->GetID() == layerID){
                    result = foiDO;
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
        
        // TODO: implement        
    }
    return result;
}
    
void FrMaskBaseCmd::ApplyToCurrentROI(vtkImageData* data){
    // TODO: implement

    // Get Current ROI
    FrMaskEditor* me;    
    me->SetInput(data);
    me->SetView(FrMaskEditor::Slice);
    me->SetOrientation(FrMaskEditor::XY);
    me->SetSliceNumber(0);
    me->Update();
}
