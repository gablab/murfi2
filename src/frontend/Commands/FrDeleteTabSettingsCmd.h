#ifndef FR_DELETETABSET_CMD
#define FR_DELETETABSET_CMD

// Forward declaration
class FrTabSettingsDocObj;

//Includes
#include "FrBaseCmd.h"

// Delete specified tab from document
class FrDeleteTabSettingsCmd : public FrBaseCmd
{
public:
    // Constructor/destructor
    FrDeleteTabSettingsCmd();

	virtual bool Execute();

    // Undo/redo section
	virtual bool CanUndo();
	virtual bool Undo();
	virtual bool Redo();

    // Properties
    FrSetPropMacro(int, TabID);
};

#endif // FR_DELETETABSET_CMD