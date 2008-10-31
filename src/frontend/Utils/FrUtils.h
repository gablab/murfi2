#ifndef FR_UTILS
#define FR_UTILS

#include "FrSettings.h"

#define MY_PI 3.1415926854

// This function is used to ensure that
// value is in specified range [min..max].
template<class T>
T ClampValue(T value, T min, T max){
    if(value < min) value = min;
    else if(value > max) value = max;
    return value;
}

float Frange(float a, float minv, float maxv);
int  Irange(int a, int minv, int maxv);

// This functions helps to get layer settings
void GetLayerSettings(FrSliceViewSettings*  sets, LayerCollection& layers);
void GetLayerSettings(FrMosaicViewSettings* sets, LayerCollection& layers);
void GetLayerSettings(FrOrthoViewSettings*  sets, LayerCollection& layers, int viewID);

#endif
