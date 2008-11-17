#ifndef FR_MASKBASE_CMD
#define FR_MASKBASE_CMD

// Forward declarations
class vtkImageData;
class FrRoiDocObj;

// Some includes
#include "FrBaseCmd.h"

// Base abstract class for ROI editing commands.
class FrMaskBaseCmd : public FrBaseCmd {
public:
    // Constructor/destructor
    FrMaskBaseCmd();
    virtual ~FrMaskBaseCmd();

    // Properties
    FrSetPropMacro(int, ImageNumber);

protected:
    // Returns current 
    FrRoiDocObj* GetCurrentRoi();
    // Returns vtkImageData of current slice of the current ROI.
    vtkImageData* GetRoiImageData(int id);
    // Applys given image data to current ROI
    virtual void ApplyToCurrentROI(vtkImageData* data);    
};

#endif // FR_MASKBASE_CMD
