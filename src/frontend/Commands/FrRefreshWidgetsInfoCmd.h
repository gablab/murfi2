#ifndef FR_REFRESH_WIDGETS_INFO_CMD
#define FR_REFRESH_WIDGETS_INFO_CMD

// This makes user no need to include 
// those headers in command source file.
#include "FrBaseCmd.h"

class FrRefreshWidgetsInfoCmd : public FrBaseCmd {
public:
    // Constructor/destructor
    FrRefreshWidgetsInfoCmd();

	virtual bool Execute();

    // Undo/redo section
	virtual bool CanUndo();
	virtual bool Undo();
	virtual bool Redo();

    // Properties
    FrPropMacro(FrMainController*, MainController);

protected:
    void UpdatePipelineForID(int id, int point);
};

#endif // FR_REFRESH_WIDGETS_INFO_CMD
