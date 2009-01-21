#ifndef FR_SAVE_GRAPHTABSETTINGS_CMD
#define FR_SAVE_GRAPHTABSETTINGS_CMD

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
class FrSaveGraphTabSettingsCmd : public FrBaseCmd {
public:
    // Constructor/destructor
    FrSaveGraphTabSettingsCmd();
    virtual ~FrSaveGraphTabSettingsCmd();

	virtual bool Execute();

    // Undo/redo section
	virtual bool CanUndo();
	virtual bool Undo();
	virtual bool Redo();

    // Properties
    FrSetPropMacro(QString, FileName);

private:
    bool SaveTabSettings(QDomElement& root, FrGraphTabDocObj* tabs);
    
    void SavePoints(QDomElement& parent, FrPointsDocObj* points);
    void SaveGraphs(QDomElement& parent, std::vector<FrGraphDocObj*> graphs);

private:
    QDomDocument* m_Document;

};

#endif // FR_SAVE_GRAPHTABSETTINGS_CMD
