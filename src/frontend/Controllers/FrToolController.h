#ifndef FR_TOOL_CONTROLLER
#define FR_TOOL_CONTROLLER

#include <vector>

class FrTool;

class FrToolController
{
public:
    FrToolController();
    ~FrToolController();

    // Delete all registered tools
    void ClearAll(bool doDelete = false);

    // Returns tool at the bottom of stack
    FrTool* GetCurrentTool();

    // Push tool into stack
    void PushTool(FrTool* tool);

    // Pop tools from stack
    FrTool* PopTool();
    
private:
    // use it as stack
    std::vector<FrTool*> m_tools;
};

#endif