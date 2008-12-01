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

#define BAD_SERIES_NUM 0xffffffff

FrImageDocObj::FrImageDocObj(){
    m_Images.clear();
    m_SeriesNumber = BAD_SERIES_NUM:
}

FrImageDocObj::FrImageDocObj(RtMRIImage* img){    
    m_Images.push_back(img);
    m_SeriesNumber = img->getDataID().getSeriesNum();
}

FrImageDocObj::~FrImageDocObj(){
    this->ClearAll();
}

void FrImageDocObj::OnAdd(FrDocument* doc){
    // NOTE : Add here Image layer
}

void FrImageDocObj::OnRemove(FrDocument* doc){
    // NOTE : Remove here image layer
}

FrDocumentObj::ObjTypes FrImageDocObj::GetType(){
    return FrDocumentObj::ImageObject;
}

RtMRIImage* FrImageDocObj::GetTimePointData(unsigned int timePoint){
    RtMRIImage* result = 0;

    ImageCollection::iterator it, itEnd(m_Images.end());
    for(it = m_Images.begin(); it != itEnd; ++it){
        RtMRIImage* img = (RtMRIImage*)(*it);
        if(img->getDataID().getTimePoint() == timePoint){
            result = img;
            break;
        }
    }

    return result;
}

bool FrImageDocObj::AddTimePointData(RtMRIImage* mriImage){
    // Some checks
    if(!mriImage) return false;
    if(m_SeriesNumber == BAD_SERIES_NUM){
        m_SeriesNumber = mriImage->getDataID().getSeriesNum();
    }

    // Add only with the same series number
    if(m_SeriesNumber != mriImage->getDataID().getSeriesNum()){
        return false;
    }

    // Check if we have this timepoint
    bool hasTP = false;
    unsigned int timePoint = mriImage->getDataID().getTimePoint();
    ImageCollection::iterator it, itEnd(m_Images.end());
    for(it = m_Images.begin(); (it != itEnd) && !hasTP; ++it){
        RtMRIImage* img = (RtMRIImage*)(*it);
        hasTP = (img->getDataID().getTimePoint() == timePoint);
    }

    if(hasTP) return false;
    m_Images.push_back(mriImage);

    return false;
}

void FrImageDocObj::ClearAll(){
    ImageCollection::iterator it, itEnd(m_Images.end());
    for(it = m_Images.begin(); it != itEnd; ++it){
        delete (*it);
    }
    m_Images.clear();
    m_SeriesNumber = BAD_SERIES_NUM;
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