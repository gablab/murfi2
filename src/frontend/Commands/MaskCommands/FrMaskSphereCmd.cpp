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

            unsigned char* imgPtr = (unsigned char*)imageData->GetScalarPointer();

            for (int x = xmin; x<xmax; x++)
                for (int y = ymin; y<ymax; y++)
                    for (int z = zmin; z<zmax; z++){
                        // check if point is inside of sphere and if yes then write it
                        Pos p;
                        p.x = x;    p.y = y;    p.z = z;
                        if(IsPointInsideOfSphere(m_Center, m_Radius, p)){
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
