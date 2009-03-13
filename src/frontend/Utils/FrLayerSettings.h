#ifndef FR_LAYER_SETTINGS
#define FR_LAYER_SETTINGS

#include "FrMacro.h"
#include "FrSettings.h"

#include "RtDataID.h"

#include "Qt/qcolor.h"
#include "Qt/qstring.h"

/////////////////////////////////////////////////////////////
// Settings of layers
#define BAD_LAYER_ID 0
#define SPC_LAYER_ID 1
#define ALL_LAYER_ID 2

#define DEF_LAYER_OPACITY    1.0
#define DEF_LAYER_VISIBILITY true

// Base abstract class for all settings
class FrLayerSettings {
public:
  FrLayerSettings(const RtDataID &id, QString name){
    this->DataID = RtDataID(id);
    this->DataID.setTimePoint(DATAID_UNSET_VALUE);
      this->Name = name;
      this->Opacity = DEF_LAYER_OPACITY;
      this->Visibility = DEF_LAYER_VISIBILITY;
    }
    virtual ~FrLayerSettings(){}

    typedef enum _Types { LImage, LRoi } LTypes;
    virtual LTypes GetType() = 0;

    // Factory method
    static FrLayerSettings* Create(LTypes type, const RtDataID &id, QString name);
    static bool CopySettings(FrLayerSettings* src, FrLayerSettings* dst);

public:
    QString Name;
    double  Opacity;
    bool    Visibility;
    RtDataID DataID;
};

// Settings for MRI layer
class FrImageLayerSettings : public FrLayerSettings {
public:
 FrImageLayerSettings(const RtDataID &id, QString name) : FrLayerSettings(id, name) {}
    virtual ~FrImageLayerSettings(){}
    virtual LTypes GetType() {
        return LImage;
    }

public:
    FrTbcSettings TbcSettings;
    FrColormapSettings ColormapSettings;
};

// Settings for ROI layer
class FrRoiLayerSettings : public FrLayerSettings {
public:
 FrRoiLayerSettings(const RtDataID &id, QString name) : FrLayerSettings(id, name) {}
    virtual ~FrRoiLayerSettings(){}
    virtual LTypes GetType() {
        return LRoi;
    }
};


// Factory method implementation
inline FrLayerSettings* FrLayerSettings::Create(LTypes type, const RtDataID &id,  QString name){
    switch(type){
        case LImage: 
            return new FrImageLayerSettings(id, name);
        case LRoi:
            return new FrRoiLayerSettings(id, name);
    }
    // Unsupported type ???
    return 0L;
}

#endif
