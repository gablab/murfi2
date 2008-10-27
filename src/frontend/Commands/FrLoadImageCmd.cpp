#include "FrLoadImageCmd.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrTabSettingsDocObj.h"
#include "FrImageDocObj.h"
#include "FrBaseView.h"
#include "FrCommandController.h"

#include "Qt/QMessageBox.h"

FrLoadImageCmd::FrLoadImageCmd(){

}

bool FrLoadImageCmd::Execute(){
    FrImageDocObj* imgObj = new FrImageDocObj();
	FrMainDocument* md = this->GetMainController()->GetMainDocument();
	FrMainWindow* mw = this->GetMainController()->GetMainView();
	FrTabSettingsDocObj* ts = md->GetCurrentTabSettings();

	// check if any image opened
	std::vector<FrDocumentObj*> images;
	md->GetAllImages(images);
	int size = images.size();
	if (size > 0){
		// here goes dialog offering user to replace current image with new one (if any image is opened)
		QMessageBox msgBox;
		msgBox.setWindowTitle("Front: Attention");
		msgBox.setText("Are you sure want to replace current image with this one?");
		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
		FrTabSettingsDocObj* newts;

		switch (msgBox.exec()) {
			case QMessageBox::Yes:
				md->DeleteAll();
				// delete here all tab settings except default tab and all images
				newts = new FrTabSettingsDocObj(true);
				md->Add(newts);

				//md->Remove(images[0]);	// remove images
				// continue
				break;
			case QMessageBox::No:
				return false;	// exit
				break;
		}
	}

	// open image
	if(imgObj->LoadFromFile(m_FileName)){
		md->Add(imgObj);
		//mw->GetCurrentView()->UpdatePipeline(FRP_FULL);
		FrBaseCmd::UpdatePipelineForID(ALL_LAYERS_ID, FRP_FULL);

		FrResetImageCmd* cmd = FrCommandController::CreateCmd<FrResetImageCmd>();
		cmd->SetTargetView(FrResetImageCmd::Current);
		cmd->Execute();
		delete cmd;
	}
	else{
		// TODO: process error
	}

	return false;
}

///////////////////////////////////////////////////////////////
// Do not implement undo/redo setion for now
bool FrLoadImageCmd::CanUndo(){
    return false;
}

bool FrLoadImageCmd::Undo(){
    return false;
}

bool FrLoadImageCmd::Redo(){
    return false;
}