#ifndef FR_USERACTION_CMD
#define FR_USERACTION_CMD

class FrLayerDocObj;
class FrGraphDocObj;

#include "FrBaseCmd.h"
#include "FrMacro.h"
#include "RtDataID.h"

// This command allows user to add, remove 
// and modify settings of the layer
class FrUserActionCmd : public FrBaseCmd {
public:
    typedef enum _Action { 
        Undefined, AddLayer, DeleteLayer, AddGraph, AddGraphWidget, 
        DeleteGraph, DeleteGraphWidget, ChangeSettings
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
    FrSetPropMacro(unsigned long, TimeSeria);
    FrSetPropMacro(unsigned long, GraphWidgetID);

private:
    // helpers
    bool addLayer();
    bool deleteLayer();
    bool addGraph();
    bool addGraphWidget();
    bool deleteGraph();
    bool deleteGraphWidget();
    bool ChangeImageSettings();
    bool changeLayerPosition();

    FrGraphDocObj* CreateIntensityGraphDocObj();

    unsigned long GetActiveLayerID();
    //FrLayerDocObj* GetLayerDocObjByID(unsigned long id);
    bool IsRoiLayer(unsigned long id);

private:
    bool m_isID;
    unsigned long m_ID;

};

#endif
