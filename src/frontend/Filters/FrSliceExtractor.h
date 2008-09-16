#ifndef FR_SLICE_EXTRACTOR
#define FR_SLICE_EXTRACTOR

#include "FrBaseFilter.h"

// VTK stuff
#include "vtkExtractVOI.h"
#include "vtkImageReslice.h"
#include "vtkImageData.h"

class FrSliceExtractor : public FrBaseFilter {
public:
    static FrSliceExtractor* New();

	virtual void SetInput(vtkImageData* data);
//	void SetOutputDimensions(int w, int h, int num);
	void SetOutputOrientation(int axis);
	void SetCurrentFrame(int frame);
	void SetCurrentPlane(int plane);
	
    virtual void ExecuteInformation();
    virtual void SimpleExecute(vtkImageData* input, vtkImageData* output);
	vtkImageData* GetOutput();
	int GetMaxSliceNumber();

private:
    // VTK style
    FrSliceExtractor();
    FrSliceExtractor(const FrSliceExtractor&);  // Not implemented.
    void operator=(const FrSliceExtractor&);  // Not implemented.  

private:
//	int m_newDims[3];
	int m_Dims[3];
	int m_frame;
	int m_axis;
	int m_maxSliceNUmber;
	
	vtkImageData* output2;
	vtkExtractVOI* ImageVOI;
	vtkImageReslice* ImageReslice;

};




#endif