#ifndef FR_GRAPHTABDOC_OBJ
#define FR_GRAPHTABDOC_OBJ

// Includes
#include "FrMacro.h"
#include "FrDocumentObj.h"
#include "FrGraphDocObj.h"
#include "FrPointsDocObj.h"

#include <vector>

#define DEF_TBS_NAME    QString("Tab")
#define DEF_TBS_DESCR   QString("Description")
#define DEF_TBS_CURRENT false

// This class represents tab settings graphs.
// It contains data that application may use construct 
// new graphs.
class FrGraphTabDocObj : public FrDocumentObj {
public:
    typedef std::vector<FrGraphDocObj*> GraphsCollection;
public:
    FrGraphTabDocObj(bool isDefault);
    virtual ~FrGraphTabDocObj();

    virtual void OnAdd(FrDocument* doc);
    virtual void OnRemove(FrDocument* doc);
    virtual ObjTypes GetType();

    void AddGraph(FrGraphDocObj* graph);
    void ClearGraphs();
    void SetPoints(FrPointsDocObj* doc);

    // Properties
    FrPropMacro(unsigned long, ID);
    FrGetPropMacro(FrPointsDocObj*, Points);
    FrGetRefPropMacro(GraphsCollection, Graphs);

    FrPropMacro(QString, Name);
    //FrPropMacro(QString, Description);
    FrPropMacro(bool, IsCurrent);
    FrGetPropMacro(bool, IsDefault)    

};

#endif

