#ifndef FR_LOADTABS_CMD
#define FR_LOADTABS_CMD

// Forward declarations
class QDomDocument; 
class QDomElement;
class FrTabSettingsDocObj;

// Some includes
#include "FrMacro.h"
#include "FrBaseCmd.h"

#include "Qt/QString.h"

// This command can save all tabs into file.
class FrLoadTabsCmd : public FrBaseCmd {
public:
    // Constructor/destructor
    FrLoadTabsCmd();
    virtual ~FrLoadTabsCmd();

	virtual bool Execute();

    // Undo/redo section
	virtual bool CanUndo();
	virtual bool Undo();
	virtual bool Redo();

    // Properties
    FrSetPropMacro(QString, FileName);

private:
    bool LoadTabSettings(QDomElement& root, FrTabSettingsDocObj* tabs);
    void LoadSliceViewSettings(QDomElement& parent,  void* svSets);
    void LoadMosaicViewSettings(QDomElement& parent, void* mvSets);
    void LoadOrthoViewSettings(QDomElement& parent,  void* ovSets);
    void LoadCameraSettings(QDomElement& parent, void* camSets);
    void LoadLayeredImgSettings(QDomElement& parent, void* liSets);

private:
    QDomDocument* m_Document;
};

#endif // FR_LOADTABS_CMD