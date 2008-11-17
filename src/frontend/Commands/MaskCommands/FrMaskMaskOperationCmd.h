#ifndef FR_2MASKOPER_CMD
#define FR_2MASKOPER_CMD

// Forward declaration
class vtkImageLogic;
class vtkImageData;

// Some includes 
#include "FrMaskBaseCmd.h"
#include "FrMacro.h"

// This class implements command that support 
// dilation and erosion for ROI mask data.
class FrMaskMaskOperationCmd : public FrMaskBaseCmd {
public:
    enum Action { None, Intersect, Union, Subtract, Invert };

public:
    // Constructor/destructor
    FrMaskMaskOperationCmd();
    virtual ~FrMaskMaskOperationCmd();

    virtual bool Execute();

    // Undo/redo section
    virtual bool CanUndo();
    virtual bool Undo();
    virtual bool Redo();

    // Properties
    FrSetPropMacro(Action, Action);    

private:
    // Helpers
    bool PerformeUnionOperation(vtkImageData* in1, vtkImageData* in2);
    bool PerformeInvertOperation(vtkImageData* in1, vtkImageData* in2);
    bool PerformeIntersectOperation(vtkImageData* in1, vtkImageData* in2);
    bool PerformeSubtractOperation(vtkImageData* in1, vtkImageData* in2);
    vtkImageData* GetTargetRoiImageData();
};

#endif // FR_2MASKOPER_CMD
