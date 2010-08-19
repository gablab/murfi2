#ifndef FR_VIEWDOC_OBJ
#define FR_VIEWDOC_OBJ

// Includes
#include "FrMacro.h"
#include "FrDocumentObj.h"
#include "FrViewSettings.h"
#include "vtkImageData.h"

//#include "FrUtils.h"
//#include "add_functions.h"

//#define BAD_LAYER_ID  0
//#define ALL_LAYER_ID  1
//#define SPEC_LAYRE_ID 2

class FrTabSettingsDocObj;

// This class represents View settings
// currently used by the Application.
class FrViewDocObj : public FrDocumentObj {
public:
    FrViewDocObj();
    virtual ~FrViewDocObj();

    // Overrides
    virtual void OnAdd(FrDocument* doc);
    virtual void OnRemove(FrDocument* doc);
    virtual ObjTypes GetType();

    // Type checking
    bool IsSliceView();
    bool IsMosaicView();
    bool IsOrthoView();

    // Methods
    void CopySettingsFrom(FrTabSettingsDocObj* src);
    void SaveSettingsTo(FrTabSettingsDocObj* dst);
    unsigned long GetActiveLayerID();

    // Properties

    // ID of the current image time point
    FrPropMacro(unsigned int, TimePoint); 

    FrPropMacro(Views, ActiveView);
    FrGetPropMacro(FrSliceViewSettings*,  SliceViewSettings);
    FrGetPropMacro(FrMosaicViewSettings*, MosaicViewSettings);
    FrGetPropMacro(FrOrthoViewSettings*,  OrthoViewSettings);

    void TransformCoordinatesToIndices(int point[2], 
				       vtkImageData *img,
				       int orientation = 0);

    void TransformCoordinatesToIndices(double point[2], 
				       vtkImageData *img,
				       int orientation = 0);

    void TransformCoordinatesToIndices(double &x, double &y, 
				       vtkImageData *img,
				       int orientation = 0);

    void TransformIndicesToCoordinates(int point[2], 
				       vtkImageData *img,
				       int orientation = 0);

    void TransformIndicesToCoordinates(double point[2], 
				       vtkImageData *img,
				       int orientation = 0);

    void TransformIndicesToCoordinates(double &x, double &y, 
				       vtkImageData *img,
				       int orientation = 0);
};

#endif // FR_VIEWDOC_OBJ

