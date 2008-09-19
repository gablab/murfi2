#ifndef FR_MULTI_CMD
#define FR_MULTI_CMD

#include <vector>
#include "FrBaseCmd.h"

// This command support manipulation with several commands
// as if they were a single command.
class FrMultiCmd : FrBaseCmd
{
public:
    // Constructor/destructor
    FrMultiCmd();
    virtual ~FrMultiCmd();

	virtual bool Execute();

    // Undo/redo section
	virtual bool CanUndo();
	virtual bool Undo();
	virtual bool Redo();

    // adds command to list
    // NOTE: you have to add command created by new
    void AddCommand(FrBaseCmd* cmd);

private:
    typedef std::vector<FrBaseCmd*> CmdList;
    CmdList m_commands;
};

#endif // AKL_COMMAND_H