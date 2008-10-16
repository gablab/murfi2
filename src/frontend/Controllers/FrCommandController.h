#ifndef FR_CMD_CONTROLLER
#define FR_CMD_CONTROLLER

class FrMainController;

#include "FrController.h"
#include "FrMacro.h"
#include "FrNotify.h"

// include all commands header file here
// Needed for FrCommandController::CreateCmd<T>() method.
#include "FrBaseCmd.h"
#include "FrChangeCamCmd.h"
#include "FrChangeSliceCmd.h"
#include "FrChangeTbcCmd.h"
#include "FrManageToolCmd.h"
#include "FrChangeViewCmd.h"
#include "FrDeleteTabSettingsCmd.h"
#include "FrMultiCmd.h"
#include "FrSaveTabSettingsCmd.h"
#include "FrUpdateTabsCmd.h"
#include "FrUpdatePipelineCmd.h"
#include "FrVoxelInfoCmd.h"
#include "FrResetImageCmd.h"
#include "FrLoadTabsCmd.h"
#include "FrSaveTabsCmd.h"
#include "FrLayerCmd.h"

// This class allow user to create 
// and manage commands. Organised like singletone;
class FrCommandController : public FrController {
public:
    //// TODO: Add some undo / redo methods
    //bool HasUndo();
    //bool HasRedo();
    //bool Undo();
    //bool Redo();

public:
    // static methods
    static FrCommandController* Instance(){
        if(!m_controller){
            m_controller = new FrCommandController();
        }
        return m_controller;
    }
    
    // Using this method user can create command.
    // User is responsible to delete the created commands
    // if he executes them not through Command controller.
    template <class T>  static T* CreateCmd(){ 
        T* cmd = new T();
        FrMainController* mc = m_controller->GetMainController();
        cmd->SetMainController(mc);
        return cmd; 
    }

private:
    // Helpers
    FrMainController* GetMainController();

private:
    FrCommandController();
    ~FrCommandController();

    static FrCommandController* m_controller;
};

#endif // FR_CMD_CONTROLLER