#ifndef FR_MASKDILATEERODE_CMD
#define FR_MASKDILATEERODE_CMD

// Forward declaration
class vtkImageDilateErode3D;

// Some includes 
#include "FrMaskBaseCmd.h"
#include "FrMacro.h"

// This class implements command that support 
// dilation and erosion for ROI mask data.
// Used square kernel for operation.
class FrMaskDilateErodeCmd : public FrMaskBaseCmd {
    enum Action { None, Dilate, Erode };
public:
    // Constructor/destructor
    FrMaskDilateErodeCmd();
    virtual ~FrMaskDilateErodeCmd();

    virtual bool Execute();

    // Undo/redo section
    virtual bool CanUndo();
    virtual bool Undo();
    virtual bool Redo();

    // Propertiy
    FrSetPropMacro(Action, Action);
    FrSetPropMacro(int, KernelSize);

private:
    // Helpers
    bool DoDilate();
    bool DoErode();

private:
    vtkImageDilateErode3D* m_Filter;
};

#endif // FR_MASKDILATEERODE_CMD
