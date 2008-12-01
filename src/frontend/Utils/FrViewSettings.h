#ifndef FR_VIEW_SETTINGS
#define FR_VIEW_SETTINGS

#include "FrMacro.h"
#include "FrLayerSettings.h"

#define DEF_SLICE_NUMBER 0

typedef enum _Views { 
    Undefined, SliceView, MosaicView, OrthoView 
} Views;

class FrSliceViewSettings {
public:
    FrSliceViewSettings(){
        this->SliceNumber = DEF_SLICE_NUMBER;
        this->ActiveLayerID = DEF_LAYER_ID;
    }

public:
    int SliceNumber;
    int ActiveLayerID;
    FrCameraSettings CamSettings;
};

// Mosaic view definitions
class FrMosaicViewSettings {
public:
    FrMosaicViewSettings(){
        this->ActiveLayerID = DEF_LAYER_ID;
    }

public:
    int ActiveLayerID;
    FrCameraSettings CamSettings;
};

// Ortho view definitions
#define ORTHO_VIEWS_CNT 3
#define CORONAL_SLICE  0
#define SAGITAL_SLICE  1
#define AXIAL_SLICE    2

class FrOrthoViewSettings {
public:
    FrOrthoViewSettings(){        
        this->ActiveLayerID = DEF_LAYER_ID;
        for(int i = 0; i < ORTHO_VIEWS_CNT; ++i){
            this->SliceNumber[i] = DEF_SLICE_NUMBER;
            this->CamSettings[i] = FrCameraSettings();
        }
    }

public:
    int ActiveLayerID;
    int SliceNumber[ORTHO_VIEWS_CNT];
    FrCameraSettings CamSettings[ORTHO_VIEWS_CNT];
};

#endif
