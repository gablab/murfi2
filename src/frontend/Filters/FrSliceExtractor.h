#ifndef FR_SLICE_EXTRACTOR
#define FR_SLICE_EXTRACTOR

class vtkImageReslice;
class vtkImageData;

#include "FrMacro.h"

// VTK stuff
#include "vtkObject.h"

// This class extract specified image from 
// 3D volum image. It is simple wrapper for 
// vtkImageReslice class.
class FrSliceExtractor : public vtkObject {
public:
    vtkTypeMacro(FrSliceExtractor,vtkObject);
    static FrSliceExtractor* New();

public:
    // Makes output out-to-date. 
    // You should override this method
    virtual void Update();

    // input / output managing
	void SetInput(vtkImageData* input);
    vtkImageData* GetInput();
    vtkImageData* GetOutput();
	
    // Properties
    enum Orientation { YZ = 0, XZ = 1, XY = 2 };
	FrGetPropMacro(Orientation, Orientation);
    void SetOrientation(Orientation orient);

	FrGetPropMacro(int, Slice);
    void SetSlice(int slice);

    // Returns max slice number
	int GetMaxSliceNumber();

protected:
    FrSliceExtractor();
    ~FrSliceExtractor();

private:
    FrSliceExtractor(const FrSliceExtractor&);  // Not implemented.
    void operator=(const FrSliceExtractor&);  // Not implemented.

private:
	int m_maxSliceNUmber;
    double m_Spacing[3];

	vtkImageReslice* m_Reslicer;
};

#endif
