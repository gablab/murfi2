#ifndef FR_ADDLAYER_CMD
#define FR_ADDLAYER_CMD

class FrTabSettingsDocObj;

#include "FrBaseCmd.h"


class FrAddLayerCmd : public FrBaseCmd
{
public:
    // Constructor/destructor
    FrAddLayerCmd();

	virtual bool Execute();

    // Undo/redo section
	virtual bool CanUndo();
	virtual bool Undo();
	virtual bool Redo();
	
private:
	
};

#endif