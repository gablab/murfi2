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
    void SetImageNumber(unsigned long value){
        m_ImageNumber = value;
    }

protected:
    // Returns current ROI document object
    FrRoiDocObj* GetCurrentRoi();
    // Return current slice number of ROI
    int GetCurrentRoiSliceNumber();
    // Returns vtkImageData of current slice of the current ROI.
    vtkImageData* GetRoiImageData(unsigned long id);    
    // Applys given image data to current ROI
    virtual void ApplyDataToRoi(vtkImageData* data, FrRoiDocObj* roiDO, int sliceNumber = -1);
    // Returns Special Layer
    FrSpecialLayer* GetSpecialLayer();
    // Transforms values in point[2] param to proper indices
    // imgID is number of image in ortho view...
    void TransformCoordinatesToIndices(int point[2], vtkImageData* img, unsigned long imgID);

protected:
    unsigned long m_ImageNumber;
};

#endif // FR_MASKBASE_CMD
