#ifndef FR_MANAGETOOL_CMD
#define FR_MANAGETOOL_CMD

// Forward declaration
 class FrTool;

#include "FrBaseCmd.h"
#include "FrMacro.h"

// Base abstract class for commands.  Supports undo and redo
// features.  Command is called by Execute() method.
// You should derive all your commans from this.
class FrManageToolCmd : public FrBaseCmd
{
public:
    enum ToolType{ UnknownTool, ManipulationTool, VoxelTool, RoiTool, VoxelSelectionTool };
    enum ToolAction { UnknownAct, NewToolAct, AddToolAct, RemoveToolAct, UpdateAct };
public:
    // Constructor/destructor
    FrManageToolCmd();

	virtual bool Execute();

    // Undo/redo section
	virtual bool CanUndo();
	virtual bool Undo();
	virtual bool Redo();

    // Properties 
    FrSetPropMacro(ToolType, ToolType);
    FrSetPropMacro(ToolAction, ToolAction);
    FrSetPropMacro(bool, IsChecked);

private:
    // Setup current tool to specified by ToolType
    // if any tool is active then remove it
    bool SetNewToolAction();
    // adds tool but without removing old one
    // useful for shortcuts
    bool AddToolAction();
    // removes current tool 
    bool RemoveToolAction();
    // Updates UI toreflect current tool
    bool UpdateCurrentToolAction();

    FrTool* CreateToolByType(ToolType toolType);
};

#endif // FR_BASE_CMD
