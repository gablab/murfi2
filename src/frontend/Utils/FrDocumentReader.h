#ifndef FR_DOC_READER
#define FR_DOC_READER

// Forward declarations
class FrDocument;
class vtkImageData;
class FrImageDocObj;
class FrRoiDocObj;

// Some includes
#include "FrMacro.h"
#include "vtkObject.h"

#include <vector>

// This class performes reading of FrMainDocument objects.
// It reads image data or ROI data (depend on Target specified). 
class FrDocumentReader : public vtkObject {
public:
    enum Target { Image, ROI };
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
    
    FrGetPropMacro(Target, Target)
    void SetTarget(Target tgt);

    // Output management
    int GetOutputCount();
    vtkImageData* GetOutput();
    vtkImageData* GetOutput(int port);

private:    
    void AddOutput(vtkImageData* data);
    void ClearOutputs();

    vtkImageData* ReadImage(FrImageDocObj* imgDO);
    vtkImageData* ReadROI(FrRoiDocObj* roiDO);
    
protected:
    FrDocumentReader();
    ~FrDocumentReader();

    // Lut has always 256 byte size
    void InitLookupTable(short* data, unsigned int dataSize, 
                         unsigned char** outLUT);
    
private:
    FrDocumentReader(const FrDocumentReader&);  // Not implemented.
    void operator=(const FrDocumentReader&);  // Not implemented.

private:
    // Collection of putput ports
    typedef std::vector<vtkImageData*> OutputCollection;
    OutputCollection m_Outputs;
};

#endif
