#ifndef FR_MASKFREESHAPE_CMD
#define FR_MASKFREESHAPE_CMD

// Forward declaration

// Some includes 
#include "FrMaskBaseCmd.h"
#include "FrMacro.h"
#include "FrSelection.h"


// This class implements command that support 
// dilation and erosion for ROI mask data.
class FrMaskFreeShapeCmd : public FrMaskBaseCmd {
public:
    enum Action { Undefined, Draw, Write };
    typedef std::vector<Pos> Points;

public:
    // Constructor/destructor
    FrMaskFreeShapeCmd();
    virtual ~FrMaskFreeShapeCmd();

    virtual bool Execute();

    // Undo/redo section
    virtual bool CanUndo();
    virtual bool Undo();
    virtual bool Redo();

    // Properties
    FrSetPropMacro(Action, Action);
    FrSetPropMacro(Points, Points);
    FrSetPropMacro(int, ImageNumber);

private:
    // Helpers
    bool DrawMask();
    bool WriteMask();

private:
};

#endif // FR_MASKFREESHAPE_CMD
