#ifndef FR_SAVETABS_CMD
#define FR_SAVETABS_CMD

// Forward declarations
class QDomDocument; 
class QDomElement;
class FrTabSettingsDocObj;

// Some includes
#include "FrMacro.h"
#include "FrBaseCmd.h"

#include "Qt/QString.h"

// This command can save all tabs into file.
class FrSaveTabsCmd : public FrBaseCmd {
public:
    // Constructor/destructor
    FrSaveTabsCmd();
    virtual ~FrSaveTabsCmd();

	virtual bool Execute();

    // Undo/redo section
	virtual bool CanUndo();
	virtual bool Undo();
	virtual bool Redo();

    // Properties
    FrSetPropMacro(QString, FileName);

private:
    bool SaveTabSettings(QDomElement& root, FrTabSettingsDocObj* tabs);
    void SaveSliceViewSettings(QDomElement& parent,  void* svSets);
    void SaveMosaicViewSettings(QDomElement& parent, void* mvSets);
    void SaveOrthoViewSettings(QDomElement& parent,  void* ovSets);
    void SaveCameraSettings(QDomElement& parent, void* camSets);
    void SaveLayeredImgSettings(QDomElement& parent, void* liSets);

private:
    QDomDocument* m_Document;
};

#endif // FR_SAVETABS_CMD