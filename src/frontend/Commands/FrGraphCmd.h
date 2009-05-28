#ifndef FR_GRAPH_CMD
#define FR_GRAPH_CMD

class FrGraphDocObj;
class FrGraphPaneDocObj;

#include "FrBaseCmd.h"
#include "FrMacro.h"

// This command allows user to add, remove 
// and modify settings of the layer
class FrGraphCmd : public FrBaseCmd {
public:
    typedef enum _Action { 
        Undefined, AddGraph, AddGraphWidget, DeleteGraph, DeleteGraphWidget 
    } Actions;

public:
    // Constructor/destructor
    FrGraphCmd();

    virtual bool Execute();

    // Undo/redo section
    virtual bool CanUndo();
    virtual bool Undo();
    virtual bool Redo();

    // Properties
    FrSetPropMacro(Actions, Action);    
    FrSetPropMacro(FrGraphPaneDocObj*, DocObj);
    void SetID(unsigned long id);

private:
    // Helper methods here
    bool addGraph();
    bool deleteGraph();
    bool addGraphWidget();
    bool deleteGraphWidget();

    unsigned long GetActiveLayerID();
    bool IsRoiLayer(unsigned long id);

private:
    bool m_isID;
    unsigned long m_ID;
};

#endif
