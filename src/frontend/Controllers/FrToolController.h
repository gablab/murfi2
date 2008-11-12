#ifndef FR_TOOL_CONTROLLER
#define FR_TOOL_CONTROLLER

#include <vector>
#include "FrController.h"

class FrTool;
class FrMainController;

class FrToolController : public FrController
{
public:
    FrToolController(FrController* controller=0L);
    ~FrToolController();

    // Delete all registered tools
    void ClearAll(bool doDelete = false);

    // Returns tool at the bottom of stack
    FrTool* GetCurrentTool();

    // Push tool into stack
    void PushTool(FrTool* tool);

    // Pop tools from stack
    FrTool* PopTool();

    // Override from base class
    virtual void Notify(int notifyCode);
    
private:
    // use it as stack
    std::vector<FrTool*> m_tools;
};

#endif
