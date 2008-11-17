#ifndef FR_MASKBASE_CMD
#define FR_MASKBASE_CMD

// Forward declarations
class vtkImageData;
class FrRoiDocObj;
class FrSpecialLayer;

// Some includes
#include "FrBaseCmd.h"

// Base abstract class for ROI editing commands.
class FrMaskBaseCmd : public FrBaseCmd {
public:
    // Constructor/destructor
    FrMaskBaseCmd();
    virtual ~FrMaskBaseCmd();

    // Properties
    void SetImageNumber(int value){
        m_ImageNumber = value;
    }

protected:
    // Returns current ROI document object
    FrRoiDocObj* GetCurrentRoi();
    // Return current slice number of ROI
    int GetCurrentRoiSliceNumber();
    // Returns vtkImageData of current slice of the current ROI.
    vtkImageData* GetRoiImageData(int id);    
    // Applys given image data to current ROI
    virtual void ApplyDataToRoi(vtkImageData* data, FrRoiDocObj* roiDO, int sliceNumber = -1);
    // Returns Special Layer
    FrSpecialLayer* GetSpecialLayer();

protected:
    int m_ImageNumber;
};

#endif // FR_MASKBASE_CMD
