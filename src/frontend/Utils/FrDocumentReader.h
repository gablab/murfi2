#ifndef FR_DOC_READER
#define FR_DOC_READER

// Forward declarations
class FrDocument;
class vtkImageData;
class FrImageDocObj;
class FrRoiDocObj;


template <class T>
class RtDataImage;
class RtMRIImage;
class RtMaskImage;

// Some includes
#include "FrMacro.h"
#include "FrCrosshair.h"
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
        Mri, Roi, Activation, Points
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
    FrGetPropMacro(FrDocument*,Document);
    void SetDocument(FrDocument* document);
    
    // Turns ON mosaic mode.
    FrGetPropMacro(bool, Mosaic);
    void SetMosaic(bool value);
    
    FrGetPropMacro(Targets, Target)
    void SetTarget(Targets target);

    FrGetPropMacro(Orientations, Orientation);
    void SetOrientation(Orientations value);

    FrGetPropMacro(int, Slice);
    void SetSlice(int slice);
    
    FrGetPropMacro(unsigned int, DataID);
    void SetDataID(unsigned int ID);

    // Output management
    FrGetPropMacro(vtkImageData*, Output);

    // Slice management
    FrGetPropMacro(unsigned int, TimeSeries);
    void SetTimeSeries(unsigned int ID);

    CrosshairParams ReadCrosshair();

private:
    void SetOutput(vtkImageData* data);
    
    vtkImageData* ReadMri(); 
    vtkImageData* ReadRoi();
    vtkImageData* ReadActivation();
    vtkImageData* ReadPoints();

    vtkImageData* GetMriSlice(RtMRIImage* mri); 
    vtkImageData* GetRoiSlice(RtMaskImage* mask);
    vtkImageData* GetPointsSlice(vtkImageData* data);
    ////vtkImageData* GetActivationSlice();
    template <class T>
    T* GetSliceDataXY(RtDataImage<T>* img, int slice);
    template <class T>
    T* GetSliceDataYZ(RtDataImage<T>* img, int slice);
    template <class T>
    T* GetSliceDataXZ(RtDataImage<T>* img, int slice);

    void CreateMriLUT(short* data, unsigned int dataSize, 
                      unsigned char** outLUT);
    
protected:
    FrDocumentReader();
    ~FrDocumentReader();
    
private:
    FrDocumentReader(const FrDocumentReader&);  // Not implemented.
    void operator=(const FrDocumentReader&);  // Not implemented.
};

// Due to gcc template compilation limitations
#include "FrDocumentReader.tpp"

#endif
