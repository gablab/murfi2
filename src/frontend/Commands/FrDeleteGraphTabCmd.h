#ifndef FR_DELETEGRAPHTAB_CMD
#define FR_DELETEGRAPHTAB_CMD

// Forward declaration
class FrGraphTabSetDocObj;

//Includes
#include "FrBaseCmd.h"

// Delete specified tab from document
class FrDeleteGraphTabCmd : public FrBaseCmd
{
public:
    // Constructor/destructor
    FrDeleteGraphTabCmd();

	virtual bool Execute();

    // Undo/redo section
	virtual bool CanUndo();
	virtual bool Undo();
	virtual bool Redo();

    // Properties
    FrSetPropMacro(unsigned long, TabID);
};

#endif // FR_DELETEGRAPHTAB_CMD
