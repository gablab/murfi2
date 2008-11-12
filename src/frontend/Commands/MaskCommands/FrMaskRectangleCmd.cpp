#include "FrMaskRectangleCmd.h"
#include "FrMainWindow.h"
#include "FrLayeredImage.h"
#include "FrBaseView.h"
#include "FrMosaicView.h"
#include "FrSliceView.h"
#include "FrOrthoView.h"

// VTK stuff
#include "vtkImageData.h"
#include "vtkImageLogic.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"
#include "vtkPointPicker.h"
#include "vtkCoordinate.h"
#include "vtkRenderer.h"


FrMaskRectangleCmd::FrMaskRectangleCmd()
: m_PointPicker(0){
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
    // TODO: add support for all views
    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrSpecialLayer* sl = mv->GetSliceView()->GetImage()->GetSpecialLayer();

    // set params
    SelectionParams params;
    params.type = 1;
    params.firstPoint[0] = m_Rect.leftX;
    params.firstPoint[1] = m_Rect.leftY;
    params.secondPoint[0] = m_Rect.rightX;
    params.secondPoint[1] = m_Rect.rightY;

    sl->SetSelection(params);
    //char text[256] = "Left.x = ";
    //char tmp[20] = "";
    //itoa(m_Rect.leftX, tmp, 10);
    //strcat(text, tmp);
    //strcat(text, ", Left.y = ");
    //itoa(m_Rect.leftY, tmp, 10);
    //strcat(text, tmp);
    //strcat(text, ", Right.x = ");
    //itoa(m_Rect.rightX, tmp, 10);
    //strcat(text, tmp);
    //strcat(text, ", RIght.y = ");
    //itoa(m_Rect.rightY, tmp, 10);
    //strcat(text, tmp);

    //sl->SetText(text);
    mv->GetSliceView()->UpdatePipeline(FRP_SETCAM);

    return result;
}

bool FrMaskRectangleCmd::WriteMask(){
    bool result = false;

    //FrMainWindow* mv = this->GetMainController()->GetMainView();
    //mv->GetSliceView()->UpdatePipeline(FRP_SETCAM);
    // TODO: hide selection

    vtkImageData* imageData = this->GetCurrentROIImageData();
    if(imageData){
        // TODO: implement
        result = true;
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
