#ifndef FR_MAIN_DOCUMENT
#define FR_MAIN_DOCUMENT

// Forward declarations
class FrDocumentObj;
class FrTabSettingsDocObj;

// Some includes
#include "FrDocument.h"
#include <vector>

class FrMainDocument: public FrDocument {
public:
    FrMainDocument();
    virtual ~FrMainDocument();

    // Some helper methods
    void GetAllImages(std::vector<FrDocumentObj*>& images);
    FrTabSettingsDocObj* GetCurrentTabSettings();
};

#endif

