#ifndef FR_UPDATEPL_CMD
#define FR_UPDATEPL_CMD

#include "FrBaseCmd.h"
#include "FrNotify.h"
#include "FrMacro.h"

class FrUpdatePipelineCmd : public FrBaseCmd {
public:
    // Constructor/destructor
    FrUpdatePipelineCmd();

	bool Execute();

    // Undo/redo section
	bool CanUndo();
	bool Undo();
	bool Redo();

    FrSetPropMacro(int, PipelinePoint);

};

#endif // FR_UPDATEPL_CMD