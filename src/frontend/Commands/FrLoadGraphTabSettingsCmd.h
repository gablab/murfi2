#ifndef FR_LOADGRAPHTABS_CMD
#define FR_LOADGRAPHTABS_CMD

// Forward declarations
class QDomDocument; 
class QDomElement;
class FrGraphTabDocObj;
class FrPointsDocObj;
class FrGraphDocObj;

// Some includes
#include "FrMacro.h"
#include "FrBaseCmd.h"

#include "Qt/qstring.h"
#include <vector>

// This command can save all tabs into file.
class FrLoadGraphTabSettingsCmd : public FrBaseCmd {
public:
    // Constructor/destructor
    FrLoadGraphTabSettingsCmd();
    virtual ~FrLoadGraphTabSettingsCmd();

	virtual bool Execute();

    // Undo/redo section
	virtual bool CanUndo();
	virtual bool Undo();
	virtual bool Redo();

    // Properties
    FrSetPropMacro(QString, FileName);

private:
    bool LoadTabSettings(QDomElement& elem, FrGraphTabDocObj* tabs);
    bool LoadPoints(QDomElement& elem, FrPointsDocObj* pointsDO);
    bool LoadGraphs(QDomElement& elem, std::vector<FrGraphDocObj*>& graphs);

    // vec should point to double[3]
    bool LoadAttrValuesXYZ(QDomElement& elem, double* vec);

private:
    QDomDocument* m_Document;
    typedef std::vector<FrGraphTabDocObj*> TabSettingsVec;
    TabSettingsVec m_tabSets;

};

#endif // FR_LOADGRAPHTABS_CMD
