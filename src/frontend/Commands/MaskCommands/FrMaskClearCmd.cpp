#include "FrMaskClearCmd.h"
#include "FrMainDocument.h"
#include "FrRoiDocObj.h"
#include "FrMainWindow.h"
#include "FrBaseView.h"

#include "RtMaskImage.h"

// VTK stuff
#include "vtkImageData.h"
#include "vtkImageLogic.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"

#define DEF_CLEAR_COLOR 0

FrMaskClearCmd::FrMaskClearCmd(){
}

FrMaskClearCmd::~FrMaskClearCmd(){
}

bool FrMaskClearCmd::Execute(){
    if(!this->GetMainController()) return false;

    bool result = false;

    FrRoiDocObj* roiDO = this->GetCurrentRoi();
    if(roiDO){
        // get mask image and init it to zeros
        roiDO->GetMaskImage()->initToZeros();


        //vtkImageData* imageData = this->GetRoiImageData(roiDO->GetID());

        //if(imageData){
        //    FrMainWindow* mv = this->GetMainController()->GetMainView();

        //    unsigned char* imgPtr = (unsigned char*)imageData->GetScalarPointer();

        //    //memcpy(imgPtr, 0, sizeof(imgPtr));

        //    // get data dimensions
        //    int dims[3];
        //    imageData->GetDimensions(dims);
        //    int xmin, xmax, ymin, ymax;
        //    xmin = ymin = 0;
        //    xmax = dims[0];
        //    ymax = dims[1];

        //    for (int x = xmin; x<xmax; x++) {
        //        for (int y = ymin; y<ymax; y++){
        //            int pos[3];
        //            pos[0] = x; pos[1] = y; pos[2] = 0;
        //            
        //            int id = imageData->ComputePointId(pos); 
        //            if (id >= 0){
        //                imgPtr[id] = DEF_CLEAR_COLOR;
        //            }
        //        }
        //    }

        //    this->ApplyDataToRoi(imageData, roiDO);

            //FrMainWindow* mv = this->GetMainController()->GetMainView();
            //mv->GetCurrentView()->UpdatePipeline(FRP_READ);
            FrBaseCmd::UpdatePipelineForID(ALL_LAYER_ID, FRP_FULL);    

            result = true;
        //}
    }

    return result;
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrMaskClearCmd::CanUndo(){
    return false;
}

bool FrMaskClearCmd::Undo(){
    return false;
}

bool FrMaskClearCmd::Redo(){
    return false;
}