#include "FrMaskPenCmd.h"
#include "FrTabSettingsDocObj.h"
#include "FrMainDocument.h"
#include "FrMainWindow.h"
#include "FrLayeredImage.h"
#include "FrBaseView.h"
#include "FrMosaicView.h"
#include "FrSliceView.h"
#include "FrOrthoView.h"
#include "FrUtils.h"
#include "FrRoiDocObj.h"
#include "FrViewDocObj.h"

// VTK stuff
#include "vtkImageData.h"
#include "vtkImageLogic.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"


FrMaskPenCmd::FrMaskPenCmd(){
}

FrMaskPenCmd::~FrMaskPenCmd(){
}

bool FrMaskPenCmd::Execute(){
    if(!this->GetMainController()) return false;

    bool result = false;

    switch (m_Action){
        case FrMaskPenCmd::Draw:
        case FrMaskPenCmd::Erase:
            result = DrawMask();
            break;
        //case FrMaskPenCmd::Erase:
        case FrMaskPenCmd::Write:
            result = WriteMask();
            break;
        case FrMaskPenCmd::Undefined:
            result = false;
            break;
    }

    return result;
}

bool FrMaskPenCmd::DrawMask(){
    if(m_Radius <= 0) return false;
    
    int pixelValue = 0;
    switch (m_Action){
        case FrMaskPenCmd::Erase:
            pixelValue = 0;
            break;
        case FrMaskPenCmd::Draw:
        case FrMaskPenCmd::Write:
            pixelValue = 255;
            break;
        default:break;
    }
    
    bool result = false;
    FrRoiDocObj* roiDO = this->GetCurrentRoi();
    if(roiDO){
        vtkImageData* imageData = this->GetRoiImageData(roiDO->GetID());

        if(imageData){
            FrMainWindow* mv = this->GetMainController()->GetMainView();

            int point[2];
            point[0] = m_Center.x;
            point[1] = m_Center.y;
            this->TransformCoordinatesToIndices(point, imageData, m_ImageNumber);

            Pos center;
            center.x = point[0];
            center.y = point[1];
            center.z = 0;

            int xmin, xmax, ymin, ymax;
            xmin = center.x - m_Radius;  
            ymin = center.y - m_Radius; 
            xmax = center.x + m_Radius;  
            ymax = center.y + m_Radius; 

            unsigned char* imgPtr = (unsigned char*)imageData->GetScalarPointer();

            for (int x = xmin; x < xmax; x++){
                for (int y = ymin; y < ymax; y++){
                    // check if point is inside of circle and if so then write it
                    Pos pos;
                    pos.x = x;  pos.y = y; pos.z = 0;

                    if (IsPointInsideOfSphere(center, m_Radius, pos)){
                        int point[3];
                        point[0] = x; point[1] = y; point[2] = 0;
                        
                        int id = imageData->ComputePointId(point);
                        if (id >= 0)
                        {
                            imgPtr[id] = pixelValue;
                        }
                    }

                }
            }
             
            this->ApplyDataToRoi(imageData, roiDO);
            mv->GetCurrentView()->UpdatePipeline(FRP_READ);

            result = true;
        }
    }

    return result;
}

bool FrMaskPenCmd::WriteMask(){
    bool result = false;

    // NOTE: do nothing here
    //FrRoiDocObj* roiDO = this->GetCurrentRoi();
    //if(roiDO){
    //    vtkImageData* imageData = this->GetRoiImageData(roiDO->GetID());
    //    this->ApplyDataToRoi(imageData, roiDO);
    //        
    //    FrMainWindow* mv = this->GetMainController()->GetMainView();
    //    mv->GetCurrentView()->UpdatePipeline(FRP_READROI);
    //    
    //    result = true;
    //}

    return result;
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrMaskPenCmd::CanUndo(){
    return false;
}

bool FrMaskPenCmd::Undo(){
    return false;
}

bool FrMaskPenCmd::Redo(){
    return false;
}
