#include "FrFreeShapeTool.h"
#include "FrInteractorStyle.h"
#include "FrCommandController.h"
#include "FrUtils.h"


FrFreeShapeTool::FrFreeShapeTool(){
    SetCurrentState(FrFreeShapeTool::None); 
}

FrFreeShapeTool::~FrFreeShapeTool(){
}

void FrFreeShapeTool::Start(){

}

void FrFreeShapeTool::Stop(){

}

bool FrFreeShapeTool::OnMouseUp(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}

bool FrFreeShapeTool::OnMouseDown(FrInteractorStyle* is, FrMouseParams& params){
    if(params.Button == FrMouseParams::LeftButton || params.Button == FrMouseParams::RightButton){
        SetCurrentState(FrFreeShapeTool::Drawing);
        Pos pos;
        pos.x = params.X;
        pos.y = params.Y;

        Points.push_back(pos);

        // check if new point is close to first point, in thic case call cmd with Param:Write
        if (CheckPoint(pos)){
            SetCurrentState(FrFreeShapeTool::None); 

            FrMaskFreeShapeCmd* cmd = FrCommandController::CreateCmd<FrMaskFreeShapeCmd>();
            //cmd->SetAction(FrMaskFreeShapeCmd::Write);

            if(params.Button == FrMouseParams::LeftButton)
                cmd->SetAction(FrMaskFreeShapeCmd::Write);
            else 
                cmd->SetAction(FrMaskFreeShapeCmd::Erase); 

            Points.pop_back();
            Points.push_back(pos);

            cmd->SetPoints(Points);
            cmd->SetImageNumber(m_ImageNumber);
            cmd->Execute();
            delete cmd;

            Points.clear();
        }
    }

    return false;
}

bool FrFreeShapeTool::OnMouseMove(FrInteractorStyle* is, FrMouseParams& params){
    int x, y;

    if (GetCurrentState() == Drawing)
    {
        x = params.X;
        y = params.Y;
        // TODO: execute command with Param:Draw
        FrMaskFreeShapeCmd* cmd = FrCommandController::CreateCmd<FrMaskFreeShapeCmd>();
        cmd->SetAction(FrMaskFreeShapeCmd::Draw);
        // set point set
        Pos pos;
        pos.x = params.X;
        pos.y = params.Y;
        Points.push_back(pos);

        cmd->SetPoints(Points);
        cmd->SetImageNumber(m_ImageNumber);
        cmd->Execute();
        delete cmd;

        Points.pop_back();
    }

    return false;
}

bool FrFreeShapeTool::OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params){
    return false;
}

bool FrFreeShapeTool::CheckPoint(Pos &pos){
    bool result = false;

    double length = GetLength(Points[0].x, Points[0].y, pos.x, pos.y);
    if (length<10 && Points.size()>1){
        pos.x = Points[0].x;
        pos.y = Points[0].y;
        result = true;
    }

    return result;
}

bool FrFreeShapeTool::OnKeyPress(FrInteractorStyle* is, FrKeyParams& params){
    return false;
}


