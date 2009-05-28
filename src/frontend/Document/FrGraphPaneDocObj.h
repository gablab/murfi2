#ifndef FR_GRAPHPANEDOC_OBJ
#define FR_GRAPHPANEDOC_OBJ

// Includes
#include "FrMacro.h"
#include "FrDocumentObj.h"
#include "FrGraphDocObj.h"
#include "FrPointsDocObj.h"

#include <vector>


// This class represents control that contains graphs.
class FrGraphPaneDocObj : public FrDocumentObj {
public:
    typedef std::vector<FrGraphDocObj*> GraphsCollection;
public:
    FrGraphPaneDocObj();
    virtual ~FrGraphPaneDocObj();

    virtual void OnAdd(FrDocument* doc);
    virtual void OnRemove(FrDocument* doc);
    virtual ObjTypes GetType();

    void AddGraph(FrGraphDocObj* graph);
    void RemoveGraph(unsigned long id);
    void ClearGraphs();

    // Properties
    FrPropMacro(unsigned long, ID);
    FrGetRefPropMacro(GraphsCollection, Graphs);

    // methods
    FrGraphDocObj* GetGraphDocObjByTS(unsigned long id);

};

#endif