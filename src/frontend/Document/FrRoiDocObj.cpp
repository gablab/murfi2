#include "FrRoiDocObj.h"
#include "FrDocument.h"
#include "FrImageDocObj.h"
#include "FrCommandController.h"

// Qt stuff
#include <Qt/qstring.h>
#include <Qt/qfile.h>

// Backend includes
#include "RtMaskImage.h"
#include "RtMRIImage.h"

#define DEF_MASK_VALUE  0
#define DEF_ROI_ID      -3 // equ BAD_LAYER_ID
#define DEF_ROI_NAME    "ROI name"
#define DEF_ROI_VIS     true
#define DEF_ROI_OPAC    1.0

FrRoiDocObj::FrRoiDocObj()
:  m_MaskImage(0), m_ID(DEF_ROI_ID),
   m_Name(DEF_ROI_NAME), m_Visibility(DEF_ROI_VIS),
   m_Opacity(DEF_ROI_OPAC) {
    
}

FrRoiDocObj::~FrRoiDocObj(){
    if(m_MaskImage) delete m_MaskImage;
}

void FrRoiDocObj::OnAdd(FrDocument* doc){
    // add roi layer to all images
    FrRoiCmd* cmd1 = FrCommandController::CreateCmd<FrRoiCmd>();
    cmd1->SetAction(FrRoiCmd::Add);
    cmd1->SetRoi(this);
    
    // Need to refresh data in layer info widget
    FrRefreshLayerInfoCmd* cmd2 = 
        FrCommandController::CreateCmd<FrRefreshLayerInfoCmd>();

    FrMultiCmd* cmd = FrCommandController::CreateCmd<FrMultiCmd>();
    cmd->AddCommand(cmd1);
    cmd->AddCommand(cmd2);
    cmd->Execute();
    delete cmd;
}

void FrRoiDocObj::OnRemove(FrDocument* doc){
    // remove roi layer from all images
    FrRoiCmd* cmd1 = FrCommandController::CreateCmd<FrRoiCmd>();
    cmd1->SetAction(FrRoiCmd::Remove);
    cmd1->SetRoi(this);
    
    // Need to refresh data in layer info widget
    FrRefreshLayerInfoCmd* cmd2 = 
        FrCommandController::CreateCmd<FrRefreshLayerInfoCmd>();

    FrMultiCmd* cmd = FrCommandController::CreateCmd<FrMultiCmd>();
    cmd->AddCommand(cmd1);
    cmd->AddCommand(cmd2);
    cmd->Execute();
    delete cmd;
}

FrDocumentObj::ObjTypes FrRoiDocObj::GetType(){
    return FrDocumentObj::RoiObject;
}

void FrRoiDocObj::CreateEmptyMaskImage(FrImageDocObj* imgDO){
    
    if(m_MaskImage) delete m_MaskImage;
    
    m_MaskImage = new RtMaskImage();
    m_MaskImage->setInfo( *(imgDO->GetImage()) );
        
    // init all values to default (not a ROI)    
    // DEF_MASK_VALUE == 0
    m_MaskImage->initToZeros();
}

bool FrRoiDocObj::LoadFromFile(FrImageDocObj* imgDO, QString& fileName){
    bool result = false;
    if(QFile::exists(fileName)){
        std::string stdFileName(fileName.toAscii());
        RtMaskImage* img = new RtMaskImage();

        if(img->readNifti(stdFileName)){
            // have to unmosaic
            if(img->seemsMosaic()){
                img->unmosaic();
            }

            // check params
            bool hasSameDim = (img->getDims().size() == imgDO->GetImage()->getDims().size());
            bool hasSamePDim = (img->getPixDims().size() == imgDO->GetImage()->getPixDims().size());
            if(hasSameDim && hasSamePDim){
                for(int i=0; i < img->getDims().size(); ++i){
                    hasSameDim = (img->getDim(i) == imgDO->GetImage()->getDim(i));
                    hasSamePDim = (img->getPixDim(i) == imgDO->GetImage()->getPixDim(i));
                    if(!hasSameDim || !hasSamePDim) break;
                }

                // if OK set loaded image as current
                if(hasSameDim && hasSamePDim){
                    if(m_MaskImage) delete m_MaskImage;
                    m_MaskImage = img;
                    result = true;
                }
                else{
                    delete img;
                    result = false;
                }
            }
        }
    }
    return result;
}

void FrRoiDocObj::CreateByThresholdMaskImage(FrImageDocObj* imgDO, double threshold){

    if(m_MaskImage) delete m_MaskImage;

    m_MaskImage = new RtMaskImage(
        *(imgDO->GetImage()), threshold);
}
