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
// It reads image data specified by SetTarget() method.
// MRI has unsigned char type, Roi has unsigned char type,
// Activation has double. In addition Mri data are normalized
// to be in the range [0..255]
class FrDocumentReader : public vtkObject {
public:
    typedef enum _Targets {
        Mri, Roi, Activation
    } Targets;

    typedef enum _Orientaion { 
        XY, YZ, XZ 
    } Orientations;

public:
    vtkTypeMacro(FrDocumentReader,vtkObject);
    static FrDocumentReader* New();

public:
    // Makes output out-to-date. 
    // You should override this method
    virtual void Update();

    // Properties
    // Turns ON mosaic mode.
    FrGetPropMacro(bool, Mosaic);
    void SetMosaic(bool value);
    
    FrGetPropMacro(FrDocument*,Document);
    void SetDocument(FrDocument* document);
    
    FrGetPropMacro(Targets, Target)
    void SetTarget(Targets target);

    // Slice management
    FrGetPropMacro(unsigned int, DataID);
    void SetDataID(unsigned int ID);

    FrGetPropMacro(Orientations, Orientation);
    void SetOrientation(Orientations value);

    FrGetPropMacro(int, slice);
    void SetSlice(int slice);
    
    // Output management
    FrGetPropMacro(vtkImageData*, Output);

private:
    void SetOutput(vtkImageData* data);
    
    vtkImageData* ReadMri(); 
    vtkImageData* ReadRoi();
    vtkImageData* ReadActivation();

    // Lut has always 256 byte size
    #define LUT_SIZE 256
    void InitMriLUT(short* data, unsigned int dataSize, 
                    unsigned char** outLUT);
    
protected:
    FrDocumentReader();
    ~FrDocumentReader();
    
private:
    FrDocumentReader(const FrDocumentReader&);  // Not implemented.
    void operator=(const FrDocumentReader&);  // Not implemented.
};

#endif
