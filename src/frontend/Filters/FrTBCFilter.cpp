#include "FrTBCFilter.h"
#include "FrUtils.h"

#include "vtkObjectFactory.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"


FrTBCFilter* FrTBCFilter::New(){  
  vtkObject* result = vtkObjectFactory::CreateInstance("FrTBCFilter");
  
  if(result){
      return (FrTBCFilter*)result;
  }

  return new FrTBCFilter();
}

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

// The execute method created by the subclass.
void FrTBCFilter::SimpleExecute(vtkImageData *inData, 
                                vtkImageData *outData){
    // Check for valid data type
    if (inData->GetScalarType() != VTK_UNSIGNED_CHAR){
        vtkErrorMacro(<<"FrTBCFilter: vtkImageData has to have only unsigned char data type.");
        return;
    }

    // Init some data for algorithm
    int dims[3]; 
    inData->GetDimensions(dims);
    int numVoxs = dims[0] * dims[1] * dims[2];
    int numComps = inData->GetNumberOfScalarComponents();
    
    vtkDataArray* inArray = inData->GetPointData()->GetScalars();
    vtkDataArray* outArray = outData->GetPointData()->GetScalars();

    double maxValue   = inData->GetScalarTypeMax();
    double brightness = maxValue * m_Brightness;
    double contrast   = maxValue * m_Contrast;

    // Create lookup table
    unsigned char luTable[256];
    InitLookupTable(luTable, brightness, contrast);

    // Process image data
    double threshold  = maxValue * m_Threshold;
    for (int component = 0; component < numComps; ++component){
        for (int voxel = 0; voxel < numVoxs; ++voxel){
            // Check threshold 
            double out = 0.0;
            double value = inArray->GetComponent(voxel, component);
            out = (value >= threshold) ? value : 0.0;

            // set brightness/contrast value
            out = luTable[int(out)];
            outArray->SetComponent(voxel, component, out);
        }
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
	    fContrast = 256.0f / (256.0f - float(contrast));
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