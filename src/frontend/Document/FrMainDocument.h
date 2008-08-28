#ifndef FR_MAIN_DOCUMENT
#define FR_MAIN_DOCUMENT

#include <vector>
#include "FrDocument.h"

class FrMainDocument: public FrDocument {
public:
    FrMainDocument();
    virtual ~FrMainDocument();

    void GetAllImages(std::vector<FrDocumentObj*>& images);
};

#endif
