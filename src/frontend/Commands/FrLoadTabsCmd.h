#ifndef FR_LOADTABS_CMD
#define FR_LOADTABS_CMD

// Forward declarations
class QDomDocument; 
class QDomElement;
class FrTabSettingsDocObj;
class FrCameraSettings;
class FrSliceViewSettings;
class FrMosaicViewSettings;
class FrOrthoViewSettings;
class FrTBCSettings;
class FrLayerSettings;

// Some includes
#include "FrMacro.h"
#include "FrBaseCmd.h"

#include "Qt/qstring.h"
#include <vector>

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
    bool LoadTabSettings(QDomElement& elem, FrTabSettingsDocObj* tabs);
    bool ValidateTabSettings(FrTabSettingsDocObj* tabs);
    bool LoadSliceViewSettings(QDomElement& elem,  FrSliceViewSettings* svSets);
    bool LoadMosaicViewSettings(QDomElement& elem, FrMosaicViewSettings* mvSets);
    bool LoadOrthoViewSettings(QDomElement& elem,  FrOrthoViewSettings* ovSets);
    bool LoadCameraSettings(QDomElement& elem,     FrCameraSettings* camSets);
    bool LoadLayeredImageSettings(QDomElement& elem, 
                                  FrLayerSettings* mlSets, 
                                  std::vector<FrLayerSettings*>& olSets);

    bool LoadLayersSettings(QDomElement& elem, std::vector<FrLayerSettings*>& olSets);
    bool LoadLayerSettings(QDomElement& elem, FrLayerSettings* liSets);
    bool LoadTbcSettings(QDomElement& elem, FrTBCSettings* tbsSets);
    // vec should point to double[3]
    bool LoadAttrValuesXYZ(QDomElement& elem, double* vec);

private:
    QDomDocument* m_Document;
    typedef std::vector<FrTabSettingsDocObj*> TabSettingsVec;
    TabSettingsVec m_tabSets;
};

#endif // FR_LOADTABS_CMD
