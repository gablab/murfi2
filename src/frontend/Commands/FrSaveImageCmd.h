#ifndef FR_SAVEIMAGE_CMD
#define FR_SAVEIMAGE_CMD

class FrTabSettingsDocObj;
class FrImageDocObj;

#include "FrBaseCmd.h"
#include "FrMacro.h"

#include "Qt/qstring.h"

// This command allows user to load new images
class FrSaveImageCmd : public FrBaseCmd
{
public:
    enum Action { Undefined, SaveImage, SaveRoi, SaveImageAndRemove };

public:
    // Constructor/destructor
    FrSaveImageCmd();

    // Performe action
	virtual bool Execute();

    // Undo/redo section
	virtual bool CanUndo();
	virtual bool Undo();
	virtual bool Redo();

    // Properties 
    FrSetPropMacro(Action, Action);
    FrSetPropMacro(bool, SaveAsMode);

private:
    bool PerformeSaveImage(bool removeAfterSave = false);
    bool PerformeSaveRoi(bool removeAfterSave = false);

    QString GetFileName(bool forROI = false);
};

#endif
