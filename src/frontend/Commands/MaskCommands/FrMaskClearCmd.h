#ifndef FR_MASKCLEAR_CMD
#define FR_MASKCLEAR_CMD

#include "FrMaskBaseCmd.h"
#include "FrMacro.h"

// This class implements command that clear ROI mask data.
class FrMaskClearCmd : public FrMaskBaseCmd {
public:
    // Constructor/destructor
    FrMaskClearCmd();
    virtual ~FrMaskClearCmd();

    virtual bool Execute();

    // Undo/redo section
    virtual bool CanUndo();
    virtual bool Undo();
    virtual bool Redo();

private:

};

#endif