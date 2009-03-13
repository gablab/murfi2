#ifndef FR_LAYER_CMD
#define FR_LAYER_CMD

class FrLayerDocObj;

#include "FrBaseCmd.h"
#include "FrMacro.h"

// This command allows user to add, remove 
// and modify settings of the layer
class FrLayerCmd : public FrBaseCmd {
public:
    typedef enum _Action { 
        Undefined, Add, Delete, 
        ChangeOld, ChangeParams, 
        ChangeColormap, UpdateSelectedID 
    } Actions;

public:
    // Constructor/destructor
    FrLayerCmd();

    virtual bool Execute();

    // Undo/redo section
    virtual bool CanUndo();
    virtual bool Undo();
    virtual bool Redo();

    // Properties
    FrSetPropMacro(Actions, Action);    
    FrSetPropMacro(FrLayerDocObj*, DocObj);
    void SetID(unsigned long id);

private:
    // Helper methods here
    bool AddLayer();
    bool DeleteLayer();
    bool UpdateSelectedLayerID();

    bool ChangeLayerOld();
    bool ChangeLayerParams();
    bool ChangeLayerColormap();

    unsigned long GetActiveLayerID();
    bool IsRoiLayer(unsigned long id);

private:
    bool m_isID;
    unsigned long m_ID;
};

#endif
