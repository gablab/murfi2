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
#include "FrViewDocObj.h"
#include "FrDocumentReader.h"

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

    switch (m_Action){
        case Action::DrawSelection:
            result = this->DrawMask(true);
            break;
        case Action::HideSelection:
            result = this->DrawMask(false);
            break;
        case Action::Erase:
        case Action::Write:
            result = this->WriteMask() && this->DrawMask(false);
            break;
        case Action::Undefined:
            result = false;
            break;
    }

    return result;
}

bool FrMaskSphereCmd::DrawMask(bool show){
    bool result = false;

    // get special layer and set selection params
    FrMainWindow* mv = this->GetMainController()->GetMainView();
    FrSpecialLayer* sl = this->GetSpecialLayer();
    
    if (!sl)
        return false;

    if(show){
        // set params
        SelectionParams params;
        params.type = 2;        // sphere
        params.center = m_Center;
        params.radius = m_Radius;

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

bool FrMaskSphereCmd::WriteMask(){
    // Nothing to draw
    if(m_Radius <= 0) return false;

    bool result = false;
    FrRoiDocObj* roiDO = this->GetCurrentRoi();
    if(roiDO){

        FrMainDocument* doc = this->GetMainController()->GetMainDocument();

        FrViewDocObj* viewDO = 0L;
        FrDocument::DocObjCollection views;
        doc->GetObjectsByType(views, FrDocumentObj::ViewObject);    
        if(views.size() > 0){
            viewDO = (FrViewDocObj*)views[0];
        }
        
        // Create doc reader
        FrDocumentReader* docReader = FrDocumentReader::New();
        docReader->SetDocument(doc);
        docReader->SetTarget(FrDocumentReader::Roi);
        docReader->SetDataID(roiDO->GetID());

        // setup doc reader and other params
        int midSlice = this->GetCurrentRoiSliceNumber();
        int startSlice = midSlice - m_Radius;
        int endSlice = midSlice + m_Radius;
        
        switch(viewDO->GetActiveView()){
            case SliceView:
                docReader->SetMosaic(false);
                docReader->SetOrientation(FrDocumentReader::XY);
                break;
            case MosaicView:
                // have to draw just one slice
                startSlice = midSlice;
                endSlice = midSlice + 1;
                docReader->SetMosaic(true);
                docReader->SetOrientation(FrDocumentReader::XY);
                break;
            case OrthoView:
                docReader->SetMosaic(false);
                switch(m_ImageNumber)
                {
                    case DEF_CORONAL: docReader->SetOrientation(FrDocumentReader::XZ); break;
                    case DEF_SAGITAL: docReader->SetOrientation(FrDocumentReader::YZ); break;
                    case DEF_AXIAL: docReader->SetOrientation(FrDocumentReader::XY); break;
                }
                break;
            default:
                docReader->Delete();
                return false;
        }       
        
        // Draw circles 
        for (int slice = startSlice; slice < endSlice; slice++){
            // do not allow to process negative slices
            if(slice < 0) continue;

            docReader->SetSlice(slice);
            docReader->Update();

            // Since doc reader may truncate slice numbers 
            // which are out of range check it and stop
            if(slice > docReader->GetSlice()){
                break;
            }

            vtkImageData* data = docReader->GetOutput();
            int radius = GetCircleRadiusForSlice(slice);

            this->DrawCircle(data, radius);
            this->ApplyDataToRoi(data, roiDO, slice);
            result = true;
        }

        // Delete doc reader
        docReader->Delete();

        FrMainWindow* mv = this->GetMainController()->GetMainView();
        mv->GetCurrentView()->UpdatePipeline(FRP_READ);
    }   

    return result;
}

void FrMaskSphereCmd::DrawCircle(vtkImageData* imageData, int radius){
    if(radius <= 0) return;
    
    int pixelValue = 0;
    switch (m_Action){
        case FrMaskSphereCmd::Erase:
            pixelValue = 0;
            break;
        case FrMaskSphereCmd::Write:
            pixelValue = 255;
            break;
    }
    
    int center[2];
    center[0] = m_Center.x;
    center[1] = m_Center.y;
    this->TransformCoordinatesToIndices(center, imageData, m_ImageNumber);

    Pos new_center;
    new_center.x = center[0];
    new_center.y = center[1];
    new_center.z = 0;

    int xmin, xmax, ymin, ymax;
    xmin = new_center.x - radius;
    ymin = new_center.y - radius; 
    xmax = new_center.x + radius;
    ymax = new_center.y + radius;

    unsigned char* imgPtr = (unsigned char*)imageData->GetScalarPointer();

    for (int x = xmin; x < xmax; x++)
        for (int y = ymin; y < ymax; y++){
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
    int midSlice = this->GetCurrentRoiSliceNumber();
    int delta = abs(midSlice - slice);
    
    return int(sqrt(double(m_Radius * m_Radius - delta * delta)));
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
