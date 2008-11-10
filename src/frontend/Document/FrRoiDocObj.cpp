#include "FrRoiDocObj.h"
#include "FrDocument.h"

// Qt stuff
#include <Qt/qstring.h>
#include <Qt/qfile.h>

// Backend includes
#include "RtMaskImage.h"

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
    // TODO : Implement update
    // add roi layer to all images
}

void FrRoiDocObj::OnRemove(FrDocument* doc){
    // TODO : Implement update
}

FrDocumentObj::ObjType FrRoiDocObj::GetType(){
    return FrDocumentObj::RoiObject;
}

bool FrRoiDocObj::LoadFromFile(QString& fileName){
    bool result = false;
    if(QFile::exists(fileName)){
        std::string stdFileName(fileName.toAscii());
        RtMaskImage* img = new RtMaskImage();

        if(img->readNifti(stdFileName)){
            if(m_MaskImage) delete m_MaskImage;
            m_MaskImage = img;
		
            SetUpdateNeeded(true);
            result = true;
        }
    }
    return result;
}

void FrRoiDocObj::CreateMaskImage(int x, int y, int z, 
                                  int sx, int sy, int sz){
    
    if(m_MaskImage) delete m_MaskImage;
    m_MaskImage = new RtMaskImage();

    // set dimensions
    std::vector<int>& dims = m_MaskImage->getDims();
    dims.push_back(x);
    dims.push_back(y);
    dims.push_back(z);

    // set spacing
    std::vector<double>& pixDims = m_MaskImage->getPixDims();
    pixDims.push_back(sx);
    pixDims.push_back(sy);
    pixDims.push_back(sz);

    // init all values to default (not selected)
    m_MaskImage->setAll(DEF_MASK_VALUE);
}
