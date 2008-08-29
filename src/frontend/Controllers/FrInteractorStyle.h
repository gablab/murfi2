#ifndef FR_INTERACTOR_STYLE
#define FR_INTERACTOR_STYLE

// Forward declaration
class FrMainController;
class FrPanTool;

// Includes
#include <vtkInteractorStyle.h>

class FrInteractorStyle : public vtkInteractorStyle {
public:
    FrInteractorStyle(FrMainController* controller);
    virtual ~FrInteractorStyle();

public:
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

protected:
    // Declare tools as friends to get full access
    friend class FrTool;
    friend class FrPanTool;
    friend class FrZoomTool;
    friend class FrTBCTool;

private:
    enum State{
        Undefined, Left, Mid, Right
    };
private:
    enum State m_state;
    FrMainController* m_controller;
};

#endif