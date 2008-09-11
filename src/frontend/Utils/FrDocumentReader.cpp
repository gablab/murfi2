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
:m_Document(0){
    this->SetNumberOfInputPorts(0);    
}

FrDocumentReader::~FrDocumentReader(){
}

int FrDocumentReader::RequestInformation (vtkInformation* vtkNotUsed(request),
                                          vtkInformationVector** vtkNotUsed(inputVector),
                                          vtkInformationVector* outputVector) {    
    // NOTE: Set dummmy extent to allow process
    // go down the processing pipeline...
    int extent[6] = { 1, 1, 1, 1, 1, 1 };
    vtkInformation* outInfo = outputVector->GetInformationObject(0);
    outInfo->Set(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(), extent, 6);

    return 1;
}

vtkImageData* FrDocumentReader::AllocateOutputData(vtkDataObject* data){
    // Check for conditions
    if(m_Document == 0L){
        vtkErrorMacro(<<"AllocateOutputData(): m_Document is not valid.");
        return 0L;
    }

    // Init output image data
    vtkImageData *output = 0;
    if ( !(output = vtkImageData::SafeDownCast(data)) ){
        return 0L;
    }

    // Fill output image data
    std::vector<FrDocumentObj*> images;
    m_Document->GetObjectsByType(images, FrDocumentObj::ImageObject);
    if(images.size() > 0){
        FrImageDocObj* ido = (FrImageDocObj*)images[0];
        RtMRIImage* img = ido->GetImage();
        if(!img) return 0L;

        // setup extent & dims
        double dimW = img->getDim(0);
        double dimH = img->getDim(1);
        double dimZ = (img->getDims().size() > 2) ? img->getDim(2) : 1.0;
        output->SetDimensions(dimW, dimH, dimZ);
        output->SetWholeExtent( output->GetExtent() );
        output->SetUpdateExtent( output->GetExtent() );

        // setup spacing
        double w = img->getPixDim(0);
        double h = img->getPixDim(1);
        double z = (img->getPixDims().size() > 2) ? img->getPixDim(2) : 1.0;
        output->SetSpacing(w, h, z);
                
        // setup other params
        output->SetNumberOfScalarComponents(1);
        output->SetScalarTypeToUnsignedChar();
        output->AllocateScalars();
    }
    return output;
}

void FrDocumentReader::ExecuteData(vtkDataObject* data){
    // Get output data
    vtkImageData* output = this->AllocateOutputData(data);
    if (!output || this->UpdateExtentIsEmpty(output)) return;

    // Init output data with image data
    std::vector<FrDocumentObj*> images;
    m_Document->GetObjectsByType(images, FrDocumentObj::ImageObject);
    if(images.size() > 0){
        FrImageDocObj* ido = (FrImageDocObj*)images[0];
        RtMRIImage* img = ido->GetImage();

        // Copy image data to output
        short* dataPtr = img->getDataCopy();
        short* srcPtr = dataPtr;
        unsigned char* dstPtr = (unsigned char*)output->
            GetPointData()->GetScalars()->GetVoidPointer(0);

        int num = img->getNumPix();
        for(int i=0; i < num; i++){
            *dstPtr = unsigned char(*srcPtr);
            dstPtr++;
            srcPtr++;
        }
        delete[] dataPtr;
    }
}
