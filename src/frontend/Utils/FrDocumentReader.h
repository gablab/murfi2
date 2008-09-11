#ifndef FR_DOC_READER
#define FR_DOC_READER

class FrDocument;
class vtkImageData;

#include "FrMacro.h"
#include "vtkObject.h"

class FrDocumentReader : public vtkObject {
public:
    static FrDocumentReader* New();

    // Makes output out-to-date. 
    // You should override this method
    virtual void Update();

    // Properties
    FrGetPropMacro(FrDocument*,Document);
    void SetDocument(FrDocument* document);
    FrGetPropMacro(vtkImageData*,Output);
    void SetOutput(vtkImageData* output);

protected:
    FrDocumentReader();
    ~FrDocumentReader();
    
private:
    FrDocumentReader(const FrDocumentReader&);  // Not implemented.
    void operator=(const FrDocumentReader&);  // Not implemented.
};

#endif