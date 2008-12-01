#include "FrDocumentReader.h"
#include "FrDocument.h"
#include "FrDocumentObj.h"
#include "FrImageDocObj.h"
#include "FrRoiDocObj.h"

#include "vtkImageData.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkStreamingDemandDrivenPipeline.h"

// Backend includes
#include "RtMRIImage.h"
#include "RtMaskImage.h"
#include "RtActivation.h"

vtkStandardNewMacro(FrDocumentReader);


FrDocumentReader::FrDocumentReader()
: m_Document(0), m_Mosaic(false), 
  m_Target(FrDocumentReader::Mri),
  m_Orientation(FrDocumentReader::XY), 
  m_Slice(0), m_DataID(0) {
}

FrDocumentReader::~FrDocumentReader(){
    this->SetOutput(0);
}

void FrDocumentReader::Update(){
    if(!m_Document) return;

    vtkImageData* result = 0;
    switch(m_Target){
        case Mri:
            result = this->ReadMri();
            break;
        case Roi:
            result = this->ReadRoi();
            break;
        case Activation:
            result = this->ReadActivation();
            break;
    }

    this->SetOutput()
}

void FrDocumentReader::SetMosaic(bool value){
    if(m_Mosaic != value){
        m_Mosaic = value;
        this->SetOutput(0);
    }
}
    
void FrDocumentReader::SetDocument(FrDocument* document){
    if(m_Document != document){
        m_Document = document;
        this->SetOutput(0);
    }
}

void FrDocumentReader::SetTarget(Targets target){
    if(m_Target != target){
        m_Target = target;
        this->SetOutput(0);
    }
}

void FrDocumentReader::SetDataID(unsigned int ID){
    if(m_DataID != ID){
        m_DataID = ID;
        this->SetOutput(0);
    }
}

void FrDocumentReader::SetOrientation(Orientations value){
    if(m_Orientation != value){
        m_Orientation = value;
        this->SetOutput(0);
    }
}

void FrDocumentReader::SetSlice(int slice){
    if(m_Slice != slice){
        m_Slice = slice;
        this->SetOutput(0);
    }
}
    
void FrDocumentReader::SetOutput(vtkImageData* data){
    if(data){
        data->Register(this);
    }
    if(m_Output){
        m_Output->UnRegister(this);
    }
    m_Output = data;
}

// Helpers
vtkImageData* FrDocumentReader::ReadMri(){
    // Find appropriate image volume
    RtMRIImage* mri = 0;
    FrDocument::DocObjCollection images;
    m_Document->GetObjectsByType(images, FrDocumentObj::ImageObject);

    // NOTE Since we support the only one series 
    // Time point is a unique data ID.
    int timePoint = m_DataID;
    FrDocument::DocObjCollection::iterator it, itEnd(images.end());
    for(it = images.begin(); it != itEnd; ++it){

        FrImageDocObj* imgDO = (FrImageDocObj*)(*it);
        if(imgDO->GetTimePoint() == timePoint){
            mri = imgDO->GetImage();
            break;
        }
    }

    // 
    vtkImageData* result = 0L;
    if(mri){
    }

    return result;
} 

vtkImageData* FrDocumentReader::ReadRoi(){
    // Find appropriate image volume
    RtMaskImage* mask = 0;
    FrDocument::DocObjCollection rois;
    m_Document->GetObjectsByType(rois, FrDocumentObj::RoiObject);

    int roiID = m_DataID;
    FrDocument::DocObjCollection::iterator it, itEnd(images.end());
    for(it = images.begin(); it != itEnd; ++it){

        FrRoiDocObj* roiDO = (FrRoiDocObj*)(*it);
        if(roiDO->GetID() == timePoint){
            mri = imgDO->GetImage();
            break;
        }
    }

    // 
    vtkImageData* result = 0L;
    if(mri){
    }

    return result;
}

vtkImageData* FrDocumentReader::ReadActivation(){
    // TODO: implement
    // Not supported for a while
    return 0L;
}

void FrDocumentReader::InitMriLUT(short* data, unsigned int dataSize, 
                                  unsigned char** outLUT){
    // assume that values cannot be negative
    short min = 0;
    short max = data[0]; 
    
    // find max and min
    short* ptr = data;
    short* pEnd = data + dataSize;
    for(; ptr != pEnd; ++ptr){
        if(*ptr > max) max = *ptr;
    }

    // create lut     
    (*outLUT) = new unsigned char[VTK_SHORT_MAX];
    memset((*outLUT), 0, VTK_SHORT_MAX);

    float mult = 255.0f / float(max - min);
    for(int i=min; i < max; ++i){
        (*outLUT)[i] = unsigned char((i - min) * mult);
    }
}