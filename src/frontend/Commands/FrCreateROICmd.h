#ifndef FR_CREATE_ROI_CMD
#define FR_CREATE_ROI_CMD

#include "FrBaseCmd.h"
#include "FrMacro.h"

class FrImageDocObj;

// This command allows user to manage ROI
class FrCreateROICmd : public FrBaseCmd {
public:
    // Constructor/destructor
    FrCreateROICmd();

    virtual bool Execute();

    // Undo/redo section
    virtual bool CanUndo();
    virtual bool Undo();
    virtual bool Redo();

    // Properties
    FrSetPropMacro(bool, CreateTest);

private:
    // Helpers
    bool CreateEmptyMask(FrImageDocObj* imgDO);
    bool ThresholdToMask(FrImageDocObj* imgDO, double threshold);
    bool LoadFromFile(FrImageDocObj* imgDO, QString filename);

};

#endif
