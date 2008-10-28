#include "FrImageDocObj.h"
#include "FrDocument.h"


// VTK includes
#include "vtkImageData.h"
#include "vtkPointData.h"
// Backend includes
#include "RtMRIImage.h"


FrImageDocObj::FrImageDocObj()
: m_Image(0){
}

FrImageDocObj::~FrImageDocObj(){
    if(m_Image) delete m_Image;
}

void FrImageDocObj::OnAdd(FrDocument* doc){
    // NOTE : Do nothing in base class
}

void FrImageDocObj::OnRemove(FrDocument* doc){
    // NOTE : Do nothing in base class 
}

FrDocumentObj::ObjType FrImageDocObj::GetType(){
    return FrDocumentObj::ImageObject;
}

unsigned int FrImageDocObj::GetMatrixSize(){
	return m_Image->getMatrixSize();
}

#include <Qt/QString.h>
#include <Qt/QFile.h>
bool FrImageDocObj::LoadFromFile(QString& fileName){
    bool result = false;
    
    if(QFile::exists(fileName)){
        std::string stdFileName = fileName.toAscii();
        RtMRIImage* img = new RtMRIImage();
        
        if(img->readNifti(stdFileName)){
            if(m_Image) delete m_Image;
            m_Image = img;
		
            SetUpdateNeeded(true);
            result = true;
        }
    }
    return result;
}

bool FrImageDocObj::LoadFromMRIImage(RtMRIImage* img){
	m_Image = img;
    SetUpdateNeeded(true);

	return true;
}