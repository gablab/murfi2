#ifndef FR_DOC_READER
#define FR_DOC_READER

class FrDocument;
class vtkImageData;

#include "FrMacro.h"
#include "vtkObject.h"

class FrDocumentReader : public vtkObject {
public:
    vtkTypeMacro(FrDocumentReader,vtkObject);
    static FrDocumentReader* New();

public:

    // Makes output out-to-date. 
    // You should override this method
    virtual void Update();

    // Properties

    // MosaicOn and UnMosaicOn are selfexculed.
    // i.e. they canot be both true.
    FrGetPropMacro(bool,MosaicOn);
    void SetMosaicOn();
    FrGetPropMacro(bool,UnMosaicOn);
    void SetUnMosaicOn();

    FrGetPropMacro(FrDocument*,Document);
    void SetDocument(FrDocument* document);

    FrGetPropMacro(vtkImageData*,Output);
    void SetOutput(vtkImageData* output);

protected:
    FrDocumentReader();
    ~FrDocumentReader();

    // Lut has always 256 byte size
    void InitLookupTable(short* data, unsigned int dataSize, 
                         unsigned char** outLUT);
    
private:
    FrDocumentReader(const FrDocumentReader&);  // Not implemented.
    void operator=(const FrDocumentReader&);  // Not implemented.
};

#endif