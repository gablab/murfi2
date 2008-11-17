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
        case Action::Draw:
            result = DrawMask();
            break;
        case Action::Erase:
        case Action::Write:
            result = WriteMask();
            break;
        case Action::Undefined:
            result = false;
            break;
    }

    return result;
}

bool FrMaskPenCmd::DrawMask(){
    bool result = false;

    int pixelValue = 0;

    switch (m_Action){
        case Action::Erase:
            pixelValue = 0;
            break;
        case Action::Write:
            pixelValue = 1;
            break;
    }    

    FrRoiDocObj* roiDO = this->GetCurrentRoi();
    if(roiDO){
        vtkImageData* imageData = this->GetRoiImageData(roiDO->GetID());
    
        if(imageData){
            FrMainWindow* mv = this->GetMainController()->GetMainView();
            FrMainDocument* md = this->GetMainController()->GetMainDocument();
            FrTabSettingsDocObj* ts = md->GetCurrentTabSettings();

            int dims[3];
            imageData->GetDimensions(dims);

            int center[3];
            center[0] = m_Center.x;
            center[1] = m_Center.y;

            GetRealImagePosition(ts, imageData, center, m_ImageNumber);

            int xmin, xmax, ymin, ymax, zmin, zmax;
            xmin = ymin = zmin = 0;    
            xmax = dims[0];
            ymax = dims[1];
            zmax = dims[2];

            switch(m_ImageNumber){
                case 0:             // x fixed
                    xmin = xmax = center[0];
                    break;
                case 1:             // y fixed
                    ymin = ymax = center[1];
                    break;
                case -1:
                case 2:             // z fixed
                    zmin = zmax = center[2];
                    break;
            }
            
            unsigned char* imgPtr = (unsigned char*)imageData->GetScalarPointer();

            for (int x = xmin; x<xmax; x++)
                for (int y = ymin; y<ymax; y++)
                    for (int z = zmin; z<zmax; z++){
                        // check if point is inside of circle and if yes then write it
                        Pos pos;
                        pos.x = x;  pos.y = y; pos.z = z;

                        if (IsPointInsideOfSphere(m_Center, m_Radius, pos)){
                            int id = imageData->FindPoint(x, y, z);
                            if (id>=0)      // point found
                                imgPtr[id] = pixelValue;
                        }
                    }

            result = true;
        }
    }
    
    return result;
}

bool FrMaskPenCmd::WriteMask(){
    bool result = false;
    
    FrRoiDocObj* roiDO = this->GetCurrentRoi();
    if(roiDO){
        vtkImageData* imageData = this->GetRoiImageData(roiDO->GetID());
        FrMaskBaseCmd::ApplyDataToRoi(imageData, roiDO);

        result = true;
    }

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
