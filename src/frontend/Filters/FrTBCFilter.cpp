#include "FrTBCFilter.h"

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
: m_threshold(DEF_THRESHOLD),
  m_brightness(DEF_BRIGHTNESS),
  m_contrast(DEF_CONTRAST){
}

void FrTBCFilter::SetThreshold(double value){
    // need to set?    
    if(value != m_threshold){
        // change threshold
        m_threshold = ClampValue(value, MIN_THRESHOLD, MAX_THRESHOLD);
        this->Modified();
    }
}

void FrTBCFilter::SetBrightness(double value){
    // need to set?    
    if(value != m_brightness){
        // change threshold
        m_brightness = ClampValue(value, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
        this->Modified();
    }
}

void FrTBCFilter::SetContrast(double value){
    // need to set?    
    if(value != m_contrast){
        // change threshold
        m_contrast = ClampValue(value, MIN_CONTRAST, MAX_CONTRAST);
        this->Modified();
    }
}

// The execute method created by the subclass.
void FrTBCFilter::ThreadedExecute(vtkImageData *inData, 
                                  vtkImageData *outData, 
                                  int extent[6], int threadId){
    // Check for valid data type
    if (inData->GetScalarType() != VTK_UNSIGNED_CHAR){
        vtkErrorMacro(<<"FrTBCFilter: vtkImageData has to have only unsigned char data type.");
        return;
    }

    // NOTE: Changing threshold for now.
    int dims[3]; 
    inData->GetDimensions(dims);
    int numVoxs = dims[0] * dims[1] * dims[2];
    int numComps = inData->GetNumberOfScalarComponents();
    
    vtkDataArray* inArray = inData->GetPointData()->GetScalars();
    vtkDataArray* outArray = outData->GetPointData()->GetScalars();

    double maxValue = inData->GetScalarTypeMax();
    double threshold = maxValue * m_threshold;
    double brightness = maxValue * m_brightness;

    // Process image data
    for (int component = 0; component < numComps; ++component){
        for (int voxel = 0; voxel < numVoxs; ++voxel){
            // Check threshold and adjust brightness
            double out = 0.0;
            double value = inArray->GetComponent(voxel, component);
            out = (value >= threshold) ? value : 0.0;
            out += brightness;

            out = ClampValue(out, MIN_BYTE_VALUE, MAX_BYTE_VALUE);
            outArray->SetComponent(voxel, component, out);
        }
    }

    //if (divide == 0) {
    //    while (dstRowPtr < dstRowEndPtr){            
    //        int value = GetValue();
    //        int out = luTable[value];
    //        // Set out
    //    }
    //}
    //else {
    //    while (dstRowPtr < dstRowEndPtr) {
    //        int value = GetValue();
    //        int shiftIndex = value * 256;
    //        out = luTable[shiftIndex + value];
    //        // set out
    //    }
    //}
}

void FrTBCFilter::InitLookupTable(){

    //double contrast, brightness;
    //double mul, div;

    //// set mul and div
    //if (contrast < 0){
    //    mul = contrast + 100;
    //    div = 100;
    //}
    //else if (contrast > 0){
    //    mul = 100;
    //    div = 100 - contrast;
    //}
    //else {
    //    mul = 1;
    //    div = 1;
    //}

    //// create lookup table
    //unsigned char* luTable = 0;
    //if (!luTable) {
    //    luTable = new unsigned char[65536];
    //}
    //memset(luTable, 0, 65536);

    //// Initialize lut
    //if (div == 0){
    //    for (int i = 0; i < 256; ++i) {
    //        luTable[i] = ((i + brightness) < 128) ?  0 : 255;
    //    }
    //}
    //else if (div == 100) {
    //    for (int i = 0; i < 256; ++i) {
    //        int shift = (i - 127) * mul / div + (127 - i + brightness);

    //        for (int col = 0; col < 256; ++col){
    //            int index = (i * 256) + col;
    //            luTable[index] = ClampValue(col + shift, MIN_BYTE_VALUE, MAX_BYTE_VALUE);
    //        }
    //    }
    //}
    //else {
    //    for (int i = 0; i < 256; ++i) {
    //        int shift = (i - 127 + brightness) * mul / div + 127 - i;

    //        for (int col = 0; col < 256; ++col) {
    //            int index = (i * 256) + col;
    //            luTable[index] = ClampValue(col + shift, MIN_BYTE_VALUE, MAX_BYTE_VALUE);
    //        }
    //    }
    //}
}