#ifndef FR_LAYER_SETTINGS
#define FR_LAYER_SETTINGS

#include "FrMacro.h"
#include "FrSettings.h"

#include "Qt/qcolor.h"
#include "Qt/qstring.h"

/////////////////////////////////////////////////////////////
// Settings of layers
#define BAD_LAYER_ID 0
#define DEF_LAYER_ID 1
#define SPC_LAYER_ID 2
#define ALL_LAYER_ID 3

#define DEF_LAYER_NAME "Default"
#define DEF_LAYER_OPACITY    1.0
#define DEF_LAYER_VISIBILITY true

// Base abstract class for all settings
class FrLayerSettings {
public:
    FrLayerSettings(){
        this->Name = DEF_LAYER_NAME;
        this->Opacity = DEF_LAYER_OPACITY;
        this->Visibility = DEF_LAYER_VISIBILITY;
    }
    virtual ~FrLayerSettings(){}

    typedef enum _Types { LImage, LColormap, LRoi } LTypes;
    virtual LTypes GetType() = 0;

    // Factory method
    static FrLayerSettings* Create(LTypes type);
    static bool CopySettings(FrLayerSettings* src, FrLayerSettings* dst);

public:
    QString Name;
    double  Opacity;
    bool    Visibility;
};

// Settings for MRI layer
class FrImageLayerSettings : public FrLayerSettings {
public:
    FrImageLayerSettings(){}
    virtual ~FrImageLayerSettings(){}
    virtual LTypes GetType() {
        return LImage;
    }

public:
    FrTbcSettings TbcSettings;
};

// Settings for colormap layer
class FrColormapLayerSettings : public FrLayerSettings {
public:
    FrColormapLayerSettings(){}
    virtual ~FrColormapLayerSettings(){}
    virtual LTypes GetType() {
        return LColormap;
    }

public:
    FrTbcSettings      TbcSettings;
    FrColormapSettings ColormapSettings;
};

// Settings for ROI layer
class FrRoiLayerSettings : public FrLayerSettings {
public:
    FrRoiLayerSettings(){}
    virtual ~FrRoiLayerSettings(){}
    virtual LTypes GetType() {
        return LRoi;
    }
};


// Factory method implementation
inline FrLayerSettings* FrLayerSettings::Create(LTypes type){
    switch(type){
        case LImage: 
            return new FrImageLayerSettings();
        case LColormap:
            return new FrColormapLayerSettings();
        case LRoi:
            return new FrRoiLayerSettings();
    }
    // Unsupported type ???
    return 0L;
}

#endif
