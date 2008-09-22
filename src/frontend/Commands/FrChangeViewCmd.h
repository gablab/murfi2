#ifndef FR_CHANGEVIEW_CMD
#define FR_CHANGEVIEW_CMD

// Forward declaration
class FrBaseView;

#include "FrBaseCmd.h"

// This command allow you to change current view mode.
class FrChangeViewCmd : public FrBaseCmd 
{
public:
    enum View { Unknown, Slice, Mosaic, Ortho };

public:
    // Constructor/destructor
    FrChangeViewCmd();
    
	virtual bool Execute();

    // Undo/redo section
	virtual bool CanUndo();
	virtual bool Undo();
	virtual bool Redo();

    // Properties
    FrSetPropMacro(View, TargetView);

private:
    FrBaseView* GetTargetView();
};

#endif // FR_BASE_CMD