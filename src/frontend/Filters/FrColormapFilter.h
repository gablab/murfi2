#ifndef FR_COLORMAP_FILTER
#define FR_COLORMAP_FILTER

#include "FrBaseFilter.h"
#include "FrMacro.h"

#include "Qt\QColor.h"

class FrColormapFilter : public FrBaseFilter{
public:
	static FrColormapFilter* New();

	virtual void SetInput(vtkImageData* data);
	
    virtual void ExecuteInformation();
    virtual void SimpleExecute(vtkImageData* input, vtkImageData* output);

	FrPropMacro(int, Threshold);
	FrPropMacro(int, PxMin);
	FrPropMacro(int, PxMax);
	FrPropMacro(int, CMType);
	FrPropMacro(QColor, Color);

private:
    // VTK style
    FrColormapFilter();
    FrColormapFilter(const FrColormapFilter&);  // Not implemented.
    void operator=(const FrColormapFilter&);  // Not implemented.  	

	int GetId(int x, int y);
	void ProcessSingleColormap(vtkImageData* input, vtkImageData* output);
	void ProcessMultiColormap(vtkImageData* input, vtkImageData* output);

private:
	int m_Dims[3];

};

#endif