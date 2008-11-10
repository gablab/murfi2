#ifndef FR_MASKCOPY_CMD
#define FR_MASKCOPY_CMD

// Forward declaration

// Some includes 
#include "FrMaskBaseCmd.h"
#include "FrMacro.h"

// This class implements command that support 
// dilation and erosion for ROI mask data.
class FrMaskCopyAdjacentCmd : public FrMaskBaseCmd {
public:
    // Constructor/destructor
    FrMaskCopyAdjacentCmd();
    virtual ~FrMaskCopyAdjacentCmd();

    virtual bool Execute();

    // Undo/redo section
    virtual bool CanUndo();
    virtual bool Undo();
    virtual bool Redo();

private:
    // Helpers

private:
};

#endif // FR_MASKCOPY_CMD
