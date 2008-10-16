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
    bool LoadSliceViewSettings(QDomElement& elem,  void* svSets);
    bool LoadMosaicViewSettings(QDomElement& elem, void* mvSets);
    bool LoadOrthoViewSettings(QDomElement& elem,  void* ovSets);
    bool LoadCameraSettings(QDomElement& elem,     void* camSets);
    bool LoadLayeredImgSettings(QDomElement& elem, void* liSets);

    // vec should point to double[3]
    bool LoadAttrValuesXYZ(QDomElement& elem, double* vec);

private:
    QDomDocument* m_Document;
    typedef std::vector<FrTabSettingsDocObj*> TabSettingsVec;
    TabSettingsVec m_tabSets;
};

#endif // FR_LOADTABS_CMD