#ifndef FR_BASE_FILTER
#define FR_BASE_FILTER


#include "vtkImageSource.h"


class FrBaseFilter : public vtkImageSource {
public:
  // Set the Input of a filter. 
  virtual void SetInput(vtkImageData *input);
  vtkImageData *GetInput();
  
  virtual int FillOutputPortInformation(int port, vtkInformation* info);

  
protected:
    // This allow us to construct and destruct in derived classes
    FrBaseFilter();
    ~FrBaseFilter();

  // These are called by the superclass.
  // You might have to override ExecuteInformation
  virtual void ExecuteInformation();
  virtual void ComputeInputUpdateExtent(int inExt[6], int outExt[6]);

  // You don't have to touch this unless you have a good reason.
  virtual void ExecuteData(vtkDataObject *output);
  // In the simplest case, this is the only method you need to define.
  virtual void SimpleExecute(vtkImageData* input, vtkImageData* output) = 0;
  
private:
    // VTK style
  FrBaseFilter(const FrBaseFilter&);  // Not implemented.
  void operator=(const FrBaseFilter&);  // Not implemented.
};


#endif