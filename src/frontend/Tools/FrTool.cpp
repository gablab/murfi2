#include "FrTool.h"
#include "FrToolController.h"

FrTool::FrTool() : m_controller(0){

}

FrTool::~FrTool(){

}

FrToolController* FrTool::GetController(){
    return m_controller;
}

void FrTool::SetController(FrToolController* ctrl){
    m_controller = ctrl;
}
