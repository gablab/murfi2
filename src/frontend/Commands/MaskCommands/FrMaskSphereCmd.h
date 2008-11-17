#ifndef FR_MASKSPHERE_CMD
#define FR_MASKSPHERE_CMD

// Forward declaration


// Some includes 
#include "FrMaskBaseCmd.h"
#include "FrMacro.h"
#include "FrSelection.h"

// This class implements command that support 
// dilation and erosion for ROI mask data.
class FrMaskSphereCmd : public FrMaskBaseCmd {
public:
    enum Action { Undefined, Write, Erase };
public:
    // Constructor/destructor
    FrMaskSphereCmd();
    virtual ~FrMaskSphereCmd();

    virtual bool Execute();

    // Undo/redo section
    virtual bool CanUndo();
    virtual bool Undo();
    virtual bool Redo();

    // Properties
    FrSetPropMacro(Action, Action);
    FrSetPropMacro(Pos, Center);
    FrSetPropMacro(int, Radius);
    FrSetPropMacro(int, ImageNumber);

private:
    // Helpers

private:
};

#endif // FR_MASKSPHERE_CMD
