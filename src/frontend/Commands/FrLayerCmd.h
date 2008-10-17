#ifndef FR_LAYER_CMD
#define FR_LAYER_CMD

class FrTabSettingsDocObj;

#include "FrBaseCmd.h"
#include "FrMacro.h"

// This command allows user to add, remove 
// and modify settings of the layer
class FrLayerCmd : public FrBaseCmd
{
public:
    enum Action { Undefined, Add, Delete, Change };

public:
    // Constructor/destructor
    FrLayerCmd();

	virtual bool Execute();

    // Undo/redo section
	virtual bool CanUndo();
	virtual bool Undo();
	virtual bool Redo();
	
    // Properties
    FrSetPropMacro(Action, Action);
    void SetID(int id);

private:
    // Helper methods here
    bool AddLayer();
    bool DeleteLayer();
    bool ChangeLayer();

private:
    bool m_isID;
    int m_ID;
};

#endif