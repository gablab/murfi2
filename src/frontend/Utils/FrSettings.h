#ifndef FR_SETTINGS
#define FR_SETTINGS

#include "FrMacro.h"
#include "Qt/qcolor.h"
#include "Qt/qstring.h"

///////////////////////////////////////////////////////////////////////
// Settings of colormap filter 
// Some defines
#define DEF_CM_MIN_VALUE  0
#define DEF_CM_MAX_VALUE  255
#define DEF_CM_MID_VALUE  128
#define DEF_CM_THRESHOLD  4
#define DEF_CM_COLOR      QColor(0,0,255,255)
#define DEF_CM_TYPE       FrColormapSettings::SingleColor

class FrColormapSettings {
public:
    typedef enum _Types { SingleColor, MultiColor } Types;
public:
    FrColormapSettings(){
        this->Color     = DEF_CM_COLOR;
        this->MaxValue  = DEF_CM_MAX_VALUE;
        this->MinValue  = DEF_CM_MIN_VALUE;
        this->MidValue  = DEF_CM_MID_VALUE;
        this->Threshold = DEF_CM_THRESHOLD;
        this->Type      = DEF_CM_TYPE;
    }  

public:
    Types   Type;
    int     MinValue;
    int     MaxValue;
    int     MidValue;
    int     Threshold;
    QColor  Color;
};

// Settings of TBC filter
// Some defines
#define DEF_TBC_THRESHOLD  0.0
#define DEF_TBC_BRIGHTNESS 0.0
#define DEF_TBC_CONTRAST   0.0

class FrTbcSettings {
public:
    FrTbcSettings(){
        this->Threshold  = DEF_TBC_THRESHOLD;
        this->Brightness = DEF_TBC_BRIGHTNESS;
        this->Contrast   = DEF_TBC_CONTRAST;
    }

public:
    double Threshold;
    double Brightness;
    double Contrast;
};

// Settings of camera
// Some defines
#define  DEF_CAM_SCALE      200.0
#define  DEF_CAM_POSITION   0.0, 0.0, 1.0
#define  DEF_CAM_FOCALPOINT 0.0, 0.0, 0.0
#define  DEF_CAM_VIEWUP     0.0, 1.0, 0.0

class FrCameraSettings {
public:
    FrCameraSettings(){
        INIT_ARR3(this->FocalPoint, DEF_CAM_FOCALPOINT);
        INIT_ARR3(this->Position, DEF_CAM_POSITION);
        INIT_ARR3(this->ViewUp, DEF_CAM_VIEWUP);
        this->Scale = DEF_CAM_SCALE;
    }

public:
    double Position[3];
    double FocalPoint[3];
    double ViewUp[3];
    double Scale;
};

#endif
