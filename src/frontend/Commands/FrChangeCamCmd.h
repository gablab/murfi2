#ifndef FR_CHANGECAM_CMD
#define FR_CHANGECAM_CMD

#include "FrBaseCmd.h"

// This command changes settings of camera
// of current view and then update view.
class FrChangeCamCmd : public FrBaseCmd
{
public:
    // Constructor/destructor
    FrChangeCamCmd();
    
	virtual bool Execute();

    // Undo/redo section
	virtual bool CanUndo();
	virtual bool Undo();
	virtual bool Redo();

private:
    bool m_isPosition;
    bool m_isFocalPoint;
    bool m_isViewUp;
    bool m_isScale;
};

#endif // FR_CHANGECAM_CMD