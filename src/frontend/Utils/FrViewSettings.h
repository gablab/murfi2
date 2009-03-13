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
        this->m_ActiveLayerID = BAD_LAYER_ID;
    }

public:
    int SliceNumber;
    FrPropMacro(unsigned long, ActiveLayerID);
    FrCameraSettings CamSettings;
};

// Mosaic view definitions
class FrMosaicViewSettings {
public:
    FrMosaicViewSettings(){
        this->m_ActiveLayerID = BAD_LAYER_ID;
    }

public:
    FrPropMacro(unsigned long, ActiveLayerID);
    FrCameraSettings CamSettings;
};

// Ortho view definitions
#define ORTHO_VIEWS_CNT 3
#define DEF_CORONAL  0
#define DEF_SAGITAL  1
#define DEF_AXIAL    2

class FrOrthoViewSettings {
public:
    FrOrthoViewSettings(){        
        this->m_ActiveLayerID = BAD_LAYER_ID;
        for(int i = 0; i < ORTHO_VIEWS_CNT; ++i){
            this->SliceNumber[i] = DEF_SLICE_NUMBER;
            this->CamSettings[i] = FrCameraSettings();
        }
    }

public:
    unsigned long GetActiveLayerID() { return m_ActiveLayerID; }
    void SetActiveLayerID(unsigned long id) { m_ActiveLayerID = id; }
    //FrPropMacro(unsigned long, ActiveLayerID);
    int SliceNumber[ORTHO_VIEWS_CNT];
    FrCameraSettings CamSettings[ORTHO_VIEWS_CNT];
 protected:
    unsigned long m_ActiveLayerID;
};

#endif
