#ifndef FR_MASKRECTANGLE_CMD
#define FR_MASKRECTANGLE_CMD

// Forward declaration

// Some includes 
#include "FrMaskBaseCmd.h"
#include "FrMacro.h"
#include "FrSelection.h"

#include <vector>


// This class implements command that support 
// dilation and erosion for ROI mask data.
class FrMaskRectangleCmd : public FrMaskBaseCmd {
public:
    enum Action { Undefined, Draw, Write };
    typedef struct _rect{
        Pos firstPoint;
        Pos secondPoint;
    }Rect;

public:
    // Constructor/destructor
    FrMaskRectangleCmd();
    virtual ~FrMaskRectangleCmd();

    virtual bool Execute();

    // Undo/redo section
    virtual bool CanUndo();
    virtual bool Undo();
    virtual bool Redo();
    
    // Properties
    FrSetPropMacro(Action, Action);
    FrSetPropMacro(Rect, Rect);

private:
    // Helpers
    bool DrawMask();
    bool WriteMask();

private:

};

#endif // FR_MASKRECTANGLE_CMD
