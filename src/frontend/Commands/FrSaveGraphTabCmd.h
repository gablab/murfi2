#ifndef FR_SAVEGRAPHTAB_CMD
#define FR_SAVEGRAPHTAB_CMD

class FrGraphTabDocObj;
class FrTBCFilter;
class vtkCamera;

#include "FrBaseCmd.h"

// This command saves current active tab settings to document
// After this command is executed a new FrTabSettingsDocObj 
// is added into MainDocument.
class FrSaveGraphTabCmd : public FrBaseCmd {
public:
    typedef enum _Actions { Undefined, SaveNew, SaveCurrent } Actions;
public:
    // Constructor/destructor
    FrSaveGraphTabCmd();

	virtual bool Execute();

    // Undo/redo section
	virtual bool CanUndo();
	virtual bool Undo();
	virtual bool Redo();

    // Properties
    FrSetPropMacro(Actions, Action);
    FrSetPropMacro(bool, IsDefault);
    FrSetPropMacro(bool, IsCurrent);

private:
    bool SaveNewTab();
    bool SaveCurrentTab();
    void InitDocObjFromActive(FrGraphTabDocObj* docObj);
};

#endif // FR_SAVEGRAPHTAB_CMD
