#include "FrMainController.h"
#include "FrCommandController.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrToolController.h"
#include "FrInteractorStyle.h"
#include "FrBaseView.h"
#include "FrPanTool.h"
#include "FrZoomTool.h"
#include "FrManipulationTool.h"
#include "FrImageDocObj.h"
#include "FrRoiTool.h"
#include "QVTKWidget.h"
#include "FrLayerDialog.h"
#include "FrLayerDocObj.h"
#include "FrBookmarkWidget.h"
#include "FrPointsDocObj.h"
#include "FrAppSettingsDocObj.h"
#include "FrDataStoreDialog.h"

// VTK stuff
#include "vtkRenderWindowInteractor.h"

// Qt stuff
#include "Qt/qmessagebox.h"
#include "Qt/qstring.h"

// test
#include "RtInputScannerImages.h"
#include "RtConfig.h"
#include "RtServerSocket.h"
#include "RtConductor.h"
#include "RtDataStore.h"


// Implementation of FrMainController
FrMainController::FrMainController(FrMainWindow* view, FrMainDocument* doc)
: m_MainView(view), m_MainDocument(doc), m_ToolController(0){

    m_ToolController = new FrToolController(this);    
}

FrMainController::~FrMainController(){    
    // delete tool controller
    if(m_ToolController) {
        delete m_ToolController;
    }

    // delete all other stuff
    m_MainView->hide();
    m_MainView->DisconnectActions();
    if(m_MainDocument) delete m_MainDocument;
    if(m_MainView) delete m_MainView;
        
    // Delete command controller
    FrCommandController::Delete();
}

void FrMainController::Initialize(){
    
    // Initialize FrCommandController
    FrCommandController::Instance()->SetOwner(this);

    // NOTE: Appsettings have to be created befor all
    if(m_MainDocument){

        FrAppSettingsDocObj* appSettings = new FrAppSettingsDocObj();
        m_MainDocument->Add(appSettings);
    }


    // Initialize view
    if(m_MainView){        
        m_MainView->SetMainController(this);
        m_MainView->SetMainDocument(m_MainDocument);
        m_MainView->Initialize();

        // Setup interactor style
        if(m_MainView->GetQVTKWidget()) {
            FrInteractorStyle* style = FrInteractorStyle::New();
            style->SetMainController(this);            
            //vtkInteractorStyle* style = vtkInteractorStyle::New();
            m_MainView->GetQVTKWidget()->GetInteractor()->SetInteractorStyle(style);
            style->Delete();
        }
        m_MainView->show();
    }
    
    // Initialize document
    if(m_MainDocument){

        FrSaveTabSettingsCmd* cmd = FrCommandController::CreateCmd<FrSaveTabSettingsCmd>();
        cmd->SetAction(FrSaveTabSettingsCmd::SaveNew);      
        cmd->SetIsDefault(true);
        cmd->Execute();
        delete cmd;
    }
    
    // Initialize Controller itself
    FrManageToolCmd* cmd = FrCommandController::CreateCmd<FrManageToolCmd>();
    cmd->SetToolType(FrManageToolCmd::ManipulationTool);
    cmd->SetToolAction(FrManageToolCmd::NewToolAct);
    cmd->Execute();
    delete cmd;

    //// NOTE for ROI testing 
    //FrLoadImageCmd* cmd1 = FrCommandController::CreateCmd<FrLoadImageCmd>();
    //cmd1->AddFileToOpen(QString("../example_data/img/f-00001-00000.nii"));
    ////cmd1->SetFileName("../example_data/img/MNI_T1_1mm.nii");
    //FrCreateROICmd* cmd2 = FrCommandController::CreateCmd<FrCreateROICmd>();
    //cmd2->SetCreateTest(true);
    //FrResetImageCmd* cmd3 = FrCommandController::CreateCmd<FrResetImageCmd>();
    //cmd3->SetTargetView(FrResetImageCmd::Current);
    //FrMultiCmd* multiCmd = FrCommandController::CreateCmd<FrMultiCmd>();
    //multiCmd->AddCommand(cmd1);
    //multiCmd->AddCommand(cmd2);
    //multiCmd->AddCommand(cmd3);
    //multiCmd->Execute();
    //delete multiCmd;
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

void FrMainController::LoadImageFromFile(std::vector<QString>& fileNames){
    // Load image
    FrLoadImageCmd* cmd1 = FrCommandController::CreateCmd<FrLoadImageCmd>();
    
    std::vector<QString>::iterator it, itEnd(fileNames.end());
    for(it = fileNames.begin(); it != fileNames.end(); ++it){
        QString& name = (*it);
        if(!name.isNull() && !name.isEmpty()){
            cmd1->AddFileToOpen(name);
        }
    }
    
    FrResetImageCmd* cmd2 = FrCommandController::CreateCmd<FrResetImageCmd>();
    cmd2->SetTargetView(FrResetImageCmd::Current);

    FrRefreshLayerInfoCmd* cmd3 = FrCommandController::CreateCmd<FrRefreshLayerInfoCmd>();

    FrMultiCmd* cmd = FrCommandController::CreateCmd<FrMultiCmd>();
    cmd->AddCommand(cmd1);
    cmd->AddCommand(cmd2);
    cmd->AddCommand(cmd3);
    cmd->Execute();
    delete cmd;
}

void FrMainController::OpenDataStore(){

    FrDataStoreDialog dlg(this->GetMainView(), true);
    
    // make gcc satisfied
    QString title("Data Store Viewer");
    dlg.SetCaption(title);
    dlg.Initialize(this->GetMainDocument());
    dlg.SimpleExec();
}

void FrMainController::IoTabSettings(QString& fileName, bool isInput){

    if(isInput){
        FrLoadTabsCmd* cmd1 = FrCommandController::CreateCmd<FrLoadTabsCmd>();
        cmd1->SetFileName(fileName);

        FrUpdateTabsCmd* cmd2 = FrCommandController::CreateCmd<FrUpdateTabsCmd>();
        cmd2->SetAction(FrUpdateTabsCmd::SetCurrentTab);
        cmd2->SetTabID(CURRENT_TAB_ID);

        FrRefreshLayerInfoCmd* cmd3 = FrCommandController::CreateCmd<FrRefreshLayerInfoCmd>();

        FrMultiCmd* cmd = FrCommandController::CreateCmd<FrMultiCmd>();
        cmd->AddCommand(cmd1);
        cmd->AddCommand(cmd2);
        cmd->AddCommand(cmd3);
        cmd->Execute();
        delete cmd;
    }
    else{
        // first save current view to tab
        FrSaveTabSettingsCmd* cmd1 = FrCommandController::CreateCmd<FrSaveTabSettingsCmd>();
        cmd1->SetAction(FrSaveTabSettingsCmd::SaveCurrent);

        FrSaveTabsCmd* cmd2 = FrCommandController::CreateCmd<FrSaveTabsCmd>();
        cmd2->SetFileName(fileName);

        FrMultiCmd* cmd = FrCommandController::CreateCmd<FrMultiCmd>();
        cmd->AddCommand(cmd1);
        cmd->AddCommand(cmd2);
        cmd->Execute();
        delete cmd;
    }
}

void FrMainController::Notify(int notifyCode){
	// Do nothing here
}

void FrMainController::SaveCurrentViewToTab(){
    FrSaveTabSettingsCmd* cmd = FrCommandController::CreateCmd<FrSaveTabSettingsCmd>();
    cmd->SetAction(FrSaveTabSettingsCmd::SaveNew);
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

    FrChangeViewCmd* cmd1 = FrCommandController::CreateCmd<FrChangeViewCmd>();
    cmd1->SetTargetView(targetView);

    FrRefreshLayerInfoCmd* cmd2 = FrCommandController::CreateCmd<FrRefreshLayerInfoCmd>();

    FrMultiCmd* cmd = FrCommandController::CreateCmd<FrMultiCmd>();
    cmd->AddCommand(cmd1);
    cmd->AddCommand(cmd2);
    cmd->Execute();
    delete cmd;
}

void FrMainController::SelectLayer(int id){
    FrLayerCmd* cmd = FrCommandController::CreateCmd<FrLayerCmd>();
    cmd->SetAction(FrLayerCmd::UpdateSelectedID);
    cmd->SetID(id);
    cmd->Execute();
    delete cmd;
}

void FrMainController::AddLayer(){
    // add layer with dialog
    FrUserActionCmd* cmd = FrCommandController::CreateCmd<FrUserActionCmd>();
    cmd->SetAction(FrUserActionCmd::Add);
    cmd->Execute();
    delete cmd;
}

void FrMainController::DeleteLayer(){
    // Deletes currently selected layer
    FrUserActionCmd* cmd = FrCommandController::CreateCmd<FrUserActionCmd>();
    cmd->SetAction(FrUserActionCmd::Delete);
    cmd->Execute();
    delete cmd;            
}

void FrMainController::ChangeLayer(int action){
    FrMultiCmd* cmd = FrCommandController::CreateCmd<FrMultiCmd>();

    // Changing using old style dialog
    if(action == 0){
        FrLayerCmd* cmd1 = FrCommandController::CreateCmd<FrLayerCmd>();
        cmd1->SetAction(FrLayerCmd::ChangeOld);
        // If not set ID is the same as current ID
        //cmd1->SetID(CUR_LAYER_ID);
        cmd->AddCommand(cmd1);

        FrRefreshLayerInfoCmd* cmd2 = 
            FrCommandController::CreateCmd<FrRefreshLayerInfoCmd>();
        cmd->AddCommand(cmd2);
    }
    // Changing Name, visibility, opacity
    else if(action == 1){
        FrLayerCmd* cmd1 = FrCommandController::CreateCmd<FrLayerCmd>();
        cmd1->SetAction(FrLayerCmd::ChangeParams);
        // If not set ID is the same as current ID
        //cmd1->SetID(CUR_LAYER_ID);
        cmd->AddCommand(cmd1);
    }
    else if (action == 2){
        FrLayerCmd* cmd1 = FrCommandController::CreateCmd<FrLayerCmd>();
        cmd1->SetAction(FrLayerCmd::ChangeColormap);
        // If not set ID is the same as current ID
        //cmd1->SetID(CUR_LAYER_ID);
        cmd->AddCommand(cmd1);
    }

    cmd->Execute();
    delete cmd;
}

void FrMainController::ChangeImageSettings(){
    FrUserActionCmd* cmd = FrCommandController::CreateCmd<FrUserActionCmd>();
    cmd->SetAction(FrUserActionCmd::ChangeSettings);
    cmd->Execute();
    delete cmd;
}

void FrMainController::ChangeBookmark(int id){
    // Create complex command and execute it
    FrMultiCmd* cmd = FrCommandController::CreateCmd<FrMultiCmd>();

    // save current tab before switching (only if we have more than 1 bookmarks
    // HACK: check if we have more than 1 tabs
    if (this->GetMainView()->GetBookmarkWidget()->GetBookmarkCount() > 1){
        FrSaveTabSettingsCmd* cmd1 = FrCommandController::CreateCmd<FrSaveTabSettingsCmd>();
        cmd1->SetAction(FrSaveTabSettingsCmd::SaveCurrent);
        cmd->AddCommand(cmd1);
    }

    FrUpdateTabsCmd* cmd2 = FrCommandController::CreateCmd<FrUpdateTabsCmd>();
    cmd2->SetAction(FrUpdateTabsCmd::SetCurrentTab);
    cmd2->SetTabSettingsDocObj(0L);
    cmd2->SetTabID( id );

    FrChangeViewCmd* cmd3 = FrCommandController::CreateCmd<FrChangeViewCmd>();
    cmd3->SetTargetView(FrChangeViewCmd::Synchronize);

    FrRefreshLayerInfoCmd* cmd4 = FrCommandController::CreateCmd<FrRefreshLayerInfoCmd>();

//    FrMultiCmd* cmd = FrCommandController::CreateCmd<FrMultiCmd>();
//    cmd->AddCommand(cmd1);
    cmd->AddCommand(cmd2);
    cmd->AddCommand(cmd3);
    cmd->AddCommand(cmd4);
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
    case 3: cmd->SetToolType(FrManageToolCmd::VoxelSelectionTool);
        break;
    default:
        // do nothing
        break;
    }

    cmd->Execute();
    delete cmd;
}

void FrMainController::ResetImage(){

    FrResetImageCmd* cmd = FrCommandController::CreateCmd<FrResetImageCmd>();
    cmd->SetTargetView(FrResetImageCmd::Current);
    cmd->Execute();
    delete cmd;
}

void FrMainController::CreatNewROI(){

    FrCreateROICmd* cmd1 = FrCommandController::CreateCmd<FrCreateROICmd>();
    
    FrRefreshLayerInfoCmd* cmd2 = FrCommandController::CreateCmd<FrRefreshLayerInfoCmd>();

    FrMultiCmd* cmd = FrCommandController::CreateCmd<FrMultiCmd>();
    cmd->AddCommand(cmd1);
    cmd->AddCommand(cmd2);
    cmd->Execute();
    delete cmd;
}

void FrMainController::UpdateRoiTool(){  

    FrRoiTool* tool = dynamic_cast<FrRoiTool*>(m_ToolController->GetCurrentTool());
    if(tool != 0L){
        tool->StartCurrentTool();
    }
}

void FrMainController::SetLiveMode(bool newLiveMode){
    // NOTE: can do it here no need to delegate
    m_MainDocument->GetAppSettings()->SetLiveMode(newLiveMode);
}

void FrMainController::SetCurrentTimePoint(int newTimePoint){
    FrTimePointCmd* cmd = FrCommandController::CreateCmd<FrTimePointCmd>();
    cmd->SetAction(FrTimePointCmd::SetUserDefined);
    cmd->SetTimePoint(newTimePoint);
    cmd->SetCheckLifeMode(false);
    cmd->Execute();
    delete cmd;
}

void FrMainController::SetPreviousTimePoint(){
    FrTimePointCmd* cmd = FrCommandController::CreateCmd<FrTimePointCmd>();
    cmd->SetAction(FrTimePointCmd::SetPrevious);
    cmd->Execute();
    delete cmd;
}

void FrMainController::SetNextTimePoint(){
    FrTimePointCmd* cmd = FrCommandController::CreateCmd<FrTimePointCmd>();
    cmd->SetAction(FrTimePointCmd::SetNext);
    cmd->Execute();
    delete cmd;
}

void FrMainController::Test(){    

    FrTimePointCmd* cmd = FrCommandController::CreateCmd<FrTimePointCmd>();
    cmd->SetAction(FrTimePointCmd::SetPrevious);
    cmd->Execute();
    delete cmd;

//    // run test input server
//	RtInputScannerImages* input =  new RtInputScannerImages();
////	RtInputScannerImages input;
//	
////	RtConfig config;
//	char *path[3];
//	path[0] = "test";
//	path[1] = "-f";
//	path[2] = "sensorymotor_singleimage.xml";
////	bool result = config.parseArgs(3, path);
//	
//	//input->activate();
//	//input->init(3, path);
////	result = input.open(config);
////	input.svc();
////	RtMRIImage img;
////	if (result){
////		input.saveImage(img);
////		int g = 55;
////	}
////	else
////		return;
//    
//    RtDataStore* ds = new RtDataStore();
//
//    RtConductor con(3, path);
//    con.init();
//    //con.addInput(input);
//    //con.addOutput(ds);
//    con.run();
//    
//    //input.close();
//
//    //FrImageDocObj* imgObj = new FrImageDocObj();
//
//        //if(imgObj->LoadFromMRIImage(&img)){// we should process all received images in future
// //       m_MainDocument->Add(imgObj);
// //       m_MainView->GetCurrentView()->UpdatePipeline(FRP_FULL);
// //       
// //       FrResetImageCmd* cmd = FrCommandController::CreateCmd<FrResetImageCmd>();
// //       cmd->SetTargetView(FrResetImageCmd::Current);
// //       cmd->Execute();
// //       delete cmd;
// //   }
// //   else{
// //       // TODO: process error
// //   }
}
