#include "FrRoiTool.h"
#include "FrToolController.h"
#include "FrInteractorStyle.h"
#include "FrCommandController.h"
#include "FrSphereTool.h"
#include "FrPenTool.h"
#include "FrSliceView.h"
#include "FrMosaicView.h"
#include "FrOrthoView.h"
#include "FrLayeredImage.h"
#include "FrMainDocument.h"
#include "FrMainWindow.h"
#include "FrUtils.h"
#include "FrLayerListWidget.h"
#include "FrROIToolWidget.h"

#include "FrRoiInfoTool.h"
#include "FrPenTool.h"
#include "FrRectangleTool.h"
#include "FrFreeShapeTool.h"
#include "FrSphereTool.h"
#include "FrMaskMaskTool.h"
#include "FrInvertTool.h"
#include "FrDilatoneErosionTool.h"

#include "FrTBCTool.h"
#include "FrSliceScrollTool.h"

// VTK stuff
#include "vtkPointPicker.h"
#include "vtkRenderer.h"
#include "vtkCoordinate.h"

#include "Qt/qstring.h"
#include "Qt/qmessagebox.h"

#define DEF_TOLERANCE 0.0
// This correspond to XY slice
// It is important for proper work of tools
#define DEF_IMAGE_NUMBER 2

FrRoiTool::FrRoiTool()
: m_curTool(0) {
    // create tools
    m_riTool = new FrRoiInfoTool();
    m_penTool = new FrPenTool();
    m_rectTool = new FrRectangleTool();
    m_fsTool = new FrFreeShapeTool();
    m_sphereTool = new FrSphereTool();
    m_mmTool = new FrMaskMaskTool();
    m_deTool = new FrDilatoneErosionTool();
    m_invTool = new FrInvertTool();

    m_PointPicker = vtkPointPicker::New();
    m_PointPicker->SetTolerance(DEF_TOLERANCE);

    m_tbcTool = new FrTBCTool();
    m_ssTool = new FrSliceScrollTool();
}

FrRoiTool::~FrRoiTool(){
    // delete tools
    if(m_riTool) delete m_riTool;
    if(m_penTool) delete m_penTool;
    if(m_rectTool) delete m_rectTool;
    if(m_fsTool) delete m_fsTool;
    if(m_sphereTool) delete m_sphereTool;
    if(m_mmTool) delete m_mmTool;
    if(m_invTool) delete m_invTool;
    if(m_deTool) delete m_deTool;

    if (m_PointPicker) m_PointPicker->Delete();
    delete m_tbcTool;
	delete m_ssTool;
}

void FrRoiTool::Start(){
    // start subtool
    this->StartCurrentTool();

    // Setup controllers and start other tools
    m_tbcTool->SetController(this->GetController());
    m_ssTool->SetController(this->GetController());
    m_tbcTool->Start();
	m_ssTool->Start();

    // Enable roi tool widget
    FrMainWindow* mv = this->GetMainController()->GetMainView();
    mv->GetLayerListWidget()->GetRoiToolWidget()->setEnabled(true);

    // Update interface to ensure tool is checked
    FrManageToolCmd* cmd = FrCommandController::CreateCmd<FrManageToolCmd>();
    cmd->SetToolType(FrManageToolCmd::RoiTool);
    cmd->SetToolAction(FrManageToolCmd::UpdateAct);
    cmd->SetIsChecked(true);
    cmd->Execute();
    delete cmd;
}

void FrRoiTool::Stop(){
    // Disable roi tool widget
    FrMainWindow* mv = this->GetMainController()->GetMainView();
    mv->GetLayerListWidget()->GetRoiToolWidget()->setEnabled(false);
    
    // Stop and unregister controller
    m_curTool->Stop();
    m_curTool->SetController(0L);
    m_curTool = 0L;

    // Stop other tools and unregister controller
    m_tbcTool->Stop();
    m_ssTool->Stop();
    m_tbcTool->SetController(0);
    m_ssTool->SetController(0);

    // Update interface to ensure tool is unchecked
    FrManageToolCmd* cmd = FrCommandController::CreateCmd<FrManageToolCmd>();
    cmd->SetToolType(FrManageToolCmd::RoiTool);
    cmd->SetToolAction(FrManageToolCmd::UpdateAct);
    cmd->SetIsChecked(false);
    cmd->Execute();
    delete cmd;
}

bool FrRoiTool::OnMouseUp(FrInteractorStyle* is, FrMouseParams& params){
    bool result = false;
    if(params.Button == FrMouseParams::MidButton){
        params.Button = FrMouseParams::LeftButton;
        result = m_ssTool->OnMouseUp(is, params);
    }
    else {
        bool isInside = this->GetMappedCoords(is, params);
        m_curTool->SetImageNumber(m_ImageNumber);
        result = m_curTool->OnMouseUp(is, params);
    }

    return result;
}

bool FrRoiTool::OnMouseDown(FrInteractorStyle* is, FrMouseParams& params){
    bool result = false;
    if(params.Button == FrMouseParams::MidButton){
        params.Button = FrMouseParams::LeftButton;
        result = m_ssTool->OnMouseDown(is, params);
    }
    else {
        bool isInside = this->GetMappedCoords(is, params);
        m_curTool->SetImageNumber(m_ImageNumber);
        m_curTool->SetImageNumber(m_ImageNumber);
        result = m_curTool->OnMouseDown(is, params);
    }

    return result;
}

bool FrRoiTool::OnMouseMove(FrInteractorStyle* is, FrMouseParams& params){
    bool isInside = GetMappedCoords(is, params);
    
    m_curTool->SetImageNumber(m_ImageNumber);
    return m_curTool->OnMouseMove(is, params);
}

bool FrRoiTool::OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params){
    
    bool result = false;

    if(params.Button == FrMouseParams::MidButton){
        params.Button = FrMouseParams::LeftButton;
        result = m_ssTool->OnMouseDrag(is, params);
    }
    else {
        bool isInside = GetMappedCoords(is, params);
        m_curTool->SetImageNumber(m_ImageNumber);
        result = m_curTool->OnMouseDrag(is, params);
    }
    return result;
}

bool FrRoiTool::GetMappedCoords(FrInteractorStyle* is, FrMouseParams& params){
    // HACK: getting main controller
    FrMainController* mc = this->GetMainController();
    FrMainWindow* mv = mc->GetMainView();
    FrMainDocument* md = mc->GetMainDocument();
    FrTabSettingsDocObj* ts = md->GetCurrentTabSettings();
              
    FrLayeredImage* lim = 0;
    std::vector<FrLayerSettings*> layers;
    std::vector<vtkRenderer*> renCollection;

    enum FrTabSettingsDocObj::View view = ts->GetActiveView();
    switch(view){
        case FrTabSettingsDocObj::SliceView:
            mv->GetSliceView()->GetImage()->GetRenderers(renCollection);
            lim = mv->GetSliceView()->GetImage();
            GetLayerSettings(ts->GetSliceViewSettings(), layers);
            m_ImageNumber = DEF_IMAGE_NUMBER;
            break;
        case FrTabSettingsDocObj::MosaicView:
            mv->GetMosaicView()->GetImage()->GetRenderers(renCollection);
            lim = mv->GetMosaicView()->GetImage();
            GetLayerSettings(ts->GetMosaicViewSettings(), layers);
            m_ImageNumber = DEF_IMAGE_NUMBER;
            break;
        case FrTabSettingsDocObj::OrthoView:
            {
                FrOrthoView* ov =  mv->GetOrthoView();

                // Find Image where click's occured
                for(int i=0; i < ORTHO_IMAGE_COUNT; ++i){
                    if (ov->GetImage(i)->GetRenderer()->IsInViewport(params.X, params.Y)){
                        m_ImageNumber = i; 
                        break;
                    }
                }
                if (m_ImageNumber != -1){
                    ov->GetImage(m_ImageNumber)->GetRenderers(renCollection);
                    lim = ov->GetImage(m_ImageNumber);
                    GetLayerSettings(ts->GetOrthoViewSettings(), layers, m_ImageNumber);
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
    vtkCoordinate* coordinate = vtkCoordinate::New();
    coordinate->SetCoordinateSystemToDisplay();
    coordinate->SetValue(params.X, params.Y, 0.0);
    double* point = coordinate->GetComputedWorldValue(renderer);

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

void FrRoiTool::StartCurrentTool(){
    // stop prev tool if any
    if(m_curTool &&  m_curTool->GetController()){
        m_curTool->Stop();
        m_curTool->SetController(0);
    }
    
    FrMainWindow* mv = this->GetMainController()->GetMainView();

    // select current roi tool
    enum FrROIToolWidget::ToolType toolType = 
        mv->GetLayerListWidget()->GetRoiToolWidget()->GetCurrentToolType();

    switch(toolType){
        case FrROIToolWidget::Info:
            m_curTool = m_riTool;
            break;
        case FrROIToolWidget::Rectangle:
            m_curTool = m_rectTool;
            break;
        case FrROIToolWidget::FreeShape:
            m_curTool = m_fsTool;
            break;
        case FrROIToolWidget::Pen:
            m_curTool = m_penTool;
            break;
        case FrROIToolWidget::Sphere:
            m_curTool = m_sphereTool;
            break;
        case FrROIToolWidget::Invert:
            m_curTool = m_invTool;
            break;
        case FrROIToolWidget::DilateErode:
            m_curTool = m_deTool;
            break;
        case FrROIToolWidget::Intersect:
            m_curTool = m_mmTool;
            m_mmTool->SetMode(FrMaskMaskTool::Intersect);
            break;
        case FrROIToolWidget::Subtract:
            m_curTool = m_mmTool;
            m_mmTool->SetMode(FrMaskMaskTool::Subtract);
            break;
        case FrROIToolWidget::Union:
            m_curTool = m_mmTool;
            m_mmTool->SetMode(FrMaskMaskTool::Union);
            break;
        case FrROIToolWidget::Copy:
            m_curTool = m_mmTool;
            m_mmTool->SetMode(FrMaskMaskTool::Copy);
            break;
    }

    // start it
    m_curTool->SetController(this->GetController());
    m_curTool->Start();
}

bool FrRoiTool::OnKeyPress(FrInteractorStyle* is, FrKeyParams& params){
    return false;
}