#ifndef FR_MASKRECTANGLE_CMD
#define FR_MASKRECTANGLE_CMD

// Forward declaration

// Some includes 
#include "FrMaskBaseCmd.h"
#include "FrMacro.h"

#include <vector>

class vtkPointPicker;

// This class implements command that support 
// dilation and erosion for ROI mask data.
class FrMaskRectangleCmd : public FrMaskBaseCmd {
public:
    enum Action { Undefined, Draw, Write };
    typedef struct _rect{
        int leftX, leftY;
        int rightX, rightY;
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
    
    // Prrperties
    FrSetPropMacro(Action, Action);
    FrSetPropMacro(Rect, Rect);
    FrSetPropMacro(vtkPointPicker*, PointPicker);

private:
    // Helpers
    bool DrawMask();
    bool WriteMask();

private:

};

#endif // FR_MASKRECTANGLE_CMD
