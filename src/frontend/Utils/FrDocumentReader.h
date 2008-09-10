#ifndef FR_DOC_READER
#define FR_DOC_READER

class FrDocument;
class vtkDataObject;
class vtkInformationVector;

#include "FrMacro.h"
#include "vtkImageAlgorithm.h"

class FrDocumentReader : public vtkImageAlgorithm {
public:
    static FrDocumentReader* New();

    // Properties
    FrPropMacro(FrDocument*,Document);

protected:
    FrDocumentReader();
    ~FrDocumentReader();

    // Execute method
    //virtual void ExecuteInformation();
    virtual void ExecuteData(vtkDataObject* data);
    virtual vtkImageData* AllocateOutputData(vtkDataObject* data);

    // Request info. You do not need to override it.
    virtual int RequestInformation(vtkInformation* request,
                                   vtkInformationVector** inputVector,
                                   vtkInformationVector* outputVector);
       
private:
    FrDocumentReader(const FrDocumentReader&);  // Not implemented.
    void operator=(const FrDocumentReader&);  // Not implemented.
};

#endif