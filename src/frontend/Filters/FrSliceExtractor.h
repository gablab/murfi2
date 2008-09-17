#ifndef FR_SLICE_EXTRACTOR
#define FR_SLICE_EXTRACTOR

#include "FrMacro.h"

// VTK stuff
#include "vtkExtractVOI.h"
#include "vtkImageReslice.h"
#include "vtkImageData.h"
#include "vtkObject.h"

class FrSliceExtractor : public vtkObject {
public:
    static FrSliceExtractor* New();

    // Makes output out-to-date. 
    // You should override this method
    virtual void Update();

	// Properties
	
	FrGetPropMacro(vtkImageData*, Input);
	void SetInput(vtkImageData* input);
	
	FrSetPropMacro(int, Axis);
	FrSetPropMacro(int, Frame);
	FrGetPropMacro(vtkImageData*, Output);

//	void SetOutputOrientation(int axis);
//	void SetCurrentFrame(int frame);

//  virtual void SimpleExecute(vtkImageData* input, vtkImageData* output);
//	vtkImageData* GetOutput();
	int GetMaxSliceNumber();

protected:
    FrSliceExtractor();
    ~FrSliceExtractor();

private:
    FrSliceExtractor(const FrSliceExtractor&);  // Not implemented.
    void operator=(const FrSliceExtractor&);  // Not implemented.

private:
//	int m_newDims[3];
	int m_Dims[3];
//	int m_frame;
//	int m_axis;
	int m_maxSliceNUmber;
	
//	vtkImageData* output2;
//	vtkExtractVOI* ImageVOI;
	vtkImageReslice* m_Reslice;

};




#endif