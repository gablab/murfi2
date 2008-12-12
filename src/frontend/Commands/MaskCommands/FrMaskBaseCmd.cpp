#include "FrMaskBaseCmd.h"
#include "FrMaskEditor.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrRoiDocObj.h"
#include "FrTabSettingsDocObj.h"
#include "FrLayeredImage.h"
#include "FrSettings.h"
#include "FrViewDocObj.h"

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

            FrViewDocObj* viewDO = 0L;
            FrDocument::DocObjCollection views;
            doc->GetObjectsByType(views, FrDocumentObj::ViewObject);    
            if(views.size() > 0){
                viewDO = (FrViewDocObj*)views[0];
            }

            switch(viewDO->GetActiveView()){
                case Views::SliceView:
                    layerID = viewDO->GetSliceViewSettings()->ActiveLayerID;
                    break;
                case Views::MosaicView:
                    layerID = viewDO->GetMosaicViewSettings()->ActiveLayerID;
                    break;
                case Views::OrthoView:
                    layerID = viewDO->GetOrthoViewSettings()->ActiveLayerID;
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
            if(0 <= m_ImageNumber && m_ImageNumber < ORTHO_VIEWS_CNT){
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

    // Setup proper view
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrViewDocObj* viewDO = 0L;
    FrDocument::DocObjCollection views;
    doc->GetObjectsByType(views, FrDocumentObj::ViewObject);    
    if(views.size() > 0){
        viewDO = (FrViewDocObj*)views[0];
    }
    me->SetView(viewDO->GetActiveView());

    // Set orientation
    switch(m_ImageNumber){
        case DEF_CORONAL:
            me->SetOrientation(FrMaskEditor::XZ);
            break;
        case DEF_SAGITAL:
            me->SetOrientation(FrMaskEditor::YZ);
            break;
        case DEF_AXIAL:
            me->SetOrientation(FrMaskEditor::XY);
            break;
        default:
            // NOTE: Do we need it?
            me->SetOrientation(FrMaskEditor::XY);
            break;
    }
    
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

        FrViewDocObj* viewDO = 0L;
        FrDocument::DocObjCollection views;
        doc->GetObjectsByType(views, FrDocumentObj::ViewObject);    
        if(views.size() > 0){
            viewDO = (FrViewDocObj*)views[0];
        }

        switch(viewDO->GetActiveView()){
            case Views::SliceView:
                sliceNumber = viewDO->GetSliceViewSettings()->SliceNumber;
                break;
            case Views::MosaicView:
                // NOTE: always 0 fo mosaic view
                sliceNumber = 0;
                break;
            case Views::OrthoView:
                // Setup proper slice for ortho view
                sliceNumber = viewDO->GetOrthoViewSettings()->SliceNumber[m_ImageNumber];   // TODO: check for correct imgNumber
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
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();

    FrViewDocObj* viewDO = 0L;
    FrDocument::DocObjCollection views;
    doc->GetObjectsByType(views, FrDocumentObj::ViewObject);    
    if(views.size() > 0){
        viewDO = (FrViewDocObj*)views[0];
    }

    Views view = viewDO->GetActiveView();
    switch(view){
        case Views::SliceView:
            sl = mv->GetSliceView()->GetImage()->GetSpecialLayer();
            break;
        case Views::MosaicView:
            sl = mv->GetMosaicView()->GetImage()->GetSpecialLayer();
            break;
        case Views::OrthoView:
            {
                FrOrthoView* ov =  mv->GetOrthoView();

                if (m_ImageNumber != -1){
                    sl = ov->GetImage(m_ImageNumber)->GetSpecialLayer();
                }
                else{
                    return 0L;
                }
                break;
            }
    } // end switch(view)

    return sl;
}
