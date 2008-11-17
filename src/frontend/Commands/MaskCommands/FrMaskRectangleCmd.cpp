#include "FrMaskRectangleCmd.h"
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
#include "vtkCoordinate.h"
#include "vtkRenderer.h"


FrMaskRectangleCmd::FrMaskRectangleCmd(){
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

bool FrMaskRectangleCmd::DrawMask(){
    bool result = false;

    // get special layer and set selection params
    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrMainDocument* md = this->GetMainController()->GetMainDocument();
    FrTabSettingsDocObj* ts = md->GetCurrentTabSettings();

    FrSpecialLayer* sl;
    
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
    params.type = 1;
    params.firstPoint = m_Rect.firstPoint;
    params.secondPoint = m_Rect.secondPoint;

    sl->SetSelection(params);
    mv->GetCurrentView()->UpdatePipeline(FRP_SETCAM);

    return result;
}

bool FrMaskRectangleCmd::WriteMask(){
    bool result = false;

    //FrMainWindow* mv = this->GetMainController()->GetMainView();
    //mv->GetSliceView()->UpdatePipeline(FRP_SETCAM);
    // TODO: hide selection

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

            int firstPoint[3];
            firstPoint[0] = m_Rect.firstPoint.x;
            firstPoint[1] = m_Rect.firstPoint.y;

            int secondPoint[3];
            secondPoint[0] = m_Rect.secondPoint.x;
            secondPoint[1] = m_Rect.secondPoint.y;

            GetRealImagePosition(ts, imageData, firstPoint, m_ImageNumber);
            GetRealImagePosition(ts, imageData, secondPoint, m_ImageNumber);

            int xmin, xmax, ymin, ymax, zmin, zmax;
            xmin = ymin = zmin = 0;    

            switch(m_ImageNumber){
                case 0:             // x fixed
                    xmin = xmax = firstPoint[0];
                    ymin = Min(firstPoint[0], secondPoint[0]);
                    ymax = Max(firstPoint[0], secondPoint[0]);
                    zmin = Min(firstPoint[1], secondPoint[1]);
                    zmax = Max(firstPoint[1], secondPoint[1]);
                    break;
                case 1:             // y fixed
                    ymin = ymax = firstPoint[1];
                    xmin = Min(firstPoint[0], secondPoint[0]);
                    xmax = Max(firstPoint[0], secondPoint[0]);
                    zmin = Min(firstPoint[1], secondPoint[1]);
                    zmax = Max(firstPoint[1], secondPoint[1]);
                    break;
                case -1:
                case 2:             // z fixed
                    zmin = zmax = firstPoint[2];
                    xmin = Min(firstPoint[0], secondPoint[0]);
                    xmax = Max(firstPoint[0], secondPoint[0]);
                    ymin = Min(firstPoint[1], secondPoint[1]);
                    ymax = Max(firstPoint[1], secondPoint[1]);
                    break;
            }
            
            unsigned char* imgPtr = (unsigned char*)imageData->GetScalarPointer();

            for (int x = xmin; x<xmax; x++)
                for (int y = ymin; y<ymax; y++)
                    for (int z = zmin; z<zmax; z++){
                        int id = imageData->FindPoint(x, y, z);
                        if (id>=0)      // point found
                            imgPtr[id] = pixelValue;
                    }

            this->ApplyDataToRoi(imageData, roiDO);
            result = true;
        }
    }

    return result;
}

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
