#include "FrImageDocObj.h"
#include "FrDocument.h"


// VTK includes
#include "vtkImageData.h"
#include "vtkPointData.h"

// Backend includes
#include "RtMRIImage.h"
#include "RtDataID.h"

// Qt stuff
#include <Qt/qstring.h>
#include <Qt/qfile.h>


FrImageDocObj::FrImageDocObj()
: m_Image(0){
}

FrImageDocObj::FrImageDocObj(RtMRIImage* img)
: m_Image(img){
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

FrDocumentObj::ObjTypes FrImageDocObj::GetType(){
    return FrDocumentObj::ImageObject;
}

unsigned int FrImageDocObj::GetMatrixSize(){
	return m_Image->getMatrixSize();
}

int FrImageDocObj::GetSeries(){
    return m_Image->getDataID().getSeriesNum();
}

int FrImageDocObj::GetTimePoint(){
    return m_Image->getDataID().getTimePoint();
}

bool FrImageDocObj::LoadFromFile(QString& fileName){
    bool result = false;

    if(QFile::exists(fileName)){
        std::string stdFileName(fileName.toAscii());
        RtMRIImage* img = new RtMRIImage();

        if(img->readNifti(stdFileName)){
            // unmosaic
            if(img->seemsMosaic()){
                img->unmosaic();
            }

            if(m_Image) delete m_Image;
            m_Image = img;
            result = true;
        }
    }
    return result;
}

bool FrImageDocObj::LoadFromMRIImage(RtMRIImage* img){
    // unmosaic!
    if(img->seemsMosaic()){
        img->unmosaic();
    }
	m_Image = img;

	return true;
}