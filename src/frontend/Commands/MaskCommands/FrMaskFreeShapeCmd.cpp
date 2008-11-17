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

bool FrMaskFreeShapeCmd::DrawMask(){
    bool result = false;   

    // get special layer and set selection params
    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrMainDocument* md = this->GetMainController()->GetMainDocument();
    FrTabSettingsDocObj* ts = md->GetCurrentTabSettings();

    FrSpecialLayer* sl;
    //int imgNumber = -1;
    
    enum FrTabSettingsDocObj::View view = ts->GetActiveView();
    switch(view){
        case FrTabSettingsDocObj::SliceView:
            sl = mv->GetSliceView()->GetImage()->GetSpecialLayer();
            break;
        case FrTabSettingsDocObj::MosaicView:
            sl = mv->GetMosaicView()->GetImage()->GetSpecialLayer();
            break;
        case FrTabSettingsDocObj::OrthoView:
            {
                FrOrthoView* ov =  mv->GetOrthoView();

                if (m_ImageNumber != -1){
                    sl = ov->GetImage(m_ImageNumber)->GetSpecialLayer();
                }
                else{
                    return false;
                }
                break;
            }
    } // end switch(view)

    // set params
    SelectionParams params;
    params.type = 3;                // polygon
    params.points = m_Points;

    sl->SetSelection(params);

    mv->GetSliceView()->UpdatePipeline(FRP_SETCAM);

    return result;
}

bool FrMaskFreeShapeCmd::WriteMask(){
    bool result = false;

    //mv->GetSliceView()->UpdatePipeline(FRP_SETCAM);
    // TODO: hide selection

    FrRoiDocObj* roiDO = this->GetCurrentRoi();
    if(roiDO){
        vtkImageData* imageData = this->GetRoiImageData(roiDO->GetID()); 
    
        // TODO: not finished

        FrMainWindow* mv = this->GetMainController()->GetMainView();
        FrMainDocument* md = this->GetMainController()->GetMainDocument();
        FrTabSettingsDocObj* ts = md->GetCurrentTabSettings();
        
        int dims[3];
        imageData->GetDimensions(dims);

        int point[3];
        point[0] = m_Points[0].x;
        point[1] = m_Points[1].y;
        point[2] = m_Points[2].z;

        GetRealImagePosition(ts, imageData, point, m_ImageNumber);
        // we should change all points to real coordinates

        int xmin, xmax, ymin, ymax, zmin, zmax;
        xmin = ymin = zmin = 0;    
        xmax = dims[0];
        ymax = dims[1];
        zmax = dims[2];

        switch(m_ImageNumber){
            case 0:             // x fixed
                xmin = xmax = point[0];
                break;
            case 1:             // y fixed
                ymin = ymax = point[1];
                break;
            case -1:
            case 2:             // z fixed
                zmin = zmax = point[2];
                break;
        }
        
        unsigned char* imgPtr = (unsigned char*)imageData->GetScalarPointer();

        for (int x = xmin; x<xmax; x++)
            for (int y = ymin; y<ymax; y++)
                for (int z = zmin; z<zmax; z++){
                    // test if point is inside of polygon, if yes, find index and write it
                    Pos p;
                    p.x = x;    p.y = y;    p.z = z;
                    if (IsPointInsideOfPolygon(m_Points, p)){
                        int id = imageData->FindPoint(x, y, z);
                        imgPtr[id] = 1;
                    }
                }

        this->ApplyDataToRoi(imageData, roiDO);
        result = true;
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
