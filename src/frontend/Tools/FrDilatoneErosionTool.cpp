#include "FrDilatoneErosionTool.h"
#include "FrCommandController.h"
#include "FrMainController.h"
#include "FrMainWindow.h"
#include "FrLayerListWidget.h"
#include "FrROIToolWidget.h"
#include "FrSpinToolWidget.h"

FrDilatoneErosionTool::FrDilatoneErosionTool(){
}

FrDilatoneErosionTool::~FrDilatoneErosionTool(){
}

void FrDilatoneErosionTool::Start(){

}

void FrDilatoneErosionTool::Stop(){

}

bool FrDilatoneErosionTool::OnMouseUp(FrInteractorStyle* is, FrMouseParams& params){
    // Result of execution
    bool result = false;

    FrMainController* mc = this->GetMainController();
    if(mc != 0){
        
        FrROIToolWidget* rtw = mc->GetMainView()->GetLayerListWidget()->GetRoiToolWidget();
        int kernelSize = rtw->GetDilateErodeWidget()->GetValue();

        FrMaskDilateErodeCmd* cmd = FrCommandController::CreateCmd<FrMaskDilateErodeCmd>();
        cmd->SetImageNumber(m_ImageNumber);
        cmd->SetKernelSize(kernelSize);
        if(params.Button == FrMouseParams::LeftButton){
            cmd->SetAction(FrMaskDilateErodeCmd::Dilate);
        }
        else if(params.Button == FrMouseParams::RightButton) {
            cmd->SetAction(FrMaskDilateErodeCmd::Erode);
        }
        result = cmd->Execute();
        delete cmd;
    }

    return result;
}

bool FrDilatoneErosionTool::OnMouseDown(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}

bool FrDilatoneErosionTool::OnMouseMove(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}

bool FrDilatoneErosionTool::OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}
