#ifndef FR_DOCUMENT_OBJ
#define FR_DOCUMENT_OBJ

// Forward declarations
class FrDocument;

// One have to provide valid type system
// All object types have to be registered here.
class FrDocumentObj
{
public: // Object types
    enum ObjType{
        BaseObject,
        SomeType1,
        SomeType2 // etc
    };

public:
    FrDocumentObj();
    virtual ~FrDocumentObj();

    virtual void OnAdd(FrDocument* doc);
    virtual void OnRemove(FrDocument* doc);

    virtual ObjType GetType();
};

#endif
