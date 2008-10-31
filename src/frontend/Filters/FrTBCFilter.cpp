#include "FrTBCFilter.h"
#include "FrUtils.h"


#include "vtkObjectFactory.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"


vtkStandardNewMacro(FrTBCFilter);


FrTBCFilter::FrTBCFilter()
: m_Threshold(DEF_THRESHOLD),
  m_Brightness(DEF_BRIGHTNESS),
  m_Contrast(DEF_CONTRAST){
}

void FrTBCFilter::SetThreshold(double value){
    // need to set?    
    if(value != m_Threshold){
        // change threshold
        m_Threshold = ClampValue(value, MIN_THRESHOLD, MAX_THRESHOLD);
        this->Modified();
    }
}

void FrTBCFilter::SetBrightness(double value){
    // need to set?    
    if(value != m_Brightness){
        // change threshold
        m_Brightness = ClampValue(value, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
        this->Modified();
    }
}

void FrTBCFilter::SetContrast(double value){
    // need to set?    
    if(value != m_Contrast){
        // change threshold
        m_Contrast = ClampValue(value, MIN_CONTRAST, MAX_CONTRAST);
        this->Modified();
    }
}

void FrTBCFilter::ExecuteInformation() {
    vtkImageData *input = this->GetInput();
    vtkImageData *output = this->GetOutput();
    
    // Make sure the Input has been set.
    if ( input == NULL){
      vtkErrorMacro(<< "ExecuteInformation: Input is not set.");
      return;
    }

    // Start with some defaults.
    output->CopyTypeSpecificInformation(input);
    // Update extent
    output->SetUpdateExtent(input->GetWholeExtent());
}

// The execute method created by the subclass.
void FrTBCFilter::SimpleExecute(vtkImageData *inData, 
                                vtkImageData *outData){
    // Check for valid data type
    if (inData->GetScalarType() != VTK_UNSIGNED_CHAR){
        vtkErrorMacro(<<"FrTBCFilter: vtkImageData has to have only unsigned char data type.");
        return;
    }
            
    vtkDataArray* inArray = inData->GetPointData()->GetScalars();
    vtkDataArray* outArray = outData->GetPointData()->GetScalars();

    // If all values are 0.0 then just copy data to output
    if(m_Threshold == 0.0 && m_Brightness==0.0 && m_Contrast == 0.0){
        outArray->DeepCopy(inArray);
        return;
    }

    // Process all values
    double maxValue   = inData->GetScalarTypeMax();
    double brightness = maxValue * m_Brightness;
    double contrast   = maxValue * m_Contrast;

    // Create lookup table
    unsigned char luTable[256];
    InitLookupTable(luTable, brightness, contrast);

    // Process image data
    unsigned char value;
    unsigned char* srcPtr = (unsigned char*)inArray->GetVoidPointer(0);
    unsigned char* dstPtr = (unsigned char*)outArray->GetVoidPointer(0);

    double threshold  = maxValue * m_Threshold;
    for(int i=0; i < inArray->GetSize(); ++i){
        value = ((*srcPtr) >= threshold) ? (*srcPtr) : 0;
        (*dstPtr) = luTable[value];
        ++srcPtr; ++dstPtr;
    }
}

void FrTBCFilter::InitLookupTable(unsigned char* luTable, int brightness, int contrast){
    // Declare vars
    int iMin = MIN_BYTE_VALUE;
    int iMax = MAX_BYTE_VALUE;
    int iOffsetX = 0;

    // Init vars
    float fContrast;
    if (contrast < 0){
	    fContrast = 1.0f + ((float)contrast / 255.0f);
	    iMin = int( ((255.0f - (255.0f * fContrast)) / 2.0f) + 0.5f );
	    iMax = 255 - iMin;
    }
    else{
        float div = (256.0f - float(contrast));
        fContrast = (div != 0.0) ? 256.0f / div : 1.0;
	    iOffsetX = int(((255.0f - (255.0f * fContrast)) / 2.0f) + 0.5f);
    }
        
    int iValue;
    int iOffset = brightness + iMin + iOffsetX;
    for (int i = 0; i < 256; i++) {
	    // calculate & store value
	    iValue = int((float(i) * fContrast) + 0.5f) + iOffset;
        luTable[i] = ClampValue(iValue, iMin, iMax);
    }
}