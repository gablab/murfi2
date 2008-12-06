#ifndef FR_LOADIMAGE_CMD
#define FR_LOADIMAGE_CMD

class FrTabSettingsDocObj;
class FrImageDocObj;
class RtMRIImage;

#include "FrBaseCmd.h"
#include "FrMacro.h"

#include "Qt/qstring.h"

// This command allows user to load new images
class FrLoadImageCmd : public FrBaseCmd
{
public:
    // Constructor/destructor
    FrLoadImageCmd();

	virtual bool Execute();

    // Undo/redo section
	virtual bool CanUndo();
	virtual bool Undo();
	virtual bool Redo();
	
    // Properties
    FrSetPropMacro(QString, FileName);

private:
    // helpers
    RtMRIImage* LoadMRIImageFromFile(QString& fileName);

private:

};

#endif
