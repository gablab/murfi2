#ifndef FR_MASK2RGBA_FILTER
#define FR_MASK2RGBA_FILTER

// Includes
#include "FrBaseFilter.h"
#include "FrMacro.h"


// This class is used to transform image data from 
// mask image to RGBA image with opacity regions.
class FrMaskToRgbaFilter : public FrBaseFilter {
public:
    vtkTypeMacro(FrMaskToRgbaFilter,FrBaseFilter);
    static FrMaskToRgbaFilter* New();

public:
    // Overrides of base class
    virtual void ExecuteInformation();
    virtual void SimpleExecute(vtkImageData *inData, 
                               vtkImageData *outData);

private:
    // VTK style
    FrMaskToRgbaFilter(); // Do not allow using new
    FrMaskToRgbaFilter(const FrMaskToRgbaFilter&);  // Not implemented.
    void operator=(const FrMaskToRgbaFilter&);  // Not implemented.
};

#endif
