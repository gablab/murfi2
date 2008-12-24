#ifndef FR_INTERACTOR_STYLE
#define FR_INTERACTOR_STYLE

// Forward declaration
class FrMainController;
class FrPanTool;

// Includes
#include <vtkInteractorStyle.h>

class FrInteractorStyle : public vtkInteractorStyle {
public:
    vtkTypeMacro(FrInteractorStyle,vtkInteractorStyle);
    static FrInteractorStyle* New();

public:
    // Setup main controller
    void SetMainController(FrMainController* ctrl);

    // Generic event overrides
    virtual void OnMouseMove();
    virtual void OnLeftButtonDown();
    virtual void OnLeftButtonUp();
    virtual void OnMiddleButtonDown();
    virtual void OnMiddleButtonUp();
    virtual void OnRightButtonDown();
    virtual void OnRightButtonUp();
    virtual void OnMouseWheelForward();
    virtual void OnMouseWheelBackward();

    virtual void OnKeyPress();

protected:
    // Declare tools as friends to get full access
    friend class FrTool;
    friend class FrPanTool;
    friend class FrZoomTool;
    friend class FrSliceScrollTool;
    friend class FrTBCTool;
    friend class FrManipulationTool;
    friend class FrRoiTool; // test
    friend class FrView2D;	// test

protected:
    FrInteractorStyle();
    virtual ~FrInteractorStyle();

private:
    enum State{
        Undefined, Left, Mid, Right
    };

private:
    enum State m_state;
    FrMainController* m_controller;

private:
    // VTK style
    FrInteractorStyle(const FrInteractorStyle&);  // Not implemented.
    void operator=(const FrInteractorStyle&);  // Not implemented.
};

#endif
