#include "FrRoiTool.h"
#include "FrToolController.h"
#include "FrInteractorStyle.h"
#include "FrCommandController.h"
#include "FrRectangleTool.h"
#include "FrFreeShapeTool.h"
#include "FrSliceView.h"
#include "FrMosaicView.h"
#include "FrOrthoView.h"
#include "FrLayeredImage.h"
#include "FrMainDocument.h"
#include "FrMainWindow.h"
#include "FrUtils.h"

// VTK stuff
#include "vtkPointPicker.h"
#include "vtkRenderer.h"
#include "vtkCoordinate.h"

#include "Qt/qstring.h"
#include "Qt/qmessagebox.h"

#define DEF_TOLERANCE 0.0


FrRoiTool::FrRoiTool(){
    m_maskRectTool = new FrRectangleTool();
    m_maskFSTool = new FrFreeShapeTool();

    m_PointPicker = vtkPointPicker::New();
    m_PointPicker->SetTolerance(DEF_TOLERANCE);
}

FrRoiTool::~FrRoiTool(){
    if (m_maskRectTool) delete m_maskRectTool;
    if (m_maskFSTool) delete m_maskFSTool;

    if (m_PointPicker) m_PointPicker->Delete();
}

void FrRoiTool::Start(){
   // Setup controller
    m_maskRectTool->SetController(this->GetController());
    m_maskFSTool->SetController(this->GetController());

    m_maskRectTool->Start();
    m_maskFSTool->Start();

    // Update interface to ensure tool is checked
    FrManageToolCmd* cmd = FrCommandController::CreateCmd<FrManageToolCmd>();
    cmd->SetToolType(FrManageToolCmd::RoiTool);
    cmd->SetToolAction(FrManageToolCmd::UpdateAct);
    cmd->SetIsChecked(true);
    cmd->Execute();
    delete cmd;
}

void FrRoiTool::Stop(){
    // Unregister controller
    m_maskRectTool->SetController(0);
    m_maskFSTool->SetController(0);

    m_maskRectTool->Stop();
    m_maskFSTool->Stop();

    // Update interface to ensure tool is unchecked
    FrManageToolCmd* cmd = FrCommandController::CreateCmd<FrManageToolCmd>();
    cmd->SetToolType(FrManageToolCmd::RoiTool);
    cmd->SetToolAction(FrManageToolCmd::UpdateAct);
    cmd->SetIsChecked(false);
    cmd->Execute();
    delete cmd;
}

bool FrRoiTool::OnMouseUp(FrInteractorStyle* is, FrMouseParams& params){
    if(params.Button == FrMouseParams::LeftButton){
        //if (params.X != -1)
        bool isInside = GetMappedCoords(is, params);
        //m_maskRectTool->OnMouseUp(is, params);
        m_maskFSTool->OnMouseUp(is, params);
        m_maskFSTool->SetImageNumber(imgNumber);
    }

    return false;
}

bool FrRoiTool::OnMouseDown(FrInteractorStyle* is, FrMouseParams& params){
    if(params.Button == FrMouseParams::LeftButton){
            bool isInside = GetMappedCoords(is, params);
        //if (params.X != -1)
//            m_maskRectTool->OnMouseDown(is, params);
            m_maskFSTool->OnMouseDown(is, params);
            m_maskFSTool->SetImageNumber(imgNumber);
    }

    return false;
}

bool FrRoiTool::OnMouseMove(FrInteractorStyle* is, FrMouseParams& params){
    bool isInside = GetMappedCoords(is, params);
    //if (params.X != -1)
//            m_maskRectTool->OnMouseMove(is, params);
        m_maskFSTool->OnMouseMove(is, params);
        m_maskFSTool->SetImageNumber(imgNumber);

    return false;
}

bool FrRoiTool::OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params){
    if(params.Button == FrMouseParams::LeftButton){
        bool isInside = GetMappedCoords(is, params);
        //if (params.X != -1)
//            m_maskRectTool->OnMouseDrag(is, params);
            m_maskFSTool->OnMouseDrag(is, params);
            m_maskFSTool->SetImageNumber(imgNumber);
    }
    
    return false;
}

bool FrRoiTool::GetMappedCoords(FrInteractorStyle* is, FrMouseParams& params){
    FrMainController* mc = dynamic_cast<FrMainController*>(this->GetController()->GetOwner());
    FrMainWindow* mv = mc->GetMainView();
    FrMainDocument* md = mc->GetMainDocument();
    FrTabSettingsDocObj* ts = md->GetCurrentTabSettings();       

    std::vector<vtkRenderer*> renCollection;
    //int imgNumber = -1;
    FrLayeredImage* lim = 0;
    std::vector<FrLayerSettings*> layers;

    enum FrTabSettingsDocObj::View view = ts->GetActiveView();
    switch(view){
        case FrTabSettingsDocObj::SliceView:
            mv->GetSliceView()->GetImage()->GetRenderers(renCollection);
            lim = mv->GetSliceView()->GetImage();
            GetLayerSettings(ts->GetSliceViewSettings(), layers);
            imgNumber = -1;
            break;
        case FrTabSettingsDocObj::MosaicView:
            mv->GetMosaicView()->GetImage()->GetRenderers(renCollection);
            lim = mv->GetMosaicView()->GetImage();
            GetLayerSettings(ts->GetMosaicViewSettings(), layers);
            imgNumber = -1;
            break;
        case FrTabSettingsDocObj::OrthoView:
            {
                FrOrthoView* ov =  mv->GetOrthoView();

                // Find Image where click's occured
                for(int i=0; i < ORTHO_IMAGE_COUNT; ++i){
                    if (ov->GetImage(i)->GetRenderer()->IsInViewport(params.X, params.Y)){
                        imgNumber = i; 
                        break;
                    }
                }
                if (imgNumber != -1){
                    ov->GetImage(imgNumber)->GetRenderers(renCollection);
                    lim = ov->GetImage(imgNumber);
                    GetLayerSettings(ts->GetOrthoViewSettings(), layers, imgNumber);
                }
                else{
                    // do something
                    return false;
                }
                break;
            }
    } // end switch(view)

    // we should get any layer with visible actor
    int visibleLayerNum = GetVisibleLayer(layers);
    vtkRenderer* renderer = renCollection[visibleLayerNum];	    

    bool isInside = true;
    if (!m_PointPicker->Pick(params.X, params.Y, 0, renderer)){
        // point is not inside of image actor
        isInside = false;
    }
//
//    // Get the mapped position of the mouse using the picker.
//    double ptMapped[3];
//    m_PointPicker->GetMapperPosition(ptMapped);
//
//    //if (notInside){
//    //    ptMapped[0] = ptMapped[1] = ptMapped[2] = -1;
//    //}
//
////    params.X = ptMapped[0];
////    params.Y = ptMapped[1];

    // test
    double* point;

    vtkCoordinate* coordinate = vtkCoordinate::New();
    coordinate->SetCoordinateSystemToDisplay();
    coordinate->SetValue(params.X, params.Y, 0.0);
    point = coordinate->GetComputedWorldValue(renderer);

    params.X = int(point[0]);
    params.Y = int(point[1]);

    coordinate->Delete();
    
    return isInside;
}

int FrRoiTool::GetVisibleLayer(std::vector<FrLayerSettings*> layers){
    std::vector<FrLayerSettings*>::iterator it, itEnd(layers.end());
    for (it = layers.begin(); it != itEnd; ++it){
        if ((*it)->Visibility){
            return (*it)->ID;
        }
    }

    return 0;   // there are no visible actors atm
}