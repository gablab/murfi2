#include "FrDocumentReader.h"
#include "FrDocument.h"
#include "FrDocumentObj.h"
#include "FrImageDocObj.h"

#include "vtkImageData.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkStreamingDemandDrivenPipeline.h"

// Backend includes
#include "RtMRIImage.h"

FrDocumentReader* FrDocumentReader::New(){  
    vtkObject* result = vtkObjectFactory::CreateInstance("FrDocumentReader");
  
    if(result) return (FrDocumentReader*)result;
    return new FrDocumentReader();
}


FrDocumentReader::FrDocumentReader()
: m_Document(0), m_Output(0), 
  m_MosaicOn(false), m_UnMosaicOn(false){
}

FrDocumentReader::~FrDocumentReader(){
    if(m_Output) m_Output->Delete();
}

void FrDocumentReader::Update(){
    
    if(!m_Document){
        vtkErrorMacro(<<"Update(): m_Document is not set.");
        return;
    }

    // Clear output
    this->SetOutput(0);

    // Read images from document
    std::vector<FrDocumentObj*> images;
    m_Document->GetObjectsByType(images, FrDocumentObj::ImageObject);
    
    if(images.size() > 0){
        FrImageDocObj* ido = (FrImageDocObj*)images[0];
        
        // Init image
        bool deleteImage = false;
        RtMRIImage* img = ido->GetImage();
        if(m_UnMosaicOn && img->seemsMosaic()){
            img = new RtMRIImage(*img);
            img->unmosaic();
            deleteImage = true;
        }
        else if(m_MosaicOn && !img->seemsMosaic()) {
            img = new RtMRIImage(*img);
            img->mosaic();
            deleteImage = true;
        }
        
        // create output object
        vtkImageData* output = vtkImageData::New();
        
        // Init params
        double dimW = img->getDim(0);
        double dimH = img->getDim(1);
        double dimZ = img->getDim(2) < 0 ? 1.0 : img->getDim(2);
        output->SetDimensions(dimW, dimH, dimZ);

        double pxW = img->getPixDim(0);
        double pxH = img->getPixDim(1);
        double pxZ = img->getPixDim(2) < 0 ? 1.0 : img->getPixDim(2);
        output->SetSpacing(pxW, pxH, pxZ);

        output->SetNumberOfScalarComponents(1);
        output->SetScalarTypeToUnsignedChar();        
        output->AllocateScalars();
                
        // Copy image data
        short* dataPtr = img->getDataCopy();
        unsigned char* dstPtr = (unsigned char*)output->GetScalarPointer();

        short* srcPtr = dataPtr;
        for(int i=0; i < img->getNumPix(); ++i){
            *dstPtr = (unsigned char)(*srcPtr);
            dstPtr++;
            srcPtr++;
        }
        // clear all
        delete[] dataPtr;
        if(deleteImage) delete img;

        // Set output
        this->SetOutput(output);
    }    
}

void FrDocumentReader::SetDocument(FrDocument* document){
    // if document is being changed then clear output!
    m_Document = document;
    // Clear output
    SetOutput(0);
}
    
void FrDocumentReader::SetOutput(vtkImageData* output){
    // Delete prev output if any
    if(m_Output){
        m_Output->Delete();
        m_Output = 0L;
    }

    // Setup output
    if(output){
        m_Output = output;
        m_Output->Register(this);
    }
}

void FrDocumentReader::SetMosaicOn(bool isOn){
    // Change only if different value's set
    if(m_MosaicOn != isOn){
        m_MosaicOn = isOn;
        
        if(m_MosaicOn){
            m_UnMosaicOn = false;
        }
        
        // Clear output
        SetOutput(0);
    }
}

void FrDocumentReader::SetUnMosaicOn(bool isOn){
    // Change only if different value's set
    if(m_UnMosaicOn != isOn){
        m_UnMosaicOn = isOn;
        
        if(m_UnMosaicOn){
            m_MosaicOn = false;
        }
        
        // Clear output
        SetOutput(0);
    }
}