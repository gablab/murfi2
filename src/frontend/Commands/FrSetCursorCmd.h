#ifndef FR_SETCURSOR_CMD
#define FR_SETCURSOR_CMD

// Forward declaration
 class FrTool;

#include "FrBaseCmd.h"
#include "FrMacro.h"

// Base abstract class for commands.  Supports undo and redo
// features.  Command is called by Execute() method.
// You should derive all your commans from this.
class FrSetCursorCmd : public FrBaseCmd
{
public:
    enum CursorType{ Unknown, Zoom, Pan };
public:
    // Constructor/destructor
    FrSetCursorCmd();

	virtual bool Execute();

    // Undo/redo section
	virtual bool CanUndo();
	virtual bool Undo();
	virtual bool Redo();

    // Properties 
    FrSetPropMacro(CursorType, CursorType);

private:

};

#endif // FR_SETCURSOR_CMD
