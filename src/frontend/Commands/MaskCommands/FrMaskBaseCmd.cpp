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
            

            FrViewDocObj* viewDO = doc->GetCurrentViewObject();
            if(viewDO == 0) return 0;

            int layerID = viewDO->GetActiveLayerID();

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
    FrViewDocObj* viewDO = doc->GetCurrentViewObject();
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
        FrViewDocObj* viewDO = doc->GetCurrentViewObject();

        switch(viewDO->GetActiveView()){
            case SliceView:
                sliceNumber = viewDO->GetSliceViewSettings()->SliceNumber;
                break;
            case MosaicView:
                // NOTE: always 0 for mosaic view
                sliceNumber = 0;
                break;
            case OrthoView:
                // Setup proper slice for ortho view
                // TODO: check for correct imgNumber
                sliceNumber = viewDO->GetOrthoViewSettings()->SliceNumber[m_ImageNumber];
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

    FrViewDocObj* viewDO = doc->GetCurrentViewObject();
    switch(viewDO->GetActiveView()){
        case SliceView:
            sl = mv->GetSliceView()->GetImage()->GetSpecialLayer();
            break;
        case MosaicView:
            sl = mv->GetMosaicView()->GetImage()->GetSpecialLayer();
            break;
        case OrthoView:
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

void FrMaskBaseCmd::TransformCoordinatesToIndices(int point[2], vtkImageData* img, int imgID){

    if(!img) return;
    FrMainDocument* doc = this->GetMainController()->GetMainDocument();
    FrViewDocObj* viewDO = doc->GetCurrentViewObject();

    double dSpacing[3];	
    img->GetSpacing(dSpacing);
    
    // calc current indices of point
    switch(viewDO->GetActiveView()){
        case SliceView:
            point[0] = int((point[0] + 1) / dSpacing[0]);
            point[1] = int((point[1] + 1) / dSpacing[1]);
            break;
        case MosaicView:
            point[0] = int((point[0] + 1) / dSpacing[0]);
            point[1] = int((point[1] + 1) / dSpacing[1]);
            break;
        case OrthoView:
            // Renderer number in ortho view
            switch(imgID){				
                case DEF_CORONAL:
                    point[0] = int((point[0] + 1) / dSpacing[0]);
                    point[1] = int((point[1] + 1) / dSpacing[1]);
                    break;
                case DEF_SAGITAL:
                    point[0] = int((point[0] + 1) / dSpacing[0]);
                    point[1] = int((point[1] + 1) / dSpacing[1]);
		            break;
                case DEF_AXIAL:
                    point[0] = int((point[0] + 1) / dSpacing[0]);
	                point[1] = int((point[1] + 1) / dSpacing[1]);
		            break;
            }
            break;
    } // end switch view
}
