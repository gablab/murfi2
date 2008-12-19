#ifndef FR_LOADIMAGE_CMD
#define FR_LOADIMAGE_CMD

class FrTabSettingsDocObj;
class FrImageDocObj;
class RtMRIImage;

#include "FrBaseCmd.h"
#include "FrMacro.h"

#include "Qt/qstring.h"
#include <vector>

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
	
    // Init Method
    void AddFileToOpen(QString& fileName){
        m_FilesToOpen.push_back(fileName);
    }

private:
    // helpers
    RtMRIImage* LoadMRIImageFromFile(QString& fileName);

private:
    std::vector<QString> m_FilesToOpen;
};

#endif
