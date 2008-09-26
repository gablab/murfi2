#ifndef FR_TOOL
#define FR_TOOL

#include "FrMacro.h"

class FrToolController;
class FrInteractorStyle;

// Params of mouse event
typedef struct _frMouseParams {
public:
    enum MouseButton {
        NoneButton, LeftButton, RightButton, MidButton
    };

public:
    int  X, Y, Delta;
    bool IsShift, IsControl;
    MouseButton Button;

    _frMouseParams() 
        : X(0), Y(0), Delta(0),
          IsShift(false), IsControl(false), 
          Button(NoneButton){ 
    }
} FrMouseParams;

// abstract class for all tools
// used by application
class FrTool {
public:
	virtual void Start() = 0;
	virtual void Stop() = 0;
    virtual bool OnMouseUp(FrInteractorStyle* is, FrMouseParams& params) = 0;
	virtual bool OnMouseDown(FrInteractorStyle* is, FrMouseParams& params) = 0;
	virtual bool OnMouseMove(FrInteractorStyle* is, FrMouseParams& params) = 0;
	virtual bool OnMouseDrag(FrInteractorStyle* is, FrMouseParams& params) = 0;

public:
	/// Default constructor
	FrTool();
	/// Destructor
	virtual ~FrTool();

    // Controller that's owning tool
    FrPropMacro(FrToolController*,Controller);

protected:
    bool m_isStarted;

private:
	/// Controller owning the tool.
	FrToolController* m_controller;
};

#endif
