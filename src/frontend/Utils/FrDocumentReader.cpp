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

#define DEF_READ_TARGET FrDocumentReader::Image

vtkStandardNewMacro(FrDocumentReader);


FrDocumentReader::FrDocumentReader()
: m_Document(0), m_Target(DEF_READ_TARGET),
  m_MosaicOn(false), m_UnMosaicOn(true){
}

FrDocumentReader::~FrDocumentReader(){
    this->ClearOutputs();
}

void FrDocumentReader::Update(){
    
    if(!m_Document){
        vtkErrorMacro(<<"Update(): m_Document is not set.");
        return;
    }
    this->ClearOutputs();

    std::vector<FrDocumentObj*> images;
    if(m_Target == FrDocumentReader::Image){
        // get image data from docment
        m_Document->GetObjectsByType(images, FrDocumentObj::ImageObject);

        if(images.size() > 0){
            FrImageDocObj* imgDO = (FrImageDocObj*)images[0];
            vtkImageData* img = this->ReadImage(imgDO);

            if(img == 0L) return;
            this->AddOutput(img);
            // Do not forget to delete to prevent mem leakage
            img->Delete();
        }
    }
    else if(m_Target == FrDocumentReader::ROI){
        // get roi data
        m_Document->GetObjectsByType(images, FrDocumentObj::RoiObject);
        std::vector<FrDocumentObj*>::iterator it, itEnd(images.end());
        for(it = images.begin(); it != itEnd; ++it){
            FrRoiDocObj* roiDO = (FrRoiDocObj*)(*it);
            vtkImageData* roi = this->ReadROI(roiDO);
            this->AddOutput(roi);
            // Do not forget to delete to prevent mem leakage
            roi->Delete();
        }
    }
    else {
        vtkErrorMacro(<<"FrDocumentReader: Not supported read target...");
    }
}

vtkImageData* FrDocumentReader::ReadImage(FrImageDocObj* imgDO){
    // First mosaic / unmosaic image
    bool deleteImage = false;
    RtMRIImage* img = imgDO->GetImage();
    if(m_UnMosaicOn && img->seemsMosaic()){
        img = new RtMRIImage(*img);
        if(!img->unmosaic()) {
            vtkErrorMacro(<<"Can't unmosaic image.");
            delete img;
            return 0L;
        }
        deleteImage = true;
    }
    else if(m_MosaicOn && !img->seemsMosaic()) {
        img = new RtMRIImage(*img);
        if(!img->mosaic()){
            vtkErrorMacro(<<"Can't mosaic image.");
            delete img;
            return 0L;
        }
        deleteImage = true;
    }
            
    // create output data object
    vtkImageData* output = vtkImageData::New();
    
    // Init params
    double dimW = double(img->getDim(0));
    double dimH = double(img->getDim(1));
    double dimZ = (img->getDim(2) < 1) ? 1.0 : img->getDim(2);
    output->SetDimensions(dimW, dimH, dimZ);

    double pxW = img->getPixDim(0);
    double pxH = img->getPixDim(1);
    double pxZ = (img->getPixDim(2) < 0.0) ? 1.0 : img->getPixDim(2);
    output->SetSpacing(pxW, pxH, pxZ);

    output->SetNumberOfScalarComponents(1);
    output->SetScalarTypeToUnsignedChar();        
    output->AllocateScalars();
            
    // Copy image data with mapping into 0..255 range
    // need src and dst pointers
    short* dataPtr = img->getDataCopy();
    unsigned int dataSize = img->getNumPix();
    unsigned char* dstPtr = (unsigned char*)output->GetPointData()->
        GetScalars()->GetVoidPointer(0);

    // Need lookup table
    unsigned char* lut = 0;
    this->InitLookupTable(dataPtr, dataSize, &lut);

    // Do copy
    short* srcPtr = dataPtr;
    for(int i=0; i < dataSize; ++i){
        *dstPtr = lut[*srcPtr];
        dstPtr++;
        srcPtr++;
    }

    // clear all
    delete[] lut;
    delete[] dataPtr;
    if(deleteImage) delete img;

    return output;
}

vtkImageData* FrDocumentReader::ReadROI(FrRoiDocObj* roiDO){
    // First mosaic / unmosaic image
    bool deleteImage = false;
    RtMaskImage* img = roiDO->GetMaskImage();
    if(m_UnMosaicOn && img->seemsMosaic()){
        img = new RtMaskImage(*img);
        if(!img->unmosaic()) {
            vtkErrorMacro(<<"Can't unmosaic image.");
            delete img;
            return 0L;
        }
        deleteImage = true;
    }
    else if(m_MosaicOn && !img->seemsMosaic()) {
        img = new RtMaskImage(*img);
        if(!img->mosaic()){
            vtkErrorMacro(<<"Can't mosaic image.");
            delete img;
            return 0L;
        }
        deleteImage = true;
    }
            
    // create output data object
    vtkImageData* output = vtkImageData::New();
    
    // Init params
    double dimW = double(img->getDim(0));
    double dimH = double(img->getDim(1));
    double dimZ = (img->getDim(2) < 1) ? 1.0 : img->getDim(2);
    output->SetDimensions(dimW, dimH, dimZ);

    double pxW = img->getPixDim(0);
    double pxH = img->getPixDim(1);
    double pxZ = (img->getPixDim(2) < 0.0) ? 1.0 : img->getPixDim(2);
    output->SetSpacing(pxW, pxH, pxZ);

    output->SetNumberOfScalarComponents(1);
    output->SetScalarTypeToUnsignedChar();        
    output->AllocateScalars();
            
    // Copy image data with mapping into 0..255 range
    // need src and dst pointers
    short* dataPtr = img->getDataCopy();
    unsigned int dataSize = img->getNumPix();
    unsigned char* dstPtr = (unsigned char*)output->GetPointData()->
        GetScalars()->GetVoidPointer(0);
    
    // Do copy
    short* srcPtr = dataPtr;
    for(int i=0; i < dataSize; ++i){
        *dstPtr = ((*srcPtr) > 0) ? VTK_UNSIGNED_CHAR_MAX : VTK_UNSIGNED_CHAR_MIN;
        dstPtr++;  srcPtr++;
    }

    // clear all
    delete[] dataPtr;
    if(deleteImage) delete img;

    // Set output
    return output;
}

void FrDocumentReader::InitLookupTable(short* data, unsigned int dataSize,
                                       unsigned char** outLUT){    
    // assume that values cannot be negative
    short max = data[0]; 
    short min = data[0];

    // find max and min
    short* ptr = data;
    short* pEnd = data + dataSize;
    while(ptr != pEnd){
        if(*ptr > max) max = *ptr;
        else if(*ptr < min) min = *ptr;
        ++ptr;
    }

    // create lut     
    (*outLUT) = new (unsigned char[VTK_SHORT_MAX]);

    float mult = 255.0f / float(max - min);
    for(int i=0; i < VTK_SHORT_MAX; ++i){
        (*outLUT)[i] = (min <= i && i <= max) ?  
            ((unsigned char)((i - min) * mult)) : 0;
    }
}


void FrDocumentReader::SetDocument(FrDocument* document){
    // if document is being changed then clear output!
    m_Document = document;
    this->ClearOutputs();
}

void FrDocumentReader::SetTarget(Target tgt){
    m_Target = tgt;
    this->ClearOutputs();
}

void FrDocumentReader::SetMosaicOn(){
    // Change only if different value's set
    if(!m_MosaicOn){
        m_MosaicOn = true;
        m_UnMosaicOn = false;

        // Clear output
        this->ClearOutputs();
    }
}

void FrDocumentReader::SetUnMosaicOn(){
    // Change only if different value's set
    if(!m_UnMosaicOn){
        m_UnMosaicOn = true;
        m_MosaicOn = false;

        // Clear output
        this->ClearOutputs();
    }
}


void FrDocumentReader::AddOutput(vtkImageData* data){
    // Setup output
    if(data != 0L){
        data->Register(this);
    }
    m_Outputs.push_back(data);
}

void FrDocumentReader::ClearOutputs(){
    OutputCollection::iterator it, itEnd(m_Outputs.end());
    for(it = m_Outputs.begin(); it != itEnd; ++it){
        if((*it) != 0){
            (*it)->UnRegister(this);
        }
    }
    m_Outputs.clear();
}

int FrDocumentReader::GetOutputCount(){
    return int(m_Outputs.size());
}

vtkImageData* FrDocumentReader::GetOutput(){
    return this->GetOutput(0);
}

vtkImageData* FrDocumentReader::GetOutput(int port){
    vtkImageData* result = 0L;
    if(0 <= port && port < m_Outputs.size()){
        result = m_Outputs[port];
    }
    return result;
}