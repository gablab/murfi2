#include "FrLoadImageCmd.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrTabSettingsDocObj.h"
#include "FrImageDocObj.h"
#include "FrBaseView.h"
#include "FrCommandController.h"
#include "FrUtils.h"
#include "FrLayerDocObj.h"

#include <algorithm>
#include "Qt/qmessagebox.h"
#include "RtDataID.h"

#define DEF_MRI_ID "mri"

FrLoadImageCmd::FrLoadImageCmd(){
    m_FilesToOpen.clear();
}

bool FrLoadImageCmd::Execute(){
    if(!this->GetMainController() ||
        this->m_FilesToOpen.empty()) return false;

	FrMainDocument* md = this->GetMainController()->GetMainDocument();
	FrMainWindow* mw = this->GetMainController()->GetMainView();
	FrViewDocObj* viewDO = md->GetCurrentViewObject();

//	// detect if any image's opened
//    std::vector<FrDocumentObj*> images;
//    md->GetObjectsByType(images, FrDocumentObj::ImageObject);
//
//    std::vector<FrDocumentObj*>::iterator it, itEnd(images.end());
//    bool hasOpenedImages = (images.begin() != itEnd);
//    bool openImage = !hasOpenedImages;
//
//    // Ask user
//    if(hasOpenedImages){
//        QMessageBox::StandardButton result = QMessageBox::information(mw,
//            QString("Front: Attention"),
//            QString("Are you sure want to replace current image with this one?"),
//            QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
//
//        if(result == QMessageBox::Yes){
//            // Delete all images and allow opening
//            // TODO: also delete rois!
//            // TODO: also delete Data from datastore!!!
//            for(it = images.begin(); it != itEnd; ++it){
//                md->Remove( (*it) );
//            }
//            openImage = true;
//        }
//    }

	// open image
    bool result = false;
    int voxInds[3];
    //if(openImage){
        // Open files
        std::sort(m_FilesToOpen.begin(), m_FilesToOpen.end());
        std::vector<QString>::iterator itr, itrEnd(m_FilesToOpen.end());

        int timePoint = 0;
	unsigned int series = getNextUniqueSeriesNum();
        for(itr = m_FilesToOpen.begin(); itr != itrEnd; ++itr){
	  RtMRIImage* img = LoadMRIImageFromFile((*itr));

	  // for now load all selected images as a single timeseries

	  if (img){

	    // create appropriate data id
	    // ohinds 2009-02-27
	    // made a unique series number so we can read more
	    // than one file
	    img->getDataID().setSeriesNum(series);
	    img->getDataID().setTimePoint(timePoint);
	    img->getDataID().setDataName((*itr).toStdString());

	    if(timePoint == 0) {
//	      FrTabSettingsDocObj* tabSets = md->GetCurrentTabSettings();
//	      FrLayerDocObj* imgLayer = new FrLayerDocObj(tabSets->GetImageLayer()->GetType(), img->getDataID(), (*itr));
//	      md->Add(imgLayer);

	      result = true;
	      voxInds[0] = viewDO->GetOrthoViewSettings()->SliceNumber[DEF_CORONAL] = img->getDim(DEF_CORONAL)/2;
	      voxInds[1] = viewDO->GetOrthoViewSettings()->SliceNumber[DEF_SAGITAL] = img->getDim(DEF_SAGITAL)/2;
	      voxInds[2] = viewDO->GetOrthoViewSettings()->SliceNumber[DEF_AXIAL] = img->getDim(DEF_AXIAL)/2;
	    }
	    timePoint++;

	    //imgLayer->CopySettings(tabSets->GetImageLayer());
	    
	    //img->setDataID(id);
	    md->AddDataToStore(img);
	  }
	  else {
	    
	    delete img;
	    QString message("Can't load image file: \n %1");
	    message.arg(*itr);
	    
	    QMessageBox::critical(mw, "Front: Load image", message);
	    result = false;
	    
	    // Do not continue loading
	    // NOTE: may be have to ask user ???
	    break;
	  }
        }
	
	if(result) {
	  FrVoxelInfoCmd* cmd = FrCommandController::CreateCmd<FrVoxelInfoCmd>();
	  cmd->SetAction(FrVoxelInfoCmd::Set);
	  cmd->SetVoxel(voxInds);
	  FrCommandController::Execute(cmd);
	}

        m_FilesToOpen.clear();
	FrBaseCmd::UpdatePipelineForID(ALL_LAYER_ID,FRP_FULL);
        //FrBaseCmd::UpdatePipelineForID(ALL_LAYER_ID, FRP_READ);
	//}
	return result;
}

RtMRIImage* FrLoadImageCmd::LoadMRIImageFromFile(QString& fileName){
    if(QFile::exists(fileName)){
        std::string stdFileName(fileName.toAscii());
        RtMRIImage* img = new RtMRIImage();

        if(img->readNifti(stdFileName)){
            // unmosaic
            if(img->seemsMosaic()){
                img->unmosaic();
            }
        }

        return img;
    }

    return 0L;
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
