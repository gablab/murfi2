#ifndef FR_SLICESCROLL_CMD
#define FR_SLICESCROLL_CMD

// Forward declarations
class FrViewDocObj;

// Includes
#include "FrBaseCmd.h"

// This command changes current slice number
// it responsible for support of single slice view
// as well as orthogonal slice view.
class FrSliceScrollCmd : public FrBaseCmd
{
public:
    // Constructor/destructor
    FrSliceScrollCmd();
    
    // Modifiers
    void SetSliceDelta(double value);
    void SetChangingSlice(int sliceIndex);
    void SetMouseXY(int x, int y);

    // Overrides
    virtual bool Execute();
    
    // Undo/redo section
    virtual bool CanUndo();
    virtual bool Undo();
    virtual bool Redo();

    int GetOrthoViewSliceNum(int x, int y);
    void GetSelectedVoxel(int result[3]);

private:
    // Params for simple views 
    bool m_isSlice;
    double m_Slice;

    // Params for ortho view
    bool m_isXY;
    int m_X;
    int m_Y;
    int m_changingSlice;
};

#endif
