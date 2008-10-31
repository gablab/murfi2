#ifndef FR_CHANGESLICE_CMD
#define FR_CHANGESLICE_CMD

#include "FrBaseCmd.h"

// This command changes current slice number
// it responsible for support of single slice view
// as well as orthogonal slice view.
class FrChangeSliceCmd : public FrBaseCmd
{
public:
    // Constructor/destructor
    FrChangeSliceCmd();
    
    // Modifiers
    void SetSliceDelta(double value);
    void SetMouseXY(int x, int y);

    // Overrides
	virtual bool Execute();

    // Undo/redo section
	virtual bool CanUndo();
	virtual bool Undo();
	virtual bool Redo();

private:
    bool ChangeOrthoViewSliceNums();
    int GetCoronalSlice(int x, int y, int imgNum);
	int GetSagitalSlice(int x, int y, int imgNum);
	int GetAxialSlice(int x, int y, int imgNum);

private:
    // Params for simple views 
    bool m_isSlice;
    double m_Slice;

    // Params for ortho view
    bool m_isXY;
    int m_X;
    int m_Y;
};

#endif // FR_CHANGESLICE_CMD
