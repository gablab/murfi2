#ifndef FR_COLORMAP_FILTER
#define FR_COLORMAP_FILTER

#include "FrBaseFilter.h"
#include "FrMacro.h"

#include "Qt/qcolor.h"

class FrColormapFilter : public FrBaseFilter{
public:
    enum Type { SingleColor, MultiColor };
public:
    vtkTypeMacro(FrColormapFilter,FrBaseFilter);
	static FrColormapFilter* New();

public:
	virtual void SetInput(vtkImageData* data);
	
    virtual void ExecuteInformation();
    virtual void SimpleExecute(vtkImageData* input, vtkImageData* output);

    // Properties
	FrGetPropMacro(int, PxMin);
    void SetPxMin(int value);
	FrGetPropMacro(int, PxMax);
    void SetPxMax(int value);
	FrGetPropMacro(int, PxMid);
    void SetPxMid(int value);
	FrGetPropMacro(int, Threshold);
    void SetThreshold(int value);
	FrGetPropMacro(int, Type);
    void SetType(int value);
	FrGetPropMacro(QColor, Color);
    void SetColor(QColor value);

    FrGetPropMacro(bool, IsModified);

private:
    // VTK style
    FrColormapFilter();
    FrColormapFilter(const FrColormapFilter&);  // Not implemented.
    void operator=(const FrColormapFilter&);  // Not implemented.  	

    void InitSingleLookupTable(unsigned char luTable[][3]);
    void InitMultiLookupTable(unsigned char luTable[][3]);

private:
	int m_Dims[3];

};

#endif
