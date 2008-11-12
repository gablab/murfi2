#include "FrMaskRectangleCmd.h"
#include "FrMainWindow.h"
#include "FrLayeredImage.h"
#include "FrBaseView.h"
#include "FrMosaicView.h"
#include "FrSliceView.h"
#include "FrOrthoView.h"

// VTK stuff
#include "vtkImageData.h"
#include "vtkImageLogic.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"
#include "vtkPointPicker.h"
#include "vtkCoordinate.h"
#include "vtkRenderer.h"


FrMaskRectangleCmd::FrMaskRectangleCmd()
: m_PointPicker(0){
}

FrMaskRectangleCmd::~FrMaskRectangleCmd(){
}

bool FrMaskRectangleCmd::Execute(){
    if(!this->GetMainController()) return false;

    bool result = false;

    switch (m_Action){
        case Action::Draw:
            result = DrawMask();
            break;
        case Action::Write:
            result = WriteMask();
            break;
        case Action::Undefined:
            result = false;
            break;
    }

    return result;
}

bool FrMaskRectangleCmd::DrawMask(){
    bool result = false;

    // get special layer and set selection params    
    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrSpecialLayer* sl = mv->GetSliceView()->GetImage()->GetSpecialLayer();

    // set params
    SelectionParams params;
    params.type = 1;
    params.leftCorner[0] = m_Rect.leftX;
    params.leftCorner[1] = m_Rect.leftY;
    params.width = m_Rect.rightX - m_Rect.leftX;         // test
    params.height = m_Rect.rightY - m_Rect.leftY;        // test

    sl->SetSelection(params);
    mv->GetSliceView()->UpdatePipeline(FRP_SETCAM);

    //enum FrTabSettingsDocObj::View view = ts->GetActiveView();
    //switch(view){
    //    case FrTabSettingsDocObj::SliceView:
    //        mv->GetSliceView()->GetImage()->GetRenderers(renCollection);
    //        lim = mv->GetSliceView()->GetImage();
    //        GetLayerSettings(ts->GetSliceViewSettings(), layers);
    //        break;
    //    case FrTabSettingsDocObj::MosaicView:
    //        mv->GetMosaicView()->GetImage()->GetRenderers(renCollection);
    //        lim = mv->GetMosaicView()->GetImage();
    //        GetLayerSettings(ts->GetMosaicViewSettings(), layers);
    //        break;
    //    case FrTabSettingsDocObj::OrthoView:
    //        {
    //            FrOrthoView* ov =  mv->GetOrthoView();

    //            // Find Image where click's occured
    //            for(int i=0; i < ORTHO_IMAGE_COUNT; ++i){
    //                if (ov->GetImage(i)->GetRenderer()->IsInViewport(m_mouseX, m_mouseY)){
    //                    imgNumber = i; 
    //                    break;
    //                }
    //            }
    //            if (imgNumber != -1){
    //                ov->GetImage(imgNumber)->GetRenderers(renCollection);
    //                lim = ov->GetImage(imgNumber);
    //                GetLayerSettings(ts->GetOrthoViewSettings(), layers, imgNumber);
    //            }
    //            else{
    //                ResetVoxelInfo();
    //                return false;
    //            }
    //            break;
    //        }
    //} // end switch(view)

    

    return result;
}

bool FrMaskRectangleCmd::WriteMask(){
    bool result = false;
    vtkImageData* imageData = this->GetCurrentROIImageData();
    if(imageData){
        // TODO: implement
        result = true;
    }
    return result;
}

//int FrMaskRectangleCmd::GetVisibleLayer(std::vector<FrLayerSettings*> layers){
//    std::vector<FrLayerSettings*>::iterator it, itEnd(layers.end());
//    for (it = layers.begin(); it != itEnd; ++it){
//        if ((*it)->Visibility){
//            return (*it)->ID;
//        }
//    }
//
//    return 0;   // there are no visible actors atm
//}


///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrMaskRectangleCmd::CanUndo(){
    return false;
}

bool FrMaskRectangleCmd::Undo(){
    return false;
}

bool FrMaskRectangleCmd::Redo(){
    return false;
}
