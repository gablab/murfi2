#ifndef FR_GRAPHDOC_OBJ
#define FR_GRAPHDOC_OBJ

// Includes
#include "FrMacro.h"
#include "FrDocumentObj.h"
#include "FrGraphSettings.h"

// This class represents graph data object
// It contains data that application may use for plotting graphs.
class FrGraphDocObj : public FrDocumentObj {
public:
    FrGraphDocObj(FrGraphSettings::GraphTypes type);
    virtual ~FrGraphDocObj();

    // Overrides
    virtual void OnAdd(FrDocument* doc);
    virtual void OnRemove(FrDocument* doc);
    virtual ObjTypes GetType();

    // Properties
    FrPropMacro(FrGraphSettings*, Settings);
    FrPropMacro(int, ID);

    // Methods
    void SetGraphData();
    void GetGraphData();
};

#endif // FR_GRAPHDOC_OBJ

