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
#include "FrViewDocObj.h"

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
        case FrMaskRectangleCmd::DrawSelection:
            result = this->DrawMask(true);
            break;
        case FrMaskRectangleCmd::HideSelection:
            result = this->DrawMask(false);
            break;
        case FrMaskRectangleCmd::Erase:
        case FrMaskRectangleCmd::Write:
            result = this->WriteMask() && this->DrawMask(false);
            break;
        case FrMaskRectangleCmd::Undefined:
            result = false;
            break;
    }

    return result;
}

bool FrMaskRectangleCmd::DrawMask(bool show){
    bool result = false;

    // get special layer and set selection params
    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrSpecialLayer* sl = this->GetSpecialLayer();

    if(show){
        // set params
        SelectionParams params;
        params.type = 1;
        params.firstPoint = m_Rect.firstPoint;
        params.secondPoint = m_Rect.secondPoint;

        sl->SetSelection(params);
        sl->SetSelectionVisibility(true);
        mv->GetCurrentView()->UpdatePipeline(FRP_SETCAM);
        result = true;
    }
    else {
        sl->SetSelectionVisibility(false);
        mv->GetCurrentView()->UpdatePipeline(FRP_SETCAM);
        result = true;
    }

    return result;
}

bool FrMaskRectangleCmd::WriteMask(){
    bool result = false;

    //FrMainWindow* mv = this->GetMainController()->GetMainView();
    //mv->GetSliceView()->UpdatePipeline(FRP_SETCAM);
    // TODO: hide selection

    int pixelValue = 0;

    switch (m_Action){
        case FrMaskRectangleCmd::Erase:
            pixelValue = 0;
            break;
        case FrMaskRectangleCmd::Write:
            pixelValue = 255;
            break;
    }

    FrRoiDocObj* roiDO = this->GetCurrentRoi();
    if(roiDO){
        vtkImageData* imageData = this->GetRoiImageData(roiDO->GetID());

        if(imageData){
            FrMainWindow* mv = this->GetMainController()->GetMainView();
            FrMainDocument* doc = this->GetMainController()->GetMainDocument();

            FrViewDocObj* viewDO = 0L;
            FrDocument::DocObjCollection views;
            doc->GetObjectsByType(views, FrDocumentObj::ViewObject);    
            if(views.size() > 0){
                viewDO = (FrViewDocObj*)views[0];
            }

            // get data dimensions
            int dims[3];
            imageData->GetDimensions(dims);

            int firstPoint[3];
            firstPoint[0] = m_Rect.firstPoint.x;
            firstPoint[1] = m_Rect.firstPoint.y;

            int secondPoint[3];
            secondPoint[0] = m_Rect.secondPoint.x;
            secondPoint[1] = m_Rect.secondPoint.y;

            GetRealImagePosition(viewDO, imageData, firstPoint, m_ImageNumber);
            GetRealImagePosition(viewDO, imageData, secondPoint, m_ImageNumber);

            int xmin, xmax, ymin, ymax;
            //xmin = Min(m_Rect.firstPoint.x, m_Rect.secondPoint.x);
            //xmax = Max(m_Rect.firstPoint.x, m_Rect.secondPoint.x);
            //ymin = Min(m_Rect.firstPoint.y, m_Rect.secondPoint.y);
            //ymax = Max(m_Rect.firstPoint.y, m_Rect.secondPoint.y);

            xmin = std::min(firstPoint[0], secondPoint[0]);
            xmax = std::max(firstPoint[0], secondPoint[0]);
            ymin = std::min(firstPoint[1], secondPoint[1]);
            ymax = std::max(firstPoint[1], secondPoint[1]);

            unsigned char* imgPtr = (unsigned char*)imageData->GetScalarPointer();

            for (int x = xmin; x<xmax; x++)
                for (int y = ymin; y<ymax; y++){
                    int pos[3];
                    pos[0] = x;     pos[1] = y;     pos[2] = 0;
                    int id = imageData->ComputePointId(pos); 
                    if (id>=0)      // point found
                        imgPtr[id] = pixelValue;
                    }

            this->ApplyDataToRoi(imageData, roiDO);

            //FrMainWindow* mv = this->GetMainController()->GetMainView();
            mv->GetCurrentView()->UpdatePipeline(FRP_READ);

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
