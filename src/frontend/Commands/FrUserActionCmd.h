#ifndef FR_USERACTION_CMD
#define FR_USERACTION_CMD

class FrLayerDocObj;

#include "FrBaseCmd.h"
#include "FrMacro.h"
#include "RtDataID.h"

// This command allows user to add, remove 
// and modify settings of the layer
class FrUserActionCmd : public FrBaseCmd {
public:
    typedef enum _Action { 
        Undefined, AddLayer, DeleteLayer, AddGraph, DeleteGraph, ChangeSettings
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
    FrSetPropMacro(RtDataID, GraphID);

private:
    // helpers
    bool addLayer();
    bool deleteLayer();
    bool addGraph();
    bool deleteGraph();
    bool ChangeImageSettings();
    bool changeLayerPosition();

    unsigned long GetActiveLayerID();
    //FrLayerDocObj* GetLayerDocObjByID(unsigned long id);
    bool IsRoiLayer(unsigned long id);

private:
    bool m_isID;
    unsigned long m_ID;

};

#endif
