#ifndef FR_TIMEPOINT_CMD
#define FR_TIMEPOINT_CMD

// Forward declarations
class FrImageDocObj;

// Some includes 
#include "FrBaseCmd.h"
#include "FrMacro.h"

// This command allows user to add, remove 
// and modify settings of the layer
class FrTimePointCmd : public FrBaseCmd {
public:
    typedef enum _Actions { None, 
        SetLast, SetNext, SetPrevious, SetUserDefined 
    } Actions;

public:
    // Constructor/destructor
    FrTimePointCmd();

    virtual bool Execute();

    // Undo/redo section
    virtual bool CanUndo();
    virtual bool Undo();
    virtual bool Redo();

    // Properties
    FrSetPropMacro(Actions, Action);
    FrSetPropMacro(unsigned int, TimePoint);
    FrSetPropMacro(bool, CheckLifeMode);

private:
    // Helper methods here
    bool SetLastTimePoint();
    bool SetNextTimePoint();
    bool SetPreviousTimePoint();
    bool SetUserDefinedTimePoint();

    FrImageDocObj* GetImageObject();
};

#endif
