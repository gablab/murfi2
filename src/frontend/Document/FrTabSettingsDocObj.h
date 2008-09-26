#ifndef FR_TABSETDOC_OBJ
#define FR_TABSETDOC_OBJ

// includes
#include "FrDocumentObj.h"
#include "FrMacro.h"

#include "Qt/QString.h"

// Type for threshold / brightness / contrast
typedef struct _tbcSettings {
    double Threshold;
    double Brightness;
    double Contrast;
} TBCSettings;

// Type for camera settings
typedef struct _camSettings {
    double Scale;
    double ViewUp[3];
    double Position[3];
    double FocalPoint[3];
} CameraSettings;

// Type for view settings.
// Contains almost everything...
typedef struct _viewSettings {
    int SliceNumber;
    TBCSettings TbcSetting;
    CameraSettings CamSettings;
} ViewSettings;

#define ORTHO_VIEW_NUM 3
typedef struct _orthoViewSettings {
    int CoronalSlice;
    int SagitalSlice;
    int AxialSlice;
    TBCSettings TbcSettings[ORTHO_VIEW_NUM];
    CameraSettings CamSettings[ORTHO_VIEW_NUM];
}OViewSettings;

typedef struct _activeView {
    enum View { Slice, Mosaic, Ortho, Unknown };
}ActiveView;

// One have to provide valid type system
// All object types have to be registered here.
class FrTabSettingsDocObj : public FrDocumentObj {
public:
    FrTabSettingsDocObj(bool isDefault = false);
    virtual ~FrTabSettingsDocObj();

    // Base class overrides
    virtual void OnAdd(FrDocument* doc);
    virtual void OnRemove(FrDocument* doc);
    virtual ObjType GetType();
    
    // Initialization
    void InitFrom(FrTabSettingsDocObj* docObj);

    // Properties
    FrPropMacro(int,ID);
    FrPropMacro(QString, Name);
    FrPropMacro(QString, Description);

    FrPropMacro(bool,IsCurrent);
    FrGetPropMacro(bool,IsDefault)
    FrPropMacro(ActiveView::View,ActiveView);

    FrGetRefPropMacro(ViewSettings,SliceViewSettings);
    FrGetRefPropMacro(ViewSettings,MosaicViewSettings);
    FrGetRefPropMacro(OViewSettings,OrthoViewSettings);
};

#endif
