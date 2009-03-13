#ifndef FR_BASE_CMD
#define FR_BASE_CMD

// This makes user no need to include 
// those headers in command source file.
#include "FrMainController.h"

// Base abstract class for commands.  Supports undo and redo
// features.  Command is called by Execute() method.
// You should derive all your commans from this.
class FrBaseCmd {
public:
    // Constructor/destructor
    FrBaseCmd() : m_MainController(0) { }
    virtual ~FrBaseCmd(){}

    virtual bool Execute() = 0;

    // Undo/redo section
    virtual bool CanUndo() = 0;
    virtual bool Undo() = 0;
    virtual bool Redo() = 0;

    // Properties
    FrPropMacro(FrMainController*, MainController);

protected:
    void UpdatePipelineForID(unsigned long id, int point);
};

#endif // FR_BASE_CMD
