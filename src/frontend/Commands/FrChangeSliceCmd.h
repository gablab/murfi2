#ifndef FR_CHANGESLICE_CMD
#define FR_CHANGESLICE_CMD

#include "FrBaseCmd.h"

// This command changes settings of camera
// of current view and then update view.
class FrChangeSliceCmd : public FrBaseCmd
{
public:
    // Constructor/destructor
    FrChangeSliceCmd();
    
    // Modifiers
    void SetSliceDelta(double value);

    //
    // Overrides
    //
	virtual bool Execute();

    // Undo/redo section
	virtual bool CanUndo();
	virtual bool Undo();
	virtual bool Redo();

private:
    bool m_isSlice;
    double m_Slice;
};

#endif // FR_CHANGESLICE_CMD