#include "FrMaskSphereCmd.h"
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


FrMaskSphereCmd::FrMaskSphereCmd(){
}

FrMaskSphereCmd::~FrMaskSphereCmd(){
}

bool FrMaskSphereCmd::Execute(){
    if(!this->GetMainController()) return false;

    bool result = false;

    FrRoiDocObj* roiDO = this->GetCurrentRoi();
    if(roiDO){
        vtkImageData* imageData = this->GetRoiImageData(roiDO->GetID());

        if(imageData){
            int midSlice = this->GetCurrentRoiSliceNumber(); 
            for (int slice = midSlice-m_Radius; slice < midSlice+m_Radius; slice++){
                int radius = GetCircleRadiusForSlice(slice);
                DrawCircle(imageData, radius);
                ApplyDataToRoi(imageData, roiDO, slice);
            }
            
            FrMainWindow* mv = this->GetMainController()->GetMainView();
            mv->GetCurrentView()->UpdatePipeline(FRP_READROI);

            result = true;
        }
    }
    
    return result;
}

void FrMaskSphereCmd::DrawCircle(vtkImageData* imageData, int radius){
    int pixelValue = 0;
            
    FrMainDocument* md = this->GetMainController()->GetMainDocument();
    FrTabSettingsDocObj* ts = md->GetCurrentTabSettings();  

    switch (m_Action){
        case Action::Erase:
            pixelValue = 0;
            break;
        case Action::Write:
            pixelValue = 255;
            break;
    }

    // get data dimensions 
    int dims[3];
    imageData->GetDimensions(dims);   

    int xmin, xmax, ymin, ymax;
    xmin = ymin = 0;    
    xmax = dims[0];
    ymax = dims[1];

    int center[3];
    center[0] = m_Center.x;
    center[1] = m_Center.y;

    GetRealImagePosition(ts, imageData, center, m_ImageNumber);    

    Pos new_center;
    new_center.x = center[0];
    new_center.y = center[1];
    new_center.z = 0;

    unsigned char* imgPtr = (unsigned char*)imageData->GetScalarPointer();

    for (int x = xmin; x<xmax; x++)
        for (int y = ymin; y<ymax; y++){
            // check if point is inside of circle and if yes then write it
            Pos p;
            p.x = x;    p.y = y;    p.z = 0;
            if(IsPointInsideOfSphere(new_center, radius, p)){
                int pos[3];
                pos[0] = x;     pos[1] = y;     pos[2] = 0;
                int id = imageData->ComputePointId(pos);
                if (id>=0)      // point found
                    imgPtr[id] = pixelValue;
            }
        }
}

int FrMaskSphereCmd::GetCircleRadiusForSlice(int slice){
    int radius;
    int midSlice = this->GetCurrentRoiSliceNumber();
    int d = abs(midSlice - abs(slice));

    radius = (int)sqrt(pow((double)m_Radius, 2) - pow((double)slice, 2));
    
    return radius;
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrMaskSphereCmd::CanUndo(){
    return false;
}

bool FrMaskSphereCmd::Undo(){
    return false;
}

bool FrMaskSphereCmd::Redo(){
    return false;
}
