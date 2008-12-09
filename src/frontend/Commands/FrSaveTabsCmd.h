#ifndef FR_SAVETABS_CMD
#define FR_SAVETABS_CMD

// Forward declarations
class QDomDocument; 
class QDomElement;
class FrTabSettingsDocObj;
class FrCameraSettings;
class FrLayerSettings;
class FrImageLayerSettings;
class FrOrthoViewSettings;
class FrMosaicViewSettings;
class FrSliceViewSettings;
class FrTbcSettings;

// Some includes
#include "FrMacro.h"
#include "FrBaseCmd.h"

#include "Qt/qstring.h"
#include <vector>

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
    
    void SaveSliceViewSettings(QDomElement& parent,  FrSliceViewSettings* svSets);
    
    void SaveMosaicViewSettings(QDomElement& parent, FrMosaicViewSettings* mvSets);
    
    void SaveOrthoViewSettings(QDomElement& parent,  FrOrthoViewSettings* ovSets);
    
    void SaveCameraSettings(QDomElement& parent, int id, FrCameraSettings* camSets);

    void SaveLayeredImageSettings(QDomElement& parent, int id,
                                  FrImageLayerSettings* mlSets, 
                                  std::vector<FrLayerSettings*>& olSets);
    
    void SaveLayerSettings(QDomElement& parent, FrLayerSettings* liSets);
    
    void SaveTbcSettings(QDomElement& parent, FrTbcSettings* tbcSets);

private:
    QDomDocument* m_Document;
};

#endif // FR_SAVETABS_CMD
