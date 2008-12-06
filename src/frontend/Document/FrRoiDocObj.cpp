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
:  m_MaskImage(0) {
    // Assign ID
    m_ID = (unsigned int)((void*)this);
}

FrRoiDocObj::FrRoiDocObj(RtMaskImage* roi)
: m_MaskImage(roi){
    // Assign ID
    m_ID = (unsigned int)((void*)this);
}

FrRoiDocObj::~FrRoiDocObj(){
    if(m_MaskImage) delete m_MaskImage;
}

void FrRoiDocObj::OnAdd(FrDocument* doc){
    // Add ROI layer doc object
}

void FrRoiDocObj::OnRemove(FrDocument* doc){
    // remove ROI layer doc object
}

FrDocumentObj::ObjTypes FrRoiDocObj::GetType(){
    return FrDocumentObj::RoiObject;
}

void FrRoiDocObj::CreateEmptyMaskImage(FrImageDocObj* imgDO){    
    if(m_MaskImage) delete m_MaskImage;
    
    m_MaskImage = new RtMaskImage();
    m_MaskImage->setInfo( *(imgDO->GetTimePointData(0)) );
        
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
            bool hasSameDim = (img->getDims().size() == imgDO->GetTimePointData(0)->getDims().size());
            bool hasSamePDim = (img->getPixDims().size() == imgDO->GetTimePointData(0)->getPixDims().size());
            if(hasSameDim && hasSamePDim){
                for(int i=0; i < img->getDims().size(); ++i){
                    hasSameDim = (img->getDim(i) == imgDO->GetTimePointData(0)->getDim(i));
                    hasSamePDim = (img->getPixDim(i) == imgDO->GetTimePointData(0)->getPixDim(i));
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
        *(imgDO->GetTimePointData(0)), threshold);
}
