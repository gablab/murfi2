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
    bool result = false;

    int pixelValue = 0;

    switch (m_Action){
        case FrMaskPenCmd::Erase:
            pixelValue = 0;
            break;
        case FrMaskPenCmd::Draw:
        case FrMaskPenCmd::Write:
            pixelValue = 255;
            break;
    }

    FrRoiDocObj* roiDO = this->GetCurrentRoi();
    if(roiDO){
        vtkImageData* imageData = this->GetRoiImageData(roiDO->GetID());

        if(imageData){
            FrMainWindow* mv = this->GetMainController()->GetMainView();
            FrMainDocument* md = this->GetMainController()->GetMainDocument();
            FrTabSettingsDocObj* ts = md->GetCurrentTabSettings();  

            // get data dimensions
            int dims[3];
            imageData->GetDimensions(dims);

            int center[3];
            center[0] = m_Center.x;
            center[1] = m_Center.y;

            GetRealImagePosition(ts, imageData, center, m_ImageNumber);    

            Pos new_center;
            new_center.x = center[0];
            new_center.y = center[1];
            new_center.z = 0;

            int xmin, xmax, ymin, ymax;
            xmin = new_center.x - m_Radius;  
            ymin = new_center.y - m_Radius; 
            xmax = new_center.x + m_Radius;  
            ymax = new_center.y + m_Radius; 

            unsigned char* imgPtr = (unsigned char*)imageData->GetScalarPointer();

            for (int x = xmin; x<xmax; x++)
                for (int y = ymin; y<ymax; y++){
                    // check if point is inside of circle and if yes then write it
                    Pos pos;
                    pos.x = x;  pos.y = y; pos.z = 0;

                    if (IsPointInsideOfSphere(new_center, m_Radius, pos)){
                        int point[3];
                        point[0] = x;     point[1] = y;     point[2] = 0;
                        int id = imageData->ComputePointId(point);
                        if (id>=0)      // point found
                            imgPtr[id] = pixelValue;
                    }
                }

            this->ApplyDataToRoi(imageData, roiDO);
            mv->GetCurrentView()->UpdatePipeline(FRP_READROI);

            result = true;
        }
    }

    return result;
}

bool FrMaskPenCmd::WriteMask(){
    bool result = false;

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
