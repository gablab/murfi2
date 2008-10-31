#ifndef FR_MOSAIC_FILTER
#define FR_MOSAIC_FILTER

#include "FrBaseFilter.h"

class FrMosaicFilter : public FrBaseFilter {
public:
    vtkTypeMacro(FrMosaicFilter,FrBaseFilter);
    static FrMosaicFilter* New();

public:
	virtual void SetInput(vtkImageData* data);
	void SetOutputDimensions(int w, int h, int num);
	
    virtual void ExecuteInformation();
    virtual void SimpleExecute(vtkImageData* input, vtkImageData* output);

private:
    // VTK style
    FrMosaicFilter();
    FrMosaicFilter(const FrMosaicFilter&);  // Not implemented.
    void operator=(const FrMosaicFilter&);  // Not implemented.  

	int GetNewId(int slice, int x, int y);

private:
	int m_newDims[3];
	int m_oldDims[3];

};

#endif
