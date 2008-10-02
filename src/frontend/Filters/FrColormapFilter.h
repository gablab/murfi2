#ifndef FR_COLORMAP_FILTER
#define FR_COLORMAP_FILTER

#include "FrBaseFilter.h"

class FrColormapFilter : public FrBaseFilter{
public:
	static FrColormapFilter* New();

	virtual void SetInput(vtkImageData* data);
	
    virtual void ExecuteInformation();
    virtual void SimpleExecute(vtkImageData* input, vtkImageData* output);


private:
    // VTK style
    FrColormapFilter();
    FrColormapFilter(const FrColormapFilter&);  // Not implemented.
    void operator=(const FrColormapFilter&);  // Not implemented.  	

	int GetId(int x, int y);

private:
	int m_Dims[3];

};

#endif