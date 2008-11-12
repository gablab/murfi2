#ifndef FR_ROI_CMD
#define FR_ROI_CMD

// Forward declarations
class FrRoiDocObj;

// Some includes 
#include "FrBaseCmd.h"
#include "FrMacro.h"

// This command allows user to add, remove 
// and modify settings of the layer
class FrRoiCmd : public FrBaseCmd {
public:
    enum Action { None, Add, Remove };

public:
    // Constructor/destructor
    FrRoiCmd();

    virtual bool Execute();

    // Undo/redo section
    virtual bool CanUndo();
    virtual bool Undo();
    virtual bool Redo();

    // Properties
    FrSetPropMacro(Action, Action);
    FrSetPropMacro(FrRoiDocObj*, Roi);

private:
    // Helper methods here
    bool AddRoi();
    bool RemoveRoi();
};

#endif
