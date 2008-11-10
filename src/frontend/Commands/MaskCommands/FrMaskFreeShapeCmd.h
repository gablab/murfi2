#ifndef FR_MASKFREESHAPE_CMD
#define FR_MASKFREESHAPE_CMD

// Forward declaration

// Some includes 
#include "FrMaskBaseCmd.h"
#include "FrMacro.h"

// This class implements command that support 
// dilation and erosion for ROI mask data.
class FrMaskFreeShapeCmd : public FrMaskBaseCmd {
public:
    // Constructor/destructor
    FrMaskFreeShapeCmd();
    virtual ~FrMaskFreeShapeCmd();

    virtual bool Execute();

    // Undo/redo section
    virtual bool CanUndo();
    virtual bool Undo();
    virtual bool Redo();

private:
    // Helpers
private:
};

#endif // FR_MASKFREESHAPE_CMD
