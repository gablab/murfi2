#ifndef FR_MASK_EDITOR
#define FR_MASK_EDITOR

// Forward declarations
class FrDocument;
class vtkImageData;
class FrRoiDocObj;
class RtMaskImage;

// Some includes
#include "FrMacro.h"
#include "vtkObject.h"


// This class performes editing of FrRoiDocObj objects.
// It reads image data or ROI data (depend on Target specified).
class FrMaskEditor : public vtkObject {
public:
    enum Orientation { Undefined=3, ZY=0, XZ=1, XY=2 };
    enum View { Unknown, Slice, Mosaic, Ortho };

public:
    vtkTypeMacro(FrMaskEditor,vtkObject);
    static FrMaskEditor* New();
        
    void SetInput(vtkImageData* in);
    void Update();

    // Properties
    FrSetPropMacro(View, View);
    FrSetPropMacro(Orientation, Orientation);    
    FrSetPropMacro(FrRoiDocObj*, DocumentObj);
    FrSetPropMacro(int, SliceNumber);

private:
    // Helpers
    bool IsDataValid(RtMaskImage* img, vtkImageData* data);
    void UpdateSliceXY(RtMaskImage* img, vtkImageData* data);
    void UpdateSliceZY(RtMaskImage* img, vtkImageData* data);
    void UpdateSliceZX(RtMaskImage* img, vtkImageData* data);
    
protected:
    FrMaskEditor();
    ~FrMaskEditor();
   
private:
    // VTK style
    FrMaskEditor(const FrMaskEditor&);  // Not implemented.
    void operator=(const FrMaskEditor&);  // Not implemented.

    // Data to be modified
    vtkImageData* m_Data;
};

#endif
