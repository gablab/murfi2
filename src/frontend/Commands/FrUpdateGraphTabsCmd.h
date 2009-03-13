#ifndef FR_UPDGRAPHTABS_CMD
#define FR_UPDGRAPHTABS_CMD

//Forward declarations
class FrGraphTabDocObj;

// Includes
#include "FrBaseCmd.h"

// Some defines
#define BAD_TAB_ID -1
#define CURRENT_TAB_ID -2

// This command is used if needed to update tabs settings
// in view (i.e. add, remove, update ) or in document (UpdateCurrent)
class FrUpdateGraphTabsCmd : public FrBaseCmd
{
public:
    typedef enum _Actions { 
        None, Add, Remove, Update, SetCurrentTab 
    } Actions;

public:
    // Constructor/destructor
    FrUpdateGraphTabsCmd();

    // Properties
    FrSetPropMacro(Actions, Action);
    FrSetPropMacro(FrGraphTabDocObj*, GraphTabDocObj);
    FrSetPropMacro(unsigned long, TabID);

    // Overrides
	bool Execute();

    // Undo/redo section
	bool CanUndo();
	bool Undo();
	bool Redo();

private:
    bool SetupTab();
    bool UpdateGraphs();
    
    void SetupTabObjects();

};

#endif // FR_UPDGRAPHTABS_CMD
