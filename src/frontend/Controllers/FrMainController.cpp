#include "FrMainController.h"
#include "FrCommandController.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrToolController.h"
#include "FrInteractorStyle.h"
#include "FrBaseView.h"
#include "FrPanTool.h"
#include "FrZoomTool.h"
#include "FrCompositeTool.h"
#include "FrImageDocObj.h"
#include "QVTKWidget.h"

// VTK stuff
#include "vtkRenderWindowInteractor.h"

// Qt stuff
#include "Qt/QMessageBox.h"
#include "Qt/QString.h"


// Implementation of FrMainController
FrMainController::FrMainController(FrMainWindow* view, FrMainDocument* doc)
    : m_MainView(view), m_MainDocument(doc), m_ToolController(0){

    m_ToolController = new FrToolController(this);
}

FrMainController::~FrMainController(){    
    if(m_ToolController) {
        delete m_ToolController;
    }
    
    if(m_MainDocument) delete m_MainDocument;
    if(m_MainView) delete m_MainView;
}

void FrMainController::Initialize(){
    
    // Initialize FrCommandController
    FrCommandController::Instance()->SetOwner(this);

    // Initialize view
    if(m_MainView){        
        m_MainView->SetMainController(this);
        m_MainView->SetMainDocument(m_MainDocument);
        m_MainView->Initialize();

        // Setup interactor style
        if(m_MainView->GetQVTKWidget()) {
            FrInteractorStyle* style = new FrInteractorStyle(this);
            m_MainView->GetQVTKWidget()->GetInteractor()->SetInteractorStyle(style);
        }
        m_MainView->show();
    }
    
    // Initialize document
    if(m_MainDocument){
        FrSaveTabSettingsCmd* cmd = FrCommandController::CreateCmd<FrSaveTabSettingsCmd>();
        cmd->SetIsDefault(true);
        cmd->Execute();
        delete cmd;
    }

    // Initialize Controller itself
    FrManageToolCmd* cmd = FrCommandController::CreateCmd<FrManageToolCmd>();
    cmd->SetToolType(FrManageToolCmd::ManipulationTool);
    cmd->SetToolAction(FrManageToolCmd::NewToolAct);
    cmd->Execute();
}

bool FrMainController::HasActiveTool(){
    bool result = false;
    if(m_ToolController){
        result = (m_ToolController->GetCurrentTool() != 0);
    }
    return result;
}

FrTool* FrMainController::GetCurrentTool(){
    FrTool* result = 0;
    if(m_ToolController){
        result = m_ToolController->GetCurrentTool();
    }
    return result;
}

void FrMainController::LoadImage(QString& fileName){
        
    FrImageDocObj* imgObj = new FrImageDocObj();

    if(imgObj->LoadFromFile(fileName)){
        m_MainDocument->Add(imgObj);
    }
    else{
        // TODO: process error
    }    
    m_MainView->GetCurrentView()->UpdatePipeline(FRP_FULL);
}

void FrMainController::Notify(int notifyCode){
	// Do nothing here
}

void FrMainController::SaveCurrentViewToTab(){

    FrSaveTabSettingsCmd* cmd = FrCommandController::CreateCmd<FrSaveTabSettingsCmd>();
    cmd->Execute();
    delete cmd;
}

void FrMainController::ChangeView(int view){
    FrChangeViewCmd::View targetView = FrChangeViewCmd::Unknown;
    switch(view){
    case 0: targetView = FrChangeViewCmd::Slice;
        break;
    case 1: targetView = FrChangeViewCmd::Mosaic;
        break;
    case 2: targetView = FrChangeViewCmd::Ortho;
        break;
    }

    FrChangeViewCmd* cmd = FrCommandController::CreateCmd<FrChangeViewCmd>();
    cmd->SetTargetView(targetView);
    cmd->Execute();
    delete cmd;    
}

void FrMainController::ChangeBookmark(int id){
    // Create complex command and execute it
    FrUpdateTabsCmd* cmd1 = FrCommandController::CreateCmd<FrUpdateTabsCmd>();
    cmd1->SetAction(FrUpdateTabsCmd::SetCurrentTab);
    cmd1->SetTabSettingsDocObj(0L);
    cmd1->SetTabID( id );

    FrChangeViewCmd* cmd2 = FrCommandController::CreateCmd<FrChangeViewCmd>();
    cmd2->SetTargetView(FrChangeViewCmd::Synchronize);
        
    FrMultiCmd* cmd = FrCommandController::CreateCmd<FrMultiCmd>();
    cmd->AddCommand(cmd1);
    cmd->AddCommand(cmd2);
    cmd->Execute();

    delete cmd;
}

void FrMainController::DeleteBookmark(int id){
    FrDeleteTabSettingsCmd* cmd = FrCommandController::CreateCmd<FrDeleteTabSettingsCmd>();
    cmd->SetTabID(id);
    if(!cmd->Execute()){
        QMessageBox::critical(this->GetMainView(),
                              "Error Closing Tab",
                              "Can't close this tab...");
    }
    delete cmd;
}

void FrMainController::SetCurrentTool(int tool){
    FrManageToolCmd* cmd = FrCommandController::CreateCmd<FrManageToolCmd>();
    cmd->SetToolAction(FrManageToolCmd::NewToolAct);

    switch(tool)
    {
    case 0: cmd->SetToolType(FrManageToolCmd::ManipulationTool);
        break;
    case 1: cmd->SetToolType(FrManageToolCmd::VoxelTool);
        break;
    case 2: cmd->SetToolType(FrManageToolCmd::RoiTool);
        break;
    default:
        // do nothing
        break;
    }

    cmd->Execute();
    delete cmd;
}