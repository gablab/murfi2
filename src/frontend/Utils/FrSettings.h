#ifndef FR_SETTINGS
#define FR_SETTINGS

#include "FrMacro.h"
#include "Qt/QColor.h"

// STL stuff
#include <vector>
#include <algorithm>

// Settings of camera
// Some defines
#define  DEF_CAM_SCALE      200.0
#define  DEF_CAM_POSITION   0.0, 0.0, 1.0
#define  DEF_CAM_FOCALPOINT 0.0, 0.0, 0.0
#define  DEF_CAM_VIEWUP     0.0, 1.0, 0.0

class FrCameraSettings {
public:
    double Position[3];
    double FocalPoint[3];
    double ViewUp[3];
    double Scale;
};

// Settings of colormap filter 
// Some defines
#define DEF_CM_MIN_VALUE  0
#define DEF_CM_MAX_VALUE  255
#define DEF_CM_THRESHOLD  128
#define DEF_CM_COLOR      0,0,255,255

class FrColormapSettings {
public:
    enum Type { SingleColor, MultiColor };
public:
    Type    Type;
    int     MinValue;
	int     MaxValue;
    int     Threshold;
	QColor  Color;
};

#define DEF_CM_TYPE    FrColormapSettings::SingleColor

// Settings of TBC filter
// Some defines
#define DEF_TBC_THRESHOLD  0.0
#define DEF_TBC_BRIGHTNESS 0.0
#define DEF_TBC_CONTRAST   0.0

class FrTBCSettings {
public:
    double Threshold;
    double Brightness;
    double Contrast;
};

// Settings of layers
#define DEF_DEFLAYER_NAME "Default"
#define DEF_LAYER_OPACITY    1.0
#define DEF_LAYER_VISIBILITY true
#define DEFAULT_LAYER_ID  0
#define ALL_LAYERS_ID    -1
#define CUR_LAYER_ID     -2
#define BAD_LAYER_ID     -3

class FrLayerSettings{
public:
    QString Name;
	// IDs params 
    int     ID;
    // Values params
    FrColormapSettings ColormapSettings;
    FrTBCSettings      TbcSettings;
    double             Opacity;
    bool               Visibility;
};

// Single slice view definitions
// Some defines
#define DEF_SLICE_NUMBER 0
typedef std::vector<FrLayerSettings*> LayerCollection;
template <class T>
bool RemoveItemFromVector(std::vector<T*>& collection, 
                          T* item, bool isDelete=false){
    LayerCollection::iterator it;
    it = std::find(collection.begin(), collection.end(), item);
    if(it != collection.end()){
        collection.erase(it);
        if(isDelete) delete item;
    }
    return (it != collection.end());
}

class FrSliceViewSettings {
public:
    // Need destructor
    ~FrSliceViewSettings(){
        while(OtherLayers.size() > 0){
            RemoveItemFromVector(OtherLayers, (*OtherLayers.begin()), true);
        }
    }
public:
    int SliceNumber;
    int ActiveLayerID;
    FrCameraSettings CamSettings;
    FrLayerSettings  MainLayer;
    LayerCollection  OtherLayers;
    // Some other params
    bool ResetNeeded;
};

// Mosaic view definitions
class FrMosaicViewSettings {
    // Need destructor
    ~FrMosaicViewSettings(){
        while(OtherLayers.size() > 0){
            RemoveItemFromVector(OtherLayers, (*OtherLayers.begin()), true);
        }
    }
public:
    int ActiveLayerID;
    FrCameraSettings CamSettings;
    FrLayerSettings  MainLayer;
    LayerCollection  OtherLayers;
    // Some other params
    bool ResetNeeded;
};

// Ortho view definitions
#define ORTHO_VIEW_NUM 3
#define CORONAL_SLICE 0
#define SAGITAL_SLICE 1
#define AXIAL_SLICE 2

class FrOrthoViewSettings {
public:
    //Here we have to have constructor and destructor
    FrOrthoViewSettings(){
        for(int i=0; i < ORTHO_VIEW_NUM; ++i)
            OtherLayers[i] = LayerCollection();
    }
    ~FrOrthoViewSettings(){
        for(int i=0; i < ORTHO_VIEW_NUM; ++i)
            while(OtherLayers[i].size() > 0){
                RemoveItemFromVector(OtherLayers[i], (*OtherLayers[i].begin()), true);
            }
    }
public:    
    int CoronalSlice;
    int SagitalSlice;
    int AxialSlice;
    int ActiveLayerID;
    FrCameraSettings CamSettings[ORTHO_VIEW_NUM];
    FrLayerSettings  MainLayer[ORTHO_VIEW_NUM];
    LayerCollection  OtherLayers[ORTHO_VIEW_NUM];
    // Some other params
    bool ResetNeeded;
};

// Initialization stuff
inline void InitCameraDefault(FrCameraSettings* sets){
    INIT_ARR3(sets->Position,DEF_CAM_POSITION);
    INIT_ARR3(sets->FocalPoint,DEF_CAM_FOCALPOINT);
    INIT_ARR3(sets->ViewUp,DEF_CAM_VIEWUP);
    sets->Scale = DEF_CAM_SCALE;
}

inline void InitColormapDefault(FrColormapSettings* sets){
    sets->Color     = QColor(DEF_CM_COLOR);
    sets->MaxValue  = DEF_CM_MAX_VALUE;
    sets->MinValue  = DEF_CM_MIN_VALUE;
    sets->Threshold = DEF_CM_THRESHOLD;
    sets->Type      = DEF_CM_TYPE;
}

inline void InitTbcDefault(FrTBCSettings* sets){
    sets->Threshold  = DEF_TBC_THRESHOLD;
    sets->Brightness = DEF_TBC_BRIGHTNESS;
    sets->Contrast   = DEF_TBC_CONTRAST;
}

inline void InitLayerDefault(FrLayerSettings* sets){
    sets->ID = BAD_LAYER_ID;
    sets->Name = DEF_DEFLAYER_NAME;
    sets->Opacity = DEF_LAYER_OPACITY;
    sets->Visibility = DEF_LAYER_VISIBILITY;

    InitColormapDefault(&sets->ColormapSettings);
    InitTbcDefault(&sets->TbcSettings);
}

#endif
