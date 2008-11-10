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
    bool PerformeStandardOperation();
    bool PerformeSubtractOperation();
    vtkImageData* GetTargetROIImageData();

private:
    vtkImageLogic* m_Filter;
};

#endif // FR_2MASKOPER_CMD
