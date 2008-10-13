#ifndef FR_REMOVELAYER_CMD
#define FR_REMOVELAYER_CMD

class FrTabSettingsDocObj;

#include "FrBaseCmd.h"


class FrRemoveLayerCmd : public FrBaseCmd
{
public:
    // Constructor/destructor
    FrRemoveLayerCmd();

	virtual bool Execute();

    // Undo/redo section
	virtual bool CanUndo();
	virtual bool Undo();
	virtual bool Redo();
	
private:
	
};

#endif