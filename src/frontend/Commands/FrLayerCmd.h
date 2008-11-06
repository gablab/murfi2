#ifndef FR_IMGLAYER_CMD
#define FR_IMGLAYER_CMD

class FrTabSettingsDocObj;
class FrLayerSettings;

#include "FrBaseCmd.h"
#include "FrMacro.h"

// This command allows user to add, remove 
// and modify settings of the layer
class FrImageLayerCmd : public FrBaseCmd {
public:
    enum Action { Undefined, Add, Delete, 
        ChangeOld, ChangeParams, ChangeColormap, 
        UpdateSelectedID };

public:
    // Constructor/destructor
    FrImageLayerCmd();

    virtual bool Execute();

    // Undo/redo section
    virtual bool CanUndo();
    virtual bool Undo();
    virtual bool Redo();

    // Properties
    FrSetPropMacro(Action, Action);
    void SetID(int id);

private:
    // Helper methods here
    bool AddLayer();
    bool DeleteLayer();
    bool UpdateSelectedLayerID();

    bool ChangeLayerOld();
    bool ChangeLayerParams();
    bool ChangeLayerColormap();

    int GetActiveLayerID();

private:
    bool m_isID;
    int m_ID;
};

#endif
