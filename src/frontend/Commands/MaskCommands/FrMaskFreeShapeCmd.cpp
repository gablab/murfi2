#include "FrMaskFreeShapeCmd.h"
#include "FrMainWindow.h"
#include "FrLayeredImage.h"
#include "FrBaseView.h"
#include "FrMosaicView.h"
#include "FrSliceView.h"
#include "FrOrthoView.h"
#include "FrUtils.h"
#include "FrTabSettingsDocObj.h"
#include "FrMainDocument.h"
#include "FrRoiDocObj.h"

// VTK stuff
#include "vtkImageData.h"
#include "vtkImageLogic.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"


FrMaskFreeShapeCmd::FrMaskFreeShapeCmd(){
}

FrMaskFreeShapeCmd::~FrMaskFreeShapeCmd(){
}

bool FrMaskFreeShapeCmd::Execute(){
    if(!this->GetMainController()) return false;
    bool result = false;

    switch (m_Action){
        case Action::Draw:
            result = DrawMask(true);
            break;
        case Action::Erase:
        case Action::Write:
            result = WriteMask() && DrawMask(false);
            break;
        case Action::Undefined:
            result = false;
            break;
    }

    return result;
}

bool FrMaskFreeShapeCmd::DrawMask(bool show){
    bool result = false;   

    // get special layer and set selection params
    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrSpecialLayer* sl = this->GetSpecialLayer();
    
    // set params
    if(show){
        SelectionParams params;
        params.type = 3;                // polygon
        params.points = m_Points;

        sl->SetSelection(params);
        sl->SetSelectionVisibility(true);
        result = true;
    }
    else{
        sl->SetSelectionVisibility(false);
        result = true;
    }
    mv->GetCurrentView()->UpdatePipeline(FRP_SETCAM);

    return result;
}

bool FrMaskFreeShapeCmd::WriteMask(){
    bool result = false;

    //mv->GetSliceView()->UpdatePipeline(FRP_SETCAM);
    // TODO: hide selection
    int pixelValue = 0;

    switch (m_Action){
        case Action::Erase:
            pixelValue = 0;
            break;
        case Action::Write:
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

            int xmin, xmax, ymin, ymax;
            xmin = ymin = 0;    
            xmax = dims[0];
            ymax = dims[1];

            // translate all points to local image coordinates
            for (int i = 0; i<m_Points.size(); i++){
                int p[3];
                p[0] = m_Points[i].x;   p[1] = m_Points[i].y;   
                GetRealImagePosition(ts, imageData, p, m_ImageNumber);    

                m_Points[i].x = p[0];   m_Points[i].y = p[1];   
            }

            unsigned char* imgPtr = (unsigned char*)imageData->GetScalarPointer();

            for (int x = xmin; x<xmax; x++)
                for (int y = ymin; y<ymax; y++){
                    // test if point is inside of polygon, if yes, find index and write it
                    Pos p;
                    p.x = x;    p.y = y;    p.z = 0;
                    
                    if (IsPointInsideOfPolygon(m_Points, p)){
                        int pos[3];
                        pos[0] = x;     pos[1] = y;     pos[2] = 0;
                        int id = imageData->ComputePointId(pos);
                        imgPtr[id] = pixelValue;
                    }
                }

            this->ApplyDataToRoi(imageData, roiDO);

            //FrMainWindow* mv = this->GetMainController()->GetMainView();
            mv->GetCurrentView()->UpdatePipeline(FRP_READROI);

            result = true;
        }
    }

    return result;
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrMaskFreeShapeCmd::CanUndo(){
    return false;
}

bool FrMaskFreeShapeCmd::Undo(){
    return false;
}

bool FrMaskFreeShapeCmd::Redo(){
    return false;
}
