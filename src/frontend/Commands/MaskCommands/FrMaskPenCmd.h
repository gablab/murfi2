#ifndef FR_MASKPEN_CMD
#define FR_MASKPEN_CMD

// Forward declaration


// Some includes 
#include "FrMaskBaseCmd.h"
#include "FrMacro.h"
#include "FrSelection.h"

// This class implements command that support 
// dilation and erosion for ROI mask data.
class FrMaskPenCmd : public FrMaskBaseCmd {
public:
    // Constructor/destructor
    FrMaskPenCmd();
    virtual ~FrMaskPenCmd();

    virtual bool Execute();

    // Undo/redo section
    virtual bool CanUndo();
    virtual bool Undo();
    virtual bool Redo();

    // Properties
    FrSetPropMacro(Pos, Center);
    FrSetPropMacro(int, Radius);
    FrSetPropMacro(int, ImageNumber);

private:
    // Helpers

private:
};

#endif // FR_MASKPEN_CMD
