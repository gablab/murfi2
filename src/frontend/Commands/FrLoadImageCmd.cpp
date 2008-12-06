#include "FrLoadImageCmd.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrTabSettingsDocObj.h"
#include "FrImageDocObj.h"
#include "FrBaseView.h"
#include "FrCommandController.h"
#include "FrUtils.h"

#include "Qt/qmessagebox.h"

#include "RtDataId.h"

#define DEF_MRI_ID "mri"

FrLoadImageCmd::FrLoadImageCmd(){

}

bool FrLoadImageCmd::Execute(){    
    if(!this->GetMainController()) return false;

	FrMainDocument* md = this->GetMainController()->GetMainDocument();
	FrMainWindow* mw = this->GetMainController()->GetMainView();
	
	// detect if any image's opened
    std::vector<FrDocumentObj*> images;
    md->GetAllImages(images);

    std::vector<FrDocumentObj*>::iterator it, itEnd(images.end());
    bool hasOpenedImages = (images.begin() != itEnd);
    bool openImage = !hasOpenedImages;

    // Ask user 
    if(hasOpenedImages){
        QMessageBox::StandardButton result = QMessageBox::information(mw,
            QString("Front: Attention"), 
            QString("Are you sure want to replace current image with this one?"),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

        if(result == QMessageBox::Yes){
            // Delete all images and allow opening
            for(it = images.begin(); it != itEnd; ++it){
                md->Remove( (*it) );
            }
            openImage = true;
        }
    }
    
	// open image
    bool result = true;
    if(openImage){
        RtMRIImage* img = LoadMRIImageFromFile(m_FileName);
    
        if (img){
            //// create appropriate data id
            //RtDataID id;
            //id.setModuleID(DEF_MRI_ID);
            //
            //img->setDataID(id);
            md->AddDataToStore(img);
            FrBaseCmd::UpdatePipelineForID(ALL_LAYER_ID, FRP_FULL);        // ?
        }
        else{
            delete img;
            QMessageBox::critical(mw, "Front: Load image", "Can't load image...");
            result = false;
        }
    }
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
