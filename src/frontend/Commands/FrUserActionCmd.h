#ifndef FR_USERACTION_CMD
#define FR_USERACTION_CMD

class FrLayerDocObj;

#include "FrBaseCmd.h"
#include "FrMacro.h"

// This command allows user to add, remove 
// and modify settings of the layer
class FrUserActionCmd : public FrBaseCmd {
public:
    typedef enum _Action { 
        Undefined, Add, Delete, ChangeSettings
    } Actions;

public:
    // Constructor/destructor
    FrUserActionCmd();

    virtual bool Execute();

    // Undo/redo section
    virtual bool CanUndo();
    virtual bool Undo();
    virtual bool Redo();

    // Properties
    FrSetPropMacro(Actions, Action);    

private:
    // helpers
    bool AddLayer();
    bool DeleteLayer();
    bool ChangeImageSettings();

    int GetActiveLayerID();
    //FrLayerDocObj* GetLayerDocObjByID(int id);
    bool IsRoiLayer(int id);

private:
    bool m_isID;
    int m_ID;

};

#endif
