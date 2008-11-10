#ifndef FR_MASKBASE_CMD
#define FR_MASKBASE_CMD

// Forward declarations
class vtkImageData;

// Some includes
#include "FrBaseCmd.h"

// Base abstract class for ROI editing commands.
class FrMaskBaseCmd : public FrBaseCmd {
public:
    // Constructor/destructor
    FrMaskBaseCmd();
    virtual ~FrMaskBaseCmd();

protected:
    // Returns vtkImageData of current slice of the current ROI.
    vtkImageData* GetCurrentROIImageData();
    // Applys given image data to current ROI
    virtual void ApplyToCurrentROI(vtkImageData* data);
};

#endif // FR_MASKBASE_CMD
