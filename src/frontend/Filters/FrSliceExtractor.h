#ifndef FR_SLICE_EXTRACTOR
#define FR_SLICE_EXTRACTOR

class vtkImageReslice;
class vtkImageData;

#include "FrMacro.h"
#include <map>

// VTK stuff
#include "vtkObject.h"

// This class extract specified image from 
// 3D volum image. It is simple wrapper for 
// vtkImageReslice class.
// Support multi port extracting.
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
    void SetInput(int port, vtkImageData* input);
    vtkImageData* GetInput();
    vtkImageData* GetInput(int port);
    vtkImageData* GetOutput();
    vtkImageData* GetOutput(int port);
    // Remove all oprts except port#0
    void ClearAdditionalPorts();
	
    // Properties
    enum Orientation { ZY = 0, XZ = 1, XY = 2 };
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
	
    // Reslicers
    typedef std::map<int, vtkImageReslice*> ReslicerCollection;
    ReslicerCollection m_Reslicer;
};

#endif
