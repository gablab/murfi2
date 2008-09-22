#ifndef FR_UPDTABS_CMD
#define FR_UPDTABS_CMD

class FrTabSettingsDocObj;

#include "FrBaseCmd.h"

// This command is used if needed to update tabs settings
// in view (i.e. add, remove, update ) or in document (UpdateCurrent)
class FrUpdateTabsCmd : public FrBaseCmd
{
public:
    enum CmdType { None, Add, Remove, Update, UpdateCurrent };

public:
    // Constructor/destructor
    FrUpdateTabsCmd();

    // Properties
    FrSetPropMacro(CmdType, Action);
    FrSetPropMacro(FrTabSettingsDocObj*, TabSettingsDocObj);
    FrSetPropMacro(int, TabID);

    // Overrides
	bool Execute();

    // Undo/redo section
	bool CanUndo();
	bool Undo();
	bool Redo();

private:
    bool UpdateDocument();
    bool UpdateView();
};

#endif // FR_UPDTABS_CMD