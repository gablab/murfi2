#ifndef FR_GRAPHTABDOC_OBJ
#define FR_GRAPHTABDOC_OBJ

// Includes
#include "FrMacro.h"
#include "FrDocumentObj.h"
#include "FrGraphDocObj.h"

#include <vector>

// This class represents tab settings graphs.
// It contains data that application may use construct 
// new graphs.
class FrGraphTabDocObj : public FrDocumentObj {
public:
    FrGraphTabDocObj();
    virtual ~FrGraphTabDocObj();

    virtual void OnAdd(FrDocument* doc);
    virtual void OnRemove(FrDocument* doc);
    virtual ObjTypes GetType();

    // Properties
    FrPropMacro(int, ID);
    //FrGetRefPropMacro(std::vector<FrGraphDocObj*>, Graphs);

    FrPropMacro(QString, Name);
    FrPropMacro(QString, Description);
    FrPropMacro(bool, IsCurrent);
    FrGetPropMacro(bool, IsDefault)    

};

#endif

