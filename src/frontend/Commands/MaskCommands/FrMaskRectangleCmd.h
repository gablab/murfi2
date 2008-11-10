#ifndef FR_MASKRECTANGLE_CMD
#define FR_MASKRECTANGLE_CMD

// Forward declaration

// Some includes 
#include "FrMaskBaseCmd.h"
#include "FrMacro.h"

// This class implements command that support 
// dilation and erosion for ROI mask data.
class FrMaskRectangleCmd : public FrMaskBaseCmd {
public:
    // Constructor/destructor
    FrMaskRectangleCmd();
    virtual ~FrMaskRectangleCmd();

    virtual bool Execute();

    // Undo/redo section
    virtual bool CanUndo();
    virtual bool Undo();
    virtual bool Redo();

private:
    // Helpers

private:
};

#endif // FR_MASKRECTANGLE_CMD
