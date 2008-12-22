#ifndef FR_GRAPH_SETTINGS
#define FR_GRAPH_SETTINGS

#include "FrMacro.h"
#include "Qt/qcolor.h"
#include "Qt/qstring.h"

// Forward declarations
class FrGraphSettings;
class FrIntencityGraphSettings;
class FrMovementsGraphSettings;
class FrStimulusGraphSettings;
class FrRoiMeanGraphSettings;
class FrRoiStdGraphSettings;

///////////////////////////////////////////////////////
// Base class representing Graph settigs.
// Contains common propeties such as Name, Color, DrawMethod and Visibility.
// Also contains factory method to make it easy to add new types of Graphs.
class FrGraphSettings {
public:
    // Type of graphs supported by 
    typedef enum _GraphTypes { GT_Intencity, GT_Movements, 
                               GT_Stimulus,  GT_RoiMean, GT_RoiStd } GraphTypes;
    // Supported draw methods
    typedef enum _DrawMethods { DM_Linear, DM_Smooth } DrawMethods;

public:
    // Factory method
    static FrGraphSettings* Create(GraphTypes type);

public:
    QString Name;
    QColor  Color;
    bool    Visibility;
    DrawMethods DrawMethod;

    // Methods
    virtual GraphTypes GetType()=0;
};

// Concreate class represent settings for voxel intencity
class FrIntencityGraphSettings : public FrGraphSettings {
public:    
    // Voxel coordinates
    int I, J, K;

    // Methods
    virtual GraphTypes GetType(){ return GT_Intencity; }
};

class FrMovementsGraphSettings : public FrGraphSettings {
public:
    // Methods
    virtual GraphTypes GetType(){ return GT_Movements; }
};

class FrStimulusGraphSettings : public FrGraphSettings {
public:
    // Methods
    virtual GraphTypes GetType(){ return GT_Stimulus; }
};

class FrRoiMeanGraphSettings : public FrGraphSettings {
public:
    // Methods
    virtual GraphTypes GetType(){ return GT_RoiMean; }
};

class FrRoiStdGraphSettings : public FrGraphSettings {
public:
    // Methods
    virtual GraphTypes GetType(){ return GT_RoiStd; }
};

// class for points for voxel selection tool
class FrPoint{
public:
    FrPoint(int px, int py, int pz, QColor pcolor){
        x = px; y = py; z = pz;
        color = pcolor;
    }
public:
    int x, y, z;
    QColor color;
};

// Implementaion of factory method
inline FrGraphSettings* FrGraphSettings::Create(GraphTypes type){
    switch(type){
        case GT_Intencity: 
            return new FrIntencityGraphSettings;
        case GT_Movements:
            return new FrMovementsGraphSettings;
        case GT_Stimulus:
            return new FrStimulusGraphSettings;
        case GT_RoiMean:
            return new FrRoiStdGraphSettings;
        case GT_RoiStd:
            return new FrRoiMeanGraphSettings;
    }
    // Unsupported type?
    return 0L;
}

#endif
